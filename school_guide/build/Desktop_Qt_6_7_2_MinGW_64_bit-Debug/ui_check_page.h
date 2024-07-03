/********************************************************************************
** Form generated from reading UI file 'check_page.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHECK_PAGE_H
#define UI_CHECK_PAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_check_page
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *return_button;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *check_page)
    {
        if (check_page->objectName().isEmpty())
            check_page->setObjectName("check_page");
        check_page->resize(800, 600);
        centralwidget = new QWidget(check_page);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(670, 490, 111, 80));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        return_button = new QPushButton(verticalLayoutWidget);
        return_button->setObjectName("return_button");

        verticalLayout->addWidget(return_button);

        check_page->setCentralWidget(centralwidget);
        menubar = new QMenuBar(check_page);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        check_page->setMenuBar(menubar);
        statusbar = new QStatusBar(check_page);
        statusbar->setObjectName("statusbar");
        check_page->setStatusBar(statusbar);

        retranslateUi(check_page);

        QMetaObject::connectSlotsByName(check_page);
    } // setupUi

    void retranslateUi(QMainWindow *check_page)
    {
        check_page->setWindowTitle(QCoreApplication::translate("check_page", "MainWindow", nullptr));
        return_button->setText(QCoreApplication::translate("check_page", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class check_page: public Ui_check_page {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECK_PAGE_H
