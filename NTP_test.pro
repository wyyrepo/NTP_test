#-------------------------------------------------
#
# Project created by QtCreator 2017-02-25T10:39:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = NTP_test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ntp.c

HEADERS  += mainwindow.h \
    ntp.h \
    version.h

FORMS    += mainwindow.ui
