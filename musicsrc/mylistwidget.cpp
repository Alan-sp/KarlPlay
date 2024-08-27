#include "mylistwidget.h"

mylistwidget::mylistwidget(QWidget *parent) : QListWidget(parent)
{
     // 关于 mylistwidget的 风格 (qss)
        this -> setStyleSheet("QListWidget{color:rgb(0, 0, 0); background:rgb(0, 0, 0, 0);border:none;}"
                                               "QListWidget::Item{height:40px; border:0px solid white;padding-left:27px; margin-left: 0px}"
                                               "QListWidget::Item:hover{color:rgb(0, 0, 0);background: rgb(228, 228, 228); border: 0px solid gray; border: 3px solid white;}"
                                               "QListWidget::Item:selected{color:rgb(0, 0, 0); background: rgb(228, 228, 228); border:1px solid gray; border: 3px solid white;}");
        this ->setFocusPolicy(Qt::NoFocus);
}
