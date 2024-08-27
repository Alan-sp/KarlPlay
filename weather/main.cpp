#include "mainwindowweather.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindowWeather w;
    w.show();
    return a.exec();
}
