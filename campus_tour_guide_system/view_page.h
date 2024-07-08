#ifndef VIEW_PAGE_H
#define VIEW_PAGE_H

#include <QWidget>
namespace Ui {
class ViewPage;
}

class ViewPage : public QWidget {
  friend class MessageMediator;
  Q_OBJECT

 public:
  explicit ViewPage(QWidget *parent = nullptr);
  ~ViewPage();

 protected:
  void resizeEvent(QResizeEvent *event) override;
 private slots:
  void on_return_button_clicked();
  void HandlePointClicked(double x, double y);
  void on_addnode_button_clicked();

  void on_clear_button_clicked();

 private:
  Ui::ViewPage *view_page;
  double last_clicked_x, last_clicked_y;

 signals:
  void BackToMainPage();
  void AddNode(double pos_x, double pos_y);
  void MathNode(double pos_x, double pos_y);
};

#endif  // VIEW_PAGE_H
