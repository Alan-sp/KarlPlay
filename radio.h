#ifndef RADIO_H
#define RADIO_H

#include <QWidget>
#include <QPushButton>
#include "video/mainwindowvideo.h"


namespace Ui {
class Radio;
}

class Radio : public QWidget
{
    Q_OBJECT

public:
    explicit Radio(QWidget *parent = nullptr);
    ~Radio();

public slots:
    void toMain();
    void toVideo();

private:
    Ui::Radio *ui;
    MainWindowVideo *mainWindowVideo;
};

#endif // RADIO_H
