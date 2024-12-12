#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newwindow.h"
#include "nplayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , newWindow(nullptr)
    , nplayer(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
    if (newWindow) delete newWindow;
    if (nplayer) delete nplayer;
}

void MainWindow::on_pushButton_clicked() {
    // Create the `nPlayer` window if it doesn't exist
    if (!nplayer) {
        nplayer = new nPlayer(this);
        connect(nplayer, &nPlayer::choiceMade, this, &MainWindow::handleChoiceMade);
    }

    this->hide();    // Hide the main window
    nplayer->show(); // Show the `nPlayer` window
}

void MainWindow::handleChoiceMade() {
    // Create the `newWindow` if it doesn't exist
    if (!newWindow) {
        newWindow = new newwindow(this);
    }

    nplayer->close();    // Close the `nPlayer` window
    newWindow->show();   // Show the `newWindow`
}
