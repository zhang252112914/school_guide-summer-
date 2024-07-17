#ifndef VIEW_PAGE_H
#define VIEW_PAGE_H

#include <QByteArray>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPair>
#include <QString>
#include <QTimer>
#include <QVector>
#include <QWidget>

#include "graphics_display.h"
#include "sender_enum.h"
#include "types.h"

namespace Ui {
class ViewPage;
}

class ViewPage : public QWidget {
  friend class MessageMediator;
  Q_OBJECT

 public:
  explicit ViewPage(QWidget *parent = nullptr);
  ~ViewPage();
  void DisplayInfo(const Info &info, const QByteArray &image_data,
                   Sender sender);

 protected:
  void resizeEvent(QResizeEvent *event) override;
 private slots:
  void on_return_button_clicked();
  void HandlePointClicked(double x, double y);
  void on_addnode_button_clicked();
  void on_clear_button_clicked();
  // 接受查询的结果并调用后端的查询函数
  void IdsReceiverAndFindCaller(const Node &node, Sender sender);
  void HandleSitesFound(QVector<QPair<QPair<double, double>, QString>> sites,
                        Sender sender);
  void IdsReceiver(const Node &node, Sender sender);
  void HandlePathVector(QVector<QPair<double, double>> route);
  void OnNodeNotFound(Sender sender);

 private:
  Ui::ViewPage *view_page;
  double last_clicked_x, last_clicked_y;
  double pre_clicked_x, pre_clicked_y;
  // 由于查询会有两个信号先后返回，所以要进行标记
  bool first_arrive = false, second_arrive = false;
  bool point_arrive = false;
  int point_id;
  int first_arrived_id, second_arrived_id;
  GraphicsDisplay *graphics_display;

 signals:
  void BackToMainPage();
  void AddNode(double pos_x, double pos_y);
  void MathNode(double pos_x, double pos_y);
  // 用于向campusmap发送查询请求，由相应的槽函数进行接受
  void IdRequest(double pos_x, double pos_y, Sender sender);
  void CallFindPath(int start, int end);
  void RequestSites(Sender sender);  // 用于请求站点信息的信号
  void MyIdRequest(double pos_x, double pos_y, Sender sender);
  void MyInfoRequest(int info_id, Sender sender);
};

#endif  // VIEW_PAGE_H
