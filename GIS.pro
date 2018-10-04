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
        mainwindow.cpp \
    circular_list.cpp \
    disk.cpp \
    rtree.cpp \
    spatialdata.cpp \
    insertwindow.cpp

HEADERS += \
    stdlib.hpp \
    mainwindow.hpp \
    circular_list.hpp \
    disk.hpp \
    rtree.hpp \
    spatialdata.hpp \
    insertwindow.hpp

FORMS += \
        mainwindow.ui \
    insertwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    LICENSE \
    .gitignore \
    formas.bin
