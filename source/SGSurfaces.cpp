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
#include "globals.h"

#include <QOpenGLFunctions>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

static const float TWO_PI = 6.28318530717958647692f;

TParametricSurface::TParametricSurface()
  : m_slices(100)
{
}

TParametricSurface::~TParametricSurface()
{
}

void
TParametricSurface::generate()
{
    const int stacks = m_slices / 2;
    m_du = 1.0f / (float)(m_slices - 1);
    m_dv = 1.0f / (float)(stacks - 1);

    m_normals.reserve(2 * m_slices * stacks);
    m_texCoords.reserve(2 * m_slices * stacks);
    m_vertices.reserve(2 * m_slices * stacks);

    for (int i = 0; i < m_slices; i++) {
        const float u = i * m_du;
        m_flipped = flip(QVector2D(u, 0));

        for (int j = 0; j < stacks; j++) {
            const float v = j * m_dv;
            QVector3D normal, p0;

            QVector2D domain = m_flipped ? QVector2D(u + m_du, v) : QVector2D(u, v);
            vertex(domain, normal, p0);
            m_normals.push_back(normal);
            m_texCoords.push_back(domain);
            m_vertices.push_back(p0);

            domain = m_flipped ? QVector2D(u, v) : QVector2D(u + m_du, v);
            vertex(domain, normal, p0);
            m_normals.push_back(normal);
            m_texCoords.push_back(domain);
            m_vertices.push_back(p0);
        }
    }
}

// Draw a parametric surface.  'slices' is the tesselation factor.  Returns the
// number of vertices.
void
TParametricSurface::draw(bool isNormalize)
{
    initializeOpenGLFunctions();

    if (isNormalize) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(QVector3D), m_normals.data());
    }

    for (int i = 0; i < NUM_TEXTURES; i++) {
        glClientActiveTexture(GL_TEXTURE0 + i);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, sizeof(QVector2D), m_texCoords.data());
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(QVector3D), m_vertices.data());

    for (int i = 0; i < m_slices; i++) {
        glDrawArrays(GL_QUAD_STRIP, i * m_slices, m_slices);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    for (int i = 0; i < NUM_TEXTURES; i++) {
        glClientActiveTexture(GL_TEXTURE0 + i);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

// Send out a normal, texture coordinate, vertex coordinate, and an optional
// custom attribute.
void
TParametricSurface::vertex(QVector2D& domain, QVector3D& normal, QVector3D& p0) const
{
    QVector3D p1, p2, p3;
    const float u = domain.x();
    const float v = domain.y();

    eval(domain, p0);
    QVector2D z1(u + m_du / 2, v);
    eval(z1, p1);
    QVector2D z2(u + m_du / 2 + m_du, v);
    eval(z2, p3);

    if (m_flipped) {
        QVector2D z3(u + m_du / 2, v - m_dv);
        eval(z3, p2);
    } else {
        QVector2D z4(u + m_du / 2, v + m_dv);
        eval(z4, p2);
    }

    normal = QVector3D::crossProduct(p3 - p1, p2 - p1);
    if (normal.length() < 0.00001f) {
        normal = p0;
    }
    normal.normalize();
}

void
TKlein::eval(QVector2D& domain, QVector3D& range) const
{
    const float u = (1 - domain.x()) * TWO_PI;
    const float v = domain.y() * TWO_PI;

    const float x0 = 3 * cosf(u) * (1 + sinf(u)) + (2 * (1 - cosf(u) / 2)) * cosf(u) * cosf(v);
    const float y0 = 8 * sinf(u) + (2 * (1 - cosf(u) / 2)) * sinf(u) * cosf(v);

    const float x1 = 3 * cosf(u) * (1 + sinf(u)) + (2 * (1 - cosf(u) / 2)) * cosf(v + M_PI);
    const float y1 = 8 * sinf(u);

    range.setX(u < M_PI ? x0 : x1);
    range.setY(u < M_PI ? y0 : y1);
    range.setZ((2 * (1 - cosf(u) / 2)) * sinf(v));
    range = range / 10;
    range.setY(-range.y());

    // Tweak the texture coordinates.
    domain.setX(domain.x() * 4);
}

// Flip the normals along a segment of the Klein bottle so that we don't need
// two-sided lighting.
bool
TKlein::flip(const QVector2D& domain) const
{
    return (domain.x() < .125);
}

void
TTrefoil::eval(QVector2D& domain, QVector3D& range) const
{
    const float a = 0.5f;
    const float b = 0.3f;
    const float c = 0.5f;
    const float d = 0.1f;
    const float u = (1 - domain.x()) * TWO_PI * 2;
    const float v = domain.y() * TWO_PI;

    const float r = a + b * cosf(1.5f * u);
    const float x = r * cosf(u);
    const float y = r * sinf(u);
    const float z = c * sinf(1.5f * u);

    QVector3D dv;
    dv.setX(-1.5f * b * sinf(1.5f * u) * cosf(u) - (a + b * cosf(1.5f * u)) * sinf(u));
    dv.setY(-1.5f * b * sinf(1.5f * u) * sinf(u) + (a + b * cosf(1.5f * u)) * cosf(u));
    dv.setZ(1.5f * c * cosf(1.5f * u));

    QVector3D q = dv;
    q.normalize();
    QVector3D qvn(q.y(), -q.x(), 0);
    qvn.normalize();
    QVector3D ww = QVector3D::crossProduct(q, qvn);

    range.setX(x + d * (qvn.x() * cosf(v) + ww.x() * sinf(v)));
    range.setY(y + d * (qvn.y() * cosf(v) + ww.y() * sinf(v)));
    range.setZ(z + d * ww.z() * sinf(v));

    // Tweak the texture coordinates.
    domain.setX(domain.x() * 20);
    domain /= 3;
}

void
TConic::eval(QVector2D& domain, QVector3D& range) const
{
    const float a = 0.2f;
    const float b = 1.5f;
    const float c = 0.1f;
    const float n = 2;

    const float u = domain.x() * TWO_PI;
    const float v = domain.y() * TWO_PI;

    range.setX(a * (1 - v / TWO_PI) * cosf(n * v) * (1 + cosf(u)) + c * cosf(n * v));
    range.setZ(a * (1 - v / TWO_PI) * sinf(n * v) * (1 + cosf(u)) + c * sinf(n * v));
    range.setY(b * v / TWO_PI + a * (1 - v / TWO_PI) * sinf(u) - 0.7f);
    range *= 1.25;
    range.setY(range.y() + 0.125);

    // Tweak the texture coordinates.
    domain.setY(domain.y() * 4);
}

void
TTorus::eval(QVector2D& domain, QVector3D& range) const
{
    const float major = 0.8f;
    const float minor = 0.2f;
    const float u = domain.x() * TWO_PI;
    const float v = domain.y() * TWO_PI;

    range.setX((major + minor * cosf(v)) * cosf(u));
    range.setY((major + minor * cosf(v)) * sinf(u));
    range.setZ(minor * sinf(v));

    // Tweak the texture coordinates.
    domain.setX(domain.x() * 4);
}

void
TSphere::eval(QVector2D& domain, QVector3D& range) const
{
    const float radius = 1;
    const float u = std::abs(domain.y() * M_PI);
    const float v = std::abs(domain.x() * TWO_PI);

    range.setX(radius * cosf(v) * sinf(u));
    range.setZ(radius * sinf(v) * sinf(u));
    range.setY(radius * cosf(u));

    domain.setY(1 - domain.y());
    domain.setX(1 - domain.x());
}

void
TPlane::eval(QVector2D& domain, QVector3D& range) const
{
    if (m_z < 0) {
        range.setX(-m_width * (domain.x() - 0.5f));
        range.setY(m_width * (domain.y() - 0.5f));
    } else {
        range.setX(m_width * (domain.x() - 0.5f));
        range.setY(m_width * (domain.y() - 0.5f));
    }
    range.setZ(m_z);
}
