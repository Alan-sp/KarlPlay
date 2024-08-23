#ifndef USER_H
#define USER_H

#include <QWidget>

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

private:
    Ui::User *ui;
};

#endif // USER_H
