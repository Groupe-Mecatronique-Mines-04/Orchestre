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
    sequence.cpp \
    partition.cpp \
    preferences.cpp \
    moteuroption.cpp \
    data.cpp \
    MIDI/Binasc.cpp \
    MIDI/MidiEvent.cpp \
    MIDI/MidiEventList.cpp \
    MIDI/MidiFile.cpp \
    MIDI/MidiMessage.cpp \
    MIDI/Options.cpp \
    melody.cpp \
    music.cpp

HEADERS  += mainwindow.h \
    sequence.h \
    partition.h \
    preferences.h \
    moteuroption.h \
    data.h \
    MIDI/Binasc.h \
    MIDI/MidiEvent.h \
    MIDI/MidiEventList.h \
    MIDI/MidiFile.h \
    MIDI/MidiMessage.h \
    MIDI/Options.h \
    melody.h \
    music.h

FORMS    += mainwindow.ui \
    preferences.ui \
    moteuroption.ui \
    music.ui

RC_FILE = guitar.rc

RESOURCES += \
    ressources.qrc

QMAKE_CXXFLAGS += -std=c++11
