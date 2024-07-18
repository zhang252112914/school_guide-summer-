#ifndef MANAGE_PAGE_H
#define MANAGE_PAGE_H

#include <add_edge_page.h>
#include <add_site_page.h>

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
  void on_add_site_button_clicked();
  void on_add_edge_button_clicked();

 private:
  Ui::ManagePage *manage_page;

 signals:
  void BackToMainPage();
  void ShowAddEdgePage();
  void ShowAddSitePage();
};

#endif  // MANAGE_PAGE_H
