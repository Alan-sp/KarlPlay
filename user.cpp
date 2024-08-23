#include "user.h"
#include "ui_user.h"

User::User(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::User)
{
    ui->setupUi(this);
    QWidget *parentWidget = parent;

    QPushButton *toSettingButton = ui->toSettingButton;
    connect(toSettingButton, &QPushButton::clicked, this, &User::backSettings);
    QPushButton *toDesktopButton = ui->toDesktopButton;
    connect(toDesktopButton, &QPushButton::clicked, this, &User::backDesktop);

}

User::~User()
{
    delete ui;
}

void User::backSettings(){
    this->hide();
}

void User::backDesktop(){
    this->hide();
    this->parentWidget()->hide();
}
