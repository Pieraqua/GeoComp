TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

unix:!macx: LIBS += -L$$PWD/../../../../Libs/glfw_build/src/ -lglfw3 -ldl -lglfw3 -lX11 -lpthread -lXrandr -lXi -lfreeimage

INCLUDEPATH += $$PWD/../../../../Libs/glfw-3.3.8/include
DEPENDPATH += $$PWD/../../../../Libs/glfw-3.3.8/include

INCLUDEPATH += $$PWD/../../../../Libs/include
DEPENDPATH += $$PWD/../../../../Libs/include

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../Libs/glfw_build/src/libglfw3.a

SOURCES += \
        main.c \
        src/geometria.c \
        src/geometria_dcel.c \
        src/lista.c \
        src/renderer_sopa.c

HEADERS += \
    include/geometria.h \
    include/geometria_dcel.h \
    include/lista.h \
    include/renderer.h
