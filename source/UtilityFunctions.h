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

#include <GL/glew.h>

#include <QString>
#include <QColor>
#include <QStringList>

#include "Vector.h"
#include "globals.h"

class QWidget;

#define PrintOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(const char *file, int line);

void IncorrectFormat(const QString& str, QWidget *errorWindow);


/******************************************************
The following two functions are used to convert color
    data back and forth between OpenGL and wxWidgets.
    Since wxWidgets uses integers from 0 to 255 to define
    colors, and OpenGL uses floats from 0.0 to 1.0, these
    conversions are neeeded.  Since the wxColour type
    doesn't give us an alpha value, we are unable to
    get this from the color selection dialog box.  For
    this reason, the alpha value is passed as 1.0 for
    the OpenGL floating point color representation.
******************************************************/

inline const QColor ToQtColor( const vec4& fpColor)
{
    return QColor::fromRgbF(fpColor.x, fpColor.y, fpColor.z);
}

inline const vec4 ToGLFPColor(const QColor& color)
{
    return vec4(color.redF(), color.greenF(), color.blueF(), 1.0f);
}

inline QString FloatToString3(const vec3& v) { return QString().sprintf("%.1f,%.1f,%.1f",v.x,v.y,v.z); }
inline QString FloatToString4(const vec4& v) { return QString().sprintf("%.1f,%.1f,%.1f,%.1f",v.x,v.y,v.z,v.w); }

inline QStringList parseVector(QString userEntry)
{
    return userEntry.split(",");
}
