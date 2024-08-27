#include "sign_up_win.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
sign_up_win::sign_up_win(QWidget *parent) : QWidget(parent)
{
    // 初始化
    _initMysql();

    this ->setWindowTitle("注册窗口");
    this ->setWindowIcon(QIcon(":/coin/mymusic.png"));  // 主窗口的图标设置!


    this ->setFixedSize(350, 200);
    login_user_name = new QLineEdit(this);
    login_user_password = new QLineEdit(this);

    login_user_name ->setGeometry(80, 30, 200, 30);
    login_user_password ->setGeometry(80, 80, 200, 30);

    ok = new QPushButton("确定", this);
    cancel = new QPushButton("取消", this);
    ok ->setGeometry(80, 120, 90, 30);
    cancel ->setGeometry(190, 120, 90, 30);

    user_name_l = new QLabel("用户名" ,this);
    user_password_l = new QLabel("密码", this);

    user_name_l ->setGeometry(30, 35, 40, 20);
    user_password_l ->setGeometry(35, 85, 40, 20);

    connect(ok, &QPushButton::clicked, this, &sign_up_win::user_sign_up);
    connect(cancel, &QPushButton::clicked, this, [=] () {
        this ->hide();
    });
}

// 注册响应
void sign_up_win::user_sign_up() {
    QString _user_name = login_user_name->text();
    QString _user_password = login_user_password->text();

    QSqlQuery query(db);

    // 查询是否存在相同用户名
    query.prepare("SELECT COUNT(*) FROM user WHERE username = :username");
    query.bindValue(":username", _user_name);
    query.exec();
    query.next();

    if (query.value(0).toInt() > 0) {
        QMessageBox::information(this, "提示", "注册失败，用户名已存在!", QMessageBox::Ok);
        return;
    }

    // 插入新用户
    query.prepare("INSERT INTO user (username, password) VALUES (:username, :password)");
    query.bindValue(":username", _user_name);
    query.bindValue(":password", _user_password);

    if (query.exec()) {
        QMessageBox::information(this, "提示", "注册成功!", QMessageBox::Ok);
        this->hide();
    } else {
        QMessageBox::information(this, "提示", "注册失败，请重试!", QMessageBox::Ok);
    }
}

void sign_up_win::_initMysql() {
    db = QSqlDatabase::addDatabase("QSQLITE");  // 使用 SQLite 驱动
        db.setDatabaseName("musicplayer.db"); // SQLite 数据库文件
        if (!db.open()) {
            qDebug() << "Failed to connect to SQLite database!";
        } else {
            qDebug() << "Connected to SQLite database successfully!";
        }
}
