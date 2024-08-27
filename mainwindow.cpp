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
    , mainWindowWeatherPage(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    UserInfo.userID = 0;
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    createDatabaseAndUserTable();
    fetchUserInfo(0, UserInfo);

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
    QTextBrowser *userBrowser = ui->userWidget;
    userBrowser->setText("Welcome, " + UserInfo.username + "!");

    //app滑动窗口
    QWidget *appScrollAreaWidget = ui->appScrollAreaContent;
    QHBoxLayout *appScrollAreaLayout = new QHBoxLayout(appScrollAreaWidget);

    //app按钮列表
    QPushButton *weatherButton = new QPushButton(this);
    connect(weatherButton, &QPushButton::clicked, this, &MainWindow::toWeather);
    appScrollAreaLayout->addWidget(weatherButton);


    for (int i = 0; i < 20; ++i) {
        QPushButton *button = new QPushButton(QString("Application %1").arg(i + 1));
        button->resize(50,50);
        appScrollAreaLayout->addWidget(button);
    }
    appScrollAreaWidget->setLayout(appScrollAreaLayout);
    appScrollAreaWidget->setMinimumSize(appScrollAreaWidget->sizeHint());
    QScrollArea *appScrollArea = ui->appScrollArea;
    appScrollArea->setWidgetResizable(true);
    appScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


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
    connect(newSettingsWidget, &SettingsWidget::refreshMainpage, this, &MainWindow::refreshPage);
}

void MainWindow::toWeather(){
    mainWindowWeatherPage = new MainWindowWeather(this);
    mainWindowWeatherPage->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::fetchUserInfo(int ID, struct UserInfo &userInfo) {
    // 创建并打开数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./KarlPlay.db");

    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    QString queryString = "SELECT id, username, password, phone, favoriteTemp, favoriteStyle FROM user WHERE id = :userid";

    query.prepare(queryString);
    query.bindValue(":userid", ID);  // 绑定参数

    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
        query.finish();
        db.close();
        return false;
    }

    if (query.next()) {
        // 提取数据
        userInfo.userID = query.value(0).toInt();
        userInfo.username = query.value(1).toString();
        userInfo.password = query.value(2).toString();
        userInfo.phone = query.value(3).toString();
        userInfo.favoriteTemp = query.value(4).toFloat();
        userInfo.favoriteStyle = query.value(5).toInt();

        qDebug() << "User found on Main:";
        qDebug() << "UserID:" << userInfo.userID;
        qDebug() << "Username:" << userInfo.username;
        qDebug() << "Password:" << userInfo.password;
        qDebug() << "Phone:" << userInfo.phone;
        qDebug() << "Favorite Temperature:" << userInfo.favoriteTemp;
        qDebug() << "Favorite Style:" << userInfo.favoriteStyle;
        query.finish();
        db.close();
        return true;
    } else {
        qDebug() << "No user found with UserID:" << ID;
        query.finish();
        db.close();
        return false;
    }
}

void MainWindow::refreshPage(){
    qDebug() << "信号已接收";
    ui->userWidget->setText("Welcome, WangYuxuan!");
    this->update();
}
