QT       += core gui core sql widgets network printsupport serialport charts

CONFIG   += c++11

SOURCES  += main.cpp \
            arduino.cpp \
            service.cpp \
            connection.cpp \
            mainwindow.cpp

HEADERS  += mainwindow.h \
    arduino.h \
    service.h \
    connection.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resource.qrc





