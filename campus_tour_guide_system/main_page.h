#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainPage;
}
QT_END_NAMESPACE

/**
 * @brief The MainPage class handles main page display and addresses subpages's
 * switch.
 */

class MainPage : public QWidget {
  friend class MessageMediator;
  Q_OBJECT

 public:
  MainPage(QWidget *parent = nullptr);
  ~MainPage();

 private slots:
  /**
   * The following functions are used to open subpages.
   * When the corresponding button is clicked, the main page will hide, and the
   * appropriate subpage will show.
   */
  void on_view_button_clicked();
  void on_manage_button_clicked();
  void on_help_button_clicked();

 private:
  Ui::MainPage *main_page;
 signals:
  void ShowViewPage();
  void ShowManagePage();
  void ShowHelpPage();
};
#endif  // MAIN_PAGE_H
