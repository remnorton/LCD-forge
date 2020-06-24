#-------------------------------------------------
#
# Project created by QtCreator 2018-10-21T10:47:38
#
#-------------------------------------------------

QT       += core gui
CONFIG   += qscintilla2

LIBS    += libqscintilla2_qt5

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


include(./common/fileinfo/fileinfo.pri)
#include(./common/styles/styles.pri)# old KDE styles
include(./common/config/config.pri)# singleton INI-based config class

TARGET = lcd-forge
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    bitmaptresdialog.cpp \
    imagedatadialog.cpp \
        main.cpp \
        mainwindow.cpp \
    wgtfonttool.cpp \
    wgtimagetool.cpp

HEADERS += \
    bitmaptresdialog.h \
    imagedatadialog.h \
        mainwindow.h \
    wgtfonttool.h \
    wgtimagetool.h

FORMS += \
    bitmaptresdialog.ui \
    imagedatadialog.ui \
        mainwindow.ui \
    wgtfonttool.ui \
    wgtimagetool.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

windows: RC_FILE = lcd-forge.rc

RESOURCES += \
    resources.qrc

DISTFILES += \
    lcd-forge.rc
