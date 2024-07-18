#ifndef ADD_EDGE_PAGE_H
#define ADD_EDGE_PAGE_H

#include <QMessageBox>
#include <QTimer>
#include <QWidget>

#include "graphics_display.h"
#include "sender_enum.h"

namespace Ui {
class AddEdgePage;
}

class AddEdgePage : public QWidget {
  friend class MessageMediator;
  Q_OBJECT

 public:
  explicit AddEdgePage(QWidget *parent = nullptr);
  ~AddEdgePage();

 private:
  Ui::AddEdgePage *add_edge_page;
  GraphicsDisplay *graph;
  double x1, y1, x2, y2;
  double px1, py1, px2, py2;
  // Node n1, n2;
  // Edge edge;
  QVector<QPair<double, double>> nodes;
  // QVector<QPair<QPair<double, double>, QPair<double, double>>> edges;
  //  判断已经点击了多少点
  bool n1_clicked = 0, n2_clicked = 0;
  bool n2_clicked_mult = 0;
  // 判断点是否已经存在
  bool n1_exist = 0, n2_exist = 0;

 signals:
  void BackToManagePage();
  // 回传寻找是否为已有点（点击范围）
  void FindNode(double x, double y, Sender s = Sender::MANAGE_PAGE);
  void NewEdge(const QPair<double, double> &start,
               const QPair<double, double> &end);
  void NewNode(double x, double y);
  void GetEdges(Sender s);

 private slots:
  // 获取鼠标点击点位
  void receive_one_node(double x, double y);
  // 确定点的位置（点击范围）
  // 只有确定鼠标所点击的点为已有点才触发
  void ConfirmNode(Node back_node, Sender s);

  void on_return_button_clicked();
  void on_confirm_button_clicked();
  void on_cancel_button_clicked();

  void PaintEdges(
      QVector<QPair<QPair<double, double>, QPair<double, double>>> edges);
};

#endif  // ADD_EDGE_PAGE_H
