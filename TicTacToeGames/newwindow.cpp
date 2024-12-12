#include "newwindow.h"
#include "ui_newwindow.h"
#include "nplayer.h"

newwindow::newwindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::newwindow)
    , susgame(nullptr)
    , nplayer(nullptr)
{
    ui->setupUi(this);
}

newwindow::~newwindow()
{
    delete ui;
    if (susgame) delete susgame;
    if (nplayer) delete nplayer;
}

void newwindow::on_pushButton_clicked()
{
    // Hide the current (NewWindow) window
    this->hide();

    // Show the parent (MainWindow) again
    QWidget *parentWindow = qobject_cast<QWidget*>(this->parent());
    if (parentWindow) {
        parentWindow->show();
    }
}



void newwindow::on_pushButton_2_clicked()
{

    if(!susgame){
        susgame = new SUSGame(this);
    }
    this->hide();
    susgame->show();

}

