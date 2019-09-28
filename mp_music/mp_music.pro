#-------------------------------------------------
#
# Project created by QtCreator 2019-09-03T20:47:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mp_music
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    catch_fileinfo.cpp \
    mylabel.cpp \
    gb2312_ucs2.c \
    song_lrc.cpp

HEADERS  += widget.h \
    catch_fileinfo.h \
    mylabel.h \
    gb2312_ucs2.h \
    song_lrc.h

FORMS    += widget.ui

RESOURCES += \
    image.qrc
