#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    // NewWindow* window = new NewWindow(this); // Create the new window
    // window->setAttribute(Qt::WA_DeleteOnClose); // Ensure the window is deleted on close
    // window->show(); // Show the new window
}
