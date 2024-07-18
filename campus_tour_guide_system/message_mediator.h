#ifndef MESSAGE_MEDIATOR_H
#define MESSAGE_MEDIATOR_H

#include <QObject>
#include <QStackedWidget>

#include "add_edge_page.h"
#include "add_site_page.h"
#include "campus_map.h"
#include "database_manager.h"
#include "help_page.h"
#include "main_page.h"
#include "manage_page.h"
#include "view_page.h"

class MessageMediator : public QObject {
  Q_OBJECT
 public:
  explicit MessageMediator(MainPage *main_page, ViewPage *view_page,
                           ManagePage *manage_page, HelpPage *help_page,
                           AddEdgePage *add_edge_page,
                           AddSitePage *add_site_page, DatabaseManager *db,
                           CampusMap *cp, QStackedWidget *sw,
                           QObject *parent = nullptr);

 private:
  MainPage *main_page;
  ViewPage *view_page;
  ManagePage *manage_page;
  HelpPage *help_page;
  AddSitePage *add_site_page;
  AddEdgePage *add_edge_page;
  DatabaseManager *db_manager;
  CampusMap *campus_map;
  QStackedWidget *stacked_widget;

 signals:
};

#endif  // MESSAGE_MEDIATOR_H
