/************************************************************************
* Author(s):    Inderaj Bains and Joshua Doss                           *
* Last Modified: 11/04/2005                                             *
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

#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QSplitter>
#include <QActionGroup>
#include <QBoxLayout>
#include <QSettings>

#include "AboutDialog.h"
#include "SGFrame.h"
#include "SGCanvasWrapper.h"
#include "SGCanvas.h"
#include "SGShaderTextWindow.h"

SGFrame *SGFrame::instance = 0;

SGFrame::SGFrame(const QString& title)
    : QMainWindow()
{
    instance = this;

    setWindowTitle(title);

    models = new SGModels();
    shaderGen = new SGShaderGenerator();

    createActions();
    createMenus();
    createStatusBar();

    oglNotebook = new SGOglNotebook(this);
    oglNotebook->resize(800, 300);
    canvas      = new SGCanvasWrapper(this);
    canvas->resize(400, 350);
    shaderText  = new SGShaderTextWindow(this);
    shaderText->resize(450, 400);

    topSizer = new QSplitter(Qt::Vertical);
    horizSizer = new QSplitter(Qt::Horizontal);

    horizSizer->addWidget(canvas);
    horizSizer->addWidget(shaderText);

    topSizer->addWidget(horizSizer);
    topSizer->addWidget(oglNotebook);

    setCentralWidget(topSizer);
}

SGFrame::~SGFrame()
{
    if(models != NULL)
    {
        delete models;
        models = NULL;
    }
    if(shaderGen != NULL)
    {
        delete shaderGen;
        shaderGen = NULL;
    }
} 

void SGFrame::createActions()
{
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    perspAct = new QAction(tr("Perspective"), this);
    perspAct->setCheckable(true);
    perspAct->setChecked(true);
    connect(perspAct, SIGNAL(triggered()), SLOT(viewActionTriggered()));

    torusAct = new QAction(tr("Torus"), this);
    torusAct->setCheckable(true);
    torusAct->setChecked(true);

    sphereAct = new QAction(tr("Sphere"), this);
    sphereAct->setCheckable(true);

    trefoilAct = new QAction(tr("Trefoil"), this);
    trefoilAct->setCheckable(true);

    kleinAct = new QAction(tr("Klein"), this);
    kleinAct->setCheckable(true);

    conicAct = new QAction(tr("Conic"), this);
    conicAct->setCheckable(true);

    planeAct = new QAction(tr("Plane"), this);
    planeAct->setCheckable(true);

    QActionGroup *modelGroup = new QActionGroup(this);
    modelGroup->addAction(torusAct);
    modelGroup->addAction(sphereAct);
    modelGroup->addAction(trefoilAct);
    modelGroup->addAction(kleinAct);
    modelGroup->addAction(conicAct);
    modelGroup->addAction(planeAct);
    connect(modelGroup, SIGNAL(triggered(QAction*)), SLOT(modelActionTriggered(QAction*)));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    helpAct = new QAction(tr("Help"), this);
    helpAct->setStatusTip(tr("Show the help"));
    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));
}

void SGFrame::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(perspAct);

    modelMenu = menuBar()->addMenu(tr("&Model"));
    modelMenu->addAction(torusAct);
    modelMenu->addAction(sphereAct);
    modelMenu->addAction(trefoilAct);
    modelMenu->addAction(kleinAct);
    modelMenu->addAction(conicAct);
    modelMenu->addAction(planeAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(helpAct);
}

void SGFrame::createStatusBar()
{
    statusBar()->showMessage(windowTitle());
}

void SGFrame::SetCanvasMode(SGCanvasWrapper::GLMode a)
{
    GetCanvasWrapper()->SetMode(a);
    GetCanvas()->updateGL();
}

void SGFrame::SetStatusText(const QString &text)
{
    statusBar()->showMessage(text);
}

bool SGFrame::isPerspective() const
{
    return perspAct->isChecked();
}

void SGFrame::Errorf(const char* format, ...)
{
    QString message;
    va_list marker;
    va_start(marker, format);
    message.vsprintf(format, marker);
    statusBar()->showMessage(message);
}

void SGFrame::readSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("app/geometry").toByteArray());
    restoreState(settings.value("app/windowState").toByteArray());
    topSizer->restoreState(settings.value("app/topSizer").toByteArray());
    horizSizer->restoreState(settings.value("app/horizSizer").toByteArray());
}

void SGFrame::closeEvent(QCloseEvent * event)
{
    QSettings settings;
    settings.setValue("app/geometry", saveGeometry());
    settings.setValue("app/windowState", saveState());
    settings.setValue("app/topSizer", topSizer->saveState());
    settings.setValue("app/horizSizer", horizSizer->saveState());

    QMainWindow::closeEvent(event);
}

void SGFrame::modelActionTriggered(QAction *action)
{
    if (action == torusAct) {
        GetCanvas()->SetModel(SGModels::ModelTorus);
    } else if (action == sphereAct) {
        GetCanvas()->SetModel(SGModels::ModelSphere);
    } else if (action == trefoilAct) {
        GetCanvas()->SetModel(SGModels::ModelTrefoil);
    } else if (action == kleinAct) {
        GetCanvas()->SetModel(SGModels::ModelKlein);
    } else if (action == conicAct) {
        GetCanvas()->SetModel(SGModels::ModelConic);
    } else if (action == planeAct) {
        GetCanvas()->SetModel(SGModels::ModelPlane);
    }

    GetCanvas()->updateGL();
}

void SGFrame::viewActionTriggered()
{
    GetCanvas()->updateGL();
}

void SGFrame::help()
{
    QDialog help(this);
    help.setWindowTitle(tr("Help - ShaderGen"));
    QVBoxLayout *layout = new QVBoxLayout(&help);
    QTextEdit *txtEdit = new QTextEdit();
    txtEdit->setReadOnly(true);
    txtEdit->setPlainText(tr("\n"
                             "GLSL ShaderGen\n\n"
                             "INTENDED PURPOSE:\n\n"
                             "The purpose of ShaderGen is to show the user how to emulate fixed functionality\n"
                             "by using the OpenGL Shading Language.\n\n"
                             "ShaderGen is intended to be educational, with the focus being on clarity of\n"
                             "generated code rather than efficiency.\n"
                             " \n"
                             " \n"
                             "USAGE:\n\n"
                             "The tool has three major parts:\n"
                             "* OpenGL Window : Displays rendered output.\n"
                             "* OpenGL State : Configure the OpenGL State using the tabs.\n"
                             "* Shader Text Windows : Use the tabs to look through the generated shaders\n\n"
                             "Operation:\n\n"
                             "1. Using the tabs, setup the OpenGL state.  You will see your results updated"
                             " in the OpenGL Window as you make state changes.  If you make a change requiring text"
                             " entry, be sure to press the enter key to update the current state.\n"
                             "2. Click the \"GENERATE SHADERS\" button on the Shader Windows panel to generate Vertex and Fragment Shaders\n"
                             "3. Click \"COMPILE\" to compile the shaders, then check the \"Infolog\" text window for compilation results\n"
                             "4. Click \"LINK\" to link the compilers, then see the \"Infolog\" window for results\n"
                             "5. To switch the OpenGL Window to display the shaded model, click the \"Shader"
                             " Equivalent\" mode radio button.\n"
                             "6. Click \"BUILD\" to do steps 2 through 5 together.\n\n\n"
                             "Note: The text boxes only accept values when enter key is pressed\n"
                             "Note: You can edit the shaders in the text windows, and then press \"COMPILE\" and then \"LINK\" to"
                             " to see the results of your edited shaders.\n"
                             "Note: The eye coordinate distance between the model center and the viewpoint is 6.0.  The model is"
                             " drawn with it's center at (0.0,0.0,6.0) in eye-coordinates and diagonals at (-1.0, -1.0, 5.0)"
                             " and (1.0, 1.0, 7.0).\n"
                             "                 "));

    layout->addWidget(txtEdit);
    help.resize(600, 600);
    help.exec();
}

void SGFrame::about()
{
    AboutDialog about(this);
    about.exec();
}
