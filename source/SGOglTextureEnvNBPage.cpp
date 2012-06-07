/************************************************************************
* Author(s):     Joshua Doss                                            *
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

#include "SGOglTextureEnvNBPage.h"
#include "SGOglNotebook.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"
#include "App.h"
#include "SGFrame.h"
#include "SGCanvas.h"
#include <wx/choice.h>
#include <wx/colordlg.h>

BEGIN_EVENT_TABLE(SGOglTextureEnvNBPage, wxPanel)
    EVT_COMMAND(Id::TextureNum, wxEVT_COMMAND_RADIOBOX_SELECTED, SGOglTextureEnvNBPage::OnRadioTextureNum)
    EVT_COMMAND(Id::TexApply, wxEVT_COMMAND_RADIOBOX_SELECTED, SGOglTextureEnvNBPage::OnRadioTexApply)
    EVT_COMMAND_RANGE(Id::TexEnableVal, Id::Tex4EnableVal, wxEVT_COMMAND_CHECKBOX_CLICKED, SGOglTextureEnvNBPage::OnCheckbox)
    EVT_COMMAND(Id::TextureChoose, wxEVT_COMMAND_CHOICE_SELECTED, SGOglTextureEnvNBPage::OnChoiceTextureChoose)
    EVT_COMMAND(Id::TextureCombineMode, wxEVT_COMMAND_CHOICE_SELECTED, SGOglTextureEnvNBPage::OnChoiceTextureCombineMode)
    EVT_COMMAND(Id::TextureCombineOperandArg0, wxEVT_COMMAND_CHOICE_SELECTED, SGOglTextureEnvNBPage::OnChoiceTextureCombineOperandArg0)
    EVT_COMMAND(Id::TextureCombineOperandArg1, wxEVT_COMMAND_CHOICE_SELECTED, SGOglTextureEnvNBPage::OnChoiceTextureCombineOperandArg1)
    EVT_COMMAND(Id::TextureCombineOperandArg2, wxEVT_COMMAND_CHOICE_SELECTED, SGOglTextureEnvNBPage::OnChoiceTextureCombineOperandArg2)
    EVT_COMMAND(Id::TextureCombineScale, wxEVT_COMMAND_CHOICE_SELECTED, SGOglTextureEnvNBPage::OnChoiceTextureCombineScale)
    EVT_COMMAND(Id::TextureCombineSrc0RGB, wxEVT_COMMAND_CHOICE_SELECTED, SGOglTextureEnvNBPage::OnChoiceTextureCombineSrc0RGB)
    EVT_COMMAND(Id::TextureCombineSrc1RGB, wxEVT_COMMAND_CHOICE_SELECTED, SGOglTextureEnvNBPage::OnChoiceTextureCombineSrc1RGB)
    EVT_COMMAND(Id::TextureCombineSrc2RGB, wxEVT_COMMAND_CHOICE_SELECTED, SGOglTextureEnvNBPage::OnChoiceTextureCombineSrc2RGB)
    EVT_BUTTON(Id::TexEnvColor, SGOglTextureEnvNBPage::OnButton)
END_EVENT_TABLE()

SGOglTextureEnvNBPage::SGOglTextureEnvNBPage(SGOglNotebook* parent, wxWindowID id)
:wxPanel(parent,id)
{
    m_parent = parent;
    SGFixedGLState* glState = m_parent->GetGLState();

    wxBoxSizer* texMainSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBox* texBox            = new wxStaticBox(this, wxID_ANY, wxT("Textures"),   wxDefaultPosition, wxDefaultSize, ZERO_PIXEL_BORDER);
    wxStaticBox* texPropertyBox    = new wxStaticBox(this, wxID_ANY, wxT("Selected Texture Properties"),   wxDefaultPosition, wxDefaultSize, ZERO_PIXEL_BORDER);

    wxStaticBoxSizer* textureSizer                 = new wxStaticBoxSizer(texBox, wxVERTICAL);
    wxStaticBoxSizer* selectedTexPropertiesSizer   = new wxStaticBoxSizer(texPropertyBox, wxHORIZONTAL);

    wxBoxSizer* texChooseTexEnvColorWithLabelSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBox* texEnableDisableBox        = new wxStaticBox(this, wxID_ANY, wxT("glEnable/glDisable (Select up to 5 Textures)"),   wxDefaultPosition, wxDefaultSize, ZERO_PIXEL_BORDER);
    wxStaticBoxSizer* texEnableDisableSizer = new wxStaticBoxSizer(texEnableDisableBox, wxHORIZONTAL);

    wxBoxSizer* texCombineModeScaleBoxSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* texCombineArgLabelBoxSizer  = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* texCombineSourceBoxSizer    = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* texCombineOperandBoxSizer   = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* texCombineOperandSourceBoxSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBox* texCombineStaticBox = new wxStaticBox(this, wxID_ANY, wxT("GL_COMBINE Parameters"), wxDefaultPosition, wxDefaultSize, FIFTY_PIXEL_BORDER);

    wxStaticBoxSizer* texCombineStaticBoxSizer= new wxStaticBoxSizer(texCombineStaticBox, wxVERTICAL);

    wxString texNumArray[5];
    texNumArray[0] = wxT("T0");
    texNumArray[1] = wxT("T1");
    texNumArray[2] = wxT("T2");
    texNumArray[3] = wxT("T3");
    texNumArray[4] = wxT("T4");
    textureBox = new wxRadioBox(this, Id::TextureNum, wxT("Selected Texture Unit"),    wxDefaultPosition, wxDefaultSize, 5, texNumArray, 1,wxRA_SPECIFY_ROWS, wxDefaultValidator);

    wxString texChooseArray[Id::NumTextures];
    texChooseArray[0] = wxT("3Dlabs");
    texChooseArray[1] = wxT("3Dlabs Normal");
    texChooseArray[2] = wxT("Rust");
    texChooseArray[3] = wxT("Leopard");
    texChooseArray[4] = wxT("Eyeball");
    texChooseArray[5] = wxT("CobbleStone");
    texChooseArray[6] = wxT("CobbleStone Normal");
    texChooseArray[7] = wxT("Bricks");
    texChooseArray[8] = wxT("Bricks Normal");
    texChooseArray[9] = wxT("Stone Wall");
    texChooseArray[10] = wxT("Stone Wall Normal");
    texChooseArray[11] = wxT("Metal Sheet");
    texChooseArray[12] = wxT("Metal Sheet Normal");
    texChoose = new wxChoice(this, Id::TextureChoose, wxDefaultPosition, wxDefaultSize, Id::NumTextures, texChooseArray, 0, wxDefaultValidator);

    texEnvColorButton = new wxButton(this, Id::TexEnvColor, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxT("TexEnvColor"));

    texEnvColorButton->SetBackgroundColour(ToWxIntColor(glState->GetTexture(textureBox->GetSelection())->texEnvColor));

    wxStaticText *texEnvButtonLabel = new wxStaticText(this, wxID_ANY, wxT("GL_TEX_ENV_COLOR"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    tex0CheckBox = new wxCheckBox(this, Id::Tex0EnableVal, wxT("T0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex0CheckBox->SetValue(glState->GetTexture(0)->textureEnabled);

    tex1CheckBox = new wxCheckBox(this, Id::Tex1EnableVal, wxT("T1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex1CheckBox->SetValue(glState->GetTexture(1)->textureEnabled);

    tex2CheckBox = new wxCheckBox(this, Id::Tex2EnableVal, wxT("T2"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex2CheckBox->SetValue(glState->GetTexture(2)->textureEnabled);

    tex3CheckBox = new wxCheckBox(this, Id::Tex3EnableVal, wxT("T3"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex3CheckBox->SetValue(glState->GetTexture(3)->textureEnabled);

    tex4CheckBox = new wxCheckBox(this, Id::Tex4EnableVal, wxT("T4"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    tex4CheckBox->SetValue(glState->GetTexture(4)->textureEnabled);

    texEnableDisableSizer->Add(tex0CheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);
    texEnableDisableSizer->Add(tex1CheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);
    texEnableDisableSizer->Add(tex2CheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);
    texEnableDisableSizer->Add(tex3CheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);
    texEnableDisableSizer->Add(tex4CheckBox, 0, wxALL | wxADJUST_MINSIZE, SEVEN_PIXEL_BORDER);

    wxString texApplyArray[6];
    texApplyArray[0] = wxT("GL_REPLACE");
    texApplyArray[1] = wxT("GL_MODULATE");
    texApplyArray[2] = wxT("GL_DECAL");
    texApplyArray[3] = wxT("GL_BLEND");
    texApplyArray[4] = wxT("GL_ADD");
    texApplyArray[5] = wxT("GL_COMBINE");
    texApplyBox = new wxRadioBox(this, Id::TexApply, wxT("Texture Application Method (Fragment Shader)"),    wxDefaultPosition, wxDefaultSize, 6, texApplyArray, 1,wxRA_SPECIFY_COLS, wxDefaultValidator);

    wxString texCombineModeArray[7];
    texCombineModeArray[0] = wxT("GL_REPLACE");
    texCombineModeArray[1] = wxT("GL_MODULATE");
    texCombineModeArray[2] = wxT("GL_ADD");
    texCombineModeArray[3] = wxT("GL_ADD_SIGNED");
    texCombineModeArray[4] = wxT("GL_INTERPOLATE");
    texCombineModeArray[5] = wxT("GL_SUBTRACT");
    texCombineModeArray[6] = wxT("GL_DOT3_RGB");
    texCombineModeChoose = new wxChoice(this, Id::TextureCombineMode, wxDefaultPosition, wxDefaultSize, 7, texCombineModeArray, 0, wxDefaultValidator);

    wxStaticText* texCombineModeLabel  = new wxStaticText(this, wxID_ANY, wxT("GL_COMBINE_RGB Mode"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* texCombineScaleLabel = new wxStaticText(this, wxID_ANY, wxT("Combine Scale"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    wxString texCombineScaleArray[3];
    texCombineScaleArray[0] = wxT("1.0");
    texCombineScaleArray[1] = wxT("2.0");
    texCombineScaleArray[2] = wxT("4.0");
    texCombineScaleChoose = new wxChoice(this, Id::TextureCombineScale, wxDefaultPosition, wxDefaultSize, 3, texCombineScaleArray, 0, wxDefaultValidator);

    wxString texCombineSrcnRGBArray[4];
    texCombineSrcnRGBArray[0] = wxT("GL_TEXTURE");
    texCombineSrcnRGBArray[1] = wxT("GL_CONSTANT");
    texCombineSrcnRGBArray[2] = wxT("GL_PRIMARY_COLOR");
    texCombineSrcnRGBArray[3] = wxT("GL_PREVIOUS");

    texCombineSrc0RGBChoose = new wxChoice(this, Id::TextureCombineSrc0RGB, wxDefaultPosition, wxDefaultSize, 4, texCombineSrcnRGBArray, 0, wxDefaultValidator);  

    texCombineSrc1RGBChoose = new wxChoice(this, Id::TextureCombineSrc1RGB, wxDefaultPosition, wxDefaultSize, 4, texCombineSrcnRGBArray, 0, wxDefaultValidator);

    texCombineSrc2RGBChoose = new wxChoice(this, Id::TextureCombineSrc2RGB, wxDefaultPosition, wxDefaultSize, 4, texCombineSrcnRGBArray, 0, wxDefaultValidator);

    wxStaticText* texCombineSrcLabel = new wxStaticText(this, wxID_ANY, wxT("Source"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* texCombineOperandLabel = new wxStaticText(this, wxID_ANY, wxT("Operand"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    wxString texCombineOperandArray [2];
    texCombineOperandArray[0] = wxT("GL_SRC_COLOR");
    texCombineOperandArray[1] = wxT("GL_ONE_MINUS_SRC_COLOR");

    texCombineOperandArg0Choose = new wxChoice(this, Id::TextureCombineOperandArg0, wxDefaultPosition, wxDefaultSize, 2, texCombineOperandArray, 0, wxDefaultValidator);

    texCombineOperandArg1Choose = new wxChoice(this, Id::TextureCombineOperandArg1, wxDefaultPosition, wxDefaultSize, 2, texCombineOperandArray, 0, wxDefaultValidator);

    texCombineOperandArg2Choose = new wxChoice(this, Id::TextureCombineOperandArg2, wxDefaultPosition, wxDefaultSize, 2, texCombineOperandArray, 0, wxDefaultValidator);

    wxStaticText* texCombineArg0Label = new wxStaticText(this, wxID_ANY, wxT("Arg0"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    wxStaticText* texCombineArg1Label = new wxStaticText(this, wxID_ANY, wxT("Arg1"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);
    wxStaticText* texCombineArg2Label = new wxStaticText(this, wxID_ANY, wxT("Arg2"), wxDefaultPosition, wxDefaultSize, wxALIGN_TOP);

    texChooseTexEnvColorWithLabelSizer->Add(texEnvButtonLabel, 0, wxALL, FIVE_PIXEL_BORDER);
    texChooseTexEnvColorWithLabelSizer->Add(texEnvColorButton, 0, wxALL, FIVE_PIXEL_BORDER);
    texChooseTexEnvColorWithLabelSizer->Add(texChoose, 0, wxALL, FIVE_PIXEL_BORDER);

    selectedTexPropertiesSizer->Add(texApplyBox, 0 , wxALL , FIVE_PIXEL_BORDER);
    selectedTexPropertiesSizer->Add(texChooseTexEnvColorWithLabelSizer, 0, wxALL, FIVE_PIXEL_BORDER);

    texCombineModeScaleBoxSizer->Add(texCombineModeLabel, 0, wxALL, FIVE_PIXEL_BORDER);
    texCombineModeScaleBoxSizer->Add(texCombineModeChoose, 0, wxALL, FIVE_PIXEL_BORDER);
    texCombineModeScaleBoxSizer->Add(texCombineScaleLabel, 0, wxALL, FIVE_PIXEL_BORDER);    
    texCombineModeScaleBoxSizer->Add(texCombineScaleChoose, 0, wxALL, FIVE_PIXEL_BORDER);

    texCombineArgLabelBoxSizer->Add(texCombineArg0Label, 0, wxTOP, TWENTY_PIXEL_BORDER);
    texCombineArgLabelBoxSizer->Add(texCombineArg1Label, 0, wxTOP, TWENTY_PIXEL_BORDER);
    texCombineArgLabelBoxSizer->Add(texCombineArg2Label, 0, wxTOP, TWENTY_PIXEL_BORDER);

    texCombineSourceBoxSizer->Add(texCombineSrcLabel, 0, wxLEFT, FIFTY_PIXEL_BORDER);
    texCombineSourceBoxSizer->Add(texCombineSrc0RGBChoose, 0, wxALL, FIVE_PIXEL_BORDER);
    texCombineSourceBoxSizer->Add(texCombineSrc1RGBChoose, 0, wxALL, FIVE_PIXEL_BORDER);
    texCombineSourceBoxSizer->Add(texCombineSrc2RGBChoose, 0, wxALL, FIVE_PIXEL_BORDER);

    texCombineOperandBoxSizer->Add(texCombineOperandLabel, 0, wxLEFT, FIFTY_PIXEL_BORDER);
    texCombineOperandBoxSizer->Add(texCombineOperandArg0Choose, 0, wxALL, FIVE_PIXEL_BORDER);
    texCombineOperandBoxSizer->Add(texCombineOperandArg1Choose, 0, wxALL, FIVE_PIXEL_BORDER);
    texCombineOperandBoxSizer->Add(texCombineOperandArg2Choose, 0, wxALL, FIVE_PIXEL_BORDER);

    texCombineOperandSourceBoxSizer->Add(texCombineArgLabelBoxSizer, 0, wxALL, FIVE_PIXEL_BORDER);
    texCombineOperandSourceBoxSizer->Add(texCombineSourceBoxSizer, 0, wxALL, FIVE_PIXEL_BORDER);
    texCombineOperandSourceBoxSizer->Add(texCombineOperandBoxSizer, 0, wxALL, FIVE_PIXEL_BORDER);

    texCombineStaticBoxSizer->Add(texCombineModeScaleBoxSizer, 0, wxALL, FIVE_PIXEL_BORDER);
    texCombineStaticBoxSizer->Add(texCombineOperandSourceBoxSizer, 0, wxALL, FIVE_PIXEL_BORDER);

    textureSizer->Add(texEnableDisableSizer, 0, wxALL, FIVE_PIXEL_BORDER);
    textureSizer->Add(textureBox, 0, wxALL, FIVE_PIXEL_BORDER);
    textureSizer->Add(selectedTexPropertiesSizer, 0, wxALL, FIVE_PIXEL_BORDER);

    texMainSizer->Add(textureSizer, 0, wxALL, FIVE_PIXEL_BORDER);
    texMainSizer->Add(texCombineStaticBoxSizer, 0, wxALL, FIVE_PIXEL_BORDER);

    SetAutoLayout(TRUE);

    SetSizer(texMainSizer);
    texMainSizer->SetSizeHints( this );

    DisableCombine();

    UpdateWidgets();
}

void SGOglTextureEnvNBPage::OnRadioTexApply(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    if(event.GetId() == Id::TexApply)
    {
        switch(texApplyBox->GetSelection()){
                case 0:
                    texture->textureApplicationMethod = GL_REPLACE;
                    DisableCombine();
                    break;
                case 1:
                    texture->textureApplicationMethod = GL_MODULATE;
                    DisableCombine();
                    break;
                case 2:
                    texture->textureApplicationMethod = GL_DECAL;
                    DisableCombine();
                    break;
                case 3:
                    texture->textureApplicationMethod = GL_BLEND;
                    DisableCombine();
                    break;
                case 4:
                    texture->textureApplicationMethod = GL_ADD;
                    DisableCombine();
                    break;
                case 5:
                    texture->textureApplicationMethod = GL_COMBINE;
                    EnableCombine();
                    break;
                default:
                    texture->textureApplicationMethod = GL_MODULATE;
                    DisableCombine();
                    break;
        }
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnRadioTextureNum(wxCommandEvent &event)
{
    UpdateWidgets();

    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnCheckbox(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    switch(event.GetId()){
        case Id::TexEnableVal:
            glState->SetTextureEnable(texEnableCheckBox->IsChecked());
            break;
        case Id::Tex0EnableVal:
            glState->GetTexture(0)->textureEnabled = tex0CheckBox->IsChecked();
            break;
        case Id::Tex1EnableVal:
            glState->GetTexture(1)->textureEnabled = tex1CheckBox->IsChecked();
            break;
        case Id::Tex2EnableVal:
            glState->GetTexture(2)->textureEnabled = tex2CheckBox->IsChecked();
            break;
        case Id::Tex3EnableVal:
            glState->GetTexture(3)->textureEnabled = tex3CheckBox->IsChecked();
            break;
        case Id::Tex4EnableVal:
            glState->GetTexture(4)->textureEnabled = tex4CheckBox->IsChecked();
            break;
        default:
            break;   
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnChoiceTextureChoose(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    texture->textureCurrentSelection = Id::TexturesStart + texChoose->GetSelection();
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}
void SGOglTextureEnvNBPage::OnChoiceTextureCombineMode(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    switch(texCombineModeChoose->GetSelection()){
                case 0:
                    texture->textureCombineMode = GL_REPLACE;
                    break;
                case 1:
                    texture->textureCombineMode = GL_MODULATE;
                    break;
                case 2:
                    texture->textureCombineMode = GL_ADD;
                    break;
                case 3:
                    texture->textureCombineMode = GL_ADD_SIGNED;
                    break;
                case 4:
                    texture->textureCombineMode = GL_INTERPOLATE;
                    break;
                case 5:
                    texture->textureCombineMode = GL_SUBTRACT;
                    break;
                case 6:
                    texture->textureCombineMode = GL_DOT3_RGB;
                    break;
                default:
                    break;                        
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineOperandArg0(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    switch(texCombineOperandArg0Choose->GetSelection()){
                case 0:
                    texture->textureCombineOperand0 = GL_SRC_COLOR;
                    break;
                case 1:
                    texture->textureCombineOperand0 = GL_ONE_MINUS_SRC_COLOR;
                    break;
                default:
                    break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineOperandArg1(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    switch(texCombineOperandArg1Choose->GetSelection()){
                case 0:
                    texture->textureCombineOperand1 = GL_SRC_COLOR;
                    break;
                case 1:
                    texture->textureCombineOperand1 = GL_ONE_MINUS_SRC_COLOR;
                    break;
                default:
                    break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineOperandArg2(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    switch(texCombineOperandArg2Choose->GetSelection()){
                case 0:
                    texture->textureCombineOperand2 = GL_SRC_COLOR;
                    break;
                case 1:
                    texture->textureCombineOperand2 = GL_ONE_MINUS_SRC_COLOR;
                    break;
                default:
                    break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineScale(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    texture->textureCombineScale = (float)(1 << texCombineScaleChoose->GetSelection());
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineSrc0RGB(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    switch(texCombineSrc0RGBChoose->GetSelection()){
                case 0:
                    texture->textureCombineSource0 = GL_TEXTURE;
                    break;
                case 1:
                    texture->textureCombineSource0 = GL_CONSTANT;
                    break;
                case 2:
                    texture->textureCombineSource0 = GL_PRIMARY_COLOR;
                    break;
                case 3:
                    texture->textureCombineSource0 = GL_PREVIOUS;
                    break;
                default:
                    break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineSrc1RGB(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    switch(texCombineSrc1RGBChoose->GetSelection()){
                case 0:
                    texture->textureCombineSource1 = GL_TEXTURE;
                    break;
                case 1:
                    texture->textureCombineSource1 = GL_CONSTANT;
                    break;
                case 2:
                    texture->textureCombineSource1 = GL_PRIMARY_COLOR;
                    break;
                case 3:
                    texture->textureCombineSource1 = GL_PREVIOUS;
                    break;
                default:
                    break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineSrc2RGB(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());
    switch(texCombineSrc2RGBChoose->GetSelection()){
                case 0:
                    texture->textureCombineSource2 = GL_TEXTURE;
                    break;
                case 1:
                    texture->textureCombineSource2 = GL_CONSTANT;
                    break;
                case 2:
                    texture->textureCombineSource2 = GL_PRIMARY_COLOR;
                    break;
                case 3:
                    texture->textureCombineSource2 = GL_PREVIOUS;
                    break;
                default:
                    break;
    }
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
}

void SGOglTextureEnvNBPage::OnButton(wxCommandEvent &event)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    wxColour texEnvColor;
    int textureSelected = textureBox->GetSelection();

    texEnvColor = wxGetColourFromUser(this, ToWxIntColor(glState->GetTexture(textureSelected)->texEnvColor));
    glState->GetTexture(textureSelected)->texEnvColor = ToGLFPColor(texEnvColor);
    FindWindowById(event.GetId())->SetBackgroundColour(texEnvColor);
    wxGetApp().GetFrame()->SetCanvasMode(0);
    wxGetApp().GetFrame()->GetCanvas()->Update();
    event.Skip();
}

void SGOglTextureEnvNBPage::DisableCombine()
{
    texCombineOperandArg0Choose->Disable();
    texCombineOperandArg1Choose->Disable();
    texCombineOperandArg2Choose->Disable();
    texCombineModeChoose       ->Disable();
    texCombineScaleChoose      ->Disable();
    texCombineSrc0RGBChoose    ->Disable();
    texCombineSrc1RGBChoose    ->Disable();
    texCombineSrc2RGBChoose    ->Disable();
}

void SGOglTextureEnvNBPage::EnableCombine()
{
    texCombineOperandArg0Choose->Enable();
    texCombineOperandArg1Choose->Enable();
    texCombineOperandArg2Choose->Enable();
    texCombineModeChoose       ->Enable();
    texCombineScaleChoose      ->Enable();
    texCombineSrc0RGBChoose    ->Enable();
    texCombineSrc1RGBChoose    ->Enable();
    texCombineSrc2RGBChoose    ->Enable();
}

void SGOglTextureEnvNBPage::UpdateWidgets()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(textureBox->GetSelection());

    texChoose->SetSelection(texture->textureCurrentSelection - Id::TexturesStart);

    if(texture->textureCombineScale < 1.9)
    {
        texCombineScaleChoose->SetSelection(COMBINE_SCALE_1_0);
    }
    else if(texture->textureCombineScale < 2.9)
    {
        texCombineScaleChoose->SetSelection(COMBINE_SCALE_2_0);
    }
    else
    {
        texCombineScaleChoose->SetSelection(COMBINE_SCALE_4_0);
    }

    switch(texture->textureApplicationMethod){
        case GL_REPLACE:
            texApplyBox->SetSelection(TEXTURE_APPLICATION_METHOD_REPLACE);
            DisableCombine();
            break;
        case GL_MODULATE:
            texApplyBox->SetSelection(TEXTURE_APPLICATION_METHOD_MODULATE);
            DisableCombine();
            break;
        case GL_DECAL:
            texApplyBox->SetSelection(TEXTURE_APPLICATION_METHOD_DECAL);
            DisableCombine();
            break;
        case GL_BLEND:
            texApplyBox->SetSelection(TEXTURE_APPLICATION_METHOD_BLEND);
            DisableCombine();
            break;
        case GL_ADD:
            texApplyBox->SetSelection(TEXTURE_APPLICATION_METHOD_ADD);
            DisableCombine();
            break;
        case GL_COMBINE:
            texApplyBox->SetSelection(TEXTURE_APPLICATION_METHOD_COMBINE);
            EnableCombine();
            break;
        default:
            texApplyBox->SetSelection(TEXTURE_APPLICATION_METHOD_MODULATE);
            DisableCombine();
            break;
    }
    switch(texture->textureCombineMode){
        case GL_REPLACE:
            texCombineModeChoose->SetSelection(COMBINE_MODE_REPLACE);
            break;
        case GL_MODULATE:
            texCombineModeChoose->SetSelection(COMBINE_MODE_MODULATE);
            break;
        case GL_ADD:
            texCombineModeChoose->SetSelection(COMBINE_MODE_ADD);
            break;
        case GL_ADD_SIGNED:
            texCombineModeChoose->SetSelection(COMBINE_MODE_ADD_SIGNED);
            break;
        case GL_INTERPOLATE:
            texCombineModeChoose->SetSelection(COMBINE_MODE_INTERPOLATE);
            break;
        case GL_SUBTRACT:
            texCombineModeChoose->SetSelection(COMBINE_MODE_SUBTRACT);
            break;
        case GL_DOT3_RGB:
            texCombineModeChoose->SetSelection(COMBINE_MODE_DOT3_RGB);
            break;
        default:
            texCombineModeChoose->SetSelection(COMBINE_MODE_MODULATE);
    }
    switch(texture->textureCombineSource0){
        case GL_TEXTURE:
            texCombineSrc0RGBChoose->SetSelection(COMBINE_SOURCE_TEXTURE);
            break;
        case GL_CONSTANT:
            texCombineSrc0RGBChoose->SetSelection(COMBINE_SOURCE_CONSTANT);
            break;
        case GL_PRIMARY_COLOR:
            texCombineSrc0RGBChoose->SetSelection(COMBINE_SOURCE_PRIMARY_COLOR);
            break;
        case GL_PREVIOUS:
            texCombineSrc0RGBChoose->SetSelection(COMBINE_SOURCE_PREVIOUS);
            break;
        default:
            break;
    }
    switch(texture->textureCombineSource1){
        case GL_TEXTURE:
            texCombineSrc1RGBChoose->SetSelection(COMBINE_SOURCE_TEXTURE);
            break;
        case GL_CONSTANT:
            texCombineSrc1RGBChoose->SetSelection(COMBINE_SOURCE_CONSTANT);
            break;
        case GL_PRIMARY_COLOR:
            texCombineSrc1RGBChoose->SetSelection(COMBINE_SOURCE_PRIMARY_COLOR);
            break;
        case GL_PREVIOUS:
            texCombineSrc1RGBChoose->SetSelection(COMBINE_SOURCE_PREVIOUS);
            break;
        default:
            break;
    }
    switch(texture->textureCombineSource2){
        case GL_TEXTURE:
            texCombineSrc2RGBChoose->SetSelection(COMBINE_SOURCE_TEXTURE);
            break;
        case GL_CONSTANT:
            texCombineSrc2RGBChoose->SetSelection(COMBINE_SOURCE_CONSTANT);
            break;
        case GL_PRIMARY_COLOR:
            texCombineSrc2RGBChoose->SetSelection(COMBINE_SOURCE_PRIMARY_COLOR);
            break;
        case GL_PREVIOUS:
            texCombineSrc2RGBChoose->SetSelection(COMBINE_SOURCE_PREVIOUS);
            break;
        default:
            break;
    }
    switch(texture->textureCombineOperand0){
        case GL_SRC_COLOR:
            texCombineOperandArg0Choose->SetSelection(COMBINE_OPERAND_SRC_COLOR);
            break;
        case GL_ONE_MINUS_SRC_COLOR:
            texCombineOperandArg0Choose->SetSelection(COMBINE_OPERAND_ONE_MINUS_SRC_COLOR);
            break;
        default:
            break;
    }
    switch(texture->textureCombineOperand1){
        case GL_SRC_COLOR:
            texCombineOperandArg1Choose->SetSelection(COMBINE_OPERAND_SRC_COLOR);
            break;
        case GL_ONE_MINUS_SRC_COLOR:
            texCombineOperandArg1Choose->SetSelection(COMBINE_OPERAND_ONE_MINUS_SRC_COLOR);
            break;
        default:
            break;
    }
    switch(texture->textureCombineOperand2){
        case GL_SRC_COLOR:
            texCombineOperandArg2Choose->SetSelection(COMBINE_OPERAND_SRC_COLOR);
            break;
        case GL_ONE_MINUS_SRC_COLOR:
            texCombineOperandArg2Choose->SetSelection(COMBINE_OPERAND_ONE_MINUS_SRC_COLOR);
            break;
        default:
            break;
    }
    texEnvColorButton->SetBackgroundColour(ToWxIntColor(glState->GetTexture(textureBox->GetSelection())->texEnvColor));
}
