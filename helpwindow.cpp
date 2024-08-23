#include "helpwindow.h"
#include "ui_helpwindow.h"

helpWindow::helpWindow(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::helpWindow)
{
    ui->setupUi(this);

    QPushButton *backButton = ui->backButton;
    connect(backButton, &QPushButton::clicked,this, &helpWindow::backMain);
}

helpWindow::~helpWindow()
{
    delete ui;
}

void helpWindow::backMain(){
    this->accept();
}
