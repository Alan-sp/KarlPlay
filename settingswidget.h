#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include<QMainWindow>
#include <QWidget>
#include <QPushButton>
#include "user.h"

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();
public slots:
    void backMain();
    void toUser();


private:
    Ui::SettingsWidget *ui;
    User *newUserWidget;
};

#endif // SETTINGSWIDGET_H
