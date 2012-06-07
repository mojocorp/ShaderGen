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

#pragma once

#include "Compulsory.h"
#include "Vector.h"

struct Light{
    bool  lightEnabled;

    float lightSpotExponent;
    float lightSpotCutoff;
    float lightConstantAttenuation;
    float lightLinearAttenuation;
    float lightQuadraticAttenuation;

    vec3 lightSpotDirectionVector;
    vec4 lightPositionVector;
    vec4 lightAmbientColorVector;
    vec4 lightDiffuseColorVector;
    vec4 lightSpecularColorVector;
};

struct Material{
    vec4 materialDiffuseColorVector;
    vec4 materialAmbientColorVector;
    vec4 materialSpecularColorVector;
    vec4 materialEmissionColorVector;
    float materialShininess;
    GLenum faceSelection;
};

struct Fog{
    float fogStart, fogEnd, fogDensity;
    vec4 fogColorVector;
    GLenum fogMode;
};

struct Texture{
    bool textureEnabled;
    
    bool texGen;

    vec4 eyePlaneCoeffS;
    vec4 eyePlaneCoeffT;

    vec4 objectPlaneCoeffS;
    vec4 objectPlaneCoeffT;

    vec4 texEnvColor;

    GLenum textureApplicationMethod;
    GLenum textureCoordinateGeneration;
    
    Id textureCurrentSelection;

    GLfloat textureCombineScale;

    GLenum textureCombineMode;

    GLenum textureCombineSource0;
    GLenum textureCombineSource1;
    GLenum textureCombineSource2;
    GLenum textureCombineOperand0;
    GLenum textureCombineOperand1;
    GLenum textureCombineOperand2;
};

//Wrapper
class SGFixedGLState{

public:
    SGFixedGLState() { Init() ;}
    ~SGFixedGLState();
    void Init();

    Light*    GetLight(int num){ return m_light[num] ;}
    Material* GetMaterial()    { return m_material   ;}
    Fog*      GetFog()         { return m_fog        ;}
    Texture * GetTexture(int num) {return m_texture[num] ;}

    bool GetLightingEnable()    const { return m_lightingEn  ;}
    bool GetFogEnable()         const { return m_fogEn       ;}
    bool GetNormalizeEnable()   const { return m_normalizeEn ;}
    bool GetSeparateSpecularColorEnable() const { return m_separateSpecularColorEn ;}
    bool Get2SidedLightingEnable() const { return m_2sidedLightingEn;}
    bool GetTextureEnable()     const { return m_textureEn;}

    void SetLightingEnable ( bool en ) { m_lightingEn  = en   ;}
    void SetFogEnable      ( bool en ) { m_fogEn       = en   ;}
    void SetNormalizeEnable( bool en ) { m_normalizeEn = en   ;}
    void SetSeparateSpecularColorEnable ( bool en ) { m_separateSpecularColorEn = en;}
    void Set2SidedLightingEnable( bool en ) { m_2sidedLightingEn = en ;}
    void SetTextureEnable  ( bool en ) { m_textureEn   = en   ;}
    GLenum GetLightEnum(int a) const { return GL_LIGHT0 + a; }
    GLenum GetTexEnum(int a) const { return GL_TEXTURE0 + a; }

    bool GetLightChanged()    { return m_changeLight  ;}
    bool GetMaterialChanged() { return m_changeMat    ;}
    bool GetFogChanged()      { return m_changeFog    ;}
    bool GetTextureChanged()  { return m_changeTexture;}

    bool GetTexGenEnable()    { return m_texGenEnable;}

    void SetLightChanged( bool en )    { m_changeLight  = en;}
    void SetMaterialChanged( bool en ) { m_changeMat    = en;}
    void SetFogChanged( bool en )      { m_changeFog    = en;}
    void SetTextureChanged( bool en )  { m_changeTexture= en;}

    void SetTexGenEnable( bool en ) { m_texGenEnable= en;}

private:
    void InitLight(int num);
    void InitMaterial();
    void InitFog();
    void InitTexture(int num);
    void Clean();

    bool m_changeLight,m_changeMat, m_changeFog, m_changeTexture;
    bool m_fogEn, m_lightingEn, m_normalizeEn, m_2sidedLightingEn, 
         m_textureEn, m_texGenEnable, m_separateSpecularColorEn;
    bool twosided;
    Light    *m_light[8];
    Texture  *m_texture[5];
    Material *m_material;
    Fog      *m_fog;

};
