#include "widgetitem.h"
#include <QDebug>
#include <QToolButton>

widgetItem::widgetItem(QWidget *parent) :
    QWidget(parent)
{

}

int widgetItem::SetMyData(quint32 id, QString picon, QString txt)
{
    //头像
    QToolButton* imgbtn = new QToolButton(this);
    imgbtn->setIcon(QIcon(picon));
    imgbtn->setIconSize(QSize(40,40));
    imgbtn->setGeometry(500,10,40,40);
    imgbtn->setStyleSheet("QToolButton:{border:none;}");
    imgbtn->show();

    QFontMetrics fm(this->font());

    //id
    QLabel* idlab = new QLabel(this);
    idlab->setText(QString::number(id));
    idlab->setGeometry(480-fm.horizontalAdvance(QString::number(id)),10,fm.horizontalAdvance(QString::number(id)),fm.height());

    //文本
    QLabel* txtlab = new QLabel(this);
    txtlab->setStyleSheet("QLabel{background-color:"+color+";border:5px solid "+color+";\
                              border-top-left-radius:15px;\
                              border-bottom-left-radius:15px;\
                              border-bottom-right-radius:15px;}");
    int txtheight = fm.height();
    countLine = 0;
    longestWidth = 0;
    txt = Splite(txt,fm);
    qDebug()<<countLine;
    txtlab->setText(txt);
    txtlab->setGeometry(480-longestWidth-30, 30, longestWidth+30, txtheight*countLine+40);
    txtlab->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    txtlab->setWordWrap(true);
    return 30+10+txtheight*countLine+40;
}

void widgetItem::SetFriendsData(quint32 id, QString picon, QPixmap txt)
{
    QToolButton* imgbtn = new QToolButton(this);
    imgbtn->setIcon(QIcon(picon));
    imgbtn->setIconSize(QSize(40,40));
    imgbtn->setGeometry(10,10,40,40);
    imgbtn->setStyleSheet("QToolButton:{border:none;}");
    imgbtn->show();

    QFontMetrics fm(this->font());

    //id
    QLabel* idlab = new QLabel(this);
    idlab->setText(QString::number(id));
    idlab->setGeometry(70,10,fm.horizontalAdvance(QString::number(id))+20,fm.height());

    QLabel* txtlab = new QLabel(this);
    txtlab -> setPixmap(txt);
    txtlab->setGeometry(70, 30, txt.width()+30, txt.height()+25);
    txtlab->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

}

int widgetItem::SetFriendsData(quint32 id, QString picon, QString txt)
{
    //头像
    QToolButton* imgbtn = new QToolButton(this);
    imgbtn->setIcon(QIcon(picon));
    imgbtn->setIconSize(QSize(40,40));
    imgbtn->setGeometry(10,10,40,40);
    imgbtn->setStyleSheet("QToolButton:{border:none;}");
    imgbtn->show();

    QFontMetrics fm(this->font());

    //id
    QLabel* idlab = new QLabel(this);
    idlab->setText(QString::number(id));
    idlab->setGeometry(70,10,fm.horizontalAdvance(QString::number(id))+20,fm.height());

    //文本
    QLabel* txtlab = new QLabel(this);
    txtlab->setStyleSheet("QLabel{background-color:rgb(255,255,255);border:2px solid "+color+";\
                            border-top-right-radius:15px;\
                            border-bottom-left-radius:15px;\
                            border-bottom-right-radius:15px}");
    int txtheight = fm.height();
    countLine = 0;
    longestWidth = 0;
    countLine = 0;
    txt = Splite(txt,fm);
    qDebug()<<countLine;
    txtlab->setText(txt);
    txtlab->setGeometry(70, 30, longestWidth+30, txtheight*countLine+35);
    txtlab->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    txtlab->setWordWrap(true);
    return 30+10+txtheight*countLine+35;
}

QString widgetItem::Splite(QString txt, QFontMetrics& font)
{
    int txtwidth = txt.size(); //当前字符串长度
    qDebug()<<txt.indexOf("\n");
    if(txt.indexOf("\n") != -1)
    {
        QString qstrLeftData = txt.left(txt.indexOf("\n"));
        qDebug()<<qstrLeftData;
        QString qstrMidData = txt.mid(txt.indexOf("\n")+1);
        qDebug()<<qstrMidData;
        return Splite(qstrLeftData, font) + "\n" + Splite(qstrMidData, font);
    }
    int txtwidth2 = font.horizontalAdvance(txt); //当前字符串所占像素长度
    //qDebug()<<txtwidth;
    qDebug()<<txtwidth2;
    if(txtwidth2<=200)  //不需要继续划分
    {
        countLine++;
        if(txtwidth2>longestWidth)
            longestWidth = txtwidth2;
        qDebug()<<longestWidth;
        return txt;
    }
    else
    {
        countLine++;
        longestWidth = 200;
        QString qstrLeftData = txt.left(txtwidth*200/txtwidth2);
        qDebug()<<qstrLeftData;
        QString qstrMidData = txt.mid(txtwidth*200/txtwidth2);
        qDebug()<<qstrMidData;
        return qstrLeftData + "\n" + Splite(qstrMidData, font);
    }
}
