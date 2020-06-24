#ifndef WGTIMAGETOOL_H
#define WGTIMAGETOOL_H

#include <QImage>
#include <QGraphicsScene>
#include "ui_wgtimagetool.h"

class wgtImageTool : public QWidget, private Ui::wgtImageTool
{
    Q_OBJECT

public:
    explicit wgtImageTool(QWidget *parent = nullptr);
private slots:
    void on_btImage_clicked();

    void on_btOpenImage_clicked();

    void on_btBitmap_clicked();

private:
    QImage          source;
    QGraphicsScene  scene;
    QString         Content_c;
};

#endif // WGTIMAGETOOL_H
