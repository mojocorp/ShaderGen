TEMPLATE = lib
TARGET = GLEW
VERSION = 1.12.0

DESTDIR  = ../../build

CONFIG += opengl staticlib
CONFIG -= qt

DEPENDPATH += . src include/GL
INCLUDEPATH += . include

DEFINES += GLEW_STATIC
DEFINES += GLEW_BUILD

# Input
HEADERS += include/GL/glew.h \
           include/GL/glxew.h \
           include/GL/wglew.h
SOURCES += src/glew.c
