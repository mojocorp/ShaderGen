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

#include "SGOglNotebook.h"
#include "SGOglLightNBPage.h"
#include "SGOglMaterialNBPage.h"
#include "SGOglFogNBPage.h"
#include "SGOglTextureCoordNBPage.h"
#include "SGOglTextureEnvNBPage.h"
#include "SGFixedGLState.h"
#include "SGTextures.h"

SGOglNotebook::SGOglNotebook( wxWindow *parent, wxWindowID id)
: wxNotebook(parent, id, wxDefaultPosition, wxSize(800,300))
{
    glState = new SGFixedGLState();
    textures = new SGTextures(this);

    lightPage        = new SGOglLightNBPage        (this, wxID_ANY);
    materialPage     = new SGOglMaterialNBPage     (this, wxID_ANY);
    fogPage          = new SGOglFogNBPage          (this, wxID_ANY);
    textureCoordPage = new SGOglTextureCoordNBPage (this, wxID_ANY);
    textureEnvPage   = new SGOglTextureEnvNBPage   (this, wxID_ANY);

    AddPage(lightPage   , wxT("LIGHT"),   true );
    AddPage(materialPage, wxT("MATERIAL"), false);
    AddPage(fogPage     , wxT("FOG"), false);
    AddPage(textureCoordPage, wxT("TEXTURE COORDINATE SET"), false);
    AddPage(textureEnvPage, wxT("TEXTURE ENVIRONMENT SET"), false);

    SetAutoLayout(TRUE);
}

void SGOglNotebook::Clean()
{
    delete glState;
    delete textures;
}
