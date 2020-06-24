#include "bitmaptresdialog.h"

BitmapTresDialog::BitmapTresDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

quint8 BitmapTresDialog::getTreshold()
{
    return stTres->value();
}

void BitmapTresDialog::setTreshold(quint8 tres)
{
    stTres->setValue(tres);
}
