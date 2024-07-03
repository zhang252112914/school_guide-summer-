QT       += core gui

QT  +=  sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    check_page.cpp \
    help_page.cpp \
    main.cpp \
    mainwindow.cpp \
    manage_page.cpp \
    serialization.cpp

HEADERS += \
    check_page.h \
    help_page.h \
    mainwindow.h \
    manage_page.h \
    my_map.h \
    serialization.h

FORMS += \
    check_page.ui \
    help_page.ui \
    mainwindow.ui \
    manage_page.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
