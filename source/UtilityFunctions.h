#pragma once

#include <QColor>
#include <QVector3D>
#include <QVector4D>
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

inline void glTexGenf(GLenum coord, GLenum pname, const QVector4D& v) {
    const float vector[] = {v.x(), v.y(), v.z(), v.w()};
    glTexGenfv(coord, pname, vector);
}

inline void glLightf(GLenum light, GLenum pname, const QVector3D& v) {
    const float vector[] = {v.x(), v.y(), v.z()};
    glLightfv(light, pname, vector);
}

inline void glLightf(GLenum light, GLenum pname, const QVector4D& v) {
    const float vector[] = {v.x(), v.y(), v.z(), v.w()};
    glLightfv(light, pname, vector);
}
