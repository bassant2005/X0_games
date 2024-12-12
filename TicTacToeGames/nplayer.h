// #ifndef NPLAYER_H
// #define NPLAYER_H

// #include <QDialog>

// namespace Ui {
// class nPlayer;
// }

// class nPlayer : public QDialog
// {
//     Q_OBJECT

// public:
//     explicit nPlayer(QWidget *parent = nullptr);
//     ~nPlayer();
//     int choice;
// private slots:
//     void on_btnPlayerVsPlayer_clicked();

//     void on_btnPlayerVsComputer_clicked();

// private:
//     Ui::nPlayer *ui;
// };


// #endif // NPLAYER_H

#ifndef NPLAYER_H
#define NPLAYER_H

#include <QDialog>

namespace Ui {
class nPlayer;
}

class nPlayer : public QDialog
{
    Q_OBJECT

public:
    explicit nPlayer(QWidget *parent = nullptr);
    ~nPlayer();

    static int choice;

signals:
    void choiceMade(); // Signal to notify that a choice is made

private slots:
    void on_btnPlayerVsPlayer_clicked();
    void on_btnPlayerVsComputer_clicked();

private:
    Ui::nPlayer *ui;
};

#endif // NPLAYER_H
