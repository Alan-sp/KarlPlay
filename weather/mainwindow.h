#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMovie>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_searchButton_clicked();
    void onWeatherDataReceived();
    void on_locateButton_clicked();  // 新增：定位按钮的槽函数
    void onMultiDayWeatherReceived();
    void fetchMultiDayWeather(double lat, double lon);
    void updateTime();  // 更新当前时间

    void onLocationDataReceived();
private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QTimer *timer;
    QMap<QString, QMap<QString, QString>> weatherDataMap;  // 存储多日天气数据
    QString apiKey = "cc92bdf5e341f7b5fbb373824755bee9";
    QString cityName;  // 当前城市名称
    QString cityId;  // 当前城市ID
    QMovie *backgroundMovie; // 用于播放背景 GIF 的 QMovie
    void fetchWeatherData(const QString &cityId);  // 获取天气数据
    void fetchCityId(const QString &cityName);  // 获取城市ID
    void updateBackgroundImage(const QString &weatherCondition);
    void setBlackGoldMixedLinearGradientBackground();
};


#endif // MAINWINDOW_H
