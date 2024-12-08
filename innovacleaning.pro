QT       += core gui core sql widgets network printsupport serialport charts

CONFIG   += c++11

SOURCES  += main.cpp \
            arduino.cpp \
            service.cpp \
            connection.cpp \
            servicewindow.cpp

HEADERS  += \
    arduino.h \
    service.h \
    connection.h \
    servicewindow.h

FORMS    += \
    servicewindow.ui

RESOURCES += \
    Resource.qrc





