#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
#include <QWidget>
#include<QPushButton>

namespace Ui {
class helpWindow;
}

class helpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit helpWindow(QDialog *parent = nullptr);
    ~helpWindow();

public slots:
    void backMain();

private:
    Ui::helpWindow *ui;
};

#endif // HELPWINDOW_H
