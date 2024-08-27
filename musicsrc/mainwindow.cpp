#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QSqlQuery>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QMessageBox>
#include <QFont>
#include <deque>
#include <QVariant>
#include <algorithm>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QBitmap>
#include <QHeaderView>
#include <QScrollBar>
#include <QSqlError>
#include <QPixmap>
#include <QLinearGradient>
#include <QBrush>
#include <QPalette>
using namespace std;

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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMysql();  // 初始化数据库连接
    createTables();  // 创建表
    Player = new QMediaPlayer();   // 实体化播放列表
    Playlist = new QMediaPlaylist();
    setBlackGoldMixedLinearGradientBackground();
    int x = 1025, y = 700;  //记录初始的宽和高
    this ->resize(x, y);   // 设置主界面的宽高
    setWindowFlag(Qt::FramelessWindowHint);  // 删除以前的 最大化 最小化 关闭自己写
    this ->setMinimumSize(QSize(x, y)); // 窗口最小 size

    this ->X = 63; // 初始化 X
    this ->V = 50; // 初始化 V

    this ->setWindowTitle("网抑云音乐");  //主窗口的运行文件名称
    this ->setWindowIcon(QIcon(":/coin/mymusic.png"));  // 主窗口的图标设置!
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);

    player->setPlaylist(playlist);

    QMenu *menu = new QMenu(this);
        menu->addAction("顺序播放", this, &MainWindow::setSequentialMode);
        menu->addAction("循环播放", this, &MainWindow::setLoopMode);
        menu->addAction("随机播放", this, &MainWindow::setRandomMode);

        // 将 QMenu 关联到按钮
        ui->playModeButton->setMenu(menu);
        ui->playModeButton->raise();
        ui->playModeButton->move(200,620);
        // 默认设置为顺序播放
        changePlayMode(currentMode);

    // 固定大小
    this ->setFixedSize(x, y);

    time = new QTimer(this);  // 初始化定时器

    //关于tablewidget 上面的名字
    list_col_table = QStringList() << " " << "歌曲" << "作者" << "专辑" << " " << " " << "时间";
    song_col_name = QStringList() << " "<< "歌曲" << " " << " " << " ";

    initMysql();
    // 以上关于mysql的初始化

    //关于字体

    QFont font, font_L;
    font.setFamily("幼圆");
    font.setPointSize(10);
    font_L.setFamily("Algerian"); // 将字体设置为艺术字，例如 Algerian
    font_L.setPointSize(10); // 设置字体大小为10
    font_L.setBold(true); // 设置加粗


     //窗口上方按钮的封装
     btnL = new QLabel(this);
     btnL ->setGeometry(0, 0, 1025, 70);
     btnL ->setStyleSheet("QLabel{background-color: rgb(255, 10, 10); border: solid;}");
     btnL ->setFont(font_L);
     btnL ->setText("                          网抑云音乐");


     search_line = new QLineEdit(btnL); // 搜索框
     search_line ->setGeometry(180, 18, 180, 40);
     search_line ->setStyleSheet("QLineEdit{border-style:none; padding:6px; border-radius: 50px;}");

     //登录按钮
     sign_in_btn = new mybtn(":/coin/sign_in.png", ":/coin/sign_in_c.png");
     sign_in_btn ->setParent(btnL);
     sign_in_btn ->move(750, 20);

    //搜索按钮
     btn_search = new mybtn(":/coin/search.png", ":/coin/search.png");
     btn_search ->setParent(btnL);
     btn_search ->move(360, 20);

     QPixmap pixmap(":/coin/mymusic.png");
     QPixmap scaledPixmap = pixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 缩小图标

     // 使用缩小后的图标创建 QIcon
     QIcon icon(scaledPixmap);

     // 设置按钮的图标
     winicon = new mybtn(":/coin/mymusic.png", ":/coin/mymusic.png");
     winicon->setParent(this);
     winicon->setIcon(icon);  // 这里设置缩小后的图标
     winicon->setIconSize(QSize(32, 32));  // 设置图标大小
     this->setLayout(nullptr); // 禁用布局管理器
     winicon->move(15, 12);
     winicon->raise();  // 确保按钮在最前面
     this->update();    // 强制更新主窗口布局

     qDebug() << "winicon pos:" << winicon->pos();
     qDebug() << "Parent size:" << this->size();

    // 播放按钮的创建
    playbt = new startbtn(":/coin/start.png", ":/coin/start_c.png", ":/coin/pause.png", ":/coin/pause_c.png");
    playbt->setParent(this);
    playbt ->move(478, 620);

    // 下一首 功能按钮的创建
    nextbt = new mybtn(":/coin/next.png", ":/coin/next_c.png");
    nextbt ->setParent(this);
    nextbt ->move(545, 623);

    // 上一首 功能的按钮的创建
    prevbt = new mybtn(":/coin/prev.png", ":/coin/prev_c.png");
    prevbt ->setParent(this);
    prevbt ->move(415, 623);

    // 音量功能 的按钮创建
    volbt = new startbtn (":/coin/vol.png", ":/coin/vol_c.png", ":/coin/unvol.png", ":/coin/unvol_c.png");
    volbt ->setParent(this);
    volbt ->move(640, 625);

    // 播放列表 按钮
    listbtn = new mybtn(":/coin/list.png", ":/coin/list_c.png");
    listbtn ->setParent(this);
    listbtn ->move(940, 625);

    //本地音乐 添加按钮
    local_btn = new mybtn(":/coin/local_add.png", ":/coin/local_add_c.png");
    local_btn ->setParent(this);  //基于this建立
    local_btn ->move(550, 300);
    local_btn ->hide(); // 先隐藏起来

    //minb init
    minb =new mybtn(":/coin/min.png", ":/coin/min_1.png");
    minb ->setParent(this);
    minb ->move(this ->width() - 98, 25);

    // maxb init
    maxb = new mybtn(":/coin/max.png", ":/coin/max_1.png");
    maxb ->move(this ->width() - 68, 25);
    maxb ->setParent(this);

    // clsb init
    clsb = new mybtn(":/coin/cls.png", ":/coin/cls_1.png");
    clsb ->setParent(this);
    clsb ->move(this ->width() - 38, 25);


    //将minb maxb clsb 加入到水平布局中.
    QHBoxLayout * hlay = new QHBoxLayout();    //设置minb maxb clsb 的水平布局让其对于窗口的相对位置不会改变
    hlay ->addWidget(minb);
    hlay ->addWidget(maxb);
    hlay ->addWidget(clsb);

    // "我的音乐"  标签
    mymusic = new mylab("我的音乐", this);
    mymusic ->setGeometry(0, 70, 160, 40);

    // 循环播放 按钮
    looper_m = new startbtn(":/coin/no_looper.png", ":/coin/no_looper.png", ":/coin/looper.png", ":/coin/looper.png");
    looper_m ->setParent(this);
    looper_m ->move(340, 627);


    // lrc_btn init
    lrc_btn = new mybtn(":/coin/lrc_word.png", ":/coin/lrc_word.png");
    lrc_btn ->setParent(this);
    lrc_btn ->move(760, 625);

    // 我的歌单标签
    mylist = new mylab("我的歌单", this);
    mylist ->setGeometry(0, 210, 160, 40);

    //add songlist init
    add_songlist = new mybtn(":/coin/add.png", ":/coin/add_c.png");
    add_songlist ->setParent(mylist);
    add_songlist ->move(mylist ->width() - 60, mylist ->height() - 30);

    // 用来显示当前播放歌曲
    musicL = new QLabel(this);
    musicL ->setGeometry(95, 620, 150, 50);
    musicL ->setFont(font);

    //用来显示当前播放歌曲的图片
    music_map = new QPushButton(this);
    music_map ->setGeometry(20, 625, 60, 60);
    music_map ->setFlat(true);


    // 登录标签
    sign_L = new QLabel(btnL);
    sign_L ->setText("未登录");
    sign_L ->setGeometry(790, 20, 100, 30);
    sign_L ->setFont(font);

    // 本地音乐的播放列表
    local_w = new QTableWidget(this);
    local_w ->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可编辑
    local_w ->setShowGrid(false); // 关闭网格
    local_w ->setFocusPolicy(Qt::NoFocus); //去掉虚线格
    local_w ->verticalHeader() ->setHidden(true); // 去掉表头行号
    local_w ->setColumnCount(5);//设置 列宽 和 列count


    local_w ->setColumnWidth(0, 40);
    local_w ->setColumnWidth(1, 150);
    local_w ->setColumnWidth(2, 40);
    local_w ->setColumnWidth(3, 40);
    local_w ->setColumnWidth(4, 575);
    local_w->hide();  //隐藏

    // 定义歌单的播放列表
    list_w = new QTableWidget(this);
    list_w ->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可编辑
    list_w ->setShowGrid(false); // 关闭网格
    list_w ->setFocusPolicy(Qt::NoFocus); //去掉虚线格
    list_w ->verticalHeader() ->setHidden(true); // 去掉表头行号
    list_w ->setColumnCount(6);//设置 列宽 和 列count

    list_w ->setColumnWidth(0, 40);
    list_w ->setColumnWidth(1, 150);
    list_w ->setColumnWidth(2, 40);
    list_w ->setColumnWidth(3, 40);
    list_w ->setColumnWidth(4, 40);
    list_w ->setColumnWidth(5, 550);
    list_w ->hide();  //隐藏

    //喜欢音乐
    like_w = new QTableWidget(this);
    like_w ->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可编辑
    like_w ->setShowGrid(false); // 关闭网格
    like_w ->setFocusPolicy(Qt::NoFocus); //去掉虚线格
    like_w ->verticalHeader() ->setHidden(true); // 去掉表头行号
    like_w ->setColumnCount(5);//设置 列宽 和 列count

    like_w ->setColumnWidth(0, 40);  // 爱心
    like_w ->setColumnWidth(1, 150); // 歌名
    like_w ->setColumnWidth(2, 40);  // 播放
    like_w ->setColumnWidth(3, 40); // 增添到其他歌单
    like_w ->setColumnWidth(4, 590);  // 空白
    like_w ->hide();  //隐藏


    // 我的音乐 的列表 窗口
    musiclist = new mylistwidget(this);
    musiclist ->setGeometry(0, 110, 160, 100);
    musiclist ->setFocusPolicy(Qt::NoFocus); // 去掉虚线框

     // 歌单列表
    songlist = new mylistwidget(this);
    songlist ->setGeometry(0, 250, 160, 350);
    songlist ->setFocusPolicy(Qt::NoFocus);


    // 歌词主窗口
    lrc = new lrc_win_main();
    lrc ->hide();

    //本地音乐列表
    mylocalmusic = new QListWidgetItem(musiclist);
    mylocalmusic ->setText("本地音乐");
    mylocalmusic ->setFont(font);

    // 歌曲的队列
    songqueue = new mytablewidget(this);
    songqueue ->setGeometry(1053, this ->height() - btnL ->height(), 250, 485 - btnL ->height());
    songqueue ->horizontalHeader()->setHidden(true); // 去掉表头
    songqueue ->setHorizontalHeaderLabels(song_col_name);

    // 喜欢的音乐列表
    likemusiclist = new QListWidgetItem(musiclist);
    likemusiclist ->setText("喜爱的音乐");
    likemusiclist ->setFont(font);

    // 显示搜索音乐的窗口
    tab_search = new QTableWidget();
    tab_search ->setParent(this);
    tab_search ->setColumnCount(7);  // 设置列数
    tab_search ->setHorizontalHeaderLabels(list_col_table);// 设置列数的 名字
    tab_search ->setGeometry(musiclist->width(), btnL ->height(), this ->width() - musiclist ->width(), this ->height() - btnL ->height() - 100);
    tab_search ->stackUnder(songlist); // 优先级在songlist 之下
    tab_search ->hide();

    tab_search ->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可编辑
    tab_search ->setShowGrid(false); // 关闭网格
    tab_search ->setFocusPolicy(Qt::NoFocus); //去掉虚线格
    tab_search ->verticalHeader() ->setHidden(true); // 去掉表头行号
    tab_search ->setStyleSheet("QTableWidget::Item::selected{background: white;}"
                               "QHeaderView::section{border: 0px solid white};"
                               "QTableWidget::Item::hover{background: black;}");


    // 歌词地基窗口
    lrc_w = new lrcwidget();
    lrc_w ->setGeometry(-this ->width(), btnL ->height(), this ->width(), this ->height() - btnL ->height() - 100);


    // 歌曲显示窗口
    lrcwin = new QTabWidget(this);
    lrcwin ->setGeometry(-this ->width(), btnL ->height(), this ->width(), this ->height() - btnL ->height() - 100);
    lrcwin ->addTab(lrc_w,"");
    lrcwin ->findChildren<QTabBar*>().at(0)->hide(); // 隐藏widget 上面的tab
    lrcwin ->show();


    //获得屏幕的分辨率
    QDesktopWidget* desk = QApplication::desktop();
    desk ->screenGeometry();

    // http init
    net_manager = new QNetworkAccessManager();
    net_request = new QNetworkRequest();
    network_manager2 = new QNetworkAccessManager();
    network_request2 = new QNetworkRequest();
    network_manager3 = new QNetworkAccessManager();
    network_request3 = new QNetworkRequest();

    //sign init
    sign = new sign_in_win();

    connect(minb, &QPushButton::clicked, [=]() {
        showMinimized();
    });   // 点击 minb 按钮 最小化

    // 关闭窗口按钮
    connect(clsb, &QPushButton::clicked, [=](){
       this ->close();
        lrc ->hide();
        Player ->stop();
        exit(0);
    });


    // 当点击playbt 时 开始 播放 或者 暂停音乐
    connect(playbt, &QPushButton::clicked, [=] () {
        if (!Playlist ->isEmpty()) {
            if (emit_i == 0) {
                emit beginplay();
                emit_i++;
            }
            int idx = Playlist ->currentIndex();
            QString textL =  nowlist[idx];
            musicL ->setText(getMName(textL) + "<br>" + getPName(textL)); //用来显示当前播放歌曲
            this ->setWindowTitle(textL); // 用来 改变窗口的 名称
            if (playf == false && nowlist.size() != 0) {
                Player ->play();
                this ->druntime = Player ->duration();  // 获得总时长
                time ->start();
                playf = true;
            } else {
                Player ->pause();
                time ->stop();
                playf = false;
            }
        } else {
            QMessageBox::information(this, "warning", "请录入音乐", QMessageBox::Ok);  // 提示用户应该录入 音乐
        }
    });


    // 当点击nextbt 时 播放下一首 音乐
    connect(nextbt, &QPushButton::clicked, [=](){
        if (!Playlist ->isEmpty()) {
            int idx = Playlist ->currentIndex();
            if(idx + 1 < nowlist.size()) {
                Player ->pause();
                Playlist ->next();
                QTimer ::singleShot(200, this, [=] () {
                    Player ->play();  // 播放
                });
                initPlayer(); // 初始化播放
                update();
            }
            else {
                Playlist ->setCurrentIndex(0);
                QTimer ::singleShot(200, this, [=] () {
                    Player ->play();  // 播放
                });
                update();
            }
        }
    });


    // 当点击prevbt 时 播放上一首 音乐
    connect(prevbt, &QPushButton::clicked, [=](){
        if (!Playlist ->isEmpty()) {
            int idx = Playlist ->currentIndex();
            if(idx - 1 >= 0) {
                Player ->pause();
                Playlist ->previous(); // 上一首
                QTimer ::singleShot(200, this, [=] () {
                    Player ->play();  // 播放
                });
                initPlayer(); // init 播放btn
                update();
            }
            else {
                Playlist ->setCurrentIndex(nowplaylist.size() - 1);
                QTimer ::singleShot(200, this, [=] () {
                    Player ->play();  // 播放
                });
                update();
            }
        }
    });


    // 当 点击 音量按钮时 volF = false 时 静音    volF = true 时 返回当时 的 音量
    connect(volbt, &QPushButton::clicked, [=] () {
        if (!volF) {
            volF = true;
            bufferVol = this -> V;   // 用来 记录还没有静音时 V 的值
            this ->V = 0;
            Player ->setVolume(V); // 音量变为 0;
            update(); // 重绘 painter
        } else  {
            volF =  false;
            this ->V= bufferVol;  // 将静音前 V 的值还给V
            Player ->setVolume(2 * V);
            update();
        }
    });


    //点击 listbtn 出现 播放列表
    connect(listbtn, &QPushButton::clicked, [=] () {
         songqueue ->raise();

        if (!is_open) {       // 打开 播放列表
            is_open = true;
            QPropertyAnimation *an  = new QPropertyAnimation(songqueue, "geometry");
            an ->setDuration(200);
            an ->setStartValue(QRect(1053, btnL ->height(), 250, this->height() - btnL ->height() - 100));
            an ->setEndValue(QRect(780, btnL ->height(), 250, this->height() - btnL ->height() - 100));
            an ->setEasingCurve(QEasingCurve::Linear);
            an ->start();
        } else {    // 关闭播放列表
            is_open = false;
            QPropertyAnimation* an = new QPropertyAnimation(songqueue, "geometry");
            an ->setDuration(200);
            an ->setStartValue(QRect(780, btnL ->height(), 250, this->height() - btnL ->height() - 100));
            an ->setEndValue(QRect(1053, btnL ->height(), 250, this->height() - btnL ->height() - 100));
            an ->setEasingCurve(QEasingCurve::Linear);
            an ->start();
        }
    });


    // 点击 add_songlist 触发信号
    connect(add_songlist, &mybtn::clicked, [=] () {
        if (user_id == -1) {
            QMessageBox::information(this, "提示", "请先登录", QMessageBox::Ok);
            return;
        }

        // list_create name init
        n_list = new create_list();
        n_list ->show();

        // 当输入名字完成后, 开始构造新的歌单
        connect(n_list, &create_list::create_ok, this, &MainWindow::songlist_add);
    });


    // 当点击 本地音乐item 时显示本地音乐
    connect(musiclist, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showlocal(QListWidgetItem*)));

    //如果 为空则添加录入音乐按钮
    connect(this, &MainWindow::file_null, [=] () {
        local_btn ->show(); // 显示 添加按钮的图标
    });

    //当点击btn_search 开始搜索
    connect(btn_search, &mybtn::clicked, this, [=] () {
        if(sign_in) {
            v_hash.clear();
            v_id.clear();
            index = 0;
            for(int i = 1; i <= 10; ++i) {
                search(search_line ->text(), i);
            }

            tab_search ->setColumnWidth(0, 5);
            tab_search ->setColumnWidth(1, (this ->width() - musiclist ->width()) / 5);
            tab_search ->setColumnWidth(2, (this ->width() - musiclist ->width()) / 5);
            tab_search ->setColumnWidth(3, (this ->width() - musiclist ->width()) / 5);
            tab_search ->setColumnWidth(4, 15);
            tab_search ->setColumnWidth(5, 15);
            tab_search ->setColumnWidth(6, (this ->width() - musiclist ->width()) / 5 + 55);

            tab_search ->show();  // 显示搜索的结果
        } else QMessageBox::information(this, "提示", "请先登录", QMessageBox::Ok);
    });

    //登录按钮的响应
    connect(sign_in_btn, &mybtn::clicked, [=] () {
       sign ->show();
    });


    //添加 按钮的功能
    connect(local_btn, &mybtn::clicked, [=] () {
        this ->filem = QFileDialog::getExistingDirectory(this,"", "",QFileDialog::ShowDirsOnly);
        filemlist = getfileName(this ->filem);   // 用来存储载入歌曲的 歌名
        if(this ->filem.size() == 0) return;

        // 如果 filem 有效 那么插入数据库 并且绘制item
        QSqlQuery sql;
        QString user_id_qstr = QString("%1").arg(user_id);
        sql.exec("insert into localmusic values(" + user_id_qstr + ",\"" + this ->filem + "\");");
        local_w ->setGeometry(musiclist ->width(), btnL->height(), this->width() - musiclist ->width(), this ->height() - btnL->height() - 100);
        local_w ->setStyleSheet("QListWidget::Item{height: 60px;}");
        local_w ->setHorizontalHeaderLabels(QStringList() << " " << "歌曲" << " " << " " << " ");
        localinit(local_w); // init QlistWidget
        local_w ->show();
        tab_search ->hide();
    });

    // 如果登录成功
    connect(sign, &sign_in_win::sign_in_success, [=] () {
        // 登录成功~
         sign_in = true;
         //先清空初始化整个播放器
         songqueue ->clear();
         songqueue ->setRowCount(0);

         local_w ->clear();
         local_w ->setRowCount(0);

         like_w ->clear();
         like_w ->setRowCount(0);

         songlist ->clear();

         nowplaylist.clear();  // 初始化nowplay
         nowlist.clear();
         nowlist_im.clear();
         nowlist_lrc.clear();
         filem.clear(); // 本地文件目录清空
         local_w ->hide();
         time ->stop(); // 计时器也停止
         Player ->stop(); // 音乐停止

         Playlist = new QMediaPlaylist();
         Player = new QMediaPlayer();

         // 导入id 开始导入对应的歌曲 和 歌单
         this ->user_id = sign ->user_id;
         this ->song_list_id = sign ->song_list_cnt;
         innowplay(); // 初始化 该用户的歌单
         initsonglist(); // 初始化 该用户的自定义歌单
         init_local(); // 初始化 本地音乐
         init_like(); // init 喜爱音乐
         sign_L ->setText(sign ->user_name ->text());
    });

    // 点击 music_map 打开 歌词窗口
    connect(music_map, &QPushButton::clicked, [=] () {
        if(!lrc_open) {
            lrc_open = true;
            QPropertyAnimation *an = new QPropertyAnimation(lrcwin, "geometry");
            an ->setDuration(300);
            an ->setStartValue(QRect(-lrcwin ->width(), lrcwin ->y(), lrcwin->width(), lrcwin->height()));
            an ->setEndValue(QRect(0, lrcwin->y(), lrcwin->width(), lrcwin->height()));
            an ->setEasingCurve(QEasingCurve::Linear);
            an ->start();
        } else {
            lrc_open = false;
            QPropertyAnimation *an = new QPropertyAnimation(lrcwin, "geometry");
            an ->setDuration(300);
            an ->setStartValue(QRect(0, lrcwin->y(), lrcwin->width(), lrcwin->height()));
            an ->setEndValue(QRect(-lrcwin ->width(), lrcwin ->y(), lrcwin->width(), lrcwin->height()));
            an ->setEasingCurve(QEasingCurve::Linear);
            an ->start();
        }
    });


    // 点击 songlist item 显示 该自定义歌单的音乐
    connect(songlist, &QListWidget::itemClicked, this, &MainWindow::show_List_music);


    //songqueue 点击触发
    connect(songqueue, &QTableWidget::cellClicked, this, &MainWindow::songqueue_fun);

    //local 点击触发
    connect(local_w, &QTableWidget::cellClicked, this, &MainWindow::local_fun);

    //like 点击触发
    connect(like_w, &QTableWidget::cellClicked, this, &MainWindow::fun_like_w);

    // list_w 点击触发
    connect(list_w, &QTableWidget::cellClicked, this, &MainWindow::fun_list_w);

    //改变模式
    connect(looper_m, &startbtn::clicked, [=] () {
        is_looper = 1 ^ is_looper;  // 变成循环
    });

    //lrc_btn 点击显示歌词床口
    connect(lrc_btn, &mybtn::clicked, [=] () {
        if (is_show) {
            is_show = false;
            lrc ->hide();
        } else {
            is_show = true;
            lrc ->show();
        }
    });

    // 关于http 网络歌曲的 json 解析 和相应播放
    connect(net_manager, &QNetworkAccessManager::finished, this, &MainWindow::reply);
    connect(network_manager2, &QNetworkAccessManager::finished, this, &MainWindow::reply2);
    connect(network_manager3, &QNetworkAccessManager::finished, this, &MainWindow::reply3);
    connect(tab_search, &QTableWidget::cellDoubleClicked, this, &MainWindow::play_net_Music);


    initPro(); // 开始初始化进度条
}

//###################################################################################################################################

// 初始化进度条
void MainWindow::initPro() {
    QPalette pal; //  调色板
    pal.setColor(QPalette::WindowText,QColor(0,0,0)); // 调颜色
    QFont font("Courier",10); // 设置字体

    liftLabel=new QLabel(this);
    rightLabel=new QLabel(this);
    // 创建 标签

    liftLabel->setAlignment(Qt::AlignCenter);
    rightLabel->setAlignment(Qt::AlignCenter);
    // text 居中

    liftLabel->setGeometry(160, 643,50,50);
    rightLabel->setGeometry(815,643, 50,50);
    // 设置位置 和 大小

    rightLabel->setFont(font);
    liftLabel->setFont(font);
    // 设置 字体

    liftLabel->setPalette(pal);
    rightLabel->setPalette(pal);
    liftLabel ->setText("00:00");
    rightLabel ->setText("00:00");
    // 设置 颜色


    // 定时器
    connect(this, &MainWindow::beginplay, [=] () {
        // 歌词动态播放
        if(!Playlist ->isEmpty()) {
            time -> setInterval(1000); // 1000 毫秒触发一次
            connect(time, &QTimer::timeout, this, [=]() {
                // 歌词动态滚动
                if(lrcMap.size() != 0) {
                    for(QMap<int, QString>::iterator j = lrcMap.begin(); j != lrcMap.end(); ++j) {
                        bool is_back = false; // is_back 为true时 退出
                        if(j != lrcMap.end() && j.key() <= this ->positontime + 1000 && j.key() >= this ->positontime - 1000) {
//                            qDebug() << j.key() << endl;
                            QFont font("幼圆"), p_font("幼圆"); // font 目前播放的歌词 p_font之前播放的歌词 恢复原样
                            // 放大正在播放的歌词
                            font.setWeight(90);
                            font.setPointSize(15);

                            //恢复已经放大过的歌词
                            p_font.setWeight(50);
                            p_font.setPointSize(13);

                            int idx = lrc_idx[j.key()]; // 找到item
                            int p_idx = p_lrcit == lrcMap.begin() - 1 ? -1 : lrc_idx[p_lrcit.key()]; // 找到上一个item
                            lrc ->l ->setText(lrc_itm[idx] ->text()); // 将内嵌歌词窗口的 歌词 给主窗口歌词
                            //恢复之前歌词字体大小
                            if(p_idx >= 0) {
                                lrc_itm[p_idx] ->setFont(p_font);
                                lrc_itm[p_idx] ->setText(p_lrcit.value());
                            }
                            //新的 p_lrcit
                            p_lrcit = j;

                            //滚动播放歌词
                            if(idx >= 4) lrc_l ->verticalScrollBar() ->setValue(idx - 4);
                            lrc_itm[idx] ->setFont(font);
                            lrc_itm[idx] ->setText(j.value());
                            is_back = true;
                        }
                        if(is_back) break;
                    }
                }
                //进度条更新
                updatepos();
                // 当Player 播放完最后一首时 让palylist 的index 变成0, Qt的QPlayMedia 真是博大精深
                if (Playlist ->currentIndex() == Playlist ->mediaCount() - 1 && Player ->duration() / 1000 == this ->positontime / 1000) {
                    Playlist ->setCurrentIndex(0);
                }
            });  // 1秒 触发一次 updatepos();
        }
    });
}


void MainWindow::initMysql() {
       db = QSqlDatabase::addDatabase("QSQLITE");
       db.setDatabaseName("musicplayer.db");

       if (!db.open()) {
           qDebug() << "Failed to open database:" << db.lastError().text();
       } else {
           qDebug() << "Database connected!";
       }

}

// 用来 单独封装控制 播放
void MainWindow::initPlayer() {
    int _idx = Playlist ->isEmpty() ? 0 : Playlist ->currentIndex();  //获取 播放位置
    QString textL =  nowlist[_idx];
    musicL ->setText(getMName(textL) + "<br>" + getPName(textL)); //用来显示当前播放歌曲
    this ->setWindowTitle(textL); // 用来 改变窗口的 名称

    if (playf == false && filemlist.size() != 0) {
        playbt ->is_play = true;
        playbt ->setIcon(QIcon(":/coin/pause.png"));  // 改变图标
        this ->druntime = Player ->duration();  // 获得总时长
        playf = true;
    }
}

//让QstringList 获得所有选中的歌曲文件路径
QStringList MainWindow::getfileName(const QString& file) {
       QDir dir(file);
       QStringList filelist;
       filelist << "*.mp3";
       QStringList res = dir.entryList(filelist, QDir::Files | QDir::Readable, QDir::Name);
       return res;
}


void MainWindow::showPlayMedia() {
    reinit(1);  //初始化
    //判断是不是net_music
    int net_idx = Playlist->currentIndex() == -1 && Playlist ->mediaCount() == 1 ? 0 : Playlist->currentIndex();
    if(Playlist ->currentIndex() == -1 && Playlist ->mediaCount() == 1) Playlist ->setCurrentIndex(0);  // 如果index 变成-1了 那么直接setCurrentIndex 为0
    if (net_idx == -1 && Playlist ->mediaCount() == 0) return; // 防止索引越界

    bool net = is_net_music(nowplaylist[net_idx]);
    int idx = Playlist ->currentIndex();
    QString textL =  nowlist[idx];
    //不同类型的 歌曲显示的名字不同
    if(net) {
        network_request3->setUrl(QUrl(nowlist_im[idx]));
        network_manager3->get(*network_request3);  // 反馈信号 准备解析json
        buildlrc(nowlist_lrc[idx]);
    } else {
        // 载入本地音乐的图片
        local_img(":/coin/songer.png");
        buildlrc(nowlist_lrc[idx]);
    }

    musicL ->setText(getMName(textL) + "<br>" + getPName(textL));
    this ->setWindowTitle(textL);  // 用来 改变窗口的 名称
}  // 改变label 显示新的 歌曲名字




void MainWindow::paintEvent(QPaintEvent *event) {
    //绘制分割线
    QPainter paint(this);
    paint.setPen(QColor(Qt::black));
    paint.drawLine(musiclist ->width(), btnL ->height(), musiclist ->width(), this ->height() - btnL->height() - 30);
    paint.drawLine(0, this ->height() - btnL->height() - 30, this ->width(), this ->height() - btnL->height() - 30);


    Q_UNUSED(event);  // 避免编译器警告
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);  // 使用平滑的pixmap变换算法
    painter.setRenderHint(QPainter::Antialiasing, true);   // 反走样

    QColor baseColor(0,20,20);  // 进度条基底颜色
    QColor inColor(255,0,0);      // 显示播放了多少的红色进度条
    QColor outColor(174, 205, 210);  // 小球

//     qDebug() << "X is : " << this ->X << endl;
    // 进度条的基底
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(baseColor);
    QRectF rect=QRectF(220,665,580,6);
    painter.drawRoundedRect(rect,3,3);
    painter.restore();

    // 在基底上 加入红色进度条
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(inColor);
    QRectF playrect = QRectF(220,665, X - 63, 6);
    painter.drawRoundedRect(playrect,0,3);
    painter.restore();

    // 加入进度条上的 圆形
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(outColor);
    painter.drawEllipse(X + 150, 658,20,20);
    painter.restore();

    // 在圆形上加入红点
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(inColor);
    painter.drawEllipse(X + 157, 665, 6, 6);
    painter.restore();


    // 构造 声音进度条
    QPainter volumeP(this);
    volumeP.setRenderHint(QPainter::SmoothPixmapTransform);  // 使用平滑的pixmap变换算法
    volumeP.setRenderHint(QPainter::Antialiasing, true);   // 反走样

    // 同绘制 进度条一样
    volumeP.save();
    volumeP.setPen(Qt::NoPen);
    volumeP.setBrush(baseColor);
    QRectF baserect = QRectF(670, 635, 50, 4);
    volumeP.drawRoundedRect(baserect, 3, 3);
    volumeP.restore();

    volumeP.save();
    volumeP.setPen(Qt::NoPen);
    volumeP.setBrush(inColor);
    QRectF inrect = QRectF(670, 635, V, 4);
    volumeP.drawRoundedRect(inrect, 0, 3);
    volumeP.restore();

    volumeP.save();
    volumeP.setPen(Qt::NoPen);
    volumeP.setBrush(inColor);
    volumeP.drawEllipse(V + 670, 633, 8, 8);
    volumeP.restore();
}

void MainWindow::setupUI() {
    // 设置主窗口的布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    // 创建播放模式选择按钮
    playModeButton = new QPushButton("选择播放模式", this);

    // 创建菜单并添加三个播放模式的选项
    playModeMenu = new QMenu(this);
    playModeMenu->addAction("顺序播放", this, SLOT(setSequentialMode()));
    playModeMenu->addAction("循环播放", this, SLOT(setLoopMode()));
    playModeMenu->addAction("随机播放", this, SLOT(setRandomMode()));

    // 将菜单与按钮关联
    playModeButton->setMenu(playModeMenu);
    playModeButton->raise();
    // 添加按钮到布局中
    mainLayout->addWidget(playModeButton);
    mainLayout->addStretch();  // 用来占位，使得按钮靠左下角
}

void MainWindow::changePlayMode(QMediaPlaylist::PlaybackMode mode) {
    currentMode = mode;  // 赋值给 currentMode
    Playlist->setPlaybackMode(currentMode);

    // 更新按钮文本为当前选择的模式
    switch (mode) {
            case QMediaPlaylist::Sequential:
                ui->playModeButton->setText("顺序播放");
                break;
            case QMediaPlaylist::Loop:
                ui->playModeButton->setText("循环播放");
                break;
            case QMediaPlaylist::Random:
                ui->playModeButton->setText("随机播放");
                break;
            default:
                break;
    }
}

void MainWindow::updatepos() {
    if (is_change == 1) time ->stop();


    // druntime: 总进度 positontime: 当前进度
    this ->positontime += 1000; // 每过一秒 加一秒
    float a;
    a = (float)positontime / (float)Player ->duration();
     X = a * 580 + 58;
    if (X > 635) X = 635;

    qint64 time = Player ->duration();  // 毫秒单位
    double secondDouble = time / 1000;  // 转化成秒
    int minint = secondDouble / 60;      // 总分钟
    int secondint = secondDouble - minint * 60;  // 总秒

    double secondtime1 = positontime / 1000;    // 当前播放的总秒数
    int minint1 = secondtime1 / 60;                // 当前播放的分钟
    int secondint1 = secondtime1 - minint1 * 60; // 当前播放的秒

    int curra = minint1 / 10;
    int currb = minint1 % 10;
    int currc = secondint1 / 10;
    int currd = secondint1 % 10;

    int alla = minint / 10;
    int allb = minint % 10;
    int allc = secondint / 10;
    int alld = secondint % 10;
    if (is_looper == 1 && curra == alla && currb == allb && currc == allc && currd == alld) {
        Player ->stop();
        int idx = Playlist ->currentIndex();
        Playlist ->setCurrentIndex(idx);
        Player ->play();
        this ->positontime = 0;
    }
    current = QString("%0%1:%2%3").arg(curra) .arg(currb).arg(currc). arg(currd);
    all = QString("%0%1:%2%3").arg(alla) .arg(allb) .arg(allc) .arg(alld);
    rightLabel->setText(all);
    liftLabel->setText(current);
    update();
    if (is_change == 1) {
        this ->time ->start();
        is_change = 0;
    }
}

void MainWindow::createTables()
{
    QSqlQuery query;
        // 创建用户表
        query.exec("CREATE TABLE IF NOT EXISTS user ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "username TEXT UNIQUE, "
                   "password TEXT)");

        // 创建播放列表表
        query.exec("CREATE TABLE IF NOT EXISTS playlist ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "user_id INTEGER, "
                   "song_path TEXT, "
                   "song_name TEXT, "
                   "album_name TEXT, "
                   "FOREIGN KEY(user_id) REFERENCES user(id))");

        // 创建喜欢的音乐表
        query.exec("CREATE TABLE IF NOT EXISTS liked_songs ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "user_id INTEGER, "
                   "song_name TEXT, "
                   "FOREIGN KEY(user_id) REFERENCES user(id))");
}


// 初始化X
void MainWindow::reinit(int status) {
    time ->stop();
    this ->druntime = Player ->duration();   // 获得总时长
    this ->positontime = 0; // 初始化
    this -> X = 63;
    if(status == 1) time ->start();
}

// 初始化 tablewidgetitem
void MainWindow::boxitem(int i, QString text, QTableWidget* lw) {
     if(text == "") {
        qDebug() << "null filename!" << endl;
        return;
     }

     QFont font;
     font.setFamily("幼圆");
     font.setPointSize(10);

      // 新的一首歌
     QString song_like = is_like(nowlist[i]);
      lw ->insertRow(i);
      lw ->setRowHeight(i, 45);


      for(int j = 0; j <= 4; ++j) lw ->setItem(i, j, new QTableWidgetItem());

      if(song_like == "0") lw ->item(i, 0) ->setIcon(QIcon(":/coin/like_c.png"));
      else lw ->item(i, 0) ->setIcon(QIcon(":/coin/like.png"));
      lw ->item(i, 1) ->setText(getPName(text) + "\n" + getMName(text));
      lw ->item(i, 2) ->setIcon(QIcon(":/coin/begin.png"));
      lw ->item(i, 3) ->setIcon(QIcon(":/coin/delete.png"));
      lw ->item(i, 4) ->setIcon(QIcon(":/coin/add.png"));
}


// 加入到歌曲队列
void MainWindow::queuefun(mytablewidget* lw, QString file, QString m_name) {
    bool is_http = is_net_music(file);
    int idx = Playlist ->isEmpty() ? -1 : Playlist ->currentIndex();
    if(file != "") {
        if(!is_http) Playlist ->insertMedia(idx + 1, QUrl::fromLocalFile(file));// 加入到多媒体 插入到下一首歌后面
        else Playlist ->insertMedia(idx + 1, QUrl(file));
        Player ->setPlaylist(Playlist);
        Playlist ->setCurrentIndex(idx + 1);  // 重新设置 多媒体位置
        time ->setInterval(1000);
        if(!is_http) {
            Player ->play();
            reinit(1); // 初始化
        } else {
            reinit(0);
        }
    }
    boxitem(idx + 1, m_name, lw);  // 加入队列 增加 item

}

// 歌手名字
QString MainWindow::getPName(QString file) {
    bool is_ok = false;
    QString res;
    for(int i = 0; i < file.size(); ++i) {
       if(is_ok && file[i] != ' ') res += file[i];
       if(file[i] == '-') is_ok = true;
       if(i + 1 < file.size() && file[i + 1] == '.') break;
    }
    return res;
}

// 音乐名字
QString MainWindow::getMName(QString file) {
    QString res;
    for(int i = 0; i < file.size(); ++i) {
        res += file[i];
        if(i + 1 < file.size() && file[i + 1] == '-') break;
    }
    return res;
}

void MainWindow::showlocal(QListWidgetItem* i) {
    if(user_id == -1) {
        QMessageBox::information(this, "提示", "请先登录", QMessageBox::Ok); // 提醒用户先登录 在使用
        return;
    }

//    qDebug() << "filem is :   "  << this ->filem << endl;
    int idx = musiclist ->row(i);  //点击的是我的音乐的第几行item （共两行）
    if (idx == 0) {  // idx == 0 说明点击的时 本地音乐
        if (this ->filem.size() != 0) local_w ->show();
        like_w ->hide();
        list_w ->hide();
        tab_search ->hide();
    }

    if (idx == 1) { // idx == 1 说明点击的时 喜爱音乐
        like_w ->show();
        local_w ->hide();
        list_w ->hide();
        tab_search ->hide();
    }
}

// init 本地音乐 当点击item时 播放 那个item 对应的歌曲
void MainWindow::localinit(QTableWidget* lw) {
    for(int i = 0; i < filemlist.size(); ++i) {
        if(filemlist[i] == "") {
           qDebug() << "null filename!" << endl;
           return;
        }
        QString local_like = is_like(filemlist[i]);

        QFont font;
        font.setFamily("幼圆");
        font.setPointSize(10);

         // 新的一首歌
         int row = lw ->rowCount();
         lw ->setFont(font);
         lw ->setRowCount(row + 1);
         lw ->setRowHeight(row, 45);

         for(int j = 0; j < 5; ++j) lw ->setItem(row, j,  new QTableWidgetItem());

         if(local_like == "0") lw ->item(row, 0) ->setIcon(QIcon(":/coin/like_c.png"));
         else lw ->item(row, 0) ->setIcon(QIcon(":/coin/like.png"));

         lw ->item(row, 1) ->setText(filemlist[i]);
         lw ->item(row, 2) ->setIcon(QIcon(":/coin/begin.png")); // 播放
         lw ->item(i, 3) ->setIcon(QIcon(":/coin/add.png"));
    }  // 录入本地音乐text
}

// 从数据库读出当前播放音乐的路径
void MainWindow::innowplay() {
    // 将mysql中的音乐路径打印出来
    QSqlQuery selectq;
    selectq.exec("select * from nowplay;");
    // 开始导入 音乐队列(路径 和 歌名)
    while (selectq.next()) {
        if (user_id == selectq.value(0).value<int>()) {
            this ->nowplaylist.push_back(selectq.value(1).value<QString>()); // 歌曲路径
            this ->nowlist.push_back(selectq.value(2).value<QString>()); // 歌曲名字
            this ->nowlist_im.push_back(selectq.value(3).value<QString>()); // 歌曲图片
            this ->nowlist_lrc.push_back(selectq.value(4).value<QString>()); // 歌曲歌词
        }
    }

    //初始化播放列表
    for(int i = 0; i < nowplaylist.size(); ++i) {
         readmysql(songqueue, nowplaylist[i], nowlist[i]); // 开始将歌曲一个一个的加入播放队列
    }

    // 初始化第一首歌
    if (nowplaylist.size() > 0) {
        bool is_net = is_net_music(nowplaylist[0]);
        if(is_net) {
            network_request3->setUrl(QUrl(nowlist_im[0]));  //设置歌曲图片
            network_manager3->get(*network_request3);  // 反馈信号 准备解析json
        } else {
            local_img(nowlist_im[0]);
        }
    }
    // 当音乐改变初始化 进度条 以及 显示音乐标签
    connect(Player, &QMediaPlayer::currentMediaChanged, this, [=]() {
        reinit(1);
        MainWindow::showPlayMedia();
     });
}


// songlist init
void MainWindow::initsonglist() {
    QFont font_l;
    font_l.setFamily("幼圆");
    font_l.setPointSize(11);
    songlist->setFont(font_l);

    QString user_id_s = QString("%0").arg(user_id);
    QSqlQuery songlist_sql;
    songlist_sql.exec("select name from listname where id = " + user_id_s + ";");
    while(songlist_sql.next()) {
        QString listname =  songlist_sql.value(0).value<QString>();
        songlist ->addItem(new QListWidgetItem(listname));
    }

    connect(songlist, &mylistwidget::itemClicked, this, &MainWindow::show_list_music);  // 点击 不同的歌单 显示不同的歌曲
}


// 显示 该自定义歌单的歌曲
void MainWindow::show_list_music(QListWidgetItem *item) {
    int listsong_id;
    QSqlQuery sql(db);
    QString text = item ->text();

    sql.exec("select * from listname where name = \"" + text + "\"and id = " + QString("%0").arg(user_id) + ";");
    while(sql.next()) {
        listsong_id = sql.value(2).value<int>();
        break;
    }

    list_w->clear();
    list_w ->setRowCount(0);
    list_w ->setGeometry(musiclist->width(), btnL->height(),this ->width() - musiclist->width(), this ->height() - btnL->height() - 100);
    list_w ->setHorizontalHeaderLabels(QStringList() << " " << "歌曲" << " " << " " << " " << " ");
    list_w ->setStyleSheet("QTableWidget::Item::selected{background: white;}"
                            "QHeaderView::section{border: 0px solid white};");
    sql.exec("select name from songlist where id = " + QString("%0").arg(user_id) + " and listid = " + QString("%0").arg(listsong_id) + ";");
    while(sql.next()) {
        int row = list_w ->rowCount();
        list_w ->setRowCount(row + 1);
        for(int i = 0; i < 5; ++i) list_w ->setItem(row, i, new QTableWidgetItem());

        QString like = is_like(sql.value(0).value<QString>());
        if(like == "0") list_w ->item(row, 0) ->setIcon(QIcon(":/coin/like_c.png"));
        else list_w ->item(row, 0) ->setIcon(QIcon(":/coin/like.png"));

        list_w ->item(row, 1) ->setText(sql.value(0).value<QString>());
        list_w ->item(row, 2) ->setIcon(QIcon(":/coin/begin.png"));
        list_w ->item(row, 3) ->setIcon(QIcon(":/coin/delete.png"));
        list_w ->item(row, 4) ->setIcon(QIcon(":/coin/add.png"));
    }
}


// local init
void MainWindow::init_local() {
    // 将mysql中的音乐路径打印出来
    QSqlQuery selectq;
    selectq.exec("select * from localmusic;");

    // 开始 录入此用户id 对应的 音乐文件
    while(selectq.next()) {
        if(user_id == selectq.value(0).value<int>()) {
            this ->filem = selectq.value(1).value<QString>();
//            qDebug() << "its id filem is:   " << this ->filem << endl;
            break;
        }
    }

    if(this ->filem.size() == 0) {
        emit file_null();
        local_w ->hide();
        return;
    }

    local_w ->clear();
    local_w ->setRowCount(0);

    filemlist = getfileName(this ->filem);   // 用来存储载入歌曲的 歌名
    local_w ->setGeometry(musiclist->width(), btnL->height(),this ->width() - musiclist->width(), this ->height() - btnL->height() - 100);
    local_w ->setHorizontalHeaderLabels(QStringList() << " " << "歌曲" << " " << " " << " ");
    local_w ->setStyleSheet("QTableWidget::Item::selected{background: white;}"
                            "QHeaderView::section{border: 0px solid white};");

    localinit(local_w); // init QlistWidget
}

// like_w init
void MainWindow::init_like() {
    like_w ->clear();
    like_w ->setRowCount(0);

    like_w ->setGeometry(musiclist->width(), btnL->height(),this ->width() - musiclist->width(), this ->height() - btnL->height() - 100);
    like_w ->setHorizontalHeaderLabels(QStringList() << " " << "歌曲" << " " << " " << " ");
    like_w ->setStyleSheet("QTableWidget::Item::selected{background: white;}"
                           "QHeaderView::section{border: 0px solid white};");

    //init mymusic list
    QFont font;
    font.setFamily("幼圆");
    font.setWeight(10);

    QSqlQuery sql;
    sql.exec("select * from liketb;");
    while(sql.next()) {
        if(sql.value(0).value<int>() == user_id) {
            int row = like_w->rowCount();
            like_w ->setRowCount(row + 1);
            for(int i = 0; i <= 3; ++i) like_w ->setItem(row, i, new QTableWidgetItem());

            like_w ->item(row, 0) ->setIcon(QIcon(":/coin/like.png"));
            like_w ->item(row, 1) ->setText(sql.value(2).value<QString>());
            like_w ->item(row, 2) ->setIcon(QIcon(":/coin/begin.png")); //播放
            like_w ->item(row, 3) ->setIcon(QIcon(":/coin/add.png")); // 添加到歌单
        }
    }
}

 // 插入到数据库 和 音乐队列中
void MainWindow::insert_nowplay(QString name, QString m_name, QString p_name, QString lrc_name) {
    update(); // 刷新
    bool is_http = is_net_music(name);
    QString user_id_qstr = QString("%0").arg(user_id);
    // 如果播放列表已经存在这首歌 那么只需要完成相对应的插入操作
    if (nowlist.count(m_name) != 0) {
        int idx = std::find(nowlist.begin(), nowlist.end(), m_name) - nowlist.begin(); // 找到这首歌的索引
        int c_idx = Playlist ->currentIndex() > idx ? Playlist ->currentIndex() - 1 : Playlist ->currentIndex(); // 当前播放的索引

        if(Playlist ->currentIndex() == idx) return; // 如果你想插入的歌就是 现在的歌 那么不需要插入

       // 重新插入歌曲
        if(idx == 0 && c_idx == Playlist ->mediaCount() - 2) {
            Playlist ->setCurrentIndex(Playlist ->currentIndex() - 1);
            if(!is_http) {
                Playlist ->addMedia(QUrl::fromLocalFile(nowplaylist[idx]));
                // 载入本地音乐的图片
                local_img(p_name);
            } else {
                Playlist ->addMedia(QUrl(nowplaylist[idx]));
                network_request3->setUrl(QUrl(p_name));
                network_manager3->get(*network_request3);  // 反馈信号 准备解析json
            }
            Playlist ->removeMedia(0);
            QTimer::singleShot(200, [=] () {
               Player ->play();    // 就是 qt player 这个机制过于博大精深 我吐了真的.......不得不这样写了
            });
        } else {
            Playlist ->removeMedia(idx);
            if(!is_http) {
                Playlist ->insertMedia(c_idx + 1, QUrl::fromLocalFile(nowplaylist[idx]));
                // 载入本地音乐的图片
                local_img(p_name);
            } else {
                Playlist ->insertMedia(c_idx + 1, QUrl(nowplaylist[idx]));
                network_request3->setUrl(QUrl(p_name));
                network_manager3->get(*network_request3);  // 反馈信号 准备解析json
            }
            Playlist ->setCurrentIndex(c_idx + 1);
            QTimer::singleShot(200, [=] () {
               Player ->play();    // 就是 qt player 这个机制过于博大精深 我吐了真的.......不得不这样写了
            });
        }


        // 重新插入 item
        songqueue ->removeRow(idx);
        buildlrc(lrc_name);
        boxitem(c_idx + 1, nowlist[idx], songqueue);  // 加入队列 增加 item

        //重新插入nowlist nowplaylist nowlist_im
        QString nowlist_s = nowlist[idx];
        QString nowplaylist_s = nowplaylist[idx];
        QString nowlist_im_s = nowlist_im[idx];
        QString nowlist_lrc_s = nowlist_lrc[idx];
        nowlist.erase(nowlist.begin() + idx, nowlist.begin() + idx + 1);
        nowplaylist.erase(nowplaylist.begin() + idx, nowplaylist.begin() + idx + 1);
        nowlist_im.erase(nowlist_im.begin() + idx, nowlist_im.begin() + idx + 1);
        nowlist_lrc.erase(nowlist_lrc.begin() + idx, nowlist_lrc.begin() + idx + 1);

        nowlist.insert(nowlist.begin() + c_idx + 1, nowlist_s);
        nowplaylist.insert(nowplaylist.begin() + c_idx + 1, nowplaylist_s);
        nowlist_im.insert(nowlist_im.begin() + c_idx + 1, nowlist_im_s);
        nowlist_lrc.insert(nowlist_lrc.begin() + c_idx + 1, nowlist_lrc_s);
        return;
    }

    // 如果is_delete = false 那么调用readmysql 里面的deletenowplay 否则调用queuefun里面的deletenowplay
    is_delete = true;
    // 插入到数据库中
    QSqlQuery sql;
    QString sfile = QString("insert into nowplay values(" + user_id_qstr + ",\"" + name + "\"," + "\"" + m_name + "\"" + ",\"" + p_name + "\"" + ",\"" + lrc_name + "\")"); // 插入从本地音乐目录路径
    sql.exec(sfile); // 执行
    if(nowlist.count(m_name) == 0) {
        if(!is_http) {
            QPixmap pix; // 载入本地音乐的图片
            local_img(p_name);
        } else  {
            network_request3->setUrl(QUrl(p_name));
            network_manager3->get(*network_request3);  // 反馈信号 准备解析json
        }
        int idx = Playlist ->currentIndex(); // 插入到此时播放音乐的后面
        nowplaylist.insert(nowplaylist.begin() + idx + 1, name);
        nowlist.insert(nowlist.begin() + idx + 1, m_name);
        nowlist_im.insert(nowlist_im.begin() + idx + 1, p_name);
        nowlist_lrc.insert(nowlist_lrc.begin() + idx + 1, lrc_name);
        queuefun(songqueue, name, m_name); //插入新的歌曲
    }
}


// 获取当前音乐文件的文件名
QString MainWindow::getname(QString file) {
    QString ans;
    for(int i = file.size() - 1; i >= 0; --i) {
        if(file[i] != '\\' && file[i] != '/') ans.push_front(file[i]);
        else break;
    }
    return ans;
}


// 初始化音乐队列 (仅在开启播放器的调用)
void MainWindow::readmysql(mytablewidget* lw, QString file, QString name) {
//    qDebug() << "in readmysql!!" << endl;
    int idx = Playlist ->isEmpty() ? 0 : Playlist ->mediaCount();
    is_net = is_net_music(file); // 判断是不是 网络音乐
    if(file != "" && nowlist_lrc.size() != 0) {
        if(!is_net) Playlist ->addMedia(QUrl::fromLocalFile(file));// 加入到多媒体 插入到下一首歌后面
        else Playlist ->addMedia(QUrl(file));
        Playlist ->setCurrentIndex(0);  // 重新设置 多媒体位置
        Player ->setPlaylist(Playlist);
        buildlrc(nowlist_lrc[0]); // 设置歌词
         boxitem(idx, name, lw);  // 加入队列 增加 item
        reinit(0); // 初始化
    } else return;
}

// fun of songqueue
void MainWindow::songqueue_fun(int row, int col) {
    g_file = nowplaylist[row];
    g_name = nowlist[row];
    g_image = nowlist_im[row];
    g_lrc = nowlist_lrc[row];

    QSqlQuery like_db;
    QString user_id_s = QString("%1").arg(user_id); // id 字符化
    bool net = is_net_music(nowplaylist[row]);

   // 添加到 我的喜欢
    if (col == 0) {
        QString song_like = is_like(nowlist[row]);
        if(song_like == "0") {  // 是否和本地音乐一起消失
            songqueue ->item(row, 0) ->setIcon(QIcon(":/coin/like.png"));
            if (!net)  {
                int idx = std::find(filemlist.begin(), filemlist.end(), nowlist[row]) - filemlist.begin();
                local_w ->item(idx, 0) ->setIcon(QIcon(":/coin/like.png"));
            }
            like_db.exec("insert into liketb values(" + user_id_s + ",\"" + nowplaylist[row] + "\",\"" + nowlist[row] + "\",\"" + nowlist_im[row] + "\", \"" + nowlist_lrc[row] + "\");");
            fun_like(nowlist[row], 0);
            //初始化
            init_local();
            init_like();
        } else if (song_like == "1") {
            songqueue ->item(row, 0) ->setIcon(QIcon(":/coin/like_c.png"));
            if (!net)  {
                int idx = std::find(filemlist.begin(), filemlist.end(), nowlist[row]) - filemlist.begin();
                local_w ->item(idx, 0) ->setIcon(QIcon(":/coin/like_c.png"));
            }
            like_db.exec("delete from liketb where id = " + user_id_s + " and music = \"" + nowplaylist[row] + "\";");
            fun_like(nowlist[row], 1);
            //初始化
            init_local();
            init_like();
        }
        return;
    }

    // 关于 播放列表的播放操作
    if (col == 1 || col == 2) {
        Player ->pause();
        Playlist ->setCurrentIndex(row);
        QTimer::singleShot(200, [=] () { // 缓冲一下 在播放
            Player ->play();
        });
        return;
    }


    //删除操作
    if (col == 3) {
        QSqlQuery sql;
        QString file = nowplaylist[row];
        QString user_id_s = QString("%1").arg(user_id);
        this ->songqueue -> removeRow(row);
        Playlist ->removeMedia(row); // 多媒体删除 音乐列表 歌曲
        nowplaylist.erase(nowplaylist.begin() + row, nowplaylist.begin() + row + 1);
        nowlist.erase(nowlist.begin() + row, nowlist.begin() + row + 1);
        nowlist_im.erase(nowlist_im.begin() + row, nowlist_im.begin() + row + 1);
        nowlist_lrc.erase(nowlist_lrc.begin() + row, nowlist_lrc.begin() + row + 1);
        sql.exec("delete from nowplay where music = '" + file + "' and id = " + user_id_s + ";");
        return;
    }


    // 收藏操作
    if (col == 4) {
        QSqlQuery sql(db);
        show_w = new show_list(user_id);
        show_w ->show();

        g_row = row;
        connect(show_w, &show_list::return_item, this, &MainWindow::this_songlist);
        return;
    }
    return;
}


//local的功能
void MainWindow::local_fun(int row, int col) {
    g_file = this ->filem + filemlist[row];
    g_name = filemlist[row];
    g_image = ":/coin/songer.png";
    g_lrc = "本地音乐暂无歌词";

    QSqlQuery like_db;
    QString user_id_s = QString("%1").arg(user_id); // id 字符化

    // 当col == 0 时 关于喜爱音乐
    if(col == 0) {
        QString local_like = is_like(filemlist[row]);
        bool isplay = false;  // 是否在播放队列
        if (nowplaylist.count(this ->filem + "/" + filemlist[row]) != 0) isplay = true;
         if(local_like == "0") {
            local_w ->item(row, 0) ->setIcon(QIcon(":/coin/like.png"));
            if (isplay) {
                int idx = std::find(nowplaylist.begin(), nowplaylist.end(), this ->filem + "/" + filemlist[row]) - nowplaylist.begin();
                songqueue ->item(idx, 0) ->setIcon(QIcon(":/coin/like.png"));
            }
            like_db.exec("insert into liketb values(" + user_id_s + ",\"" + this ->filem + "/" + filemlist[row] + "\",\"" + getname(filemlist[row]) + "\",\"" + ":/coin/songer.png" + "\", \"本地音乐暂无歌词\");");
            fun_like(filemlist[row], 0);
            //初始化
            init_local();
            init_like();
        } else{
            local_w ->item(row, 0) ->setIcon(QIcon(":/coin/like_c.png"));
            if (isplay) {
                int idx = std::find(nowplaylist.begin(), nowplaylist.end(), this ->filem + "/" + filemlist[row]) - nowplaylist.begin();
                songqueue ->item(idx, 0) ->setIcon(QIcon(":/coin/like_c.png"));
            }
            like_db.exec("delete from liketb where id = " + user_id_s + " and music = \"" + this ->filem + "/" + filemlist[row] + "\";");
            fun_like(filemlist[row], 1);
            //初始化
            init_local();
            init_like();
        }
         return;
    }

    // 当col == 1 时播放歌曲
    if(col == 1 || col == 2) {
        insert_nowplay(this ->filem + "/" + filemlist[row], filemlist[row], ":/coin/songer.png", "本地音乐暂无歌词");  // 插入到数据库 音乐队列中
        reinit(1);
        if(emit_i == 0) {
            emit beginplay();
            emit_i++;
        }  // 防止重复激发 timer
        initPlayer();   // 初始化播放按钮
    }

    //收藏 歌曲
    if(col == 3) {
        QSqlQuery sql(db);
        show_w = new show_list(user_id);
        show_w ->show();

        g_row = row;
        connect(show_w, &show_list::return_item, this, &MainWindow::this_songlist);
        return;
    }
}


//like_w 点击
void MainWindow::fun_like_w(int row, int col) {
    QString user_id_s = QString("%1").arg(user_id); // id 字符化
    QSqlQuery like_p;
    like_p.exec("select * from liketb where id = " + user_id_s + ";");
    while(like_p.next()) {
        if (like_w ->item(row, 1) ->text() == like_p.value(2).value<QString>()) {
            // 插入
            g_file = like_p.value(1).value<QString>();
            g_name = like_p.value(2).value<QString>();
            g_image = like_p.value(3).value<QString>();
            g_lrc = like_p.value(4).value<QString>();
            break;
        }
    }


    // 关于爱心歌曲 保存与删除
    if(col == 0) {
        QSqlQuery like_db;
        like_db.exec("delete from liketb where id = " + user_id_s + " and name = \"" + like_w ->item(row, 1) ->text() + "\";");
        int lc_idx = std::find(filemlist.begin(), filemlist.end(), like_w ->item(row, 1) ->text()) - filemlist.begin();
        int np_idx = std::find(nowlist.begin(), nowlist.end(), like_w ->item(row, 1) ->text()) - nowlist.begin();
         // 不再是 爱心了
        if(filemlist.count(like_w ->item(row, 1) ->text()) != 0) local_w ->item(lc_idx, 0) ->setIcon(QIcon(":/coin/like_c.png"));
        if(nowlist.count(like_w ->item(row, 1) ->text()) != 0) songqueue ->item(np_idx, 0) ->setIcon(QIcon(":/coin/like_c.png"));

        like_w ->removeRow(row);
        for(int j = 0; j < 5; ++j) like_w ->takeItem(row, j);


        //初始化
        init_like();
        init_local();
        return;
    }


    // 点击 播放音乐
    if (col == 1 || col == 2) {
        QSqlQuery like_p;
        like_p.exec("select * from liketb where id = " + user_id_s + ";");
        while(like_p.next()) {
            if (like_w ->item(row, 1) ->text() == like_p.value(2).value<QString>()) {
                // 插入
                insert_nowplay(like_p.value(1).value<QString>(), like_p.value(2).value<QString>(), like_p.value(3).value<QString>(), like_p.value(4).value<QString>());
                break;
            }
        }
    }


    // 加入歌单
    if (col == 3) {
        QSqlQuery sql(db);
        show_w = new show_list(user_id);
        show_w ->show();

        g_row = row;
        connect(show_w, &show_list::return_item, this, &MainWindow::this_songlist);
        return;
    }
}


// 播放歌曲发出信号
void MainWindow::play_net_Music(int row, int col) {
    // 爱心操作
    if(col == 0) {
        is_insert = 0; // 不插入
        //歌曲请求 row 是行号
    }

    else if(col == 1 || col == 4) {
        is_insert = 1; // 插入
    }

    // 收藏到 歌单
    else if (col == 5) {
        in_list = 1; // 收藏不插入播放队列
        is_insert = 2;
        g_row = row;
    } else return;

    //歌曲请求 row 是行号
    QString KGAPISTR1 =QString("http://www.kugou.com/yy/index.php?r=play/getdata"
    "&hash=%1&album_id=%2&_=1497972864535").arg(v_hash.at(row)) .arg(v_id.at(row));
    network_request2->setUrl(QUrl(KGAPISTR1));
    //这句话很重要，我们手动复制url放到浏览器可以获取json，但是通过代码不行，必须加上下面这句才可以
    network_request2->setRawHeader("Cookie","kg_mid=2333");
    network_request2->setHeader(QNetworkRequest::CookieHeader, 2333);
    network_manager2->get(*network_request2);
}


// 每个 歌单的 功能
void MainWindow::fun_list_w(int row, int col) {
    QSqlQuery list_db;
    QString user_id_s = QString("%1").arg(user_id); // id 字符化
    QString list_id_s = QString("%0").arg(g_list_id);

    list_db.exec("select * from songlist where id = " + user_id_s + " and listid = " + list_id_s + " and name = \"" + list_w ->item(row, 1) ->text() + "\";");
    while (list_db.next()) {
        g_file = list_db.value(2).value<QString>();
        g_name = list_db.value(3).value<QString>();
        g_image = list_db.value(4).value<QString>();
        g_lrc = list_db.value(5).value<QString>();
        break;
    }

    // 爱心
    if (col == 0) {
        QString list_like = is_like(list_w ->item(row, 1) ->text());
        bool isplay = false;  // 是否在播放队列
        bool islocal = false; // 是否在local中
        if (nowlist.count(list_w ->item(row, 1) ->text()) != 0) isplay = true;
        if (filemlist.count(list_w ->item(row, 1) ->text()) != 0) islocal = true;
        if(list_like == "0") {
            list_w ->item(row, 0) ->setIcon(QIcon(":/coin/like.png"));
            if (isplay) {
                int idx = std::find(nowlist.begin(), nowlist.end(), list_w ->item(row, 1) ->text()) - nowlist.begin();
                songqueue ->item(idx, 0) ->setIcon(QIcon(":/coin/like.png"));
            }
            if (islocal) {
                int idx = std::find(filemlist.begin(), filemlist.end(), list_w ->item(row, 1) ->text()) - filemlist.begin();
                local_w ->item(idx, 0) ->setIcon(QIcon(":/coin/like.png"));
            }

            QSqlQuery list_q;
            list_q.exec("select music from songlist where id = " + user_id_s + " and listid = " + list_id_s + " and name = \"" + list_w ->item(row, 1) ->text() + "\";");
            list_q.next();
            list_db.exec("insert into liketb values(" + user_id_s + ",\"" + list_q.value(0).value<QString>() + "\",\"" + list_w ->item(row, 1) ->text() + "\",\"" + ":/coin/songer.png" + "\", \"本地音乐暂无歌词\");");
            fun_like(list_w ->item(row, 1) ->text(), 0);

            //初始化
            init_local();
            init_like();
        } else {
            list_w ->item(row, 0) ->setIcon(QIcon(":/coin/like_c.png"));
            if (isplay) {
                int idx = std::find(nowlist.begin(), nowlist.end(), list_w ->item(row, 1) ->text()) - nowlist.begin();
                songqueue ->item(idx, 0) ->setIcon(QIcon(":/coin/like_c.png"));
            }
            if (islocal) {
                int idx = std::find(filemlist.begin(), filemlist.end(), list_w ->item(row, 1) ->text()) - filemlist.begin();
                local_w ->item(idx, 0) ->setIcon(QIcon(":/coin/like_c.png"));
            }
            list_db.exec("delete from liketb where id = " + user_id_s + " and name = \"" + list_w ->item(row, 1) ->text() + "\";");
            fun_like(list_w ->item(row, 1) ->text(), 1);

            //初始化
            init_local();
            init_like();
        }
         return;
    }



    //播放
    if(col == 1 || col == 2) {
        QSqlQuery list_q;
        list_q.exec("select * from songlist where id = " + user_id_s + " and listid = " + list_id_s + " and name = \"" + list_w ->item(row, 1) ->text() + "\";");
        while(list_q.next()) {
            QString f = list_q.value(2).value<QString>();
            QString n = list_q.value(3).value<QString>();
            QString im = list_q.value(4).value<QString>();
            QString l = list_q.value(5).value<QString>();
            insert_nowplay(f, n, im, l);
        }
    }


    // 删除
    if (col == 3) {
        QSqlQuery list_q;
        QString ex = QString("delete from songlist where id = " + user_id_s + " and listid = " + list_id_s + " and name = \"" + list_w ->item(row, 1) ->text() + "\";");
        list_q.exec(ex);
        list_w ->removeRow(row);
    }


    // 收藏
    if(col == 4) {
        QSqlQuery sql(db);
        show_w = new show_list(user_id);
        show_w ->show();

        g_row = row;
        connect(show_w, &show_list::return_item, this, &MainWindow::this_songlist);
        return;
    }

}

void MainWindow::setSequentialMode()
{
    changePlayMode(QMediaPlaylist::Sequential);
}

void MainWindow::setLoopMode()
{
    changePlayMode(QMediaPlaylist::Loop);
}

void MainWindow::setRandomMode()
{
    changePlayMode(QMediaPlaylist::Random);
}


// 判断是否是网络音乐
bool MainWindow::is_net_music(QString file) {
    QString buf;
    for(int i = 0; i < file.size(); ++i) {
         buf += file[i];
        if(buf == "http") return true;
    }
    return false;
}


// like_w的显示
void MainWindow::fun_like(QString name, int type) {
    int row = like_w ->rowCount();

    // 添加
    if(type == 0) {
        like_w ->setRowCount(row + 1);

        for(int i = 0; i <= 4; ++i) like_w ->setItem(row, i, new QTableWidgetItem());
        like_w ->item(row, 0) ->setIcon(QIcon(":/coin/like.png"));
        like_w ->item(row, 1) ->setText(name);
        like_w ->item(row, 2) ->setIcon(QIcon(":/coin/begin.png"));
        like_w ->item(row, 3) ->setIcon(QIcon(":/coin/add.png"));
    } else {  // 删除
        for(int i = 0; i < row; ++i) {
            if (name == like_w ->item(i, 1) ->text()) {
                for(int j = 0; j < 5; ++j) like_w ->takeItem(i, j);
            }
        }
    }
}

// add to songlist
void MainWindow::this_songlist(QListWidgetItem *item) {
    QString user_id_s = QString("%1").arg(user_id); // id 字符化
    QString name = item ->text(); // 获取这个 歌单的名字
    QString list_id_lim;
    QSqlQuery sql(db);

    sql.exec("select * from listname where name = \"" + name + "\"and id = " + user_id_s + ";");
    while(sql.next()) {
        list_id_lim = QString("%0").arg(sql.value(2).value<int>());
        qDebug() << "list_id is: " << list_id_lim << endl;
    }


    sql.exec("select name from songlist where id = " + user_id_s + " and listid = " + list_id_lim + ";");
    while(sql.next()) {
        if (sql.value(0).value<QString>() == g_name) {
            QMessageBox::information(this, "提示", "该首歌已经录入了~", QMessageBox::Ok);
            return;
        }
    }


    if (g_row != -1) {
        QString ex = QString("insert into songlist values(" + user_id_s + ", " + list_id_lim + ", \"" + g_file + "\", \"" + g_name + "\", \"" + g_image + "\", \"" + g_lrc + "\");");
        g_row = -1;
        qDebug() << ex << endl;
        bool ok = sql.exec(ex);
        if (ok) qDebug() << "insert list ok" << endl;
        else qDebug() << "no insert list" << endl;
    }
    return;
}


//将图片变成圆角
QPixmap MainWindow::PixmapToRound(QPixmap &pix, int radius) {
    QSize size(2 * radius, 2 * radius);
    QBitmap bit(size);
    QPainter painter(&bit);

    //经典抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(Qt::black);
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 10, 10);
    QPixmap ans = pix.scaled(size);
    ans.setMask(bit);
    return ans;
}


// 搜索 text 通过 http网络, 给予finish 相应
void MainWindow::search(QString text, int idx) {
    QString net_search = QString("http://mobilecdn.kugou.com/api/v3/search/song?format=json&keyword=%1&page=%2&pagesize=18a").arg(text).arg(idx);
    net_request ->setUrl(QUrl(net_search));
    net_manager ->get(*net_request);
}


// 判断是不是喜欢音乐
QString MainWindow::is_like(QString file) {

    QSqlQuery sql;
    sql.exec("select * from liketb;");
    while (sql.next()) {
       if (sql.value(0).value<int>() == user_id && sql.value(2).value<QString>() == file) return "1";
    }
    return "0";
}

// net_manager finish 响应reply 开始在tableWidget 上显示
void MainWindow::reply(QNetworkReply* re) {
    QVariant status_code = re ->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (re ->error() == QNetworkReply::NoError) {
        QByteArray bytes = re ->readAll();
        QString res(bytes);
        parseJson(res, index);
        index += 20;
    } else {
        qDebug() << "error!" << endl;
    }
}

// 解析 json 显示到 tablewidget上
void MainWindow::parseJson(QString json, int idx) {
        QString songname_original;   //歌曲名
            QString singername;        //歌手
            QString hashStr;           //hash
            QString album_name;        //专辑
            int duration;          	   //时间
            QByteArray byte_array;
            QJsonParseError json_error;
            QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
            if (json_error.error == QJsonParseError::NoError)
            {
                if (parse_doucment.isObject())
                {
                    QJsonObject rootObj = parse_doucment.object();
                    if (rootObj.contains("data"))
                    {
                        QJsonValue valuedata = rootObj.value("data");
                        if (valuedata.isObject())
                        {
                            QJsonObject valuedataObject = valuedata.toObject();
                            if (valuedataObject.contains("info"))
                            {
                                QJsonValue valueArray = valuedataObject.value("info");
                                if (valueArray.isArray())
                                {
                                    QJsonArray array = valueArray.toArray();
                                    int size = array.size();
                                    for (int i = idx, j = 0; i < size + idx && j < size; i++, ++j)
                                    {
                                        QJsonValue value = array.at(j);
                                       if (value.isObject())
                                       {
                                            QJsonObject object = value.toObject();
                                            if (object.contains("songname_original"))//歌曲名
                                            {
                                                QJsonValue AlbumID_value = object.take("songname_original");
                                                if (AlbumID_value.isString())
                                                {
                                                    songname_original = AlbumID_value.toString();
                                                }
                                            }
                                            if (object.contains("singername"))//歌手
                                            {
                                                QJsonValue AlbumID_value = object.take("singername");
                                                if (AlbumID_value.isString())
                                                {
                                                    singername = AlbumID_value.toString();
                                                }
                                            }
                                            if (object.contains("album_name"))//专辑
                                            {
                                                QJsonValue AlbumID_value = object.take("album_name");
                                                if (AlbumID_value.isString())
                                                {
                                                    album_name = AlbumID_value.toString();
                                                }
                                            }
                                            if (object.contains("hash")) //hash
                                            {
                                                QJsonValue FileHash_value = object.take("hash");
                                                if (FileHash_value.isString())
                                                {
                                                    hashStr = FileHash_value.toString();
                                                    //用Vector保存每首歌曲的hash
                                                    v_hash.push_back(FileHash_value.toString());
                                                }
                                            }
                                            if (object.contains("album_id"))
                                            {
                                                QJsonValue FileHash_value = object.take("album_id");
                                                if (FileHash_value.isString())
                                                {
                                                    //用Vector保存每首歌曲的album_id
                                                    v_id.push_back(FileHash_value.toString());
                                                }
                                            }
                                            if (object.contains("duration"))//时长
                                            {
                                               QJsonValue AlbumID_value = object.take("duration").toInt();
                                               duration = AlbumID_value.toInt();
                                            }
                                            //将解析出的内容放到列表中
                                            tab_search ->setRowCount(i + 1);

                                            // 爱心
                                            QString is_like_ = is_like(songname_original);
                                            tab_search ->setItem(i, 0, new QTableWidgetItem());
                                            if(is_like_ == "0") tab_search ->item(i, 0) ->setIcon(QIcon(":/coin/like_c.png"));
                                            else tab_search ->item(i, 0) ->setIcon(QIcon(":/coin/like_c.png"));

                                            tab_search ->setItem(i,1,new QTableWidgetItem(songname_original));
                                            //文字居中
                                            tab_search->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


                                            tab_search ->setItem(i,2,new QTableWidgetItem(singername));
                                            tab_search ->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                                            tab_search ->setItem(i,3,new QTableWidgetItem(album_name));
                                            tab_search ->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                                            tab_search ->setItem(i,4,new QTableWidgetItem());
                                            tab_search ->item(i,4) ->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                                            tab_search ->item(i, 4) ->setIcon(QIcon(":/coin/begin.png"));

                                            tab_search ->setItem(i,5,new QTableWidgetItem());
                                            tab_search ->item(i,5) ->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                                            tab_search ->item(i, 5) ->setIcon(QIcon(":/coin/add.png"));

                                            QString time = QString("%1:%2").arg(duration/60).arg(duration%60);
                                            tab_search ->setItem(i,6,new QTableWidgetItem(time));
                                            tab_search ->item(i,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                                      }
                                 }
                            }
                        }
                    }
                }
            }
        }
}


void MainWindow::reply2(QNetworkReply *re)  // 如果net_messageer finish  判断是否载入 然后进行js
{
    //获取响应的信息，状态码为200表示正常
         QVariant status_code = re->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        //无错误返回
        if(re->error() == QNetworkReply::NoError)
        {
            QByteArray bytes = re->readAll();  //获取字节
            QString result(bytes);  //转化为字符串
            parseJson2(result);//解析json
        }
        else
        {
            //处理错误
            qDebug()<<"歌曲播放失败";
        }
}


//解析 json
void MainWindow::parseJson2(QString json) {
    QString audio_name;//歌手-歌名
    QString play_url;//播放地址
    QString img;
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error == QJsonParseError::NoError) {
       if(parse_doucment.isObject())
       {
           QJsonObject rootObj = parse_doucment.object();
           if(rootObj.contains("data"))
           {
               QJsonValue valuedata = rootObj.value("data");
               if(valuedata.isObject())
               {
                   QJsonObject valuedataObject = valuedata.toObject();
                   QString play_urlStr("");
                   if(valuedataObject.contains("play_url"))
                   {
                       QJsonValue play_url_value = valuedataObject.take("play_url");
                       if(play_url_value.isString())
                       {
                           play_urlStr = play_url_value.toString();      //歌曲的url
                           if(play_urlStr!="")
                           {
                               net_file = play_urlStr;
//                               Player ->setMedia(QUrl(play_urlStr));
//                               Player->play();
                           }
                       }
                   }
                   if(valuedataObject.contains("audio_name"))
                   {
                       QJsonValue play_name_value = valuedataObject.take("audio_name");
                       if(play_name_value.isString())
                       {
                           QString audio_name = play_name_value.toString();    //歌曲名字
                           if(audio_name!="")
                           {
                               //显示
                               qDebug()<<audio_name;
                               net_name = audio_name;
                           }
                       }
                   }

                   // 获得该首歌的图片
                   if (valuedataObject.contains("img"))
                   {
                       QJsonValue play_name_value = valuedataObject.take("img");
                       if (play_name_value.isString())
                       {
                           QString audio_name = play_name_value.toString();
                           if (audio_name != "")
                           {
                               QByteArray m_Jpg;
                               m_Jpg.append(audio_name);
                               this ->net_image = audio_name; // 保存图片路径
                               network_request3->setUrl(QUrl(audio_name));
                               network_manager3->get(*network_request3);  // 反馈信号 准备解析json
                           }
                       }
                   }
                   // 歌词显示
                   idd = 0; // 初始化 lrc_itm 的索引
                   if (valuedataObject.contains("lyrics")) //lrc
                   {
                           QJsonValue play_url_value = valuedataObject.take("lyrics");
                           if (play_url_value.isString())
                           {
                               QString play_lrcStr = play_url_value.toString();
                               this ->net_lrc = play_lrcStr; // 保存http 歌词
                               if (play_urlStr != "")
                               {
                                   if (play_lrcStr != "")
                                   {	//将整个歌词给s
                                       QString s = play_lrcStr;
                                       buildlrc(s);
                                       p_lrcit = lrcMap.begin() - 1;
                                   }
                                   else
                                   {
                                       //没有歌词;
                                   }
                               }
                           }
                      }
                   g_file = net_file;
                   g_name = net_name;
                   g_image = net_image;
                   g_lrc = net_lrc;

                   if(is_insert == 1) insert_nowplay(net_file, net_name, net_image, net_lrc);  //将网络歌曲插入到队列
                   else if (is_insert == 0) {  // 插入到喜爱音乐里面
                       QSqlQuery like_db;
                       int row = -1;
                       for(int i = 0; i < tab_search->rowCount(); ++i) {
                           if (getPName(net_name) == tab_search ->item(i, 1) ->text()) {
                               row = i;
                               break;
                           }
                       }

                       if (row == -1) return;
                       QString user_id_s = QString("%1").arg(user_id); // id 字符化
                       QString is_like_net = is_like(net_name);
                       int np_idx = std::find(nowlist.begin(), nowlist.end(), net_name) - nowlist.begin();
                       if (is_like_net == "0") {
                           tab_search ->item(row, 0) ->setIcon(QIcon(":/coin/like.png"));
                           if(nowlist.count(tab_search->item(row, 1) ->text()) != 0) songqueue ->item(np_idx, 0) ->setIcon(QIcon(":/coin/like.png"));
                           like_db.exec("insert into liketb values(" + user_id_s + ",\"" + net_file + "\",\"" + net_name + "\",\"" + net_image + "\", \"" + net_lrc + "\");");
                           fun_like(net_name, 0);
                       } else {
                           tab_search ->item(row, 0) ->setIcon(QIcon(":/coin/like_c.png"));
                           if(nowlist.count(tab_search->item(row, 1) ->text()) != 0) songqueue ->item(np_idx, 0) ->setIcon(QIcon(":/coin/like_c.png"));
                           like_db.exec("delete from liketb where id = " + user_id_s + " and music = \"" + net_file + "\";");
                           fun_like(net_name, 1);
                       }
                   }

                   if (in_list == 1) {
                       in_list = 0; // 变回0 避免不必要的错误

                       // 执行 收藏歌曲操作
                       QSqlQuery sql(db);
                       show_w = new show_list(user_id);
                       show_w ->show();

                       connect(show_w, &show_list::return_item, this, &MainWindow::this_songlist);
                   }
               }
                    //下一篇的歌词获取也是在这里添加代码
               else {
                    qDebug()<<"出错";
                }
           }
       }
    }
}

// 图片获取 触发信号开始解析
void MainWindow::reply3(QNetworkReply *reply)
{
    //获取响应的信息，状态码为200表示正常
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        //无错误返回
        if(reply->error() == QNetworkReply::NoError && is_insert == 1)
        {

            QByteArray bytes = reply->readAll();  //获取字节
            //由于获取的图片像素过大，而我们显示的图片很小，所以我们需要压缩图片的像素，我们label的大小为45*45，所以我们把图片压缩为45*45
            QSize size(50, 50);
            QBitmap bit(size);
            QPixmap pixmap, s_pix, w_l_pixmap;
            pixmap.loadFromData(bytes);
            w_l_pixmap.loadFromData(bytes);
            w_l_pixmap = PixmapToRound(w_l_pixmap, 180);

            // 歌曲图标
            s_pix = PixmapToRound(pixmap, 30);
            music_map ->setFixedSize(s_pix.width(), s_pix.height());
            music_map ->setIconSize(QSize(s_pix.width(), s_pix.height()));
            music_map ->setIcon(QIcon(s_pix));
            music_map ->setStyleSheet("QPushButton{border: 0px;}");
            music_map ->show();

            // 歌词窗口 绘制图片
            lrc_w->l->setPixmap(w_l_pixmap);
        }
        else
        {
            //处理错误
            qDebug()<<"NET_图片显示错误";
        }
}

// 同上 只不过是本地的音乐图片 处理
void MainWindow::local_img(QString im) {
    QPixmap pix, w_p;
    pix.load(im);
    pix = PixmapToRound(pix, 30);

    w_p.load(im);
    w_p = PixmapToRound(w_p, 180);

    music_map ->setFixedSize(pix.width(), pix.height());
    music_map ->setIconSize(QSize(pix.width(), pix.height()));
    music_map ->setIcon(QIcon(pix));
    music_map ->setStyleSheet("QPushButton{border: 0px;}");
    music_map ->show();

    lrc_w ->l ->setPixmap(w_p);
}

// 初始化歌词播放
void MainWindow::initlrc_win() {
    // 初始化roll_cnt roll lrc_itm
    lrc_itm.clear();
    roll_cnt = 0;
    roll = 0;

    lrc_l = new QListWidget(lrc_w);
    lrc_l ->setStyleSheet("QListWidget::item{height: 45px;}"
                          "QListWidget::item::hover{background: rgb(228, 228, 228);}");
    lrc_l ->setGeometry(650, 80, 340, 360);
    lrc_l ->setFrameShape(QListWidget::NoFrame);   // 无边框
    lrc_l ->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //无进度条
    lrc_l ->setFocusPolicy(Qt::NoFocus);  // 点击无 solid

    //准备载入歌词 到窗口
    QMap<int, QString>::iterator it = lrcMap.begin();

    while(it != lrcMap.end()) {
        QFont font;
        font.setFamily("幼圆");
        font.setPointSize(13);
        font.setWeight(60);

        QString lrc = it.value();
        QListWidgetItem *lrc_it = new QListWidgetItem(it.value(), lrc_l);
        lrc_it ->setFont(font);
        lrc_itm.push_back(lrc_it);
        lrc_l ->addItem(lrc_it);
        it++;
    }
    lrc_l ->show();
}

// 解析构建歌词
void MainWindow::buildlrc(QString s) {
    // 初始化
    lrcMap.clear();
    lrc_idx.clear();
    idd = 0;

    // 本地音乐处理
    if(s == "本地音乐暂无歌词") {
//        qDebug() << "local lrc!" << endl;
        lrcMap[0] = s;
        lrc_idx[0] = 0;
        initlrc_win();
        return;
    }

    QStringList s1 = s.split("\n");
    for (int i = 3; i < s1.size() - 1; i++)
    {
        QString ss1 = s1[i];
        //歌词中开头有一些是无意义的字符，用正则表达式判断，只保存包含有时间戳的字符串。
        QRegExp ipRegExp = QRegExp("\\[\\d\\d\\S\\d\\d\\S\\d\\d\\]");
        //若包含则返回flase
        bool match = ipRegExp.indexIn(ss1);
        if (match == false)
        {
            //时间解析格式(分*60+秒)*100+厘秒
            int s_1 = ss1.mid(1, 2).toInt();      //分
            int s_2 = ss1.mid(4, 2).toInt();      //秒
            int s_3 = ss1.mid(7, 2).toInt();      //厘秒
            int s_count = ((s_1 * 60 + s_2) * 100 + s_3) * 10;   //毫秒换算

            int lrctime = s_count;
            QString lrcstr = ss1.mid(10);  // 歌词载入
//                                               qDebug() << "durtion: " << s_count << "  " << lrcstr << endl;
            //用Qmap来保存

            lrcMap[lrctime] = lrcstr;
            lrc_idx[lrctime] = idd++;
        }
    }
    initlrc_win();
}


void MainWindow::songlist_add(QString name) {
    QFont font;
    font.setFamily("幼圆");
    font.setPointSize(11);

    QString user_id_s = QString("%0").arg(user_id);
    QString list_id_s = QString("%0").arg(song_list_id);

    QSqlQuery l_sql(db);
    l_sql.exec("select name from listname where id = " + user_id_s + ";");
    while(l_sql.next()) {
        if (l_sql.value(0).value<QString>() == name) {
            QMessageBox::information(this, "提示", "失败(可能已经有该名字的歌单了)!", QMessageBox::Ok);
            return;
        }
    }

    song_list_id += 1;
    QString n_list_id = QString("%0").arg(song_list_id);
    l_sql.exec("insert into listname values(" + user_id_s + ", \"" + name + "\", " + list_id_s + ");");
    l_sql.exec("update user set cnt = " + n_list_id + " where id = " + user_id_s + ";");


    songlist ->addItem(new QListWidgetItem(name));
}


void MainWindow::show_List_music(QListWidgetItem *c_item) {
    // 初始化
    list_w ->clear();
    list_w ->setRowCount(0);

    list_w ->setGeometry(musiclist->width(), btnL->height(),this ->width() - musiclist->width(), this ->height() - btnL->height() - 100);
    list_w ->setHorizontalHeaderLabels(QStringList() << " " << "歌曲" << " " << " " << " " << " ");
    list_w ->setStyleSheet("QTableWidget::Item::selected{background: white;}"
                            "QHeaderView::section{border: 0px solid white};");


    //delete init
    if (delete_btn) delete_btn ->hide(); // 上一个 消失

    QWidget *w = new QWidget();
    delete_btn = new mybtn(":/coin/cls.png", ":/coin/cls_1.png");
    delete_btn ->setParent(w);
    delete_btn ->move(90, 10);
    songlist ->setItemWidget(c_item, w);
    /////////////////////////////////////////////////////////////////////////

    QString user_id_s = QString("%0").arg(user_id);
    QString name = c_item ->text();

    QSqlQuery sql;
    sql.exec("select listid from listname where id = " + user_id_s + " and name = \"" + name + "\";");
    sql.next();
    QString list_id = QString("%0").arg(sql.value(0).value<int>());

    QSqlQuery i_sql;
    i_sql.exec("select * from songlist where id = " + user_id_s + ", and listid = " + list_id + ";");

    while(i_sql.next()) {
        int row = list_w ->rowCount();
        list_w ->setRowCount(row + 1);
        for (int i = 0; i < 6; ++i) list_w ->setItem(row, i, new QTableWidgetItem());

        list_w ->item(row, 0) ->setIcon(QIcon(":/coin/like.png"));  // 爱心
        list_w ->item(row, 1) ->setText(i_sql.value(3).value<QString>()); // 歌名
        list_w ->item(row, 2) ->setIcon(QIcon(":/coin/begin.png")); // 播放
        list_w ->item(row, 3) ->setIcon(QIcon(":/coin/delete.png")); // 删除
        list_w ->item(row, 4) ->setIcon(QIcon(":/coin/add.png")); // 增添
    }

    local_w ->hide();
    tab_search ->hide();
    like_w ->hide();
    list_w ->show();
    g_list_id = list_id;  // 全局变量

    connect(delete_btn, &mybtn::clicked, [=] () { // 当点击delete_btn 删除这个歌单
        QMessageBox::StandardButton b = QMessageBox::question(this, "询问", "是否删除这个歌单?", QMessageBox::Yes | QMessageBox::No);
        if (b == QMessageBox::Yes) {
            int row = songlist ->row(c_item);
            songlist ->takeItem(row);
            delete_btn = nullptr;


            QSqlQuery sql_l;
            sql_l.exec("delete from songlist where id = " + user_id_s + " and listid = " + list_id + ";");
            sql_l.exec("delete from listname where id = " + user_id_s + " and listid = " + list_id + ";");
        }
    });
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::mousePressEvent(QMouseEvent * e) {
    // 音量进度条
    if (e ->pos().x() >= 670 && e ->pos().x() < 720 && e ->pos().y() >= 635 && e ->pos().y() <= 640)  {
        V = e ->pos().x() - 670;
        Player ->setVolume(V * 2);   // 调整音量
        if (this ->volF) {
           volF = false;
           volbt->is_play = false;
           volbt ->setIcon(QIcon(":/coin/vol.png"));
        }
        update();
    }

    //点击进度条
    if( e->pos().y() > 640 && e->pos().y() < 675 && e ->pos().x() >= 220 && e ->pos().x() < 785)
    {
            int value;
            value = e->pos().x();
            time ->stop();
            if(value < 220)
            {
                X = 56;
            }
            else if (value > 780)
            {
                X = 635;
            }
            else
            {
                X = value - 150;
            }
            if(X <= 620)
            {
                changePro();
            }
           update(); // 重绘
           setCursor(Qt::PointingHandCursor);
           if(playf) time ->start();
    }

    // 点击窗口
    if(e ->button() == Qt::LeftButton && e ->pos().y() >= 0 && e ->pos().y() < 50) {
        leftflag = true;
    }
    glbal_p = e ->globalPos() - this ->pos();
} // init mousePressEvent

void MainWindow::mouseMoveEvent(QMouseEvent *e) {
    // 音量进度条
    if (e ->pos().x() >= 670 && e ->pos().x() < 720 && e ->pos().y() >= 635 && e ->pos().y() <= 640)  {
        V = e ->pos().x() - 670;
        Player ->setVolume(V * 2);   // 调整音量
        if (this ->volF) {
           volF = false;
           volbt->is_play = false;
           volbt ->setIcon(QIcon(":/coin/vol.png"));
        }
        update();
    }

    // 移动进度条
    if (e->pos().y() > 640 && e->pos().y() < 675 && e ->pos().x() >= 220 && e ->pos().x() < 785) {
        time ->stop();
        int value = e->pos().x();
        if (value < 220)
        {
            X = 56;
        }
        else if (value > 780)
        {
            X = 635;
        }
        else
        {
            X =  e->pos().x() - 150;
        }

        changePro();
        update();
        setCursor(Qt::PointingHandCursor);
        if(playf) time ->start();
    }

    // 移动窗口
    if(leftflag && e ->pos().y() >= 0 && e ->pos().y() < 50) {
        win_p = e ->globalPos();
        this ->move(win_p - glbal_p);
    }
} // init mouseMoveEvent

// 通过拖动进度条  改变歌曲进度
void MainWindow::changePro() {
    this ->positontime = (float)((X - 56) / (float)580.0) * Player ->duration();  //根据鼠标的坐标 反推出此时 音乐的进度
    Player ->setPosition(this ->positontime);       // 改变音乐的进度
    double secondtime1 = positontime / 1000;    // 当前播放的总秒数
    int minint1 = secondtime1 / 60;                // 当前播放的分钟
    int secondint1 = secondtime1 - minint1 * 60; // 当前播放的秒

    int curra = minint1 / 10;
    int currb = minint1 % 10;
    int currc = secondint1 / 10;
    int currd = secondint1 % 10;
    current = QString("%0%1:%2%3").arg(curra) .arg(currb).arg(currc). arg(currd);
}


MainWindow::~MainWindow()
{
    delete ui;
}

