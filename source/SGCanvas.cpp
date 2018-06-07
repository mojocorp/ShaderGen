/************************************************************************
*                                                                       *
*               Copyright (C) 2002-2005  3Dlabs Inc. Ltd.               *
*                                                                       *
*                        All rights reserved.                           *
*                                                                       *
* Redistribution and use in source and binary forms, with or without    *
* modification, are permitted provided that the following conditions    *
* are met:                                                              *
*                                                                       *
*     Redistributions of source code must retain the above copyright    *
*     notice, this list of conditions and the following disclaimer.     *
*                                                                       *
*     Redistributions in binary form must reproduce the above           *
*     copyright notice, this list of conditions and the following       *
*     disclaimer in the documentation and/or other materials provided   *
*     with the distribution.                                            *
*                                                                       *
*     Neither the name of 3Dlabs Inc. Ltd. nor the names of its         *
*     contributors may be used to endorse or promote products derived   *
*     from this software without specific prior written permission.     *
*                                                                       *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   *
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     *
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS     *
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE        *
* COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, *
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  *
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT    *
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN     *
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                           *
*                                                                       *
************************************************************************/

#include <QKeyEvent>
#include <QMessageBox>

#include "SGCanvas.h"
#include "SGCanvasMouseHandler.h"
#include "SGFixedGLState.h"
#include "SGFrame.h"
#include "SGShaderTextWindow.h"
#include "SGTextures.h"
#include <stdio.h>

namespace {
/******************************************************
The following functions are used to convert
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
}

SGCanvas::SGCanvas(SGFrame* frame)
  : QOpenGLWidget(frame)
  , CameraZ(-5)
  , m_mouse(this)
  , m_glCompiled(false)
{
    setFocusPolicy(Qt::StrongFocus);
    m_mode = GLModeChoiceFixed;
    m_frame = frame;
    m_models = new SGModels();
    m_modelCurrent = SGModels::ModelTorus;
}

SGCanvas::~SGCanvas()
{
    delete m_models;
}

SGFixedGLState*
SGCanvas::getGLState()
{
    return m_frame->getGLState();
}

void
SGCanvas::initializeGL()
{
    // check OpenGl implementation
    const int gl_major = context()->format().majorVersion();

    int gl_numTextures;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &gl_numTextures);
    if (gl_major < 2.0) {
        QString errorString(tr("You must have OpenGL 2.0 compliant drivers to run ShaderGen!"));
        QMessageBox::critical(this, tr("OpenGL 2.0 Driver Not Found"), errorString);

        exit(1);
    } else if (gl_numTextures < NUM_TEXTURES) {
        QString errorString(tr("Your OpenGL Graphics Card Only Supports %1 Texture Units, Some "
                               "ShaderGen Features May Not Work As Expected!")
                              .arg(gl_numTextures));
        QMessageBox::critical(this, tr("Insufficient Texture Units"), errorString);
    }

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    m_prog.create();
}

void
SGCanvas::paintGL()
{
    const float aspect = (float)width() / (float)height();
    const float vp = 0.8f;
    const float left = -vp;
    const float right = vp;
    const float bottom = -vp / aspect;
    const float top = vp / aspect;
    const float zNear = 2;
    const float zFar = 10;
    const float zoom = m_mouse.getZoom();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const float fov = 45.f;

    m_projection.setToIdentity();
    if (m_frame->isPerspective()) {
        m_projection.perspective(fov * zoom, aspect, zNear, zFar);
    } else {
        m_projection.ortho(3 * left * zoom, 3 * right * zoom, 3 * bottom * zoom, 3 * top * zoom,
                           zNear, zFar);
    }
    glMultMatrixf(m_projection.constData());

    m_modelView.setToIdentity();
    if (m_frame->isPerspective()) {
        m_modelView.translate(0.0f, 0.0f, CameraZ - 1.0f);
    } else {
        m_modelView.translate(0.0f, 0.0f, CameraZ);
    }
    m_modelView.rotate(20.0f, 1.0f, 0.0f, 0.0f);
    m_modelView *= m_mouse.matrix();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    PrintOpenGLError();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    SGFixedGLState* glState = getGLState();
    for (GLint unit = 0; unit < NUM_TEXTURES; unit++) {
        const Texture& texture = glState->getTexture(unit);
        if (texture.textureEnabled) {
            m_frame->getTextures()->bind(texture.textureCurrentSelection, unit);
        } else {
            m_frame->getTextures()->release(unit);
        }
    }

    PrintOpenGLError();

    setupFromFixedState();

    if (m_mode == SGCanvas::GLModeChoiceFixed) {
        m_prog.release();
    } else {
        m_prog.bind();
        for (int unit = 0; unit < NUM_TEXTURES; unit++) {
            m_prog.setUniformValue(qPrintable(QString("texUnit%1").arg(unit)), unit);
        }
    }

    PrintOpenGLError();
    glPushMatrix();

    glMultMatrixf(m_modelView.constData());
    m_models->drawModel(m_modelCurrent, getGLState()->getNormalizeEnable());
    PrintOpenGLError();
    glPopMatrix();

    glFinish();
    PrintOpenGLError();
}

void
SGCanvas::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void
SGCanvas::setupFromFixedState()
{
    PrintOpenGLError();
    SGFixedGLState* glState = getGLState();

    glEnable(GL_AUTO_NORMAL);

    if (glState->getLightingEnable()) {
        glEnable(GL_LIGHTING);

        for (int i = 0; i < 8; i++) {
            const Light& light = glState->getLight(i);
            if (light.lightEnabled) {
                glLightf(GL_LIGHT0 + i, GL_POSITION, light.lightPositionVector);
                glLightf(GL_LIGHT0 + i, GL_AMBIENT, light.lightAmbientColorVector);
                glLightf(GL_LIGHT0 + i, GL_DIFFUSE, light.lightDiffuseColorVector);
                glLightf(GL_LIGHT0 + i, GL_SPECULAR, light.lightSpecularColorVector);
                glLightf(GL_LIGHT0 + i, GL_SPOT_DIRECTION, light.lightSpotDirectionVector);
                glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, light.lightSpotExponent);
                glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, light.lightSpotCutoff);
                glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, light.lightConstantAttenuation);
                glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, light.lightLinearAttenuation);
                glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, light.lightQuadraticAttenuation);
                glEnable(GL_LIGHT0 + i);
            } else {
                glDisable(GL_LIGHT0 + i);
            }
        }
    } else {
        glDisable(GL_LIGHTING);
    }
    if (glState->getNormalizeEnable()) {
        glEnable(GL_NORMALIZE);
    } else {
        glDisable(GL_NORMALIZE);
    }
    if (glState->getRescaleNormalEnable()) {
        glEnable(GL_RESCALE_NORMAL);
    } else {
        glDisable(GL_RESCALE_NORMAL);
    }

    if (glState->getSeparateSpecularColorEnable()) {
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    } else {
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
    }

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    const Material& material = glState->getMaterial();
    glMaterialf(GL_FRONT, GL_DIFFUSE, material.materialDiffuseColorVector);
    glMaterialf(GL_FRONT, GL_SPECULAR, material.materialSpecularColorVector);
    glMaterialf(GL_FRONT, GL_AMBIENT, material.materialAmbientColorVector);
    glMaterialf(GL_FRONT, GL_SHININESS, material.materialShininess);
    glMaterialf(GL_FRONT, GL_EMISSION, material.materialEmissionColorVector);

    if (glState->getFogEnable()) {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, glState->getFog().fogMode);
        glFogi(GL_FOG_COORD_SRC, glState->getFog().fogSource);
        glFogf(GL_FOG_COLOR, glState->getFog().fogColorVector);
        glFogf(GL_FOG_DENSITY, glState->getFog().fogDensity);
        glFogf(GL_FOG_START, glState->getFog().fogStart);
        glFogf(GL_FOG_END, glState->getFog().fogEnd);
    } else {
        glDisable(GL_FOG);
    }
}

QVector3D
SGCanvas::getWorldSpace(int x, int y)
{
    QVector3D v(x, height() - 1 - y, CameraZ);

    return v.unproject(m_modelView, m_projection, QRect(0, 0, width(), height()));
}

QVector2D
SGCanvas::getNormalizedPosition(const QPoint& pos) const
{
    return QVector2D(pos.x() / float(width()), pos.y() / float(height()));
}

void
SGCanvas::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
        case Qt::Key_PageDown: // page down
            m_mouse.setZoom(m_mouse.getZoom() - 0.1f);
            break;
        case Qt::Key_PageUp: // page up
            m_mouse.setZoom(m_mouse.getZoom() + 0.1f);
            break;
        default:
            break;
    }

    update();
}

void
SGCanvas::mousePressEvent(QMouseEvent* event)
{
    m_mouse.onMousePress(event);
    update();
}

void
SGCanvas::mouseMoveEvent(QMouseEvent* event)
{
    m_mouse.onMouseMove(event);
    update();
}

void
SGCanvas::mouseReleaseEvent(QMouseEvent* /*event*/)
{
}

bool
SGCanvas::linkShaders(const QString& vertexShader, const QString& fragmentShader)
{
    if (!m_glCompiled && !compileShaders(vertexShader, fragmentShader)) {
        writeMessage(tr("Compilation failed, not attempting link, check shader code"));
        return false;
    }

    writeMessage(tr("Attempting to link programs...."));
    m_prog.link();

    if (m_prog.log().length() > 0) {
        m_frame->getShaderTextWindow()->log(m_prog.log());
    }

    if (!m_prog.isLinked()) {
        writeMessage(tr("Error in linking programs!!"));
        return false;
    }

    writeMessage(tr("Linked programs successfully"));

    return true;
}

bool
SGCanvas::compileShaders(const QString& vertexShader, const QString& fragmentShader)
{
    m_glCompiled = false;

    m_prog.removeAllShaders();

    if (!m_prog.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader)) {
        writeMessage(tr("Vertex shader failed to compile"));
        return false;
    } else {
        writeMessage(tr("Vertex shader compiled successfully"));
    }
    if (m_prog.log().length() > 0) {
        m_frame->getShaderTextWindow()->log(m_prog.log());
    }

    if (!m_prog.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader)) {
        writeMessage(tr("Fragment shader failed to compile"));
        return false;
    } else {
        writeMessage(tr("Fragment shader compiled successfully"));
    }
    if (m_prog.log().length() > 0) {
        m_frame->getShaderTextWindow()->log(m_prog.log());
    }

    m_glCompiled = true;

    return m_glCompiled;
}

void
SGCanvas::setMode(GLMode m)
{
    if (m_mode == m)
        return;

    m_mode = m;
    if (m_mode == GLModeChoiceShader) {
        m_glCompiled = false;

        const QString vert = m_frame->getVertexShader();
        const QString frag = m_frame->getFragmentShader();

        linkShaders(vert, frag);
    }
    update();
}

void
SGCanvas::writeMessage(const QString str)
{
    m_frame->setStatusText(str);
    m_frame->getShaderTextWindow()->log(str);
}
