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
#include <QOpenGLFunctions>
#include <QTime>

#include "SGCanvas.h"
#include "SGCanvasMouseHandler.h"
#include "SGFrame.h"

static float StartZoom = 0.8f;

SGCanvasMouseHandler::SGCanvasMouseHandler(SGCanvas* canvas1)
  : m_canvas(canvas1)
{
    reset();
}

void
SGCanvasMouseHandler::reset()
{
    m_frames = 0.0f;

    m_startZoom = StartZoom;
    if (m_canvas) {
        m_canvas->SetZoom(m_startZoom);
    }
    stop();
    m_vPrev = QVector3D(0.0f, 0.0f, 0.0f);
    m_vInc = QVector3D(0.0f, 0.0f, 0.0f);
    m_validStart = false;
    m_xform.setToIdentity();
}

void
SGCanvasMouseHandler::stop()
{
    m_vInc = QVector3D(0.0f, 0.0f, 0.0f);
}

void
SGCanvasMouseHandler::onMousePress(QMouseEvent* event)
{
    QVector3D cursor = m_canvas->getWorldSpace(event->x(), event->y());

    if (event->buttons() & Qt::LeftButton) {
        if (!(event->modifiers() & Qt::ControlModifier)) {
            stop();
        }
        m_vStart = cursor;
        m_mStart = m_xform;
        m_startZoom = m_canvas->getZoom();
        m_vPrev = cursor;
        m_validStart = true;
    }
    // Right mouse button zooms.
    else if (event->buttons() & Qt::RightButton) {
        m_vStart = cursor;
        m_startZoom = m_canvas->getZoom();
        m_validStart = true;
    }
}

void
SGCanvasMouseHandler::onMouseMove(QMouseEvent* event)
{
    QVector3D cursor = m_canvas->getWorldSpace(event->x(), event->y());

    if (event->buttons() & Qt::LeftButton) {
        if (!m_validStart) {
            m_vInc = QVector3D(0.0f, 0.0f, 0.0f);
        } else {
            if (event->modifiers() & Qt::ControlModifier) {
                float delta = cursor.y() - m_vStart.y();
                if (delta) {
                    m_canvas->SetZoom(m_startZoom + delta);
                    m_canvas->updateGL();
                }
            } else {
                float theta = 180 * (cursor - m_vStart).length();
                if (theta) {
                    QVector3D axis = QVector3D::crossProduct(m_vStart, cursor);
                    axis.normalize();

                    glLoadIdentity();
                    glRotatef(-theta, axis.x(), axis.y(), axis.z());
                    glMultMatrixf(m_mStart.constData());
                    glGetFloatv(GL_MODELVIEW_MATRIX, m_xform.data());
                    m_canvas->updateGL();
                }
            }
            m_vInc = cursor - m_vPrev;
        }
        m_vPrev = cursor;
    } else if (event->buttons() & Qt::RightButton) {
        if (m_validStart) {
            float delta = cursor.y() - m_vStart.y();
            if (delta) {
                m_canvas->SetZoom(m_startZoom + delta);
                m_canvas->updateGL();
            }
        }
    }
}

void
SGCanvasMouseHandler::onMouseRelease(QMouseEvent*)
{
    m_validStart = false;
}

void
SGCanvasMouseHandler::loadMatrix() const
{
    glLoadIdentity();

    if (m_canvas->getFrame()->isPerspective()) {
        glTranslatef(0.0f, 0.0f, m_canvas->CameraZ - 1.0f);
    } else {
        glTranslatef(0.0f, 0.0f, m_canvas->CameraZ);
    }

    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    glMultMatrixf(m_xform.constData());
}

