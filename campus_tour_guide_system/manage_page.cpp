#include "manage_page.h"

#include "ui_manage_page.h"

ManagePage::ManagePage(QWidget *parent)
    : QWidget(parent), manage_page(new Ui::ManagePage) {
  manage_page->setupUi(this);
  setWindowTitle("地图管理");
}

ManagePage::~ManagePage() { delete manage_page; }

void ManagePage::on_return_button_clicked() {
  this->hide();
  emit BackToMainPage();
}

void ManagePage::on_add_edge_button_clicked() {
  this->hide();
  emit ShowAddEdgePage();
}

void ManagePage::on_add_site_button_clicked() {
  this->hide();
  emit ShowAddSiteEdge();
}
