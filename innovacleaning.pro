QT       += core gui

CONFIG   += c++11

SOURCES  += main.cpp \
            client.cpp \
            connection.cpp \
            mainwindow.cpp

HEADERS  += mainwindow.h \
    client.h \
    connection.h

FORMS    += mainwindow.ui

QT += widgets

QT += core sql

QT += core

QT += core gui serialport

RESOURCES += \
    Resource.qrc





