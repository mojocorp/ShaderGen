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
#include "SGOglMaterialNBPage.h"
#include "SGOglNotebook.h"
#include "UtilityFunctions.h"
#include "App.h"
#include "SGFrame.h"
#include "SGCanvas.h"
#include <wx/colordlg.h>

BEGIN_EVENT_TABLE(SGOglMaterialNBPage, wxPanel)
    EVT_COMMAND_RANGE(Id::FFSD_START_MATERIAL, Id::FFSD_END_MATERIAL, wxEVT_COMMAND_TEXT_ENTER, SGOglMaterialNBPage::OnTextEnter)
    EVT_COMMAND_RANGE(Id::FFSD_START_MATERIAL, Id::FFSD_END_MATERIAL, wxEVT_COMMAND_BUTTON_CLICKED, SGOglMaterialNBPage::OnButton)
END_EVENT_TABLE()

SGOglMaterialNBPage::SGOglMaterialNBPage(SGOglNotebook* parent, wxWindowID id)
:wxPanel(parent,id)
{
    m_parent = parent;
    SGFixedGLState* glState = m_parent->GetGLState();
    Material* mat = glState->GetMaterial();

    wxStaticBox* materialBox  = new wxStaticBox(this, wxID_ANY, wxT("Material Properties"),   wxDefaultPosition, wxDefaultSize, 0);
    wxStaticBoxSizer* materialSizer = new wxStaticBoxSizer(materialBox, wxHORIZONTAL);
    wxBoxSizer* v1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* v2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* v3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* v4 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* v5 = new wxBoxSizer(wxVERTICAL);


    shininessMaterial = new wxTextCtrl(this, Id::ShininessMaterialVal, FloatToString1(mat->materialShininess),                   wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Specular Exponent"));

    ambientMaterial = new wxButton(this, Id::AmbientMaterialVal, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("Ambient Color of Material"));
    diffuseMaterial = new wxButton(this, Id::DiffuseMaterialVal, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("Diffuse Color of Material"));
    specularMaterial = new wxButton(this, Id::SpecularMaterialVal,wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("Specular Color of Material"));
    emissionMaterial = new wxButton(this, Id::EmissionMaterialVal,wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("Emissive Color of Material"));

    ambientMaterial->SetBackgroundColour(ToWxIntColor(mat->materialAmbientColorVector));
    diffuseMaterial->SetBackgroundColour(ToWxIntColor(mat->materialDiffuseColorVector));
    specularMaterial->SetBackgroundColour(ToWxIntColor(mat->materialSpecularColorVector));
    emissionMaterial->SetBackgroundColour(ToWxIntColor(mat->materialEmissionColorVector));

    wxStaticText* ambientMatLbl = new wxStaticText(this, 0, wxT("GL_AMBIENT"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* specularMatLbl = new wxStaticText(this, 0, wxT("GL_SPECULAR"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* diffuseMatLbl = new wxStaticText(this, 0, wxT("GL_DIFFUSE"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* shininessLbl = new wxStaticText(this, 0, wxT("GL_SHININESS"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* emissionLbl = new wxStaticText(this, 0, wxT("GL_EMISSION"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    v2->Add(ambientMatLbl,0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);
    v2->Add(diffuseMatLbl,0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);
    v2->Add(specularMatLbl,0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);

    v3->Add(ambientMaterial,0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);
    v3->Add(diffuseMaterial,0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);
    v3->Add(specularMaterial,0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);

    v4->Add(shininessLbl,0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);
    v4->Add(emissionLbl,0, wxTOP |wxBOTTOM | wxADJUST_MINSIZE, 9);

    v5->Add(shininessMaterial,0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);
    v5->Add(emissionMaterial,0, wxALIGN_CENTER | wxALL | wxADJUST_MINSIZE, 5);

    materialSizer->Add(v1, 0 , wxALL |wxADJUST_MINSIZE, 10);
    materialSizer->Add(v2, 0 , wxALL |wxADJUST_MINSIZE, 10);
    materialSizer->Add(v3, 0 , wxALL |wxADJUST_MINSIZE, 10);
    materialSizer->Add(v4, 0 , wxALL |wxADJUST_MINSIZE, 10);
    materialSizer->Add(v5, 0 , wxALL |wxADJUST_MINSIZE, 10);

    this->SetAutoLayout(TRUE);
    SetSizer( materialSizer );
    materialSizer->SetSizeHints( this );
}

void SGOglMaterialNBPage::OnButton(wxCommandEvent& event)
{
    SGFixedGLState* glState = this->m_parent->GetGLState();
    Material* mat = glState->GetMaterial();
    glState->SetMaterialChanged(true);
    switch(event.GetId()){
        case Id::AmbientMaterialVal :
            mat->materialAmbientColorVector = GetDialog(mat, 1);
            ambientMaterial->SetBackgroundColour(ToWxIntColor(mat->materialAmbientColorVector));
            break;
        case Id::DiffuseMaterialVal :
            mat->materialDiffuseColorVector = GetDialog(mat, 2);
            diffuseMaterial->SetBackgroundColour(ToWxIntColor(mat->materialDiffuseColorVector));
            break;
        case Id::SpecularMaterialVal :
            mat->materialSpecularColorVector = GetDialog(mat, 3);
            specularMaterial->SetBackgroundColour(ToWxIntColor(mat->materialSpecularColorVector));
            break;
        case Id::EmissionMaterialVal :
            mat->materialEmissionColorVector = GetDialog(mat, 4);
            emissionMaterial->SetBackgroundColour(ToWxIntColor(mat->materialEmissionColorVector));
            break;
        default :
            break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
    event.Skip();
}

void SGOglMaterialNBPage::OnTextEnter(wxCommandEvent& event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetMaterialChanged(true);
    wxArrayString userEnteredValues;
    if((shininessMaterial->GetValue()).IsEmpty())
    {
        IncorrectFormat(wxT("one floating point value."), *this);
        return;
    }
    glState->GetMaterial()->materialShininess = atof((shininessMaterial->GetValue()).c_str());

    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

const vec4 SGOglMaterialNBPage::GetDialog(Material *m, int i)
{
    wxColour oglColor;
    wxColourData data;
    wxColourDialog colLog;
    data.SetChooseFull(TRUE);
    switch(i){
        case 1:
            data.SetColour(ToWxIntColor(m->materialAmbientColorVector));
            break;
        case 2:
            data.SetColour(ToWxIntColor(m->materialDiffuseColorVector));
            break;
        case 3:
            data.SetColour(ToWxIntColor(m->materialSpecularColorVector));
            break;
        case 4:
            data.SetColour(ToWxIntColor(m->materialEmissionColorVector));
            break;
        default:
            break;
    }
    colLog.Create(this, &data);
    colLog.ShowModal();
    data = colLog.GetColourData();
    oglColor = data.GetColour();
    return ToGLFPColor(oglColor);
}
