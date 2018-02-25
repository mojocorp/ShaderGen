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

const int NUM_TEXTURES = 5;

// default values for lights
const int NUM_LIGHTS_ENABLED_AT_START = 3;
const int NUM_LIGHTS = 8;

const QVector3D DEFAULT_LIGHT_SPOT_DIRECTION = QVector3D(0.0f, 0.0f, -1.0f);

const float DEFAULT_SPOT_CUT = 180.0f;
const float DEFAULT_SPOT_EXP = 0.0f;
const float DEFAULT_LIGHT_CONST_ATTEN = 1.0f;
const float DEFAULT_LIGHT_LINEAR_ATTEN = 0.0f;
const float DEFAULT_LIGHT_QUAD_ATTEN = 0.0f;

const QVector4D DEFAULT_LIGHT_POSITION_ONE = QVector4D(3.0f, -10.0f, 0.0f, 1.0f);
const QColor DEFAULT_LIGHT_AMBIENT_COLOR_ONE = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
const QColor DEFAULT_LIGHT_DIFFUSE_COLOR_ONE = QColor::fromRgbF(1.0f, 0.0f, 0.0f, 1.0f);
const QColor DEFAULT_LIGHT_SPECULAR_COLOR_ONE = QColor::fromRgbF(0.5f, 0.5f, 0.5f, 1.0f);

const QVector4D DEFAULT_LIGHT_POSITION_TWO = QVector4D(-3.0f, -10.0f, 0.0f, 0.0f);
const QColor DEFAULT_LIGHT_AMBIENT_COLOR_TWO = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
const QColor DEFAULT_LIGHT_DIFFUSE_COLOR_TWO = QColor::fromRgbF(0.0f, 0.0f, 1.0f, 1.0f);
const QColor DEFAULT_LIGHT_SPECULAR_COLOR_TWO = QColor::fromRgbF(0.0f, 1.0f, 0.0f, 1.0f);

const QVector4D DEFAULT_LIGHT_POSITION_THREE = QVector4D(10.0f, 0.0f, 1.0f, 1.0f);
const QColor DEFAULT_LIGHT_AMBIENT_COLOR_THREE = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
const QColor DEFAULT_LIGHT_DIFFUSE_COLOR_THREE = QColor::fromRgbF(0.0f, 1.0f, 0.0f, 1.0f);
const QColor DEFAULT_LIGHT_SPECULAR_COLOR_THREE = QColor::fromRgbF(0.0f, 1.0f, 0.0f, 1.0f);

const QVector4D DEFAULT_LIGHT_POSITION_OTHER = QVector4D(3.0f, 10.0f, 0.0f, 1.0f);
const QColor DEFAULT_LIGHT_AMBIENT_COLOR_OTHER = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
const QColor DEFAULT_LIGHT_DIFFUSE_COLOR_OTHER = QColor::fromRgbF(1.0f, 1.0f, 0.0f, 1.0f);
const QColor DEFAULT_LIGHT_SPECULAR_COLOR_OTHER = QColor::fromRgbF(0.5f, 0.5f, 0.5f, 1.0f);

// default values for materials
const float DEFAULT_MATERIAL_SHININESS = 30.0f;

const QColor DEFAULT_MATERIAL_AMBIENT_COLOR = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);
const QColor DEFAULT_MATERIAL_DIFFUSE_COLOR = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 1.0f);
const QColor DEFAULT_MATERIAL_SPECULAR_COLOR = QColor::fromRgbF(0.3f, 0.3f, 0.3f, 1.0f);
const QColor DEFAULT_MATERIAL_EMISSION_COLOR = QColor::fromRgbF(0.3f, 0.3f, 0.3f, 1.0f);

// default values for fog
const float DEFAULT_FOG_DENSITY = 0.0f;
const float DEFAULT_FOG_START = 5.0f;
const float DEFAULT_FOG_END = 7.0f;

const QColor DEFAULT_FOG_COLOR = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f);

// default values for textures
const float DEFAULT_COMBINE_RGB_SCALE = 1.0f;

const int NUM_TEXTURE_COORDS = 4; // s,t,r(p),q

const QColor DEFAULT_TEX_ENV_COLOR = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 1.0f);
const QVector4D DEFAULT_EYE_PLANE_COEFF_S = QVector4D(1.0f, 0.0f, 0.0f, 0.0f);
const QVector4D DEFAULT_EYE_PLANE_COEFF_T = QVector4D(0.0f, 1.0f, 0.0f, 0.0f);
const QVector4D DEFAULT_OBJ_PLANE_COEFF_S = QVector4D(1.0f, 0.0f, 0.0f, 0.0f);
const QVector4D DEFAULT_OBJ_PLANE_COEFF_T = QVector4D(0.0f, 1.0f, 0.0f, 0.0f);
