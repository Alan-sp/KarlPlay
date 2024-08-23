#include <QDesktopWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , newHelpWindow(nullptr)  // 初始化指针为 nullptr
    , newSettingsWidget(nullptr)  // 初始化指针为 nullptr
    , newMediaWidget(nullptr)
    , newUserWidget(nullptr)
{
    ui->setupUi(this);

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    //紧急求助按钮
    QPushButton *helpButtonLeft = ui->helpButtonLeft;
    QPushButton *helpButtonRight = ui->helpButtonRight;
    connect(helpButtonLeft, &QPushButton::clicked, this, &MainWindow::toHelp);
    connect(helpButtonRight, &QPushButton::clicked, this, &MainWindow::toHelp);

    //设置
    QPushButton *settingsButton = ui->settingsButton;
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::toSettings);

    //媒体
    QPushButton *mediaButton = ui->mediaButton;
    connect(mediaButton, &QPushButton::clicked, this, &MainWindow::toMedia);

    //个人中心
    QPushButton *userButton = ui->userButton;
    connect(userButton, &QPushButton::clicked, this, &MainWindow::toUser);

}

void MainWindow::toHelp(){
    newHelpWindow = new helpWindow();  // 创建 HelpWindow 实例
    newHelpWindow->exec();
}

void MainWindow::toSettings(){
    newSettingsWidget = new SettingsWidget(this);
    newSettingsWidget->show();
}

//这个的类叫Radio，命名错了改太麻烦了，带.ui
void MainWindow::toMedia(){
    newMediaWidget = new Radio(this);
    newMediaWidget->show();
}

void MainWindow::toUser() {
    newSettingsWidget = new SettingsWidget(this);
    newSettingsWidget->show();
    newSettingsWidget->toUser();
}

MainWindow::~MainWindow()
{
    delete ui;
}

