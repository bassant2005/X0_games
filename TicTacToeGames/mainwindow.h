#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "newwindow.h"
#include "nplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void handleChoiceMade(); // Slot to handle when the user makes a choice

private:
    Ui::MainWindow *ui;
    newwindow *newWindow;
    nPlayer *nplayer;
};

#endif // MAINWINDOW_H
