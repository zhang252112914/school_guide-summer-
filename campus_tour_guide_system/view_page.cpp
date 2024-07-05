#include "view_page.h"

#include "ui_view_page.h"

ViewPage::ViewPage(QWidget *parent)
    : QWidget(parent), view_page(new Ui::ViewPage) {
  view_page->setupUi(this);
}

ViewPage::~ViewPage() { delete view_page; }

void ViewPage::on_return_button_clicked() {
  this->hide();
  emit BackToMainPage();
}
