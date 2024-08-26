#ifndef SIGNIN_H
#define SIGNIN_H

#include <QDialog>
#include<QPushButton>
#include<QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace Ui {
class SignIn;
}

class SignIn : public QDialog
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent = nullptr);
    ~SignIn();

public slots:
    void back();
    void signIn();
private:
    Ui::SignIn *ui;
    void insertInfoSignIn();
    QString username;
    QString password;
    QString repassword;
    QString phone;
    float favoriteTemp;
    int favoriteStyle;
    void clean();
    void insertUserIntoDatabase();
};

#endif // SIGNIN_H
