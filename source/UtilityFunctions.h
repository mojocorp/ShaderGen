#pragma once

#include <QColor>
#include <GL/glew.h>

/******************************************************
The following two functions are used to convert color
    data back and forth between OpenGL and Qt.
******************************************************/
inline void glFogf (GLenum pname, const QColor& c) {
    const float color[] = {c.redF(), c.greenF(), c.blueF()};
    glFogfv(pname, color);
}

inline void glLightf(GLenum light, GLenum pname, const QColor& c) {
    const float color[] = {c.redF(), c.greenF(), c.blueF()};
    glLightfv(light, pname, color);
}

inline void glMaterialf(GLenum face, GLenum pname, const QColor& c) {
    const float color[] = {c.redF(), c.greenF(), c.blueF()};
    glMaterialfv(face, pname, color);
}

inline void glTexEnvf(GLenum target, GLenum pname, const QColor& c) {
    const float color[] = {c.redF(), c.greenF(), c.blueF()};
    glTexEnvfv(target, pname, color);
}

