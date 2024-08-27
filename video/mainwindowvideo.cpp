#include "mainwindowvideo.h"
#include "ui_mainwindowvideo.h"


MainWindowVideo::MainWindowVideo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowVideo)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    playlist = new QMediaPlaylist;
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player = new QMediaPlayer(this);

    playlist->clear();

    player->setPlaylist(playlist);
    player->setVolume(60);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    videoItem = new QGraphicsVideoItem;
    videoItem->setSize(QSizeF(ui->graphicsView->width(),ui->graphicsView->height()));
    videoItem->setFlags(QGraphicsItem::ItemIsMovable|
                        QGraphicsItem::ItemIsSelectable|
                        QGraphicsItem::ItemIsFocusable);
    scene->addItem(videoItem);
    player->setVideoOutput(videoItem);
    ui->listView->hide();
    ui->btn_closelist->hide();

    ui->slider_vol->setMaximum(100);
    ui->slider_vol->setMinimum(0);



    connect(player,&QMediaPlayer::positionChanged,this,[=]{
        ui->slider_pos->setValue(player->position());
    });
    connect(player,&QMediaPlayer::durationChanged,this,[this](){
        ui->slider_pos->setMaximum(player->duration());
    });
    connect(ui->slider_pos,QOverload<int>::of(&QSlider::valueChanged),this,&MainWindowVideo::onSliderValueChanged);


    connect(player,&QMediaPlayer::durationChanged,this,[=](qint64 duration){
        ui->label_right->setText(QString("%1.%2").arg(duration/1000/60,2,10,QChar('0')).arg(duration/1000%60));
    });

    connect(player,&QMediaPlayer::positionChanged,this,[=](qint64 pos){
        ui->label_left->setText(QString("%1.%2").arg(pos/1000/60,2,10,QChar('0')).arg(pos/1000%60));
    });


    connect(ui->slider_vol,&QSlider::valueChanged,this,[this](int value){
        player->setVolume(value);
    });

    connect(player,&QMediaPlayer::volumeChanged,this,[this](int volume){
        ui->slider_vol->setValue(volume);
    });










}

MainWindowVideo::~MainWindowVideo()
{
    delete ui;
}


void MainWindowVideo::on_btn_play_clicked()
{
    if(player->state()==QMediaPlayer::PlayingState){
        player->pause();
        ui->btn_play->setIcon(QIcon(":/new/prefix1/img/images/play.png"));
    }else{
        player->play();

        ui->btn_play->setIcon(QIcon(":/new/prefix1/img/images/pause.png"));
    }

}

void MainWindowVideo::on_btn_openfile_clicked()
{

        QString strCurrentPath = QDir::homePath(); //获取系统当前目录
        QString strDlgTitle = "选择视频文件";
        QString strFilter = "MP4 File(*.mp4);;All File(*.*)";// 打开视频文件过滤
        QStringList strAllFiles = QFileDialog::getOpenFileNames(this,strDlgTitle,strCurrentPath,strFilter);

        if(strAllFiles.isEmpty())
        {
            QMessageBox::information(this,"错误","打开视频文件失败，请重新检查",QMessageBox::Yes|QMessageBox::No);
            return;
        }
        for(int i = 0;i<strAllFiles.count();i++)
           {
               QString aFile = strAllFiles.at(i);
               playlist->addMedia(QUrl::fromLocalFile(aFile));
               QFileInfo fileInfo(aFile);

               ui->listView->addItem(fileInfo.fileName()); //用于显示
           }
           if(player->state()!=QMediaPlayer::PlayingState)
           {
               playlist->setCurrentIndex(0);
           }

           player->play();






}

void MainWindowVideo::on_btn_volume_clicked()
{
    bool mte = player->isMuted();
    player->setMuted(!mte);

    if(mte){
        ui->btn_volume->setIcon(QIcon(":/new/prefix1/img/images/volume-down.png"));
    }else{
        ui->btn_volume->setIcon(QIcon(":/new/prefix1/img/images/volume-mute.png"));
    }
}

void MainWindowVideo::on_btn_openlist_clicked()
{
    ui->btn_closelist->show();
    ui->listView->show();
    ui->btn_openlist->hide();
}

void MainWindowVideo::on_btn_closelist_clicked()
{
    ui->btn_openlist->show();
    ui->listView->hide();
    ui->btn_closelist->hide();
}

void MainWindowVideo::onSliderValueChanged(int pos)
{
    player->setPosition(pos);
}

void MainWindowVideo::on_btn_last_clicked()
{

        M_Amount = playlist->mediaCount();
        if(M_Value == 0)
        {
            M_Value =M_Amount;
            playlist->setCurrentIndex(M_Value);
            player->play();
            return;
        }
        M_Value-=1;
        playlist->setCurrentIndex(M_Value);
        player->play();
}



void MainWindowVideo::on_btn_next_clicked()
{
    M_Amount = playlist->mediaCount();
    if(M_Value == M_Amount)
    {
        M_Value =0;
        playlist->setCurrentIndex(M_Value);
        player->play();
        return;
    }
    playlist->setCurrentIndex(++M_Value);
    player->play();
}

//void MainWindowVideo::on_listView_itemClicked(QListWidgetItem *item)
//{
//    QString str = item->text();
//    for (int i = 0;i<M_Amount;i++) {
//        playlist->setCurrentIndex(i);
//        if(playlist->)

//    }
//}

//void MainWindowVideo::on_listView_itemClicked(QListWidgetItem *item)
//{

//}
