#include "susgame.h"
#include "ui_susgame.h"

SUSGame::SUSGame(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SUSGame)
{
    ui->setupUi(this);
}

SUSGame::~SUSGame()
{
    delete ui;
}
