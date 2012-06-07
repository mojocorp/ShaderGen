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

static const float pi = 3.14159265358979323846f;
static const float twopi = 6.28318530717958647692f;
static const float sqrt2 = 1.4142135623730950488016887242097f;

struct ivec2 {
    ivec2() : x(0), y(0) {}
    ivec2(int x, int y) : x(x), y(y) {}
    int x, y;
};

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

struct ivec3 {
    ivec3() : x(0), y(0), z(0) {}
    ivec3(int x, int y, int z) : x(x), y(y), z(z) {}
    int x, y, z;
};

struct vec3 {
    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3 operator-(const vec3& v) { return vec3(x - v.x, y - v.y, z - v.z); }
    vec3 operator+(const vec3& v) { return vec3(x + v.x, y + v.y, z + v.z); }
    vec3 operator-() const { return vec3(-x, -y, -z); }
    vec3 operator/(float f) const { return vec3(x / f, y / f, z / f); }
    vec3 operator*(float f) const { return vec3(x * f, y * f, z * f); }
    void operator*=(float scale) { x *= scale; y *= scale; z *= scale; }
    void operator/=(float f) { x /= f; y /= f; z /= f; }
    float magnitude() const;
    void unitize();
    operator float*() {return &x; }
    float x, y, z;
};

struct vec4 {
    vec4() : x(0), y(0), z(0), w(0) {}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    vec4 operator-(const vec4& v) { return vec4(x-v.x, y-v.y, z-v.z, w-v.w); }
    vec4 operator+(const vec4& v) { return vec4(x+v.x, y+v.y, z+v.z, w+v.w); }
    vec4 operator-() const { return vec4(-x, -y, -z, -w); }
    vec4 operator/(float f) const { return vec4(x / f, y / f, z / f, w / f); }
    vec4 operator*(float f) const { return vec4(x * f, y * f, z * f, w * f); }
    void operator*=(float scale) { x *= scale; y *= scale; z *= scale; w *= scale; }
    void operator/=(float f) { x /= f; y /= f; z /= f; w /= f; }
    float magnitude() const;
    void unitize();
    operator float*() { return &x; }
    float x, y, z, w;
};

struct mat2 { float data[4]; };

struct mat3 { float data[9]; };

struct mat4 {
    float data[16];
    void identity();
};

mat4 operator*(const mat4& a, const mat4& b);

void glVertex(const vec2& v);
void glVertex(const vec3& v);
void glNormal(const vec3& v);
void glColor(const vec3& v);
void glTexCoord(const vec2& v);
void glMultiTexCoord(const vec2& v, GLint u);
void glTranslate(const vec3& v);

vec3 blend(const vec3& v, const mat4& m0, const mat4& m1, float w0, float w1);
vec3 cross(const vec3& a, const vec3& b);
vec2 operator*(const vec2& v, const mat4& m);
vec3 operator*(const vec3& v, const mat4& m);
