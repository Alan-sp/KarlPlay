#include "mainwindowvideo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindowVideo w;
    w.show();
    return a.exec();
}
