#include "user.h"
#include "ui_user.h"

User::User(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::User)
  , signInWidget(nullptr)
  , logInWidget(nullptr)
{
    ui->setupUi(this);

    QPushButton *toSettingButton = ui->toSettingButton;
    connect(toSettingButton, &QPushButton::clicked, this, &User::backSettings);
    QPushButton *toDesktopButton = ui->toDesktopButton;
    connect(toDesktopButton, &QPushButton::clicked, this, &User::backDesktop);
    QPushButton *logInButton = ui->logInButton;
    connect(logInButton, &QPushButton::clicked, this, &User::toLogIn);
    QPushButton *signInButton = ui->signInButton;
    connect(signInButton, &QPushButton::clicked, this, &User::toSignIn);

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
void User::toLogIn(){
    if(!logInWidget){
        logInWidget = new LogIn(this);
    }
    logInWidget->exec();
}
void User::toSignIn(){
    if(!signInWidget){
        signInWidget = new SignIn(this);
    }
    signInWidget->exec();
}

void createDatabaseAndUserTable() {
    // 检查数据库文件是否存在
    QString dbPath = "./KarlPlay.db";
    if (!QFile::exists(dbPath)) {
        qDebug() << "Database file does not exist. Creating a new one.";
    } else {
        qDebug() << "Database file already exists.";
    }

    // 创建并打开数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Failed to open or create database:" << db.lastError().text();
        return;
    }

    qDebug() << "Database opened or created successfully.";

    // 创建user表的SQL语句
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS user (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,
            password TEXT NOT NULL,
            phone TEXT NOT NULL,
            favoriteTemp FLOAT,
            favoriteStyle INTEGER
        )
    )";
    QString insertFirstLine = QString(
        "INSERT INTO user (id, username, password, phone, favoriteTemp, favoriteStyle) "
        "VALUES (0, 'please log in', 'NULL', 'NULL', 26, 0)"
    );
    QSqlQuery createInit(db);
    createInit.exec(createTableQuery);
    QSqlQuery addFirstLine(db);
    addFirstLine.exec(insertFirstLine);
    createInit.finish();
    addFirstLine.finish();
    // 关闭数据库连接
    db.close();
}




