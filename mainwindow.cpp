/*
 * mainwindow.cpp
 *
 *  Created on: 21.10.2018
 *		Author: Rudenko Roman (aka Rem Norton)
 *
 */


#include "wgtfonttool.h"
#include "wgtimagetool.h"
#include "mainwindow.h"
#include "fileinfo.h"

QWidget* current = 0;
static QString title = "LCD forge v.";
//
//
//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    title += stringVersion(fileVersion(qApp->applicationFilePath()));
    setWindowTitle(title);
}

void MainWindow::setTitleSuffix(QString suffix)
{
    setWindowTitle(QString("%1 (%2)").arg(title).arg(suffix.toUpper()));
}

void MainWindow::on_actionFont_tool_triggered()
{
    if (current)
    {
        vlMain->removeWidget(current);
        current->deleteLater();
    }

    current = new wgtFontTool;
    vlMain->addWidget(current);
    setTitleSuffix("Font tool");
}

void MainWindow::on_actionImage_tool_triggered()
{
    if (current)
    {
        vlMain->removeWidget(current);
        current->deleteLater();
    }

    current = new wgtImageTool;
    vlMain->addWidget(current);
    setTitleSuffix("Image tool");
}
