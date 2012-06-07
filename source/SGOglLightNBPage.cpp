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

#include "SGOglLightNBPage.h"
#include "SGOglNotebook.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"
#include "App.h"
#include "SGFrame.h"
#include "SGCanvas.h"
#include <wx/colordlg.h>
#include <wx/msgdlg.h>

BEGIN_EVENT_TABLE(SGOglLightNBPage, wxPanel)
    EVT_COMMAND_RANGE(Id::FFSD_START_LIGHT, Id::FFSD_END_LIGHT, wxEVT_COMMAND_RADIOBOX_SELECTED, SGOglLightNBPage::OnRadio)
    EVT_COMMAND_RANGE(Id::FFSD_START_LIGHT, Id::FFSD_END_LIGHT, wxEVT_COMMAND_TEXT_ENTER,        SGOglLightNBPage::OnTextEnter)
    EVT_COMMAND_RANGE(Id::FFSD_START_LIGHT, Id::FFSD_END_LIGHT, wxEVT_COMMAND_BUTTON_CLICKED,    SGOglLightNBPage::OnButton)
    EVT_COMMAND_RANGE(Id::FFSD_START_LIGHT, Id::FFSD_END_LIGHT, wxEVT_COMMAND_CHECKBOX_CLICKED,  SGOglLightNBPage::OnCheckbox)
END_EVENT_TABLE()

SGOglLightNBPage::SGOglLightNBPage(SGOglNotebook* parent, wxWindowID id)
: wxPanel(parent,id)
{
    m_parent = parent;
    SGFixedGLState* glState = m_parent->GetGLState();

    wxStaticBox* lightBox             = new wxStaticBox(this, wxID_ANY, wxT("Lighting Parameters"),  wxDefaultPosition, wxDefaultSize, 0);
    wxStaticBox* enableDisableBox     = new wxStaticBox(this, wxID_ANY, wxT("glEnable/glDisable"),   wxDefaultPosition, wxDefaultSize, 0);
    wxStaticBox* selectedLightBox     = new wxStaticBox(this, wxID_ANY, wxT("Selected Light Properties"),   wxDefaultPosition, wxDefaultSize, 10);
    wxStaticBoxSizer* lightSizer      = new wxStaticBoxSizer(lightBox, wxVERTICAL);
    wxStaticBoxSizer* h1              = new wxStaticBoxSizer(enableDisableBox, wxHORIZONTAL);
    wxBoxSizer* h3                    = new wxStaticBoxSizer(selectedLightBox, wxHORIZONTAL);
    wxBoxSizer* h2             = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* h3v1           = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* h3v2           = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* h3v3           = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* h3v4           = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* labelh3v1      = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* labelh3v2      = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* labelh3v3      = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* labelh3v4      = new wxBoxSizer(wxVERTICAL);

    wxString LightSelectionArray[8];
    LightSelectionArray[0]  = wxT("L0  ");
    LightSelectionArray[1]  = wxT("L1  ");
    LightSelectionArray[2]  = wxT("L2  ");
    LightSelectionArray[3]  = wxT("L3  ");
    LightSelectionArray[4]  = wxT("L4  ");
    LightSelectionArray[5]  = wxT("L5  ");
    LightSelectionArray[6]  = wxT("L6  ");
    LightSelectionArray[7]  = wxT("L7  ");

    lightSelectionBox    = new wxRadioBox(this, Id::LightSelectionBoxVal, wxT("Select Light"),    wxDefaultPosition, wxDefaultSize, 8, LightSelectionArray , 1, wxRA_SPECIFY_ROWS, wxDefaultValidator);

    lcb0 = new wxCheckBox(this, Id::L0EnableVal, wxT("L0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    lcb1 = new wxCheckBox(this, Id::L1EnableVal, wxT("L1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    lcb2 = new wxCheckBox(this, Id::L2EnableVal, wxT("L2"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    lcb3 = new wxCheckBox(this, Id::L3EnableVal, wxT("L3"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    lcb4 = new wxCheckBox(this, Id::L4EnableVal, wxT("L4"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    lcb5 = new wxCheckBox(this, Id::L5EnableVal, wxT("L5"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    lcb6 = new wxCheckBox(this, Id::L6EnableVal, wxT("L6"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    lcb7 = new wxCheckBox(this, Id::L7EnableVal, wxT("L7"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    lightingCheckBox  = new wxCheckBox(this, Id::LightingEnableVal,wxT("GL_LIGHTING"),  wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    normalizeCheckBox = new wxCheckBox(this, Id::NormalizeEnableVal,wxT("GL_NORMALIZE"),wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    separateSpecularColorCheckBox = new wxCheckBox(this, Id::SeparateSpecularColorEnableVal,wxT("GL_SEPARATE_SPECULAR_COLOR"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);

    lcb0->SetValue(glState->GetLight(0)->lightEnabled);
    lcb1->SetValue(glState->GetLight(1)->lightEnabled);
    lcb2->SetValue(glState->GetLight(2)->lightEnabled);
    lcb3->SetValue(glState->GetLight(3)->lightEnabled);
    lcb4->SetValue(glState->GetLight(4)->lightEnabled);
    lcb5->SetValue(glState->GetLight(5)->lightEnabled);
    lcb6->SetValue(glState->GetLight(6)->lightEnabled);
    lcb7->SetValue(glState->GetLight(7)->lightEnabled);

    lightingCheckBox->SetValue(glState->GetLightingEnable());
    normalizeCheckBox->SetValue(glState->GetNormalizeEnable());
    separateSpecularColorCheckBox->SetValue(glState->GetSeparateSpecularColorEnable());

    Light *light = glState->GetLight(lightSelectionBox->GetSelection());

    lightPosition = new wxTextCtrl(this, Id::LightPositionVal,  FloatToString4(light->lightPositionVector),      wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Light Position")       );

    spotDirection = new wxTextCtrl(this, Id::SpotDirectionVal, FloatToString3(light->lightSpotDirectionVector),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Spotlight Direction")  );
    spotExponent  = new wxTextCtrl(this, Id::SpotExponentVal,  FloatToString1(light->lightSpotExponent),   wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Spotlight Exponent")   );
    spotCutoff    = new wxTextCtrl(this, Id::SpotCutoffVal,    FloatToString1(light->lightSpotCutoff),     wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Spot Cutoff")          );

    constantAttenuation  = new wxTextCtrl(this, Id::AttenuationConstantVal,  FloatToString1(light->lightConstantAttenuation),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Constant Attenuation") );
    linearAttenuation    = new wxTextCtrl(this, Id::AttenuationLinearVal,    FloatToString1(light->lightLinearAttenuation),    wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Linear Attenuation")   );
    quadraticAttenuation = new wxTextCtrl(this, Id::AttenuationQuadraticVal, FloatToString1(light->lightQuadraticAttenuation), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Quadratic Attenuation"));

    ambientLight  = new wxButton(this, Id::AmbientLightVal,   wxT(""),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Ambient"));
    specularLight = new wxButton(this, Id::SpecularLightVal,  wxT(""),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Specular"));
    diffuseLight  = new wxButton(this, Id::DiffuseLightVal,   wxT(""),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Diffuse"));

    ambientLight->SetBackgroundColour(ToWxIntColor(light->lightAmbientColorVector));
    specularLight->SetBackgroundColour(ToWxIntColor(light->lightSpecularColorVector));
    diffuseLight->SetBackgroundColour(ToWxIntColor(light->lightDiffuseColorVector));

    wxStaticText* positionLbl = new wxStaticText(this, 0, wxT("GL_POSITION"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    wxStaticText* ambientLbl  = new wxStaticText(this, 0, wxT("GL_AMBIENT"),   wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* specularLbl = new wxStaticText(this, 0, wxT("GL_SPECULAR"),  wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* diffuseLbl  = new wxStaticText(this, 0, wxT("GL_DIFFUSE"),   wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    wxStaticText* spotDirectionLbl  = new wxStaticText(this, 0, wxT("GL_SPOT_DIRECTION"),wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* spotExponentLbl   = new wxStaticText(this, 0, wxT("GL_SPOT_EXPONENT"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* spotCutoffLbl     = new wxStaticText(this, 0, wxT("GL_SPOT_CUTOFF"),   wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    wxStaticText* constantAttenLbl  = new wxStaticText(this, 0, wxT("GL_CONSTANT_ATTENUATION"),   wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* linearAttenLbl    = new wxStaticText(this, 0, wxT("GL_LINEAR_ATTENUATION"),     wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* quadraticAttenLbl = new wxStaticText(this, 0, wxT("GL_QUADRATIC_ATTENUATION"),  wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);


    h1->Add(lcb0, 0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(lcb1, 0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(lcb2, 0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(lcb3, 0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(lcb4, 0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(lcb5, 0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(lcb6, 0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(lcb7, 0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(lightingCheckBox, 0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(normalizeCheckBox,0, wxALL | wxADJUST_MINSIZE, 5);
    h1->Add(separateSpecularColorCheckBox,0,wxALL | wxADJUST_MINSIZE, 5);

    h3v1->Add(lightPosition, 0, wxALIGN_TOP | wxALL | wxADJUST_MINSIZE, 5);

    labelh3v1->Add(positionLbl, 0, wxALIGN_TOP | wxALL | wxADJUST_MINSIZE, 5);

    h3v2->Add(ambientLight, 0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5 );
    h3v2->Add(specularLight, 0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);
    h3v2->Add(diffuseLight , 0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);

    labelh3v2->Add(ambientLbl , 0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);
    labelh3v2->Add(specularLbl, 0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);
    labelh3v2->Add(diffuseLbl , 0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);

    h3v3->Add(spotDirection, 0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);
    h3v3->Add(spotExponent , 0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5 );
    h3v3->Add(spotCutoff  ,  0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5 );

    labelh3v3->Add(spotDirectionLbl,  0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 9);
    labelh3v3->Add(spotExponentLbl ,  0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 9 );
    labelh3v3->Add(spotCutoffLbl  ,   0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 9 );

    h3v4->Add(constantAttenuation,    0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5 );
    h3v4->Add(linearAttenuation  ,    0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5 );
    h3v4->Add(quadraticAttenuation,   0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);

    labelh3v4->Add(constantAttenLbl,  0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 9 );
    labelh3v4->Add(linearAttenLbl  ,  0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 9 );
    labelh3v4->Add(quadraticAttenLbl, 0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 9);
    
    h2->Add(lightSelectionBox);

    h3->Add(labelh3v1, 0, wxTOP | wxADJUST_MINSIZE, 5);
    h3->Add(h3v1);
    h3->Add(labelh3v2, 0, wxTOP | wxADJUST_MINSIZE, 5);
    h3->Add(h3v2);
    h3->Add(labelh3v3, 0, wxTOP | wxADJUST_MINSIZE, 5);
    h3->Add(h3v3);
    h3->Add(labelh3v4, 0, wxTOP | wxADJUST_MINSIZE, 5);
    h3->Add(h3v4);

    lightSizer->Add(h1, 0 , wxALL | wxADJUST_MINSIZE, 10);
    lightSizer->Add(h2, 0 , wxALL | wxADJUST_MINSIZE, 10);
    lightSizer->Add(h3, 0 , wxALL | wxADJUST_MINSIZE, 10);

    SetAutoLayout(TRUE);
    SetSizer( lightSizer );
    lightSizer->SetSizeHints( this );
}

void SGOglLightNBPage::OnButton(wxCommandEvent& event)
{
    wxColour oglColor;
    SGFixedGLState* glState = m_parent->GetGLState();
    int lightSelected = lightSelectionBox->GetSelection();
    glState->SetLightChanged(true);

    switch(event.GetId()){
        case Id::AmbientLightVal :
            oglColor = wxGetColourFromUser(this, ToWxIntColor((glState->GetLight(lightSelected))->lightAmbientColorVector));
            glState->GetLight(lightSelected)->lightAmbientColorVector = ToGLFPColor(oglColor);
            break;
        case Id::DiffuseLightVal :
            oglColor = wxGetColourFromUser(this, ToWxIntColor((glState->GetLight(lightSelected))->lightDiffuseColorVector));
            glState->GetLight(lightSelected)->lightDiffuseColorVector = ToGLFPColor(oglColor);
            break;
        case Id::SpecularLightVal :
            oglColor = wxGetColourFromUser(this, ToWxIntColor((glState->GetLight(lightSelected))->lightSpecularColorVector));
            glState->GetLight(lightSelected)->lightSpecularColorVector = ToGLFPColor(oglColor);
            break;
        default :
            break;
    }
    FindWindowById(event.GetId())->SetBackgroundColour(oglColor);
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
    event.Skip();
}

void SGOglLightNBPage::OnCheckbox(wxCommandEvent& event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetLightChanged(true);
    switch(event.GetId()){
        case Id::L0EnableVal:
            glState->GetLight(0)->lightEnabled = lcb0->IsChecked();
            break;
        case Id::L1EnableVal:
            glState->GetLight(1)->lightEnabled = lcb1->IsChecked();
            break;
        case Id::L2EnableVal:
            glState->GetLight(2)->lightEnabled = lcb2->IsChecked();
            break;
        case Id::L3EnableVal:
            glState->GetLight(3)->lightEnabled = lcb3->IsChecked();
            break;
        case Id::L4EnableVal:
            glState->GetLight(4)->lightEnabled = lcb4->IsChecked();
            break;
        case Id::L5EnableVal:
            glState->GetLight(5)->lightEnabled = lcb5->IsChecked();
            break;
        case Id::L6EnableVal:
            glState->GetLight(6)->lightEnabled = lcb6->IsChecked();
            break;
        case Id::L7EnableVal:
            glState->GetLight(7)->lightEnabled = lcb7->IsChecked();
            break;
        case Id::LightingEnableVal:
            glState->SetLightingEnable(lightingCheckBox->IsChecked());
            break;
        case Id::NormalizeEnableVal:
            glState->SetNormalizeEnable(normalizeCheckBox->IsChecked());
            break;
        case Id::SeparateSpecularColorEnableVal:
            glState->SetSeparateSpecularColorEnable(separateSpecularColorCheckBox->IsChecked());
            break;
        default:
            break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglLightNBPage::OnTextEnter(wxCommandEvent& event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetLightChanged(true);
    wxArrayString userEnteredValues;
    vec3 tempLightSpotDirectionVector;
    vec4 tempLightPosVector;
    int lightSelected = lightSelectionBox->GetSelection();

    switch(event.GetId()){

        case Id::AttenuationConstantVal:
            if((constantAttenuation->GetValue()).IsEmpty())
            {
                IncorrectFormat("one floating point value.", *this);
                break;
            }
            glState->GetLight(lightSelected)->lightConstantAttenuation = atof((constantAttenuation ->GetValue()).c_str());
            break;

        case Id::AttenuationLinearVal:
            if((linearAttenuation->GetValue()).IsEmpty())
            {
                IncorrectFormat(wxT("one floating point value."), *this);
                break;
            }
            glState->GetLight(lightSelected)->lightLinearAttenuation = atof((linearAttenuation   ->GetValue()).c_str());
            break;

        case Id::AttenuationQuadraticVal:
            if((quadraticAttenuation->GetValue()).IsEmpty())
            {
                IncorrectFormat(wxT("one floating point value."), *this);
                break;
            }
            glState->GetLight(lightSelected)->lightQuadraticAttenuation = atof((quadraticAttenuation->GetValue()).c_str());
            break;

        case Id::LightPositionVal:
            if(!(lightPosition->GetValue()).IsEmpty())
            {
                if((parseVector(lightPosition->GetValue())).GetCount() == 4)
                {
                    userEnteredValues = parseVector(lightPosition->GetValue());
                }
                else
                {
                    IncorrectFormat(wxT("four floating point values, with each value separated by a comma."), *this);
                    return;
                }
            }
            else
            {
                    IncorrectFormat(wxT("four floating point values, with each value separated by a comma."), *this);
                    return;
                }
            for(int i = 0; i < 4; i++)
            {
                tempLightPosVector[i] = atof(userEnteredValues.Item(i).c_str());
            }
            if(tempLightPosVector[3] != 1 && tempLightPosVector[3] != 0.0)
            {
                wxMessageDialog badW(this, wxT("The w component of GL_POSITION must be 0.0 or 1.0"), wxT(""), wxOK, wxDefaultPosition);
                badW.ShowModal();
            }

            glState->GetLight(lightSelected)->lightPositionVector = tempLightPosVector;
            break;

        case Id::SpotCutoffVal:
            if((spotCutoff->GetValue()).IsEmpty())
            {
                IncorrectFormat(wxT("one floating point value."), *this);
                break;
            }
            glState->GetLight(lightSelected)->lightSpotCutoff = atof(spotCutoff->GetValue().c_str());
            break;

        case Id::SpotDirectionVal:
            if(!(spotDirection->GetValue()).IsEmpty())
            {
                if((parseVector(spotDirection->GetValue())).GetCount() == 3)
                {
                    userEnteredValues = parseVector(spotDirection->GetValue());
                }
                else
                {
                    IncorrectFormat(wxT("three floating point values, with each value separated by a comma."), *this);
                    break;
                }
            }
            else
            {
                    IncorrectFormat(wxT("three floating point values, with each value separated by a comma."), *this);
                    break;
            }
            userEnteredValues = parseVector(spotDirection->GetValue());
            for(int i = 0; i < 3; i++)
            {
                tempLightSpotDirectionVector[i] = atof((userEnteredValues.Item(i)).c_str());
            }
            glState->GetLight(lightSelected)->lightSpotDirectionVector = tempLightSpotDirectionVector;
            break;

        case Id::SpotExponentVal:
            if((spotExponent->GetValue()).IsEmpty())
            {
                IncorrectFormat(wxT("one floating point value."), *this);
                return;
            }
            glState->GetLight(lightSelected)->lightSpotExponent = atof(spotExponent->GetValue().c_str());
            break;

        default:
            break;
        }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglLightNBPage::OnRadio(wxCommandEvent& event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetLightChanged(true);
    Light* light = glState->GetLight(lightSelectionBox->GetSelection());
    switch(event.GetId()){
        case Id::LightSelectionBoxVal:

            ambientLight ->SetBackgroundColour(ToWxIntColor(light->lightAmbientColorVector ));
            diffuseLight ->SetBackgroundColour(ToWxIntColor(light->lightDiffuseColorVector ));
            specularLight->SetBackgroundColour(ToWxIntColor(light->lightSpecularColorVector));

            spotDirection->SetValue(FloatToString3( light->lightSpotDirectionVector ));
            spotExponent ->SetValue(wxString::Format(wxT("%.1f"),light->lightSpotExponent));
            spotCutoff   ->SetValue(wxString::Format(wxT("%.1f"),light->lightSpotCutoff));

            constantAttenuation->SetValue(wxString::Format(wxT("%.1f"),light->lightConstantAttenuation));
            quadraticAttenuation->SetValue(wxString::Format(wxT("%.1f"),light->lightQuadraticAttenuation));
            linearAttenuation->SetValue(wxString::Format(wxT("%.1f"),light->lightLinearAttenuation));
            
            lightPosition->SetValue(FloatToString4(light->lightPositionVector));
            
        default:
            break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}
