#ifndef CHATCLENT_H
#define CHATCLENT_H

#include <QWidget>
#include <QtNetwork/QTcpServer>
#include <QTextEdit>
#include <QLineEdit>
#include <QMainWindow>
#include <QListWidget>

namespace Ui {
class ChatClient;
}

class ChatClient : public QWidget
{
    Q_OBJECT

public:
    explicit ChatClient(QWidget *parent = nullptr);
    ~ChatClient();

private slots:
    void sendMessage();
    void receiveMessage();
    void sendImage();

private:
    Ui::ChatClient *ui;
    QTcpSocket *socket;
    QTextEdit *messageBox;
    QLineEdit *inputLine;
    QListWidget *pchatw;
};

#endif // CHATCLENT_H
