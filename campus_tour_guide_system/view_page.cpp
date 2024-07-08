#include "view_page.h"

#include "ui_view_page.h"

ViewPage::ViewPage(QWidget *parent)
    : QWidget(parent), view_page(new Ui::ViewPage) {
  view_page->setupUi(this);

  GraphicsDisplay *graphics_view =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_view);
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
