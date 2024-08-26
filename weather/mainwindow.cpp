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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this))
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);  // 每秒触发一次

    QPixmap background(":/weather.png"); // 使用资源文件
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio); // 调整图片大小以适应窗口
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onWeatherDataReceived);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::on_searchButton_clicked);
    connect(ui->locateButton, &QPushButton::clicked, this, &MainWindow::on_locateButton_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
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
    QString url = QString("https://geoapi.qweather.com/v2/city/lookup?location=beij&key=YOUR_KEY")
                    .arg(cityName)
                    .arg(apiKey);
    QNetworkRequest *request=new QNetworkRequest(QUrl(url));

    // 通过 networkManager 发起请求
    QNetworkReply *reply = networkManager->get(*request);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onWeatherDataReceived);
}

void MainWindow::onWeatherDataReceived(QNetworkReply *reply)
{
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
        ui->weatherLabel->setText("Weather: " + weather);
        ui->temperatureLabel->setText("Temperature: " + QString::number(temp) + " °C");
        ui->latitudeLabel->setText("Latitude: " + QString::number(latitude));
        ui->longitudeLabel->setText("Longitude: " + QString::number(longitude));
        ui->regionLabel->setText("City: " + cityName + ", " + country);

        // 获取多日天气信息
        QString cityId = jsonObj["id"].toString();
        fetchMultiDayWeather(cityId);

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
    QString locateUrl = "http://whois.pconline.com.cn/ipJson.jsp?ip";
    QNetworkRequest *request = new QNetworkRequest(QUrl(locateUrl));

    // 发起网络请求获取地理位置信息
    QNetworkReply *reply = networkManager->get(*request);

    // 连接信号与槽函数，处理网络请求完成后获取地理位置信息
    connect(reply, &QNetworkReply::finished, this, &MainWindow::onLocationDataReceived);
}

void MainWindow::onCityIdReceived(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        // 处理接收到的数据
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        // 从返回的JSON对象中提取城市ID（假设API返回的数据包含一个叫做"id"的字段）
        QString cityId;
        if (jsonObj.contains("id")) {
            cityId = QString::number(jsonObj["id"].toInt());
            qDebug() << "City ID:" << cityId;

            // 使用 cityId 进一步获取天气信息
            fetchMultiDayWeather(cityId);
        } else {
            qDebug() << "City ID not found in the response.";
            QMessageBox::warning(this, "Data Error", "City ID not found in the response.");
        }

    } else {
        qDebug() << "Error retrieving city ID data:" << reply->errorString();
        QMessageBox::warning(this, "Network Error", "Failed to retrieve city ID.");
    }

    // 清理 reply 对象
    reply->deleteLater();
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


void MainWindow::fetchMultiDayWeather(const QString &cityId)
{
    QString multiDayWeatherUrl = QString("https://geoapi.qweather.com/v2/city/lookup?location=beij&key=YOUR_KEY")
                                 .arg(cityId)
                                 .arg(apiKey);

    // 创建一个持久的 QNetworkRequest 对象，并通过构造函数传递 URL
    QNetworkRequest *request=new QNetworkRequest(QUrl(multiDayWeatherUrl));

    // 通过 networkManager 发起请求
    QNetworkReply *reply = networkManager->get(*request);

    // 连接信号与槽函数，处理网络请求的完成事件
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

        if (jsonObj.contains("list") && jsonObj["list"].isArray()) {
            QJsonArray dailyArray = jsonObj["list"].toArray();
            for (const QJsonValue &value : dailyArray) {
                QJsonObject dayObj = value.toObject();
                QString date = dayObj["dt_txt"].toString();
                QMap<QString, QString> dayData;
                dayData["tempMin"] = QString::number(dayObj["main"].toObject()["temp_min"].toDouble());
                dayData["tempMax"] = QString::number(dayObj["main"].toObject()["temp_max"].toDouble());
                dayData["description"] = dayObj["weather"].toArray()[0].toObject()["description"].toString();
                weatherDataMap[date] = dayData;
            }

            // 更新界面上的多日天气信息（以显示第一个日期为例）
            if (!weatherDataMap.isEmpty()) {
                QString firstDay = weatherDataMap.keys().first();
                ui->multiDayWeatherLabel->setText("Day 1: " + firstDay + ", " + weatherDataMap[firstDay]["description"] +
                                                  ", Temp: " + weatherDataMap[firstDay]["tempMin"] + " - " +
                                                  weatherDataMap[firstDay]["tempMax"] + "°C");
            }
        }
    } else {
        qDebug() << "Error fetching multi-day weather data:" << reply->errorString();
    }
    reply->deleteLater();
}

void MainWindow::updateTime()
{
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->timeLabel->setText("Current Time: " + currentTime);
}
