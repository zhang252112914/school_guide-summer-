#include "help_page.h"

#include "ui_help_page.h"

HelpPage::HelpPage(QWidget *parent)
    : QWidget(parent), help_page(new Ui::HelpPage) {
  help_page->setupUi(this);
}

HelpPage::~HelpPage() { delete help_page; }

void HelpPage::on_return_button_clicked() {
  this->hide();
  emit BackToMainPage();
}
