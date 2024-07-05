#include "main_page.h"

#include "ui_main_page.h"

MainPage::MainPage(QWidget *parent)
    : QWidget(parent), main_page(new Ui::MainPage) {
  main_page->setupUi(this);
  setWindowTitle("武汉大学校园导航系统");

  help_page = new HelpPage();
  view_page = new ViewPage();
  manage_page = new ManagePage();

  connect(view_page, &ViewPage::BackToMainPage, this, &MainPage::show);
  connect(manage_page, &ManagePage::BackToMainPage, this, &MainPage::show);
  connect(help_page, &HelpPage::BackToMainPage, this, &MainPage::show);
}

MainPage::~MainPage() { delete main_page; }

/**
 * The following three functions handle button click event.
 * When corresponding button is clicked, main page will hide and subpage will
 * show.
 */
void MainPage::on_view_button_clicked() {
  this->hide();
  view_page->show();
}

void MainPage::on_manage_button_clicked() {
  this->hide();
  manage_page->show();
}

void MainPage::on_help_button_clicked() {
  this->hide();
  help_page->show();
}
