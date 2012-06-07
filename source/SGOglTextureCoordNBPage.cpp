/************************************************************************
* Author(s):     Joshua Doss                                            *
* Last Modified: 11/09/2005                                             *
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

#include "SGOglTextureCoordNBPage.h"
#include "SGOglNotebook.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"
#include "App.h"
#include "SGFrame.h"
#include "SGCanvas.h"
#include <wx/colordlg.h>

BEGIN_EVENT_TABLE(SGOglTextureCoordNBPage, wxPanel)
    EVT_COMMAND(Id::TextureCoordUnit, wxEVT_COMMAND_RADIOBOX_SELECTED, SGOglTextureCoordNBPage::OnRadioTextureCoordUnit)
    EVT_COMMAND(Id::TexCoordGen, wxEVT_COMMAND_RADIOBOX_SELECTED, SGOglTextureCoordNBPage::OnRadioTexCoordGen)
    EVT_COMMAND_RANGE(Id::Tex0TexGenEnableVal, Id::Tex4TexGenEnableVal, wxEVT_COMMAND_CHECKBOX_CLICKED, SGOglTextureCoordNBPage::OnCheckbox)
    EVT_COMMAND(Id::EyePlaneCoeffValS, wxEVT_COMMAND_TEXT_ENTER, SGOglTextureCoordNBPage::OnTextEnterEyeCoeffS)
    EVT_COMMAND(Id::EyePlaneCoeffValT, wxEVT_COMMAND_TEXT_ENTER, SGOglTextureCoordNBPage::OnTextEnterEyeCoeffT)
    EVT_COMMAND(Id::ObjectPlaneCoeffValS, wxEVT_COMMAND_TEXT_ENTER, SGOglTextureCoordNBPage::OnTextEnterObjCoeffS)
    EVT_COMMAND(Id::ObjectPlaneCoeffValT, wxEVT_COMMAND_TEXT_ENTER, SGOglTextureCoordNBPage::OnTextEnterObjCoeffT)
END_EVENT_TABLE()

SGOglTextureCoordNBPage::SGOglTextureCoordNBPage(SGOglNotebook* parent, wxWindowID id)
:wxPanel(parent,id)
{
    m_parent = parent;
    SGFixedGLState* glState = m_parent->GetGLState();

    wxStaticBox* texBox            = new wxStaticBox(this, wxID_ANY, wxT("Textures"),   wxDefaultPosition, wxDefaultSize, ZERO_PIXEL_BORDER);
    wxStaticBox* texPropertyBox    = new wxStaticBox(this, wxID_ANY, wxT("Selected Texture Properties"),   wxDefaultPosition, wxDefaultSize, ZERO_PIXEL_BORDER);

    wxStaticBoxSizer* textureSizer                 = new wxStaticBoxSizer(texBox, wxVERTICAL);
    wxStaticBoxSizer* selectedTexPropertiesSizer   = new wxStaticBoxSizer(texPropertyBox, wxHORIZONTAL);

    wxStaticBox* eyePlaneCoeffBox = new wxStaticBox(this, wxID_ANY, wxT("Eye Plane Coefficients"), wxDefaultPosition, wxDefaultSize, ZERO_PIXEL_BORDER);
    wxStaticBoxSizer* eyePlaneCoeffBoxSizer = new wxStaticBoxSizer(eyePlaneCoeffBox, wxHORIZONTAL);

    wxBoxSizer* eyePlaneLabelSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* eyePlaneTextSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* eyePlaneCoeffLabelS = new wxStaticText(this, wxID_ANY, wxT("GL_S"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* eyePlaneCoeffLabelT = new wxStaticText(this, wxID_ANY, wxT("GL_T"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    
    eyePlaneCoeffTextS = new wxTextCtrl(this, Id::EyePlaneCoeffValS, FloatToString4(glState->GetTexture(0)->eyePlaneCoeffS), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Eye Plane Coefficient S")  );
    eyePlaneCoeffTextT = new wxTextCtrl(this, Id::EyePlaneCoeffValT, FloatToString4(glState->GetTexture(0)->eyePlaneCoeffT), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Eye Plane Coefficient T")  );
   
    eyePlaneLabelSizer->Add(eyePlaneCoeffLabelS, 0, wxTOP, TEN_PIXEL_BORDER);
    eyePlaneLabelSizer->Add(eyePlaneCoeffLabelT, 0, wxTOP, FIFTEEN_PIXEL_BORDER);

    eyePlaneTextSizer->Add(eyePlaneCoeffTextS, 0, wxALL, FIVE_PIXEL_BORDER);
    eyePlaneTextSizer->Add(eyePlaneCoeffTextT, 0, wxALL, FIVE_PIXEL_BORDER);

    eyePlaneCoeffBoxSizer->Add(eyePlaneLabelSizer, 0, wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);
    eyePlaneCoeffBoxSizer->Add(eyePlaneTextSizer, 0, wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);

    wxStaticBox* objectPlaneCoeffBox = new wxStaticBox(this, wxID_ANY, wxT("Object Plane Coefficients"), wxDefaultPosition, wxDefaultSize, ZERO_PIXEL_BORDER);
    wxStaticBoxSizer* objectPlaneCoeffBoxSizer = new wxStaticBoxSizer(objectPlaneCoeffBox, wxHORIZONTAL);
    
    wxBoxSizer* objectPlaneLabelSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* objectPlaneTextSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* objectPlaneCoeffLabelS = new wxStaticText(this, wxID_ANY, wxT("GL_S"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* objectPlaneCoeffLabelT = new wxStaticText(this, wxID_ANY, wxT("GL_T"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    objectPlaneCoeffTextS = new wxTextCtrl(this, Id::ObjectPlaneCoeffValS, FloatToString4(glState->GetTexture(0)->objectPlaneCoeffS), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Object Plane Coefficient S")  );
    objectPlaneCoeffTextT = new wxTextCtrl(this, Id::ObjectPlaneCoeffValT, FloatToString4(glState->GetTexture(0)->objectPlaneCoeffT), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("Object Plane Coefficient T")  );

    objectPlaneLabelSizer->Add(objectPlaneCoeffLabelS, 0, wxTOP, TEN_PIXEL_BORDER);
    objectPlaneLabelSizer->Add(objectPlaneCoeffLabelT, 0, wxTOP, FIFTEEN_PIXEL_BORDER);

    objectPlaneTextSizer->Add(objectPlaneCoeffTextS, 0, wxALL, FIVE_PIXEL_BORDER);
    objectPlaneTextSizer->Add(objectPlaneCoeffTextT, 0, wxALL, FIVE_PIXEL_BORDER);

    objectPlaneCoeffBoxSizer->Add(objectPlaneLabelSizer, 0, wxALL |wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);
    objectPlaneCoeffBoxSizer->Add(objectPlaneTextSizer, 0, wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);

    wxBoxSizer* coeffSizer = new wxBoxSizer(wxVERTICAL);
    coeffSizer->Add(eyePlaneCoeffBoxSizer, 0, wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);
    coeffSizer->Add(objectPlaneCoeffBoxSizer, 0, wxALL | wxADJUST_MINSIZE, FIVE_PIXEL_BORDER);

    wxString texNumArray[5];
    texNumArray[0] = wxT("T0");
    texNumArray[1] = wxT("T1");
    texNumArray[2] = wxT("T2");
    texNumArray[3] = wxT("T3");
    texNumArray[4] = wxT("T4");
    texCoordUnitBox = new wxRadioBox(this, Id::TextureCoordUnit, wxT("Selected Texture Unit"), wxDefaultPosition, wxDefaultSize, 5, texNumArray, 1,wxRA_SPECIFY_ROWS, wxDefaultValidator);
  
    tex0TexGenEnableCheckBox = new wxCheckBox(this, Id::Tex0TexGenEnableVal, wxT("T0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex0TexGenEnableCheckBox->SetValue(glState->GetTexture(0)->texGen);

    tex1TexGenEnableCheckBox = new wxCheckBox(this, Id::Tex1TexGenEnableVal, wxT("T1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex1TexGenEnableCheckBox->SetValue(glState->GetTexture(1)->texGen);

    tex2TexGenEnableCheckBox = new wxCheckBox(this, Id::Tex2TexGenEnableVal, wxT("T2"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex2TexGenEnableCheckBox->SetValue(glState->GetTexture(2)->texGen);

    tex3TexGenEnableCheckBox = new wxCheckBox(this, Id::Tex3TexGenEnableVal, wxT("T3"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex3TexGenEnableCheckBox->SetValue(glState->GetTexture(3)->texGen);

    tex4TexGenEnableCheckBox = new wxCheckBox(this, Id::Tex4TexGenEnableVal, wxT("T4"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex4TexGenEnableCheckBox->SetValue(glState->GetTexture(4)->texGen);

    wxStaticBox* texGenEnableDisableBox        = new wxStaticBox(this, wxID_ANY, wxT("glEnable/glDisable TexGen"),   wxDefaultPosition, wxDefaultSize, ZERO_PIXEL_BORDER);
    wxStaticBoxSizer* texGenEnableDisableSizer    = new wxStaticBoxSizer(texGenEnableDisableBox, wxHORIZONTAL);    

    texGenEnableDisableSizer->Add(tex0TexGenEnableCheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);
    texGenEnableDisableSizer->Add(tex1TexGenEnableCheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);
    texGenEnableDisableSizer->Add(tex2TexGenEnableCheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);
    texGenEnableDisableSizer->Add(tex3TexGenEnableCheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);
    texGenEnableDisableSizer->Add(tex4TexGenEnableCheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);

    wxBoxSizer* selectedTextureAndEnableSizer = new wxBoxSizer(wxHORIZONTAL);
    selectedTextureAndEnableSizer->Add(texGenEnableDisableSizer, 0, wxRIGHT | wxLEFT, SEVEN_PIXEL_BORDER);
    selectedTextureAndEnableSizer->Add(texCoordUnitBox);
    textureSizer->Add(selectedTextureAndEnableSizer);

    wxString coordGenArray[5];
    coordGenArray[0] = wxT("GL_OBJECT_LINEAR");
    coordGenArray[1] = wxT("GL_EYE_LINEAR");
    coordGenArray[2] = wxT("GL_SPHERE_MAP");
    coordGenArray[3] = wxT("GL_REFLECTION_MAP");
    coordGenArray[4] = wxT("GL_NORMAL_MAP");
    coordGenBox      = new wxRadioBox(this, Id::TexCoordGen, wxT("Texture Coordinate Generation Method (Vertex Shader)"),    wxDefaultPosition, wxDefaultSize, 5, coordGenArray, 1,wxRA_SPECIFY_COLS, wxDefaultValidator);

    selectedTexPropertiesSizer->Add(coordGenBox, 0 , wxALL , FIVE_PIXEL_BORDER);
    selectedTexPropertiesSizer->Add(coeffSizer, 0, wxALL, ZERO_PIXEL_BORDER);

    textureSizer->Add(selectedTexPropertiesSizer, 0, wxALL, FIVE_PIXEL_BORDER);
    SetAutoLayout(TRUE);
    
    SetSizer(textureSizer);
    textureSizer->SetSizeHints(this);
}

void SGOglTextureCoordNBPage::OnRadioTextureCoordUnit(wxCommandEvent &event)
{
    UpdateWidgets();
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureCoordNBPage::OnRadioTexCoordGen(wxCommandEvent& event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    int workingTextureCoords;

    workingTextureCoords = texCoordUnitBox->GetSelection();

    switch(coordGenBox->GetSelection()){
                case TEXTURE_COORDINATE_OBJECT_LINEAR:
                    glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_OBJECT_LINEAR;
                    break;
                case TEXTURE_COORDINATE_EYE_LINEAR:
                    glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_EYE_LINEAR;
                    break;
                case TEXTURE_COORDINATE_SPHERE_MAP:
                    glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_SPHERE_MAP;
                    break;
                case TEXTURE_COORDINATE_REFLECTION_MAP:
                    glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_REFLECTION_MAP;
                    break;
                case TEXTURE_COORDINATE_NORMAL_MAP:
                    glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_NORMAL_MAP;
                    break;
                default:
                    break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureCoordNBPage::OnCheckbox(wxCommandEvent &event)
{
    SGFixedGLState *glState = m_parent->GetGLState();

    switch(event.GetId()){
        case Id::Tex0TexGenEnableVal:
            glState->GetTexture(0)->texGen = tex0TexGenEnableCheckBox->IsChecked();
            break;
        case Id::Tex1TexGenEnableVal:
            glState->GetTexture(1)->texGen = tex1TexGenEnableCheckBox->IsChecked();
            break;  
        case Id::Tex2TexGenEnableVal:
            glState->GetTexture(2)->texGen = tex2TexGenEnableCheckBox->IsChecked();
            break;
        case Id::Tex3TexGenEnableVal:
            glState->GetTexture(3)->texGen = tex3TexGenEnableCheckBox->IsChecked();
            break;
        case Id::Tex4TexGenEnableVal:
            glState->GetTexture(4)->texGen = tex4TexGenEnableCheckBox->IsChecked();
            break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureCoordNBPage::OnTextEnterEyeCoeffS(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    wxArrayString userEnteredValues;

    vec4 eyePlaneSVec;

    if((parseVector(eyePlaneCoeffTextS->GetValue())).GetCount() == 4)
    {
        userEnteredValues = parseVector(eyePlaneCoeffTextS->GetValue());
        for(int i = 0; i < 4; i++)
        {
            eyePlaneSVec[i] = atof((userEnteredValues.Item(i)).c_str());
        }
        glState->GetTexture(texCoordUnitBox->GetSelection())->eyePlaneCoeffS = eyePlaneSVec;
    }
    else
    {
        IncorrectFormat(wxT("four floating point values, with each value seperated by a comma."), *this);
        return;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureCoordNBPage::OnTextEnterEyeCoeffT(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    wxArrayString userEnteredValues;

    vec4 eyePlaneTVec;

    if((parseVector(eyePlaneCoeffTextT->GetValue())).GetCount() == 4)
    {
        userEnteredValues = parseVector(eyePlaneCoeffTextT->GetValue());
        for(int i = 0; i < 4; i++)
        {
            eyePlaneTVec[i] = atof((userEnteredValues.Item(i)).c_str());
        }
        glState->GetTexture(texCoordUnitBox->GetSelection())->eyePlaneCoeffT = eyePlaneTVec;
    }
    else
    {
        IncorrectFormat(wxT("four floating point values, with each value seperated by a comma."), *this);
        return;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureCoordNBPage::OnTextEnterObjCoeffS(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    wxArrayString userEnteredValues;

    vec4 objPlaneSVec;

    if((parseVector(objectPlaneCoeffTextS->GetValue())).GetCount() == 4)
    {
        userEnteredValues = parseVector(objectPlaneCoeffTextS->GetValue());
        for(int i = 0; i < 4; i++)
        {
            objPlaneSVec[i] = atof((userEnteredValues.Item(i)).c_str());
        }
        glState->GetTexture(texCoordUnitBox->GetSelection())->objectPlaneCoeffS = objPlaneSVec;                
    }
    else
    {
        IncorrectFormat(wxT("four floating point values, with each value seperated by a comma."), *this);
        return;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureCoordNBPage::OnTextEnterObjCoeffT(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    wxArrayString userEnteredValues;

    vec4 objPlaneTVec;

    if((parseVector(objectPlaneCoeffTextT->GetValue())).GetCount() == 4)
    {
        userEnteredValues = parseVector(objectPlaneCoeffTextT->GetValue());
        for(int i = 0; i < 4; i++)
        {
            objPlaneTVec[i] = atof((userEnteredValues.Item(i)).c_str());
        }
        glState->GetTexture(texCoordUnitBox->GetSelection())->objectPlaneCoeffT = objPlaneTVec;
    }
    else
    {
        IncorrectFormat("four floating point values, with each value seperated by a comma.", *this);
        return;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureCoordNBPage::UpdateWidgets()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(texCoordUnitBox->GetSelection());

    switch(texture->textureCoordinateGeneration){
        case GL_OBJECT_LINEAR:
            coordGenBox->SetSelection(TEXTURE_COORDINATE_OBJECT_LINEAR);
            break;
        case GL_EYE_LINEAR:
            coordGenBox->SetSelection(TEXTURE_COORDINATE_EYE_LINEAR);
            break;
        case GL_SPHERE_MAP:
            coordGenBox->SetSelection(TEXTURE_COORDINATE_SPHERE_MAP);
            break;
        case GL_REFLECTION_MAP:
            coordGenBox->SetSelection(TEXTURE_COORDINATE_REFLECTION_MAP);
            break;
        case GL_NORMAL_MAP:
            coordGenBox->SetSelection(TEXTURE_COORDINATE_NORMAL_MAP);
            break;
        default:
            break;
    }

    eyePlaneCoeffTextS->SetValue(FloatToString4(texture->eyePlaneCoeffS));
    eyePlaneCoeffTextT->SetValue(FloatToString4(texture->eyePlaneCoeffT));
    objectPlaneCoeffTextS->SetValue(FloatToString4(texture->objectPlaneCoeffS));
    objectPlaneCoeffTextT->SetValue(FloatToString4(texture->objectPlaneCoeffT));
}
