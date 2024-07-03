#include "manage_page.h"
#include "ui_manage_page.h"

#include <QPushButton>

manage_page::manage_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::manage_page)
{
    ui->setupUi(this);
    connect(ui->return_button, &QPushButton::clicked, [=](){
        emit manage_back2_main();
    });
}

manage_page::~manage_page()
{
    delete ui;
}
