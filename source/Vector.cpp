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

#include <math.h>
#include <string.h>

#include <GL/glew.h>
#include "Vector.h"

// Basic vector math.  This is by no means complete; add to this file as necessary.

float vec3::length() const
{
    return sqrtf(x*x + y*y + z*z);
}

void vec3::normalize()
{
    float m = length();
    x /= m;
    y /= m;
    z /= m;
}

vec3 cross(const vec3& u, const vec3& v)
{
    return vec3(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}

vec3 operator*(const vec3& v, const mat4& m)
{
    float x = m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z  + m.data[12];
    float y = m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z  + m.data[13];
    float z = m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14];
    return vec3(x, y, z);
}

void mat4::identity()
{
    memset(data, 0, sizeof(data));
    data[0] = 1;
    data[5] = 1;
    data[10] = 1;
    data[15] = 1;
}

void glVertex(const vec3& v)
{
    glVertex3fv((float*) &v);
}

void glNormal(const vec3& v)
{
    glNormal3fv((float*) &v);
}

void glMultiTexCoord(const vec2& v, const GLint u)
{
    glMultiTexCoord2f(u, v.x, v.y);
}
