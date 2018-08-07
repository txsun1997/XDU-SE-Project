#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T15:13:47
#
#-------------------------------------------------

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = M_OnLibrary
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindlg.cpp \
    buttondelegate.cpp \
    updatedlg.cpp \
    orderBtndelegate.cpp \
    newbookdlg.cpp \
    adddlg.cpp

HEADERS  += mainwindow.h \
    logindlg.h \
    buttondelegate.h \
    updatedlg.h \
    orderBtndelegate.h \
    newbookdlg.h \
    adddlg.h

FORMS    += mainwindow.ui \
    logindlg.ui \
    updatedlg.ui \
    newbookdlg.ui \
    adddlg.ui

RESOURCES += \
    logo.qrc
RC_FILE += icon.rc
