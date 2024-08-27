QT       += core gui network
QT       += sql
QT       += multimedia multimediawidgets

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
    draggablescrollarea.cpp \
    helpwindow.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    radio.cpp \
    settingswidget.cpp \
    signin.cpp \
    user.cpp \
    video/mainwindowvideo.cpp \
    weather/mainwindowweather.cpp

HEADERS += \
    draggablescrollarea.h \
    helpwindow.h \
    login.h \
    mainwindow.h \
    radio.h \
    settingswidget.h \
    signin.h \
    user.h \
    video/mainwindowvideo.h \
    weather/mainwindowweather.h

FORMS += \
    draggablescrollarea.ui \
    helpwindow.ui \
    login.ui \
    mainwindow.ui \
    radio.ui \
    settingswidget.ui \
    signin.ui \
    user.ui \
    video/mainwindowvideo.ui \
    weather/mainwindowweather.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    video/images.qrc \
    weather/mainwindow.qrc

DISTFILES += \
    video/images/K-Lite_Codec_Pack_1850_Basic.exe \
    video/images/angle-double-small-right.png \
    video/images/angle-left.png \
    video/images/angle-right.png \
    video/images/angle-small-right.png \
    video/images/folder-open.png \
    video/images/forward-fast.png \
    video/images/forward.png \
    video/images/home.png \
    video/images/pause.png \
    video/images/play.png \
    video/images/rewind-button-circle.png \
    video/images/rewind.png \
    video/images/volume-down.png \
    video/images/volume-mute.png \
    video/images/wifi.png \
    weather/weather.png
