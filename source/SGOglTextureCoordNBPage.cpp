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

#include "QVectorEdit.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QOpenGLFunctions>
#include <QRadioButton>
#include <QVBoxLayout>

#include "SGFixedGLState.h"
#include "SGOglTextureCoordNBPage.h"

SGOglTextureCoordNBPage::SGOglTextureCoordNBPage(SGFixedGLState* glState, QWidget* parent)
  : QWidget(parent)
  , m_glState(glState)
{
    QGroupBox* texPropertyBox = new QGroupBox(tr("Selected Texture Properties"), this);

    QGridLayout* textureSizer = new QGridLayout(this);
    QGridLayout* selectedTexPropertiesSizer = new QGridLayout(texPropertyBox);

    QGroupBox* eyePlaneCoeffBox = new QGroupBox(tr("Eye Plane Coefficients"), this);
    QHBoxLayout* eyePlaneCoeffBoxSizer = new QHBoxLayout(eyePlaneCoeffBox);

    QVBoxLayout* eyePlaneLabelSizer = new QVBoxLayout();
    QVBoxLayout* eyePlaneTextSizer = new QVBoxLayout();

    QLabel* eyePlaneCoeffLabelS = new QLabel(tr("GL_S"), this);
    QLabel* eyePlaneCoeffLabelT = new QLabel(tr("GL_T"), this);

    m_eyePlaneCoeffTextS = new QVectorEdit(this);
    connect(m_eyePlaneCoeffTextS, SIGNAL(valueChanged()), SLOT(onTextEnterEyeCoeffS()));

    m_eyePlaneCoeffTextT = new QVectorEdit(this);
    connect(m_eyePlaneCoeffTextT, SIGNAL(valueChanged()), SLOT(onTextEnterEyeCoeffT()));

    eyePlaneLabelSizer->addWidget(eyePlaneCoeffLabelS);
    eyePlaneLabelSizer->addWidget(eyePlaneCoeffLabelT);

    eyePlaneTextSizer->addWidget(m_eyePlaneCoeffTextS);
    eyePlaneTextSizer->addWidget(m_eyePlaneCoeffTextT);

    eyePlaneCoeffBoxSizer->addLayout(eyePlaneLabelSizer);
    eyePlaneCoeffBoxSizer->addLayout(eyePlaneTextSizer);

    QGroupBox* objectPlaneCoeffBox = new QGroupBox(tr("Object Plane Coefficients"), this);
    QHBoxLayout* objectPlaneCoeffBoxSizer = new QHBoxLayout(objectPlaneCoeffBox);

    QVBoxLayout* objectPlaneLabelSizer = new QVBoxLayout();
    QVBoxLayout* objectPlaneTextSizer = new QVBoxLayout();

    QLabel* objectPlaneCoeffLabelS = new QLabel(tr("GL_S"), this);
    QLabel* objectPlaneCoeffLabelT = new QLabel(tr("GL_T"), this);

    m_objectPlaneCoeffTextS = new QVectorEdit(this);
    connect(m_objectPlaneCoeffTextS, SIGNAL(valueChanged()), SLOT(onTextEnterObjCoeffS()));

    m_objectPlaneCoeffTextT = new QVectorEdit(this);
    connect(m_objectPlaneCoeffTextT, SIGNAL(valueChanged()), SLOT(onTextEnterObjCoeffT()));

    objectPlaneLabelSizer->addWidget(objectPlaneCoeffLabelS);
    objectPlaneLabelSizer->addWidget(objectPlaneCoeffLabelT);

    objectPlaneTextSizer->addWidget(m_objectPlaneCoeffTextS);
    objectPlaneTextSizer->addWidget(m_objectPlaneCoeffTextT);

    objectPlaneCoeffBoxSizer->addLayout(objectPlaneLabelSizer);
    objectPlaneCoeffBoxSizer->addLayout(objectPlaneTextSizer);

    QGroupBox* texCoordUnitBox = new QGroupBox(tr("Selected Texture Unit"), this);
    QHBoxLayout* texCoordUnitSizer = new QHBoxLayout(texCoordUnitBox);

    QRadioButton* tex0TexSelRadioButton = new QRadioButton(tr("T0"), this);
    QRadioButton* tex1TexSelRadioButton = new QRadioButton(tr("T1"), this);
    QRadioButton* tex2TexSelRadioButton = new QRadioButton(tr("T2"), this);
    QRadioButton* tex3TexSelRadioButton = new QRadioButton(tr("T3"), this);
    QRadioButton* tex4TexSelRadioButton = new QRadioButton(tr("T4"), this);

    texCoordUnitSizer->addWidget(tex0TexSelRadioButton);
    texCoordUnitSizer->addWidget(tex1TexSelRadioButton);
    texCoordUnitSizer->addWidget(tex2TexSelRadioButton);
    texCoordUnitSizer->addWidget(tex3TexSelRadioButton);
    texCoordUnitSizer->addWidget(tex4TexSelRadioButton);

    m_texCoordUnitGroup = new QButtonGroup(this);
    m_texCoordUnitGroup->addButton(tex0TexSelRadioButton, 0);
    m_texCoordUnitGroup->addButton(tex1TexSelRadioButton, 1);
    m_texCoordUnitGroup->addButton(tex2TexSelRadioButton, 2);
    m_texCoordUnitGroup->addButton(tex3TexSelRadioButton, 3);
    m_texCoordUnitGroup->addButton(tex4TexSelRadioButton, 4);
    m_texCoordUnitGroup->button(0)->setChecked(true);
    connect(m_texCoordUnitGroup, SIGNAL(buttonClicked(int)), SLOT(onRadioTextureCoordUnit(int)));

    m_tex0TexGenEnableCheckBox = new QCheckBox(tr("T0"), this);
    m_tex1TexGenEnableCheckBox = new QCheckBox(tr("T1"), this);
    m_tex2TexGenEnableCheckBox = new QCheckBox(tr("T2"), this);
    m_tex3TexGenEnableCheckBox = new QCheckBox(tr("T3"), this);
    m_tex4TexGenEnableCheckBox = new QCheckBox(tr("T4"), this);

    QGroupBox* texGenEnableDisableBox = new QGroupBox(tr("glEnable/glDisable TexGen"), this);
    QHBoxLayout* texGenEnableDisableSizer = new QHBoxLayout(texGenEnableDisableBox);

    texGenEnableDisableSizer->addWidget(m_tex0TexGenEnableCheckBox);
    texGenEnableDisableSizer->addWidget(m_tex1TexGenEnableCheckBox);
    texGenEnableDisableSizer->addWidget(m_tex2TexGenEnableCheckBox);
    texGenEnableDisableSizer->addWidget(m_tex3TexGenEnableCheckBox);
    texGenEnableDisableSizer->addWidget(m_tex4TexGenEnableCheckBox);

    m_texCoordSelGroup = new QButtonGroup(this);
    m_texCoordSelGroup->setExclusive(false);
    m_texCoordSelGroup->addButton(m_tex0TexGenEnableCheckBox, 0);
    m_texCoordSelGroup->addButton(m_tex1TexGenEnableCheckBox, 1);
    m_texCoordSelGroup->addButton(m_tex2TexGenEnableCheckBox, 2);
    m_texCoordSelGroup->addButton(m_tex3TexGenEnableCheckBox, 3);
    m_texCoordSelGroup->addButton(m_tex4TexGenEnableCheckBox, 4);
    connect(m_texCoordSelGroup, SIGNAL(buttonClicked(int)), SLOT(onCheckbox(int)));

    QGroupBox* coordGenBox =
      new QGroupBox(tr("Texture Coordinate Generation Method (Vertex Shader)"), this);
    QVBoxLayout* coordGenSizer = new QVBoxLayout(coordGenBox);

    QRadioButton* olinearCoordGenRadioButton = new QRadioButton(tr("GL_OBJECT_LINEAR"), this);
    QRadioButton* elinearCoordGenRadioButton = new QRadioButton(tr("GL_EYE_LINEAR"), this);
    QRadioButton* spheremCoordGenRadioButton = new QRadioButton(tr("GL_SPHERE_MAP"), this);
    QRadioButton* reflecmCoordGenRadioButton = new QRadioButton(tr("GL_REFLECTION_MAP"), this);
    QRadioButton* normalmCoordGenRadioButton = new QRadioButton(tr("GL_NORMAL_MAP"), this);

    coordGenSizer->addWidget(olinearCoordGenRadioButton);
    coordGenSizer->addWidget(elinearCoordGenRadioButton);
    coordGenSizer->addWidget(spheremCoordGenRadioButton);
    coordGenSizer->addWidget(reflecmCoordGenRadioButton);
    coordGenSizer->addWidget(normalmCoordGenRadioButton);

    m_coordGenGroup = new QButtonGroup(this);
    m_coordGenGroup->addButton(olinearCoordGenRadioButton, 0);
    m_coordGenGroup->addButton(elinearCoordGenRadioButton, 1);
    m_coordGenGroup->addButton(spheremCoordGenRadioButton, 2);
    m_coordGenGroup->addButton(reflecmCoordGenRadioButton, 3);
    m_coordGenGroup->addButton(normalmCoordGenRadioButton, 4);
    m_coordGenGroup->button(0)->setChecked(true);
    connect(m_coordGenGroup, SIGNAL(buttonClicked(int)), SLOT(onRadioTexCoordGen(int)));

    selectedTexPropertiesSizer->addWidget(coordGenBox, 0, 0, 2, 1);
    selectedTexPropertiesSizer->addWidget(eyePlaneCoeffBox, 0, 1);
    selectedTexPropertiesSizer->addWidget(objectPlaneCoeffBox, 0, 2);

    textureSizer->addWidget(texGenEnableDisableBox, 0, 0);
    textureSizer->addWidget(texCoordUnitBox, 1, 0);
    textureSizer->addWidget(texPropertyBox, 2, 0, 1, 2);
    textureSizer->setRowStretch(2, 2);
    textureSizer->setColumnStretch(2, 2);

    setup();
}

void
SGOglTextureCoordNBPage::setup()
{
    m_eyePlaneCoeffTextS->setValue(m_glState->getTexture(0).eyePlaneCoeffS);
    m_eyePlaneCoeffTextT->setValue(m_glState->getTexture(0).eyePlaneCoeffT);
    m_objectPlaneCoeffTextS->setValue(m_glState->getTexture(0).objectPlaneCoeffS);
    m_objectPlaneCoeffTextT->setValue(m_glState->getTexture(0).objectPlaneCoeffT);

    m_tex0TexGenEnableCheckBox->setChecked(m_glState->getTexture(0).texGen);
    m_tex1TexGenEnableCheckBox->setChecked(m_glState->getTexture(1).texGen);
    m_tex2TexGenEnableCheckBox->setChecked(m_glState->getTexture(2).texGen);
    m_tex3TexGenEnableCheckBox->setChecked(m_glState->getTexture(3).texGen);
    m_tex4TexGenEnableCheckBox->setChecked(m_glState->getTexture(4).texGen);

    const Texture& texture = m_glState->getTexture(m_texCoordUnitGroup->checkedId());

    switch (texture.textureCoordinateGeneration) {
        case GL_OBJECT_LINEAR:
            m_coordGenGroup->button(0)->setChecked(true);
            break;
        case GL_EYE_LINEAR:
            m_coordGenGroup->button(1)->setChecked(true);
            break;
        case GL_SPHERE_MAP:
            m_coordGenGroup->button(2)->setChecked(true);
            break;
        case GL_REFLECTION_MAP:
            m_coordGenGroup->button(3)->setChecked(true);
            break;
        case GL_NORMAL_MAP:
            m_coordGenGroup->button(4)->setChecked(true);
            break;
        default:
            break;
    }

    m_eyePlaneCoeffTextS->setValue(texture.eyePlaneCoeffS);
    m_eyePlaneCoeffTextT->setValue(texture.eyePlaneCoeffT);
    m_objectPlaneCoeffTextS->setValue(texture.objectPlaneCoeffS);
    m_objectPlaneCoeffTextT->setValue(texture.objectPlaneCoeffT);
}

void
SGOglTextureCoordNBPage::onRadioTextureCoordUnit(int)
{
    updateWidgets();
    emit valueChanged();
}

void
SGOglTextureCoordNBPage::onRadioTexCoordGen(int index)
{
    const int workingTextureCoords = m_texCoordUnitGroup->checkedId();

    switch (index) {
        case TEXTURE_COORDINATE_OBJECT_LINEAR:
            m_glState->getTexture(workingTextureCoords).textureCoordinateGeneration =
              GL_OBJECT_LINEAR;
            break;
        case TEXTURE_COORDINATE_EYE_LINEAR:
            m_glState->getTexture(workingTextureCoords).textureCoordinateGeneration = GL_EYE_LINEAR;
            break;
        case TEXTURE_COORDINATE_SPHERE_MAP:
            m_glState->getTexture(workingTextureCoords).textureCoordinateGeneration = GL_SPHERE_MAP;
            break;
        case TEXTURE_COORDINATE_REFLECTION_MAP:
            m_glState->getTexture(workingTextureCoords).textureCoordinateGeneration =
              GL_REFLECTION_MAP;
            break;
        case TEXTURE_COORDINATE_NORMAL_MAP:
            m_glState->getTexture(workingTextureCoords).textureCoordinateGeneration = GL_NORMAL_MAP;
            break;
        default:
            break;
    }
    emit valueChanged();
}

void
SGOglTextureCoordNBPage::onCheckbox(int index)
{
    m_glState->setTexGenEnable(m_texCoordSelGroup->checkedId() != -1);
    switch (index) {
        case 0:
            m_glState->getTexture(0).texGen = m_tex0TexGenEnableCheckBox->isChecked();
            break;
        case 1:
            m_glState->getTexture(1).texGen = m_tex1TexGenEnableCheckBox->isChecked();
            break;
        case 2:
            m_glState->getTexture(2).texGen = m_tex2TexGenEnableCheckBox->isChecked();
            break;
        case 3:
            m_glState->getTexture(3).texGen = m_tex3TexGenEnableCheckBox->isChecked();
            break;
        case 4:
            m_glState->getTexture(4).texGen = m_tex4TexGenEnableCheckBox->isChecked();
            break;
    }
    emit valueChanged();
}

void
SGOglTextureCoordNBPage::onTextEnterEyeCoeffS()
{
    QVector4D eyePlaneSVec = m_eyePlaneCoeffTextS->getValue();

    m_glState->getTexture(m_texCoordUnitGroup->checkedId()).eyePlaneCoeffS = eyePlaneSVec;

    emit valueChanged();
}

void
SGOglTextureCoordNBPage::onTextEnterEyeCoeffT()
{
    QVector4D eyePlaneTVec = m_eyePlaneCoeffTextT->getValue();

    m_glState->getTexture(m_texCoordUnitGroup->checkedId()).eyePlaneCoeffT = eyePlaneTVec;

    emit valueChanged();
}

void
SGOglTextureCoordNBPage::onTextEnterObjCoeffS()
{
    QVector4D objPlaneSVec = m_objectPlaneCoeffTextS->getValue();

    m_glState->getTexture(m_texCoordUnitGroup->checkedId()).objectPlaneCoeffS = objPlaneSVec;

    emit valueChanged();
}

void
SGOglTextureCoordNBPage::onTextEnterObjCoeffT()
{
    QVector4D objPlaneTVec = m_objectPlaneCoeffTextT->getValue();

    m_glState->getTexture(m_texCoordUnitGroup->checkedId()).objectPlaneCoeffT = objPlaneTVec;

    emit valueChanged();
}

void
SGOglTextureCoordNBPage::updateWidgets()
{
    const Texture& texture = m_glState->getTexture(m_texCoordUnitGroup->checkedId());

    switch (texture.textureCoordinateGeneration) {
        case GL_OBJECT_LINEAR:
            m_coordGenGroup->button(TEXTURE_COORDINATE_OBJECT_LINEAR)->setChecked(true);
            break;
        case GL_EYE_LINEAR:
            m_coordGenGroup->button(TEXTURE_COORDINATE_EYE_LINEAR)->setChecked(true);
            break;
        case GL_SPHERE_MAP:
            m_coordGenGroup->button(TEXTURE_COORDINATE_SPHERE_MAP)->setChecked(true);
            break;
        case GL_REFLECTION_MAP:
            m_coordGenGroup->button(TEXTURE_COORDINATE_REFLECTION_MAP)->setChecked(true);
            break;
        case GL_NORMAL_MAP:
            m_coordGenGroup->button(TEXTURE_COORDINATE_NORMAL_MAP)->setChecked(true);
            break;
        default:
            break;
    }

    m_eyePlaneCoeffTextS->setValue(texture.eyePlaneCoeffS);
    m_eyePlaneCoeffTextT->setValue(texture.eyePlaneCoeffT);
    m_objectPlaneCoeffTextS->setValue(texture.objectPlaneCoeffS);
    m_objectPlaneCoeffTextT->setValue(texture.objectPlaneCoeffT);
}
