#ifndef HELP_PAGE_H
#define HELP_PAGE_H

#include <QString>
#include <QWidget>

namespace Ui {
class HelpPage;
}

class HelpPage : public QWidget {
  friend class MessageMediator;
  Q_OBJECT

 public:
  explicit HelpPage(QWidget *parent = nullptr);
  ~HelpPage();
  void ShowGif(QString path);
  void CloseAllGif();

 private:
  void UpdateSize();
  Ui::HelpPage *help_page;

 signals:
  void BackToMainPage();
 private slots:
  void on_return_button_clicked();
};

#endif  // HELP_PAGE_H
