

KarlPlay


----------------------------------------------------------------

开发进度：
0.0.1 202408232122 右上角的个人中心能点 所有框分别跟什么模块相关在mainWwidget.ui的命名中查看
0.0.2 202408251428 引入本地数据库


----------------------------------------------------------------


本地数据库说明：
开始运行程序，自动在build路径下生成一个KarlPlay.db数据库，在其中自动生成一个user表，并默认0号用户是未登录状态（id:0, username:"pleasae log in", passeord:"NULL", phone:"NULL", favoriteStyle:NULL, favoriteTemp:NULL）
数据库信息：
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL,
    password TEXT NOT NULL,
    phone TEXT NOT NULL,
    favoriteTemp FLOAT,
    favoriteStyle INTEGER
数据库创建函数void createDatabaseAndUserTable()在user.cpp中，非成员函数
数据库用户id查询用户信息函数bool MainWindow::fetchUserInfo(int ID, struct UserInfo &userInfo)在MainWindow类中
MainWindow类中有struct UserInfo用于寄存当前用户信息 
    struct UserInfo{
        int userID;
        QString username;
        QString password;
        QString phone;
        float favoriteTemp;
        int favoriteStyle;
    }UserInfo;