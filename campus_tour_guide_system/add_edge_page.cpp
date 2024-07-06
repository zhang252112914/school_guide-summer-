#include "add_edge_page.h"

#include "ui_add_edge_page.h"

AddEdgePage::AddEdgePage(QWidget *parent)
    : QWidget(parent), add_edge_page(new Ui::AddEdgePage) {
  add_edge_page->setupUi(this);
  setWindowTitle("Add Path");
}

AddEdgePage::~AddEdgePage() { delete add_edge_page; }

void AddEdgePage::on_return_button_clicked() {
  this->hide();
  emit BackToManagePage();
}

void AddEdgePage::on_confirm_button_clicked() {
  QMessageBox::information(this, "提示", "添加成功");
  emit AddEdge();
}
