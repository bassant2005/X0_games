#ifndef SUSGAME_H
#define SUSGAME_H

#include <QDialog>

namespace Ui {
class SUSGame;
}

class SUSGame : public QDialog
{
    Q_OBJECT

public:
    explicit SUSGame(QWidget *parent = nullptr);
    ~SUSGame();

private:
    Ui::SUSGame *ui;
};

#endif // SUSGAME_H
