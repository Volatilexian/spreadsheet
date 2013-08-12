#-------------------------------------------------
#
# Project created by QtCreator 2013-08-12T11:17:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spreadsheet
TEMPLATE = app


SOURCES += \
    spreadsheet.cpp \
    sortdialog.cpp \
    mainwindow.cpp \
    main.cpp \
    gotocell.cpp \
    finddialog.cpp \
    cell.cpp

HEADERS  += \
    sortdialog.h \
    mainwindow.h \
    gotocell.h \
    finddialog.h \
    cell.h \
    spreadsheet.h

FORMS    += \
    sortdialog.ui \
    mainwindow.ui \
    gotocell.ui \
    finddialog.ui

RESOURCES += \
    icons.qrc
