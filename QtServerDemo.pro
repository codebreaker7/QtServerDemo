QT += core network
QT += widgets

TARGET = QtServerDemo
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    dataserver.cpp

HEADERS += \
    dataserver.h

