QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    carbuttonpanel.cpp \
    display.cpp \
    door.cpp \
    ecs.cpp \
    elevator.cpp \
    floor.cpp \
    floorbutton.cpp \
    logconsole.cpp \
    main.cpp \
    mainwindow.cpp \
    passenger.cpp \
    passengeraction.cpp \
    sensor.cpp \
    simulationmanager.cpp

HEADERS += \
    carbuttonpanel.h \
    display.h \
    door.h \
    ecs.h \
    elevator.h \
    floor.h \
    floorbutton.h \
    logconsole.h \
    mainwindow.h \
    passenger.h \
    passengeraction.h \
    sensor.h \
    simulationmanager.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
