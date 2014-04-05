#-------------------------------------------------
#
# Project created by QtCreator 2013-06-07T15:59:19
#
#-------------------------------------------------

QT       += core gui qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = watering_analizer
TEMPLATE = app
INCLUDEPATH += /usr/include/qwt
LIBS += -L/usr/lib -lqwt

SOURCES += main.cpp\
    com/qextserialport.cpp \
    com/qextserialenumerator_unix.cpp \
    com/posix_qextserialport.cpp \
    analizedialog.cpp \
    dosersetup.cpp \
    makeconfig.cpp \
    globalstatedialog.cpp

HEADERS  += \
    com/qextserialenumerator.h \
    com/qextserialport_global.h \
    com/qextserialport.h \
    analizedialog.h \
    data_structs.h \
    dosersetup.h \
    makeconfig.h \
    globalstatedialog.h

FORMS    += \
    analizedialog.ui \
    dosersetup.ui \
    makeconfig.ui \
    globalstatedialog.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc
