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
#include "Globals.h"
#include <wx/tokenzr.h>

#define PrintOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line);

void IncorrectFormat(wxString str, wxWindow& errorWindow);


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

static const wxColour ToWxIntColor( const vec4 fpColor)
{
    unsigned char r = (unsigned char) ( fpColor.x * 255);
    unsigned char g = (unsigned char) ( fpColor.y * 255);
    unsigned char b = (unsigned char) ( fpColor.z * 255);
    return wxColour(r, g, b);
}

static const vec4 ToGLFPColor(const wxColour color)
{
    float r = (float)color.Red()  /255.0f;
    float g = (float)color.Green()/255.0f;
    float b = (float)color.Blue() /255.0f;
    float a = 1.0f;
    return vec4(r, g, b,a);
}

static wxString FloatToString3(const float *v) { return wxString::Format("%.1f,%.1f,%.1f",v[0],v[1],v[2]); }
static wxString FloatToString4(const float *v) { return wxString::Format("%.1f,%.1f,%.1f,%.1f",v[0],v[1],v[2],v[3]); }
static wxString FloatToString1(const float v) { return wxString::Format("%.1f",v); }

static wxString DoubleToString1(const double v) { return wxString::Format("%.6f",v); }

static const wxArrayString parseVector(wxString userEntry)
{
    wxStringTokenizer vecTokenizer(userEntry, ",");
    wxArrayString noCommas;
    while(vecTokenizer.HasMoreTokens()){
        wxString currToken = vecTokenizer.GetNextToken();
        noCommas.Add(currToken, 1);
    }
    return noCommas;
}