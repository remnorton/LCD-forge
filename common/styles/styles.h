#ifndef STYLES_H
#define STYLES_H
#include <QtCore>
#include <QStyle>
#include "qplastiquestyle.h"
#include "qcleanlooksstyle.h"


QStringList legasyStyles();
QStyle *getLegasyStyle(QString styleName);

#endif // STYLES_H
