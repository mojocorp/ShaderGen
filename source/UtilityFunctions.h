#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#include <GL/glew.h>

/******************************************************
The following two functions are used to convert color
    data back and forth between OpenGL and Qt.
******************************************************/
inline void
glFogf(GLenum pname, const QColor& c)
{
    const GLfloat color[] = { c.redF(), c.greenF(), c.blueF() };
    glFogfv(pname, color);
}

inline void
glLightf(GLenum light, GLenum pname, const QColor& c)
{
    const GLfloat color[] = { c.redF(), c.greenF(), c.blueF() };
    glLightfv(light, pname, color);
}

inline void
glMaterialf(GLenum face, GLenum pname, const QColor& c)
{
    const GLfloat color[] = { c.redF(), c.greenF(), c.blueF() };
    glMaterialfv(face, pname, color);
}

inline void
glTexEnvf(GLenum target, GLenum pname, const QColor& c)
{
    const GLfloat color[] = { c.redF(), c.greenF(), c.blueF() };
    glTexEnvfv(target, pname, color);
}

inline void
glTexGenf(GLenum coord, GLenum pname, const QVector4D& v)
{
    const GLfloat vector[] = { v.x(), v.y(), v.z(), v.w() };
    glTexGenfv(coord, pname, vector);
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

inline void
glVertex(const QVector3D& v)
{
    glVertex3f(v.x(), v.y(), v.z());
}

inline void
glNormal(const QVector3D& v)
{
    glNormal3f(v.x(), v.y(), v.z());
}

inline void
glMultMatrix(const GLfloat* m)
{
    glMultMatrixf(m);
}

inline void
glMultMatrix(const GLdouble* m)
{
    glMultMatrixd(m);
}

inline void
glMultMatrix(const QMatrix4x4& m)
{
    glMultMatrix(m.constData());
}

inline void
glGet(GLenum pname, GLdouble* params)
{
    glGetDoublev(pname, params);
}
inline void
glGet(GLenum pname, GLfloat* params)
{
    glGetFloatv(pname, params);
}
inline void
glGet(GLenum pname, QMatrix4x4& m)
{
    glGet(pname, m.data());
}
