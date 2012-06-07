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

#include "App.h"
#include "SGCanvasWrapper.h"
#include "SGCanvas.h"
#include "SGFrame.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"

BEGIN_EVENT_TABLE(SGCanvasWrapper, wxPanel)
    EVT_COMMAND_RANGE(Id::GLModeChoiceVal, Id::GLModeChoiceVal, wxEVT_COMMAND_RADIOBOX_SELECTED, SGCanvasWrapper::OnRadio)
    EVT_KEY_DOWN(SGCanvasWrapper::OnKey)
END_EVENT_TABLE()

SGCanvasWrapper::SGCanvasWrapper(SGFrame *parent, wxWindowID id,const wxPoint& pos, const wxSize& size)
:wxPanel(parent, id, pos, size)
{
    m_parent = parent;
    mode = Id::GLModeChoiceFixed;

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

    canvas      = new SGCanvas(parent, this, -1, wxDefaultPosition, wxSize(size.x,size.y- 50) );

    wxString glModeEnumArray[3];
    glModeEnumArray[0] = wxT("FIXED FUNCTIONALITY MODE");
    glModeEnumArray[1] = wxT("EQUIVALENT SHADER MODE ");
    glModeChoice = new wxRadioBox(this, Id::GLModeChoiceVal, wxT("Select GL Mode"), wxDefaultPosition, wxDefaultSize, 2, glModeEnumArray, 1, wxRA_SPECIFY_ROWS, wxDefaultValidator);


    topSizer->Add(glModeChoice,         0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 10);
    topSizer->Add(canvas,         1, wxEXPAND | wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 10);


    SetSizer( topSizer );
    topSizer->SetSizeHints( this );
    this->Show(TRUE);
}

void SGCanvasWrapper::OnRadio(wxCommandEvent& event)
{
    switch(event.GetId()){

        case Id::GLModeChoiceVal:
            if(glModeChoice->GetSelection()== 0)
            {
                mode = Id::GLModeChoiceFixed;
            }
            else
            {
                canvas->SwitchToShaderMode();
                mode = Id::GLModeChoiceShader;
            }
            break;

        default:
            break;
        }
    canvas->Update();
    event.Skip();
}

SGFixedGLState* SGCanvasWrapper::GetGLState() 
{
    return m_parent->GetGLState();
}

void SGCanvasWrapper::SwitchMode()
{
    if(mode == Id::GLModeChoiceFixed)
    {
        glModeChoice->SetSelection(1);
        canvas->SwitchToShaderMode();
        mode = Id::GLModeChoiceShader;
    }
    else if(mode == Id::GLModeChoiceShader)
    {
        glModeChoice->SetSelection(0);
        mode = Id::GLModeChoiceFixed;
    }
    canvas->Update();
}

void SGCanvasWrapper::SetMode(int a)
{
    if(glModeChoice->GetSelection() == a)
    {
        return;
    }
    SwitchMode();
}

void SGCanvasWrapper::OnKey(wxKeyEvent& event)
{
    SwitchMode();
    canvas->Update();
}
