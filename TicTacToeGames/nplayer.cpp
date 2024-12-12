// #include "nplayer.h"
// #include "ui_nplayer.h"

// nPlayer::nPlayer(QWidget *parent)
//     : QDialog(parent)
//     , ui(new Ui::nPlayer)
// {
//     ui->setupUi(this);
// }

// nPlayer::~nPlayer()
// {
//     delete ui;
// }

// void nPlayer::on_btnPlayerVsPlayer_clicked()
// {
//     choice = 1;
//     this->hide();
// }


// void nPlayer::on_btnPlayerVsComputer_clicked()
// {
//     choice = 2;
//     this->hide();
// }

#include "nplayer.h"
#include "ui_nplayer.h"

int nPlayer::choice = 0; // Initialize the static choice variable

nPlayer::nPlayer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::nPlayer)
{
    ui->setupUi(this);
}

nPlayer::~nPlayer()
{
    delete ui;
}

void nPlayer::on_btnPlayerVsPlayer_clicked()
{
    nPlayer::choice = 1; // Set choice to 1 for Player vs Player
    emit choiceMade();   // Emit the signal
    this->close();       // Close the `nPlayer` window
}

void nPlayer::on_btnPlayerVsComputer_clicked()
{
    nPlayer::choice = 2; // Set choice to 2 for Player vs Computer
    emit choiceMade();   // Emit the signal
    this->close();       // Close the `nPlayer` window
}
