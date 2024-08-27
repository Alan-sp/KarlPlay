QT       += core gui sql multimedia network
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp\
    mybtn.cpp\
    mywidget.cpp \
    startbtn.cpp\
    mylab.cpp\
    mylistwidget.cpp\
    sign_in_win.cpp\
    sign_up_win.cpp\
    lrcwidget.cpp\
    mytablewidget.cpp\
    create_list.cpp\
    show_list.cpp\
    lrc_win_main.cpp

HEADERS += \
    mainwindow.h\
    mybtn.h\
    mywidget.h \
    startbtn.h\
    mylab.h\
    mylistwidget.h\
    sign_in_win.h\
    sign_up_win.h\
    lrcwidget.h\
    mytablewidget.h\
    create_list.h\
    show_list.h\
    lrc_win_main.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

CONFIG += resources_big

DISTFILES += \
    ../../../../home/xth/Music/123.mp3 \
    ../../../../home/xth/Music/234.mp3 \
    ../../../../home/xth/Music/345.mp3
