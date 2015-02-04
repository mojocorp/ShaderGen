/************************************************************************
* Author(s):     Joshua Doss                                            *
* Last Modified: 11/09/2005                                             *
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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include "QVectorEdit.h"

#include "SGOglTextureCoordNBPage.h"
#include "SGOglNotebook.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"
#include "SGFrame.h"
#include "SGCanvas.h"

SGOglTextureCoordNBPage::SGOglTextureCoordNBPage(SGOglNotebook* parent)
    :QWidget(parent)
{
    m_parent = parent;
    SGFixedGLState* glState = m_parent->GetGLState();

    QGroupBox* texBox            = new QGroupBox(tr("Textures"), this);
    QGroupBox* texPropertyBox    = new QGroupBox(tr("Selected Texture Properties"), this);

    QGridLayout* textureSizer                 = new QGridLayout(texBox);
    QGridLayout* selectedTexPropertiesSizer   = new QGridLayout(texPropertyBox);

    QGroupBox* eyePlaneCoeffBox = new QGroupBox(tr("Eye Plane Coefficients"), this);
    QHBoxLayout* eyePlaneCoeffBoxSizer = new QHBoxLayout(eyePlaneCoeffBox);

    QVBoxLayout* eyePlaneLabelSizer = new QVBoxLayout();
    QVBoxLayout* eyePlaneTextSizer = new QVBoxLayout();

    QLabel* eyePlaneCoeffLabelS = new QLabel(tr("GL_S"), this);
    QLabel* eyePlaneCoeffLabelT = new QLabel(tr("GL_T"), this);
    
    eyePlaneCoeffTextS = new QVectorEdit(glState->GetTexture(0)->eyePlaneCoeffS, this);
    connect(eyePlaneCoeffTextS, SIGNAL(valueChanged()), SLOT(OnTextEnterEyeCoeffS()));

    eyePlaneCoeffTextT = new QVectorEdit(glState->GetTexture(0)->eyePlaneCoeffT, this);
    connect(eyePlaneCoeffTextT, SIGNAL(valueChanged()), SLOT(OnTextEnterEyeCoeffT()));

    eyePlaneLabelSizer->addWidget(eyePlaneCoeffLabelS);
    eyePlaneLabelSizer->addWidget(eyePlaneCoeffLabelT);

    eyePlaneTextSizer->addWidget(eyePlaneCoeffTextS);
    eyePlaneTextSizer->addWidget(eyePlaneCoeffTextT);

    eyePlaneCoeffBoxSizer->addLayout(eyePlaneLabelSizer);
    eyePlaneCoeffBoxSizer->addLayout(eyePlaneTextSizer);

    QGroupBox* objectPlaneCoeffBox = new QGroupBox(tr("Object Plane Coefficients"), this);
    QHBoxLayout* objectPlaneCoeffBoxSizer = new QHBoxLayout(objectPlaneCoeffBox);
    
    QVBoxLayout* objectPlaneLabelSizer = new QVBoxLayout();
    QVBoxLayout* objectPlaneTextSizer = new QVBoxLayout();

    QLabel* objectPlaneCoeffLabelS = new QLabel(tr("GL_S"), this);
    QLabel* objectPlaneCoeffLabelT = new QLabel(tr("GL_T"), this);

    objectPlaneCoeffTextS = new QVectorEdit(glState->GetTexture(0)->objectPlaneCoeffS, this);
    connect(objectPlaneCoeffTextS, SIGNAL(valueChanged()), SLOT(OnTextEnterObjCoeffS()));

    objectPlaneCoeffTextT = new QVectorEdit(glState->GetTexture(0)->objectPlaneCoeffT, this);
    connect(objectPlaneCoeffTextT, SIGNAL(valueChanged()), SLOT(OnTextEnterObjCoeffT()));

    objectPlaneLabelSizer->addWidget(objectPlaneCoeffLabelS);
    objectPlaneLabelSizer->addWidget(objectPlaneCoeffLabelT);

    objectPlaneTextSizer->addWidget(objectPlaneCoeffTextS);
    objectPlaneTextSizer->addWidget(objectPlaneCoeffTextT);

    objectPlaneCoeffBoxSizer->addLayout(objectPlaneLabelSizer);
    objectPlaneCoeffBoxSizer->addLayout(objectPlaneTextSizer);

    QGroupBox *texCoordUnitBox = new QGroupBox(tr("Selected Texture Unit"), this);
    QHBoxLayout *texCoordUnitSizer = new QHBoxLayout(texCoordUnitBox);

    QRadioButton *tex0TexSelRadioButton = new QRadioButton(tr("T0"), this);
    QRadioButton *tex1TexSelRadioButton = new QRadioButton(tr("T1"), this);
    QRadioButton *tex2TexSelRadioButton = new QRadioButton(tr("T2"), this);
    QRadioButton *tex3TexSelRadioButton = new QRadioButton(tr("T3"), this);
    QRadioButton *tex4TexSelRadioButton = new QRadioButton(tr("T4"), this);

    texCoordUnitSizer->addWidget(tex0TexSelRadioButton);
    texCoordUnitSizer->addWidget(tex1TexSelRadioButton);
    texCoordUnitSizer->addWidget(tex2TexSelRadioButton);
    texCoordUnitSizer->addWidget(tex3TexSelRadioButton);
    texCoordUnitSizer->addWidget(tex4TexSelRadioButton);

    texCoordUnitGroup = new QButtonGroup(this);
    texCoordUnitGroup->addButton(tex0TexSelRadioButton, 0);
    texCoordUnitGroup->addButton(tex1TexSelRadioButton, 1);
    texCoordUnitGroup->addButton(tex2TexSelRadioButton, 2);
    texCoordUnitGroup->addButton(tex3TexSelRadioButton, 3);
    texCoordUnitGroup->addButton(tex4TexSelRadioButton, 4);
    texCoordUnitGroup->button(0)->setChecked(true);
    connect(texCoordUnitGroup, SIGNAL(buttonClicked(int)), SLOT(OnRadioTextureCoordUnit(int)));

    tex0TexGenEnableCheckBox = new QCheckBox(tr("T0"), this);
    tex0TexGenEnableCheckBox->setChecked(glState->GetTexture(0)->texGen);

    tex1TexGenEnableCheckBox = new QCheckBox(tr("T1"), this);
    tex1TexGenEnableCheckBox->setChecked(glState->GetTexture(1)->texGen);

    tex2TexGenEnableCheckBox = new QCheckBox(tr("T2"), this);
    tex2TexGenEnableCheckBox->setChecked(glState->GetTexture(2)->texGen);

    tex3TexGenEnableCheckBox = new QCheckBox(tr("T3"), this);
    tex3TexGenEnableCheckBox->setChecked(glState->GetTexture(3)->texGen);

    tex4TexGenEnableCheckBox = new QCheckBox(tr("T4"), this);
    tex4TexGenEnableCheckBox->setChecked(glState->GetTexture(4)->texGen);

    QGroupBox* texGenEnableDisableBox        = new QGroupBox(tr("glEnable/glDisable TexGen"), this);
    QHBoxLayout* texGenEnableDisableSizer    = new QHBoxLayout(texGenEnableDisableBox);

    texGenEnableDisableSizer->addWidget(tex0TexGenEnableCheckBox);
    texGenEnableDisableSizer->addWidget(tex1TexGenEnableCheckBox);
    texGenEnableDisableSizer->addWidget(tex2TexGenEnableCheckBox);
    texGenEnableDisableSizer->addWidget(tex3TexGenEnableCheckBox);
    texGenEnableDisableSizer->addWidget(tex4TexGenEnableCheckBox);

    texCoordSelGroup = new QButtonGroup(this);
    texCoordSelGroup->setExclusive(false);
    texCoordSelGroup->addButton(tex0TexGenEnableCheckBox, 0);
    texCoordSelGroup->addButton(tex1TexGenEnableCheckBox, 1);
    texCoordSelGroup->addButton(tex2TexGenEnableCheckBox, 2);
    texCoordSelGroup->addButton(tex3TexGenEnableCheckBox, 3);
    texCoordSelGroup->addButton(tex4TexGenEnableCheckBox, 4);
    connect(texCoordSelGroup, SIGNAL(buttonClicked(int)), SLOT(OnCheckbox(int)));

    QGroupBox *coordGenBox      = new QGroupBox(tr("Texture Coordinate Generation Method (Vertex Shader)"), this);
    QVBoxLayout *coordGenSizer = new QVBoxLayout(coordGenBox);

    QRadioButton *olinearCoordGenRadioButton = new QRadioButton(tr("GL_OBJECT_LINEAR"), this);
    QRadioButton *elinearCoordGenRadioButton = new QRadioButton(tr("GL_EYE_LINEAR"), this);
    QRadioButton *spheremCoordGenRadioButton = new QRadioButton(tr("GL_SPHERE_MAP"), this);
    QRadioButton *reflecmCoordGenRadioButton = new QRadioButton(tr("GL_REFLECTION_MAP"), this);
    QRadioButton *normalmCoordGenRadioButton = new QRadioButton(tr("GL_NORMAL_MAP"), this);

    coordGenSizer->addWidget(olinearCoordGenRadioButton);
    coordGenSizer->addWidget(elinearCoordGenRadioButton);
    coordGenSizer->addWidget(spheremCoordGenRadioButton);
    coordGenSizer->addWidget(reflecmCoordGenRadioButton);
    coordGenSizer->addWidget(normalmCoordGenRadioButton);

    coordGenGroup = new QButtonGroup(this);
    coordGenGroup->addButton(olinearCoordGenRadioButton, 0);
    coordGenGroup->addButton(elinearCoordGenRadioButton, 1);
    coordGenGroup->addButton(spheremCoordGenRadioButton, 2);
    coordGenGroup->addButton(reflecmCoordGenRadioButton, 3);
    coordGenGroup->addButton(normalmCoordGenRadioButton, 4);
    coordGenGroup->button(0)->setChecked(true);
    connect(coordGenGroup, SIGNAL(buttonClicked(int)), SLOT(OnRadioTexCoordGen(int)));

    selectedTexPropertiesSizer->addWidget(coordGenBox,         0, 0, 2, 1);
    selectedTexPropertiesSizer->addWidget(eyePlaneCoeffBox,    0, 1);
    selectedTexPropertiesSizer->addWidget(objectPlaneCoeffBox, 1, 1);

    textureSizer->addWidget(texGenEnableDisableBox, 0, 0);
    textureSizer->addWidget(texCoordUnitBox,        0, 1);
    textureSizer->addWidget(texPropertyBox,         1, 0, 1, 2);
    textureSizer->setRowStretch(2, 2);
    textureSizer->setColumnStretch(2, 2);

    setLayout(new QVBoxLayout);
    layout()->addWidget(texBox);
}

void SGOglTextureCoordNBPage::OnRadioTextureCoordUnit(int)
{
    UpdateWidgets();
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureCoordNBPage::OnRadioTexCoordGen(int index)
{
    SGFixedGLState* glState = m_parent->GetGLState();

    int workingTextureCoords = texCoordUnitGroup->checkedId();

    switch(index){
    case TEXTURE_COORDINATE_OBJECT_LINEAR:
        glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_OBJECT_LINEAR;
        break;
    case TEXTURE_COORDINATE_EYE_LINEAR:
        glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_EYE_LINEAR;
        break;
    case TEXTURE_COORDINATE_SPHERE_MAP:
        glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_SPHERE_MAP;
        break;
    case TEXTURE_COORDINATE_REFLECTION_MAP:
        glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_REFLECTION_MAP;
        break;
    case TEXTURE_COORDINATE_NORMAL_MAP:
        glState->GetTexture(workingTextureCoords)->textureCoordinateGeneration = GL_NORMAL_MAP;
        break;
    default:
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureCoordNBPage::OnCheckbox(int index)
{
    SGFixedGLState *glState = m_parent->GetGLState();

    switch(index){
    case 0:
        glState->GetTexture(0)->texGen = tex0TexGenEnableCheckBox->isChecked();
        break;
    case 1:
        glState->GetTexture(1)->texGen = tex1TexGenEnableCheckBox->isChecked();
        break;
    case 2:
        glState->GetTexture(2)->texGen = tex2TexGenEnableCheckBox->isChecked();
        break;
    case 3:
        glState->GetTexture(3)->texGen = tex3TexGenEnableCheckBox->isChecked();
        break;
    case 4:
        glState->GetTexture(4)->texGen = tex4TexGenEnableCheckBox->isChecked();
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureCoordNBPage::OnTextEnterEyeCoeffS()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);

    vec4 eyePlaneSVec = eyePlaneCoeffTextS->getValue();

    glState->GetTexture(texCoordUnitGroup->checkedId())->eyePlaneCoeffS = eyePlaneSVec;

    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureCoordNBPage::OnTextEnterEyeCoeffT()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);

    vec4 eyePlaneTVec = eyePlaneCoeffTextT->getValue();

    glState->GetTexture(texCoordUnitGroup->checkedId())->eyePlaneCoeffT = eyePlaneTVec;

    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureCoordNBPage::OnTextEnterObjCoeffS()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);

    vec4 objPlaneSVec = objectPlaneCoeffTextS->getValue();

    glState->GetTexture(texCoordUnitGroup->checkedId())->objectPlaneCoeffS = objPlaneSVec;

    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureCoordNBPage::OnTextEnterObjCoeffT()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);

    vec4 objPlaneTVec = objectPlaneCoeffTextT->getValue();

    glState->GetTexture(texCoordUnitGroup->checkedId())->objectPlaneCoeffT = objPlaneTVec;

    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureCoordNBPage::UpdateWidgets()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture* texture = glState->GetTexture(texCoordUnitGroup->checkedId());

    switch(texture->textureCoordinateGeneration){
    case GL_OBJECT_LINEAR:
        coordGenGroup->button(TEXTURE_COORDINATE_OBJECT_LINEAR)->setChecked(true);
        break;
    case GL_EYE_LINEAR:
        coordGenGroup->button(TEXTURE_COORDINATE_EYE_LINEAR)->setChecked(true);
        break;
    case GL_SPHERE_MAP:
        coordGenGroup->button(TEXTURE_COORDINATE_SPHERE_MAP)->setChecked(true);
        break;
    case GL_REFLECTION_MAP:
        coordGenGroup->button(TEXTURE_COORDINATE_REFLECTION_MAP)->setChecked(true);
        break;
    case GL_NORMAL_MAP:
        coordGenGroup->button(TEXTURE_COORDINATE_NORMAL_MAP)->setChecked(true);
        break;
    default:
        break;
    }

    eyePlaneCoeffTextS->setValue(texture->eyePlaneCoeffS);
    eyePlaneCoeffTextT->setValue(texture->eyePlaneCoeffT);
    objectPlaneCoeffTextS->setValue(texture->objectPlaneCoeffS);
    objectPlaneCoeffTextT->setValue(texture->objectPlaneCoeffT);
}

