#ifndef ADD_SITE_PAGE_H
#define ADD_SITE_PAGE_H

#include <QMessageBox>
#include <QWidget>

#include "graphics_display.h"
#include "sender_enum.h"

//现在还有几个重要的点没有实现：1、照片信息的传递
// 2、是否应该给用户显示已存在的node和site

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
  int id;

 signals:
  void BackToManagePage();
  void AddNode();
  //根据获取到的信息及时更新add_site_page中的id，时刻为信息发送做准备
  void TurnCoordinateToId(double x, double y, Sender s = Sender::MANAGE_PAGE);
  //暂时没有考虑具体的照片传递方式，后面可以继续修改
  //这个函数用于向campusmap发送信息集合
  void MessageCollection(int id, QString name, QString info, QString pic_path);
 private slots:
  //接收返回的id
  void ModifyId(int back_id, Sender sender);
  void InvalidId(Sender sender);
  void on_return_button_clicked();
  //点击按钮后收集页面上的所有信息并形成集合借助上面的信号进行发送
  void on_confirm_button_clicked();
};

#endif  // ADD_SITE_PAGE_H
