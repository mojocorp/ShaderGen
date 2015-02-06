QT      -= gui
TARGET   = qjson-backport
TEMPLATE = lib
CONFIG += staticlib
DESTDIR  = ../../build

DEFINES += QJSONBACKPORT_LIBRARY
HEADERS += \
    qjson_p.h \
    qjsondocument.h \
    qjsonobject.h \
    qjsonvalue.h \
    qjsonarray.h \
    qjsonwriter_p.h \
    qjsonparser_p.h

SOURCES += \
    qjsonexport.h \
    qjson.cpp \
    qjsondocument.cpp \
    qjsonobject.cpp \
    qjsonarray.cpp \
    qjsonvalue.cpp \
    qjsonwriter.cpp \
    qjsonparser.cpp
