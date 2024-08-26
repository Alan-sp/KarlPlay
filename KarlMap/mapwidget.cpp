#include "mapwidget.h"
#include "ui_mapwidget.h"
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebChannel>
#include <QString>
#include <QWebEngineView>
#include <QPushButton>
#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include "webchannelobject.h"

int mapWidget::op =-1;

mapWidget::mapWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mapWidget)
{
    ui->setupUi(this);
    view = ui->webView;

    page = view->page();
    webChannel = new QWebChannel(this);
    view->page()->setWebChannel(webChannel);

    QString filePath = "file:///home/alansp/KarlPlay/KarlMap/map.html";

    QUrl url(filePath);

    page->load(url);

    webChannelObject = new WebChannelObject();
    webChannel->registerObject(QStringLiteral("webChannelObject"),webChannelObject);

    connect(ui->myPosition,&QPushButton::clicked,this,&mapWidget::getPosition);
    connect(ui->pushButton,&QPushButton::clicked,this,&mapWidget::onButtonClicked);
    connect(webChannelObject,&WebChannelObject::messageReceived,this,&mapWidget::receiveMessage);
    connect(ui->chooseBtn0,&QPushButton::clicked,this,&mapWidget::onMapClicked0);
    connect(ui->chooseBtn1,&QPushButton::clicked,this,&mapWidget::onMapClicked1);
}

void mapWidget::receiveMessage(const QString &message)
{
    if(op==-1) return;
    else if(op==0) ui->lonEdit->setText(message);
    else ui->lanEdit->setText(message);
    op = -1;
}

void mapWidget::onButtonClicked()
{
    QString lan = ui->lanEdit->toPlainText();
    QString lon = ui->lonEdit->toPlainText();
    QString cmd = QString("driveDirect(\"%1\",\"%2\")").arg(lon).arg(lan);
    page->runJavaScript(cmd);
}


void mapWidget::getPosition()
{
    op = 0;
    QString cmd = QString("getPosition()");
    page->runJavaScript(cmd);
}

void mapWidget::onMapClicked0()
{
    op = 0;
}

void mapWidget::onMapClicked1()
{
    op = 1;
}

mapWidget::~mapWidget()
{
    delete ui;
}//北京理工大学 西单
