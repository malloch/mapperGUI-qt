#-------------------------------------------------
#
# Project created by QtCreator 2014-07-29T16:21:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mapperGUI-qt
TEMPLATE = app

CONFIG += c++11
QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
LIBS += -stdlib=libc++

DEFINES += __GXX_EXPERIMENTAL_CXX0X__

SOURCES += main.cpp\
        mainwindow.cpp \
    graphnode.cpp \
    graphedge.cpp \
    graphtab.cpp \
    listtab.cpp \
    linkview.cpp \
    connectionprops.cpp \
    signallist.cpp \
    livesignalview.cpp \
    patchingtab.cpp \
    devicelist.cpp \
    modeeditor.cpp \
    rangeeditor.cpp \
    mapperstuff.cpp \
    signalsearchbar.cpp \
    listview.cpp \
    tab.cpp

HEADERS  += mainwindow.h \
    graphnode.h \
    graphedge.h \
    graphtab.h \
    listtab.h \
    linkview.h \
    connectionprops.h \
    signallist.h \
    livesignalview.h \
    patchingtab.h \
    devicelist.h \
    modeeditor.h \
    rangeeditor.h \
    mapperstuff.h \
    signalsearchbar.h \
    listview.h \
    tab.h

FORMS    += mainwindow.ui \
    linkview.ui \
    connectionprops.ui \
    signallist.ui \
    livesignalview.ui \
    modeeditor.ui \
    rangeeditor.ui \
    signalsearchbar.ui \
    patchingtab.ui \
    listtab.ui \
    listview.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/release/ -lmapper-0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/debug/ -lmapper-0
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lmapper-0

INCLUDEPATH += $$PWD/../../../../../../usr/local/include/mapper-0
DEPENDPATH += $$PWD/../../../../../../usr/local/include/mapper-0

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/release/ -llo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/debug/ -llo
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -llo

INCLUDEPATH += $$PWD/../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../usr/local/include
