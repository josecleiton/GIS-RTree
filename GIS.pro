#-------------------------------------------------
#
# Project created by QtCreator 2018-08-25T07:43:52
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = GIS
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
        main.cpp \
    variaveis.cpp \
    rtree/rtree.cpp \
    spatialdata/circularlist.cpp \
    spatialdata/disk.cpp \
    spatialdata/spatialdata.cpp \
    ui/findwindow.cpp \
    ui/insertcirclewindow.cpp \
    ui/insertpointwindow.cpp \
    ui/insertwindow.cpp \
    ui/mainwindow.cpp \
    ui/rectanglesearchwindow.cpp \
    ui/renderarea.cpp \
    ui/searchwindow.cpp \

HEADERS += \
    variaveis.hpp \
    variaveis.hpp \
    rtree/rtree.hpp \
    spatialdata/circularlist.hpp \
    spatialdata/disk.hpp \
    spatialdata/spatialdata.hpp \
    ui/findwindow.hpp \
    ui/insertcirclewindow.hpp \
    ui/insertpointwindow.hpp \
    ui/insertwindow.hpp \
    ui/mainwindow.hpp \
    ui/rectanglesearchwindow.hpp \
    ui/regex.hpp \
    ui/renderarea.hpp \
    ui/searchwindow.hpp \

FORMS += \
    ui/findwindow.ui \
    ui/insertcirclewindow.ui \
    ui/insertpointwindow.ui \
    ui/insertwindow.ui \
    ui/mainwindow.ui \
    ui/rectanglesearchwindow.ui \
    ui/searchwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    LICENSE \
    .gitignore \
    rtree/rtree.bin \
    spatialdata/formas.bin \
    ui/jorge.png \
    ui/ui.pri

INCLUDEPATH += $$PWD/ui \
        $$PWD/rtree \
        $$PWD/spatialdata
