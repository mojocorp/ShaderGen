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

#include "SGShaderTextWindow.h"
#include "SGFrame.h"
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/file.h>
#include "SGCanvas.h"

const int FIXED_TYPE_POINT_SIZE = 8;

BEGIN_EVENT_TABLE(SGShaderTextWindow, wxPanel)
    EVT_COMMAND(Id::GenerateShaders, wxEVT_COMMAND_BUTTON_CLICKED, SGShaderTextWindow::Refresh)
    EVT_COMMAND(Id::Link, wxEVT_COMMAND_BUTTON_CLICKED, SGShaderTextWindow::OnButtonLink)
    EVT_COMMAND(Id::Compile, wxEVT_COMMAND_BUTTON_CLICKED, SGShaderTextWindow::OnButtonCompile)
    EVT_COMMAND(Id::Build, wxEVT_COMMAND_BUTTON_CLICKED, SGShaderTextWindow::OnButtonBuild)
    EVT_COMMAND(Id::ClearLog, wxEVT_COMMAND_BUTTON_CLICKED, SGShaderTextWindow::OnButtonClearLog)
END_EVENT_TABLE()

SGShaderTextWindow::SGShaderTextWindow(SGFrame *frame):wxPanel(frame, -1)
{
    m_frame = frame;

    notebook = new wxNotebook(this, wxID_ANY);

    textBoxVert = new wxTextCtrl(notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(450,400), wxTE_MULTILINE | wxTE_RICH );
    textBoxFrag = new wxTextCtrl(notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(450,400), wxTE_MULTILINE | wxTE_RICH );
    textBoxInfo = new wxTextCtrl(notebook, wxID_ANY, "", wxDefaultPosition, wxSize(450,400), wxTE_MULTILINE | wxTE_RICH | wxTE_READONLY );

    fixedFont = wxFont(FIXED_TYPE_POINT_SIZE, wxTELETYPE, wxNORMAL, wxNORMAL, false, wxT(""), wxFONTENCODING_SYSTEM );

    textBoxVert->SetFont(fixedFont);
    textBoxFrag->SetFont(fixedFont);
    textBoxInfo->SetFont(fixedFont);

    notebook->AddPage( textBoxVert, wxT("Vertex Shader"), TRUE);
    notebook->AddPage( textBoxFrag, wxT("Fragment Shader"), FALSE);
    notebook->AddPage( textBoxInfo, wxT("InfoLog"), FALSE);

    haveRefreshed = haveCompiled = false;

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);

    button_sizer->Add(new wxButton(this, Id::GenerateShaders, wxT("1. GENERATE SHADERS")),0 , wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);
    button_sizer->Add(new wxButton(this, Id::Compile, wxT("2. COMPILE")),0 , wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);
    button_sizer->Add(new wxButton(this, Id::Link, wxT("3. LINK")), 0, wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);
    button_sizer->Add(new wxButton(this, Id::Build, wxT("BUILD")),0 ,wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);
    button_sizer->Add(new wxButton(this, Id::ClearLog, wxT("CLEAR INFO LOG")),0 , wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);

    topSizer->Add(notebook, 1, wxEXPAND | wxALL | wxADJUST_MINSIZE, TEN_PIXEL_BORDER);
    topSizer->Add(button_sizer, 0, wxALIGN_CENTER | wxADJUST_MINSIZE);

    SetAutoLayout(TRUE);
    SetSizer(topSizer);
    topSizer->SetSizeHints(this);
    Show(TRUE);
    topSizer->FitInside(frame);
}

void SGShaderTextWindow::OnButtonCompile(wxCommandEvent &event)
{
    if(haveRefreshed)
    {
        haveCompiled = m_frame->GetCanvas()->CompileShaders(GetVertexShaderBox()->GetValue(),GetFragmentShaderBox()->GetValue());
    }
    notebook->SetSelection(2);
}

void SGShaderTextWindow::OnButtonLink(wxCommandEvent &event)
{
    if(haveCompiled)
    {
        haveLinked = m_frame->GetCanvas()->LinkShaders(GetVertexShaderBox()->GetValue(),GetFragmentShaderBox()->GetValue());
        notebook->SetSelection(2);
        if(haveLinked)
        {
            m_frame->SetCanvasMode(1);
        }

        bool bottomReached = false;

        while(bottomReached)
        {
            bottomReached = textBoxInfo->ScrollLines(2);
        }
    }
}

void SGShaderTextWindow::OnButtonBuild(wxCommandEvent &event)
{
    Refresh(event);
    haveCompiled = m_frame->GetCanvas()->CompileShaders(GetVertexShaderBox()->GetValue(),GetFragmentShaderBox()->GetValue());
    notebook->SetSelection(2);
    if(haveCompiled)
    {
        haveLinked = m_frame->GetCanvas()->LinkShaders(GetVertexShaderBox()->GetValue(),GetFragmentShaderBox()->GetValue());
        if(haveLinked)
        {
            m_frame->SetCanvasMode(1);
        }

        bool bottomReached = false;

        while(bottomReached)
        {
            bottomReached = textBoxInfo->ScrollLines(2);
        }
    }
}

void SGShaderTextWindow::Refresh(wxCommandEvent &event)
{
    Refresh();
}

void SGShaderTextWindow::Refresh()
{
    GetFragmentShaderBox()->SetValue(m_frame->GetFragmentShader());
    GetVertexShaderBox()->SetValue(m_frame->GetVertexShader());

    haveRefreshed = true;
}

void SGShaderTextWindow::OnButtonClearLog(wxCommandEvent &event)
{
    textBoxInfo->Clear();
}
