#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QCoreApplication>  // 如果你在控制台应用中使用 Qt 核心模块
#include <QSqlDatabase>      // QSqlDatabase 类，用于处理数据库连接
#include <QSqlQuery>         // QSqlQuery 类，用于执行 SQL 查询
#include <QSqlError>         // QSqlError 类，用于处理 SQL 错误信息
#include <QVariant>          // QVariant 类，用于处理查询结果中的数据
#include <QDebug>            // QDebug 类，用于输出调试信息
#include <QDir>              // QDir 类，用于处理目录路径
#include <QFile>
#include "signin.h"
#include "login.h"

namespace Ui {
class User;
}

class User : public QWidget
{
    Q_OBJECT

public:
    explicit User(QWidget *parent = nullptr);
    ~User();

public slots:
    void backSettings();
    void backDesktop();
    void toLogIn();
    void toSignIn();

private:
    Ui::User *ui;
    SignIn *signInWidget;
    LogIn *logInWidget;


};

#endif // USER_H
