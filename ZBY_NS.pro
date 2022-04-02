QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    capturecamera.cpp \
    database.cpp \
    dataform.cpp \
    filterdialog.cpp \
    lockdialog.cpp \
    logform.cpp \
    main.cpp \
    mainwindow.cpp \
    ocr_num.cpp \
    settingdialog.cpp \
    testdialog.cpp

HEADERS += \
    DataInterchange/datainterchangeinterface.h \
    LogController/logcontroller.h \
    capturecamera.h \
    database.h \
    dataform.h \
    filterdialog.h \
    lockdialog.h \
    logform.h \
    mainwindow.h \
    ocr_num.h \
    settingdialog.h \
    testdialog.h

FORMS += \
    dataform.ui \
    filterdialog.ui \
    lockdialog.ui \
    logform.ui \
    mainwindow.ui \
    settingdialog.ui \
    testdialog.ui

TRANSLATIONS += \
    ZBY_NS_en_AS.ts

MOC_DIR=tmp/moc
RCC_DIR=tmp/rcc
UI_DIR=tmp/ui
OBJECTS_DIR=tmp/obj

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx|win32: LIBS += -L$$PWD/CLINET/lib/ -ldhnetsdk

INCLUDEPATH += $$PWD/CLINET/include
DEPENDPATH += $$PWD/CLINET/include

RESOURCES += \
    js.qrc

DISTFILES += \
    res.rc

RC_FILE = res.rc


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/LogController/release/ -lCc1_Logging
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/LogController/debug/ -lCc1_Logging
#else:unix:!macx: LIBS += -L$$PWD/LogController/ -lCc1_Logging

#INCLUDEPATH += $$PWD/LogController/release
#DEPENDPATH += $$PWD/LogController/release

unix:!macx|win32: LIBS += -L$$PWD/CLINET/lib/ -ldhconfigsdk

INCLUDEPATH += $$PWD/CLINET/include
DEPENDPATH += $$PWD/CLINET/include

#unix:!macx|win32: LIBS += -L$$PWD/LogController/release/ -lCc1_Logging

#INCLUDEPATH += $$PWD/LogController/release
#DEPENDPATH += $$PWD/LogController/release

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/build-LogController-Desktop_Qt_5_12_9_MinGW_64_bit-Release/release/ -lCc1_Logging
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/build-LogController-Desktop_Qt_5_12_9_MinGW_64_bit-Release/debug/ -lCc1_Logging
else:unix:!macx: LIBS += -L$$PWD/build-LogController-Desktop_Qt_5_12_9_MinGW_64_bit-Release/ -lCc1_Logging

INCLUDEPATH += $$PWD/build-LogController-Desktop_Qt_5_12_9_MinGW_64_bit-Release/release
DEPENDPATH += $$PWD/build-LogController-Desktop_Qt_5_12_9_MinGW_64_bit-Release/release
