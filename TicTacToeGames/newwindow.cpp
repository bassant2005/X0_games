#include "newwindow.h"
#include "ui_newwindow.h"

newwindow::newwindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::newwindow)
{
    ui->setupUi(this);
}

newwindow::~newwindow()
{
    delete ui;
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

