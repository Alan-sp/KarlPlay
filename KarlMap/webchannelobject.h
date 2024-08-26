#ifndef WEBCHANNELOBJECT_H
#define WEBCHANNELOBJECT_H

#include <QObject>

class WebChannelObject : public QObject
{
    Q_OBJECT
public:
    explicit WebChannelObject(QObject *parent = nullptr);

signals:
    void messageReceived(const QString &message);

public slots:
    void sendMessageToJS(const QString &message);
};

#endif // WEBCHANNELOBJECT_H
