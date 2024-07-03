#include "check_page.h"
#include "ui_check_page.h"

#include <QPushButton>

check_page::check_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::check_page)
{
    ui->setupUi(this);

    connect(ui->return_button, &QPushButton::clicked,[=](){
        //这里关键是如何获取主页面的地址，或者说怎么回头
        //通过发消息回去让主页面自己调用函数恢复
        emit this->check_back2_main();
    });
}

check_page::~check_page()
{
    delete ui;
}
