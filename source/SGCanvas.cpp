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
#include "UtilityFunctions.h"
#include <stdio.h>

SGCanvas::SGCanvas(SGFrame* frame)
  : QGLWidget(frame)
  , CameraZ(-5)
  , m_mouse(this)
  , m_glCompiled(false)
{
    setFocusPolicy(Qt::StrongFocus);
    m_mode = GLModeChoiceFixed;
    m_frame = frame;
    m_models = new SGModels();
    m_zoom = 0.8f;
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
    checkGlImplementation();

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void
SGCanvas::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    PrintOpenGLError();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    SGFixedGLState* glState = getGLState();
    for (GLint i = 0; i < 5; i++) {
        if (glState->getTexture(i).textureEnabled) {
            m_frame->getTextures()->bind(glState->getTexture(i).textureCurrentSelection, i);
        } else {
            m_frame->getTextures()->release(i);
        }
    }

    PrintOpenGLError();

    if (m_mode == SGCanvas::GLModeChoiceFixed) {
        setupFromFixedState();
    } else {
        if (m_prog.isLinked()) {
            m_prog.bind();
        } else {
            m_prog.release();
        }
    }

    PrintOpenGLError();
    glPushMatrix();
    m_mouse.loadMatrix();
    m_models->drawModel(m_modelCurrent, getGLState()->getNormalizeEnable());
    PrintOpenGLError();
    glPopMatrix();

    glFinish();
    PrintOpenGLError();
}

void
SGCanvas::resizeGL(int width, int height)
{
    const float aspect = (float)width / (float)height;
    const float vp = 0.8f;
    m_left = -vp;
    m_right = vp;
    m_bottom = -vp / aspect;
    m_top = vp / aspect;
    m_znear = 2;
    m_zfar = 10;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    PrintOpenGLError();
    if (m_frame->isPerspective()) {
        glFrustum(m_left * m_zoom, m_right * m_zoom, m_bottom * m_zoom, m_top * m_zoom, m_znear,
                  m_zfar);
    } else {
        glOrtho(3 * m_left * m_zoom, 3 * m_right * m_zoom, 3 * m_bottom * m_zoom,
                3 * m_top * m_zoom, m_znear, m_zfar);
    }
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
    QVector3D v;
    v.setX((float)x / (float)width());
    v.setY(1 - (float)y / (float)height());
    v.setZ(CameraZ);
    v.setX(v.x() * (m_right - m_left));
    v.setY(v.y() * (m_top - m_bottom));
    v.setX(v.x() + m_left);
    v.setY(v.y() + m_bottom);

    return v;
}

void
SGCanvas::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
        case Qt::Key_PageDown: // page down
            m_zoom -= 0.1f;
            break;
        case Qt::Key_PageUp: // page up
            m_zoom += 0.1f;
            break;
        default:
            break;
    }

    updateGL();
}

void
SGCanvas::mousePressEvent(QMouseEvent* event)
{
    m_mouse.onMousePress(event);
    updateGL();
}

void
SGCanvas::mouseMoveEvent(QMouseEvent* event)
{
    m_mouse.onMouseMove(event);
    updateGL();
}

void
SGCanvas::mouseReleaseEvent(QMouseEvent* event)
{
    m_mouse.onMouseRelease(event);
    updateGL();
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
        m_frame->getShaderTextWindow()->log(tr("InfoLog:") + m_prog.log());
    }

    if (!m_prog.isLinked()) {
        writeMessage(tr("Error in linking programs!!"));
        return false;
    } else {
        m_prog.bind();
        for (int i = 0; i < 5; i++) {
            switch (i) {
                case 0:
                    m_prog.setUniformValue("texUnit0", 0);
                    break;
                case 1:
                    m_prog.setUniformValue("texUnit1", 1);
                    break;
                case 2:
                    m_prog.setUniformValue("texUnit2", 2);
                    break;
                case 3:
                    m_prog.setUniformValue("texUnit3", 3);
                    break;
                case 4:
                    m_prog.setUniformValue("texUnit4", 4);
                    break;
                default:
                    break;
            }
        }
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
        m_frame->getShaderTextWindow()->log(tr("InfoLog:") + m_prog.log());
    }

    if (!m_prog.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader)) {
        writeMessage(tr("Fragment shader failed to compile"));
        return false;
    } else {
        writeMessage(tr("Fragment shader compiled successfully"));
    }
    if (m_prog.log().length() > 0) {
        m_frame->getShaderTextWindow()->log(tr("InfoLog:") + m_prog.log());
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
        switchToShaderMode();
    }
    updateGL();
}

int
SGCanvas::switchToShaderMode()
{
    const QString vert = m_frame->getVertexShader();
    const QString frag = m_frame->getFragmentShader();

    m_frame->getShaderTextWindow()->setFragmentShaderText(frag);
    m_frame->getShaderTextWindow()->setVertexShaderText(vert);

    linkShaders(vert, frag);
    updateGL();
    return 0;
}

void
SGCanvas::writeMessage(const QString str)
{
    m_frame->setStatusText(str);
    m_frame->getShaderTextWindow()->log(str);
}

void
SGCanvas::checkGlImplementation()
{
    const int gl_major = context()->format().majorVersion();

    int gl_numTextures;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &gl_numTextures);
    if (gl_major < 2.0) {
        unsupportedOpenGLVersion();
    } else if (gl_numTextures < 5) {
        notEnoughTextureUnits(gl_numTextures);
    }
}

void
SGCanvas::unsupportedOpenGLVersion()
{
    QString errorString(tr("You must have OpenGL 2.0 compliant drivers to run ShaderGen!"));
    QMessageBox::critical(this, tr("OpenGL 2.0 Driver Not Found"), errorString);

    exit(1);
}

void
SGCanvas::notEnoughTextureUnits(const int numTextures)
{
    QString errorString(tr("Your OpenGL Graphics Card Only Supports %1 Texture Units, Some "
                           "ShaderGen Features May Not Work As Expected!")
                          .arg(numTextures));
    QMessageBox::critical(this, tr("Insufficient Texture Units"), errorString);
}
