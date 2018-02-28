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
#include <QOpenGLFunctions>
#include <QRadioButton>

#include "SGFixedGLState.h"
#include "SGOglFogNBPage.h"

SGOglFogNBPage::SGOglFogNBPage(SGFixedGLState* glState, QWidget* parent)
  : QWidget(parent)
  , m_glState(glState)
{
    QGridLayout* fogSizer = new QGridLayout(this);

    QRadioButton* fogCoord = new QRadioButton(tr("GL_FOG_COORD"), this);
    QRadioButton* fogFragmentDepth = new QRadioButton(tr("GL_FRAGMENT_DEPTH"), this);

    QGroupBox* fogSourceChoiceBox = new QGroupBox(tr("Select Source"), this);
    QVBoxLayout* fogSourceChoiceSizer = new QVBoxLayout(fogSourceChoiceBox);

    fogSourceChoiceSizer->addWidget(fogCoord);
    fogSourceChoiceSizer->addWidget(fogFragmentDepth);

    m_fogSourceChoice = new QButtonGroup(this);
    m_fogSourceChoice->addButton(fogCoord, 0);
    m_fogSourceChoice->addButton(fogFragmentDepth, 1);
    connect(m_fogSourceChoice, SIGNAL(buttonClicked(int)), SLOT(fogSourceChanged(int)));

    GLenum src = 0;
    switch (glState->getFog().fogSource) {
        case GL_FOG_COORD:
            src = 0;
            break;
        case GL_FRAGMENT_DEPTH:
            src = 1;
            break;
        default:
            src = 0;
            break;
    }

    m_fogSourceChoice->button(src)->setChecked(true);

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
    connect(m_fogModeChoice, SIGNAL(buttonClicked(int)), SLOT(fogModeChanged(int)));

    GLenum mode = 1;
    switch (glState->getFog().fogMode) {
        case GL_LINEAR:
            mode = 0;
            break;
        case GL_EXP:
            mode = 1;
            break;
        case GL_EXP2:
            mode = 2;
            break;
        default:
            mode = 1;
            break;
    }

    m_fogModeChoice->button(mode)->setChecked(true);

    m_fogDensity = new QDoubleSpinBox(this);
    m_fogDensity->setRange(0, 100);

    m_fogStart = new QDoubleSpinBox(this);
    m_fogStart->setRange(-1000, 1000);

    m_fogEnd = new QDoubleSpinBox(this);
    m_fogEnd->setRange(-1000, 1000);

    connect(m_fogDensity, SIGNAL(valueChanged(double)), SLOT(fogDensityChanged(double)));
    connect(m_fogStart, SIGNAL(valueChanged(double)), SLOT(fogStartChanged(double)));
    connect(m_fogEnd, SIGNAL(valueChanged(double)), SLOT(fogEndChanged(double)));

    m_fogColor = new QColorButton(this);
    connect(m_fogColor, SIGNAL(selected(const QColor&)), SLOT(fogColorChanged(const QColor&)));

    QLabel* fogDensityLbl = new QLabel(tr("GL_FOG_DENSITY"), this);
    QLabel* fogStartLbl = new QLabel(tr("GL_FOG_START"), this);
    QLabel* fogEndLbl = new QLabel(tr("GL_FOG_END"), this);
    QLabel* fogColorLbl = new QLabel(tr("GL_FOG_COLOR"), this);

    m_fogCheckBox = new QCheckBox(tr("GL_FOG_ENABLE"), this);
    m_fogCheckBox->setChecked(glState->getFogEnable());
    connect(m_fogCheckBox, SIGNAL(clicked(bool)), SLOT(onCheckbox()));

    fogSizer->addWidget(m_fogCheckBox, 0, 0);

    fogSizer->addWidget(fogSourceChoiceBox, 1, 0, 3, 1);

    fogSizer->addWidget(fogModeChoiceBox, 1, 1, 3, 1);

    fogSizer->addWidget(fogDensityLbl, 1, 2);
    fogSizer->addWidget(fogStartLbl, 2, 2);
    fogSizer->addWidget(fogEndLbl, 3, 2);

    fogSizer->addWidget(m_fogDensity, 1, 3);
    fogSizer->addWidget(m_fogStart, 2, 3);
    fogSizer->addWidget(m_fogEnd, 3, 3);

    fogSizer->addWidget(fogColorLbl, 1, 4);

    fogSizer->addWidget(m_fogColor, 1, 5);
    fogSizer->setRowStretch(4, 2);
    fogSizer->setColumnStretch(6, 2);

    setup();
}

void
SGOglFogNBPage::setup()
{
    m_fogCheckBox->setChecked(m_glState->getFogEnable());

    const Fog& fog = m_glState->getFog();
    switch (fog.fogSource) {
        case GL_FOG_COORD:
            m_fogSourceChoice->button(0)->setChecked(true);
            break;
        case GL_FRAGMENT_DEPTH:
            m_fogSourceChoice->button(1)->setChecked(true);
            break;
    }

    switch (fog.fogMode) {
        case GL_LINEAR:
            m_fogModeChoice->button(0)->setChecked(true);
            break;
        case GL_EXP:
            m_fogModeChoice->button(1)->setChecked(true);
            break;
        case GL_EXP2:
            m_fogModeChoice->button(2)->setChecked(true);
            break;
    }

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
SGOglFogNBPage::fogModeChanged(int index)
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

void
SGOglFogNBPage::fogSourceChanged(int index)
{
    switch (index) {
        case 0:
            m_glState->getFog().fogSource = GL_FOG_COORD;
            break;
        case 1:
            m_glState->getFog().fogSource = GL_FRAGMENT_DEPTH;
            break;
        default:
            break;
    }
    emit valueChanged();
}
