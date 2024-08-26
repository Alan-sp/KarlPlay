#include "webchannelobject.h"

WebChannelObject::WebChannelObject(QObject *parent) : QObject(parent){}

void WebChannelObject::sendMessageToJS(const QString &message)
{
    emit messageReceived(message);
}
