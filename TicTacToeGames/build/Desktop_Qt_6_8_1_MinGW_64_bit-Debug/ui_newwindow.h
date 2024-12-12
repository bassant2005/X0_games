/********************************************************************************
** Form generated from reading UI file 'newwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWWINDOW_H
#define UI_NEWWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>

QT_BEGIN_NAMESPACE

class Ui_newwindow
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *newwindow)
    {
        if (newwindow->objectName().isEmpty())
            newwindow->setObjectName("newwindow");
        newwindow->resize(783, 463);
        buttonBox = new QDialogButtonBox(newwindow);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        retranslateUi(newwindow);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, newwindow, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, newwindow, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(newwindow);
    } // setupUi

    void retranslateUi(QDialog *newwindow)
    {
        newwindow->setWindowTitle(QCoreApplication::translate("newwindow", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class newwindow: public Ui_newwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWWINDOW_H
