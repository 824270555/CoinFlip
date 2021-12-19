#-------------------------------------------------
#
# Project created by QtCreator 2021-09-08T08:06:23
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CoinFlip
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainscene.cpp \
    mypushbutton.cpp \
    chooselevelscene.cpp \
    playscene.cpp \
    mycoin.cpp \
    dataconfig.cpp

HEADERS  += mainscene.h \
    mypushbutton.h \
    chooselevelscene.h \
    playscene.h \
    mycoin.h \
    dataconfig.h

FORMS    += mainscene.ui

RESOURCES += \
    res.qrc
