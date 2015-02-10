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

#include <QColor>
#include <QVector3D>
#include <QVector4D>
#include <QJsonObject>

#include "globals.h"
#include "SGTextures.h"

struct Light{
    bool  lightEnabled;

    float lightSpotExponent;
    float lightSpotCutoff;
    float lightConstantAttenuation;
    float lightLinearAttenuation;
    float lightQuadraticAttenuation;

    QVector3D lightSpotDirectionVector;
    QVector4D lightPositionVector;
    QColor lightAmbientColorVector;
    QColor lightDiffuseColorVector;
    QColor lightSpecularColorVector;
};

struct Material{
    QColor materialDiffuseColorVector;
    QColor materialAmbientColorVector;
    QColor materialSpecularColorVector;
    QColor materialEmissionColorVector;
    float materialShininess;
    GLenum faceSelection;
};

struct Fog{
    float fogStart, fogEnd, fogDensity;
    QColor fogColorVector;
    GLenum fogMode;
};

struct Texture{
    bool textureEnabled;
    
    bool texGen;

    QVector4D eyePlaneCoeffS;
    QVector4D eyePlaneCoeffT;

    QVector4D objectPlaneCoeffS;
    QVector4D objectPlaneCoeffT;

    QColor texEnvColor;

    GLenum textureApplicationMethod;
    GLenum textureCoordinateGeneration;
    
    SGTextures::TextureId textureCurrentSelection;

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

    Light&    GetLight(int num){ return m_light[num] ;}
    Material& GetMaterial()    { return m_material   ;}
    Fog&      GetFog()         { return m_fog        ;}
    Texture&  GetTexture(int num) {return m_texture[num] ;}

    bool GetLightingEnable()    const { return m_lightingEnable  ;}
    bool GetFogEnable()         const { return m_fogEnable       ;}
    bool GetNormalizeEnable()   const { return m_normalizeEnable ;}
    bool GetSeparateSpecularColorEnable() const { return m_separateSpecularColorEnable ;}
    bool Get2SidedLightingEnable() const { return m_2sidedLightingEnable;}
    bool GetTextureEnable()     const { return m_textureEnable;}

    void SetLightingEnable ( bool en ) { m_lightingEnable  = en   ;}
    void SetFogEnable      ( bool en ) { m_fogEnable       = en   ;}
    void SetNormalizeEnable( bool en ) { m_normalizeEnable = en   ;}
    void SetSeparateSpecularColorEnable ( bool en ) { m_separateSpecularColorEnable = en;}
    void Set2SidedLightingEnable( bool en ) { m_2sidedLightingEnable = en ;}
    void SetTextureEnable  ( bool en ) { m_textureEnable   = en   ;}
    GLenum GetLightEnum(int a) const { return GL_LIGHT0 + a; }
    GLenum GetTexEnum(int a) const { return GL_TEXTURE0 + a; }

    bool GetTexGenEnable()    { return m_texGenEnable;}

    void SetTexGenEnable( bool en ) { m_texGenEnable= en;}

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
private:
    void InitLight(int num);
    void InitMaterial();
    void InitFog();
    void InitTexture(int num);

    bool m_fogEnable, m_lightingEnable, m_normalizeEnable, m_2sidedLightingEnable,
    m_textureEnable, m_texGenEnable, m_separateSpecularColorEnable;
    Light    m_light[NUM_LIGHTS];
    Texture  m_texture[NUM_TEXTURES];
    Material m_material;
    Fog      m_fog;

};
