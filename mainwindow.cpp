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
    // 创建并设置 DraggableScrollArea 实例
    DraggableScrollArea *draggableScrollArea = new DraggableScrollArea(this);

    // 创建 appScrollAreaWidget 和布局
    QWidget *appScrollAreaWidget = new QWidget(draggableScrollArea);
    QHBoxLayout *appScrollAreaLayout = new QHBoxLayout(appScrollAreaWidget);

    // 创建并连接 weatherButton
    QPushButton *weatherButton = new QPushButton("Weather", this);
    connect(weatherButton, &QPushButton::clicked, this, &MainWindow::toWeather);
    appScrollAreaLayout->addWidget(weatherButton);

    // 添加20个按钮到布局中
    for (int i = 0; i < 20; ++i) {
        QPushButton *button = new QPushButton(QString("Application %1").arg(i + 1));
        button->setFixedSize(50, 50);  // 设置按钮大小
        appScrollAreaLayout->addWidget(button);
    }

    // 设置布局为 appScrollAreaWidget 的布局
    appScrollAreaWidget->setLayout(appScrollAreaLayout);

    // 将 widget 设置为 DraggableScrollArea 的内容
    draggableScrollArea->setWidget(appScrollAreaWidget);
    draggableScrollArea->setWidgetResizable(true);
    draggableScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 获取原来 appScrollArea 的位置和大小
    QRect originalGeometry = ui->appScrollArea->geometry();

    // 替换 ui 中的原有 appScrollArea
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->centralwidget->layout());
    if (layout) {
        layout->replaceWidget(ui->appScrollArea, draggableScrollArea);
    } else {
        // 如果没有布局，手动设置 geometry
        draggableScrollArea->setGeometry(originalGeometry);
    }

    // 删除原有的 appScrollArea
    delete ui->appScrollArea;

    // 更新指针到新的 DraggableScrollArea
    ui->appScrollArea = draggableScrollArea;
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
