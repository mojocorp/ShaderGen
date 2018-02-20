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

#include <GL/glew.h>
#include <QMouseEvent>
#include <QTime>

#include "SGCanvas.h"
#include "SGCanvasMouseHandler.h"
#include "SGFrame.h"
#include "UtilityFunctions.h"

const float SGCanvasMouseHandler::StartZoom = 0.8f;
const float SGCanvasMouseHandler::InertiaThreshold = 1.0f;
const int SGCanvasMouseHandler::Delay = 10;

SGCanvasMouseHandler::SGCanvasMouseHandler()
{
    canvas = 0;
    reset();
}

SGCanvasMouseHandler::SGCanvasMouseHandler(SGCanvas* canvas1)
{
    canvas = canvas1;
    reset();
}

void
SGCanvasMouseHandler::reset()
{
    frames = 0.0f;

    startZoom = StartZoom;
    if (canvas) {
        canvas->SetZoom(startZoom);
    }
    stop();
    vPrev = QVector3D(0.0f, 0.0f, 0.0f);
    vInc = QVector3D(0.0f, 0.0f, 0.0f);
    validStart = false;
    xform.setToIdentity();
}

void
SGCanvasMouseHandler::stop()
{
    vInc = QVector3D(0.0f, 0.0f, 0.0f);
}

void
SGCanvasMouseHandler::onMousePress(QMouseEvent* event)
{
    QVector3D cursor = canvas->getWorldSpace(event->x(), event->y());

    if (event->buttons() & Qt::LeftButton) {
        if (!(event->modifiers() & Qt::ControlModifier)) {
            stop();
        }
        vStart = cursor;
        mStart = xform;
        startZoom = canvas->getZoom();
        vPrev = cursor;
        validStart = true;
    }
    // Right mouse button zooms.
    else if (event->buttons() & Qt::RightButton) {
        vStart = cursor;
        startZoom = canvas->getZoom();
        validStart = true;
    }
}

void
SGCanvasMouseHandler::onMouseMove(QMouseEvent* event)
{
    QVector3D cursor = canvas->getWorldSpace(event->x(), event->y());

    if (event->buttons() & Qt::LeftButton) {
        if (!validStart) {
            vInc = QVector3D(0.0f, 0.0f, 0.0f);
        } else {
            if (event->modifiers() & Qt::ControlModifier) {
                float delta = cursor.y() - vStart.y();
                if (delta) {
                    canvas->SetZoom(startZoom + delta);
                    canvas->updateGL();
                }
            } else {
                float theta = 180 * (cursor - vStart).length();
                if (theta) {
                    QVector3D axis = QVector3D::crossProduct(vStart, cursor);
                    axis.normalize();

                    glLoadIdentity();
                    glRotatef(-theta, axis.x(), axis.y(), axis.z());
                    glMultMatrix(mStart);
                    glGet(GL_MODELVIEW_MATRIX, xform);
                    canvas->updateGL();
                }
            }
            vInc = cursor - vPrev;
        }
        vPrev = cursor;
    } else if (event->buttons() & Qt::RightButton) {
        if (validStart) {
            float delta = cursor.y() - vStart.y();
            if (delta) {
                canvas->SetZoom(startZoom + delta);
                canvas->updateGL();
            }
        }
    }
}

void
SGCanvasMouseHandler::onMouseRelease(QMouseEvent*)
{
    validStart = false;
}

void
SGCanvasMouseHandler::loadMatrix() const
{
    glLoadIdentity();

    if (canvas->getFrame()->isPerspective()) {
        glTranslatef(0.0f, 0.0f, SGCanvas::CameraZ - 1.0f);
    } else {
        glTranslatef(0.0f, 0.0f, SGCanvas::CameraZ);
    }

    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    multMatrix();
}

void
SGCanvasMouseHandler::multMatrix() const
{
    glMultMatrix(xform);
}
