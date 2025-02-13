QT       += core gui
QT += serialport
QT += multimedia
QT += multimediawidgets
QT += charts
QT += sql
QT += core gui network


TARGET = SoftDJIN
TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += C:\Users\DARTY Sousse\Downloads\opencv\build\include




# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camwidget.cpp \
    loginwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    parcourirdb.cpp \
    welcomepage.cpp

HEADERS += \
    camwidget.h \
    loginwidget.h \
    mainwindow.h \
    parcourirdb.h \
    welcomepage.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    drugsinspectionLV.py \


RESOURCES += \
    ressources/pharmacist.jpg \
    ressources/wlc.png \
    ressources/Noir_1.png \
    ressources/asqii.png \
    ressources/chercher.png \
    ressources/homeicon.png \
    ressources/homey.png \
    ressources/setting.png \
