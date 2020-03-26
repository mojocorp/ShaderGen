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
#include <QMessageBox>
#include <QRadioButton>
#include <QVBoxLayout>

#include "QColorButton.h"
#include "QVectorEdit.h"

#include "SGFixedGLState.h"
#include "SGOglLightNBPage.h"

SGOglLightNBPage::SGOglLightNBPage(SGFixedGLState* glState, QWidget* parent)
  : QWidget(parent)
  , m_glState(glState)
{
    auto enableDisableBox = new QGroupBox(tr("glEnable/glDisable"), this);
    auto lightSelectionBox = new QGroupBox(tr("Select Light"), this);
    auto selectedLightBox = new QGroupBox(tr("Selected Light Properties"), this);
    auto lightSizer = new QGridLayout(this);
    auto h1 = new QHBoxLayout(enableDisableBox);
    auto h2 = new QHBoxLayout(lightSelectionBox);
    auto h3 = new QGridLayout(selectedLightBox);

    m_lightSelectionGroup = new QButtonGroup(this);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        QRadioButton* lrb = new QRadioButton(QString("L%1  ").arg(i), this);
        h2->addWidget(lrb);
        m_lightSelectionGroup->addButton(lrb, i);
    }
    m_lightSelectionGroup->button(0)->setChecked(true);
    connect(m_lightSelectionGroup, SIGNAL(buttonClicked(int)), SLOT(onRadio(int)));

    m_lightCheckGroup = new QButtonGroup(this);
    m_lightCheckGroup->setExclusive(false);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        QCheckBox* lcb = new QCheckBox(QString("L%1").arg(i), this);
        h1->addWidget(lcb);
        m_lightCheckGroup->addButton(lcb, i);
    }
    connect(m_lightCheckGroup, SIGNAL(buttonClicked(int)), SLOT(onCheckbox(int)));

    m_lightingCheckBox = new QCheckBox(tr("GL_LIGHTING"), this);
    connect(m_lightingCheckBox, SIGNAL(clicked()), SLOT(lightingChanged()));

    m_twoSidedLightingCheckBox = new QCheckBox(tr("GL_LIGHT_MODEL_TWO_SIDE"), this);
    connect(m_twoSidedLightingCheckBox, SIGNAL(clicked()), SLOT(twoSidedLightingChanged()));

    m_localViewerLightingCheckBox = new QCheckBox(tr("GL_LIGHT_MODEL_LOCAL_VIEWER"), this);
    connect(m_localViewerLightingCheckBox, SIGNAL(clicked()), SLOT(localViewerLightingChanged()));

    m_normalizeCheckBox = new QCheckBox(tr("GL_NORMALIZE"), this);
    connect(m_normalizeCheckBox, SIGNAL(clicked()), SLOT(normalizeChanged()));

    m_rescaleNormalCheckBox = new QCheckBox(tr("GL_RESCALE_NORMAL"), this);
    connect(m_rescaleNormalCheckBox, SIGNAL(clicked()), SLOT(rescaleNormalChanged()));

    m_separateSpecularColorCheckBox = new QCheckBox(tr("GL_SEPARATE_SPECULAR_COLOR"), this);
    connect(m_separateSpecularColorCheckBox, SIGNAL(clicked()), SLOT(separateSpecularChanged()));

    h1->addWidget(m_lightingCheckBox);
    h1->addWidget(m_twoSidedLightingCheckBox);
    h1->addWidget(m_localViewerLightingCheckBox);
    h1->addWidget(m_normalizeCheckBox);
    h1->addWidget(m_rescaleNormalCheckBox);
    h1->addWidget(m_separateSpecularColorCheckBox);

    m_lightPosition = new QVectorEdit(this);
    connect(m_lightPosition, SIGNAL(valueChanged()), SLOT(lightPositionChanged()));

    m_spotDirection = new QVectorEdit(this);
    m_spotDirection->setNumFields(3);
    connect(m_spotDirection, SIGNAL(valueChanged()), SLOT(spotDirectionChanged()));

    m_spotExponent = new QDoubleSpinBox(this);
    m_spotExponent->setRange(-1000, 1000);
    connect(m_spotExponent, SIGNAL(valueChanged(double)), SLOT(spotExponentChanged()));

    m_spotCutoff = new QDoubleSpinBox(this);
    m_spotCutoff->setRange(-1000, 1000);
    connect(m_spotCutoff, SIGNAL(valueChanged(double)), SLOT(spotCutoffChanged()));

    m_constantAttenuation = new QDoubleSpinBox(this);
    m_constantAttenuation->setRange(-1000, 1000);
    connect(
      m_constantAttenuation, SIGNAL(valueChanged(double)), SLOT(constantAttenuationChanged()));

    m_linearAttenuation = new QDoubleSpinBox(this);
    m_linearAttenuation->setRange(-1000, 1000);
    connect(m_linearAttenuation, SIGNAL(valueChanged(double)), SLOT(linearAttenuationChanged()));

    m_quadraticAttenuation = new QDoubleSpinBox(this);
    m_quadraticAttenuation->setRange(-1000, 1000);
    connect(
      m_quadraticAttenuation, SIGNAL(valueChanged(double)), SLOT(quadraticAttenuationChanged()));

    m_ambientLight = new QColorButton(this);
    connect(m_ambientLight, SIGNAL(clicked()), SLOT(ambientLightChanged()));

    m_specularLight = new QColorButton(this);
    connect(m_specularLight, SIGNAL(clicked()), SLOT(specularLightChanged()));

    m_diffuseLight = new QColorButton(this);
    connect(m_diffuseLight, SIGNAL(clicked()), SLOT(diffuseLightChanged()));

    auto positionLbl = new QLabel(tr("GL_POSITION"), this);

    auto ambientLbl = new QLabel(tr("GL_AMBIENT"), this);
    auto specularLbl = new QLabel(tr("GL_SPECULAR"), this);
    auto diffuseLbl = new QLabel(tr("GL_DIFFUSE"), this);

    auto spotDirectionLbl = new QLabel(tr("GL_SPOT_DIRECTION"), this);
    auto spotExponentLbl = new QLabel(tr("GL_SPOT_EXPONENT"), this);
    auto spotCutoffLbl = new QLabel(tr("GL_SPOT_CUTOFF"), this);

    auto constantAttenLbl = new QLabel(tr("GL_CONSTANT_ATTENUATION"), this);
    auto linearAttenLbl = new QLabel(tr("GL_LINEAR_ATTENUATION"), this);
    auto quadraticAttenLbl = new QLabel(tr("GL_QUADRATIC_ATTENUATION"), this);

    h3->addWidget(positionLbl, 0, 0);

    h3->addWidget(m_lightPosition, 0, 1);

    h3->addWidget(ambientLbl, 0, 2);
    h3->addWidget(specularLbl, 1, 2);
    h3->addWidget(diffuseLbl, 2, 2);

    h3->addWidget(m_ambientLight, 0, 3);
    h3->addWidget(m_specularLight, 1, 3);
    h3->addWidget(m_diffuseLight, 2, 3);

    h3->addWidget(spotDirectionLbl, 0, 4);
    h3->addWidget(spotExponentLbl, 1, 4);
    h3->addWidget(spotCutoffLbl, 2, 4);

    h3->addWidget(m_spotDirection, 0, 5);
    h3->addWidget(m_spotExponent, 1, 5);
    h3->addWidget(m_spotCutoff, 2, 5);

    h3->addWidget(constantAttenLbl, 0, 6);
    h3->addWidget(linearAttenLbl, 1, 6);
    h3->addWidget(quadraticAttenLbl, 2, 6);

    h3->addWidget(m_constantAttenuation, 0, 7);
    h3->addWidget(m_linearAttenuation, 1, 7);
    h3->addWidget(m_quadraticAttenuation, 2, 7);

    lightSizer->addWidget(enableDisableBox, 0, 0, 1, 2);
    lightSizer->addWidget(lightSelectionBox, 1, 0, 1, 1);
    lightSizer->addWidget(selectedLightBox, 2, 0, 1, 3);
    lightSizer->setRowStretch(3, 2);
    lightSizer->setColumnStretch(4, 2);

    setup();
}

void
SGOglLightNBPage::setup()
{
    for (int i = 0; i < NUM_LIGHTS; i++) {
        m_lightCheckGroup->button(i)->setChecked(m_glState->getLight(i).enabled);
    }
    m_lightingCheckBox->setChecked(m_glState->getLightingEnable());
    m_twoSidedLightingCheckBox->setChecked(m_glState->getTwoSidedLightingEnable());
    m_localViewerLightingCheckBox->setChecked(m_glState->getLocalViewerLightingEnable());
    m_normalizeCheckBox->setChecked(m_glState->getNormalizeEnable());
    m_rescaleNormalCheckBox->setChecked(m_glState->getRescaleNormalEnable());
    m_separateSpecularColorCheckBox->setChecked(m_glState->getSeparateSpecularColorEnable());

    const Light& light = m_glState->getLight(m_lightSelectionGroup->checkedId());

    m_lightPosition->setValue(light.position);
    m_spotDirection->setValue(light.spotDirection);
    m_spotExponent->setValue(light.spotExponent);
    m_spotCutoff->setValue(light.spotCutoff);
    m_constantAttenuation->setValue(light.constantAttenuation);
    m_linearAttenuation->setValue(light.linearAttenuation);
    m_quadraticAttenuation->setValue(light.quadraticAttenuation);
    m_ambientLight->setColor(light.ambientColor);
    m_specularLight->setColor(light.specularColor);
    m_diffuseLight->setColor(light.diffuseColor);
}

void
SGOglLightNBPage::lightingChanged()
{
    m_glState->setLightingEnable(m_lightingCheckBox->isChecked());
    emit valueChanged();
}

void
SGOglLightNBPage::twoSidedLightingChanged()
{
    m_glState->setTwoSidedLightingEnable(m_twoSidedLightingCheckBox->isChecked());
    emit valueChanged();
}

void
SGOglLightNBPage::localViewerLightingChanged()
{
    m_glState->setLocalViewerLightingEnable(m_localViewerLightingCheckBox->isChecked());
    emit valueChanged();
}

void
SGOglLightNBPage::normalizeChanged()
{
    m_glState->setNormalizeEnable(m_normalizeCheckBox->isChecked());
    emit valueChanged();
}

void
SGOglLightNBPage::rescaleNormalChanged()
{
    m_glState->setRescaleNormalEnable(m_rescaleNormalCheckBox->isChecked());
    emit valueChanged();
}

void
SGOglLightNBPage::separateSpecularChanged()
{
    m_glState->setSeparateSpecularColorEnable(m_separateSpecularColorCheckBox->isChecked());
    emit valueChanged();
}

void
SGOglLightNBPage::ambientLightChanged()
{
    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).ambientColor = m_ambientLight->color();

    emit valueChanged();
}

void
SGOglLightNBPage::specularLightChanged()
{
    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).specularColor = m_specularLight->color();

    emit valueChanged();
}

void
SGOglLightNBPage::diffuseLightChanged()
{
    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).diffuseColor = m_diffuseLight->color();

    emit valueChanged();
}

void
SGOglLightNBPage::onCheckbox(int index)
{
    const QAbstractButton* btn = m_lightCheckGroup->button(index);
    m_glState->getLight(index).enabled = btn->isChecked();

    emit valueChanged();
}

void
SGOglLightNBPage::spotExponentChanged()
{
    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).spotExponent = m_spotExponent->value();
    emit valueChanged();
}

void
SGOglLightNBPage::spotCutoffChanged()
{
    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).spotCutoff = m_spotCutoff->value();
    emit valueChanged();
}

void
SGOglLightNBPage::constantAttenuationChanged()
{
    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).constantAttenuation = m_constantAttenuation->value();
    emit valueChanged();
}

void
SGOglLightNBPage::quadraticAttenuationChanged()
{
    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).quadraticAttenuation = m_quadraticAttenuation->value();
    emit valueChanged();
}

void
SGOglLightNBPage::linearAttenuationChanged()
{
    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).linearAttenuation = m_linearAttenuation->value();
    emit valueChanged();
}

void
SGOglLightNBPage::lightPositionChanged()
{
    QVector4D tempLightPosVector = m_lightPosition->getValue();
    if (tempLightPosVector.w() != 1 && tempLightPosVector.w() != 0.0) {
        QMessageBox::warning(this, tr(""), tr("The w component of GL_POSITION must be 0.0 or 1.0"));
    }

    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).position = tempLightPosVector;
    emit valueChanged();
}

void
SGOglLightNBPage::spotDirectionChanged()
{
    QVector4D tempLightSpotDirectionVector = m_spotDirection->getValue();
    int lightSelected = m_lightSelectionGroup->checkedId();
    m_glState->getLight(lightSelected).spotDirection = QVector3D(tempLightSpotDirectionVector.x(),
                                                                 tempLightSpotDirectionVector.y(),
                                                                 tempLightSpotDirectionVector.z());
    emit valueChanged();
}

void
SGOglLightNBPage::onRadio(int index)
{
    const Light& light = m_glState->getLight(index);

    m_ambientLight->setColor(light.ambientColor);
    m_diffuseLight->setColor(light.diffuseColor);
    m_specularLight->setColor(light.specularColor);
    m_spotDirection->setValue(light.spotDirection);
    m_spotExponent->setValue(light.spotExponent);
    m_spotCutoff->setValue(light.spotCutoff);

    m_constantAttenuation->setValue(light.constantAttenuation);
    m_quadraticAttenuation->setValue(light.quadraticAttenuation);
    m_linearAttenuation->setValue(light.linearAttenuation);

    m_lightPosition->setValue(light.position);

    emit valueChanged();
}
