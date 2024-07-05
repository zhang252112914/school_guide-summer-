#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include <help_page.h>
#include <manage_page.h>
#include <view_page.h>

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

  /**
   * @brief Pointers to subpages within the main application interface.
   *
   * help_page: Points to the help subpage, providing user assistance.
   * manage_page: Points to the manage subpage, enabling management features.
   * view_page: Points to the view subpage, where main content is displayed.
   */
  HelpPage *help_page;
  ManagePage *manage_page;
  ViewPage *view_page;
};
#endif  // MAIN_PAGE_H
