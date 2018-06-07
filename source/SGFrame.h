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

#include <QMainWindow>

#include "SGCanvas.h"
#include "SGShaderGenerator.h"

class SGOglNotebook;
class SGShaderTextWindow;
class SGCanvas;
class SGFixedGLState;
class SGTextures;
class QSplitter;
class QButtonGroup;

#define PrintOpenGLError() SGFrame::printOglError(__FILE__, __LINE__)

class SGFrame : public QMainWindow
{
    Q_OBJECT
  public:
    SGFrame(const QString& title);
    ~SGFrame();

    SGFixedGLState* getGLState() { return m_glState; }
    SGTextures* getTextures() { return m_textures; }

    SGShaderTextWindow* getShaderTextWindow() { return m_shaderText; }
    SGCanvas* getCanvas() { return m_canvas; }

    void setCanvasMode(SGCanvas::GLMode a);
    void setStatusText(const QString& text);
    bool isPerspective() const;

    QString getVertexShader() const { return m_shaderGen->buildVertexShader(); }
    QString getFragmentShader() const { return m_shaderGen->buildFragmentShader(); }

    /// Returns 1 if an OpenGL error occurred, 0 otherwise.
    static int printOglError(const char* file, int line);

    bool loadFile(const QString& filename);
    bool saveFile(const QString& filename) const;

    void readSettings();

  protected:
    virtual void closeEvent(QCloseEvent* event);
  private slots:
    void onGLModeChanged(int);
    void modelActionTriggered(QAction* action);
    void viewActionTriggered();
    void switchGLModeTriggered();
    void setFixedGLMode();
    void help();
    void about();
    bool open();
    bool saveAs();

  private:
    void createActions();
    void createMenus();
    void createStatusBar();

    QMenu* m_fileMenu;
    QMenu* m_viewMenu;
    QMenu* m_modelMenu;
    QMenu* m_helpMenu;

    QAction* m_openAct;
    QAction* m_saveAsAct;
    QAction* m_exitAct;
    QAction* m_perspAct;
    QAction* m_switchGLModeAct;
    QAction* m_torusAct;
    QAction* m_sphereAct;
    QAction* m_trefoilAct;
    QAction* m_kleinAct;
    QAction* m_conicAct;
    QAction* m_planeAct;
    QAction* m_aboutAct;
    QAction* m_helpAct;

    SGFixedGLState* m_glState;
    SGTextures* m_textures;
    SGOglNotebook* m_oglNotebook;
    SGCanvas* m_canvas;
    QButtonGroup* m_glModeChoice;
    SGShaderTextWindow* m_shaderText;
    SGShaderGenerator* m_shaderGen;
    QSplitter* m_topSizer;
    QSplitter* m_horizSizer;
};
