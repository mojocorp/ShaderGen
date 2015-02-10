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

#include <QGroupBox>
#include <QBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include "QColorButton.h"

#include "SGOglFogNBPage.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"

SGOglFogNBPage::SGOglFogNBPage(SGFixedGLState* glState, QWidget* parent)
    :QWidget(parent),
      m_glState(glState)
{
    QGridLayout* fogSizer = new QGridLayout(this);

    QRadioButton *fogLinear = new QRadioButton(tr("GL_LINEAR"), this);
    QRadioButton *fogExp = new QRadioButton(tr("GL_EXP"), this);
    QRadioButton *fogExp2 = new QRadioButton(tr("GL_EXP2"), this);

    QGroupBox *fogModeChoiceBox = new QGroupBox(tr("Select Fog Mode"), this);
    QVBoxLayout *fogModeChoiceSizer = new QVBoxLayout(fogModeChoiceBox);

    fogModeChoiceSizer->addWidget(fogLinear);
    fogModeChoiceSizer->addWidget(fogExp);
    fogModeChoiceSizer->addWidget(fogExp2);

    fogModeChoice = new QButtonGroup(this);
    fogModeChoice->addButton(fogLinear, 0);
    fogModeChoice->addButton(fogExp, 1);
    fogModeChoice->addButton(fogExp2, 2);
    connect(fogModeChoice, SIGNAL(buttonClicked(int)), SLOT(onRadio(int)));

    GLenum aa = glState->getFog().fogMode;
    switch(aa)
    {
    case GL_LINEAR: aa = 0; break;
    case GL_EXP:    aa = 1; break;
    case GL_EXP2:   aa = 2; break;
    default:        aa = 1; break;
    }
    
    fogModeChoice->button(aa)->setChecked(true);

    fogDensity = new QDoubleSpinBox(this);
    fogDensity->setRange(0, 100);

    fogStart   = new QDoubleSpinBox(this);
    fogStart->setRange(-1000, 1000);

    fogEnd     = new QDoubleSpinBox(this);
    fogEnd->setRange(-1000, 1000);

    connect(fogDensity, SIGNAL(valueChanged(double)), SLOT(fogDensityChanged(double)));
    connect(fogStart, SIGNAL(valueChanged(double)), SLOT(fogDensityChanged(double)));
    connect(fogEnd, SIGNAL(valueChanged(double)), SLOT(fogDensityChanged(double)));

    fogColor = new QColorButton  (this);
    connect(fogColor, SIGNAL(selected(QColor)), SLOT(fogColorChanged(QColor)));

    QLabel* fogDensityLbl      = new QLabel(tr("GL_FOG_DENSITY"), this);
    QLabel* fogStartLbl        = new QLabel(tr("GL_FOG_START"), this);
    QLabel* fogEndLbl          = new QLabel(tr("GL_FOG_END"), this);
    QLabel* fogColorLbl        = new QLabel(tr("GL_FOG_COLOR"), this);

    fogCheckBox              = new QCheckBox  (tr("GL_FOG_ENABLE"), this);
    fogCheckBox->setChecked(glState->getFogEnable());
    connect(fogCheckBox, SIGNAL(clicked(bool)), SLOT(onCheckbox()));

    fogSizer->addWidget(fogCheckBox, 0, 0);

    fogSizer->addWidget(fogModeChoiceBox, 1, 0, 3, 1);

    fogSizer->addWidget(fogDensityLbl, 1, 1);
    fogSizer->addWidget(fogStartLbl,   2, 1);
    fogSizer->addWidget(fogEndLbl,     3, 1);

    fogSizer->addWidget(fogDensity, 1, 2);
    fogSizer->addWidget(fogStart,   2, 2);
    fogSizer->addWidget(fogEnd,     3, 2);

    fogSizer->addWidget(fogColorLbl, 1, 3);

    fogSizer->addWidget(fogColor, 1, 4);
    fogSizer->setRowStretch(4, 2);
    fogSizer->setColumnStretch(5, 2);

    setup();
}

void SGOglFogNBPage::setup()
{
    const Fog &fog = m_glState->getFog();
    fogDensity->setValue(fog.fogDensity);
    fogStart->setValue(fog.fogStart);
    fogEnd->setValue(fog.fogEnd);
    fogColor->setColor(fog.fogColorVector);
}

void SGOglFogNBPage::fogColorChanged(const QColor & color)
{
    m_glState->getFog().fogColorVector = color;

    emit valueChanged();
}

void SGOglFogNBPage::onCheckbox()
{
    m_glState->setFogEnable(fogCheckBox->isChecked());
    emit valueChanged();
}

void SGOglFogNBPage::fogDensityChanged(double density)
{
    m_glState->getFog().fogDensity = density;
    emit valueChanged();
}

void SGOglFogNBPage::fogStartChanged(double start)
{
    m_glState->getFog().fogStart   = start;
    emit valueChanged();
}

void SGOglFogNBPage::fogEndChanged(double end)
{
    m_glState->getFog().fogEnd   = end;
    emit valueChanged();
}

void SGOglFogNBPage::onRadio(int index)
{
    switch(index){
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
