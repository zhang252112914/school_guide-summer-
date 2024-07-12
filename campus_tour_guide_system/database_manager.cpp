#include "database_manager.h"

DatabaseManager::DatabaseManager(const QString &config_file_path,
                                 QObject *parent)
    : QObject{parent} {
  db = QSqlDatabase::addDatabase("QMYSQL");
  QSettings settings(config_file_path, QSettings::IniFormat);
  QString hostname = settings.value("Database/Hostname").toString();
  QString dbname = settings.value("Database/DatabaseName").toString();
  QString username = settings.value("Database/Username").toString();
  QString password = settings.value("Database/Password").toString();

  qDebug() << hostname << dbname << username << password;

  db.setHostName(hostname);
  db.setDatabaseName(dbname);
  db.setUserName(username);
  db.setPassword(password);

  // Try to open database
  if (!db.open()) {
    qDebug() << "Error: connection with database fail" << db.lastError().text();
  } else {
    qDebug() << "Database: connection ok";
  }

  // Create Table if not exists
  QSqlQuery query;
  bool success = query.exec(
      "CREATE TABLE IF NOT EXISTS nodes ("
      "id INT AUTO_INCREMENT PRIMARY KEY, "
      "pos_x DOUBLE NOT NULL, "
      "pos_y DOUBLE NOT NULL, "
      "info_id INT)");
  if (!success) {
    qDebug() << "Failed to create table:" << query.lastError().text();
  }

  success = query.exec(
      "CREATE TABLE IF NOT EXISTS edges ("
      "id INT AUTO_INCREMENT PRIMARY KEY, "
      "end_node_one_id INT NOT NULL, "
      "end_node_two_id INT NOT NULL, "
      "length DOUBLE NOT NULL)");
  if (!success) {
    qDebug() << "Failed to create table:" << query.lastError().text();
  }

  success = query.exec(
      "CREATE TABLE IF NOT EXISTS infos ("
      "id INT AUTO_INCREMENT PRIMARY KEY, "
      "name VARCHAR(255) NOT NULL, "
      "description TEXT NOT NULL, "
      "pic_path VARCHAR(255) NOT NULL)");
  if (!success) {
    qDebug() << "Failed to create table:" << query.lastError().text();
  }
}

DatabaseManager::~DatabaseManager() { db.close(); }

bool DatabaseManager::DatabaseManager::IsOpen() const { return db.isOpen(); }

// Read data from database and translate its type to node, edge and info
void DatabaseManager::DeserializeNodes() {
  if (!db.isOpen()) {
    qDebug() << "Error: Database is not open";
    return;
  }

  QSqlQuery query(db);
  if (!query.prepare("SELECT id, pos_x, pos_y, info_id FROM nodes")) {
    qDebug() << "Query preparation failed:" << query.lastError().text();
    return;
  }

  if (!query.exec()) {
    qDebug() << "Query execution failed:" << query.lastError().text();
    return;
  }

  while (query.next()) {
    Node node;
    node.id = query.value(0).toInt();
    node.pos_x = query.value(1).toDouble();
    node.pos_y = query.value(2).toDouble();
    QVariant info_id = query.value(3);
    if (!info_id.isNull()) {
      node.info_id = info_id.toInt();
      node.info_valid = true;
    }
    emit NodeLoaded(node);  // emit signal
  }
}

void DatabaseManager::DeserializeEdges() {
  if (!db.isOpen()) {
    qDebug() << "Error: Database is not open";
    return;
  }

  QSqlQuery query(db);
  if (!query.prepare(
          "SELECT id, end_node_one_id, end_node_two_id, length FROM edges")) {
    qDebug() << "Query preparation failed:" << query.lastError().text();
    return;
  }

  if (!query.exec()) {
    qDebug() << "Query execution failed:" << query.lastError().text();
    return;
  }

  while (query.next()) {
    Edge edge;
    edge.id = query.value(0).toInt();
    edge.end_node_one_id = query.value(1).toInt();
    edge.end_node_two_id = query.value(2).toInt();
    edge.length = query.value(3).toDouble();
    emit EdgeLoaded(edge);
  }
}

void DatabaseManager::DeserializeInfos() {
  if (!db.isOpen()) {
    qDebug() << "Error: Database is not open";
    return;
  }

  QSqlQuery query(db);
  if (!query.prepare("SELECT id, name, description, pic_path FROM infos")) {
    qDebug() << "Query preparation failed:" << query.lastError().text();
    return;
  }

  if (!query.exec()) {
    qDebug() << "Query execution failed:" << query.lastError().text();
    return;
  }

  while (query.next()) {
    Info info;
    info.id = query.value(0).toInt();
    info.name = query.value(1).toString();
    info.description = query.value(2).toString();
    info.pic_path = query.value(3).toString();

    emit InfoLoaded(info);
  }
}

// Get new data from CampusMap object and store it in database
void DatabaseManager::SerializeNodeSlot(const Node &node) {
  if (!db.isOpen()) {
    qDebug() << "Error: Database is not open";
    return;
  }

  QSqlQuery query;
  query.prepare(
      "INSERT INTO nodes (id, pos_x, pos_y, info_id) VALUES (:id, :pos_x, "
      ":pos_y, :info_id)");
  query.bindValue(":id", node.id);
  query.bindValue(":pos_x", node.pos_x);
  query.bindValue(":pos_y", node.pos_y);
  query.bindValue(":info_id", node.info_valid ? QVariant(node.info_id)
                                              : QVariant(QVariant::Int));

  if (!query.exec()) {
    qDebug() << "Error inserting data:" << query.lastError().text();
  } else {
    qDebug() << "Data inserted successfully!";
  }
}

void DatabaseManager::SerializeEdgeSlot(const Edge &edge) {
  if (!db.isOpen()) {
    qDebug() << "Error: Database is not open";
    return;
  }

  QSqlQuery query;
  query.prepare(
      "INSERT INTO edges (id, end_node_one_id, end_node_two_id, length) VALUES "
      "(:id, :end_node_one_id, :end_node_two_id, :length)");
  query.bindValue(":id", edge.id);
  query.bindValue(":end_node_one_id", edge.end_node_one_id);
  query.bindValue(":end_node_two_id", edge.end_node_two_id);
  query.bindValue(":length", edge.length);

  if (!query.exec()) {
    qDebug() << "Error inserting data:" << query.lastError().text();
  } else {
    qDebug() << "Data inserted successfully!";
  }
}

void DatabaseManager::SerializeInfoSlot(const Info &info) {
  if (!db.isOpen()) {
    qDebug() << "Error: Database is not open";
    return;
  }

  QSqlQuery query;
  query.prepare(
      "INSERT INTO infos (id, name, description, pic_path) VALUES (:id, :name, "
      ":description, :pic_path)");
  query.bindValue(":id", info.id);
  query.bindValue(":name", info.name);
  query.bindValue(":description", info.description);
  query.bindValue(":pic_path", info.pic_path);

  if (!query.exec()) {
    qDebug() << "Error inserting data:" << query.lastError().text();
  } else {
    qDebug() << "Data inserted successfully!";
  }
}

void DatabaseManager::UpdateInfoSlot(const Info &info) {
  if (!db.isOpen()) {
    qDebug() << "Error: Database is not open";
    return;
  }

  QSqlQuery query;
  query.prepare(
      "UPDATE infos SET name = :name, description = :description, "
      "pic_path = :pic_path WHERE id = :id");
  query.bindValue(":id", info.id);
  query.bindValue(":name", info.name);
  query.bindValue(":description", info.description);
  query.bindValue(":pic_path", info.pic_path);

  if (!query.exec()) {
    qDebug() << "Update failed: " << query.lastError();
  } else {
    qDebug() << "Update succeeded";
  }
}

void DatabaseManager::DeleteInfoSlot(int id) {
  if (!db.isOpen()) {
    qDebug() << "Error: Database is not open";
    return;
  }

  if (!db.transaction()) {
    qDebug() << "Error: Unable to start transaction";
    return;
  }

  QSqlQuery nodes_query;
  nodes_query.prepare(
      "UPDATE nodes SET info_id = NULL WHERE info_id = :info_id");
  nodes_query.bindValue(":info_id", id);

  if (!nodes_query.exec()) {
    qDebug() << "Update nodes failed: " << nodes_query.lastError();
    db.rollback();
    return;
  }

  QSqlQuery infos_query;
  infos_query.prepare("DELETE FROM infos WHERE id = :id");
  infos_query.bindValue(":id", id);

  if (!infos_query.exec()) {
    qDebug() << "Delete from infos failed: " << infos_query.lastError();
    db.rollback();
    return;
  }

  if (!db.commit()) {
    qDebug() << "Error: Unable to commit transaction";
    db.rollback();
  } else {
    qDebug() << "Delete succeeded";
  }
}
