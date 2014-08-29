#-------------------------------------------------
#
# Project created by QtCreator 2014-08-28T10:00:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Filter_Project
TEMPLATE = app


SOURCES += main.cpp\
        filtreprogrami.cpp

HEADERS  += filtreprogrami.h

FORMS    += filtreprogrami.ui

RESOURCES += \
    Filter.qrc

OTHER_FILES +=

#opencv kütüphanelerini kullanabilmek için
INCLUDEPATH +=C://opencv//release//install//include
LIBS  += C://opencv//release//bin//*.dll
