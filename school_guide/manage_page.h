#ifndef MANAGE_PAGE_H
#define MANAGE_PAGE_H

#include <QMainWindow>

namespace Ui {
class manage_page;
}

class manage_page : public QMainWindow
{
    Q_OBJECT

public:
    explicit manage_page(QWidget *parent = nullptr);
    ~manage_page();

signals:
    void manage_back2_main();

private:
    Ui::manage_page *ui;
};

#endif // MANAGE_PAGE_H
