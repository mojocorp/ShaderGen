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

#pragma once

#include "Compulsory.h"

class SGOglNotebook;

class SGOglTextureEnvNBPage: public wxPanel
{
public:
    SGOglTextureEnvNBPage(SGOglNotebook*  parent, wxWindowID id);

    void OnRadioTexApply(wxCommandEvent& event);
    void OnRadioTextureNum(wxCommandEvent& event);
    void OnCheckbox(wxCommandEvent &event);


    void OnChoiceTextureChoose(wxCommandEvent &event);
    void OnChoiceTextureCombineMode(wxCommandEvent &event);
    void OnChoiceTextureCombineOperandArg0(wxCommandEvent &event);
    void OnChoiceTextureCombineOperandArg1(wxCommandEvent &event);
    void OnChoiceTextureCombineOperandArg2(wxCommandEvent &event);
    void OnChoiceTextureCombineScale(wxCommandEvent &event);
    void OnChoiceTextureCombineSrc0RGB(wxCommandEvent &event);
    void OnChoiceTextureCombineSrc1RGB(wxCommandEvent &event);
    void OnChoiceTextureCombineSrc2RGB(wxCommandEvent &event);
    
    //void OnChoice(wxCommandEvent &event);

    void OnButton(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()

private:

    enum TextureApplicationMethod {
        TEXTURE_APPLICATION_METHOD_REPLACE = 0,
        TEXTURE_APPLICATION_METHOD_MODULATE,
        TEXTURE_APPLICATION_METHOD_DECAL,
        TEXTURE_APPLICATION_METHOD_BLEND,
        TEXTURE_APPLICATION_METHOD_ADD,
        TEXTURE_APPLICATION_METHOD_COMBINE
    };

    enum TextureCombineMode {
        COMBINE_MODE_REPLACE = 0,
        COMBINE_MODE_MODULATE,
        COMBINE_MODE_ADD,
        COMBINE_MODE_ADD_SIGNED,
        COMBINE_MODE_INTERPOLATE,
        COMBINE_MODE_SUBTRACT,
        COMBINE_MODE_DOT3_RGB,
        COMBINE_MODE_DOT3_RGBA
    };

    enum TextureCombineSource {
        COMBINE_SOURCE_TEXTURE = 0,
        COMBINE_SOURCE_CONSTANT,
        COMBINE_SOURCE_PRIMARY_COLOR,
        COMBINE_SOURCE_PREVIOUS
    };

    enum TextureCombineOperand {
        COMBINE_OPERAND_SRC_COLOR = 0,
        COMBINE_OPERAND_ONE_MINUS_SRC_COLOR,
        COMBINE_OPERAND_SRC_ALPHA,
        COMBINE_OPERAND_ONE_MINUS_SRC_ALPHA
    };

    enum TextureCombineScale {
        COMBINE_SCALE_1_0 = 0,
        COMBINE_SCALE_2_0,
        COMBINE_SCALE_4_0
    };

    void DisableCombine();
    void EnableCombine();
    void UpdateWidgets();

    SGOglNotebook *m_parent;

    wxRadioBox *texApplyBox, *textureBox;
    
    wxCheckBox *texEnable, *tex0CheckBox, *tex1CheckBox, *tex2CheckBox,
               *tex3CheckBox, *tex4CheckBox, *texEnableCheckBox;
    
    wxChoice   *texChoose, *texCombineModeChoose, *texCombineScaleChoose,
               *texCombineSrc0RGBChoose, *texCombineSrc1RGBChoose, 
               *texCombineSrc2RGBChoose, *texCombineOperandArg0Choose,
               *texCombineOperandArg1Choose, *texCombineOperandArg2Choose;   

    wxButton *texEnvColorButton;
};