#ifndef STYLES_CPP
#define STYLES_CPP
#include "styles.h"

static QStringList legasyList = {"Plastique","Cleanlooks"};

QStringList legasyStyles()
{
    return legasyList;
}

QStyle* getLegasyStyle(QString styleName)
{
    if (!legasyList.contains(styleName)) return 0;
    if (styleName == "Plastique") return new QPlastiqueStyle;
    if (styleName == "Cleanlooks") return new QCleanlooksStyle;
}

#endif // STYLES_CPP
