#include "graphics_display.h"

#include <QMouseEvent>
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
  scene->setSceneRect(map_pixmap.rect());

  // Ensure the whole image is visible
  this->fitInView(map_item, Qt::KeepAspectRatio);

  // Optionally, disable scrollbars if you want the view to be fixed
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QGraphicsScene *GraphicsDisplay::GetScene() const { return scene; }

void GraphicsDisplay::AddPoint(int x, int y) {
  qDebug() << "points.size() = " << points.size();

  // Create a red ellipse to mark the point
  QGraphicsEllipseItem *point =
      scene->addEllipse(x - 5, y - 5, 10, 10, QPen(Qt::red), QBrush(Qt::red));
  points.append(point);
}

void GraphicsDisplay::mousePressEvent(QMouseEvent *event) {
  QPointF scene_pos = this->mapToScene(event->pos());
  double x = scene_pos.x();
  double y = scene_pos.y();

  if (scene->sceneRect().contains(scene_pos)) {
    AddPoint(x, y);
    emit PointClicked(x, y);  // 发送坐标信号
  }
}
