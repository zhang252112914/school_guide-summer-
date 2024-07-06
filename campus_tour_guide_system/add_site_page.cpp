#include "add_site_page.h"

#include <QMessageBox>

#include "ui_add_site_page.h"

AddSitePage::AddSitePage(QWidget *parent)
    : QWidget(parent), add_site_page(new Ui::AddSitePage) {
  add_site_page->setupUi(this);
  setWindowTitle("Add Site");
}

AddSitePage::~AddSitePage() { delete add_site_page; }

void AddSitePage::on_return_button_clicked() {
  this->hide();
  emit BackToManagePage();
}

void AddSitePage::on_confirm_button_clicked() {
  QMessageBox::information(this, "提示", "添加成功");
  emit AddNode();
}
