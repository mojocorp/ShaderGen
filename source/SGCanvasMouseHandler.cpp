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

#include <QMouseEvent>
#include <QTime>

#include "SGCanvas.h"
#include "SGCanvasMouseHandler.h"
#include "SGFrame.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

QVector3D
sphereSheetProjector(const QVector2D& pos)
{
    QVector3D rayOrigin(pos.x() - 0.5f, 0.5f - pos.y(), -0.5f);
    const QVector3D rayDirection(0, 0, 1);
    const QVector3D sphereOrigin(0, 0, 0);
    const float sphereRadius = 0.35f;

    // ray / sphere intersection
    const QVector3D r_to_s = rayOrigin - sphereOrigin;

    // Compute A, B and C coefficients
    const float A = rayDirection.lengthSquared();
    const float B = 2.0f * QVector3D::dotProduct(r_to_s, rayDirection);
    const float C = r_to_s.lengthSquared() - sphereRadius * sphereRadius;

    // Find discriminant
    const float disc = B * B - 4.0 * A * C;

    // if discriminant is negative there are no real roots
    if (disc >= 0.0) {
        const float t0 = (-B + std::sqrt(disc)) / (2.0 * A);
        if (t0 > 0) {
            rayOrigin = rayOrigin + t0 * rayDirection;
        }
    }

    // intersection with the sheet
    const QVector3D planeHit =
      rayOrigin + rayDirection * rayOrigin.distanceToPlane(sphereOrigin, -rayDirection);

    // distance from plane hit point to plane center in the projector
    const float planarDist = (planeHit - sphereOrigin).length();

    // let sphere and hyperbolic sheet meet at 45°
    const float meetDist = sphereRadius * (float)std::cos(M_PI / 4.0);

    if (planarDist < meetDist)
        return rayOrigin;

    // By Pythagoras' we know that the value of the sphere at 45°
    // angle from the groundplane will be (radius^2 * 0.5).
    const float v = (sphereRadius * sphereRadius) * 0.5f;

    // A hyperbolic function is given by y = 1 / x, where x in our
    // case is the "radial" distance from the plane centerpoint to the
    // plane intersection point.
    const float hyperbval = (1.0f / planarDist) * v;

    return planeHit + QVector3D(0.0f, 0.0f, hyperbval);
}

SGCanvasMouseHandler::SGCanvasMouseHandler(SGCanvas* canvas1)
  : m_canvas(canvas1)
{
    m_zoom = 0.5f;
}

void
SGCanvasMouseHandler::onMousePress(QMouseEvent* event)
{
    m_vStart = event->pos();
    m_mStart = m_xform;
    m_startZoom = m_zoom;
}

void
SGCanvasMouseHandler::onMouseMove(QMouseEvent* event)
{
    if ((event->buttons() & Qt::RightButton) ||
        ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ControlModifier))) {
        const QVector3D lstart = m_canvas->getWorldSpace(m_vStart.x(), m_vStart.y());
        const QVector3D lend = m_canvas->getWorldSpace(event->x(), event->y());
        const float delta = lend.y() - lstart.y();
        if (delta) {
            m_zoom = m_startZoom + delta;
        }
    } else {
        if (event->buttons() & Qt::LeftButton) {
            const QVector3D lstart =
              sphereSheetProjector(m_canvas->getNormalizedPosition(m_vStart));
            const QVector3D lend =
              sphereSheetProjector(m_canvas->getNormalizedPosition(event->pos()));
            const QQuaternion rotation = QQuaternion::rotationTo(lstart, lend);

            m_xform.setToIdentity();
            m_xform.rotate(rotation);
            m_xform *= m_mStart;
        }
    }
}
