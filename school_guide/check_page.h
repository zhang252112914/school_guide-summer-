#ifndef CHECK_PAGE_H
#define CHECK_PAGE_H

#include <QMainWindow>

namespace Ui {
class check_page;
}

class check_page : public QMainWindow
{
    Q_OBJECT

public:
    explicit check_page(QWidget *parent = nullptr);
    ~check_page();

signals:
    void check_back2_main();  //一个信号


private:
    Ui::check_page *ui;
};

#endif // CHECK_PAGE_H
