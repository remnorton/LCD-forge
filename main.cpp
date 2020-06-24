/*
 * main.cpp
 *
 *  Created on: 21.10.2018
 *		Author: Rudenko Roman (aka Rem Norton)
 *
 */


#include "mainwindow.h"
#include "config.h"
#include <QApplication>
#include <QStyleFactory>
//#include "styles.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QConfig::init(".","lcdforge.ini");

    //a.setStyle(getLegasyStyle("Cleanlooks"));
    //a.setStyle(getLegasyStyle("Plastique"));
    a.setStyle(QStyleFactory::create("fusion"));

    MainWindow w;
    w.show();

    return a.exec();
}
