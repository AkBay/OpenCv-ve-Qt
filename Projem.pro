#-------------------------------------------------
#
# Project created by QtCreator 2014-08-12T17:19:13
#
#-------------------------------------------------

QT       += core gui
QT       +=bluetooth
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
#opencv kütüphanelerini kullanabilmek için
INCLUDEPATH +=C://opencv//release//install//include
LIBS  += C://opencv//release//bin//*.dll
