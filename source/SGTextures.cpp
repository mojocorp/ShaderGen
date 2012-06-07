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

#include "SGTextures.h"
#include <wx/image.h>
#include "SGFixedGLState.h"
#include "SGOglNotebook.h"
#include "App.h"
#include <png/png.h>
#include "UtilityFunctions.h"

const wxString folder = "../textures/";

SGTextures::SGTextures(SGOglNotebook *parent)
{
    m_parent = parent;
    TextureNames[0] = "3Dlabs.bmp";
    TextureNames[1] = "3DlabsNormal.bmp";
    TextureNames[2] = "rust.bmp";
    TextureNames[3] = "leopard.bmp";
    TextureNames[4] = "eyeball.bmp";
    TextureNames[5] = "cobblestonesDiffuse.bmp";
    TextureNames[6] = "cobblestonesNormal.bmp";
    TextureNames[7] = "bricksDiffuse.bmp";
    TextureNames[8] = "bricksNormal.bmp";
    TextureNames[9] = "stonewallDiffuse.bmp";
    TextureNames[10] = "stonewallNormal.bmp";
    TextureNames[11] = "metalSheetDiffuse.bmp";
    TextureNames[12] = "metalSheetNormal.bmp";
    logo = 0;
    memset( textures, 0, sizeof(textures) );
}

SGTextures::~SGTextures()
{
}

//Since wxWidgets does not handle alpha values correctly, we need this function.
bool SGTextures::LoadPng(const char* filename)
{
    png_struct_def* png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    png_info_struct* info = png_create_info_struct(png);
    png_info_struct* end = png_create_info_struct(png);

    FILE* fp = fopen(filename, "rb");
    if (!fp)
    {
        wxGetApp().Errorf("Unable to open texture file '%s'.\n", filename);
        return false;
    }
    png_init_io(png, fp);
    png_read_info(png, info);
    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte colorType = png_get_color_type(png, info);
    if (colorType != PNG_COLOR_TYPE_RGB_ALPHA && colorType != PNG_COLOR_TYPE_RGB)
    {
        wxGetApp().Errorf("Texture file '%s' uses an unsupported format.  For PNG files only RGB and RGBA are supported.\n", filename);
        return false;
    }

    bool alpha = colorType == PNG_COLOR_TYPE_RGB_ALPHA;
    int components = alpha ? 4 : 3;

    if (png_get_bit_depth(png, info) != 8)
    {
        wxGetApp().Errorf("Texture file '%s' uses an unsupported bit depth.  For PNG files only 8 bpp is supported.\n", filename);
        return false;
    }

    png_bytepp rows = new png_bytep[height];
    png_bytep image = new png_byte[height * width * components];

    for (int i = 0; i < height; i++)
    {
        rows[i] = image + i * width * components;
    }

    png_set_rows(png, info, rows);
    png_read_image(png, rows);
    gluBuild2DMipmaps(GL_TEXTURE_2D, components, width, height, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);

    png_destroy_read_struct(&png, &info, &end);
    fclose(fp);
    delete[] image;
    delete[] rows;
    return alpha;
}

void SGTextures::ActivateLogo()
{
    for(int i=0;i<5;i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glDisable(GL_TEXTURE_2D);
    }
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    glEnable(GL_TEXTURE_2D); 

    if(logo)
    {
        glBindTexture( GL_TEXTURE_2D, logo);
        return;
    }
    else
    {
        glGenTextures(1, &logo);
        glBindTexture( GL_TEXTURE_2D, logo);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(!LoadPng(folder + "3Dlabs.png"))
    {
        wxGetApp().Errorf("Unable to load image 3Dlabs logo");
        return;
    }
}

void SGTextures::Activate(Id id, GLint unit)
{
    PrintOpenGLError();
    SGFixedGLState *glState = m_parent->GetGLState();
    PrintOpenGLError();

    glActiveTexture(GL_TEXTURE0 + unit);

    PrintOpenGLError();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    PrintOpenGLError();

    int tempInteger;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &tempInteger);

    if(glState->GetTexture(unit)->texGen)
    {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, glState->GetTexture(unit)->textureCoordinateGeneration);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, glState->GetTexture(unit)->textureCoordinateGeneration);
        PrintOpenGLError();
        if(glState->GetTexture(unit)->textureCoordinateGeneration == GL_OBJECT_LINEAR)
        {
            glTexGenfv(GL_S, GL_OBJECT_PLANE, glState->GetTexture(unit)->objectPlaneCoeffS); 
            glTexGenfv(GL_T, GL_OBJECT_PLANE, glState->GetTexture(unit)->objectPlaneCoeffT);        
        }
        PrintOpenGLError();
        if(glState->GetTexture(unit)->textureCoordinateGeneration == GL_EYE_LINEAR)
        {
            PrintOpenGLError();
            glTexGenfv(GL_S, GL_EYE_PLANE, glState->GetTexture(unit)->eyePlaneCoeffS);
            PrintOpenGLError();
            glTexGenfv(GL_T, GL_EYE_PLANE, glState->GetTexture(unit)->eyePlaneCoeffT);
        }
        PrintOpenGLError();
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    }

    PrintOpenGLError();

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, glState->GetTexture(unit)->textureApplicationMethod);

    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, glState->GetTexture(unit)->texEnvColor);

    PrintOpenGLError();

    if(glState->GetTexture(unit)->textureApplicationMethod == GL_COMBINE)
    {
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, glState->GetTexture(unit)->textureCombineMode);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, glState->GetTexture(unit)->textureCombineSource0);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, glState->GetTexture(unit)->textureCombineSource1);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_RGB, glState->GetTexture(unit)->textureCombineSource2);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, glState->GetTexture(unit)->textureCombineOperand0);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, glState->GetTexture(unit)->textureCombineOperand1);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, glState->GetTexture(unit)->textureCombineOperand2);
        glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, glState->GetTexture(unit)->textureCombineScale);
    }

    glEnable(GL_TEXTURE_2D);

    int index = id - Id::TexturesStart;

    if( textures[index].glid )
    {
        glBindTexture( GL_TEXTURE_2D, textures[index].glid); 
        return;
    }
    else
    {
        glGenTextures(1, &textures[index].glid);
        glBindTexture(GL_TEXTURE_2D, textures[index].glid);
    }

    PrintOpenGLError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    wxImage image( folder + TextureNames[index] );
    image = image.Mirror(FALSE);

    if( !image.Ok())
    {
        wxGetApp().Errorf("Unable to load image %s", TextureNames[index].c_str());
        return;
    }
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetData());

}

void SGTextures::Deactivate(GLint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glDisable(GL_TEXTURE_2D);
}