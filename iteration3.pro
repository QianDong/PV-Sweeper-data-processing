#-------------------------------------------------
#
# Project created by QtCreator 2016-04-25T18:42:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = iteration3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    mylabel.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    read.h \
    gettime.h \
    mylabel.h

FORMS    += mainwindow.ui
