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
#include "SGOglNotebook.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"
#include "SGFrame.h"
#include "SGCanvas.h"

SGOglLightNBPage::SGOglLightNBPage(SGOglNotebook* parent)
    : QWidget(parent)
{
    m_parent = parent;
    SGFixedGLState* glState = m_parent->GetGLState();

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
        lcb->setChecked(glState->GetLight(i).lightEnabled);
        h1->addWidget(lcb);
        lightCheckGroup->addButton(lcb, i);
    }
    connect(lightCheckGroup, SIGNAL(buttonClicked(int)), SLOT(OnCheckbox(int)));

    lightingCheckBox  = new QCheckBox(tr("GL_LIGHTING"), this);
    lightingCheckBox->setChecked(glState->GetLightingEnable());
    connect(lightingCheckBox, SIGNAL(clicked()), SLOT(lightingChanged()));

    normalizeCheckBox = new QCheckBox(tr("GL_NORMALIZE"), this);
    normalizeCheckBox->setChecked(glState->GetNormalizeEnable());
    connect(normalizeCheckBox, SIGNAL(clicked()), SLOT(normalizeChanged()));

    separateSpecularColorCheckBox = new QCheckBox(tr("GL_SEPARATE_SPECULAR_COLOR"), this);
    separateSpecularColorCheckBox->setChecked(glState->GetSeparateSpecularColorEnable());
    connect(separateSpecularColorCheckBox, SIGNAL(clicked()), SLOT(separateSpecularChanged()));

    h1->addWidget(lightingCheckBox);
    h1->addWidget(normalizeCheckBox);
    h1->addWidget(separateSpecularColorCheckBox);

    const Light &light = glState->GetLight(lightSelectionGroup->checkedId());

    lightPosition = new QVectorEdit(light.lightPositionVector, this);
    connect(lightPosition, SIGNAL(valueChanged()), SLOT(lightPositionChanged()));

    spotDirection = new QVectorEdit(light.lightSpotDirectionVector, this);
    connect(spotDirection, SIGNAL(valueChanged()), SLOT(spotDirectionChanged()));

    spotExponent  = new QDoubleSpinBox(this);
    spotExponent->setRange(-1000, 1000);
    spotExponent->setValue(light.lightSpotExponent);
    connect(spotExponent, SIGNAL(valueChanged(double)), SLOT(spotExponentChanged()));

    spotCutoff    = new QDoubleSpinBox(this);
    spotCutoff->setRange(-1000, 1000);
    spotCutoff->setValue(light.lightSpotCutoff);
    connect(spotCutoff, SIGNAL(valueChanged(double)), SLOT(spotCutoffChanged()));

    constantAttenuation  = new QDoubleSpinBox(this);
    constantAttenuation->setRange(-1000, 1000);
    constantAttenuation->setValue(light.lightConstantAttenuation);
    connect(constantAttenuation, SIGNAL(valueChanged(double)), SLOT(constantAttenuationChanged()));

    linearAttenuation    = new QDoubleSpinBox(this);
    linearAttenuation->setRange(-1000, 1000);
    linearAttenuation->setValue(light.lightLinearAttenuation);
    connect(linearAttenuation, SIGNAL(valueChanged(double)), SLOT(linearAttenuationChanged()));

    quadraticAttenuation = new QDoubleSpinBox(this);
    quadraticAttenuation->setRange(-1000, 1000);
    quadraticAttenuation->setValue(light.lightQuadraticAttenuation);
    connect(quadraticAttenuation, SIGNAL(valueChanged(double)), SLOT(quadraticAttenuationChanged()));

    ambientLight  = new QColorButton(this);
    ambientLight->setColor(light.lightAmbientColorVector);
    connect(ambientLight, SIGNAL(clicked()), SLOT(ambientLightChanged()));

    specularLight = new QColorButton(this);
    specularLight->setColor(light.lightSpecularColorVector);
    connect(specularLight, SIGNAL(clicked()), SLOT(specularLightChanged()));

    diffuseLight  = new QColorButton(this);
    diffuseLight->setColor(light.lightDiffuseColorVector);
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
}

void SGOglLightNBPage::lightingChanged()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetLightingEnable(lightingCheckBox->isChecked());
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::normalizeChanged()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetNormalizeEnable(normalizeCheckBox->isChecked());
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::separateSpecularChanged()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetSeparateSpecularColorEnable(separateSpecularColorCheckBox->isChecked());
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void lightPositionChanged();
void SGOglLightNBPage::ambientLightChanged()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    int lightSelected = lightSelectionGroup->checkedId();
    glState->GetLight(lightSelected).lightAmbientColorVector = ambientLight->color();

    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::specularLightChanged()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    int lightSelected = lightSelectionGroup->checkedId();
    glState->GetLight(lightSelected).lightSpecularColorVector = specularLight->color();

    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::diffuseLightChanged()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    int lightSelected = lightSelectionGroup->checkedId();
    glState->GetLight(lightSelected).lightDiffuseColorVector = diffuseLight->color();

    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::OnCheckbox(int index)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    const QAbstractButton* btn = lightCheckGroup->button(index);
    glState->GetLight(index).lightEnabled = btn->isChecked();

    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::spotExponentChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->GetLight(lightSelected).lightSpotExponent = spotExponent->value();
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::spotCutoffChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->GetLight(lightSelected).lightSpotCutoff = spotCutoff->value();
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::constantAttenuationChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->GetLight(lightSelected).lightConstantAttenuation = constantAttenuation ->value();
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::quadraticAttenuationChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->GetLight(lightSelected).lightQuadraticAttenuation = quadraticAttenuation->value();
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::linearAttenuationChanged()
{
    int lightSelected = lightSelectionGroup->checkedId();
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->GetLight(lightSelected).lightLinearAttenuation = linearAttenuation->value();
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::lightPositionChanged()
{
    QVector4D tempLightPosVector = lightPosition->getValue();
    if(tempLightPosVector.w() != 1 && tempLightPosVector.w() != 0.0)
    {
        QMessageBox::warning(this, tr(""), tr("The w component of GL_POSITION must be 0.0 or 1.0"));
    }

    int lightSelected = lightSelectionGroup->checkedId();
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->GetLight(lightSelected).lightPositionVector = tempLightPosVector;
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::spotDirectionChanged()
{
    QVector4D tempLightSpotDirectionVector = spotDirection->getValue();
    int lightSelected = lightSelectionGroup->checkedId();
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->GetLight(lightSelected).lightSpotDirectionVector = QVector3D(tempLightSpotDirectionVector.x(),
                                                                           tempLightSpotDirectionVector.y(),
                                                                           tempLightSpotDirectionVector.z());
    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglLightNBPage::OnRadio(int index)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    const Light& light = glState->GetLight(index);

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

    m_parent->GetFrame()->SetCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

