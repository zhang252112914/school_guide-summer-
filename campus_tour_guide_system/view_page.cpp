#include "view_page.h"

#include "ui_view_page.h"
ViewPage::ViewPage(QWidget *parent)
    : QWidget(parent), view_page(new Ui::ViewPage) {
  view_page->setupUi(this);

  GraphicsDisplay *graphicsView =
      qobject_cast<GraphicsDisplay *>(view_page->graphicsView);
  if (graphicsView) {
    connect(graphicsView, &GraphicsDisplay::pointClicked, this,
            &ViewPage::handlePointClicked);
  }
}

ViewPage::~ViewPage() { delete view_page; }

void ViewPage::on_return_button_clicked() {
  this->hide();
  emit BackToMainPage();
}
void ViewPage::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  GraphicsDisplay *graphicsView =
      qobject_cast<GraphicsDisplay *>(view_page->graphicsView);
  if (graphicsView) {
    graphicsView->fitViewToScene();
  }
}

void ViewPage::handlePointClicked(double x, double y) {
  lastClickedX = x;  // 保存x坐标
  lastClickedY = y;  // 保存y坐标
  qDebug() << "Received point coordinates: x =" << x << ", y =" << y;
}

void ViewPage::on_addNode_Btn_clicked() {
  GraphicsDisplay *graphicsView =
      qobject_cast<GraphicsDisplay *>(view_page->graphicsView);
  if (graphicsView) {
    // 连接点
    graphicsView->connectPoints();
  }
}

void ViewPage::on_pushButton_clicked() {
  GraphicsDisplay *graphicsView =
      qobject_cast<GraphicsDisplay *>(view_page->graphicsView);
  if (graphicsView) {
    graphicsView->clearPoints();  // 调用清除点的函数
  }
}
