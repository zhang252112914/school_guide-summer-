#ifndef MANAGE_PAGE_H
#define MANAGE_PAGE_H

#include <QWidget>

namespace Ui {
class ManagePage;
}

class ManagePage : public QWidget {
  friend class MessageMediator;
  Q_OBJECT

 public:
  explicit ManagePage(QWidget *parent = nullptr);
  ~ManagePage();

 private slots:
  void on_return_button_clicked();

 private:
  Ui::ManagePage *manage_page;

 signals:
  void BackToMainPage();
};

#endif  // MANAGE_PAGE_H
