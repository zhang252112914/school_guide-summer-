#include "graphics_display.h"

GraphicsDisplay::GraphicsDisplay(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this)) {
  this->setScene(scene);

  // Load the map image
  QPixmap map_pixmap(":/map.png");  // 加载图片
  QSize newSize(2500, 2000);        // 新的大小，自行设置
  QPixmap scaledPixmap =
      map_pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  map_item = new QGraphicsPixmapItem(scaledPixmap);
  scene->addItem(map_item);

  // Set the scene size to the size of the map image
  scene->setSceneRect(map_item->boundingRect());

  // Ensure the whole image is visible initially
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

void GraphicsDisplay::resizeEvent(QResizeEvent *event) {
  QGraphicsView::resizeEvent(event);
  FitViewToScene();
}

void GraphicsDisplay::ClearPoints() {
  // 删除所有线条
  for (auto *point : blackPoints) {
    scene->removeItem(point);
    delete point;
  }
  blackPoints.clear();

  // 强制刷新视图
  scene->update();
  this->viewport()->update();
}

void GraphicsDisplay::AddPoint(double x, double y) {
  QGraphicsEllipseItem *point =
      scene->addEllipse(x - 5, y - 5, 10, 10, QPen(Qt::blue), QBrush(Qt::blue));
  bluePoints.append(point);  // 添加到蓝色点的容器
  scene->update();
  this->viewport()->update();
}
void GraphicsDisplay::ConnectPoints() {
  if (bluePoints.size() < 2) {
    qDebug() << "Not enough points to connect.";
    return;
  }
  for (int i = 0; i < bluePoints.size() - 1; i++) {
    QPointF p1 = bluePoints[i]->rect().center();
    QPointF p2 = bluePoints[i + 1]->rect().center();
    QGraphicsLineItem *line = scene->addLine(QLineF(p1, p2), QPen(Qt::blue, 2));
    lines.append(line);
  }
}

void GraphicsDisplay::AddLine(QPointF p1, QPointF p2) {
  QGraphicsLineItem *line = scene->addLine(QLineF(p1, p2), QPen(Qt::blue, 2));
  lines.append(line);
  scene->update();
  this->viewport()->update();
}

void GraphicsDisplay::ClearBluePoints() {
  for (auto point : bluePoints) {
    scene->removeItem(point);
    delete point;
  }
  bluePoints.clear();
  // 清除线条
  for (auto line : lines) {
    scene->removeItem(line);
    delete line;
  }
  lines.clear();
  scene->update();
  this->viewport()->update();
}

void GraphicsDisplay::ClearRedPoints() {
  for (auto point : points) {
    scene->removeItem(point);
    delete point;
  }
  points.clear();
  scene->update();
  this->viewport()->update();
}

void GraphicsDisplay::mousePressEvent(QMouseEvent *event) {
  QPointF scenePos = this->mapToScene(event->pos());
  double x = scenePos.x();
  double y = scenePos.y();

  if (scene->sceneRect().contains(scenePos)) {
    // AddBlackPoint(x, y);
    qDebug() << "Mouse press event received at x:" << x << " y:" << y;

    emit PointClicked(x, y);  // 发送坐标信号
  }
}

void GraphicsDisplay::AddPoint(int x, int y, const QString &labelText) {
  // 创建一个更大的红色点
  QGraphicsEllipseItem *point =
      scene->addEllipse(x - 10, y - 10, 20, 20, QPen(Qt::red), QBrush(Qt::red));
  points.append(point);

  // 创建一个标签
  QGraphicsTextItem *label = scene->addText(labelText);
  label->setDefaultTextColor(Qt::red);
  label->setPos(x + 20, y - 10);  // 标签位置也相应调整
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
  qDebug() << blackPoints.size() << "个";
  if (blackPoints.size() >= maxBlackPoints) {  // 如果达到或超过最大点数限制
                                               // 移除最早添加的点
    qDebug() << "do it";
    scene->removeItem(blackPoints[0]);
    delete blackPoints[0];
    blackPoints.removeAt(0);
    qDebug() << blackPoints.size() << "个";
  }
  // 添加新的更大的黑色点
  QGraphicsEllipseItem *point = scene->addEllipse(
      x - 10, y - 10, 20, 20, QPen(Qt::black), QBrush(Qt::black));
  blackPoints.append(point);
  qDebug() << blackPoints.size() << "个";
}

void GraphicsDisplay::PaintForAddSitePage(QVector<Node> nodes) {
  qDebug() << "hey";
  bluePoints.clear();
  for (auto it : nodes) {
    QGraphicsEllipseItem *point = nullptr;
    if (it.info_valid) {
      // 是一个景点
      point = scene->addEllipse(it.pos_x - 5, it.pos_y - 5, 10, 10,
                                QPen(Qt::red), QBrush(Qt::red));
    } else {
      // 普通节点
      point = scene->addEllipse(it.pos_x - 5, it.pos_y - 5, 10, 10,
                                QPen(Qt::yellow), QBrush(Qt::yellow));
    }
    bluePoints.append(point);  // 就放在蓝色容器吧，只有add_site_page这么做
    scene->update();
    this->viewport()->update();
  }
}

void GraphicsDisplay::ClearMapPoints() {
  for (auto it : bluePoints) {
    scene->removeItem(it);
    delete it;
  }
  bluePoints.clear();
  scene->update();
  this->viewport()->update();
}

void GraphicsDisplay::PaintForAddEdgePage(
    QVector<QPair<QPair<double, double>, QPair<double, double>>> edges) {
  for (auto edge : edges) {
    double px1 = edge.first.first;
    double py1 = edge.first.second;
    double px2 = edge.second.first;
    double py2 = edge.second.second;
    QGraphicsEllipseItem *point_one = nullptr, *point_two = nullptr;
    QPointF p1(px1, py1), p2(px2, py2);
    point_one = scene->addEllipse(px1 - 5, py1 - 5, 10, 10, QPen(Qt::blue),
                                  QBrush(Qt::blue));
    point_two = scene->addEllipse(px2 - 5, py2 - 5, 10, 10, QPen(Qt::blue),
                                  QBrush(Qt::blue));
    QGraphicsLineItem *line = scene->addLine(QLineF(p1, p2), QPen(Qt::blue, 2));
    bluePoints.append(point_one);
    bluePoints.append(point_two);
    lines.append(line);
    scene->update();
    this->viewport()->update();
    qDebug() << px1 << " " << py1 << " " << px2 << " " << py2;
  }
}

void GraphicsDisplay::PaintBluePoint(Node node) {
  QGraphicsEllipseItem *ptr = scene->addEllipse(
      node.pos_x - 5, node.pos_y - 5, 10, 10, QPen(Qt::blue), QBrush(Qt::blue));
  scene->update();
  this->viewport()->update();
  blackPoints.append(ptr);
}

void GraphicsDisplay::DeletePointOfAddPage() {
  if (!blackPoints.empty()) {
    scene->removeItem(blackPoints[0]);
    delete blackPoints[0];
    blackPoints.clear();
  }
}

void GraphicsDisplay::DeletePointOneOfAddEdgePage() {
  if (!blackPoints.empty()) {
    scene->removeItem(blackPoints[0]);
    delete blackPoints[0];
    blackPoints.removeAt(0);
  }
}

void GraphicsDisplay::DeletePointTwoOfAddEdgePage() {
  if (!blackPoints.empty()) {
    scene->removeItem(blackPoints[1]);
    delete blackPoints[1];
    blackPoints.removeAt(1);
  }
}

void GraphicsDisplay::DeletePointsOfAddEdgePage() {
  if (!blackPoints.empty()) {
    scene->removeItem(blackPoints[0]);
    scene->removeItem(blackPoints[1]);
    delete blackPoints[0];
    delete blackPoints[1];
    blackPoints.clear();
  }
}
