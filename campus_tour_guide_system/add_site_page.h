#ifndef ADD_SITE_PAGE_H
#define ADD_SITE_PAGE_H

#include <QMessageBox>
#include <QVector>
#include <QWidget>

#include "graphics_display.h"
#include "sender_enum.h"
#include "types.h"

//现在还有几个重要的点没有实现：1、照片信息的传递
//现在需要rebase，后面还要在这个基础上进一步完善添加信息的内容

//这里有一个比较麻烦的问题，就是构造函数不能发出信号，所以绘图的信号只能由前一个页面发出调用请求的时候一并发出
//我的选择是将show信号一并绑定

namespace Ui {
class AddSitePage;
}

class AddSitePage : public QWidget {
  friend class MessageMediator;
  Q_OBJECT

 public:
  explicit AddSitePage(QWidget *parent = nullptr);
  ~AddSitePage();

 private:
  Ui::AddSitePage *add_site_page;
  GraphicsDisplay *graph;
  void receive_one_node(double x, double y);
  double pos_x, pos_y;
  Node single_node;

 signals:
  void BackToManagePage();
  void AddNode();
  //根据获取到的信息及时更新add_site_page中的id，时刻为信息发送做准备
  void CoordinateToNode(double x, double y, Sender s = Sender::MANAGE_PAGE);
  //暂时没有考虑具体的照片传递方式，后面可以继续修改
  //这个函数用于向campusmap发送信息集合
  void MessageCollection(Node node, QString name, QString info,
                         QString pic_path);

  //每次在初始化的时候都会发出绘制的请求
  void PaintRequest();

 private slots:
  //接收返回的id
  void ModifyNode(Node back_node, Sender sender);
  void InvalidNode(Sender sender);
  void on_return_button_clicked();
  //点击按钮后收集页面上的所有信息并形成集合借助上面的信号进行发送
  void on_confirm_button_clicked();

  void PaintRequestWrapper();

  //接受campus_map传递过来的节点集，然后在背景图上标记处相应的节点
  void PaintMap(QVector<Node> nodes);
};

#endif  // ADD_SITE_PAGE_H
