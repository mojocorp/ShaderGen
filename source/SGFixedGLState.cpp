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

#include "SGFixedGLState.h"
#include "globals.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QOpenGLFunctions>

void
SGFixedGLState::init()
{
    for (int i = 0; i < NUM_LIGHTS; i++) {
        initLight(i);
    }

    for (int i = 0; i < NUM_LIGHTS_ENABLED_AT_START; i++) {
        m_light[i].enabled = true;
    }

    for (int i = 0; i < NUM_TEXTURES; i++) {
        initTexture(i);
    }

    initMaterial();
    initFog();

    setLightingEnable(true);
    setTwoSidedLightingEnable(false);
    setFogEnable(false);
    setSeparateSpecularColorEnable(false);
    setLocalViewerLightingEnable(false);

    setTextureEnable(false);
    setTexGenEnable(false);
    setNormalizeEnable(true);
    setRescaleNormalEnable(false);
}

void
SGFixedGLState::initLight(int num)
{
    Light& light = m_light[num];

    light.enabled = false;

    switch (num) {
        case 1:
            light.position = DEFAULT_LIGHT_POSITION_ONE;
            light.ambientColor = DEFAULT_LIGHT_AMBIENT_COLOR_ONE;
            light.diffuseColor = DEFAULT_LIGHT_DIFFUSE_COLOR_ONE;
            light.specularColor = DEFAULT_LIGHT_SPECULAR_COLOR_ONE;
            break;
        case 2:
            light.position = DEFAULT_LIGHT_POSITION_TWO;
            light.ambientColor = DEFAULT_LIGHT_AMBIENT_COLOR_TWO;
            light.diffuseColor = DEFAULT_LIGHT_DIFFUSE_COLOR_TWO;
            light.specularColor = DEFAULT_LIGHT_SPECULAR_COLOR_TWO;
            break;
        case 3:
            light.position = DEFAULT_LIGHT_POSITION_THREE;
            light.ambientColor = DEFAULT_LIGHT_AMBIENT_COLOR_THREE;
            light.diffuseColor = DEFAULT_LIGHT_DIFFUSE_COLOR_THREE;
            light.specularColor = DEFAULT_LIGHT_SPECULAR_COLOR_THREE;
            break;
        default:
            light.position = DEFAULT_LIGHT_POSITION_OTHER;
            light.ambientColor = DEFAULT_LIGHT_AMBIENT_COLOR_OTHER;
            light.diffuseColor = DEFAULT_LIGHT_DIFFUSE_COLOR_OTHER;
            light.specularColor = DEFAULT_LIGHT_SPECULAR_COLOR_OTHER;
            break;
    }
    light.spotDirection = DEFAULT_LIGHT_SPOT_DIRECTION;

    light.spotCutoff = DEFAULT_SPOT_CUT;
    light.spotExponent = DEFAULT_SPOT_EXP;
    light.constantAttenuation = DEFAULT_LIGHT_CONST_ATTEN;
    light.linearAttenuation = DEFAULT_LIGHT_LINEAR_ATTEN;
    light.quadraticAttenuation = DEFAULT_LIGHT_QUAD_ATTEN;
}

void
SGFixedGLState::initMaterial()
{
    m_material.ambientColor = DEFAULT_MATERIAL_AMBIENT_COLOR;
    m_material.diffuseColor = DEFAULT_MATERIAL_DIFFUSE_COLOR;
    m_material.specularColor = DEFAULT_MATERIAL_SPECULAR_COLOR;

    m_material.shininess = DEFAULT_MATERIAL_SHININESS;

    m_material.emissionColor = DEFAULT_MATERIAL_EMISSION_COLOR;
}

void
SGFixedGLState::initFog()
{
    m_fog.mode = GL_LINEAR;
    m_fog.source = GL_FRAGMENT_DEPTH;

    m_fog.density = DEFAULT_FOG_DENSITY;
    m_fog.start = DEFAULT_FOG_START;
    m_fog.end = DEFAULT_FOG_END;

    m_fog.color = DEFAULT_FOG_COLOR;
}

void
SGFixedGLState::initTexture(int num)
{
    Texture& texture = m_texture[num];

    texture.enabled = false;

    texture.texGen = false;
    texture.applicationMethod = GL_MODULATE;
    texture.coordinateGeneration = GL_OBJECT_LINEAR;
    texture.combineMode = GL_MODULATE;
    texture.combineScale = DEFAULT_COMBINE_RGB_SCALE;
    texture.combineSource0 = GL_TEXTURE;
    texture.combineSource1 = GL_PREVIOUS;
    texture.combineSource2 = GL_CONSTANT;
    texture.combineOperand0 = GL_SRC_COLOR;
    texture.combineOperand1 = GL_SRC_COLOR;
    texture.combineOperand2 = GL_SRC_COLOR;
    texture.currentSelection = num;
    texture.texEnvColor = DEFAULT_TEX_ENV_COLOR;
    texture.eyePlaneCoeffS = DEFAULT_EYE_PLANE_COEFF_S;
    texture.eyePlaneCoeffT = DEFAULT_EYE_PLANE_COEFF_T;
    texture.objectPlaneCoeffS = DEFAULT_OBJ_PLANE_COEFF_S;
    texture.objectPlaneCoeffT = DEFAULT_OBJ_PLANE_COEFF_T;
}

static QJsonArray
fromVector3D(const QVector3D& vec)
{
    QJsonArray array;
    array.append(vec.x());
    array.append(vec.y());
    array.append(vec.z());
    return array;
}

static QJsonArray
fromVector4D(const QVector4D& vec)
{
    QJsonArray array;
    array.append(vec.x());
    array.append(vec.y());
    array.append(vec.z());
    array.append(vec.w());
    return array;
}

static QVector3D
toVector3D(const QJsonValueRef& value)
{
    QJsonArray array = value.toArray();
    return { float(array[0].toDouble()), float(array[1].toDouble()), float(array[2].toDouble()) };
}

static QVector4D
toVector4D(const QJsonValueRef& value)
{
    QJsonArray array = value.toArray();
    return { float(array[0].toDouble()),
             float(array[1].toDouble()),
             float(array[2].toDouble()),
             float(array[3].toDouble()) };
}

void
SGFixedGLState::read(const QJsonObject& json)
{
    m_fogEnable = json["fogEnable"].toBool();
    m_lightingEnable = json["lightingEnable"].toBool();
    m_twoSidedLightingEnable = json["twoSidedLightingEnable"].toBool();
    m_localViewerLightingEnable = json["localViewerLightingEnable"].toBool();
    m_normalizeEnable = json["normalizeEnable"].toBool();
    m_rescaleNormalEnable = json["rescaleNormal"].toBool();
    m_twoSidedLightingEnable = json["twoSidedLightingEnable"].toBool();
    m_textureEnable = json["textureEnable"].toBool();
    m_texGenEnable = json["texGenEnable"].toBool();
    m_separateSpecularColorEnable = json["separateSpecularColorEnable"].toBool();

    QJsonArray lightArray = json["lights"].toArray();
    for (int i = 0; i < lightArray.size(); ++i) {
        QJsonObject lightObject = lightArray[i].toObject();
        Light& light = m_light[i];

        light.enabled = lightObject["enabled"].toBool();

        light.spotExponent = lightObject["spotExponent"].toDouble();
        light.spotCutoff = lightObject["spotCutoff"].toDouble();
        light.constantAttenuation = lightObject["constantAttenuation"].toDouble();
        light.linearAttenuation = lightObject["linearAttenuation"].toDouble();
        light.quadraticAttenuation = lightObject["quadraticAttenuation"].toDouble();

        light.spotDirection = toVector3D(lightObject["spotDirectionVector"]);
        light.position = toVector4D(lightObject["positionVector"]);
        light.ambientColor.setNamedColor(lightObject["ambientColorVector"].toString());
        light.diffuseColor.setNamedColor(lightObject["diffuseColorVector"].toString());
        light.specularColor.setNamedColor(lightObject["specularColorVector"].toString());
    }

    QJsonArray textureArray = json["textures"].toArray();
    for (int i = 0; i < textureArray.size(); ++i) {
        QJsonObject textureObject = textureArray[i].toObject();
        Texture& texture = m_texture[i];
        texture.enabled = textureObject["textureEnabled"].toBool();
        texture.texGen = textureObject["texGen"].toBool();
        texture.eyePlaneCoeffS = toVector4D(textureObject["eyePlaneCoeffS"]);
        texture.eyePlaneCoeffT = toVector4D(textureObject["eyePlaneCoeffT"]);
        texture.objectPlaneCoeffS = toVector4D(textureObject["objectPlaneCoeffS"]);
        texture.objectPlaneCoeffT = toVector4D(textureObject["objectPlaneCoeffT"]);
        texture.texEnvColor.setNamedColor(textureObject["texEnvColor"].toString());
        texture.applicationMethod = textureObject["applicationMethod"].toInt();
        texture.coordinateGeneration = textureObject["coordinateGeneration"].toInt();
        texture.currentSelection = textureObject["currentSelection"].toInt();
        texture.combineScale = textureObject["combineScale"].toDouble();
        texture.combineMode = textureObject["combineMode"].toInt();
        texture.combineSource0 = textureObject["combineSource0"].toInt();
        texture.combineSource1 = textureObject["combineSource1"].toInt();
        texture.combineSource2 = textureObject["combineSource2"].toInt();
        texture.combineOperand0 = textureObject["combineOperand0"].toInt();
        texture.combineOperand1 = textureObject["combineOperand1"].toInt();
        texture.combineOperand2 = textureObject["combineOperand2"].toInt();
    }

    QJsonObject materialObject = json["material"].toObject();
    m_material.diffuseColor.setNamedColor(materialObject["diffuseColor"].toString());
    m_material.ambientColor.setNamedColor(materialObject["ambientColor"].toString());
    m_material.specularColor.setNamedColor(materialObject["specularColor"].toString());
    m_material.emissionColor.setNamedColor(materialObject["emissionColor"].toString());
    m_material.shininess = materialObject["shininess"].toDouble();
    m_material.faceSelection = materialObject["faceSelection"].toInt();

    QJsonObject fogObject = json["fog"].toObject();
    m_fog.start = fogObject["start"].toDouble();
    m_fog.end = fogObject["end"].toDouble();
    m_fog.density = fogObject["density"].toDouble();
    m_fog.color.setNamedColor(fogObject["colorVector"].toString());
    m_fog.mode = fogObject["mode"].toInt();
    m_fog.source = fogObject["source"].toInt();
}

void
SGFixedGLState::write(QJsonObject& json) const
{
    json["fogEnable"] = m_fogEnable;
    json["lightingEnable"] = m_lightingEnable;
    json["twoSidedLightingEnable"] = m_twoSidedLightingEnable;
    json["localViewerLightingEnable"] = m_localViewerLightingEnable;
    json["normalizeEnable"] = m_normalizeEnable;
    json["rescaleNormalEnable"] = m_rescaleNormalEnable;
    json["twoSidedLightingEnable"] = m_twoSidedLightingEnable,
    json["textureEnable"] = m_textureEnable;
    json["texGenEnable"] = m_texGenEnable;
    json["separateSpecularColorEnable"] = m_separateSpecularColorEnable;

    QJsonArray lightArray;
    for (const auto& light : m_light) {
        QJsonObject lightObject;
        lightObject["enabled"] = light.enabled;

        lightObject["spotExponent"] = light.spotExponent;
        lightObject["spotCutoff"] = light.spotCutoff;
        lightObject["constantAttenuation"] = light.constantAttenuation;
        lightObject["linearAttenuation"] = light.linearAttenuation;
        lightObject["quadraticAttenuation"] = light.quadraticAttenuation;

        lightObject["spotDirectionVector"] = fromVector3D(light.spotDirection);
        lightObject["positionVector"] = fromVector4D(light.position);
        lightObject["ambientColorVector"] = light.ambientColor.name();
        lightObject["diffuseColorVector"] = light.diffuseColor.name();
        lightObject["specularColorVector"] = light.specularColor.name();
        lightArray.append(lightObject);
    }
    json["lights"] = lightArray;

    QJsonArray textureArray;
    for (const auto& texture : m_texture) {
        QJsonObject textureObject;
        textureObject["textureEnabled"] = texture.enabled;
        textureObject["texGen"] = texture.texGen;
        textureObject["eyePlaneCoeffS"] = fromVector4D(texture.eyePlaneCoeffS);
        textureObject["eyePlaneCoeffT"] = fromVector4D(texture.eyePlaneCoeffT);
        textureObject["objectPlaneCoeffS"] = fromVector4D(texture.objectPlaneCoeffS);
        textureObject["objectPlaneCoeffT"] = fromVector4D(texture.objectPlaneCoeffT);
        textureObject["texEnvColor"] = texture.texEnvColor.name();
        textureObject["applicationMethod"] = (int)texture.applicationMethod;
        textureObject["coordinateGeneration"] = (int)texture.coordinateGeneration;
        textureObject["currentSelection"] = texture.currentSelection;
        textureObject["combineScale"] = texture.combineScale;
        textureObject["combineMode"] = (int)texture.combineMode;
        textureObject["combineSource0"] = (int)texture.combineSource0;
        textureObject["combineSource1"] = (int)texture.combineSource1;
        textureObject["combineSource2"] = (int)texture.combineSource2;
        textureObject["combineOperand0"] = (int)texture.combineOperand0;
        textureObject["combineOperand1"] = (int)texture.combineOperand1;
        textureObject["combineOperand2"] = (int)texture.combineOperand2;
        textureArray.append(textureObject);
    }
    json["textures"] = textureArray;

    QJsonObject materialObject;
    materialObject["diffuseColor"] = m_material.diffuseColor.name();
    materialObject["ambientColor"] = m_material.ambientColor.name();
    materialObject["specularColor"] = m_material.specularColor.name();
    materialObject["emissionColor"] = m_material.emissionColor.name();
    materialObject["shininess"] = m_material.shininess;
    materialObject["faceSelection"] = (int)m_material.faceSelection;
    json["material"] = materialObject;

    QJsonObject fogObject;
    fogObject["start"] = m_fog.start;
    fogObject["end"] = m_fog.end;
    fogObject["density"] = m_fog.density;
    fogObject["colorVector"] = m_fog.color.name();
    fogObject["mode"] = (int)m_fog.mode;
    fogObject["source"] = (int)m_fog.source;
    json["fog"] = fogObject;
}
