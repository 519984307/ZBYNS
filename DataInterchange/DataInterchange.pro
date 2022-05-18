#-------------------------------------------------
#
# Project created by QtCreator 2020-05-20T22:28:50
#
#-------------------------------------------------

QT       -= gui

QT      += network

CONFIG += plugin

TARGET = DataInterTCP
TEMPLATE = lib

CONFIG += skip_target_version_ext
VERSION = 1.0.1.5
QMAKE_TARGET_COPYRIGHT = "Copyright 2020 Shen zhen zhong bai yuan"

DEFINES += DATAINTERCHANGE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        datainterchange.cpp \
    tcpserver.cpp \
    tcpclient.cpp

HEADERS += \
        datainterchange.h \
        datainterchange_global.h \ 
    datainterchangeinterface.h \
    tcpserver.h \
    tcpclient.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug, debug|release): {
unix:TARGET=$$join(TARGET,,,_debug)
win32:TARGET=$$join(TARGET,,Cc1_,d)
}
else:CONFIG(release, debug|release): {
unix:TARGET=$$join(TARGET,,,_release)
win32:TARGET=$$join(TARGET,,Cc1_,)
}

DESTDIR+=../MainUI/Plugins
