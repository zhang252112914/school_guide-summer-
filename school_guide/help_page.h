#ifndef HELP_PAGE_H
#define HELP_PAGE_H

#include <QMainWindow>


namespace Ui {
class help_page;
}

class help_page : public QMainWindow
{
    Q_OBJECT

public:
    explicit help_page(QWidget *parent = nullptr);
    ~help_page();

signals:
    void help_back2_main();

private:
    Ui::help_page *ui;
};

#endif // HELP_PAGE_H
