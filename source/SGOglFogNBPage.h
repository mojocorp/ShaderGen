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

#include <QButtonGroup>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QWidget>

class SGFixedGLState;
class QColorButton;

class SGOglFogNBPage : public QWidget
{
    Q_OBJECT
  public:
    SGOglFogNBPage(SGFixedGLState* m_glState, QWidget* parent);

    void setup();
  signals:
    void valueChanged();
  protected slots:
    void onCheckbox();
    void fogColorChanged(const QColor& color);
    void fogDensityChanged(double);
    void fogStartChanged(double);
    void fogEndChanged(double);
    void fogModeChanged(int index);
    void fogSourceChanged(int index);

  private:
    SGFixedGLState* m_glState;

    QButtonGroup* m_fogSourceChoice;
    QButtonGroup* m_fogModeChoice;
    QColorButton* m_fogColor;
    QDoubleSpinBox* m_fogDensity;
    QDoubleSpinBox* m_fogStart;
    QDoubleSpinBox* m_fogEnd;
    QCheckBox* m_fogCheckBox;
};
