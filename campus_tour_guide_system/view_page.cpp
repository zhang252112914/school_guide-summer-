#include "view_page.h"

#include "ui_view_page.h"

ViewPage::ViewPage(QWidget *parent)
    : QWidget(parent), view_page(new Ui::ViewPage) {
  view_page->setupUi(this);
  graphics_display =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_display);
  if (graphics_display) {
    connect(graphics_display, &GraphicsDisplay::PointClicked, this,
            &ViewPage::HandlePointClicked);
  }
  // 设置文本编辑器为只读
  view_page->text_edit->setReadOnly(true);
  // 使用 QTimer在构造函数完成后立即发出 RequestSites 信号
  QTimer::singleShot(0, this,
                     [this]() { emit RequestSites(Sender::VIEW_PAGE); });
}

ViewPage::~ViewPage() { delete view_page; }

void ViewPage::on_return_button_clicked() {
  GraphicsDisplay *graphics_display =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_display);
  if (graphics_display) {
    graphics_display->ClearPoints();
    view_page->info_graphics_view->scene()->clear();
    view_page->text_edit->clear();
    graphics_display->ClearBluePoints();  // 调用清除点的函数
  }
  this->hide();
  emit BackToMainPage();
}

void ViewPage::HandleSitesFound(
    QVector<QPair<QPair<double, double>, QString>> sites, Sender sender) {
  if (sender != Sender::VIEW_PAGE) {
    return;
  }
  if (sites.isEmpty()) {
    qDebug() << "No sites found.";
  }
  GraphicsDisplay *graphics_display =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_display);
  graphics_display->ClearPoints();
  for (const auto &site : sites) {
    qDebug() << "Site position: " << site.first.first << ", "
             << site.first.second << " with name: " << site.second;
    double x = site.first.first;
    double y = site.first.second;
    const QString &label = site.second;
    graphics_display->AddPoint(x, y, label);
  }
  qDebug() << "do ";
}

void ViewPage::HandlePathVector(QVector<QPair<double, double>> route) {
  GraphicsDisplay *graphics_display =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_display);
  graphics_display->ClearBluePoints();
  if (route.isEmpty()) {
    qDebug() << "no path found";
  }
  for (const auto &routes : route) {
    double x = routes.first;
    double y = routes.second;
    graphics_display->AddPoint(x, y);
    // graphics_display->ConnectPoints();
  }
  graphics_display->ConnectPoints();
}

void ViewPage::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  GraphicsDisplay *graphics_display =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_display);
  if (graphics_display) {
    graphics_display->FitViewToScene();
  }
}

void ViewPage::HandlePointClicked(double x, double y) {
  GraphicsDisplay *graphics_display =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_display);
  if (!graphics_display) return;

  // 记录点击的坐标
  pre_clicked_x = last_clicked_x;
  pre_clicked_y = last_clicked_y;
  last_clicked_x = x;
  last_clicked_y = y;
  qDebug() << "HandlePointClicked called with x =" << x << ", y =" << y;

  //  请求坐标匹配
  emit MyIdRequest(x, y, Sender::VIEW_PAGE);
}

void ViewPage::IdsReceiver(const Node &node, Sender sender) {
  GraphicsDisplay *graphics_display =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_display);
  if (sender != Sender::VIEW_PAGE) {
    return;
  }
  qDebug() << "nice";
  if (!node.info_valid) {
    return;
  }
  if (graphics_display) {
    graphics_display->AddBlackPoint(node.pos_x, node.pos_y);
  }
  emit MyInfoRequest(node.id, Sender::VIEW_PAGE);
}

void ViewPage::on_addnode_button_clicked() {
  GraphicsDisplay *graphics_display =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_display);
  if (graphics_display) {
    // 连接点
    graphics_display->ConnectPoints();
  }
}

void ViewPage::on_clear_button_clicked() {
  GraphicsDisplay *graphics_display =
      qobject_cast<GraphicsDisplay *>(view_page->graphics_display);
  if (graphics_display) {
    graphics_display->ClearPoints();
    view_page->info_graphics_view->scene()->clear();
    view_page->text_edit->clear();
    graphics_display->ClearBluePoints();  // 调用清除点的函数
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

void ViewPage::DisplayInfo(const Info &info, const QByteArray &image_data,
                           Sender sender) {
  // 清除之前的图像和文本
  if (sender != Sender::VIEW_PAGE) {
    return;
  }
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
  view_page->text_edit->clear();
  QString text =
      QString("Name: %1\nDescription: %2").arg(info.name, info.description);
  view_page->text_edit->setText(text);
}
void ViewPage::OnNodeNotFound(Sender sender) {
  if (sender == Sender::VIEW_PAGE) {
    QMessageBox::warning(this, "景点搜索", "不是景点");
  }
}
