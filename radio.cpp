#include "radio.h"
#include "ui_radio.h"

Radio::Radio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Radio)
  , mainWindowVideo(nullptr)
{
    ui->setupUi(this);

    QPushButton *backButton = ui->backButton;
    connect(backButton, &QPushButton::clicked, this, &Radio::toMain);
    QPushButton *videoButton = ui->videoButton;
    connect(videoButton, &QPushButton::clicked, this, &Radio::toVideo);
}

Radio::~Radio()
{
    delete ui;
}

void Radio::toMain(){
    this->hide();
}

void Radio::toVideo(){
    if(!mainWindowVideo){
        mainWindowVideo = new MainWindowVideo(this);
    }
    QRect parentGeometry = this->geometry();
    int x = parentGeometry.x() + (parentGeometry.width() - mainWindowVideo->width());
    int y = parentGeometry.y() + (parentGeometry.height() - mainWindowVideo->height());
    mainWindowVideo->move(x, y);
    mainWindowVideo->show();
}
