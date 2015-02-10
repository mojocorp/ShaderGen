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

#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QMessageBox>

#include "QColorButton.h"
#include "QVectorEdit.h"

#include "SGOglLightNBPage.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"

SGOglLightNBPage::SGOglLightNBPage(SGFixedGLState* glState, QWidget* parent)
    : QWidget(parent),
      m_glState(glState)
{
    QGroupBox* lightBox             = new QGroupBox(tr("Lighting Parameters"), this);
    QGroupBox* enableDisableBox     = new QGroupBox(tr("glEnable/glDisable"), this);
    QGroupBox* lightSelectionBox    = new QGroupBox(tr("Select Light"), this);
    QGroupBox* selectedLightBox     = new QGroupBox(tr("Selected Light Properties"), this);
    QGridLayout* lightSizer     = new QGridLayout(lightBox);
    QHBoxLayout* h1             = new QHBoxLayout(enableDisableBox);
    QHBoxLayout* h2             = new QHBoxLayout(lightSelectionBox);
    QGridLayout* h3             = new QGridLayout(selectedLightBox);

    lightSelectionGroup = new QButtonGroup(this);
    for (int i=0; i<NUM_LIGHTS; i++) {
        QRadioButton *lrb = new QRadioButton(QString("L%1  ").arg(i), this);
        h2->addWidget(lrb);
        lightSelectionGroup->addButton(lrb, i);
    }
    lightSelectionGroup->button(0)->setChecked(true);
    connect(lightSelectionGroup, SIGNAL(buttonClicked(int)), SLOT(OnRadio(int)));

    lightCheckGroup = new QButtonGroup(this);
    lightCheckGroup->setExclusive(false);
    for (int i=0; i<NUM_LIGHTS; i++) {
        QCheckBox *lcb = new QCheckBox(QString("L%1").arg(i), this);
        h1->addWidget(lcb);
        lightCheckGroup->addButton(lcb, i);
    }
    connect(lightCheckGroup, SIGNAL(buttonClicked(int)), SLOT(OnCheckbox(int)));

    lightingCheckBox  = new QCheckBox(tr("GL_LIGHTING"), this);
    connect(lightingCheckBox, SIGNAL(clicked()), SLOT(lightingChanged()));

    normalizeCheckBox = new QCheckBox(tr("GL_NORMALIZE"), this);
    connect(normalizeCheckBox, SIGNAL(clicked()), SLOT(normalizeChanged()));

    separateSpecularColorCheckBox = new QCheckBox(tr("GL_SEPARATE_SPECULAR_COLOR"), this);
    connect(separateSpecularColorCheckBox, SIGNAL(clicked()), SLOT(separateSpecularChanged()));

    h1->addWidget(lightingCheckBox);
    h1->addWidget(normalizeCheckBox);
    h1->addWidget(separateSpecularColorCheckBox);

    lightPosition = new QVectorEdit(QVector4D(), this);
    connect(lightPosition, SIGNAL(valueChanged()), SLOT(lightPositionChanged()));

    spotDirection = new QVectorEdit(QVector3D(), this);
    connect(spotDirection, SIGNAL(valueChanged()), SLOT(spotDirectionChanged()));

    spotExponent  = new QDoubleSpinBox(this);
    spotExponent->setRange(-1000, 1000);
    connect(spotExponent, SIGNAL(valueChanged(double)), SLOT(spotExponentChanged()));

    spotCutoff    = new QDoubleSpinBox(this);
    spotCutoff->setRange(-1000, 1000);
    connect(spotCutoff, SIGNAL(valueChanged(double)), SLOT(spotCutoffChanged()));

    constantAttenuation  = new QDoubleSpinBox(this);
    constantAttenuation->setRange(-1000, 1000);
    connect(constantAttenuation, SIGNAL(valueChanged(double)), SLOT(constantAttenuationChanged()));

    linearAttenuation    = new QDoubleSpinBox(this);
    linearAttenuation->setRange(-1000, 1000);
    connect(linearAttenuation, SIGNAL(valueChanged(double)), SLOT(linearAttenuationChanged()));

    quadraticAttenuation = new QDoubleSpinBox(this);
    quadraticAttenuation->setRange(-1000, 1000);
    connect(quadraticAttenuation, SIGNAL(valueChanged(double)), SLOT(quadraticAttenuationChanged()));

    ambientLight  = new QColorButton(this);
    connect(ambientLight, SIGNAL(clicked()), SLOT(ambientLightChanged()));

    specularLight = new QColorButton(this);
    connect(specularLight, SIGNAL(clicked()), SLOT(specularLightChanged()));

    diffuseLight  = new QColorButton(this);
    connect(diffuseLight, SIGNAL(clicked()), SLOT(diffuseLightChanged()));

    QLabel* positionLbl = new QLabel(tr("GL_POSITION"), this);

    QLabel* ambientLbl  = new QLabel(tr("GL_AMBIENT"),  this);
    QLabel* specularLbl = new QLabel(tr("GL_SPECULAR"), this);
    QLabel* diffuseLbl  = new QLabel(tr("GL_DIFFUSE"),  this);

    QLabel* spotDirectionLbl  = new QLabel(tr("GL_SPOT_DIRECTION"), this);
    QLabel* spotExponentLbl   = new QLabel(tr("GL_SPOT_EXPONENT"),  this);
    QLabel* spotCutoffLbl     = new QLabel(tr("GL_SPOT_CUTOFF"),    this);

    QLabel* constantAttenLbl  = new QLabel(tr("GL_CONSTANT_ATTENUATION"),  this);
    QLabel* linearAttenLbl    = new QLabel(tr("GL_LINEAR_ATTENUATION"),    this);
    QLabel* quadraticAttenLbl = new QLabel(tr("GL_QUADRATIC_ATTENUATION"), this);

    h3->addWidget(positionLbl, 0, 0);

    h3->addWidget(lightPosition, 0, 1);

    h3->addWidget(ambientLbl,  0, 2);
    h3->addWidget(specularLbl, 1, 2);
    h3->addWidget(diffuseLbl,  2, 2);

    h3->addWidget(ambientLight,  0, 3);
    h3->addWidget(specularLight, 1, 3);
    h3->addWidget(diffuseLight,  2, 3);

    h3->addWidget(spotDirectionLbl, 0, 4);
    h3->addWidget(spotExponentLbl,  1, 4);
    h3->addWidget(spotCutoffLbl,    2, 4);

    h3->addWidget(spotDirection, 0, 5);
    h3->addWidget(spotExponent,  1, 5);
    h3->addWidget(spotCutoff,    2, 5);

    h3->addWidget(constantAttenLbl,  0, 6);
    h3->addWidget(linearAttenLbl,    1, 6);
    h3->addWidget(quadraticAttenLbl, 2, 6);

    h3->addWidget(constantAttenuation,  0, 7);
    h3->addWidget(linearAttenuation,    1, 7);
    h3->addWidget(quadraticAttenuation, 2, 7);


    lightSizer->addWidget(enableDisableBox,  0, 0, 1, 2);
    lightSizer->addWidget(lightSelectionBox, 1, 0, 1, 1);
    lightSizer->addWidget(selectedLightBox,  2, 0, 1, 3);
    lightSizer->setRowStretch(3, 2);
    lightSizer->setColumnStretch(4, 2);

    setLayout(new QVBoxLayout);
    layout()->addWidget(lightBox);

    setup();
}

void SGOglLightNBPage::setup()
{
    for (int i=0; i<NUM_LIGHTS; i++) {
        lightCheckGroup->button(i)->setChecked(m_glState->GetLight(i).lightEnabled);
    }
    lightingCheckBox->setChecked(m_glState->GetLightingEnable());
    normalizeCheckBox->setChecked(m_glState->GetNormalizeEnable());
    separateSpecularColorCheckBox->setChecked(m_glState->GetSeparateSpecularColorEnable());

    const Light &light = m_glState->GetLight(lightSelectionGroup->checkedId());

    lightPosition->setValue(light.lightPositionVector);
    spotDirection->setValue(light.lightSpotDirectionVector);
    spotExponent->setValue(light.lightSpotExponent);
    spotCutoff->setValue(light.lightSpotCutoff);
    constantAttenuation->setValue(light.lightConstantAttenuation);
    linearAttenuation->setValue(light.lightLinearAttenuation);
    quadraticAttenuation->setValue(light.lightQuadraticAttenuation);
    ambientLight->setColor(light.lightAmbientColorVector);
    specularLight->setColor(light.lightSpecularColorVector);
    diffuseLight->setColor(light.lightDiffuseColorVector);
}

void SGOglLightNBPage::lightingChanged()
{
    m_glState->SetLightingEnable(lightingCheckBox->isChecked());
    emit valueChanged();
}

void SGOglLightNBPage::normalizeChanged()
{
    m_glState->SetNormalizeEnable(normalizeCheckBox->isChecked());
    emit valueChanged();
}

void SGOglLightNBPage::separateSpecularChanged()
{
    m_glState->SetSeparateSpecularColorEnable(separateSpecularColorCheckBox->isChecked());
    emit valueChanged();
}

void SGOglLightNBPage::ambientLightChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightAmbientColorVector = ambientLight->color();

    emit valueChanged();
}

void SGOglLightNBPage::specularLightChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightSpecularColorVector = specularLight->color();

    emit valueChanged();
}

void SGOglLightNBPage::diffuseLightChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightDiffuseColorVector = diffuseLight->color();

    emit valueChanged();
}

void SGOglLightNBPage::OnCheckbox(int index)
{
    const QAbstractButton* btn = lightCheckGroup->button(index);
    m_glState->GetLight(index).lightEnabled = btn->isChecked();

    emit valueChanged();
}

void SGOglLightNBPage::spotExponentChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightSpotExponent = spotExponent->value();
    emit valueChanged();
}

void SGOglLightNBPage::spotCutoffChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightSpotCutoff = spotCutoff->value();
    emit valueChanged();
}

void SGOglLightNBPage::constantAttenuationChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightConstantAttenuation = constantAttenuation ->value();
    emit valueChanged();
}

void SGOglLightNBPage::quadraticAttenuationChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightQuadraticAttenuation = quadraticAttenuation->value();
    emit valueChanged();
}

void SGOglLightNBPage::linearAttenuationChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightLinearAttenuation = linearAttenuation->value();
    emit valueChanged();
}

void SGOglLightNBPage::lightPositionChanged()
{
    QVector4D tempLightPosVector = lightPosition->getValue();
    if(tempLightPosVector.w() != 1 && tempLightPosVector.w() != 0.0)
    {
        QMessageBox::warning(this, tr(""), tr("The w component of GL_POSITION must be 0.0 or 1.0"));
    }

    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightPositionVector = tempLightPosVector;
    emit valueChanged();
}

void SGOglLightNBPage::spotDirectionChanged()
{
    QVector4D tempLightSpotDirectionVector = spotDirection->getValue();
    int lightSelected = lightSelectionGroup->checkedId();
    m_glState->GetLight(lightSelected).lightSpotDirectionVector = QVector3D(tempLightSpotDirectionVector.x(),
                                                                          tempLightSpotDirectionVector.y(),
                                                                          tempLightSpotDirectionVector.z());
    emit valueChanged();
}

void SGOglLightNBPage::OnRadio(int index)
{
    const Light& light = m_glState->GetLight(index);

    ambientLight ->setColor(light.lightAmbientColorVector);
    diffuseLight ->setColor(light.lightDiffuseColorVector);
    specularLight->setColor(light.lightSpecularColorVector);
    spotDirection->setValue(light.lightSpotDirectionVector);
    spotExponent ->setValue(light.lightSpotExponent);
    spotCutoff   ->setValue(light.lightSpotCutoff);

    constantAttenuation->setValue(light.lightConstantAttenuation);
    quadraticAttenuation->setValue(light.lightQuadraticAttenuation);
    linearAttenuation->setValue(light.lightLinearAttenuation);

    lightPosition->setValue(light.lightPositionVector);

    emit valueChanged();
}

