INCLUDEPATH +=  $$PWD \
              $$PWD/motif \

include($$PWD/plastique/plastique.pri)
include($$PWD/cleanlooks/cleanlooks.pri)
#include($$PWD/shared/shared.pri)
              

HEADERS += \
    $$PWD/styles.h

SOURCES += \
    $$PWD/styles.cpp
