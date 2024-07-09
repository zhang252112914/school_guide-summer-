#include "view_page.h"

#include "ui_view_page.h"

ViewPage::ViewPage(QWidget *parent)
    : QWidget(parent), view_page(new Ui::ViewPage) {
  view_page->setupUi(this);

  connect(view_page->route_button, &QPushButton::clicked, this,
          &ViewPage::HandleRouteRequest);
  graphics_view = qobject_cast<GraphicsDisplay *>(view_page->graphics_view);
  if (graphics_view) {
    connect(graphics_view, &GraphicsDisplay::PointClicked, this,
            &ViewPage::HandlePointClicked);
  }
}

ViewPage::~ViewPage() { delete view_page; }

void ViewPage::on_return_button_clicked() {
  this->hide();
  emit BackToMainPage();
}

void ViewPage::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  GraphicsDisplay *graphics_view =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_view);
  if (graphics_view) {
    graphics_view->FitViewToScene();
  }
}

void ViewPage::HandlePointClicked(double x, double y) {
  pre_clicked_x = last_clicked_x;
  pre_clicked_y = last_clicked_y;
  last_clicked_x = x;  // 保存x坐标
  last_clicked_y = y;  // 保存y坐标
  qDebug() << "Received point coordinates: x =" << x << ", y =" << y;
}

void ViewPage::on_addnode_button_clicked() {
  GraphicsDisplay *graphics_view =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_view);
  if (graphics_view) {
    // 连接点
    graphics_view->ConnectPoints();
  }
}

void ViewPage::on_clear_button_clicked() {
  GraphicsDisplay *graphics_view =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_view);
  if (graphics_view) {
    graphics_view->ClearPoints();  // 调用清除点的函数
  }
}

void ViewPage::HandleRouteRequest() {
  emit IdRequest(pre_clicked_x, pre_clicked_y, last_clicked_x, last_clicked_y);
}

void ViewPage::IdsReceiverAndFindCaller(int id, int request_id) {
  if (!first_arrive) {
    first_arrived_id = id;
    first_arrive = true;
    qDebug() << "id1 arrived";
  } else {
    second_arrived_id = id;
    second_arrive = true;
    qDebug() << "id2 arrived";
  }

  if (first_arrive && second_arrive) {
    first_arrive = second_arrive = false;
    emit CallFindPath(first_arrived_id, second_arrived_id);
  }
}
