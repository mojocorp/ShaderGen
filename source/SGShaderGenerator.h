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

#pragma once

#include "Compulsory.h"
#include "globals.h"

class SGShaderGenerator
{
public:
    SGShaderGenerator();
    ~SGShaderGenerator();
    const char* BuildVertexShader();
    const char* BuildFragmentShader();

private:
    typedef struct _currentTextureState{
        GLboolean textureCoordinateGenerationReflection;
        GLboolean textureCoordinateGenerationSphereMap;
        GLboolean textureCoordinateGenerationEye;
        GLboolean textureCoordinateGenerationObject;
        GLboolean textureCoordinateGenerationNormal;
        GLboolean textureEnabled;
        GLboolean textureGenerationEnableArray[NUM_TEXTURE_COORDS];
    } CurrentTextureState;

    wxString fragShader;
    wxString vertShader;

    bool fLightPoint, fLightSpot, fLightDir, fLightDirSpot;
    bool fMapSphere, fMapReflection, vTexGenEnable;

    GLboolean texGenReflection;
    GLboolean texGenSphereMap;
    GLboolean texGenEye;
    GLboolean texGenObject;
    GLboolean texGenNormal;
    GLboolean texturesEnabled;

    //NUKE!!!!!!!!!!!
    GLboolean texGenEnableArray[NUM_TEXTURE_COORDS];
    
    CurrentTextureState currentTexture[NUM_TEXTURES];

    void InitTextures();
    void BuildFragFog(wxString &str);
    void BuildFragTex(wxString &str);

    void BuildFragSeparateSpecularColor(wxString &str);

    void BuildLightCode  ( wxString &str );
    void BuildVertMain   ( wxString &str );
    void BuildFuncFnormal( wxString &str );
    void BuildFuncFog    ( wxString &str );
    void BuildFuncPoint  ( wxString &str );
    void BuildTexCoord   ( wxString &str );

    void AddFuncLightDirectional   (wxString &str) const;
    void AddFuncLightPoint         (wxString &str) const;
    void AddFuncLightSpot          (wxString &str) const;
    void AddFuncSphereMap          (wxString &str) const;
    void AddFuncReflectionMap      (wxString &str) const;
    void AddFuncLightSpotDirection (wxString &str) const;
};
