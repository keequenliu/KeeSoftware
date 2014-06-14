#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T11:22:56
#
#-------------------------------------------------

#QT       += core gui
config -=qt

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

win32:DEFINES+=_WINDOWS
else:unix:DEFINES+=_UNIX

#widow 平台
win32-msvc2012{
OSG_INCLUDE=C:\Work\Projects\OSG\Install\x86\include
OSG_LIB=C:\Work\Projects\OSG\Install\x86\lib

TIFF_INCLUDE=C:\Work\Projects\OSG\3rdparty\x86\include
TIFF_LIB=C:\Work\Projects\OSG\3rdparty\x86\lib

INCLUDEPATH+=$$OSG_INCLUDE $$TIFF_INCLUDE

LIBS+=-L$$OSG_LIB -losg -losgGA -losgText -losgShadow -losgViewer -losgDB -losgUtil -losgSim -losgTerrain -losgManipulator -losgWidget -lOpenThreads
LIBS+=-L$$TIFF_LIB -llibtiff

}

linux-g++-64{
#3dparty lib
INCLUDEPATH+=/usr/local/include/
LIBS+=-L/usr/lib64 -ltiff

#osg
INCLUDEPATH+=/usr/local/include
LIBS+=-L/usr/local/lib64 -losg -losgGA -losgText -losgShadow -losgViewer -losgDB -losgUtil -losgSim -losgTerrain -losgManipulator -losgWidget -lOpenThreads

}

SOURCES += main.cpp\
    Network/Socket.cpp \
    Log/Log.cpp \
    Utils/Singleton.cpp \
    Utils/Diagnostic.cpp \
    Utils/Time.cpp \
    TS/TSPacket.cpp \
    TIFF/TIFFParse.cpp \
    OSG/RenderState.cpp \
    OSG/CoreViewer.cpp

HEADERS  += Network/Socket.h \
    Log/Log.h \
    Utils/Singleton.h \
    Utils/Diagnostic.h \
    Utils/Time.h \
    TS/TSPacket.h \
    TS/PSI.h \
    TIFF/TIFFParse.h \
    OSG/RenderState.h \
    OSG/CoreViewer.h

FORMS    += mainwindow.ui

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../OSG/Install/x86/lib/ -losg
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../OSG/Install/x86/lib/ -losgd
#else:unix: LIBS += -L$$PWD/../../../OSG/Install/x86/lib/ -losg

#INCLUDEPATH += $$PWD/../../../OSG/Install/x86/include
#DEPENDPATH += $$PWD/../../../OSG/Install/x86/include
