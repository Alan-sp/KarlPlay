#ifndef WIDGETITEM_H
#define WIDGETITEM_H
#include <QWidget>
#include <QImage>
#include <QLabel>

class widgetItem : public QWidget
{
public:
    explicit widgetItem(QWidget *parent = nullptr);
//    ~ItemWidget();

    int SetMyData(quint32 id, QString picon, QString txt); //用户消息框
    int SetFriendsData(quint32 id,QString picon, QString txt); //用户消息框
    void SetFriendsData(quint32 id,QString picon, QPixmap txt); //用户消息框
    void setColor(QString c);
    int SetMyImg(quint32 id,QString picon ,QString imgpth);
    int SetFriendsImg(quint32 id,QString picon ,QString imgpth);
private:
    QString Splite(QString txt, QFontMetrics& font);
    int countLine;
    int longestWidth;
    QString color; //气泡颜色
//    ChatMessage FIle;
    QString filename;
};

#endif // WIDGETITEM_H
