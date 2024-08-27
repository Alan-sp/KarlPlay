#include "signin.h"
#include "ui_signin.h"

SignIn::SignIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint & ~Qt::WindowCloseButtonHint);
    QPushButton *signInButton = ui->signInButton;
    connect(signInButton, &QPushButton::clicked, this, &SignIn::signIn);
    QPushButton *backButton = ui->backButton;
    connect(backButton, &QPushButton::clicked, this, &SignIn::back);
    signInButton->setDefault(true);

    //调试代码
    qDebug() << this->parent()->parent()->parent()->metaObject()->className();

}

SignIn::~SignIn()
{
    delete ui;
}
void SignIn::back(){
    this->hide();
    this->clean();
}
void SignIn::signIn(){
    username = ui->usernameBox->text();
    password = ui->passwordBox->text();
    repassword = ui->rePasswordBox->text();
    phone = ui->phoneBox->text();
    favoriteTemp = ui->favoriteTempSlider->value();
    favoriteStyle = ui->favoriteStyleBox->currentIndex();


    // 检查两次密码是否一致
    if (password != repassword) {
        // 弹出错误提示
        QMessageBox::warning(this, "错误", "两次密码输入不一致！");
        ui->passwordBox->clear();
        ui->rePasswordBox->clear();
    } else if(password == ""||username == ""){
        QMessageBox::warning(this, "错误", "用户名密码不能为空！");
        ui->passwordBox->clear();
        ui->rePasswordBox->clear();
    }else {
        // 弹出成功提示
        QMessageBox::information(this, "注册成功", "注册成功！");
        insertUserIntoDatabase();
        this->clean();
        this->close();
    }
}
void SignIn::clean(){
    ui->usernameBox->clear();            // 清空用户名输入框
    ui->passwordBox->clear();            // 清空密码输入框
    ui->rePasswordBox->clear();          // 清空确认密码输入框
    ui->phoneBox->clear();               // 清空电话号码输入框
    ui->favoriteTempSlider->setValue(0); // 重置滑动条到默认值
    ui->favoriteStyleBox->setCurrentIndex(0); // 重置下拉框到第一个选项
}

void SignIn::insertUserIntoDatabase() {
    // 打开数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./KarlPlay.db");

    if (!db.open()) {
        qDebug() << "Error: Could not open database." << db.lastError().text();
        return;
    }

    // 创建一个查询对象
    QSqlQuery query;

    // 创建用户表（如果尚不存在）
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS user ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "username TEXT NOT NULL, "
                               "password TEXT NOT NULL, "
                               "phone TEXT NOT NULL, "
                               "favoriteTemp REAL, "
                               "favoriteStyle INTEGER)";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error: Could not create table." << query.lastError().text();
        return;
    }

    // 插入数据的查询语句
    QString insertQuery = "INSERT INTO user (username, password, phone, favoriteTemp, favoriteStyle) "
                          "VALUES (:username, :password, :phone, :favoriteTemp, :favoriteStyle)";

    query.prepare(insertQuery);
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":phone", phone);
    query.bindValue(":favoriteTemp", favoriteTemp);
    query.bindValue(":favoriteStyle", favoriteStyle);

    // 执行插入操作
    if (!query.exec()) {
        qDebug() << "Error: Could not insert data." << query.lastError().text();
    } else {
        qDebug() << "User inserted successfully!";
    }

    db.close(); // 关闭数据库连接
}
