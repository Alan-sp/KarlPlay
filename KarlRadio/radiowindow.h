#ifndef RADIOWINDOW_H
#define RADIOWINDOW_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

namespace Ui {
class RadioWindow;
}

class RadioWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RadioWindow(QWidget *parent = nullptr);
    ~RadioWindow();

public slots:
    void onFrequencyChanged(double value) {
        double frequency = value / 10.0;
        frequencyLabel->setText(QString::number(frequency, 'f', 1) + " MHz");

    }

    void onStationsLoaded() {
            QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
                QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                QJsonArray stationsArray = jsonDoc.array();

                for (const QJsonValue &stationValue : stationsArray) {
                    QJsonObject stationObject = stationValue.toObject();
                    QString name = stationObject["name"].toString();
                    QString url = stationObject["url_resolved"].toString();

                    QListWidgetItem *item = new QListWidgetItem(name, listWidget);
                    item->setData(Qt::UserRole, url);
                }
            } else {
                qDebug() << "Error loading stations:" << reply->errorString();
            }

            reply->deleteLater();
        }

        void onStationSelected(QListWidgetItem *item) {
            selectedStationUrl = item->data(Qt::UserRole).toString();
        }

        void onPlayClicked() {
            if (!selectedStationUrl.isEmpty()) {
                player->setMedia(QUrl(selectedStationUrl));
                player->play();
            }
        }

    void loadRadioStations() {
            QUrl url("http://de1.api.radio-browser.info/json/stations/topvote");
            QNetworkRequest request(url);

            QNetworkReply *reply = networkManager->get(request);
            connect(reply, &QNetworkReply::finished, this, &RadioWindow::onStationsLoaded);
        }
        // 更新接收器频率
        // radioControl.setFrequency(frequency);


private:
    Ui::RadioWindow *ui;
    QSlider *slider;
    QLabel *label;
    QLabel *frequencyLabel;
    QListWidget *listWidget;
    QPushButton *playButton;
    QMediaPlayer *player;
    QNetworkAccessManager *networkManager;
    QString selectedStationUrl;
};

#endif // RADIOWINDOW_H
