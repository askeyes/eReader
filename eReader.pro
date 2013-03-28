#-------------------------------------------------
#
# Project created by QtCreator 2010-11-19T15:37:18
#
#-------------------------------------------------

QT       += core gui

TARGET = eReader
TEMPLATE = app


SOURCES += main.cpp\
        ereader.cpp \
    imageSytele/imageSytele.cpp \
    explore.cpp \
    myedittext.cpp \
    bookmarkbrowser.cpp

HEADERS  += ereader.h \
    imageSytele/imageSytele.h \
    explore.h \
    myedittext.h \
    bookmarkbrowser.h

FORMS    += ereader.ui \
    explore.ui \
    bookmarkbrowser.ui

RESOURCES += \
    res.qrc

RC_FILE = ebook.rc
