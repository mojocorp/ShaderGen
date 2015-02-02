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

#include "SGSurfaces.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "UtilityFunctions.h"

// Draw a parametric surface.  'slices' is the tesselation factor.  Returns the number of vertices.
int TParametricSurface::Draw(int slices)
{
    int totalVerts = 0;
    int stacks = slices / 2;
    du = 1.0f / (float) (slices-1);
    dv = 1.0f / (float) (stacks-1);

    bool isNormalize = glIsEnabled(GL_NORMALIZE);

    for (int i=0; i<slices; i++)
    {
        float u = i * du;
        glBegin(GL_QUAD_STRIP);
        flipped = Flip(vec2(u,0));

        for (int j=0; j<stacks; j++)
        {
            float v = j * dv;
            vec3 normal, p0;
            vec2 domain = flipped ? vec2(u + du, v) : vec2(u, v);
            Vertex(domain, normal, p0, isNormalize);
            if (isNormalize)
                glNormal(normal);
            for(int i=0; i<5; i++)
            {
                glMultiTexCoord(domain, GL_TEXTURE0 + i);
            }
            glVertex(p0);
            domain =  flipped ? vec2(u, v) : vec2(u + du, v);
            Vertex(domain, normal, p0, isNormalize);
            if (isNormalize)
                glNormal(normal);
            for(int i=0; i<5; i++)
            {
                glMultiTexCoord(domain, GL_TEXTURE0 + i);
            }

            glVertex(p0);
            totalVerts += 2;
        }
        glEnd();
    }

    PrintOpenGLError();
    return totalVerts;
}

// Send out a normal, texture coordinate, vertex coordinate, and an optional custom attribute.
void TParametricSurface::Vertex(vec2& domain, vec3& normal, vec3& p0, bool isNormalize)
{
    vec3 p1, p2, p3;
    float u = domain.u;
    float v = domain.v;

    Eval(domain, p0);
    vec2 z1(u + du/2, v);
    Eval(z1, p1);
    vec2 z2(u + du/2 + du, v);
    Eval(z2, p3);

    if (flipped)
    {
        vec2 z3(u + du/2, v - dv);
        Eval(z3, p2);
    }
    else
    {
        vec2 z4(u + du/2, v + dv);
        Eval(z4, p2);
    }
    if(isNormalize)
    {
        normal = cross(p3 - p1, p2 - p1);
        if (normal.magnitude() < 0.00001f)
        {
            normal = p0;
        }
        normal.unitize();
    }
}

void TKlein::Eval(vec2& domain, vec3& range)
{
    float u = (1 - domain.u) * twopi;
    float v = domain.v * twopi;

    float x0 = 3 * cosf(u) * (1 + sinf(u)) + (2 * (1 - cosf(u) / 2)) * cosf(u) * cosf(v);
    float y0  = 8 * sinf(u) + (2 * (1 - cosf(u) / 2)) * sinf(u) * cosf(v);

    float x1 = 3 * cosf(u) * (1 + sinf(u)) + (2 * (1 - cosf(u) / 2)) * cosf(v + pi);
    float y1 = 8 * sinf(u);

    range.x = u < pi ? x0 : x1;
    range.y = u < pi ? y0 : y1;
    range.z = (2 * (1 - cosf(u) / 2)) * sinf(v);
    range = range / 10;
    range.y = -range.y;

    // Tweak the texture coordinates.
    domain.u *= 4;
}

// Flip the normals along a segment of the Klein bottle so that we don't need two-sided lighting.
bool TKlein::Flip(const vec2& domain)
{
    return (domain.u < .125);
}

void TTrefoil::Eval(vec2& domain, vec3& range)
{
    const float a = 0.5f;
    const float b = 0.3f;
    const float c = 0.5f;
    const float d = 0.1f;
    float u = (1 - domain.u) * twopi * 2;
    float v = domain.v * twopi;

    float r = a + b * cosf(1.5f * u);
    float x = r * cosf(u);
    float y = r * sinf(u);
    float z = c * sinf(1.5f * u);

    vec3 dv;
    dv.x = -1.5f * b * sinf(1.5f * u) * cosf(u) - (a + b * cosf(1.5f * u)) * sinf(u);
    dv.y = -1.5f * b * sinf(1.5f * u) * sinf(u) + (a + b * cosf(1.5f * u)) * cosf(u);
    dv.z = 1.5f * c * cosf(1.5f * u);

    vec3 q = dv; q.unitize();
    vec3 qvn(q.y, -q.x, 0);
    qvn.unitize();
    vec3 ww = cross(q,qvn);

    range.x = x + d * (qvn.x * cosf(v) + ww.x * sinf(v));
    range.y = y + d * (qvn.y * cosf(v) + ww.y * sinf(v));
    range.z = z + d * ww.z * sinf(v);

    // Tweak the texture coordinates.
    domain.u *= 20;
    domain /= 3;
}

void TConic::Eval(vec2& domain, vec3& range)
{
    const float a = 0.2f;
    const float b = 1.5f;
    const float c = 0.1f;
    const float n = 2;

    float u = domain.u * twopi;
    float v = domain.v * twopi;

    range.x = a * (1 - v / twopi) * cosf(n * v) * (1 + cosf(u)) + c * cosf(n * v);
    range.z = a * (1 - v / twopi) * sinf(n * v) * (1 + cosf(u)) + c * sinf(n * v);
    range.y = b * v / twopi + a * (1 - v / twopi) * sinf(u) - 0.7f;
    range *= 1.25;
    range.y += 0.125;

    // Tweak the texture coordinates.
    domain.v *= 4;
}

void TTorus::Eval(vec2& domain, vec3& range)
{
    const float major = 0.8f;
    const float minor = 0.2f;
    float u = domain.u * twopi;
    float v = domain.v * twopi;

    range.x = (major + minor * cosf(v)) * cosf(u);
    range.y = (major + minor * cosf(v)) * sinf(u);
    range.z = minor * sinf(v);

    // Tweak the texture coordinates.
    domain.u *= 4;
}

void TSphere::Eval(vec2& domain, vec3& range)
{
    const float radius = 1;
    float u = fabsf(domain.v * pi);
    float v = fabsf(domain.u * twopi);

    range.x = radius * cosf(v) * sinf(u);
    range.z = radius * sinf(v) * sinf(u);
    range.y = radius * cosf(u);

    domain.v = 1 - domain.v;
    domain.u = 1 - domain.u;
}

void TPlane::Eval(vec2& domain, vec3& range)
{
    if (z < 0)
    {
        range.x = -width * (domain.u - 0.5f);
        range.y = width * (domain.v - 0.5f);
    }
    else
    {
        range.x = width * (domain.u - 0.5f);
        range.y = width * (domain.v - 0.5f);
    }
    range.z = z;
}
