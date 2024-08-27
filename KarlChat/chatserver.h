#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>


namespace Ui {
class ChatServer;
}

class ChatServer : public QTcpServer
{
    Q_OBJECT

public:
    ChatServer(QObject *parent = nullptr) : QTcpServer(parent){}

protected:
     void incomingConnection(qintptr socketDescriptor);

private slots:
    void readMessage();
    void clientDisconnected();

private:
    Ui::ChatServer *ui;
    QList<QTcpSocket*> clients;
};

#endif // CHATSERVER_H
