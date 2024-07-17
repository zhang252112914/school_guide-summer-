QT       += core gui sql \
    quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    campus_map.cpp \
    database_manager.cpp \
    graphics_display.cpp \
    help_page.cpp \
    main.cpp \
    main_page.cpp \
    manage_page.cpp \
    message_mediator.cpp \
    view_page.cpp \
    id_manager.cpp

HEADERS += \
    campus_map.h \
    database_manager.h \
    graphics_display.h \
    help_page.h \
    main_page.h \
    manage_page.h \
    message_mediator.h \
    sender_enum.h \
    types.h \
    view_page.h \
    id_manager.h

FORMS += \
    help_page.ui \
    main_page.ui \
    manage_page.ui \
    view_page.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    ../../../schoolpicture.jpeg
