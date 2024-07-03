#include "help_page.h"
#include "ui_help_page.h"

#include <QPushButton>

help_page::help_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::help_page)
{
    ui->setupUi(this);

    connect(ui->return_button, &QPushButton::clicked, [=](){
        emit help_back2_main();
    });
}

help_page::~help_page()
{
    delete ui;
}
