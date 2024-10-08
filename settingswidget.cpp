#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent)
  , newUserWidget(nullptr)
  , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    QPushButton *backButton = ui->backButton;
    connect(backButton, &QPushButton::clicked,this, &SettingsWidget::backMain);

    //个人中心
    QPushButton *userButton = ui->userButton;
    connect(userButton, &QPushButton::clicked, this, &SettingsWidget::toUser);

}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::backMain(){
    qDebug() << "按钮按下";
    emit refreshMainpage();
    this->hide();
}

void SettingsWidget::toUser(){
    if(!newUserWidget){
        newUserWidget = new User(this);
    }
    newUserWidget->show();
}
