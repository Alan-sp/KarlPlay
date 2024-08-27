#include "sign_in_win.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

sign_in_win::sign_in_win(QWidget *parent) : QWidget(parent)
{
    this ->setWindowTitle("登录窗口");
    this ->setWindowIcon(QIcon(":/coin/mymusic.png"));  // 主窗口的图标设置!
    this ->setFixedSize(350, 200);
    user_name = new QLineEdit(this);
    user_password = new QLineEdit(this);

    user_name ->setGeometry(80, 30, 200, 30);
    user_password ->setGeometry(80, 80, 200, 30);

    sign_in_btn = new QPushButton("登录", this);
    sign_up_btn = new QPushButton("注册", this);
    sign_in_btn ->setGeometry(80, 120, 90, 30);
    sign_up_btn ->setGeometry(190, 120, 90, 30);

    user_name_l = new QLabel("用户名" ,this);
    user_password_l = new QLabel("密码", this);

    user_name_l ->setGeometry(30, 35, 40, 20);
    user_password_l ->setGeometry(35, 85, 40, 20);

    connect(sign_in_btn, &QPushButton::clicked, this, &sign_in_win::user_sign_in);
    connect(sign_up_btn, &QPushButton::clicked, this, [=] () {
        login_win = new sign_up_win();
        login_win ->show();
    });

    initMysql();
}

void sign_in_win::user_sign_in() {
    QSqlQuery selectq = QSqlQuery(db);
    selectq.exec("select * from user");
    while(selectq.next()) {
        QString name = selectq.value(1).value<QString>();
        QString password = selectq.value(2).value<QString>();
        if(name == user_name ->text() && password == user_password ->text()) {
            this ->user_id = selectq.value(0).value<int>();
            this ->song_list_cnt = selectq.value(3).value<int>();
            break;
        }
    }
    if(user_id == -1) QMessageBox::information(this, "提示", "登录失败", QMessageBox::Ok);
    else {
        QMessageBox::information(this, "提示", "登录成功", QMessageBox::Ok);
        emit sign_in_success();
        this ->hide();
        user_id = -1;
    }
}

void sign_in_win::initMysql() {
    db = QSqlDatabase::addDatabase("QSQLITE");  // 使用 SQLite 驱动
        db.setDatabaseName("musicplayer.db"); // SQLite 数据库文件
        if (!db.open()) {
            qDebug() << "Failed to connect to SQLite database!";
        } else {
            qDebug() << "Connected to SQLite database successfully!";
        };
}
