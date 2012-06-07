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

#include "SGOglFogNBPage.h"
#include "SGOglNotebook.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"
#include "App.h"
#include "SGFrame.h"
#include "SGCanvas.h"
#include "Vector.h"
#include <wx/colordlg.h>

BEGIN_EVENT_TABLE(SGOglFogNBPage, wxPanel)
    EVT_COMMAND_RANGE(Id::FFSD_START_FOG, Id::FFSD_END_FOG, wxEVT_COMMAND_RADIOBOX_SELECTED, SGOglFogNBPage::OnRadio)
    EVT_COMMAND_RANGE(Id::FFSD_START_FOG, Id::FFSD_END_FOG, wxEVT_COMMAND_TEXT_ENTER,        SGOglFogNBPage::OnTextEnter)
    EVT_COMMAND_RANGE(Id::FFSD_START_FOG, Id::FFSD_END_FOG, wxEVT_COMMAND_BUTTON_CLICKED ,   SGOglFogNBPage::OnButton)
    EVT_COMMAND_RANGE(Id::FFSD_START_FOG, Id::FFSD_END_FOG, wxEVT_COMMAND_CHECKBOX_CLICKED,  SGOglFogNBPage::OnCheckbox)
END_EVENT_TABLE()


SGOglFogNBPage::SGOglFogNBPage(SGOglNotebook* parent, wxWindowID id)
:wxPanel(parent,id)
{
    m_parent = parent;
    SGFixedGLState* glState = m_parent->GetGLState();
    Fog *fog = glState->GetFog();

    wxStaticBox* fogBox            = new wxStaticBox(this, -1, wxT("Fog Parameters"),  wxDefaultPosition, wxDefaultSize, 0);
    wxStaticBoxSizer* fogSizer     = new wxStaticBoxSizer(fogBox, wxHORIZONTAL);
    wxBoxSizer* v1       = new wxBoxSizer(wxVERTICAL  );
    wxBoxSizer* v2       = new wxBoxSizer(wxVERTICAL  );
    wxBoxSizer* v3       = new wxBoxSizer(wxVERTICAL  );
    wxBoxSizer* v4       = new wxBoxSizer(wxVERTICAL  );
    wxBoxSizer* v5       = new wxBoxSizer(wxVERTICAL  );

    wxString fogEnumArray[3];
    fogEnumArray[0] = wxT("GL_LINEAR");
    fogEnumArray[1] = wxT("GL_EXP");
    fogEnumArray[2] = wxT("GL_EXP2");
    fogModeChoice = new wxRadioBox(this, Id::FogModeChoiceVal, wxT("Select Fog Mode"), wxDefaultPosition, wxDefaultSize, 3, fogEnumArray, 1, wxRA_SPECIFY_COLS, wxDefaultValidator);
    GLenum aa = glState->GetFog()->fogMode;
    
    if( aa == GL_LINEAR )
    {
        aa = 0;
    }
    else if( aa == GL_EXP )
    {
        aa = 1;
    }
    else if ( aa == GL_EXP2 )
    {
        aa = 2;
    }
    else
    {
        aa = 1;
    }
    
    fogModeChoice->SetSelection(aa);

    fogDensity = new wxTextCtrl(this, Id::FogDensityVal, FloatToString1(fog->fogDensity),wxDefaultPosition,  wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("GL_FOG_DENSITY"));
    fogStart   = new wxTextCtrl(this, Id::FogStartVal,   FloatToString1(fog->fogStart),  wxDefaultPosition,  wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("GL_FOG_START"));
    fogEnd     = new wxTextCtrl(this, Id::FogEndVal,     FloatToString1(fog->fogEnd),    wxDefaultPosition,  wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("GL_FOG_END"));
    fogColor   = new wxButton  (this, Id::FogColorVal,  "",  wxDefaultPosition,  wxDefaultSize, 0, wxDefaultValidator, wxT("GL_FOG_COLOR"));
    fogColor->SetBackgroundColour(ToWxIntColor(fog->fogColorVector));

    wxStaticText* fogDensityLbl      = new wxStaticText(this, 0, wxT("GL_FOG_DENSITY"),wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* fogStartLbl        = new wxStaticText(this, 0, wxT("GL_FOG_START"),  wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* fogEndLbl          = new wxStaticText(this, 0, wxT("GL_FOG_END"),    wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* fogColorLbl        = new wxStaticText(this, 0, wxT("GL_FOG_COLOR"),  wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    fogCheckBox              = new wxCheckBox  (this, Id::FogEnableVal, wxT("GL_FOG_ENABLE"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    fogCheckBox->SetValue(glState->GetFogEnable());

    v1->Add(fogCheckBox,         0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);
    v1->Add(fogModeChoice);

    v2->Add(fogDensityLbl,       0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);
    v2->Add(fogStartLbl,         0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);
    v2->Add(fogEndLbl,           0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);

    v3->Add(fogDensity,          0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);
    v3->Add(fogStart,            0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);
    v3->Add(fogEnd,              0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);

    v4->Add(fogColorLbl,         0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);

    v5->Add(fogColor,            0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);

    fogSizer->Add(v1, 0 , wxALL |wxADJUST_MINSIZE, 10);
    fogSizer->Add(v2, 0 , wxALL |wxADJUST_MINSIZE, 10);
    fogSizer->Add(v3, 0 , wxALL |wxADJUST_MINSIZE, 10);
    fogSizer->Add(v4, 0 , wxALL |wxADJUST_MINSIZE, 10);
    fogSizer->Add(v5, 0 , wxALL |wxADJUST_MINSIZE, 10);

    SetAutoLayout(TRUE);
    SetSizer( fogSizer );
    fogSizer->SetSizeHints( this );\
}

void SGOglFogNBPage::OnButton(wxCommandEvent& event)
{
    wxColour oglColor;
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetFogChanged(true);


    oglColor = wxGetColourFromUser(this, ToWxIntColor( glState->GetFog()->fogColorVector ));
    glState->GetFog()->fogColorVector = ToGLFPColor(oglColor);
    fogColor->SetBackgroundColour(oglColor);

    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
    event.Skip();
}

void SGOglFogNBPage::OnCheckbox(wxCommandEvent& event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetFogChanged(true);

    glState->SetFogEnable(fogCheckBox->IsChecked());

    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglFogNBPage::OnTextEnter(wxCommandEvent& event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetFogChanged(true);
    switch(event.GetId())
    {
        case Id::FogDensityVal:
            if((fogDensity->GetValue()).IsEmpty())
            {
                IncorrectFormat("one floating point value.", *this);
                break;
            }
            glState->GetFog()->fogDensity = atof((fogDensity->GetValue()).c_str());
            break;
        case Id::FogStartVal:
            if((fogStart->GetValue()).IsEmpty())
            {
                IncorrectFormat("one floating point value.", *this);
                break;
            }
            glState->GetFog()->fogStart   = atof((fogStart->GetValue()).c_str());
            break;
        case Id::FogEndVal:
            if((fogEnd->GetValue()).IsEmpty())
            {
                IncorrectFormat("one floating point value.", *this);
                break;
            }
            glState->GetFog()->fogEnd     = atof((fogEnd->GetValue()).c_str());
            break;
        default:
            break;
        }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglFogNBPage::OnRadio(wxCommandEvent& event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetFogChanged(true);
            switch(fogModeChoice->GetSelection()){
                case 0:
                    glState->GetFog()->fogMode = GL_LINEAR;
                    break;
                case 1:
                    glState->GetFog()->fogMode = GL_EXP;
                    break;
                case 2:
                    glState->GetFog()->fogMode = GL_EXP2;
                    break;
                default:
                    break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}