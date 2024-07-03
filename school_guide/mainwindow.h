#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#include "check_page.h"   //包含头文件
#include "manage_page.h"
#include "help_page.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent* event);

    check_page *cp = nullptr;   //用于保存子页面的指针
    manage_page *mp = nullptr;
    help_page *hp = nullptr;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
