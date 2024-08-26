#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTextBrowser>
#include <QGraphicsGridLayout>
#include <QScrollArea>
#include "helpwindow.h"
#include "settingswidget.h"
#include "radio.h"
#include "user.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    struct UserInfo {
        int userID;
        QString username;
        QString password;
        QString phone;
        float favoriteTemp;
        int favoriteStyle;
    }UserInfo;
    bool fetchUserInfo(int ID, struct UserInfo &userInfo);
    void refresh();

public slots:
    void toHelp();
    void toSettings();
    void toMedia();
    void toUser();

private:
    Ui::MainWindow *ui;
    helpWindow *newHelpWindow;
    SettingsWidget *newSettingsWidget;
    Radio *newMediaWidget;
    User *newUserWidget;
};

void createDatabaseAndUserTable();
#endif // MAINWINDOW_H
