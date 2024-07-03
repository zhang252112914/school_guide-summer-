/********************************************************************************
** Form generated from reading UI file 'help_page.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELP_PAGE_H
#define UI_HELP_PAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_help_page
{
public:
    QWidget *centralwidget;
    QPushButton *return_button;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *help_page)
    {
        if (help_page->objectName().isEmpty())
            help_page->setObjectName("help_page");
        help_page->resize(800, 600);
        centralwidget = new QWidget(help_page);
        centralwidget->setObjectName("centralwidget");
        return_button = new QPushButton(centralwidget);
        return_button->setObjectName("return_button");
        return_button->setGeometry(QRect(650, 470, 80, 18));
        help_page->setCentralWidget(centralwidget);
        menubar = new QMenuBar(help_page);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        help_page->setMenuBar(menubar);
        statusbar = new QStatusBar(help_page);
        statusbar->setObjectName("statusbar");
        help_page->setStatusBar(statusbar);

        retranslateUi(help_page);

        QMetaObject::connectSlotsByName(help_page);
    } // setupUi

    void retranslateUi(QMainWindow *help_page)
    {
        help_page->setWindowTitle(QCoreApplication::translate("help_page", "MainWindow", nullptr));
        return_button->setText(QCoreApplication::translate("help_page", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class help_page: public Ui_help_page {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_PAGE_H
