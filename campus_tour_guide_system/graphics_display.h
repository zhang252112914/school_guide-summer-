#ifndef GRAPHICS_DISPLAY_H
#define GRAPHICS_DISPLAY_H

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPair>
#include <QString>
#include <QVector>

#include "campus_map.h"
class GraphicsDisplay : public QGraphicsView {
  Q_OBJECT

 public:
  explicit GraphicsDisplay(QWidget *parent = nullptr);
  QGraphicsScene *GetScene() const;
  void FitViewToScene();
  CampusMap *campus_map;
  GraphicsDisplay *my_graphics;

  void ClearMapPoints();

  // 公共方法来添加点和连接点
  void AddPoint(int x, int y, const QString &labelText);
  void ConnectPoints();  // 新增方法来绘制线段连接点
  void AddLine(QPointF p1, QPointF p2);
  void ClearPoints();                      // 清除点和线的方法
  void AddPoint(double x, double y);       // 重载，不需要标签
  void AddBlackPoint(double x, double y);  // 添加黑色点的方法
  void ClearBluePoints();
  void ClearRedPoints();
  void resizeEvent(QResizeEvent *event) override;

  // 专为addsitepage绘制节点的函数（用颜色区分普通节点和景点）
  void PaintForAddSitePage(QVector<Node> nodes);
  void PaintForAddEdgePage(
      QVector<QPair<QPair<double, double>, QPair<double, double>>> edges);
  void PaintBluePoint(Node node);
  void DeletePointOfAddPage();
  void DeletePointOneOfAddEdgePage();
  void DeletePointTwoOfAddEdgePage();
  void DeletePointsOfAddEdgePage();

 public slots:
  void DisplayPoint(double x, double y, bool matched);

 private:
  QGraphicsScene *scene;
  QGraphicsPixmapItem *map_item;
  QVector<QGraphicsEllipseItem *> points;       // 存储点的容器
  QVector<QGraphicsTextItem *> labels;          // 存储标签的容器
  QVector<QGraphicsLineItem *> lines;           // 新增向量来存储线段
  QVector<QGraphicsEllipseItem *> blackPoints;  // 保存黑色点的容器
  int maxBlackPoints = 2;
  QVector<QGraphicsEllipseItem *> bluePoints;  // 存储蓝色点

 protected:
  void mousePressEvent(QMouseEvent *event) override;

 signals:
  void MathNode(double pos_x, double pos_y);
  void PointClicked(double x, double y);  // 发送点击位置的信号
};

#endif  // GRAPHICS_DISPLAY_H
