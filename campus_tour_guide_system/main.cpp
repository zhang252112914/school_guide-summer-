#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMetaType>

#include "campus_map.h"
#include "database_manager.h"
#include "help_page.h"
#include "main_page.h"
#include "manage_page.h"
#include "message_mediator.h"
#include "types.h"
#include "view_page.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // Declare MetaType
  qRegisterMetaType<Node>("Node");
  qRegisterMetaType<Edge>("Edge");
  qRegisterMetaType<Info>("Info");

  MainPage *main_page = new MainPage();
  ViewPage *view_page = new ViewPage();
  ManagePage *manage_page = new ManagePage();
  HelpPage *help_page = new HelpPage();

  // DatabaseManager initialization
  QString path = QDir::currentPath() + QDir::separator() + "my_database.db";
  qDebug() << path;
  DatabaseManager *db_manager = new DatabaseManager(path);

  // CampusMap initialization
  CampusMap *campus_map = new CampusMap();

  MessageMediator *message_mediator = new MessageMediator(
      main_page, view_page, manage_page, help_page, db_manager, campus_map);

  db_manager->DeserializeNodes();
  db_manager->DeserializeEdges();
  db_manager->DeserializeInfos();

  main_page->show();
  int return_value = a.exec();
  delete campus_map;
  delete db_manager;
  delete message_mediator;
  return return_value;
}
