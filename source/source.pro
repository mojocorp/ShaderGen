TEMPLATE = app
TARGET = ShaderGen
DESTDIR  = ../build

QT += opengl
DEPENDPATH += .
INCLUDEPATH += . ../3rdparty/glew/include

DEFINES += GLEW_STATIC

win32 {
RC_FILE = info.rc
LIBS += -lOpenGL32 -lglu32
}

macx {
ICON = ShaderGen.icns
QMAKE_INFO_PLIST = Info.plist
}

unix:!macx {
LIBS += -lGLU
}

# Input
HEADERS += globals.h\
           SGCanvasWrapper.h \
           SGFrame.h \
           SGFixedGLState.h \
           SGModels.h \
           SGOglNotebook.h \
           SGSurfaces.h \
           SGShaderGenerator.h \
           SGShaderTextWindow.h \
           SGCanvasMouseHandler.h \
           SGCanvas.h \
           SGTextures.h \
           SGOglFogNBPage.h \
           SGOglMaterialNBPage.h \
           UtilityFunctions.h \
           SGOglLightNBPage.h \
           SGOglTextureCoordNBPage.h \
           SGOglTextureEnvNBPage.h \
           QColorButton.h \
           QVectorEdit.h \
           QCodeEditor.h

SOURCES += App.cpp \
           SGCanvasWrapper.cpp \
           SGFrame.cpp \
           SGFixedGLState.cpp \
           SGModels.cpp \
           SGOglNotebook.cpp \
           SGSurfaces.cpp \
           SGShaderGenerator.cpp \
           SGCanvas.cpp \
           SGCanvasMouseHandler.cpp \
           SGShaderTextWindow.cpp \
           SGOglFogNBPage.cpp \
           SGOglMaterialNBPage.cpp \
           SGTextures.cpp \
           SGOglLightNBPage.cpp \
           SGOglTextureCoordNBPage.cpp \
           SGOglTextureEnvNBPage.cpp \
           QColorButton.cpp \
           QVectorEdit.cpp \
           QCodeEditor.cpp

LIBS += -L../build -lGLEW

RESOURCES += textures.qrc
