#include "view_page.h"

#include "ui_view_page.h"

ViewPage::ViewPage(QWidget *parent)
    : QWidget(parent), view_page(new Ui::ViewPage) {
  view_page->setupUi(this);
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

void ViewPage::handleSitesFound(
    QVector<QPair<QPair<double, double>, QString>> sites, Sender sender) {
  if (sites.isEmpty()) {
    qDebug() << "No sites found.";
  }
  for (const auto &site : sites) {
    qDebug() << "Site position: " << site.first.first << ", "
             << site.first.second << " with name: " << site.second;
    double x = site.first.first;
    double y = site.first.second;
    const QString &label = site.second;
    graphics_view->AddPoint(x, y, label);
  }
  qDebug() << "do ";
}

void ViewPage::HandlePathVector(QVector<QPair<double, double>> route) {
  GraphicsDisplay *graphicsView =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_view);
  graphicsView->ClearBluePoints();
  if (route.isEmpty()) {
    qDebug() << "no path found";
  }
  for (const auto &routes : route) {
    double x = routes.first;
    double y = routes.second;
    graphicsView->AddPoint(x, y);
    graphicsView->ConnectPoints();
  }
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
  GraphicsDisplay *graphicsView =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_view);
  if (!graphicsView) return;

  // 记录点击的坐标
  pre_clicked_x = last_clicked_x;
  pre_clicked_y = last_clicked_y;
  last_clicked_x = x;
  last_clicked_y = y;

  qDebug() << "Received point coordinates: x =" << x << ", y =" << y;
  emit IdRequest(pre_clicked_x, pre_clicked_y, Sender::VIEW_PAGE);
  emit IdRequest(last_clicked_x, last_clicked_y, Sender::VIEW_PAGE);
  // 请求坐标匹配
  emit MyIdRequest(x, y, Sender::VIEW_PAGE);
}

void ViewPage::IdsReceiver(double x, double y, Sender sender) {
  GraphicsDisplay *graphics_view =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_view);
  if (graphics_view) {
    graphics_view->addBlackPoint(x, y);
  }
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
    graphics_view->ClearPoints();
    graphics_view->ClearBluePoints();  // 调用清除点的函数
  }
}

void ViewPage::IdsReceiverAndFindCaller(int id, Sender sender) {
  if (sender == Sender::VIEW_PAGE) {
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
}

void ViewPage::on_route_button_clicked() {
  emit requestSites(Sender::VIEW_PAGE);
}
