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

// Abstract base class representing a parametric surface.
class TParametricSurface
{
  public:
    int Draw(int slices);
    virtual void Eval(vec2& domain, vec3& range) = 0;
    virtual void Vertex(vec2 domain, GLboolean isNormalize);
    virtual bool Flip(const vec2& domain) { return false; }
    virtual int CustomAttributeLocation() { return -1; }
    virtual float CustomAttributeValue(const vec2& domain) { return 0; }
  protected:
    bool flipped;
    float du, dv;
};

class TSphere : public TParametricSurface
{
  public:
    void Eval(vec2& domain, vec3& range);
};

class TTorus : public TParametricSurface
{
  public:
    void Eval(vec2& domain, vec3& range);
};

class TConic : public TParametricSurface
{
  public:
    void Eval(vec2& domain, vec3& range);
};

class TTrefoil : public TParametricSurface
{
  public:
    void Eval(vec2& domain, vec3& range);
};

class TKlein : public TParametricSurface
{
  public:
    void Eval(vec2& domain, vec3& range);
    bool Flip(const vec2& domain);
};

class TPlane : public TParametricSurface
{
  public:
    TPlane(float z = 0, float width = 2) : z(z), width(width) {}
    void Eval(vec2& domain, vec3& range);
  protected:
    float z, width;
};
