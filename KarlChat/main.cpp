#include "chatserver.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ChatServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {
            qFatal("无法启动服务器: %s", qPrintable(server.errorString()));
            return 1;
        }
        qDebug("服务器启动，正在监听端口 12345...");

    return a.exec();
}
