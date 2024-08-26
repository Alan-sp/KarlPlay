#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void onWeatherDataReceived(QNetworkReply *reply);
    void on_locateButton_clicked();  // 新增：定位按钮的槽函数
    void onCityIdReceived(QNetworkReply *reply);
    void onMultiDayWeatherReceived();
    void updateTime();  // 更新当前时间
    void fetchMultiDayWeather(const QString &cityId);
    void onLocationDataReceived();
private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QTimer *timer;
    QMap<QString, QMap<QString, QString>> weatherDataMap;  // 存储多日天气数据
    QString apiKey = "cc92bdf5e341f7b5fbb373824755bee9";
    QString cityName;  // 当前城市名称
    QString cityId;  // 当前城市ID

    void fetchWeatherData(const QString &cityId);  // 获取天气数据
    void fetchCityId(const QString &cityName);  // 获取城市ID
};


#endif // MAINWINDOW_H
