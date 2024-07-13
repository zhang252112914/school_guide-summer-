#include "graphics_display.h"

GraphicsDisplay::GraphicsDisplay(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this)) {
  this->setScene(scene);
  connect(campus_map, &CampusMap::SitesFound, my_graphics,
          &GraphicsDisplay::DisplaySites);
  // Load the map image
  QPixmap map_pixmap(":/map.png");  // 加载图片
  QSize newSize(2500, 2000);        // 新的大小，自行设置
  QPixmap scaledPixmap =
      map_pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  map_item = new QGraphicsPixmapItem(scaledPixmap);
  scene->addItem(map_item);

  // Set the scene size to the size of the map image
  scene->setSceneRect(map_pixmap.rect());

  // Ensure the whole image is visible
  this->fitInView(map_item, Qt::KeepAspectRatio);

  // Optionally, disable scrollbars if you want the view to be fixed
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QGraphicsScene *GraphicsDisplay::GetScene() const { return scene; }

void GraphicsDisplay::FitViewToScene() {
  if (scene) {
    this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
  }
}

void GraphicsDisplay::ClearPoints() {
  /*
  for (auto point : points) {
      scene->removeItem(point);
      delete point;
  }
  points.clear();

  // 删除所有标签
  for (auto label : labels) {
      scene->removeItem(label);
      delete label;
  }
  labels.clear();*/
  // 删除所有线条
  for (auto *point : black_points) {
    scene->removeItem(point);
    delete point;
  }
  black_points.clear();

  // 强制刷新视图

  scene->update();
  this->viewport()->update();
}

void GraphicsDisplay::AddPoint(double x, double y) {
  QGraphicsEllipseItem *point =
      scene->addEllipse(x - 5, y - 5, 10, 10, QPen(Qt::blue), QBrush(Qt::blue));
  blue_points.append(point);  // 添加到蓝色点的容器
  scene->update();
  this->viewport()->update();
}
void GraphicsDisplay::ConnectPoints() {
  if (blue_points.size() < 2) {
    qDebug() << "Not enough points to connect.";
    return;
  }
  for (int i = 0; i < blue_points.size() - 1; i++) {
    QPointF p1 = blue_points[i]->rect().center();
    QPointF p2 = blue_points[i + 1]->rect().center();
    QGraphicsLineItem *line = scene->addLine(QLineF(p1, p2), QPen(Qt::blue, 2));
    lines.append(line);
  }
}

void GraphicsDisplay::ClearBluePoints() {
  for (auto point : blue_points) {
    scene->removeItem(point);
    delete point;
  }
  blue_points.clear();
  // 清除线条
  for (auto line : lines) {
    scene->removeItem(line);
    delete line;
  }
  lines.clear();
  scene->update();
  this->viewport()->update();
}

void GraphicsDisplay::mousePressEvent(QMouseEvent *event) {
  QPointF scenePos = this->mapToScene(event->pos());
  double x = scenePos.x();
  double y = scenePos.y();

  if (scene->sceneRect().contains(scenePos)) {
    // AddBlackPoint(x, y);
    emit PointClicked(x, y);  // 发送坐标信号
  }
}

void GraphicsDisplay::DisplaySites(
    const QVector<QPair<QPair<double, double>, QString>> &sites) {
  ClearPoints();  // 首先清除所有旧的点和标签

  for (const auto &site : sites) {
    double x = site.first.first;
    double y = site.first.second;
    const QString &label_text = site.second;

    AddPoint(x, y, label_text);
  }
}

void GraphicsDisplay::AddPoint(int x, int y, const QString &label_text) {
  // 创建一个点
  QGraphicsEllipseItem *point =
      scene->addEllipse(x - 5, y - 5, 10, 10, QPen(Qt::red), QBrush(Qt::red));
  points.append(point);

  // 创建一个标签
  QGraphicsTextItem *label = scene->addText(label_text);
  label->setDefaultTextColor(Qt::red);
  label->setPos(x + 10, y - 10);
  labels.append(label);

  // 刷新视图
  scene->update();
  this->viewport()->update();
}

void GraphicsDisplay::DisplayPoint(double x, double y, bool matched) {
  QColor color =
      matched ? Qt::black : Qt::red;  // 如果匹配，显示黑色，否则显示红色
  QGraphicsEllipseItem *point =
      scene->addEllipse(x - 5, y - 5, 10, 10, QPen(color), QBrush(color));
  points.append(point);
  scene->update();
}

void GraphicsDisplay::AddBlackPoint(double x, double y) {
  if (black_points.size() >= maxblack_points) {  // 如果已达到或超过最大点数限制
    // 移除最早添加的点
    QGraphicsEllipseItem *oldestPoint = black_points.takeFirst();
    scene->removeItem(oldestPoint);
    delete oldestPoint;
  }
  // 添加新的黑色点
  QGraphicsEllipseItem *point = scene->addEllipse(
      x - 5, y - 5, 10, 10, QPen(Qt::black), QBrush(Qt::black));
  black_points.append(point);
}
