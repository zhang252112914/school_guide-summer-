#ifndef GRAPHICS_DISPLAY_H
#define GRAPHICS_DISPLAY_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QVector>

class GraphicsDisplay : public QGraphicsView {
  Q_OBJECT

 public:
  explicit GraphicsDisplay(QWidget *parent = nullptr);
  QGraphicsScene *GetScene() const;

  // 公共方法来添加点和连接点
  void AddPoint(int x, int y);

 private:
  QGraphicsScene *scene;
  QGraphicsPixmapItem *map_item;
  QVector<QGraphicsEllipseItem *> points;  // 存储点的容器
  QVector<QGraphicsTextItem *> labels;     // 存储标签的容器
  QVector<QGraphicsLineItem *> lines;      // 新增向量来存储线段

 protected:
  void mousePressEvent(QMouseEvent *event) override;

 signals:
  void PointClicked(double x, double y);  // 发送点击位置的信号
};

#endif  // GRAPHICS_DISPLAY_H
