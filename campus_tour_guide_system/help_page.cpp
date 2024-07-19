#include "help_page.h"

#include <QLabel>
#include <QMovie>

#include "ui_help_page.h"

HelpPage::HelpPage(QWidget *parent)
    : QWidget(parent), help_page(new Ui::HelpPage) {
  help_page->setupUi(this);

  // 连接用于触发gif展示的信号
  // 需要统一gif的尺寸以保持美观
  connect(help_page->func_show1, &QPushButton::clicked, this,
          [=]() { ShowGif(":/1.gif"); });

  connect(help_page->func_show2, &QPushButton::clicked, this,
          [=]() { ShowGif(":/2.gif"); });

  connect(help_page->func_show3, &QPushButton::clicked, this,
          [=]() { ShowGif(":/3.gif"); });

  connect(help_page->func_show4, &QPushButton::clicked, this,
          [=]() { ShowGif(":/3.gif"); });

  connect(help_page->func_show5, &QPushButton::clicked, this,
          [=]() { ShowGif(""); });
}

HelpPage::~HelpPage() { delete help_page; }

void HelpPage::on_return_button_clicked() {
  this->hide();
  emit BackToMainPage();
}

void HelpPage::ShowGif(QString path) {
  QLabel *gif_label = (help_page->gif_show_label);
  QMovie *gif_movie = new QMovie(path);
  gif_label->setMovie(gif_movie);
  gif_movie->start();
  gif_label->show();
}

void HelpPage::CloseAllGif() { help_page->gif_show_label->close(); }
