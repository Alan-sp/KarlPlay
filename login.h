#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QDialog>
#include <QPushButton>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QObject>

namespace Ui {
class LogIn;
}

class LogIn : public QDialog
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = nullptr);
    ~LogIn();

signals:
    void signalRefreshMainWindow();

public slots:
    void logIn();

private:
    Ui::LogIn *ui;
    void checkLogin(const QString &username, const QString &password);
    int userID;
    QString username;
    QString password;
    QString phone;
    float favoriteTemp;
    int favoriteStyle;
    void clean();
    void refreshMainWindow();
};

#endif // LOGIN_H
