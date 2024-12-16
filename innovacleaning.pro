QT       += core gui sql widgets network printsupport serialport charts

CONFIG   += c++11

SOURCES  += main.cpp \
            arduino.cpp \
            employewindow.cpp \
            service.cpp \
            connection.cpp \
            servicewindow.cpp \
            clientwindow.cpp \
            produitwindow.cpp \
            equipement.cpp \
            equipe.cpp \
            client.cpp \
            conge.cpp \
            employe.cpp \
            dialogmodifier.cpp \
            dashboarddialog.cpp \
            produit.cpp \
            historique.cpp \
            curvewidget.cpp

HEADERS  += \
    arduino.h \
    employewindow.h \
    service.h \
    connection.h \
    servicewindow.h \
    clientwindow.h \
    produitwindow.h \
    equipement.h \
    equipe.h \
    conge.h \
    employe.h \
    dialogmodifier.h \
    dashboarddialog.h \
    client.h \
    produit.h \
    historique.h \
    curvewidget.h

FORMS    += \
    employewindow.ui \
    servicewindow.ui \
    clientwindow.ui \
    equipement.ui \
    dashboarddialog.ui \
    dialogmodifier.ui \
    produitwindow.ui

RESOURCES += \
    Resource.qrc \
    img.qrc





