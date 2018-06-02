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

#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include "SGCanvasMouseHandler.h"
#include "SGModels.h"

class SGFrame;
class SGFixedGLState;

class SGCanvas : public QOpenGLWidget
{
  public:
    SGCanvas(SGFrame* frame);
    ~SGCanvas();

    SGFrame* getFrame() { return m_frame; }

    bool linkShaders(const QString& vertexShader, const QString& fragmentShader);
    bool compileShaders(const QString& vertexShader, const QString& fragmentShader);

    // Mode for GL, Fixed or Shader
    enum GLMode
    {
        GLModeChoiceFixed,
        GLModeChoiceShader
    };
    void setMode(GLMode a);
    GLMode getMode() const { return m_mode; }

    const float CameraZ;

    QVector3D getWorldSpace(int x, int y);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void setModel(SGModels::ModelId id) { m_modelCurrent = id; }
    QVector2D getNormalizedPosition(const QPoint& pos) const;

  protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

  private:
    SGFixedGLState* getGLState();

    GLMode m_mode;
    SGModels* m_models;
    SGCanvasMouseHandler m_mouse;
    SGModels::ModelId m_modelCurrent;
    SGFrame* m_frame;

    QMatrix4x4 m_modelView;
    QMatrix4x4 m_projection;

    bool m_glCompiled;
    QOpenGLShaderProgram m_prog;

    void setupFromFixedState();
    void writeMessage(const QString str);
};
