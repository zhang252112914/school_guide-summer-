#ifndef ADD_EDGE_PAGE_H
#define ADD_EDGE_PAGE_H

#include <QMessageBox>
#include <QWidget>

namespace Ui {
class AddEdgePage;
}

class AddEdgePage : public QWidget {
  friend class MessageMediator;
  Q_OBJECT

 public:
  explicit AddEdgePage(QWidget *parent = nullptr);
  ~AddEdgePage();

 private:
  Ui::AddEdgePage *add_edge_page;

 signals:
  void BackToManagePage();
  void AddEdge();
 private slots:
  void on_return_button_clicked();
  void on_confirm_button_clicked();
};

#endif  // ADD_EDGE_PAGE_H
