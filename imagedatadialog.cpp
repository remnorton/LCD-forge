#include "imagedatadialog.h"

ImageDataDialog::ImageDataDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

quint8 ImageDataDialog::getColorOrder()
{
    return (rbBGR->isChecked())?CO_BGR:CO_RGB;
}

quint8 ImageDataDialog::getBitPerPixel()
{
    switch (cbPixelFormat->currentIndex())
    {
        case 0: //RGB565
            return 16;
        case 1: //RGB666
            return 18;
        case 2: //RGB888
            return 24;
    }
    return 0;
}
