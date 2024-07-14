#include "add_site_page.h"

#include <QMessageBox>

#include "ui_add_site_page.h"

AddSitePage::AddSitePage(QWidget *parent)
    : QWidget(parent), add_site_page(new Ui::AddSitePage) {
  add_site_page->setupUi(this);
  setWindowTitle("Add Site");
  graph = qobject_cast<GraphicsDisplay *>(add_site_page->graph_view);
  if (graph) {
    connect(graph, &GraphicsDisplay::PointClicked, this,
            &AddSitePage::receive_one_node);
  }
  //设置输入框的提示内容
  add_site_page->name_input->setPlaceholderText("请输入景点名称");
  add_site_page->info_input->setPlaceholderText("请输入景点介绍");
}

AddSitePage::~AddSitePage() { delete add_site_page; }

void AddSitePage::on_return_button_clicked() {
  emit BackToManagePage();
  this->hide();
}

void AddSitePage::on_confirm_button_clicked() {
  QString name = add_site_page->name_input->text();
  QString info = add_site_page->info_input->toPlainText();
  QString pic_path = "";
  //只有在id有效的时候才会发送添加景点的信号
  if (id != -1) {
    emit MessageCollection(id, name, info, pic_path);
  }
  //反馈给用户一些信息,可能做出一个弹窗会更加合适
  else {
    qDebug() << "The node you select is invalid";
  }
}

void AddSitePage::receive_one_node(double x, double y) {
  pos_x = x;
  pos_y = y;
  qDebug() << "Receive x and y:" << x << ":" << y;
  emit TurnCoordinateToId(x, y);
}

void AddSitePage::ModifyId(int back_id, Sender sender) {
  if (sender == Sender::MANAGE_PAGE) {
    id = back_id;
  }
}

void AddSitePage::InvalidId(Sender sender) {
  if (sender == Sender::MANAGE_PAGE) {
    qDebug() << "useless click";
    id = -1;
  }
}
