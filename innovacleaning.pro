QT       += core gui sql widgets network printsupport serialport charts

CONFIG   += c++11

SOURCES  += main.cpp \
            arduino.cpp \
            service.cpp \
            connection.cpp \
            servicewindow.cpp \
            clientwindow.cpp \
            produitwindow.cpp \
            client.cpp \
            produit.cpp \
            historique.cpp \
            curvewidget.cpp

HEADERS  += \
    arduino.h \
    service.h \
    connection.h \
    servicewindow.h \
    clientwindow.h \
    produitwindow.h \
    client.h \
    produit.h \
    historique.h \
    curvewidget.h

FORMS    += \
    servicewindow.ui \
    clientwindow.ui \
    produitwindow.ui

RESOURCES += \
    Resource.qrc \
    img.qrc





