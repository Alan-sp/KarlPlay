#ifndef MAINWINDOWWEATHER_H
#define MAINWINDOWWEATHER_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowWeather; }
QT_END_NAMESPACE

class MainWindowWeather : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowWeather(QWidget *parent = nullptr);
    ~MainWindowWeather();
private slots:
    void on_searchButton_clicked();
    void onWeatherDataReceived();
    void on_locateButton_clicked();  // 新增：定位按钮的槽函数
    void onMultiDayWeatherReceived();
    void fetchMultiDayWeather(double lat, double lon);
    void updateTime();  // 更新当前时间

    void onLocationDataReceived();
private:
    Ui::MainWindowWeather *ui;
    QNetworkAccessManager *networkManager;
    QTimer *timer;
    QMap<QString, QMap<QString, QString>> weatherDataMap;  // 存储多日天气数据
    QString apiKey = "cc92bdf5e341f7b5fbb373824755bee9";
    QString cityName;  // 当前城市名称
    QString cityId;  // 当前城市ID
    QMovie *backgroundMovie;  // 用于显示GIF动画
    QLabel *backgroundLabel;  // 用于显示背景GIF的标签
    void fetchWeatherData(const QString &cityId);  // 获取天气数据
    void fetchCityId(const QString &cityName);  // 获取城市ID
    void updateBackgroundImage(const QString &weatherCondition);
    void setBlackGoldMixedLinearGradientBackground();
};


#endif // MAINWINDOW_H
