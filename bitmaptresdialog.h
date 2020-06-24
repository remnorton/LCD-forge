#ifndef BITMAOTRESDIALOG_H
#define BITMAOTRESDIALOG_H

#include "ui_bitmaptresdialog.h"

class BitmapTresDialog : public QDialog, private Ui::BitmaoTresDialog
{
    Q_OBJECT

public:
    explicit BitmapTresDialog(QWidget *parent = nullptr);
    quint8 getTreshold();
    void setTreshold(quint8 tres);
};

#endif // BITMAOTRESDIALOG_H
