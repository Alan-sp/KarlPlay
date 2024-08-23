#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTextBrowser>
#include <QGraphicsGridLayout>
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
#endif // MAINWINDOW_H
