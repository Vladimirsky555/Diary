#-------------------------------------------------
#
# Project created by QtCreator 2020-01-12T16:55:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Diary
TEMPLATE = app

win32: RC_ICONS = $$PWD/bin/images/app.ico
INCLUDEPATH += $${_PRO_FILE_PWD_}/src

QMAKE_TARGET_COMPANY = BearWin
QMAKE_TARGET_PRODUCT = Diary
QMAKE_TARGET_DESCRIPTION = Notes for every day
QMAKE_TARGET_COPYRIGHT = BearWin

HEADERS  += src/application.h \
            src/mainwindow/mainwindow.h \
            src/dialog/additemdialog.h \
			src/dialog/showitemdialog.h \ 
    src/highlighter.h \
    src/model/model.h
     
    


SOURCES += src/main.cpp \
           src/application.cpp \
           src/mainwindow/mainwindow.cpp \
           src/dialog/additemdialog.cpp \          
           src/dialog/showitemdialog.cpp \
    src/highlighter.cpp \
    src/model/model.cpp



FORMS    += src/mainwindow/mainwindow.ui \
            src/dialog/additemdialog.ui \
            src/dialog/showitemdialog.ui

RESOURCES += \
    bin/images.qrc
