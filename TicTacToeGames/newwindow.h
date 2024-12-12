#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QDialog>
#include "susgame.h"
#include "nplayer.h"

namespace Ui {
class newwindow;
}

class newwindow : public QDialog
{
    Q_OBJECT

public:
    explicit newwindow(QWidget *parent = nullptr);
    ~newwindow();
    void Show();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::newwindow *ui;
    SUSGame *susgame;
    nPlayer *nplayer;
};

#endif // NEWWINDOW_H
