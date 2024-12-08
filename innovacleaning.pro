QT       += core gui core sql widgets network printsupport serialport charts

CONFIG   += c++11

SOURCES  += main.cpp \
            arduino.cpp \
            service.cpp \
            connection.cpp \
            servicewindow.cpp \
            clientwindow.cpp \
            client.cpp

HEADERS  += \
    arduino.h \
    service.h \
    connection.h \
    servicewindow.h \
    clientwindow.h \
    client.h

FORMS    += \
    servicewindow.ui \
    clientwindow.ui

RESOURCES += \
    Resource.qrc





