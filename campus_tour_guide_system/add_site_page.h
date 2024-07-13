#ifndef ADD_SITE_PAGE_H
#define ADD_SITE_PAGE_H

#include <QMessageBox>
#include <QWidget>

#include "graphics_display.h"

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

 signals:
  void BackToManagePage();
  void AddNode();
 private slots:
  void on_return_button_clicked();
  void on_confirm_button_clicked();
};

#endif  // ADD_SITE_PAGE_H
