#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "check_page.h"

#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->cp = new check_page;  //实例化子页面

    connect(ui->check_button, &QPushButton::clicked, [=](){  //连接两个界面，并定义连接时候二者的行为，以此达到切换窗口的作用
        this->cp->show();
        this->hide();  //隐藏主窗口
    });

    //和地图子页面建立返回的联系
    connect(this->cp, &check_page::check_back2_main, [=](){
        this->show();
        this->cp->hide();
    });

    this->mp = new manage_page();
    connect(ui->manage_button, &QPushButton::clicked, [=](){
        this->cp->show();
        this->hide();
    });
    connect(this->mp, &manage_page::manage_back2_main, [=](){
        this->show();
        this->cp->hide();
    });

    this->hp = new help_page();
    connect(ui->help_button, &QPushButton::clicked, [=](){
        this->hp->show();
        this->hide();
    });
    connect(this->hp, &help_page::help_back2_main, [=](){
        this->show();
        this->hp->hide();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(), QPixmap(":/R-C.jpg"));
}
