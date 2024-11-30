QT       += core gui core sql widgets network printsupport

CONFIG   += c++11

SOURCES  += main.cpp \
            service.cpp \
            connection.cpp \
            mainwindow.cpp

HEADERS  += mainwindow.h \
    service.h \
    connection.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resource.qrc





