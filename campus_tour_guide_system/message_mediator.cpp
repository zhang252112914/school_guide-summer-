#include "message_mediator.h"

MessageMediator::MessageMediator(MainPage *main_page, ViewPage *view_page,
                                 ManagePage *manage_page, HelpPage *help_page,
                                 AddEdgePage *add_edge_page,
                                 AddSitePage *add_site_page,
                                 DatabaseManager *db, CampusMap *cp,
                                 QStackedWidget *sw, QObject *parent)
    : QObject{parent},
      main_page(main_page),
      view_page(view_page),
      manage_page(manage_page),
      help_page(help_page),
      add_edge_page(add_edge_page),
      add_site_page(add_site_page),
      db_manager(db),
      campus_map(cp),
      stacked_widget(sw) {
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

  // Assuming 'stacked_widget' is your Qstacked_widget and is accessible here
  // Connect to change the page index
  connect(main_page, &MainPage::ShowViewPage, [this]() {
    this->stacked_widget->setCurrentIndex(1);  // Index of view_page
  });
  connect(main_page, &MainPage::ShowManagePage, [this]() {
    this->stacked_widget->setCurrentIndex(2);  // Index of manage_page
  });
  connect(main_page, &MainPage::ShowHelpPage, [this]() {
    this->stacked_widget->setCurrentIndex(3);  // Index of help_page
  });

  // Going back to main page from any other page
  connect(view_page, &ViewPage::BackToMainPage, [this]() {
    this->stacked_widget->setCurrentIndex(0);  // Index of main_page
  });
  connect(manage_page, &ManagePage::BackToMainPage, [this]() {
    this->stacked_widget->setCurrentIndex(0);  // Index of main_page
  });
  connect(help_page, &HelpPage::BackToMainPage, [this]() {
    this->help_page
        ->CloseAllGif();  // Assume this method handles necessary cleanup
    this->stacked_widget->setCurrentIndex(0);  // Index of main_page
  });

  connect(view_page, &ViewPage::IdRequest, campus_map,
          &CampusMap::SearchNodeSlot);
  connect(view_page, &ViewPage::RequestSites, campus_map,
          &CampusMap::GetSiteSlot);
  connect(view_page, &ViewPage::MyIdRequest, campus_map,
          &CampusMap::SearchNodeSlot);
  connect(view_page, &ViewPage::MyInfoRequest, campus_map,
          &CampusMap::GetInfoFromIdSlot);
  connect(view_page, &ViewPage::CallFindPath, campus_map, &CampusMap::FindPath);

  connect(campus_map, &CampusMap::NodeFound, view_page,
          &ViewPage::IdsReceiverAndFindCaller);
  connect(campus_map, &CampusMap::SitesFound, view_page,
          &ViewPage::HandleSitesFound);
  connect(campus_map, &CampusMap::NodeFound, view_page, &ViewPage::IdsReceiver);
  connect(campus_map, &CampusMap::ReturnPathVector, view_page,
          &ViewPage::HandlePathVector);
  connect(campus_map, &CampusMap::InfoFound, view_page, &ViewPage::DisplayInfo);
  connect(manage_page, &ManagePage::ShowAddEdgePage, add_edge_page,
          &AddEdgePage::show);

  connect(manage_page, &ManagePage::ShowAddEdgePage,
          [this]() { this->stacked_widget->setCurrentIndex(4); });
  connect(manage_page, &ManagePage::ShowAddSitePage,
          [this]() { this->stacked_widget->setCurrentIndex(5); });

  connect(add_edge_page, &AddEdgePage::BackToManagePage,
          [this]() { this->stacked_widget->setCurrentIndex(2); });
  connect(add_site_page, &AddSitePage::BackToManagePage,
          [this]() { this->stacked_widget->setCurrentIndex(2); });

  // add_site_page的信号和槽
  // 下面两个connect都是针对add_site_page和campus_map之间关于id的交互
  connect(add_site_page, &AddSitePage::CoordinateToNode, campus_map,
          &CampusMap::SearchNodeSlot);
  connect(campus_map, &CampusMap::NodeFound, add_site_page,
          &AddSitePage::ModifyNode);
  connect(campus_map, &CampusMap::NodeNotFound, add_site_page,
          &AddSitePage::InvalidNode);
  connect(add_site_page, &AddSitePage::PaintRequest, campus_map,
          &CampusMap::ReturnNodesToAddSitePage);
  connect(campus_map, &CampusMap::NodesFeedBack, add_site_page,
          &AddSitePage::PaintMap);

  // 用于景点信息查询的
  connect(add_site_page, &AddSitePage::PresentInfoRequest, campus_map,
          &CampusMap::GetInfoFromIdSlot);
  connect(campus_map, &CampusMap::InfoFound, add_site_page,
          &AddSitePage::PresentInfo);

  // 添加或修改节点的
  connect(add_site_page, &AddSitePage::AddMessageCollection, campus_map,
          &CampusMap::AddInfoSlot);
  connect(add_site_page, &AddSitePage::EditMessageCollection, campus_map,
          &CampusMap::EditInfoSlot);
  connect(manage_page, &ManagePage::ShowAddSitePage, add_site_page,
          &AddSitePage::PaintRequestWrapper);
  connect(manage_page, &ManagePage::ShowAddEdgePage, add_edge_page,
          &AddEdgePage::RequestWrapper);

  connect(add_edge_page, &AddEdgePage::NewNode, campus_map,
          &CampusMap::AddNode);
  connect(add_edge_page, &AddEdgePage::NewEdge, campus_map,
          &CampusMap::AddEdge);
  connect(add_edge_page, &AddEdgePage::GetEdges, campus_map,
          &CampusMap::GetEdgeSlot);
  connect(campus_map, &CampusMap::EdgesFound, add_edge_page,
          &AddEdgePage::PaintEdges);
  connect(add_edge_page, &AddEdgePage::FindNode, campus_map,
          &CampusMap::SearchNodeSlot);
  connect(campus_map, &CampusMap::NodeFound, add_edge_page,
          &AddEdgePage::ConfirmNode);
}
