#include "message_mediator.h"

MessageMediator::MessageMediator(MainPage *main_page, ViewPage *view_page,
                                 ManagePage *manage_page, HelpPage *help_page,
                                 DatabaseManager *db, CampusMap *cp,
                                 QObject *parent)
    : QObject{parent},
      main_page(main_page),
      view_page(view_page),
      manage_page(manage_page),
      help_page(help_page),
      db_manager(db),
      campus_map(cp) {
  // Capture signals sent by DatabaseManager object and transfer data to Slot
  // functions
  connect(campus_map, &CampusMap::NodeAdded, db_manager,
          &DatabaseManager::SerializeNodeSlot);
  connect(campus_map, &CampusMap::EdgeAdded, db_manager,
          &DatabaseManager::SerializeEdgeSlot);
  connect(campus_map, &CampusMap::InfoAdded, db_manager,
          &DatabaseManager::SerializeInfoSlot);
  connect(campus_map, &CampusMap::InfoEdited, db_manager,
          &DatabaseManager::UpdateInfoSlot);
  connect(campus_map, &CampusMap::InfoDeleted, db_manager,
          &DatabaseManager::DeleteInfoSlot);

  connect(db_manager, &DatabaseManager::NodeLoaded, campus_map,
          &CampusMap::ReadNodeSlot);
  connect(db_manager, &DatabaseManager::EdgeLoaded, campus_map,
          &CampusMap::ReadEdgeSlot);
  connect(db_manager, &DatabaseManager::InfoLoaded, campus_map,
          &CampusMap::ReadInfoSlot);

  connect(view_page, &ViewPage::BackToMainPage, main_page, &MainPage::show);
  connect(manage_page, &ManagePage::BackToMainPage, main_page, &MainPage::show);
  connect(help_page, &HelpPage::BackToMainPage, main_page, [=]() {
    help_page->CloseAllGif();
    main_page->show();
  });

  connect(main_page, &MainPage::ShowViewPage, view_page, &ViewPage::show);
  connect(main_page, &MainPage::ShowManagePage, manage_page, &ManagePage::show);
  connect(main_page, &MainPage::ShowHelpPage, help_page, &HelpPage::show);
}
