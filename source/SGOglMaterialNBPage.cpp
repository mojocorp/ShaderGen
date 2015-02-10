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
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "QColorButton.h"

#include "SGFixedGLState.h"
#include "SGOglMaterialNBPage.h"
#include "UtilityFunctions.h"

SGOglMaterialNBPage::SGOglMaterialNBPage(SGFixedGLState* glState, QWidget* parent)
    :QWidget(parent),
      m_glState(glState)
{
    QGroupBox* materialBox  = new QGroupBox(tr("Material Properties"), this);
    QGridLayout* materialSizer = new QGridLayout(materialBox);

    shininessMaterial = new QDoubleSpinBox(this);
    shininessMaterial->setRange(0, 1000);
    connect(shininessMaterial, SIGNAL(valueChanged(double)), SLOT(shininessChanged()));

    ambientMaterial = new QColorButton(this);
    connect(ambientMaterial, SIGNAL(selected(QColor)), SLOT(ambientChanged()));

    diffuseMaterial = new QColorButton(this);
    connect(diffuseMaterial, SIGNAL(selected(QColor)), SLOT(diffuseChanged()));

    specularMaterial = new QColorButton(this);
    connect(specularMaterial, SIGNAL(selected(QColor)), SLOT(specularChanged()));

    emissionMaterial = new QColorButton(this);
    connect(emissionMaterial, SIGNAL(selected(QColor)), SLOT(emissionChanged()));

    QLabel* ambientMatLbl = new QLabel(tr("GL_AMBIENT"), this);
    QLabel* specularMatLbl = new QLabel(tr("GL_SPECULAR"), this);
    QLabel* diffuseMatLbl = new QLabel(tr("GL_DIFFUSE"), this);
    QLabel* shininessLbl = new QLabel(tr("GL_SHININESS"), this);
    QLabel* emissionLbl = new QLabel(tr("GL_EMISSION"), this);

    materialSizer->addWidget(ambientMatLbl, 0, 0);
    materialSizer->addWidget(diffuseMatLbl, 1, 0);
    materialSizer->addWidget(specularMatLbl, 2, 0);

    materialSizer->addWidget(ambientMaterial, 0, 1);
    materialSizer->addWidget(diffuseMaterial, 1, 1);
    materialSizer->addWidget(specularMaterial, 2, 1);

    materialSizer->addWidget(shininessLbl, 0, 2);
    materialSizer->addWidget(emissionLbl, 1, 2);

    materialSizer->addWidget(shininessMaterial, 0, 3);
    materialSizer->addWidget(emissionMaterial, 1, 3);

    materialSizer->setColumnStretch(4, 2);
    materialSizer->setRowStretch(3, 2);

    setLayout(new QVBoxLayout);
    layout()->addWidget(materialBox);

    setup();
}

void SGOglMaterialNBPage::setup()
{
    const Material& mat = m_glState->getMaterial();
    shininessMaterial->setValue(mat.materialShininess);
    ambientMaterial->setColor(mat.materialAmbientColorVector);
    diffuseMaterial->setColor(mat.materialDiffuseColorVector);
    specularMaterial->setColor(mat.materialSpecularColorVector);
    emissionMaterial->setColor(mat.materialEmissionColorVector);
}

void SGOglMaterialNBPage::ambientChanged()
{
    Material& mat = m_glState->getMaterial();

    mat.materialAmbientColorVector = ambientMaterial->color();

    emit valueChanged();
}

void SGOglMaterialNBPage::diffuseChanged()
{
    Material& mat = m_glState->getMaterial();

    mat.materialDiffuseColorVector = diffuseMaterial->color();

    emit valueChanged();
}

void SGOglMaterialNBPage::specularChanged()
{
    Material& mat = m_glState->getMaterial();

    mat.materialSpecularColorVector = specularMaterial->color();

    emit valueChanged();
}

void SGOglMaterialNBPage::emissionChanged()
{
    Material& mat = m_glState->getMaterial();

    mat.materialEmissionColorVector = emissionMaterial->color();

    emit valueChanged();
}

void SGOglMaterialNBPage::shininessChanged()
{
    m_glState->getMaterial().materialShininess = shininessMaterial->value();

    emit valueChanged();
}

