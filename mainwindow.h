/*
 * mainwindow.h
 *
 *  Created on: 21.10.2018
 *		Author: Rudenko Roman (aka Rem Norton)
 *
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    void setTitleSuffix(QString suffix);
private slots:
    void on_actionFont_tool_triggered();
    void on_actionImage_tool_triggered();
};

#endif // MAINWINDOW_H
