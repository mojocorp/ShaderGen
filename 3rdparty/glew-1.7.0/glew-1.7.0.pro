TEMPLATE = lib
TARGET = GLEW

DESTDIR  = ../../build

CONFIG += opengl
CONFIG -= qt

DEPENDPATH += . src include/GL
INCLUDEPATH += . include

#DEFINES +=  GLEW_STATIC
DEFINES += GLEW_BUILD

# Input
HEADERS += include/GL/glew.h \
           include/GL/glxew.h \
           include/GL/wglew.h
SOURCES += src/glew.c
