#ifndef ADD_SITE_PAGE_H
#define ADD_SITE_PAGE_H

#include <QBuffer>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QTransform>
#include <QVector>
#include <QWidget>

#include "graphics_display.h"
#include "sender_enum.h"
#include "types.h"

//现在还有几个重要的点没有实现：1、照片信息的传递
//现在需要rebase，后面还要在这个基础上进一步完善添加信息的内容

//这里有一个比较麻烦的问题，就是构造函数不能发出信号，所以绘图的信号只能由前一个页面发出调用请求的时候一并发出
//我的选择是将show信号一并绑定

// add_site_page很大程度上需要依靠graphicsdisplay，但是原有的一些函数并不能满足所有要求
//所以我在不改变原有数据成员的基础上（尽量利用原有的储存结构），通过添加一些函数满足要求
//所以导致有的数据成员名字和功能不是很匹配，所以在这里做一下记录
// blackPoints:用于存放唯一一个点的point*（画图用的）
// single_node:专门用于add_site_page保存唯一一个点信息的地方

namespace Ui {
class AddSitePage;
}

class AddSitePage : public QWidget {
  friend class MessageMediator;
  Q_OBJECT

 public:
  explicit AddSitePage(QWidget *parent = nullptr);
  ~AddSitePage();

  QByteArray compressImage(const QImage &image);

 private:
  Ui::AddSitePage *add_site_page;
  GraphicsDisplay *graph;
  void receive_one_node(double x, double y);
  double pos_x, pos_y;
  Node single_node;
  QByteArray image_data_container;
  bool image_changed;

 signals:
  void BackToManagePage();
  void AddNode();
  //根据获取到的信息及时更新add_site_page中的id，时刻为信息发送做准备
  void CoordinateToNode(double x, double y, Sender s = Sender::MANAGE_PAGE);
  //暂时没有考虑具体的照片传递方式，后面可以继续修改
  //这个函数用于向campusmap发送信息集合
  void AddMessageCollection(int id, QString name, QString description,
                            QByteArray image);
  void EditMessageCollection(Info new_info, QByteArray image_data, UpdateFlags);

  //每次在初始化的时候都会发出绘制的请求
  void PaintRequest();
  //请求展示已有景点信息
  void PresentInfoRequest(int id, Sender sender = Sender::MANAGE_PAGE);

 private slots:
  //接收返回的id
  void ModifyNode(Node back_node, Sender sender);
  void InvalidNode(Sender sender);
  void on_return_button_clicked();
  //点击按钮后收集页面上的所有信息并形成集合借助上面的信号进行发送
  void on_confirm_button_clicked();
  void on_cancel_button_clicked();
  void on_add_picture_button_clicked();

  void PaintRequestWrapper();

  //接受campus_map传递过来的节点集，然后在背景图上标记处相应的节点
  void PaintMap(QVector<Node> nodes);

  //如果是已有景点那么应该展示已有信息辅助用户修改
  void PresentInfo(const Info &info, const QByteArray &image_data,
                   Sender sender);
};

#endif  // ADD_SITE_PAGE_H
