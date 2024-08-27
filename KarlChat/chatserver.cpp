#include "chatserver.h"
#include "ui_chatserver.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

void ChatServer:: incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);
    clients << clientSocket;

    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::readMessage);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::clientDisconnected);
}

void ChatServer::readMessage() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
        QByteArray data = clientSocket->readAll();
        QString dataStr = QString::fromUtf8(data);

        if (dataStr.startsWith("IMG")) { // 图片消息处理
            int imgSize = dataStr.split(" ")[1].toInt();
            QByteArray imageData = data.right(imgSize);

            // 将图片转发给所有其他客户端
            for (QTcpSocket *client : clients) {
                if (client != clientSocket) {
                    client->write(data);
                }
            }
        } else {
            // 普通消息处理
            for (QTcpSocket *client : clients) {
                if (client != clientSocket) {
                    client->write(data);
                }
            }
        }
}

void ChatServer::clientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    clients.removeAll(clientSocket);
    clientSocket->deleteLater();
}
