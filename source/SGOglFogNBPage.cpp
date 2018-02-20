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

#include "QColorButton.h"
#include <QBoxLayout>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>

#include "SGFixedGLState.h"
#include "SGOglFogNBPage.h"
#include "UtilityFunctions.h"

SGOglFogNBPage::SGOglFogNBPage(SGFixedGLState* glState, QWidget* parent)
  : QWidget(parent)
  , m_glState(glState)
{
    QGridLayout* fogSizer = new QGridLayout(this);

    QRadioButton* fogLinear = new QRadioButton(tr("GL_LINEAR"), this);
    QRadioButton* fogExp = new QRadioButton(tr("GL_EXP"), this);
    QRadioButton* fogExp2 = new QRadioButton(tr("GL_EXP2"), this);

    QGroupBox* fogModeChoiceBox = new QGroupBox(tr("Select Fog Mode"), this);
    QVBoxLayout* fogModeChoiceSizer = new QVBoxLayout(fogModeChoiceBox);

    fogModeChoiceSizer->addWidget(fogLinear);
    fogModeChoiceSizer->addWidget(fogExp);
    fogModeChoiceSizer->addWidget(fogExp2);

    m_fogModeChoice = new QButtonGroup(this);
    m_fogModeChoice->addButton(fogLinear, 0);
    m_fogModeChoice->addButton(fogExp, 1);
    m_fogModeChoice->addButton(fogExp2, 2);
    connect(m_fogModeChoice, SIGNAL(buttonClicked(int)), SLOT(onRadio(int)));

    GLenum aa = glState->getFog().fogMode;
    switch (aa) {
        case GL_LINEAR:
            aa = 0;
            break;
        case GL_EXP:
            aa = 1;
            break;
        case GL_EXP2:
            aa = 2;
            break;
        default:
            aa = 1;
            break;
    }

    m_fogModeChoice->button(aa)->setChecked(true);

    m_fogDensity = new QDoubleSpinBox(this);
    m_fogDensity->setRange(0, 100);

    m_fogStart = new QDoubleSpinBox(this);
    m_fogStart->setRange(-1000, 1000);

    m_fogEnd = new QDoubleSpinBox(this);
    m_fogEnd->setRange(-1000, 1000);

    connect(m_fogDensity, SIGNAL(valueChanged(double)), SLOT(fogDensityChanged(double)));
    connect(m_fogStart, SIGNAL(valueChanged(double)), SLOT(fogDensityChanged(double)));
    connect(m_fogEnd, SIGNAL(valueChanged(double)), SLOT(fogDensityChanged(double)));

    m_fogColor = new QColorButton(this);
    connect(m_fogColor, SIGNAL(selected(QColor)), SLOT(fogColorChanged(QColor)));

    QLabel* fogDensityLbl = new QLabel(tr("GL_FOG_DENSITY"), this);
    QLabel* fogStartLbl = new QLabel(tr("GL_FOG_START"), this);
    QLabel* fogEndLbl = new QLabel(tr("GL_FOG_END"), this);
    QLabel* fogColorLbl = new QLabel(tr("GL_FOG_COLOR"), this);

    m_fogCheckBox = new QCheckBox(tr("GL_FOG_ENABLE"), this);
    m_fogCheckBox->setChecked(glState->getFogEnable());
    connect(m_fogCheckBox, SIGNAL(clicked(bool)), SLOT(onCheckbox()));

    fogSizer->addWidget(m_fogCheckBox, 0, 0);

    fogSizer->addWidget(fogModeChoiceBox, 1, 0, 3, 1);

    fogSizer->addWidget(fogDensityLbl, 1, 1);
    fogSizer->addWidget(fogStartLbl, 2, 1);
    fogSizer->addWidget(fogEndLbl, 3, 1);

    fogSizer->addWidget(m_fogDensity, 1, 2);
    fogSizer->addWidget(m_fogStart, 2, 2);
    fogSizer->addWidget(m_fogEnd, 3, 2);

    fogSizer->addWidget(fogColorLbl, 1, 3);

    fogSizer->addWidget(m_fogColor, 1, 4);
    fogSizer->setRowStretch(4, 2);
    fogSizer->setColumnStretch(5, 2);

    setup();
}

void
SGOglFogNBPage::setup()
{
    const Fog& fog = m_glState->getFog();
    m_fogDensity->setValue(fog.fogDensity);
    m_fogStart->setValue(fog.fogStart);
    m_fogEnd->setValue(fog.fogEnd);
    m_fogColor->setColor(fog.fogColorVector);
}

void
SGOglFogNBPage::fogColorChanged(const QColor& color)
{
    m_glState->getFog().fogColorVector = color;

    emit valueChanged();
}

void
SGOglFogNBPage::onCheckbox()
{
    m_glState->setFogEnable(m_fogCheckBox->isChecked());
    emit valueChanged();
}

void
SGOglFogNBPage::fogDensityChanged(double density)
{
    m_glState->getFog().fogDensity = density;
    emit valueChanged();
}

void
SGOglFogNBPage::fogStartChanged(double start)
{
    m_glState->getFog().fogStart = start;
    emit valueChanged();
}

void
SGOglFogNBPage::fogEndChanged(double end)
{
    m_glState->getFog().fogEnd = end;
    emit valueChanged();
}

void
SGOglFogNBPage::onRadio(int index)
{
    switch (index) {
        case 0:
            m_glState->getFog().fogMode = GL_LINEAR;
            break;
        case 1:
            m_glState->getFog().fogMode = GL_EXP;
            break;
        case 2:
            m_glState->getFog().fogMode = GL_EXP2;
            break;
        default:
            break;
    }
    emit valueChanged();
}
