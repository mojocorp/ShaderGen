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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "SGShaderTextWindow.h"
#include "SGFrame.h"
#include "SGCanvas.h"
#include "QCodeEditor.h"

SGShaderTextWindow::SGShaderTextWindow(SGFrame *frame):QFrame(frame)
{
    m_frame = frame;

    notebook = new QTabWidget(this);

    textBoxVert = new QCodeEditor(notebook);
    textBoxFrag = new QCodeEditor(notebook);
    textBoxInfo = new QTextEdit(notebook);

    QFont fixedFont("Monospace");
    fixedFont.setStyleHint(QFont::TypeWriter);

    textBoxVert->setFont(fixedFont);
    textBoxFrag->setFont(fixedFont);
    textBoxInfo->setFont(fixedFont);

    notebook->addTab( textBoxVert, "Vertex Shader");
    notebook->addTab( textBoxFrag, "Fragment Shader");
    notebook->addTab( textBoxInfo, "InfoLog");

    haveRefreshed = haveCompiled = false;

    QVBoxLayout *topSizer = new QVBoxLayout();
    QHBoxLayout *button_sizer = new QHBoxLayout();

    QPushButton *pb;
    pb = new QPushButton("1. GENERATE SHADERS", this);
    connect(pb, SIGNAL(clicked()), SLOT(refresh()));
    button_sizer->addWidget(pb);

    pb = new QPushButton("2. COMPILE", this);
    connect(pb, SIGNAL(clicked()), SLOT(compile()));
    button_sizer->addWidget(pb);

    pb = new QPushButton("3. LINK", this);
    connect(pb, SIGNAL(clicked()), SLOT(link()));
    button_sizer->addWidget(pb);

    pb = new QPushButton("BUILD", this);
    connect(pb, SIGNAL(clicked()), SLOT(build()));
    button_sizer->addWidget(pb);

    pb = new QPushButton("CLEAR INFO LOG", this);
    connect(pb, SIGNAL(clicked()), SLOT(clearLog()));
    button_sizer->addWidget(pb);

    topSizer->addWidget(notebook);
    topSizer->addLayout(button_sizer);

    setLayout(topSizer);
}

void SGShaderTextWindow::compile()
{
    if(haveRefreshed)
    {
        haveCompiled = m_frame->GetCanvas()->CompileShaders(GetVertexShaderBox()->toPlainText(),GetFragmentShaderBox()->toPlainText());
    }
    notebook->setCurrentWidget(textBoxInfo);
}

void SGShaderTextWindow::link()
{
    if(haveCompiled)
    {
        haveLinked = m_frame->GetCanvas()->LinkShaders(GetVertexShaderBox()->toPlainText(),GetFragmentShaderBox()->toPlainText());
        notebook->setCurrentWidget(textBoxInfo);
        if(haveLinked)
        {
            m_frame->SetCanvasMode(SGCanvasWrapper::GLModeChoiceShader);
        }

        textBoxInfo->textCursor().movePosition(QTextCursor::End);
    }
}

void SGShaderTextWindow::build()
{
    refresh();
    haveCompiled = m_frame->GetCanvas()->CompileShaders(GetVertexShaderBox()->toPlainText(),GetFragmentShaderBox()->toPlainText());
    notebook->setCurrentWidget(textBoxInfo);
    if(haveCompiled)
    {
        haveLinked = m_frame->GetCanvas()->LinkShaders(GetVertexShaderBox()->toPlainText(),GetFragmentShaderBox()->toPlainText());
        if(haveLinked)
        {
            m_frame->SetCanvasMode(SGCanvasWrapper::GLModeChoiceShader);
        }

        textBoxInfo->textCursor().movePosition(QTextCursor::End);
    }
}

void SGShaderTextWindow::refresh()
{
    textBoxFrag->setPlainText(m_frame->GetFragmentShader());
    textBoxVert->setPlainText(m_frame->GetVertexShader());

    haveRefreshed = true;
}

void SGShaderTextWindow::clearLog()
{
    textBoxInfo->clear();
}

