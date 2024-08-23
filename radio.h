#ifndef RADIO_H
#define RADIO_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class Radio;
}

class Radio : public QWidget
{
    Q_OBJECT

public:
    explicit Radio(QWidget *parent = nullptr);
    ~Radio();

public slots:
    void toMain();

private:
    Ui::Radio *ui;
};

#endif // RADIO_H
