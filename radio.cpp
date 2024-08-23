#include "radio.h"
#include "ui_radio.h"

Radio::Radio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Radio)
{
    ui->setupUi(this);

    QPushButton *backButton = ui->backButton;
    connect(backButton, &QPushButton::clicked, this, &Radio::toMain);
}

Radio::~Radio()
{
    delete ui;
}

void Radio::toMain(){
    this->hide();
}
