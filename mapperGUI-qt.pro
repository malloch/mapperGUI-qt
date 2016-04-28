#-------------------------------------------------
#
# Project created by QtCreator 2014-07-29T16:21:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mapperGUI-qt
TEMPLATE = app

CONFIG += c++11
CONFIG += no_keywords #signals
QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
LIBS += -stdlib=libc++

DEFINES += __GXX_EXPERIMENTAL_CXX0X__

SOURCES += main.cpp\
        mainwindow.cpp \
    listtab.cpp \
    signallist.cpp \
    rangeeditor.cpp \
    signalsearchbar.cpp \
    listview.cpp \
    tab.cpp \
    mapperwidget.cpp \
    mapprops.cpp \
    bipartitemapview.cpp

HEADERS  += mainwindow.h \
    listtab.h \
    signallist.h \
    rangeeditor.h \
    signalsearchbar.h \
    listview.h \
    tab.h \
    mapperwidget.h \
    mapprops.h \
    bipartitemapview.h

FORMS    += mainwindow.ui \
    signallist.ui \
    rangeeditor.ui \
    signalsearchbar.ui \
    listtab.ui \
    listview.ui \
    mapprops.ui \
    bipartitemapview.ui

LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lmapper-0
LIBS += -L$$PWD/../../../../../../usr/local/lib/ -llo

INCLUDEPATH += $$PWD/../../../../../../usr/local/include/mapper-0
INCLUDEPATH += $$PWD/../../../../../../usr/include/
DEPENDPATH += $$PWD/../../../../../../usr/local/include/mapper-0

INCLUDEPATH += $$PWD/../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../usr/local/include
