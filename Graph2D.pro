#-------------------------------------------------
#
# Project created by QtCreator 2019-02-16T14:37:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graph2D
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

INCLUDEPATH += $$PWD/include/
CONFIG += c++11

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/abstractwindow.cpp \
    src/abstractitem.cpp \
    src/graphicsview.cpp \
    src/node.cpp \
    src/edge.cpp \
    src/settingswindow.cpp \
    src/tab.cpp \
    src/abstractalgorithm.cpp \
    src/bfsalgorithm.cpp \
    src/raport.cpp \
    src/dfsalgorithm.cpp \
    src/dejikstralgorithm.cpp

HEADERS += \
        include/mainwindow.h \
    include/abstractwindow.h \
    include/log.h \
    include/abstractitem.h \
    include/graphicsview.h \
    include/node.h \
    include/edge.h \
    include/settingswindow.h \
    include/tab.h \
    include/abstractalgorithm.h \
    include/bfsalgorithm.h \
    include/raport.h \
    include/dfsalgorithm.h \
    include/dejikstralgorithm.h
