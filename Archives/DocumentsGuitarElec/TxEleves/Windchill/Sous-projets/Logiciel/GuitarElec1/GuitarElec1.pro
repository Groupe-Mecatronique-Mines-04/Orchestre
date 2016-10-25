#-------------------------------------------------
#
# Project created by QtCreator 2015-11-24T16:44:23
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GuitarElec1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    firstload.cpp \
    manual.cpp \
    sequence.cpp \
    moteur1.cpp \
    partition.cpp \
    preferences.cpp \
    moteuroption.cpp \
    data.cpp

HEADERS  += mainwindow.h \
    firstload.h \
    manual.h \
    sequence.h \
    moteur1.h \
    partition.h \
    preferences.h \
    moteuroption.h \
    data.h

FORMS    += mainwindow.ui \
    preferences.ui \
    moteuroption.ui
