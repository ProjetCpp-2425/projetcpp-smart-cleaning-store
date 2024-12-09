QT       += core gui core sql widgets network printsupport serialport charts

CONFIG   += c++11

SOURCES  += main.cpp \
            arduino.cpp \
            conge.cpp \
            dashboarddialog.cpp \
            dialogmodifier.cpp \
            employe.cpp \
            equipe.cpp \
            equipement.cpp \
            mainwindow.cpp \
            service.cpp \
            connection.cpp \
            servicewindow.cpp \
            clientwindow.cpp \
            client.cpp

HEADERS  += \
    arduino.h \
    conge.h \
    dashboarddialog.h \
    dialogmodifier.h \
    employe.h \
    equipe.h \
    equipement.h \
    mainwindow.h \
    service.h \
    connection.h \
    servicewindow.h \
    clientwindow.h \
    client.h

FORMS    += \
    dashboarddialog.ui \
    dialogmodifier.ui \
    equipement.ui \
    mainwindow.ui \
    servicewindow.ui \
    clientwindow.ui

RESOURCES += \
    Resource.qrc





