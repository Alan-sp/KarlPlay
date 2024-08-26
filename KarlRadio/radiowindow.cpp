#include "radiowindow.h"
#include "ui_radiowindow.h"
#include "KnobWidget.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QStyleFactory>
#include <QSpinBox>


RadioWindow::RadioWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RadioWindow)
{
//    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(this);

    KnobWidget *knob = new KnobWidget;

    layout->addWidget(knob);

    frequencyLabel = new QLabel("100.0 MHz", this);
    layout->addWidget(frequencyLabel);

    listWidget = new QListWidget(this);
    layout->addWidget(listWidget);

    // 播放按钮
    playButton = new QPushButton("Play", this);
    layout->addWidget(playButton);

    // 音频播放器
    player = new QMediaPlayer(this);

    // 网络访问管理器
    networkManager = new QNetworkAccessManager(this);

    connect(playButton, &QPushButton::clicked, this, &RadioWindow::onPlayClicked);
    connect(listWidget, &QListWidget::itemClicked, this, &RadioWindow::onStationSelected);

    // 加载电台列表
    loadRadioStations();

    connect(knob, &KnobWidget::valueChanged, this, &RadioWindow::onFrequencyChanged);
}

RadioWindow::~RadioWindow()
{
    delete ui;
}
