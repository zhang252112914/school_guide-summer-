/********************************************************************************
** Form generated from reading UI file 'manage_page.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGE_PAGE_H
#define UI_MANAGE_PAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_manage_page
{
public:
    QWidget *centralwidget;
    QPushButton *return_button;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *manage_page)
    {
        if (manage_page->objectName().isEmpty())
            manage_page->setObjectName("manage_page");
        manage_page->resize(800, 600);
        centralwidget = new QWidget(manage_page);
        centralwidget->setObjectName("centralwidget");
        return_button = new QPushButton(centralwidget);
        return_button->setObjectName("return_button");
        return_button->setGeometry(QRect(570, 390, 80, 18));
        manage_page->setCentralWidget(centralwidget);
        menubar = new QMenuBar(manage_page);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        manage_page->setMenuBar(menubar);
        statusbar = new QStatusBar(manage_page);
        statusbar->setObjectName("statusbar");
        manage_page->setStatusBar(statusbar);

        retranslateUi(manage_page);

        QMetaObject::connectSlotsByName(manage_page);
    } // setupUi

    void retranslateUi(QMainWindow *manage_page)
    {
        manage_page->setWindowTitle(QCoreApplication::translate("manage_page", "MainWindow", nullptr));
        return_button->setText(QCoreApplication::translate("manage_page", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class manage_page: public Ui_manage_page {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGE_PAGE_H
