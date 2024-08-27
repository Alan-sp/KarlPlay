#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPalette>
#include <QPixmap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QDateTime>
#include <QTimer>
#include <QMovie>
#include <QLabel>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this))
    , timer(new QTimer(this))
{
    ui->setupUi(this);
    setBlackGoldMixedLinearGradientBackground();
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);  // 每秒触发一次

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onWeatherDataReceived);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::on_searchButton_clicked);
    connect(ui->locateButton, &QPushButton::clicked, this, &MainWindow::on_locateButton_clicked);

}
void MainWindow::setBlackGoldMixedLinearGradientBackground()
{
    // 创建一个径向渐变，中心在窗口的略偏下方，半径为窗口宽度的3/4
        QRadialGradient radialGradient(this->width() / 2, this->height() * 3 / 4, this->width() * 3 / 4);

        // 中心位置为黑色
        radialGradient.setColorAt(0.0, QColor(0, 0, 0));   // 完全黑色

        // 增加颜色点使过渡更柔和
        radialGradient.setColorAt(0.3, QColor(30, 30, 0, 200));  // 非常暗的金色，透明度稍高
        radialGradient.setColorAt(0.5, QColor(80, 70, 0, 180));  // 深金色，透明度稍高
        radialGradient.setColorAt(0.7, QColor(150, 130, 0, 120)); // 中度金色，带有更高透明度

        // 渐变到较远处的亮金色区域
        radialGradient.setColorAt(0.85, QColor(200, 180, 0, 80)); // 较亮的金色，透明度增加

        // 最外部位置的浅金色，带有更多透明度
        radialGradient.setColorAt(1.0, QColor(255, 215, 0, 50)); // 边缘带有浅金色和更高透明度

        // 应用渐变为背景
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(radialGradient));
        this->setPalette(palette);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateBackgroundImage(const QString &weatherCondition)
{
    if (backgroundMovie) {
            backgroundMovie->stop();
            delete backgroundMovie;
            backgroundMovie = nullptr;
        }

        QString gifPath;

        if (weatherCondition.contains("clear", Qt::CaseInsensitive)) {
            gifPath = ":/../../Desktop/sunny_background.gif"; // 替换为实际的晴天 GIF 路径
        } else if (weatherCondition.contains("rain", Qt::CaseInsensitive)) {
            gifPath = ":/../../Desktop/rainy_background.gif"; // 替换为实际的雨天 GIF 路径
        } else if (weatherCondition.contains("cloud", Qt::CaseInsensitive)) {
            gifPath = ":/../../Desktop/cloudy_background.gif"; // 替换为实际的多云 GIF 路径
        } else if (weatherCondition.contains("snow", Qt::CaseInsensitive)) {
            gifPath = ":/../../Desktop/snowy_background.gif"; // 替换为实际的雪天 GIF 路径
        } else {
            gifPath = ":/gifs/default.gif"; // 默认 GIF 路径
        }

        backgroundMovie = new QMovie(gifPath);

        if (!backgroundMovie->isValid()) {
            qDebug() << "Failed to load GIF:" << gifPath;
            return;
        }

        QLabel *backgroundLabel = new QLabel(this);
        backgroundLabel->setGeometry(this->rect());
        backgroundLabel->setMovie(backgroundMovie);
        backgroundLabel->lower(); // 将标签放在所有控件的最底层
        backgroundMovie->start();
}
void MainWindow::on_searchButton_clicked()
{
    cityName = ui->cityLineEdit->text();  // 获取用户输入的城市名称
    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a city name.");
        return;
    }

    fetchWeatherData(cityName);
}

void MainWindow::fetchWeatherData(const QString &cityName)
{
    QString url = QString("http://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric")
                    .arg(cityName)
                    .arg(apiKey);
    QNetworkRequest *request=new QNetworkRequest(QUrl(url));

    // 通过 networkManager 发起请求
    QNetworkReply *reply = networkManager->get(*request);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::onWeatherDataReceived);
}

void MainWindow::onWeatherDataReceived()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
        if (!reply) {
            // 处理错误情况
            return;
        }
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug() << "Received response data:" << responseData;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        // 获取当前天气信息
        QString weather = jsonObj["weather"].toArray()[0].toObject()["description"].toString();
        double temp = jsonObj["main"].toObject()["temp"].toDouble();
        double latitude = jsonObj["coord"].toObject()["lat"].toDouble();
        double longitude = jsonObj["coord"].toObject()["lon"].toDouble();
        QString cityName = jsonObj["name"].toString();
        QString country = jsonObj["sys"].toObject()["country"].toString();

        // 显示基本天气信息和经纬度
        ui->weatherLabel->setText("天气: " + weather);
        ui->temperatureLabel->setText("温度: " + QString::number(temp) + " °C");
        ui->latitudeLabel->setText("纬度: " + QString::number(latitude));
        ui->longitudeLabel->setText("经度: " + QString::number(longitude));
        ui->regionLabel->setText("城市: " + cityName + ", " + country);
        updateBackgroundImage(weather);
        // 获取多日天气信息
        QString cityId = jsonObj["id"].toString();
        fetchMultiDayWeather(latitude,longitude);

    } else {
        // 如果网络请求失败，显示错误信息
        ui->weatherLabel->setText("Error retrieving data.");
        ui->temperatureLabel->clear();
        ui->latitudeLabel->clear();
        ui->longitudeLabel->clear();
        ui->regionLabel->clear();
        qDebug() << "Network Error: " << reply->errorString(); // 打印错误信息
    }
    reply->deleteLater();
}


void MainWindow::on_locateButton_clicked()
{
    // 使用IP地址获取当前所在城市的API
    QString locateUrl = "https://ipinfo.io/json";
    QNetworkRequest *request = new QNetworkRequest(QUrl(locateUrl));

    // 发起网络请求获取地理位置信息
    QNetworkReply *reply = networkManager->get(*request);

    // 连接信号与槽函数，处理网络请求完成后获取地理位置信息
    connect(reply, &QNetworkReply::finished, this, &MainWindow::onLocationDataReceived);
}


void MainWindow::onLocationDataReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        // 从返回的数据中获取城市名称
        QString city = jsonObj["city"].toString();
        qDebug() << "Located city:" << city;

        // 获取当前城市的天气
        fetchWeatherData(city);
    } else {
        qDebug() << "Error retrieving location data:" << reply->errorString();
        QMessageBox::warning(this, "Location Error", "Failed to retrieve location information.");
    }
    reply->deleteLater();
}




void MainWindow::fetchMultiDayWeather(double lat, double lon)
{
    QString multiDayWeatherUrl = QString("https://api.openweathermap.org/data/2.5/onecall?lat=%1&lon=%2&exclude=hourly,minutely&appid=%3&units=metric")
                                 .arg(lat)
                                 .arg(lon)
                                 .arg(apiKey);

    QNetworkRequest *request = new QNetworkRequest(QUrl(multiDayWeatherUrl));

    QNetworkReply *reply = networkManager->get(*request);

    connect(reply, &QNetworkReply::finished, this, &MainWindow::onMultiDayWeatherReceived);
}


void MainWindow::onMultiDayWeatherReceived()
{

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

        if (!reply) {
            // 如果 sender 不是 QNetworkReply，直接返回
            return;
        }
        if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();
                QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
                QJsonObject jsonObj = jsonDoc.object();

                // 处理 "daily" 数组
                QJsonArray dailyArray = jsonObj["daily"].toArray();
                for (const QJsonValue &value : dailyArray) {
                    QJsonObject dayObj = value.toObject();
                    double tempDay = dayObj["temp"].toObject()["day"].toDouble();
                    QString description = dayObj["weather"].toArray()[0].toObject()["description"].toString();
                    qDebug() << "Day Temp:" << tempDay << "Description:" << description;
                    // 继续处理其他数据...
                }
    } else {
        qDebug() << "Error fetching multi-day weather data:" << reply->errorString();
    }
    reply->deleteLater();
}


void MainWindow::updateTime()
{
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->timeLabel->setText("当前时间: " + currentTime);
}
