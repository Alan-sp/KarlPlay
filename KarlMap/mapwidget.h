#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QMainWindow>
#include <QString>
#include <QWebEnginePage>
#include <QWebEngineView>
#include "webchannelobject.h"

namespace Ui {
class mapWidget;
}

class mapWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit mapWidget(QWidget *parent = nullptr);
    ~mapWidget();

private:
    static int op;
    Ui::mapWidget *ui;
    QWebEngineView *view;
    QWebChannel *webChannel;
    QWebEnginePage *page;
    WebChannelObject *webChannelObject;

public slots:
    void onButtonClicked();
    void getPosition();
    void receiveMessage(const QString &message);
    void onMapClicked0();
    void onMapClicked1();

};

#endif // MAPWIDGET_H
