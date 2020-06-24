TARGET  = qmotifstyle
PLUGIN_TYPE = styles
PLUGIN_CLASS_NAME = QMotifStylePlugin
load(qt_plugin)

QT = core gui widgets

CONFIG += plugin static

HEADERS += qcdestyle.h
HEADERS += qmotifstyle.h
SOURCES += qcdestyle.cpp
SOURCES += qmotifstyle.cpp
SOURCES += plugin.cpp

OTHER_FILES += motif.json
