#ifndef MAINWINDOWVIDEO_H
#define MAINWINDOWVIDEO_H

#include <QMainWindow>
#include <QUrl>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QMessageBox>
#include <QAudio>
#include <QAudioOutput>
#include <QMediaPlaylist>
#include <QListView>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowVideo; }
QT_END_NAMESPACE

class MainWindowVideo : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowVideo(QWidget *parent = nullptr);
    ~MainWindowVideo();

private slots:
    void on_btn_play_clicked();

    void on_btn_openfile_clicked();

    void on_btn_volume_clicked();

    void on_btn_openlist_clicked();

    void on_btn_closelist_clicked();

    void onSliderValueChanged(int pos);

    void on_btn_last_clicked();


    void on_btn_next_clicked();

    //void on_listView_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindowVideo *ui;
    QMediaPlayer *player;
    QGraphicsVideoItem *videoItem;
    QMediaPlaylist *playlist;
    QVideoWidget *playweight;
    int M_Amount=0;   //用来记录文件总数
    int M_Value=0;    //当前播放的文件标记

};
#endif // MAINWINDOW_H
