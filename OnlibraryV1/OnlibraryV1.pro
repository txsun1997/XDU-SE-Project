#-------------------------------------------------
#
# Project created by QtCreator 2017-05-16T22:10:21
#
#-------------------------------------------------

QT += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OnlibraryV1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindlg.cpp \
    registerdlg.cpp \
    buttondelegate.cpp \
    detaildlg.cpp \
    orderBtndelegate.cpp \
    scoredlg.cpp \
    cartbuttondelegate.cpp \
    cartdetaildlg.cpp \
    buydlg.cpp \
    shcdlg.cpp \
    QPasswordLineEdit.cpp \
    zhaohuidlg.cpp

HEADERS  += mainwindow.h \
    logindlg.h \
    registerdlg.h \
    buttondelegate.h \
    detaildlg.h \
    orderBtndelegate.h \
    scoredlg.h \
    cartbuttondelegate.h \
    cartdetaildlg.h \
    buydlg.h \
    shcdlg.h \
    QPasswordLineEdit.h \
    zhaohuidlg.h

FORMS    += mainwindow.ui \
    logindlg.ui \
    registerdlg.ui \
    detaildlg.ui \
    scoredlg.ui \
    cartdetaildlg.ui \
    buydlg.ui \
    shcdlg.ui \
    zhaohuidlg.ui

RESOURCES += \
    logo.qrc \
    meigong.qrc \
    icon.qrc

RC_FILE += icon.rc
