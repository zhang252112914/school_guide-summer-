#ifndef INFO_PAGE_H
#define INFO_PAGE_H

#include <QWidget>

#include "sender_enum.h"
#include "types.h"
#include "ui_inform.h"
namespace Ui {
class InfoPage;
}

class InfoPage : public QWidget {
  Q_OBJECT

 public:
  explicit InfoPage(QWidget *parent = nullptr);
  void displayInfo(const Info &info, Sender sender);
  ~InfoPage();

 private:
  Ui::InfoPage *ui;
};

#endif  // INFO_PAGE_H
