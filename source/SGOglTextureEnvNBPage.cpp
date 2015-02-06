/************************************************************************
* Author(s):     Joshua Doss                                            *
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

#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include "QColorButton.h"

#include "SGOglTextureEnvNBPage.h"
#include "SGOglNotebook.h"
#include "SGFixedGLState.h"
#include "UtilityFunctions.h"
#include "SGFrame.h"
#include "SGCanvas.h"

SGOglTextureEnvNBPage::SGOglTextureEnvNBPage(SGOglNotebook* parent)
    :QWidget(parent)
{
    m_parent = parent;
    SGFixedGLState* glState = m_parent->GetGLState();

    QGridLayout* texMainSizer    = new QGridLayout(this);

    QGroupBox* texBox            = new QGroupBox(tr("Textures"), this);
    QGroupBox* texPropertyBox    = new QGroupBox(tr("Selected Texture Properties"), this);

    QGridLayout* textureSizer                 = new QGridLayout(texBox);
    QGridLayout* selectedTexPropertiesSizer   = new QGridLayout(texPropertyBox);

    QGroupBox* texEnableDisableBox        = new QGroupBox(tr("glEnable/glDisable (Select up to 5 Textures)"), this);
    QHBoxLayout* texEnableDisableSizer = new QHBoxLayout(texEnableDisableBox);

    QHBoxLayout* texCombineModeScaleBoxSizer = new QHBoxLayout();

    QGroupBox* texCombineStaticBox = new QGroupBox(tr("GL_COMBINE Parameters"), this);

    QGridLayout* texCombineStaticBoxSizer= new QGridLayout(texCombineStaticBox);

    QString texNumArray[5];
    texNumArray[0] = tr("T0");
    texNumArray[1] = tr("T1");
    texNumArray[2] = tr("T2");
    texNumArray[3] = tr("T3");
    texNumArray[4] = tr("T4");
    QGroupBox *textureBox = new QGroupBox(tr("Selected Texture Unit"), this);
    QHBoxLayout *textureBoxSizer = new QHBoxLayout(textureBox);
    textureGroup = new QButtonGroup(this);

    for (int i=0; i<5; i++) {
        QRadioButton *rb = new QRadioButton(texNumArray[i], textureBox);
        textureBoxSizer->addWidget(rb);
        textureGroup->addButton(rb, i);
    }
    textureGroup->button(0)->setChecked(true);
    connect(textureGroup, SIGNAL(buttonClicked(int)), SLOT(OnRadioTextureNum()));

    QStringList texChooseArray;
    texChooseArray << tr("3Dlabs");
    texChooseArray << tr("3Dlabs Normal");
    texChooseArray << tr("Rust");
    texChooseArray << tr("Leopard");
    texChooseArray << tr("Eyeball");
    texChooseArray << tr("CobbleStone");
    texChooseArray << tr("CobbleStone Normal");
    texChooseArray << tr("Bricks");
    texChooseArray << tr("Bricks Normal");
    texChooseArray << tr("Stone Wall");
    texChooseArray << tr("Stone Wall Normal");
    texChooseArray << tr("Metal Sheet");
    texChooseArray << tr("Metal Sheet Normal");
    texChoose = new QComboBox(this);
    texChoose->addItems(texChooseArray);
    connect(texChoose, SIGNAL(activated(int)), SLOT(OnChoiceTextureChoose()));

    texEnvColorButton = new QColorButton(this);
    texEnvColorButton->setColor(glState->GetTexture(textureGroup->checkedId()).texEnvColor);
    connect(texEnvColorButton, SIGNAL(clicked()), SLOT(OnButton()));

    QLabel *texEnvButtonLabel = new QLabel(tr("GL_TEX_ENV_COLOR"), this);

    tex0CheckBox = new QCheckBox(tr("T0"), this);
    tex0CheckBox->setChecked(glState->GetTexture(0).textureEnabled);

    tex1CheckBox = new QCheckBox(tr("T1"), this);
    tex1CheckBox->setChecked(glState->GetTexture(1).textureEnabled);

    tex2CheckBox = new QCheckBox(tr("T2"), this);
    tex2CheckBox->setChecked(glState->GetTexture(2).textureEnabled);

    tex3CheckBox = new QCheckBox(tr("T3"), this);
    tex3CheckBox->setChecked(glState->GetTexture(3).textureEnabled);

    tex4CheckBox = new QCheckBox(tr("T4"), this);
    tex4CheckBox->setChecked(glState->GetTexture(4).textureEnabled);

    texEnableDisableSizer->addWidget(tex0CheckBox);
    texEnableDisableSizer->addWidget(tex1CheckBox);
    texEnableDisableSizer->addWidget(tex2CheckBox);
    texEnableDisableSizer->addWidget(tex3CheckBox);
    texEnableDisableSizer->addWidget(tex4CheckBox);

    texCheckBoxGroup = new QButtonGroup(this);
    texCheckBoxGroup->setExclusive(false);
    texCheckBoxGroup->addButton(tex0CheckBox, 0);
    texCheckBoxGroup->addButton(tex1CheckBox, 1);
    texCheckBoxGroup->addButton(tex2CheckBox, 2);
    texCheckBoxGroup->addButton(tex3CheckBox, 3);
    texCheckBoxGroup->addButton(tex4CheckBox, 4);
    connect(texCheckBoxGroup, SIGNAL(buttonClicked(int)), SLOT(OnCheckbox(int)));

    QString texApplyArray[6];
    texApplyArray[0] = tr("GL_REPLACE");
    texApplyArray[1] = tr("GL_MODULATE");
    texApplyArray[2] = tr("GL_DECAL");
    texApplyArray[3] = tr("GL_BLEND");
    texApplyArray[4] = tr("GL_ADD");
    texApplyArray[5] = tr("GL_COMBINE");
    QGroupBox *texApplyBox = new QGroupBox(tr("Texture Application Method (Fragment Shader)"), this);
    QVBoxLayout *texApplyBoxSizer = new QVBoxLayout(texApplyBox);
    texApplyGroup = new QButtonGroup(this);

    for (int i=0; i<6; i++) {
        QRadioButton *rb = new QRadioButton(texApplyArray[i], texApplyBox);
        texApplyBoxSizer->addWidget(rb);
        texApplyGroup->addButton(rb, i);
    }
    texApplyGroup->button(1)->setChecked(true);
    connect(texApplyGroup, SIGNAL(buttonClicked(int)), SLOT(OnRadioTexApply()));

    QStringList texCombineModeArray;
    texCombineModeArray << tr("GL_REPLACE");
    texCombineModeArray << tr("GL_MODULATE");
    texCombineModeArray << tr("GL_ADD");
    texCombineModeArray << tr("GL_ADD_SIGNED");
    texCombineModeArray << tr("GL_INTERPOLATE");
    texCombineModeArray << tr("GL_SUBTRACT");
    texCombineModeArray << tr("GL_DOT3_RGB");
    texCombineModeChoose = new QComboBox(this);
    texCombineModeChoose->addItems(texCombineModeArray);
    connect(texCombineModeChoose, SIGNAL(activated(int)), SLOT(OnChoiceTextureCombineMode()));

    QLabel* texCombineModeLabel  = new QLabel(tr("GL_COMBINE_RGB Mode"), this);
    QLabel* texCombineScaleLabel = new QLabel(tr("Combine Scale"), this);

    QStringList texCombineScaleArray;
    texCombineScaleArray << tr("1.0");
    texCombineScaleArray << tr("2.0");
    texCombineScaleArray << tr("4.0");
    texCombineScaleChoose = new QComboBox(this);
    texCombineScaleChoose->addItems(texCombineScaleArray);
    connect(texCombineScaleChoose, SIGNAL(activated(int)), SLOT(OnChoiceTextureCombineScale()));

    QStringList texCombineSrcnRGBArray;
    texCombineSrcnRGBArray << tr("GL_TEXTURE");
    texCombineSrcnRGBArray << tr("GL_CONSTANT");
    texCombineSrcnRGBArray << tr("GL_PRIMARY_COLOR");
    texCombineSrcnRGBArray << tr("GL_PREVIOUS");

    texCombineSrc0RGBChoose = new QComboBox(this);
    texCombineSrc0RGBChoose->addItems(texCombineSrcnRGBArray);
    connect(texCombineSrc0RGBChoose, SIGNAL(activated(int)), SLOT(OnChoiceTextureCombineSrc0RGB()));

    texCombineSrc1RGBChoose = new QComboBox(this);
    texCombineSrc1RGBChoose->addItems(texCombineSrcnRGBArray);
    connect(texCombineSrc1RGBChoose, SIGNAL(activated(int)), SLOT(OnChoiceTextureCombineSrc1RGB()));

    texCombineSrc2RGBChoose = new QComboBox(this);
    texCombineSrc2RGBChoose->addItems(texCombineSrcnRGBArray);
    connect(texCombineSrc2RGBChoose, SIGNAL(activated(int)), SLOT(OnChoiceTextureCombineSrc2RGB()));

    QLabel* texCombineSrcLabel = new QLabel(tr("Source"), this);
    QLabel* texCombineOperandLabel = new QLabel(tr("Operand"), this);

    QStringList texCombineOperandArray;
    texCombineOperandArray << tr("GL_SRC_COLOR");
    texCombineOperandArray << tr("GL_ONE_MINUS_SRC_COLOR");

    texCombineOperandArg0Choose = new QComboBox(this);
    texCombineOperandArg0Choose->addItems(texCombineOperandArray);
    connect(texCombineOperandArg0Choose, SIGNAL(activated(int)), SLOT(OnChoiceTextureCombineOperandArg0()));

    texCombineOperandArg1Choose = new QComboBox(this);
    texCombineOperandArg1Choose->addItems(texCombineOperandArray);
    connect(texCombineOperandArg1Choose, SIGNAL(activated(int)), SLOT(OnChoiceTextureCombineOperandArg1()));

    texCombineOperandArg2Choose = new QComboBox(this);
    texCombineOperandArg2Choose->addItems(texCombineOperandArray);
    connect(texCombineOperandArg2Choose, SIGNAL(activated(int)), SLOT(OnChoiceTextureCombineOperandArg2()));

    QLabel* texCombineArg0Label = new QLabel(tr("Arg0"), this);

    QLabel* texCombineArg1Label = new QLabel(tr("Arg1"), this);
    QLabel* texCombineArg2Label = new QLabel(tr("Arg2"), this);

    selectedTexPropertiesSizer->addWidget(texApplyBox, 0, 0, 4, 1);

    selectedTexPropertiesSizer->addWidget(texEnvButtonLabel, 0, 1);
    selectedTexPropertiesSizer->addWidget(texEnvColorButton, 1, 1);
    selectedTexPropertiesSizer->addWidget(texChoose,         2, 1);

    texCombineModeScaleBoxSizer->addWidget(texCombineModeLabel);
    texCombineModeScaleBoxSizer->addWidget(texCombineModeChoose);
    texCombineModeScaleBoxSizer->addWidget(texCombineScaleLabel);
    texCombineModeScaleBoxSizer->addWidget(texCombineScaleChoose);

    texCombineStaticBoxSizer->addLayout(texCombineModeScaleBoxSizer,   0, 0, 1, 4, Qt::AlignLeft);

    texCombineStaticBoxSizer->addWidget(texCombineArg0Label, 2, 0, Qt::AlignHCenter);
    texCombineStaticBoxSizer->addWidget(texCombineArg1Label, 3, 0, Qt::AlignHCenter);
    texCombineStaticBoxSizer->addWidget(texCombineArg2Label, 4, 0, Qt::AlignHCenter);

    texCombineStaticBoxSizer->addWidget(texCombineSrcLabel,      1, 1, Qt::AlignHCenter);
    texCombineStaticBoxSizer->addWidget(texCombineSrc0RGBChoose, 2, 1);
    texCombineStaticBoxSizer->addWidget(texCombineSrc1RGBChoose, 3, 1);
    texCombineStaticBoxSizer->addWidget(texCombineSrc2RGBChoose, 4, 1);

    texCombineStaticBoxSizer->addWidget(texCombineOperandLabel,      1, 2, Qt::AlignHCenter);
    texCombineStaticBoxSizer->addWidget(texCombineOperandArg0Choose, 2, 2);
    texCombineStaticBoxSizer->addWidget(texCombineOperandArg1Choose, 3, 2);
    texCombineStaticBoxSizer->addWidget(texCombineOperandArg2Choose, 4, 2);

    texCombineStaticBoxSizer->setRowStretch(4, 2);

    textureSizer->addWidget(texEnableDisableBox, 0, 0);
    textureSizer->addWidget(textureBox,          1, 0);
    textureSizer->addWidget(texPropertyBox,      2, 0, 1, 2);

    texMainSizer->addWidget(texBox, 0, 0, Qt::AlignTop);
    texMainSizer->addWidget(texCombineStaticBox, 0, 1, Qt::AlignTop);
    texMainSizer->setColumnStretch(2, 2);

    DisableCombine();

    UpdateWidgets();
}

void SGOglTextureEnvNBPage::OnRadioTexApply()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());

    switch(texApplyGroup->checkedId()){
    case 0:
        texture.textureApplicationMethod = GL_REPLACE;
        DisableCombine();
        break;
    case 1:
        texture.textureApplicationMethod = GL_MODULATE;
        DisableCombine();
        break;
    case 2:
        texture.textureApplicationMethod = GL_DECAL;
        DisableCombine();
        break;
    case 3:
        texture.textureApplicationMethod = GL_BLEND;
        DisableCombine();
        break;
    case 4:
        texture.textureApplicationMethod = GL_ADD;
        DisableCombine();
        break;
    case 5:
        texture.textureApplicationMethod = GL_COMBINE;
        EnableCombine();
        break;
    default:
        texture.textureApplicationMethod = GL_MODULATE;
        DisableCombine();
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnRadioTextureNum()
{
    UpdateWidgets();

    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnCheckbox(int index)
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    glState->SetTextureEnable(texCheckBoxGroup->checkedId() != -1);
    switch(index){
    case 0:
        glState->GetTexture(0).textureEnabled = tex0CheckBox->isChecked();
        break;
    case 1:
        glState->GetTexture(1).textureEnabled = tex1CheckBox->isChecked();
        break;
    case 2:
        glState->GetTexture(2).textureEnabled = tex2CheckBox->isChecked();
        break;
    case 3:
        glState->GetTexture(3).textureEnabled = tex3CheckBox->isChecked();
        break;
    case 4:
        glState->GetTexture(4).textureEnabled = tex4CheckBox->isChecked();
        break;
    default:
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnChoiceTextureChoose()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());
    texture.textureCurrentSelection = (SGTextures::TextureId)texChoose->currentIndex();
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineMode()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());
    switch(texCombineModeChoose->currentIndex()){
    case 0:
        texture.textureCombineMode = GL_REPLACE;
        break;
    case 1:
        texture.textureCombineMode = GL_MODULATE;
        break;
    case 2:
        texture.textureCombineMode = GL_ADD;
        break;
    case 3:
        texture.textureCombineMode = GL_ADD_SIGNED;
        break;
    case 4:
        texture.textureCombineMode = GL_INTERPOLATE;
        break;
    case 5:
        texture.textureCombineMode = GL_SUBTRACT;
        break;
    case 6:
        texture.textureCombineMode = GL_DOT3_RGB;
        break;
    default:
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineOperandArg0()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());
    switch(texCombineOperandArg0Choose->currentIndex()){
    case 0:
        texture.textureCombineOperand0 = GL_SRC_COLOR;
        break;
    case 1:
        texture.textureCombineOperand0 = GL_ONE_MINUS_SRC_COLOR;
        break;
    default:
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineOperandArg1()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());
    switch(texCombineOperandArg1Choose->currentIndex()){
    case 0:
        texture.textureCombineOperand1 = GL_SRC_COLOR;
        break;
    case 1:
        texture.textureCombineOperand1 = GL_ONE_MINUS_SRC_COLOR;
        break;
    default:
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineOperandArg2()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());
    switch(texCombineOperandArg2Choose->currentIndex()){
    case 0:
        texture.textureCombineOperand2 = GL_SRC_COLOR;
        break;
    case 1:
        texture.textureCombineOperand2 = GL_ONE_MINUS_SRC_COLOR;
        break;
    default:
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineScale()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());
    texture.textureCombineScale = (float)(1 << texCombineScaleChoose->currentIndex());
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineSrc0RGB()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());
    switch(texCombineSrc0RGBChoose->currentIndex()){
    case 0:
        texture.textureCombineSource0 = GL_TEXTURE;
        break;
    case 1:
        texture.textureCombineSource0 = GL_CONSTANT;
        break;
    case 2:
        texture.textureCombineSource0 = GL_PRIMARY_COLOR;
        break;
    case 3:
        texture.textureCombineSource0 = GL_PREVIOUS;
        break;
    default:
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineSrc1RGB()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());
    switch(texCombineSrc1RGBChoose->currentIndex()){
    case 0:
        texture.textureCombineSource1 = GL_TEXTURE;
        break;
    case 1:
        texture.textureCombineSource1 = GL_CONSTANT;
        break;
    case 2:
        texture.textureCombineSource1 = GL_PRIMARY_COLOR;
        break;
    case 3:
        texture.textureCombineSource1 = GL_PREVIOUS;
        break;
    default:
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnChoiceTextureCombineSrc2RGB()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());
    switch(texCombineSrc2RGBChoose->currentIndex()){
    case 0:
        texture.textureCombineSource2 = GL_TEXTURE;
        break;
    case 1:
        texture.textureCombineSource2 = GL_CONSTANT;
        break;
    case 2:
        texture.textureCombineSource2 = GL_PRIMARY_COLOR;
        break;
    case 3:
        texture.textureCombineSource2 = GL_PREVIOUS;
        break;
    default:
        break;
    }
    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::OnButton()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);

    int textureSelected = textureGroup->checkedId();

    QColor texEnvColor = texEnvColorButton->color();
    glState->GetTexture(textureSelected).texEnvColor = texEnvColor;

    m_parent->GetFrame()->SetCanvasMode(SGCanvasWrapper::GLModeChoiceFixed);
    m_parent->GetFrame()->GetCanvas()->updateGL();
}

void SGOglTextureEnvNBPage::DisableCombine()
{
    texCombineOperandArg0Choose->setEnabled(false);
    texCombineOperandArg1Choose->setEnabled(false);
    texCombineOperandArg2Choose->setEnabled(false);
    texCombineModeChoose       ->setEnabled(false);
    texCombineScaleChoose      ->setEnabled(false);
    texCombineSrc0RGBChoose    ->setEnabled(false);
    texCombineSrc1RGBChoose    ->setEnabled(false);
    texCombineSrc2RGBChoose    ->setEnabled(false);
}

void SGOglTextureEnvNBPage::EnableCombine()
{
    texCombineOperandArg0Choose->setEnabled(true);
    texCombineOperandArg1Choose->setEnabled(true);
    texCombineOperandArg2Choose->setEnabled(true);
    texCombineModeChoose       ->setEnabled(true);
    texCombineScaleChoose      ->setEnabled(true);
    texCombineSrc0RGBChoose    ->setEnabled(true);
    texCombineSrc1RGBChoose    ->setEnabled(true);
    texCombineSrc2RGBChoose    ->setEnabled(true);
}

void SGOglTextureEnvNBPage::UpdateWidgets()
{
    SGFixedGLState* glState = m_parent->GetGLState();
    glState->SetTextureChanged(true);
    Texture& texture = glState->GetTexture(textureGroup->checkedId());

    texChoose->setCurrentIndex(texture.textureCurrentSelection);

    if(texture.textureCombineScale < 1.9)
    {
        texCombineScaleChoose->setCurrentIndex(COMBINE_SCALE_1_0);
    }
    else if(texture.textureCombineScale < 2.9)
    {
        texCombineScaleChoose->setCurrentIndex(COMBINE_SCALE_2_0);
    }
    else
    {
        texCombineScaleChoose->setCurrentIndex(COMBINE_SCALE_4_0);
    }

    switch(texture.textureApplicationMethod){
    case GL_REPLACE:
        texApplyGroup->button(TEXTURE_APPLICATION_METHOD_REPLACE)->setChecked(true);
        DisableCombine();
        break;
    case GL_MODULATE:
        texApplyGroup->button(TEXTURE_APPLICATION_METHOD_MODULATE)->setChecked(true);
        DisableCombine();
        break;
    case GL_DECAL:
        texApplyGroup->button(TEXTURE_APPLICATION_METHOD_DECAL)->setChecked(true);
        DisableCombine();
        break;
    case GL_BLEND:
        texApplyGroup->button(TEXTURE_APPLICATION_METHOD_BLEND)->setChecked(true);
        DisableCombine();
        break;
    case GL_ADD:
        texApplyGroup->button(TEXTURE_APPLICATION_METHOD_ADD)->setChecked(true);
        DisableCombine();
        break;
    case GL_COMBINE:
        texApplyGroup->button(TEXTURE_APPLICATION_METHOD_COMBINE)->setChecked(true);
        EnableCombine();
        break;
    default:
        texApplyGroup->button(TEXTURE_APPLICATION_METHOD_MODULATE)->setChecked(true);
        DisableCombine();
        break;
    }
    switch(texture.textureCombineMode){
    case GL_REPLACE:
        texCombineModeChoose->setCurrentIndex(COMBINE_MODE_REPLACE);
        break;
    case GL_MODULATE:
        texCombineModeChoose->setCurrentIndex(COMBINE_MODE_MODULATE);
        break;
    case GL_ADD:
        texCombineModeChoose->setCurrentIndex(COMBINE_MODE_ADD);
        break;
    case GL_ADD_SIGNED:
        texCombineModeChoose->setCurrentIndex(COMBINE_MODE_ADD_SIGNED);
        break;
    case GL_INTERPOLATE:
        texCombineModeChoose->setCurrentIndex(COMBINE_MODE_INTERPOLATE);
        break;
    case GL_SUBTRACT:
        texCombineModeChoose->setCurrentIndex(COMBINE_MODE_SUBTRACT);
        break;
    case GL_DOT3_RGB:
        texCombineModeChoose->setCurrentIndex(COMBINE_MODE_DOT3_RGB);
        break;
    default:
        texCombineModeChoose->setCurrentIndex(COMBINE_MODE_MODULATE);
    }
    switch(texture.textureCombineSource0){
    case GL_TEXTURE:
        texCombineSrc0RGBChoose->setCurrentIndex(COMBINE_SOURCE_TEXTURE);
        break;
    case GL_CONSTANT:
        texCombineSrc0RGBChoose->setCurrentIndex(COMBINE_SOURCE_CONSTANT);
        break;
    case GL_PRIMARY_COLOR:
        texCombineSrc0RGBChoose->setCurrentIndex(COMBINE_SOURCE_PRIMARY_COLOR);
        break;
    case GL_PREVIOUS:
        texCombineSrc0RGBChoose->setCurrentIndex(COMBINE_SOURCE_PREVIOUS);
        break;
    default:
        break;
    }
    switch(texture.textureCombineSource1){
    case GL_TEXTURE:
        texCombineSrc1RGBChoose->setCurrentIndex(COMBINE_SOURCE_TEXTURE);
        break;
    case GL_CONSTANT:
        texCombineSrc1RGBChoose->setCurrentIndex(COMBINE_SOURCE_CONSTANT);
        break;
    case GL_PRIMARY_COLOR:
        texCombineSrc1RGBChoose->setCurrentIndex(COMBINE_SOURCE_PRIMARY_COLOR);
        break;
    case GL_PREVIOUS:
        texCombineSrc1RGBChoose->setCurrentIndex(COMBINE_SOURCE_PREVIOUS);
        break;
    default:
        break;
    }
    switch(texture.textureCombineSource2){
    case GL_TEXTURE:
        texCombineSrc2RGBChoose->setCurrentIndex(COMBINE_SOURCE_TEXTURE);
        break;
    case GL_CONSTANT:
        texCombineSrc2RGBChoose->setCurrentIndex(COMBINE_SOURCE_CONSTANT);
        break;
    case GL_PRIMARY_COLOR:
        texCombineSrc2RGBChoose->setCurrentIndex(COMBINE_SOURCE_PRIMARY_COLOR);
        break;
    case GL_PREVIOUS:
        texCombineSrc2RGBChoose->setCurrentIndex(COMBINE_SOURCE_PREVIOUS);
        break;
    default:
        break;
    }
    switch(texture.textureCombineOperand0){
    case GL_SRC_COLOR:
        texCombineOperandArg0Choose->setCurrentIndex(COMBINE_OPERAND_SRC_COLOR);
        break;
    case GL_ONE_MINUS_SRC_COLOR:
        texCombineOperandArg0Choose->setCurrentIndex(COMBINE_OPERAND_ONE_MINUS_SRC_COLOR);
        break;
    default:
        break;
    }
    switch(texture.textureCombineOperand1){
    case GL_SRC_COLOR:
        texCombineOperandArg1Choose->setCurrentIndex(COMBINE_OPERAND_SRC_COLOR);
        break;
    case GL_ONE_MINUS_SRC_COLOR:
        texCombineOperandArg1Choose->setCurrentIndex(COMBINE_OPERAND_ONE_MINUS_SRC_COLOR);
        break;
    default:
        break;
    }
    switch(texture.textureCombineOperand2){
    case GL_SRC_COLOR:
        texCombineOperandArg2Choose->setCurrentIndex(COMBINE_OPERAND_SRC_COLOR);
        break;
    case GL_ONE_MINUS_SRC_COLOR:
        texCombineOperandArg2Choose->setCurrentIndex(COMBINE_OPERAND_ONE_MINUS_SRC_COLOR);
        break;
    default:
        break;
    }
    texEnvColorButton->setColor(glState->GetTexture(textureGroup->checkedId()).texEnvColor);
}
