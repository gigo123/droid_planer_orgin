#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T10:16:07
#
#-------------------------------------------------

QT    += core gui

QT += network
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = grapfic_plan
TEMPLATE = app

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
        main.cpp \
        mainwindow.cpp \
    baterfly_dialog.cpp \
    box_line_dialog.cpp \
    circledialog.cpp \
    endless_dialog.cpp \
    line_dialog.cpp \
    manevr_dialog.cpp \
    manevr_item.cpp \
    march_dialog.cpp \
    point_dialog.cpp \
    region_dialog.cpp \
    snake_dialog.cpp \
    stratecoord.cpp \ 
    pointt.cpp


HEADERS += \
        mainwindow.h \
    baterfly_dialog.h \
    box_line_dialog.h \
    circledialog.h \
    endless_dialog.h \
    line_dialog.h \
    manevr_dialog.h \
    manevr_item.h \
    march_dialog.h \
    point_dialog.h \
    region_dialog.h \
    snake_dialog.h \
    stratecoord.h \
    pointt.h


FORMS += \
        mainwindow.ui \
    baterfly_dialog.ui \
    box_line_dialog.ui \
    circledialog.ui \
    endless_dialog.ui \
    line_dialog.ui \
    maner_dialog.ui \
    march_dialog.ui \
    point_dialog.ui \
    region_dialog.ui \
    snake_dialog.ui

SUBDIRS += \
    grapfic_plan.pro

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../QMapControl 0.9.7.9/build-QMapControl-Desktop_Qt_5_9_3_MinGW_32bit-Debug/Samples/bin/' -lqmapcontrol0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../QMapControl 0.9.7.9/build-QMapControl-Desktop_Qt_5_9_3_MinGW_32bit-Debug/Samples/bin/' -lqmapcontrol0
else:unix: LIBS += -L$$PWD/'../../QMapControl 0.9.7.9/build-QMapControl-Desktop_Qt_5_9_3_MinGW_32bit-Debug/Samples/bin/' -lqmapcontrol

INCLUDEPATH += $$PWD/'../../QMapControl 0.9.7.9/QMapControl/src'
DEPENDPATH += $$PWD/'../../QMapControl 0.9.7.9/QMapControl/src'
