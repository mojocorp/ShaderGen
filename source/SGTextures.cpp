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

#include <QApplication>
#include <QMessageBox>

#include "SGFixedGLState.h"
#include "SGFrame.h"
#include "SGTextures.h"

#include <QOpenGLTexture>

#include "UtilityFunctions.h"
#include <stdio.h>

SGTextures::SGTextures(SGFrame* frame, SGFixedGLState* state)
  : m_frame(frame)
  , m_glState(state)
{
    m_textureNames[0] = "3Dlabs.png";
    m_textureNames[1] = "3DlabsNormal.png";
    m_textureNames[2] = "rust.png";
    m_textureNames[3] = "Leopard.png";
    m_textureNames[4] = "eyeball.png";
    m_textureNames[5] = "cobblestonesDiffuse.png";
    m_textureNames[6] = "cobblestonesNormal.png";
    m_textureNames[7] = "bricksDiffuse.png";
    m_textureNames[8] = "bricksNormal.png";
    m_textureNames[9] = "stonewallDiffuse.png";
    m_textureNames[10] = "stonewallNormal.png";
    m_textureNames[11] = "metalSheetDiffuse.png";
    m_textureNames[12] = "metalSheetNormal.png";

    for (int i = 0; i < 13; i++) {
        m_textures[i] = NULL;
    }
}

SGTextures::~SGTextures()
{
}

void
SGTextures::bind(TextureId id, GLint unit)
{
    for (int i = 0; i < 13; i++) {
        if (!m_textures[i]) {
            const QImage image = QImage(":/textures/" + m_textureNames[i]).mirrored();
            if (image.isNull()) {
                QMessageBox::critical(
                  m_frame, "GLSL ShaderGen",
                  QString("Unable to load image %1").arg(m_textureNames[i]));
                continue;
            }
            m_textures[i] = new QOpenGLTexture(image, QOpenGLTexture::GenerateMipMaps);
            m_textures[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            m_textures[i]->setMagnificationFilter(QOpenGLTexture::Linear);
            m_textures[i]->setWrapMode(QOpenGLTexture::Repeat);
        }
    }
    PrintOpenGLError();

    glActiveTexture(GL_TEXTURE0 + unit);

    PrintOpenGLError();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    PrintOpenGLError();

    int tempInteger;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &tempInteger);

    if (m_glState->getTexture(unit).texGen) {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE,
                  m_glState->getTexture(unit).textureCoordinateGeneration);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE,
                  m_glState->getTexture(unit).textureCoordinateGeneration);
        PrintOpenGLError();
        if (m_glState->getTexture(unit).textureCoordinateGeneration == GL_OBJECT_LINEAR) {
            glTexGenf(GL_S, GL_OBJECT_PLANE, m_glState->getTexture(unit).objectPlaneCoeffS);
            glTexGenf(GL_T, GL_OBJECT_PLANE, m_glState->getTexture(unit).objectPlaneCoeffT);
        }
        PrintOpenGLError();
        if (m_glState->getTexture(unit).textureCoordinateGeneration == GL_EYE_LINEAR) {
            PrintOpenGLError();
            glTexGenf(GL_S, GL_EYE_PLANE, m_glState->getTexture(unit).eyePlaneCoeffS);
            PrintOpenGLError();
            glTexGenf(GL_T, GL_EYE_PLANE, m_glState->getTexture(unit).eyePlaneCoeffT);
        }
        PrintOpenGLError();
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    }

    PrintOpenGLError();

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              m_glState->getTexture(unit).textureApplicationMethod);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, m_glState->getTexture(unit).texEnvColor);

    PrintOpenGLError();

    if (m_glState->getTexture(unit).textureApplicationMethod == GL_COMBINE) {
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, m_glState->getTexture(unit).textureCombineMode);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, m_glState->getTexture(unit).textureCombineSource0);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, m_glState->getTexture(unit).textureCombineSource1);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_RGB, m_glState->getTexture(unit).textureCombineSource2);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB,
                  m_glState->getTexture(unit).textureCombineOperand0);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB,
                  m_glState->getTexture(unit).textureCombineOperand1);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB,
                  m_glState->getTexture(unit).textureCombineOperand2);
        glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, m_glState->getTexture(unit).textureCombineScale);
    }

    glEnable(GL_TEXTURE_2D);

    m_textures[id]->bind(unit);
}

void
SGTextures::release(GLint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glDisable(GL_TEXTURE_2D);
}
