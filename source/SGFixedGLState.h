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
#include <QJsonObject>
#include <QVector3D>
#include <QVector4D>

#include "globals.h"

struct Light
{
    bool enabled;

    float spotExponent;
    float spotCutoff;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;

    QVector3D spotDirection;
    QVector4D position;
    QColor ambientColor;
    QColor diffuseColor;
    QColor specularColor;
};

struct Material
{
    QColor diffuseColor;
    QColor ambientColor;
    QColor specularColor;
    QColor emissionColor;
    float shininess;
    int faceSelection;
};

struct Fog
{
    float start, end, density;
    QColor color;
    int mode;
    int source;
};

struct Texture
{
    bool enabled;

    bool texGen;

    QVector4D eyePlaneCoeffS;
    QVector4D eyePlaneCoeffT;

    QVector4D objectPlaneCoeffS;
    QVector4D objectPlaneCoeffT;

    QColor texEnvColor;

    int applicationMethod;
    int coordinateGeneration;

    int currentSelection;

    float combineScale;

    int combineMode;

    int combineSource0;
    int combineSource1;
    int combineSource2;
    int combineOperand0;
    int combineOperand1;
    int combineOperand2;
};

// Wrapper
class SGFixedGLState
{

  public:
    SGFixedGLState() { init(); }
    ~SGFixedGLState() = default;
    void init();

    Light& getLight(int num) { return m_light[num]; }
    Material& getMaterial() { return m_material; }
    Fog& getFog() { return m_fog; }
    Texture& getTexture(int num) { return m_texture[num]; }

    bool getLightingEnable() const { return m_lightingEnable; }
    bool getTwoSidedLightingEnable() const { return m_twoSidedLightingEnable; }
    bool getLocalViewerLightingEnable() const { return m_localViewerLightingEnable; }
    bool getFogEnable() const { return m_fogEnable; }
    bool getNormalizeEnable() const { return m_normalizeEnable; }
    bool getRescaleNormalEnable() const { return m_rescaleNormalEnable; }
    bool getSeparateSpecularColorEnable() const { return m_separateSpecularColorEnable; }
    bool getTextureEnable() const { return m_textureEnable; }

    void setLightingEnable(bool en) { m_lightingEnable = en; }
    void setTwoSidedLightingEnable(bool en) { m_twoSidedLightingEnable = en; }
    void setLocalViewerLightingEnable(bool en) { m_localViewerLightingEnable = en; }
    void setFogEnable(bool en) { m_fogEnable = en; }
    void setNormalizeEnable(bool en) { m_normalizeEnable = en; }
    void setRescaleNormalEnable(bool en) { m_rescaleNormalEnable = en; }
    void setSeparateSpecularColorEnable(bool en) { m_separateSpecularColorEnable = en; }
    void setTextureEnable(bool en) { m_textureEnable = en; }

    bool getTexGenEnable() const { return m_texGenEnable; }

    void setTexGenEnable(bool en) { m_texGenEnable = en; }

    void read(const QJsonObject& json);
    void write(QJsonObject& json) const;

  private:
    void initLight(int num);
    void initMaterial();
    void initFog();
    void initTexture(int num);

    bool m_fogEnable, m_lightingEnable, m_normalizeEnable, m_twoSidedLightingEnable,
      m_textureEnable, m_texGenEnable, m_separateSpecularColorEnable, m_rescaleNormalEnable,
      m_localViewerLightingEnable;
    Light m_light[NUM_LIGHTS];
    Texture m_texture[NUM_TEXTURES];
    Material m_material;
    Fog m_fog;
};
