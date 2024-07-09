#include "database_manager.h"

DatabaseManager::DatabaseManager(const QString &path, QObject *parent)
    : QObject{parent} {
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(path);

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
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "pos_x REAL NOT NULL, "
      "pos_y REAL NOT NULL)");
  if (!success) {
    qDebug() << "Failed to create table:" << query.lastError().text();
  }

  success = query.exec(
      "CREATE TABLE IF NOT EXISTS edges ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "end_node_one_id INTEGER NOT NULL, "
      "end_node_two_id INTEGER NOT NULL, "
      "length REAL NOT NULL)");
  if (!success) {
    qDebug() << "Failed to create table:" << query.lastError().text();
  }

  success = query.exec(
      "CREATE TABLE IF NOT EXISTS infos ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "name TEXT NOT NULL, "
      "description TEXT NOT NULL, "
      "pic_path TEXT NOT NULL)");
  if (!success) {
    qDebug() << "Failed to create table:" << query.lastError().text();
  }
}

DatabaseManager::~DatabaseManager() { db.close(); }

// Read data from database and translate its type to node, edge and info
void DatabaseManager::DeserializeNodes() {
  if (!db.isOpen()) {
    qDebug() << "Error: Database is not open";
    return;
  }

  QSqlQuery query(db);
  if (!query.prepare("SELECT id, pos_x, pos_y FROM nodes")) {
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
      "INSERT INTO nodes (id, pos_x, pos_y) VALUES (:id, :pos_x, :pos_y)");
  query.bindValue(":id", node.id);
  query.bindValue(":pos_x", node.pos_x);
  query.bindValue(":pos_y", node.pos_y);

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
      "UPDATE infos SET id = :id, name = :name, description = :description, "
      "pic_path = :pic_path");
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

  QSqlQuery query;
  query.prepare("DELETE FROM infos WHERE id = :id");
  query.bindValue(":id", id);

  if (!query.exec()) {
    qDebug() << "Delete failed: " << query.lastError();
  } else {
    qDebug() << "Delete succeeded";
  }
}
