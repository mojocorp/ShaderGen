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

#include <QJsonObject>
#include <QJsonArray>
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

static QJsonArray fromVector3D(const QVector3D &vec) {
    QJsonArray array;
    array.append(vec.x());
    array.append(vec.y());
    array.append(vec.z());
    return array;
}

static QJsonArray fromVector4D(const QVector4D &vec) {
    QJsonArray array;
    array.append(vec.x());
    array.append(vec.y());
    array.append(vec.z());
    array.append(vec.w());
    return array;
}

static QVector3D toVector3D(const QJsonValueRef &value) {
    QJsonArray array = value.toArray();
    return QVector3D(array[0].toDouble(),
                     array[1].toDouble(),
                     array[2].toDouble());
}

static QVector4D toVector4D(const QJsonValueRef &value) {
    QJsonArray array = value.toArray();
    return QVector4D(array[0].toDouble(),
                     array[1].toDouble(),
                     array[2].toDouble(),
                     array[3].toDouble());
}

void SGFixedGLState::read(const QJsonObject &json)
{
    m_fogEnable = json["fogEnable"].toBool();
    m_lightingEnable = json["lightingEnable"].toBool();
    m_normalizeEnable = json["normalizeEnable"].toBool();
    m_2sidedLightingEnable = json["2sidedLightingEnable"].toBool();
    m_textureEnable = json["textureEnable"].toBool();
    m_texGenEnable = json["texGenEnable"].toBool();
    m_separateSpecularColorEnable = json["separateSpecularColorEnable"].toBool();

    QJsonArray lightArray = json["lights"].toArray();
    for (int i=0; i<lightArray.size(); ++i) {
        QJsonObject lightObject = lightArray[i].toObject();

        m_light[i].lightEnabled = lightObject["enabled"].toBool();

        m_light[i].lightSpotExponent = lightObject["spotExponent"].toDouble();
        m_light[i].lightSpotCutoff = lightObject["spotCutoff"].toDouble();
        m_light[i].lightConstantAttenuation = lightObject["constantAttenuation"].toDouble();
        m_light[i].lightLinearAttenuation = lightObject["linearAttenuation"].toDouble();
        m_light[i].lightQuadraticAttenuation = lightObject["quadraticAttenuation"].toDouble();

        m_light[i].lightSpotDirectionVector = toVector3D(lightObject["spotDirectionVector"]);
        m_light[i].lightPositionVector = toVector4D(lightObject["positionVector"]);
        m_light[i].lightAmbientColorVector.setNamedColor(lightObject["ambientColorVector"].toString());
        m_light[i].lightDiffuseColorVector.setNamedColor(lightObject["diffuseColorVector"].toString());
        m_light[i].lightSpecularColorVector.setNamedColor(lightObject["specularColorVector"].toString());
    }

    QJsonArray textureArray = json["textures"].toArray();
    for (int i=0; i<textureArray.size(); ++i) {
        QJsonObject textureObject = textureArray[i].toObject();
        m_texture[i].textureEnabled = textureObject["textureEnabled"].toBool();
        m_texture[i].texGen = textureObject["texGen"].toBool();
        m_texture[i].eyePlaneCoeffS = toVector4D(textureObject["eyePlaneCoeffS"]);
        m_texture[i].eyePlaneCoeffT = toVector4D(textureObject["eyePlaneCoeffT"]);
        m_texture[i].objectPlaneCoeffS = toVector4D(textureObject["objectPlaneCoeffS"]);
        m_texture[i].objectPlaneCoeffT = toVector4D(textureObject["objectPlaneCoeffT"]);
        m_texture[i].texEnvColor.setNamedColor(textureObject["texEnvColor"].toString());
        m_texture[i].textureApplicationMethod = textureObject["applicationMethod"].toInt();
        m_texture[i].textureCoordinateGeneration = textureObject["coordinateGeneration"].toInt();
        m_texture[i].textureCurrentSelection = (SGTextures::TextureId)textureObject["currentSelection"].toInt();
        m_texture[i].textureCombineScale = textureObject["combineScale"].toDouble();
        m_texture[i].textureCombineMode = textureObject["combineMode"].toInt();
        m_texture[i].textureCombineSource0 = textureObject["combineSource0"].toInt();
        m_texture[i].textureCombineSource1 = textureObject["combineSource1"].toInt();
        m_texture[i].textureCombineSource2 = textureObject["combineSource2"].toInt();
        m_texture[i].textureCombineOperand0 = textureObject["combineOperand0"].toInt();
        m_texture[i].textureCombineOperand1 = textureObject["combineOperand1"].toInt();
        m_texture[i].textureCombineOperand2 = textureObject["combineOperand2"].toInt();
    }

    QJsonObject materialObject = json["material"].toObject();
    m_material.materialDiffuseColorVector.setNamedColor(materialObject["diffuseColor"].toString());
    m_material.materialAmbientColorVector.setNamedColor(materialObject["ambientColor"].toString());
    m_material.materialSpecularColorVector.setNamedColor(materialObject["specularColor"].toString());
    m_material.materialEmissionColorVector.setNamedColor(materialObject["emissionColor"].toString());
    m_material.materialShininess = materialObject["shininess"].toDouble();
    m_material.faceSelection = materialObject["faceSelection"].toInt();

    QJsonObject fogObject = json["fog"].toObject();
    m_fog.fogStart = fogObject["start"].toDouble();
    m_fog.fogEnd = fogObject["end"].toDouble();
    m_fog.fogDensity = fogObject["density"].toDouble();
    m_fog.fogColorVector.setNamedColor(fogObject["colorVector"].toString());
    m_fog.fogMode = fogObject["mode"].toInt();
}

void SGFixedGLState::write(QJsonObject &json) const
{
    json["fogEnable"] = m_fogEnable;
    json["lightingEnable"] = m_lightingEnable;
    json["normalizeEnable"] = m_normalizeEnable;
    json["2sidedLightingEnable"] = m_2sidedLightingEnable,
    json["textureEnable"] = m_textureEnable;
    json["texGenEnable"] = m_texGenEnable;
    json["separateSpecularColorEnable"] = m_separateSpecularColorEnable;

    QJsonArray lightArray;
    for (int i=0; i<NUM_LIGHTS; i++) {
        QJsonObject lightObject;
        lightObject["enabled"] = m_light[i].lightEnabled;

        lightObject["spotExponent"] = m_light[i].lightSpotExponent;
        lightObject["spotCutoff"] = m_light[i].lightSpotCutoff;
        lightObject["constantAttenuation"] = m_light[i].lightConstantAttenuation;
        lightObject["linearAttenuation"] = m_light[i].lightLinearAttenuation;
        lightObject["quadraticAttenuation"] = m_light[i].lightQuadraticAttenuation;

        lightObject["spotDirectionVector"] = fromVector3D(m_light[i].lightSpotDirectionVector);
        lightObject["positionVector"] = fromVector4D(m_light[i].lightPositionVector);
        lightObject["ambientColorVector"] = m_light[i].lightAmbientColorVector.name();
        lightObject["diffuseColorVector"] = m_light[i].lightDiffuseColorVector.name();
        lightObject["specularColorVector"] = m_light[i].lightSpecularColorVector.name();
        lightArray.append(lightObject);
    }
    json["lights"] = lightArray;

    QJsonArray textureArray;
    for (int i=0; i<NUM_TEXTURES; i++) {
        QJsonObject textureObject;
        textureObject["textureEnabled"] = m_texture[i].textureEnabled;
        textureObject["texGen"] = m_texture[i].texGen;
        textureObject["eyePlaneCoeffS"] = fromVector4D(m_texture[i].eyePlaneCoeffS);
        textureObject["eyePlaneCoeffT"] = fromVector4D(m_texture[i].eyePlaneCoeffT);
        textureObject["objectPlaneCoeffS"] = fromVector4D(m_texture[i].objectPlaneCoeffS);
        textureObject["objectPlaneCoeffT"] = fromVector4D(m_texture[i].objectPlaneCoeffT);
        textureObject["texEnvColor"] = m_texture[i].texEnvColor.name();
        textureObject["applicationMethod"] = (int)m_texture[i].textureApplicationMethod;
        textureObject["coordinateGeneration"] = (int)m_texture[i].textureCoordinateGeneration;
        textureObject["currentSelection"] = m_texture[i].textureCurrentSelection;
        textureObject["combineScale"] = m_texture[i].textureCombineScale;
        textureObject["combineMode"] = (int)m_texture[i].textureCombineMode;
        textureObject["combineSource0"] = (int)m_texture[i].textureCombineSource0;
        textureObject["combineSource1"] = (int)m_texture[i].textureCombineSource1;
        textureObject["combineSource2"] = (int)m_texture[i].textureCombineSource2;
        textureObject["combineOperand0"] = (int)m_texture[i].textureCombineOperand0;
        textureObject["combineOperand1"] = (int)m_texture[i].textureCombineOperand1;
        textureObject["combineOperand2"] = (int)m_texture[i].textureCombineOperand2;
        textureArray.append(textureObject);
    }
    json["textures"] = textureArray;

    QJsonObject materialObject;
    materialObject["diffuseColor"] = m_material.materialDiffuseColorVector.name();
    materialObject["ambientColor"] = m_material.materialAmbientColorVector.name();
    materialObject["specularColor"] = m_material.materialSpecularColorVector.name();
    materialObject["emissionColor"] = m_material.materialEmissionColorVector.name();
    materialObject["shininess"] = m_material.materialShininess;
    materialObject["faceSelection"] = (int)m_material.faceSelection;
    json["material"] = materialObject;

    QJsonObject fogObject;
    fogObject["start"] = m_fog.fogStart;
    fogObject["end"] = m_fog.fogEnd;
    fogObject["density"] = m_fog.fogDensity;
    fogObject["colorVector"] = m_fog.fogColorVector.name();
    fogObject["mode"] = (int)m_fog.fogMode;
    json["fog"] = fogObject;
}
