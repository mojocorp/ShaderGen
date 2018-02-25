#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

/******************************************************
The following two functions are used to convert color
    data back and forth between OpenGL and Qt.
******************************************************/
inline void
glFogf(GLenum pname, const QColor& c)
{
    const GLfloat color[] = { (GLfloat)c.redF(), (GLfloat)c.greenF(), (GLfloat)c.blueF() };
    glFogfv(pname, color);
}

inline void
glLightf(GLenum light, GLenum pname, const QColor& c)
{
    const GLfloat color[] = { (GLfloat)c.redF(), (GLfloat)c.greenF(), (GLfloat)c.blueF() };
    glLightfv(light, pname, color);
}

inline void
glMaterialf(GLenum face, GLenum pname, const QColor& c)
{
    const GLfloat color[] = { (GLfloat)c.redF(), (GLfloat)c.greenF(), (GLfloat)c.blueF() };
    glMaterialfv(face, pname, color);
}

inline void
glMultiTexCoord(const QVector2D& v, GLint u)
{
    glMultiTexCoord2f(u, v.x(), v.y());
}

inline void
glLightf(GLenum light, GLenum pname, const QVector3D& v)
{
    const GLfloat vector[] = { v.x(), v.y(), v.z() };
    glLightfv(light, pname, vector);
}

inline void
glLightf(GLenum light, GLenum pname, const QVector4D& v)
{
    const GLfloat vector[] = { v.x(), v.y(), v.z(), v.w() };
    glLightfv(light, pname, vector);
}



