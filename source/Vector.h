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
/************************************************************************ 
* Basic vector math.  This is by no means complete; add to this file    *
*  as necessary.                                                        *
************************************************************************/

#pragma once

#include <GL/glew.h>

static const float pi = 3.14159265358979323846f;
static const float twopi = 6.28318530717958647692f;

struct vec2 {
    vec2() : x(0), y(0) {}
    vec2(float x, float y) : x(x), y(y) {}
    vec2(const float p[]) : x(p[0]), y(p[1]) {}
    vec2 operator/(float f) const { return vec2(x / f, y / f); }
    vec2 operator*(float scale) const { return vec2(x * scale, y * scale); }
    vec2 operator+(vec2 v) const { return vec2(v.x + x, v.y + y); }
    void operator*=(float scale) { x *= scale; y *= scale; }
    void operator/=(float scale) { x /= scale; y /= scale; }
    union { float x, u, s; };
    union { float y, v, t; };
    void flip() { float temp = x; x = y; y = temp; }
};

struct mat4 {
    float data[16];
    void identity();
};

void glMultiTexCoord(const vec2& v, GLint u);

