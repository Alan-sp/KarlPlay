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
    , mapWidgetPage(nullptr)
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

    //导航
    QPushButton *navigateButton = ui->navigateButton;
    QPushButton *navigateHomeButton = ui->navigateHomeButton;
    QPushButton *navigateWorkButton = ui->navigateWorkButton;
    connect(navigateButton, &QPushButton::clicked, this, &MainWindow::toMap);
    connect(navigateHomeButton, &QPushButton::clicked, this, &MainWindow::toMap);
    connect(navigateWorkButton, &QPushButton::clicked, this, &MainWindow::toMap);

    //时间
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    QTextBrowser *timeWidget = ui->timeWidget;
    timeWidget->setText(timeString);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);

    QTextBrowser *systemName = ui->systemName;
    systemName->setText("KarlPlay车载系统\n尊贵的Karl车主，欢迎您！");

    // 创建并初始化 ScrollHome 实例
    ScrollHome *scrollHome = new ScrollHome(this);

    // 初始化 ScrollHome 的图标布局参数
    int iconRow = 1;  // 每列显示的图标数量（即每一行只显示一个图标）
    int iconSpacing = 20;  // 图标之间的间距
    int widgetWidth = 4180;  // 容器宽度
    int widgetHeight = 400;  // 容器高度

    scrollHome->InitHome(iconRow, iconSpacing, widgetWidth, widgetHeight);

    // 添加应用图标按钮到 ScrollHome
    for (int i = 0; i < g_appResSize; ++i) {
        // 创建一个新的应用图标按钮
        QPushButton *appButton = new QPushButton(scrollHome);
        appButton->setText(g_appRes[i].appName);  // 设置按钮文本为应用名称
        appButton->setIcon(QIcon(g_appRes[i].url));  // 设置按钮图标
        appButton->setIconSize(QSize(64, 64));  // 设置图标大小
        appButton->setFixedSize(400, 400);  // 设置按钮大小
        appButton->setStyleSheet("QPushButton { background-color: #2ecc71; color: white; border-radius: 10px; }"
                                 "QPushButton:pressed { background-color: #27ae60; }");

        // 将按钮添加到 ScrollHome 的单元格中
        int row = i % iconRow;
        int column = i / iconRow;
        scrollHome->setCellWidget(row, column, appButton);

        // 设置每列的宽度，以适应按钮和间距
        scrollHome->setColumnWidth(column, 420);

        // 连接按钮的点击信号到后续要处理的应用程序加载逻辑
        connect(appButton, &QPushButton::clicked, [=]() {
            // 在这里处理点击后要执行的应用程序加载逻辑
            qDebug() << "Application " << g_appRes[i].appName << " clicked!";
            // 示例：在这里可以加载应用程序或显示相关内容
            onAppButtonClicked(i);
        });
    }

    // 创建一个 QScrollArea 并将 ScrollHome 设置为其内容
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollHome);
    scrollArea->setWidgetResizable(true);

    // 禁止纵向滚动，只允许横向滚动
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 禁止纵向滚动
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 允许横向滚动

    // 添加手势识别以实现更好的拖动体验
    QScroller *scroller = QScroller::scroller(scrollArea->viewport());
    scroller->grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    // 设置滚动属性以优化滚动效果
    QScrollerProperties properties;
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    properties.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.5);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.5);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.2);
    properties.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.5);

    // 应用滚动属性
    scroller->setScrollerProperties(properties);

    // 获取原来 appScrollArea 的位置和大小
    QRect originalGeometry = ui->appScrollArea->geometry();

    // 替换 ui 中的原有 appScrollArea
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->centralwidget->layout());
    if (layout) {
        layout->replaceWidget(ui->appScrollArea, scrollArea);
    } else {
        // 如果没有布局，手动设置 geometry
        scrollArea->setGeometry(originalGeometry);
    }

    // 删除原有的 appScrollArea
    delete ui->appScrollArea;

    // 更新指针到新的 scrollArea
    scrollArea->setStyleSheet("QScrollArea { background-color: #000000; border: none; }");
    ui->appScrollArea = scrollArea;
    ui->appScrollArea->setStyleSheet(
        "QScrollArea {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0.1, y2:0, stop:0 rgba(0, 0, 0, 0), stop:1 rgba(0, 0, 0, 255)),"
        "                qlineargradient(spread:pad, x1:0.9, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 0));"
        "}"
    );


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

void MainWindow::toMap(){
    mapWidgetPage = new mapWidget(this);
    mapWidgetPage->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAppButtonClicked(int index){
    qDebug() << "Application " << g_appRes[index].appName << " clicked!";
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

void MainWindow::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    QTextBrowser *timeWidget = ui->timeWidget;
    timeWidget->setText(timeString);
}
