#include "login.h"
#include "ui_login.h"

LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    QPushButton *logInButton = ui->logInButton;
    connect(logInButton, &QPushButton::clicked, this, &LogIn::logIn);
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::logIn(){
    username = ui->usernameEdit->text();
    password = ui->passwordEdit->text();
    qDebug() << username << password;
    checkLogin(username, password);
}

void LogIn::checkLogin(const QString &username, const QString &password) {
    // 连接到SQLite数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./KarlPlay.db");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to the database.");
        return;
    } else {
        qDebug() << "Database connected successfully.";
    }

    // 创建查询对象
    QSqlQuery logInQuery(db);
    logInQuery.prepare("SELECT * FROM user WHERE username = :username AND password = :password");
    logInQuery.bindValue(":username", username);
    logInQuery.bindValue(":password", password);

    // 执行查询
    if (logInQuery.exec()) {
        if (logInQuery.next()) { // 如果查询结果不为空，说明用户名和密码匹配
            QSqlRecord record = logInQuery.record();
            for (int i = 0; i < record.count(); ++i) {
                qDebug() << record.fieldName(i) << ": " << record.value(i).toString();
            }
            QMessageBox::information(nullptr, "Login Success", "Login successful!");
            this->close();
            this->clean();
        } else {
            QMessageBox::warning(nullptr, "Login Failed", "Username or password is incorrect.");
        }
    } else {
        QMessageBox::critical(nullptr, "Query Error", "Failed to execute the query.");
        qDebug() << "SQL Error:" << logInQuery.lastError().text();
        qDebug() << "SQL Error Type:" << logInQuery.lastError().type();
    }

    logInQuery.finish();
    // 关闭数据库连接
    db.close();
}


void LogIn::clean(){
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
}
