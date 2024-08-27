#ifndef SCROLLHOME_H
#define SCROLLHOME_H

#include <QObject>
#include <QTableWidget>
#include <QGridLayout>
#include <QPushButton>
class AppRes{

public:
    QString appName;
    QString url;
    QRect clikecdRect;
    int resIndex;
    AppRes(QString name, QString url)
    {
        this->appName = name;
        this->url = url;
    }

};

static AppRes g_appRes[]={
    {"语音助手",":icon_settings"},
    {"天气",":icon_weather"},
    {"浏览器",":icon_liulanqi"},
    {"微聊",":icon_chat"},
    {"地图导航",":icon_map"},
    {"图片查看器",":icon_photo"},
    {"视频播放器",":icon_vidoe"},
    {"收音机",":icon_carradio"},
    {"音乐播放器",":icon_music"},
    {"设置",":icon_settings"},

};

static const int g_appResSize = sizeof(g_appRes)/sizeof(g_appRes[0]);

class ScrollHome : public QTableWidget
{
    Q_OBJECT

    int iconSize;       //图标大小
    QSize itemSize;
    int iconSpacing;
    int iconRow;     //
    QMargins margins;   //上下左右边距

    QFont font;
    QPalette palette;
    QWidget *parent;

    QPushButton* newAppButton(QWidget* app,AppRes& appRes);

    QWidget* newAppIcon(AppRes& appRes);

public:
    explicit ScrollHome(QWidget *parent );

    void InitHome(int iconRow, int iconSpacing, int widgetWidth, int widgetHeight );

    int GetIconSize();

signals:
    void appClicked(AppRes* app);

protected slots:
    void onIcon();

};

#endif // SCROLLHOME_H
