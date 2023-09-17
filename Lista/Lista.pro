TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

SOURCES += \
        main.c \
        src/lista.c

HEADERS += \
    include/lista.h
