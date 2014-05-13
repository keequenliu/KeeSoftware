#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T11:22:56
#
#-------------------------------------------------

QT       += core gui

CONFIG +=warn_off debug_and_release

VERSION=0.1

#库或程序
#TEMPLATE = lib
TEMPLATE = app

CONFIG(debug, debug|release):TARGET = KS_APP
CONFIG(release, debug|release):TARGET = KS_APP


OBJECTS_DIR=../.obj
MOC_DIR=../.obj


#QMAKE_RPATHDIR=$$DESTDIR

#编译完后执行脚本
#CONFIG(release,debug|release) QMAKE_POST_LINK += ../install/VMInstall.sh


SOURCES += main.cpp\
        mainwindow.cpp \
    Network/Socket.cpp \
    Log/Log.cpp \
    Utils/Singleton.cpp \
    Utils/Diagnostic.cpp

HEADERS  += mainwindow.h \
    Network/Socket.h \
    Log/Log.h \
    Utils/Singleton.h \
    Utils/Diagnostic.h

FORMS    += mainwindow.ui
