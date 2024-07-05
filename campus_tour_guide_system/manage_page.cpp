#include "manage_page.h"

#include "ui_manage_page.h"

ManagePage::ManagePage(QWidget *parent)
    : QWidget(parent), manage_page(new Ui::ManagePage) {
  manage_page->setupUi(this);
}

ManagePage::~ManagePage() { delete manage_page; }

void ManagePage::on_return_button_clicked() {
  this->hide();
  emit BackToMainPage();
}
