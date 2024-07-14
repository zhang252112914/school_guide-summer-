#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMetaType>
#include <memory>

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
  qRegisterMetaType<UpdateFlags>("UpdateFlags");

  // Create MainPage and other pages
  auto main_page = std::make_unique<MainPage>();
  auto view_page = std::make_unique<ViewPage>();
  auto manage_page = std::make_unique<ManagePage>();
  auto help_page = std::make_unique<HelpPage>();

  // DatabaseManager initialization
  QDir dir;
  QString path = QDir::homePath() + "/Documents/myapp/config";

  if (!dir.exists(path)) {
    if (!dir.mkpath(path)) {
      qDebug() << "Failed to create directory:" << path;
      return -1;
    }
  }

  QString config_file_path = path + "/db_config.conf";
  qDebug() << "Config file path:" << config_file_path;

  QFile configFile(config_file_path);
  if (!configFile.exists()) {
    qDebug() << "Config file does not exist:" << config_file_path;
  }

  auto db_manager = std::make_unique<DatabaseManager>(config_file_path);

  if (!db_manager->IsOpen()) {
    qDebug() << "Failed to open the database.";
    return -1;
  }

  // CampusMap initialization
  auto campus_map = std::make_unique<CampusMap>();

  // MessageMediator initialization
  auto message_mediator = std::make_unique<MessageMediator>(
      main_page.get(), view_page.get(), manage_page.get(), help_page.get(),
      db_manager.get(), campus_map.get());

  // Deserialize data
  db_manager->DeserializeNodes();
  db_manager->DeserializeEdges();
  db_manager->DeserializeInfos();

  // Show main page
  main_page->show();

  int return_value = a.exec();

  // Smart pointers will automatically clean up
  return return_value;
}
