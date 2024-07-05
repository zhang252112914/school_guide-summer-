#ifndef VIEW_PAGE_H
#define VIEW_PAGE_H

#include <QWidget>

namespace Ui {
class ViewPage;
}

class ViewPage : public QWidget {
  Q_OBJECT

 public:
  explicit ViewPage(QWidget *parent = nullptr);
  ~ViewPage();

 private slots:
  void on_return_button_clicked();

 private:
  Ui::ViewPage *view_page;

 signals:
  void BackToMainPage();
};

#endif  // VIEW_PAGE_H
