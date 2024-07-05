#ifndef MESSAGE_MEDIATOR_H
#define MESSAGE_MEDIATOR_H

#include <QObject>

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
                           DatabaseManager *db, CampusMap *cp,
                           QObject *parent = nullptr);

 private:
  MainPage *main_page;
  ViewPage *view_page;
  ManagePage *manage_page;
  HelpPage *help_page;
  DatabaseManager *db_manager;
  CampusMap *campus_map;

 signals:
};

#endif  // MESSAGE_MEDIATOR_H
