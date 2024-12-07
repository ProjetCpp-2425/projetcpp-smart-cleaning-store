QT       += core gui serialport charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    conge.cpp \
    connection.cpp \
    dialogmodifier.cpp \
    dialogscore.cpp \
    employe.cpp \
    main.cpp \
    mainwindow.cpp \
    smtp.cpp

HEADERS += \  \
    arduino.h \
    conge.h \
    connection.h \
    dialogmodifier.h \
    dialogscore.h \
    employe.h \
    mainwindow.h \
    smtp.h

FORMS += \
    dialogmodifier.ui \
    dialogscore.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QT+= sql
QT += charts
QT += network
