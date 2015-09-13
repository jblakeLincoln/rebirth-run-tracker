#-------------------------------------------------
#
# Project created by QtCreator 2015-07-20T02:51:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rebirth-run-tracker
TEMPLATE = app

#CONFIG += console

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/RebirthItem.cpp

HEADERS  += src/MainWindow.h \
    src/RebirthItem.h \
    RebirthRun.h

FORMS    += src/MainWindow.ui


RESOURCES += resources.qrc

win32:RC_ICONS += images/icon.ico

VERSION = 0.1

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
