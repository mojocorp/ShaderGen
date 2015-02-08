/************************************************************************
* Author(s):    Inderaj Bains and Joshua Doss                           *
* Last Modified: 10/31/2005                                             *
************************************************************************/
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

#include <GL/glew.h>

#include <QGLWidget>

#include "SGCanvasMouseHandler.h"
#include "SGCanvasWrapper.h"
#include "SGModels.h"

class SGFrame;

class SGCanvas : public QGLWidget
{
public:
    SGCanvas(SGFrame* frame, SGCanvasWrapper* parent);
    ~SGCanvas();

    SGFixedGLState* GetGLState() { return m_parent->GetGLState(); }
    SGFrame* GetFrame(){ return m_frame;}

    bool LinkShaders(const QString & vertexShader, const QString & fragmentShader);
    bool CompileShaders(const QString & vertexShader, const QString & fragmentShader);

    int GetMode() { return m_parent->GetMode();}
    int SwitchToShaderMode();

    static const float CameraZ;
    float GetZoom() { return m_zoom; }

    QVector3D GetWorldSpace(int x, int y);

    void initializeGL();
    void paintGL();
    void SetZoom(float zoom) { m_zoom = zoom;}
    void GLSetup();
    void SetModel(SGModels::ModelId id) { modelCurrent = id;}
    void PrintInfoLog(GLuint obj);

    GLuint logo;
protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
private:
    SGCanvasWrapper *m_parent;
    SGModels *models;
    SGCanvasMouseHandler mouse;
    SGModels::ModelId modelCurrent;
    SGFrame* m_frame;
    int m_width, m_height;
    float m_left, m_right, m_bottom, m_top, m_znear, m_zfar, m_zoom;

    GLint GetUniLoc(unsigned int program, const GLchar *name);

    bool glReady, glCompiled, glLinked;
    unsigned int vertS, fragS, prog;

    void SetupFromFixedState();
    void WriteMessage(const QString str);
    void DrawLogo() const;
    void UnsupportedOpenGLVersion(void);
    void GetGlVersion(int *major, int *minor);
    void CheckGlImplementation();
    void NotEnoughTextureUnits(const int numTextures);
};
