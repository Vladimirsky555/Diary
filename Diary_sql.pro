#-------------------------------------------------
#
# Project created by QtCreator 2020-09-25T21:07:42
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Diary_sql
TEMPLATE = app

win32: RC_ICONS = $$PWD/bin/images/app.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    data.cpp \
    model.cpp \
    dialog.cpp \
    application.cpp \
    showitemwindow.cpp \
    regexphighlighter.cpp \
    logindialog.cpp

HEADERS  += mainwindow.h \
    data.h \
    model.h \
    dialog.h \
    application.h \
    showitemwindow.h \
    regexphighlighter.h \
    logindialog.h

FORMS    += mainwindow.ui \
    dialog.ui \
    showitemwindow.ui \
    logindialog.ui

RESOURCES += \
    bin/img.qrc

