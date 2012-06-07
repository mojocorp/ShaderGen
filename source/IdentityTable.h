/************************************************************************
* Author(s):    Inderaj Bains and Joshua Doss                           *
* Last Modified: 11/02/2005                                             *
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

struct Id {
    enum EId {
// Wx Mapped IDs
        FileExit =  wxID_EXIT,
        Help     =  wxID_HELP,

//Menu Items
        ViewPerspective= wxID_HIGHEST + 1,
        About,
        HelpAboutClose,
        ModalAbout,

//Mode for GL, Fixed or Shader
        GLModeChoiceVal,
        GLModeChoiceFixed,
        GLModeChoiceShader,

//The Text Window
        FFSD_START_SHADER,
        Compile,
        GenerateShaders,
        Link,
        Build,
        ClearLog,
        FFSD_END_SHADER,

//The Models
        ModelTorus,
        ModelPlane,
        ModelSphere,
        ModelConic,
        ModelTrefoil,
        ModelKlein,

//The textures
        TextureLogo,
        TextureLogoNormal,
        TextureRust,
        TextureLeopard,
        TextureEyeball,
        TextureCobblestone,
        TextureCobblestoneNormal,
        TextureBricks,
        TextureBricksNormal,
        TextureStonewall,
        TextureStonewallNormal,
        TextureMetalSheet,
        TextureMetalSheetNormal,

//OpenGL Notebook Pages Ids

//Textures
    FFSD_START_TEXTURE,
        TexCoordGen,
        TextureCoordUnit,
        TexApply,
        TextureNum,
        TexEnableVal,
        Tex0EnableVal,
        Tex1EnableVal,
        Tex2EnableVal,
        Tex3EnableVal,
        Tex4EnableVal,
        Tex0TexGenEnableVal,
        Tex1TexGenEnableVal,
        Tex2TexGenEnableVal,
        Tex3TexGenEnableVal,
        Tex4TexGenEnableVal,
        EyePlaneCoeffValS,
        EyePlaneCoeffValT,
        ObjectPlaneCoeffValS,
        ObjectPlaneCoeffValT,
        TextureChoose,
        TextureCombineMode,
        TextureCombineScale,
        TextureCombineSrc0RGB,
        TextureCombineSrc1RGB,
        TextureCombineSrc2RGB,
        TextureCombineOperandArg0,
        TextureCombineOperandArg1,
        TextureCombineOperandArg2,
        TexEnvColor,
    FFSD_END_TEXTURE,

//Lighting
    FFSD_START_LIGHT,

    //spotLight
        SpotCutoffVal,
        SpotDirectionVal,
        SpotExponentVal,
    //attenuation
        AttenuationConstantVal,
        AttenuationLinearVal,
        AttenuationQuadraticVal,
    //Enable
        L0EnableVal,
        L1EnableVal,
        L2EnableVal,
        L3EnableVal,
        L4EnableVal,
        L5EnableVal,
        L6EnableVal,
        L7EnableVal,
        LightingEnableVal,
        NormalizeEnableVal,
        SeparateSpecularColorEnableVal,
    //Others
        LightSelectionBoxVal,
        LightPositionVal,
        TwoSidedLightVal,
        AmbientLightVal,
        SpecularLightVal,
        DiffuseLightVal,

    FFSD_END_LIGHT,


    //Material
    FFSD_START_MATERIAL,

        FaceSelectionBoxVal,
        AmbientMaterialVal,
        DiffuseMaterialVal,
        AmbientAndDiffuseMaterialVal,
        SpecularMaterialVal,
        ShininessMaterialVal,
        EmissionMaterialVal,
        ColorIndicesVal,  

    FFSD_END_MATERIAL,

    //Fog

    FFSD_START_FOG,
        FogDensityVal,
        FogStartVal,
        FogEndVal,
        FogIndexVal,
        FogColorVal,
        FogModeChoiceVal,
        FogEnableVal,
    FFSD_END_FOG,

    //Dependent Variables
        ModelsStart = ModelTorus,
        ModelsEnd   = ModelKlein,
        NumModels   = ModelsEnd - ModelsStart + 1,

        TexturesStart = TextureLogo,
        //TexturesEnd   = TextureLava,
        TexturesEnd = TextureMetalSheetNormal,
        NumTextures   = TexturesEnd - TexturesStart + 1
    };


  union {
        EId name;
        int value;
    };

    Id(EId name) : name(name) {}
    Id(int value) : value(value) {}
    Id() : value(0) {}
    operator EId() const { return name; }
    void operator++() { ++value; }
    void operator++(int) { value++; }
};
