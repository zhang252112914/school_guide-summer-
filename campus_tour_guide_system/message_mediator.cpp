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
  connect(campus_map, &CampusMap::RequestImageData, db_manager,
          &DatabaseManager::FetchImageData);

  connect(db_manager, &DatabaseManager::NodeLoaded, campus_map,
          &CampusMap::ReadNodeSlot);
  connect(db_manager, &DatabaseManager::EdgeLoaded, campus_map,
          &CampusMap::ReadEdgeSlot);
  connect(db_manager, &DatabaseManager::InfoLoaded, campus_map,
          &CampusMap::ReadInfoSlot);
  connect(db_manager, &DatabaseManager::ImageDataFetched, campus_map,
          &CampusMap::HandleImageDataFetchedSlot);

  connect(view_page, &ViewPage::BackToMainPage, main_page, &MainPage::show);
  connect(manage_page, &ManagePage::BackToMainPage, main_page, &MainPage::show);
  connect(help_page, &HelpPage::BackToMainPage, main_page, [=]() {
    help_page->CloseAllGif();
    main_page->show();
  });

  connect(main_page, &MainPage::ShowViewPage, view_page, &ViewPage::show);
  connect(main_page, &MainPage::ShowManagePage, manage_page, &ManagePage::show);
  connect(main_page, &MainPage::ShowHelpPage, help_page, &HelpPage::show);

  connect(view_page, &ViewPage::IdRequest, campus_map,
          &CampusMap::SearchNodeSlot);
  connect(campus_map, &CampusMap::NodeFound, view_page,
          &ViewPage::IdsReceiverAndFindCaller);
  connect(
      campus_map, &CampusMap::NodeNotFound, view_page,
      [view_page](Sender sender) { qDebug() << "No matching node found."; });

  connect(campus_map, &CampusMap::SitesFound, view_page,
          &ViewPage::HandleSitesFound);
  connect(view_page, &ViewPage::RequestSites, campus_map,
          &CampusMap::GetSiteSlot);
  connect(campus_map, &CampusMap::NodeFound, view_page, &ViewPage::IdsReceiver);
  connect(view_page, &ViewPage::MyIdRequest, campus_map,
          &CampusMap::SearchNodeSlot);
  connect(campus_map, &CampusMap::NodeNotFound, view_page,
          [view_page](Sender sender) { qDebug() << "not matched"; });
  connect(campus_map, &CampusMap::ReturnPathVector, view_page,
          &ViewPage::HandlePathVector);
  connect(campus_map, &CampusMap::InfoFound, view_page, &ViewPage::displayInfo);
  connect(view_page, &ViewPage::MyInfoRequest, campus_map,
          &CampusMap::GetInfoFromIdSlot);
}
