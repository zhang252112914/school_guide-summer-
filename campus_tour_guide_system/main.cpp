#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QMainWindow>
#include <QMetaType>
#include <QStackedWidget>
#include <memory>

#include "add_edge_page.h"
#include "add_site_page.h"
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

  // Create a main window or a central widget if necessary
  QMainWindow main_window;
  QIcon app_icon(":/paper-plane.png");
  main_window.setWindowIcon(app_icon);

  // QStackedWidget managed by mainWindow
  QStackedWidget *stacked_widget = new QStackedWidget(&main_window);

  // Directly create and manage widgets with raw pointers, since QStackedWidget
  // takes ownership
  MainPage *main_page = new MainPage();
  ViewPage *view_page = new ViewPage();
  ManagePage *manage_page = new ManagePage();
  HelpPage *help_page = new HelpPage();
  AddEdgePage *add_edge_page = new AddEdgePage();
  AddSitePage *add_site_page = new AddSitePage();

  // Add pages to the stacked widget
  stacked_widget->addWidget(main_page);
  stacked_widget->addWidget(view_page);
  stacked_widget->addWidget(manage_page);
  stacked_widget->addWidget(help_page);
  stacked_widget->addWidget(add_edge_page);
  stacked_widget->addWidget(add_site_page);

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
      main_page, view_page, manage_page, help_page, add_edge_page,
      add_site_page, db_manager.get(), campus_map.get(), stacked_widget);

  // Deserialize data
  db_manager->DeserializeNodes();
  db_manager->DeserializeEdges();
  db_manager->DeserializeInfos();

  // Show main page
  main_window.setCentralWidget(stacked_widget);
  main_window.show();

  return a.exec();
}
