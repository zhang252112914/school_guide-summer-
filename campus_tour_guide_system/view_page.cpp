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

void ViewPage::HandleSitesFound(
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
void ViewPage::IdsReceiver(const Node &node, Sender sender) {
  GraphicsDisplay *graphics_view =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_view);
  if (!node.info_valid) {
    return;
  }
  if (graphics_view) {
    graphics_view->AddBlackPoint(node.pos_x, node.pos_y);
  }
  emit MyInfoRequest(node.id, Sender::VIEW_PAGE);
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

void ViewPage::IdsReceiverAndFindCaller(const Node &node, Sender sender) {
  if (sender == Sender::VIEW_PAGE) {
    if (!node.info_valid) {
      return;
    }
    if (!first_arrive) {
      first_arrived_id = node.id;
      first_arrive = true;
      qDebug() << "id1 arrived";
    } else {
      second_arrived_id = node.id;
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
  emit RequestSites(Sender::VIEW_PAGE);
}
/*
void ViewPage::DisplayInfo(const Info &info, const QByteArray &image_data,
                           Sender sender) {
  // 清除之前的图像和文本
  view_page->info_graphics_view->scene()->clear();
  view_page->text_edit->clear();
  QGraphicsScene *scene = new QGraphicsScene(this);
  view_page->info_graphics_view->setScene(scene);
  QPixmap image;
  if (!image_data.isEmpty()) {
    if (!image.loadFromData(image_data)) {
      qDebug() << "Unable to load image from data.";
    }
  }

  if (!image.isNull()) {
    scene->addPixmap(image.scaled(view_page->info_graphics_view->size(),
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation));
  } else {
    qDebug() << "No image data provided.";
  }

  // 显示名称和描述
  QString text =
      QString("Name: %1\nDescription: %2").arg(info.name, info.description);
  view_page->text_edit->setText(text);
}*/
void ViewPage::DisplayInfo(const Info &info, const QByteArray &image_data,
                           Sender sender) {
  // 清除之前的图像和文本
  if (view_page->info_graphics_view->scene()) {
    view_page->info_graphics_view->scene()->clear();
  } else {
    QGraphicsScene *scene = new QGraphicsScene(this);
    view_page->info_graphics_view->setScene(scene);
  }

  QPixmap image;
  if (!image_data.isEmpty()) {
    if (image.loadFromData(image_data)) {
      // 图片加载成功，根据 QGraphicsView 的大小调整图片大小
      QGraphicsScene *scene = view_page->info_graphics_view->scene();
      scene->addPixmap(image.scaled(view_page->info_graphics_view->size(),
                                    Qt::IgnoreAspectRatio,  // 改为忽略宽高比
                                    Qt::SmoothTransformation));  // 保持平滑转换
    } else {
      qDebug() << "Unable to load image from data.";
    }
  } else {
    qDebug() << "No image data provided.";
  }

  // 显示名称和描述
  QString text =
      QString("Name: %1\nDescription: %2").arg(info.name, info.description);
  view_page->text_edit->setText(text);
}
