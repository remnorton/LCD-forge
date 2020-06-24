#ifndef IMAGEDATADIALOG_H
#define IMAGEDATADIALOG_H

#include "ui_imagedatadialog.h"

enum
{
    CO_RGB = 0,
    CO_BGR
};

class ImageDataDialog : public QDialog, private Ui::ImageDataDialog
{
    Q_OBJECT

public:
    explicit ImageDataDialog(QWidget *parent = nullptr);
    quint8 getColorOrder();
    quint8 getBitPerPixel();
};

#endif // IMAGEDATADIALOG_H
