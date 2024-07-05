#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMetaType>

#include "campus_map.h"
#include "database_manager.h"
#include "main_page.h"
#include "types.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // Declare MetaType
  qRegisterMetaType<Node>("Node");
  qRegisterMetaType<Edge>("Edge");
  qRegisterMetaType<Info>("Info");

  MainPage w;

  // DatabaseManager initialization
  QString path = QDir::currentPath() + QDir::separator() + "my_database.db";
  qDebug() << path;
  DatabaseManager *db_manager = new DatabaseManager(path);

  // CampusMap initialization
  CampusMap *campus_map = new CampusMap();

  db_manager->SetCampusMap(campus_map);
  campus_map->SetDatabaseManager(db_manager);

  db_manager->DeserializeNodes();
  db_manager->DeserializeEdges();
  db_manager->DeserializeInfos();

  w.show();
  int return_value = a.exec();
  delete campus_map;
  delete db_manager;
  return return_value;
}
