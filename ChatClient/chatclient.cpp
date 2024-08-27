#include "chatclient.h"
#include "ui_chatclient.h"
#include "widgetitem.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <QPushButton>
#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QBuffer>
#include <QPixmap>
#include <QLabel>
#include <QListWidgetItem>
#include <QHBoxLayout>

ChatClient::ChatClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatClient)
{
//    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout(this);

//    messageBox = new QTextEdit(this);
//    messageBox->setReadOnly(true);
//    layout->addWidget(messageBox);
    pchatw = new QListWidget(this);
    layout -> addWidget(pchatw);

    inputLine = new QLineEdit(this);
    layout->addWidget(inputLine);

    layout->addLayout(hLayout);
    QPushButton *sendButton = new QPushButton("发送", this);
    hLayout->addWidget(sendButton);

    QPushButton *sendImageBtn = new QPushButton("图片",this);
    hLayout->addWidget(sendImageBtn);

    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 12345);

    connect(inputLine, &QLineEdit::returnPressed, this, &ChatClient::sendMessage);
    connect(sendButton, &QPushButton::clicked, this, &ChatClient::sendMessage);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::receiveMessage);
    connect(sendImageBtn, &QPushButton::clicked,this,&ChatClient::sendImage);
}

void ChatClient::sendMessage()
{
    QString message = inputLine->text();
    if (!message.isEmpty()) {
        socket->write(message.toUtf8());
        inputLine->clear();
    }
}

void ChatClient::receiveMessage()
{
    QByteArray data = socket->readAll();
    QString dataStr = QString::fromUtf8(data);

    if (dataStr.startsWith("IMG")) { // 接收到图片
        int imgSize = dataStr.split(" ")[1].toInt();
        QByteArray imageData = data.right(imgSize);
        QImage image;
        image.loadFromData(imageData);
        QPixmap pixmap = QPixmap::fromImage(image);

        widgetItem* item=new widgetItem(this);
        item->SetFriendsData(1,":/img/defaultimg.png",pixmap);

        // 在消息框中显示图片
        QListWidgetItem * line = new QListWidgetItem;
        line->setSizeHint(QSize(350,pixmap.height()+70));
        pchatw->addItem(line);
        pchatw->setItemWidget(line,item);
        pchatw->scrollToBottom();
    } else {
        // 处理普通消息
        widgetItem* item=new widgetItem(this);
        int height=item->SetFriendsData(1,":/img/defaultimg.png",dataStr);
        QListWidgetItem * line = new QListWidgetItem;
        line->setSizeHint(QSize(350, height));
        pchatw->addItem(line);
        pchatw->setItemWidget(line, item);
        pchatw->scrollToBottom();
     }
}

void ChatClient::sendImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.xpm *.jpg)");
        if (!fileName.isEmpty()) {
            QImage image(fileName);
            if (!image.isNull()) {
                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                image.save(&buffer, "JPG"); // 将图片保存到字节数组中

                QString header = QString("IMG %1 ").arg(byteArray.size()); // 发送图片大小
                socket->write(header.toUtf8());
                socket->write(byteArray);
                qDebug()<<"image selected";
            }
        }
}

ChatClient::~ChatClient()
{
    delete ui;
}
