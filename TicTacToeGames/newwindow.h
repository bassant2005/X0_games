#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QDialog>

namespace Ui {
class newwindow;
}

class newwindow : public QDialog
{
    Q_OBJECT

public:
    explicit newwindow(QWidget *parent = nullptr);
    ~newwindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::newwindow *ui;
};

#endif // NEWWINDOW_H