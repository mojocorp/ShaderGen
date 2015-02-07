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

#include "globals.h"
#include "SGFixedGLState.h"

void SGFixedGLState::Init()
{
    for(int i=0; i< NUM_LIGHTS; i++)
    {
        InitLight(i);
    }

    for(int i=0; i<NUM_LIGHTS_ENABLED_AT_START; i++)
    {
        m_light[i].lightEnabled = true;
    }

    for(int i=0; i<NUM_TEXTURES; i++)
    {
        InitTexture(i);
    }

    InitMaterial();
    InitFog();

    SetLightingEnable(true);
    SetFogEnable(false);
    SetSeparateSpecularColorEnable(false);
    Set2SidedLightingEnable(false);
    
    SetTextureEnable(false);
    SetTexGenEnable(false);
    SetNormalizeEnable(true);
}

SGFixedGLState::~SGFixedGLState()
{

}

void SGFixedGLState::InitLight(int num)
{
    m_light[num].lightEnabled = false;

    switch(num){
    case 1:
        m_light[num].lightPositionVector = DEFAULT_LIGHT_POSITION_ONE;
        m_light[num].lightAmbientColorVector = DEFAULT_LIGHT_AMBIENT_COLOR_ONE;
        m_light[num].lightDiffuseColorVector = DEFAULT_LIGHT_DIFFUSE_COLOR_ONE;
        m_light[num].lightSpecularColorVector = DEFAULT_LIGHT_SPECULAR_COLOR_ONE;
        break;
    case 2:
        m_light[num].lightPositionVector = DEFAULT_LIGHT_POSITION_TWO;
        m_light[num].lightAmbientColorVector  = DEFAULT_LIGHT_AMBIENT_COLOR_TWO;
        m_light[num].lightDiffuseColorVector  = DEFAULT_LIGHT_DIFFUSE_COLOR_TWO;
        m_light[num].lightSpecularColorVector = DEFAULT_LIGHT_SPECULAR_COLOR_TWO;
        break;
    case 3:
        m_light[num].lightPositionVector = DEFAULT_LIGHT_POSITION_THREE;
        m_light[num].lightAmbientColorVector  = DEFAULT_LIGHT_AMBIENT_COLOR_THREE;
        m_light[num].lightDiffuseColorVector  = DEFAULT_LIGHT_DIFFUSE_COLOR_THREE;
        m_light[num].lightSpecularColorVector = DEFAULT_LIGHT_SPECULAR_COLOR_THREE;
        break;
    default:
        m_light[num].lightPositionVector = DEFAULT_LIGHT_POSITION_OTHER;
        m_light[num].lightAmbientColorVector  = DEFAULT_LIGHT_AMBIENT_COLOR_OTHER;
        m_light[num].lightDiffuseColorVector  = DEFAULT_LIGHT_DIFFUSE_COLOR_OTHER;
        m_light[num].lightSpecularColorVector = DEFAULT_LIGHT_SPECULAR_COLOR_OTHER;
        break;
    }
    m_light[num].lightSpotDirectionVector = DEFAULT_LIGHT_SPOT_DIRECTION;
    
    m_light[num].lightSpotCutoff = DEFAULT_SPOT_CUT;
    m_light[num].lightSpotExponent = DEFAULT_SPOT_EXP;
    m_light[num].lightConstantAttenuation = DEFAULT_LIGHT_CONST_ATTEN;
    m_light[num].lightLinearAttenuation = DEFAULT_LIGHT_LINEAR_ATTEN;
    m_light[num].lightQuadraticAttenuation = DEFAULT_LIGHT_QUAD_ATTEN;
}

void SGFixedGLState::InitMaterial()
{
    m_material.materialAmbientColorVector = DEFAULT_MATERIAL_AMBIENT_COLOR;
    m_material.materialDiffuseColorVector = DEFAULT_MATERIAL_DIFFUSE_COLOR;
    m_material.materialSpecularColorVector = DEFAULT_MATERIAL_SPECULAR_COLOR;

    m_material.materialShininess = DEFAULT_MATERIAL_SHININESS;

    m_material.materialEmissionColorVector = DEFAULT_MATERIAL_EMISSION_COLOR;
}

void SGFixedGLState::InitFog()
{
    m_fog.fogMode    = GL_LINEAR;

    m_fog.fogDensity = DEFAULT_FOG_DENSITY;
    m_fog.fogStart   = DEFAULT_FOG_START;
    m_fog.fogEnd     = DEFAULT_FOG_END;

    m_fog.fogColorVector = DEFAULT_FOG_COLOR;
}

void SGFixedGLState::InitTexture(int num)
{
    m_texture[num].textureEnabled = false;

    m_texture[num].texGen = false;
    m_texture[num].textureApplicationMethod = GL_MODULATE;
    m_texture[num].textureCoordinateGeneration = GL_OBJECT_LINEAR;
    m_texture[num].textureCombineMode = GL_MODULATE;
    m_texture[num].textureCombineScale = DEFAULT_COMBINE_RGB_SCALE;
    m_texture[num].textureCombineSource0 = GL_TEXTURE;
    m_texture[num].textureCombineSource1 = GL_PREVIOUS;
    m_texture[num].textureCombineSource2 = GL_CONSTANT;
    m_texture[num].textureCombineOperand0 = GL_SRC_COLOR;
    m_texture[num].textureCombineOperand1 = GL_SRC_COLOR;
    m_texture[num].textureCombineOperand2 = GL_SRC_COLOR;
    m_texture[num].textureCurrentSelection = (SGTextures::TextureId)num;
    m_texture[num].texEnvColor = DEFAULT_TEX_ENV_COLOR;
    m_texture[num].eyePlaneCoeffS = DEFAULT_EYE_PLANE_COEFF_S;
    m_texture[num].eyePlaneCoeffT = DEFAULT_EYE_PLANE_COEFF_T;
    m_texture[num].objectPlaneCoeffS = DEFAULT_OBJ_PLANE_COEFF_S;
    m_texture[num].objectPlaneCoeffT = DEFAULT_OBJ_PLANE_COEFF_T;
}
