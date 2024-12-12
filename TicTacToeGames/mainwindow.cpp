#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newwindow.h" // Include the secondary window

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , newWindow(nullptr) // Initialize the new window pointer
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
    if (newWindow) delete newWindow; // Clean up the secondary window
}

void MainWindow::on_pushButton_clicked() {
    // Create the new window if it doesn't already exist
    if (!newWindow) {
        newWindow = new newwindow(this); // Pass the main window as the parent
    }

    // Hide the main window and show the new window
    this->hide();
    newWindow->show();
}
