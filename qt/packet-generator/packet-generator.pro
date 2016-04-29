#-------------------------------------------------
#
# Project created by QtCreator 2015-12-02T21:02:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = packet-generator
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
    mainwindow.cpp \
    eth802Q.cpp \
    ethheader.cpp \
    ipheader.cpp \
    tcpheader.cpp \
    udpheader.cpp \
    sendsocket.cpp \
    workerthread.cpp

HEADERS  += mainwindow.h \
    eth802Q.h \
    ethheader.h \
    ipheader.h \
    tcpheader.h \
    udpheader.h \
    includes.h \
    sendsocket.h \
    workerthread.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc
