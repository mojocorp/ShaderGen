/************************************************************************
* Author(s):     Joshua Doss                                            *
* Last Modified: 11/08/2005                                             *
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
/************************************************************************
* This class creates the shader(s) to emulate the current               *
* OpenGL Fixed State.  This class does not query the state              *
* structures set up in the SGFixedGLState class, but rather             *
* queries the state using the OpenGL API.  For this reason,             *
* one can easily use this class as with an existing or new              *
* application to extract the fixed state and generate an equivalent     *
* shader.                                                               *
************************************************************************/

#include "SGShaderGenerator.h"
#include "UtilityFunctions.h"

const float LOG2E = 1.442695f;

SGShaderGenerator::SGShaderGenerator()
 : texturesEnabled(GL_FALSE), 
   fLightPoint(false),
   fLightSpot(false),
   fLightDir(false),
   fLightDirSpot(false),
   fMapSphere(false),
   fMapReflection(false),
   vTexGenEnable(false),
   texGenReflection(GL_FALSE),
   texGenSphereMap(GL_FALSE),
   texGenEye(GL_FALSE),
   texGenObject(GL_FALSE),
   texGenNormal(GL_FALSE),
   fragShader(wxT(" ")),
   vertShader(wxT(" ")),
   currentTexture()
{
    for(int i = 0; i < NUM_TEXTURES; i++)
    {
        currentTexture[i].textureCoordinateGenerationEye = GL_FALSE;
        currentTexture[i].textureCoordinateGenerationNormal = GL_FALSE;
        currentTexture[i].textureCoordinateGenerationObject = GL_FALSE;
        currentTexture[i].textureCoordinateGenerationReflection = GL_FALSE;
        currentTexture[i].textureCoordinateGenerationSphereMap = GL_FALSE;
        currentTexture[i].textureEnabled = GL_FALSE;
        for(int k = 0; k < NUM_TEXTURE_COORDS; k++)
        {
            currentTexture[i].textureGenerationEnableArray[k] = GL_FALSE;
        }
    }
}

SGShaderGenerator::~SGShaderGenerator()
{
    fragShader.Clear();
    vertShader.Clear();
}

void SGShaderGenerator::InitTextures()
{
    vTexGenEnable = false;
    texturesEnabled = false;

    for(int i=0;i<NUM_TEXTURES;i++)
    {
        currentTexture[i].textureCoordinateGenerationReflection = false;
        currentTexture[i].textureCoordinateGenerationSphereMap = false;
        currentTexture[i].textureCoordinateGenerationEye = false;
        currentTexture[i].textureCoordinateGenerationObject = false;
        currentTexture[i].textureCoordinateGenerationNormal = false;
        currentTexture[i].textureEnabled = false;

        glActiveTexture(GL_TEXTURE0 + i);

        currentTexture[i].textureEnabled = glIsEnabled(GL_TEXTURE_2D);

        if(currentTexture[i].textureEnabled == GL_TRUE)
        {

            int tempIntS, tempIntT, tempIntR, tempIntQ;

            vTexGenEnable = true;
            texturesEnabled = true;

            glGetTexGeniv(GL_S, GL_TEXTURE_GEN_MODE, &tempIntS);
            glGetTexGeniv(GL_T, GL_TEXTURE_GEN_MODE, &tempIntT);
            glGetTexGeniv(GL_R, GL_TEXTURE_GEN_MODE, &tempIntR);
            glGetTexGeniv(GL_Q, GL_TEXTURE_GEN_MODE, &tempIntQ);

            currentTexture[i].textureGenerationEnableArray[0] = glIsEnabled(GL_TEXTURE_GEN_S);
            currentTexture[i].textureGenerationEnableArray[1] = glIsEnabled(GL_TEXTURE_GEN_T);
            currentTexture[i].textureGenerationEnableArray[2] = glIsEnabled(GL_TEXTURE_GEN_R);        
            currentTexture[i].textureGenerationEnableArray[3] = glIsEnabled(GL_TEXTURE_GEN_Q);

            //The following block queries the GL State to see if there are any texture coordinates
            //  being generated automatically.  If automatic texture coordination is being used,
            //  it marks the appropriate boolean to true, thus enabling the creation of the 
            //  vertex shader user defined functions that reproduce the coordinate generation.
            if( currentTexture[i].textureGenerationEnableArray[0] ||
                currentTexture[i].textureGenerationEnableArray[1] ||
                currentTexture[i].textureGenerationEnableArray[2] ||
                currentTexture[i].textureGenerationEnableArray[3])
            {
                currentTexture[i].textureCoordinateGenerationReflection = (tempIntS == GL_REFLECTION_MAP || tempIntT == GL_REFLECTION_MAP ||
                    tempIntR == GL_REFLECTION_MAP);
                currentTexture[i].textureCoordinateGenerationSphereMap = (tempIntS == GL_SPHERE_MAP || tempIntT == GL_SPHERE_MAP);
                currentTexture[i].textureCoordinateGenerationEye = (tempIntS == GL_EYE_LINEAR && tempIntT == GL_EYE_LINEAR && 
                    tempIntQ == GL_EYE_LINEAR && tempIntR == GL_EYE_LINEAR);
                currentTexture[i].textureCoordinateGenerationObject = (tempIntS == GL_OBJECT_LINEAR || tempIntT == GL_OBJECT_LINEAR || 
                    tempIntQ == GL_OBJECT_LINEAR || tempIntR == GL_OBJECT_LINEAR);
                currentTexture[i].textureCoordinateGenerationNormal = (tempIntS == GL_NORMAL_MAP || tempIntT == GL_NORMAL_MAP || 
                    tempIntQ == GL_NORMAL_MAP || tempIntR == GL_NORMAL_MAP);
            }
        }
    }
}

/**********************************************
* FRAGMENT SHADER
**********************************************/

const char* SGShaderGenerator::BuildFragmentShader()
{    
    fragShader = wxT(""
                    "/*******************************************************\n"
                    "* Fixed.frag Fixed Function Equivalent Fragment Shader *\n"
                    "*   Automatically Generated by 3Dlabs GLSL ShaderGen   *\n"
                    "*             http://developer.3dlabs.com              *\n"
                    "*******************************************************/\n");

    InitTextures();

    for(int i=0; i<NUM_TEXTURES; i++)
    {
        if(currentTexture[i].textureEnabled)
        {
            fragShader += wxString::Format(wxT("uniform sampler2D texUnit%i;\n"), i);
            texturesEnabled = true;
        }
    }

    fragShader +=   wxT("\n"
                    "void main (void) \n"
                    "{\n"
                    "    vec4 color;\n"
                    "\n"
                    "    color = gl_Color;\n\n");

    BuildFragTex(fragShader);

    //No GUI Support in ShaderGen 1.0/2.0/3.0
    BuildFragSeparateSpecularColor(fragShader);

    BuildFragFog(fragShader);

    fragShader +=   wxT("    gl_FragColor = color;\n"
                    "}\n");
 
    return fragShader.c_str();
}

void SGShaderGenerator::BuildFragFog(wxString &str)
{
    double fogDensity;
    double preComputedValue;
    int fogMode;
    wxString stringTemp;

    glGetIntegerv(GL_FOG_MODE, &fogMode);
    glGetDoublev(GL_FOG_DENSITY, &fogDensity);

    if(glIsEnabled(GL_FOG))
    {
        str += wxT("\n    float fog;\n\n");

        //In previous releases of GLSL ShaderGen, the fog calculation was 
        //done entirely in the shader.  To educate developers on best
        //shader programming practices, we've moved the calculation of 
        //(-gl_Fog.density * LOG2E) since these values rarely change they 
        //should be calculated by the application and used as constants 
        //in the shader for the most efficient results.

        //if(fogMode == GL_EXP || fogMode == GL_EXP2)
        //    str += "        const float LOG2E = 1.442695;
        
        if (fogMode == GL_LINEAR)
        {
            str += wxT("    fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;\n\n");
        } 
        else if (fogMode == GL_EXP) 
        {
            preComputedValue = -fogDensity * LOG2E;
            
            stringTemp = wxT(DoubleToString1(preComputedValue));

            str += wxT("\n      //fog = exp2(-gl_Fog.density * gl_FogFragCoord * LOG2E);\n"
                   "\n      //The equation for fog allows us to move a portion of \n"
                   "        //the calculation (-gl_Fog.density * LOG2E) to the CPU\n"
                   "        //increasing the efficiency of our shader by doing a \n"
                   "        //scalar multiplication on the CPU rather than the GPU.\n\n"
                   "        fog = exp2(gl_FogFragCoord *");
            
            str += stringTemp;
            str += wxT(");\n\n");

            //In the first 2 releases of ShaderGen, the fog calculation was 
            //done entirely in the shader.  To educate developers on best
            //shader programming practices, we've moved the calculation of 
            //(-gl_Fog.density * LOG2E) since these values rarely change they 
            //should be calculated by the application and used as constants 
            //in the shader for the most efficient results.

            //str += wxT("        fog = exp2(-gl_Fog.density * gl_FogFragCoord * LOG2E);\n\n");
        }
        else if (fogMode == GL_EXP2)
        {
            preComputedValue = -fogDensity * fogDensity * LOG2E;
            
            stringTemp = wxT(DoubleToString1(preComputedValue));

            str += wxT("\n//    fog = exp2(-gl_Fog.density * gl_Fog.density *\n"
                   "    //      gl_FogFragCoord * gl_FogFragCoord * LOG2E);\n"
                   "\n  //The equation for fog allows us to move a portion of \n"
                   "    //the calculation (-gl_Fog.density * gl_Fog.density * LOG2E)\n"
                   "    //to the CPU increasing the efficiency of our shader by doing a \n"
                   "    //scalar multiplication on the CPU rather than the GPU.\n\n"
                   "    fog = exp2(gl_FogFragCoord * gl_FogFragCoord * ");

            str += stringTemp;
            str += wxT(");\n\n");
        }
        str += wxT("    fog = clamp(fog, 0.0, 1.0);\n\n"
               "    color = vec4(mix( vec3(gl_Fog.color), vec3(color), fog), color.a);\n\n");
    }
}

void SGShaderGenerator::BuildFragSeparateSpecularColor(wxString &str)
{
    int sepSpecInt;
    
    glGetIntegerv(GL_LIGHT_MODEL_COLOR_CONTROL, &sepSpecInt);

    if(sepSpecInt == GL_SEPARATE_SPECULAR_COLOR)
    {
        str += wxT("    color += gl_SecondaryColor;\n"
               "    color = clamp(color, 0.0, 1.0);\n\n");
    }
}

void SGShaderGenerator::BuildFragTex(wxString & str)
{
    wxString fragmentTextureString;

    for(int i=0;i<NUM_TEXTURES;i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        int texEnv;

        glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &texEnv);

        if(glIsEnabled(GL_TEXTURE_2D)) 
        {
            switch(texEnv){
                case GL_MODULATE:
                    fragmentTextureString.Printf(wxT("    color *= texture2D(texUnit%i, gl_TexCoord[%i].xy);\n\n"), i, i);
                    break;
                case GL_DECAL:
                    fragmentTextureString.Printf(wxT("    vec4 texture%i = texture2D(texUnit%i, gl_TexCoord[%i].xy);\n"
                        "    vec3 tempColor%i = mix(color.rgb, texture%i.rgb, texture%i.a);\n"
                        "    color = vec4 (tempColor%i, color.a);\n\n"), i,i,i,i,i,i,i);
                    break;
                case GL_BLEND:
                    fragmentTextureString.Printf(wxT("    vec4 texture%i = texture2D(texUnit%i, gl_TexCoord[%i].xy);\n"
                        "    vec3 tempColor%i = mix(color.rgb, gl_TextureEnvColor[%i].rgb, texture%i.rgb);\n"
                        "    color = vec4 (tempColor%i, color.a * texture%i.a);\n\n"), i,i,i,i,i,i,i,i);
                    break;
                case GL_REPLACE:
                    fragmentTextureString.Printf(wxT("    color = texture2D(texUnit%i, gl_TexCoord[%i].xy);\n\n"), i, i);
                    break;
                case GL_ADD:
                    fragmentTextureString.Printf(wxT("    vec4 texture%i = texture2D(texUnit%i, gl_TexCoord[%i].xy);\n"
                        "    color.rgb += texture%i.rgb;\n"
                        "    color.a   *= texture%i.a;\n"
                        "    color = clamp(color, 0.0, 1.0);\n\n"), i,i,i,i,i);
                    break;
                case GL_COMBINE:
                    int combineMode, combineSrc0, combineSrc1, combineSrc2;
                    int combineOperand0, combineOperand1, combineOperand2;

                    int combineRGBScale;

                    wxString Arg0, Arg1, Arg2, Operand0, Operand1, Operand2;

                    glGetTexEnviv(GL_TEXTURE_ENV, GL_COMBINE_RGB, &combineMode);
                    glGetTexEnviv(GL_TEXTURE_ENV, GL_SRC0_RGB, &combineSrc0);
                    glGetTexEnviv(GL_TEXTURE_ENV, GL_SRC1_RGB, &combineSrc1);
                    glGetTexEnviv(GL_TEXTURE_ENV, GL_SRC2_RGB, &combineSrc2);
                    glGetTexEnviv(GL_TEXTURE_ENV, GL_OPERAND0_RGB, &combineOperand0);
                    glGetTexEnviv(GL_TEXTURE_ENV, GL_OPERAND1_RGB, &combineOperand1);
                    glGetTexEnviv(GL_TEXTURE_ENV, GL_OPERAND2_RGB, &combineOperand2);
                    glGetTexEnviv(GL_TEXTURE_ENV, GL_RGB_SCALE, &combineRGBScale);

                    if(combineSrc0 == GL_TEXTURE)
                    {
                        if(combineOperand0 == GL_SRC_COLOR)
                        {
                            wxT(Arg0.Printf("    vec4 texture%iArg0 = texture2D(texUnit%i, gl_TexCoord[%i].xy);\n"
                                "    vec4 texUnit%iArg0 = texture%iArg0;\n", i, i, i, i, i));
                        }
                        else if(combineOperand0 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg0.Printf(wxT("    vec4 texture%iArg0 = texture2D(texUnit%i,gl_TexCoord[%i].xy);\n"
                                "        vec4 texUnit%iArg0 = vec4(1.0) - texture%iArg0;\n"), i, i, i, i, i);
                        }
                    }
                    else if(combineSrc0 == GL_CONSTANT)
                    {
                        if(combineOperand0 == GL_SRC_COLOR)
                        {
                            Arg0.Printf(wxT("    vec4 texUnit%iArg0 = gl_TextureEnvColor[%i];\n"), i, i);
                        }
                        else if(combineOperand0 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg0.Printf(wxT("    vec4 texUnit%iArg0 = vec4(1.0) - gl_TextureEnvColor[%i];\n"), i, i);
                        }
                    } 
                    else if(combineSrc0 == GL_PRIMARY_COLOR)
                    {
                        if(combineOperand0 == GL_SRC_COLOR)
                        {
                            Arg0.Printf(wxT("    vec4 texUnit%iArg0 = gl_Color;"), i);
                        }
                        else if(combineOperand0 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg0.Printf(wxT("    vec4 texUnit%iArg0 = vec4(1.0) - gl_Color;"), i);
                        }
                    }
                    else if(combineSrc0 == GL_PREVIOUS)
                    {
                        if(combineOperand0 == GL_SRC_COLOR)
                        {
                            Arg0.Printf(wxT("    vec4 texUnit%iArg0 = color;"), i);
                        }
                        else if(combineOperand0 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg0.Printf(wxT("    vec4 texUnit%iArg0 = vec4(1.0) - color;"), i);
                        }
                    }
                    if(combineSrc1 == GL_TEXTURE)
                    {
                        if(combineOperand1 == GL_SRC_COLOR)
                        {
                            Arg1.Printf(wxT("    vec4 texture%iArg1 = texture2D(texUnit%i,gl_TexCoord[%i].xy);\n"
                                "    vec4 texUnit%iArg1 = texture%iArg1;\n"), i, i, i, i, i);
                        }
                        else if(combineOperand1 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg1.Printf(wxT("    vec4 texture%iArg1 = texture2D(texUnit%i,gl_TexCoord[%i].xy);\n"
                                "    vec4 texUnit%iArg1 = vec4(1.0) - texture%iArg1;\n"), i, i, i, i, i);
                        }
                    }
                    else if(combineSrc1 == GL_CONSTANT)
                    {
                        if(combineOperand1 == GL_SRC_COLOR)
                        {
                            Arg1.Printf(wxT("    vec4 texUnit%iArg1 = gl_TextureEnvColor[%i];\n"), i, i);
                        }
                        else if(combineOperand1 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg1.Printf(wxT("    vec4 texUnit%iArg1 = vec4(1.0) - gl_TextureEnvColor[%i];\n"), i, i);
                        }
                    }
                    else if(combineSrc1 == GL_PRIMARY_COLOR)
                    {
                        if(combineOperand1 == GL_SRC_COLOR)
                        {
                            Arg1.Printf(wxT("    vec4 texUnit%iArg1 = gl_Color;\n"), i);
                        }
                        else if(combineOperand1 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg1.Printf(wxT("    vec4 texUnit%iArg1 = vec4(1.0) - gl_Color;\n"), i);
                        }
                    }
                    else if(combineSrc1 == GL_PREVIOUS)
                    {
                        if(combineOperand1 == GL_SRC_COLOR)
                        {
                            Arg1.Printf(wxT("    vec4 texUnit%iArg1 = color;\n"), i);
                        }
                        else if(combineOperand1 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg1.Printf(wxT("   vec4 texUnit%iArg1 = vec4(1.0) - color;\n"), i);
                        }
                    }
                    if(combineSrc2 == GL_TEXTURE)
                    {
                        if(combineOperand2 == GL_SRC_COLOR)
                        {
                            Arg2.Printf(wxT("    vec4 texture%iArg2 = texture2D(texUnit%i,gl_TexCoord[%i].xy);\n"
                                "    vec4 texUnit%iArg2 = texture%iArg2\n;"), i, i, i, i, i);
                        }
                        else if(combineOperand2 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg2.Printf(wxT("    vec4 texture%iArg2 = texture2D(texUnit%i,gl_TexCoord[%i].xy);\n"
                                "    vec4 texUnit%iArg2 = vec4(1.0) - texture%iArg2\n;"), i, i, i, i, i);
                        }
                    }
                    else if(combineSrc2 == GL_CONSTANT)
                    {
                        if(combineOperand2 == GL_SRC_COLOR)
                        {
                            Arg2.Printf(wxT("    vec4 texUnit%iArg2 = gl_TextureEnvColor[%i];\n"), i, i);
                        }
                        else if(combineOperand2 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg2.Printf(wxT("    vec4 texUnit%iArg2 = vec4(1.0) - gl_TextureEnvColor[%i];\n"), i, i);
                        }
                    }
                    else if(combineSrc2 == GL_PRIMARY_COLOR)
                    {
                        if(combineOperand2 == GL_SRC_COLOR)
                        {
                            Arg2.Printf(wxT("    vec4 texUnit%iArg2 = gl_Color;\n"), i);
                        }
                        else if(combineOperand2 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg2.Printf(wxT("    vec4 texUnit%iArg2 = vec4(1.0) - gl_Color;\n"), i);
                        }
                    }
                    else if(combineSrc2 == GL_PREVIOUS)
                    {
                        if(combineOperand2 == GL_SRC_COLOR)
                        {
                            Arg2.Printf(wxT("    vec4 texUnit%iArg2 = color;\n"), i);
                        }
                        else if(combineOperand2 == GL_ONE_MINUS_SRC_COLOR)
                        {
                            Arg2.Printf(wxT("    vec4 texUnit%iArg2 = vec4(1.0) - color;\n"), i);
                        }
                    }
                    switch(combineMode){
                        case GL_REPLACE:
                            if(combineRGBScale > 1)
                            {
                                fragmentTextureString.Printf(Arg0 + wxT("    color = clamp(%.1f * texUnit%iArg0, 0.0, 1.0);\n"), (float)combineRGBScale, i);
                            }
                            else
                            {
                                fragmentTextureString.Printf(Arg0 + wxT("    color = clamp(texUnit%iArg0, 0.0, 1.0);\n"), i);
                            }
                            break;
                        case GL_MODULATE:
                            if(combineRGBScale > 1)
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = clamp(%.1f * texUnit%iArg0 * texUnit%iArg1, 0.0, 1.0);\n"), (float)combineRGBScale, i, i);
                            }
                            else
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = clamp(texUnit%iArg0 * texUnit%iArg1, 0.0, 1.0);\n"), i, i);
                            }
                            break;
                        case GL_ADD:
                            if(combineRGBScale > 1)
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = clamp(%.1f * vec4(texUnit%iArg0 + texUnit%iArg1), 0.0, 1.0);\n"), (float)combineRGBScale, i, i);
                            }
                            else
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = clamp(texUnit%iArg0 + texUnit%iArg1, 0.0, 1.0);\n"), i, i);
                            }
                            break;    
                        case GL_ADD_SIGNED:
                            if(combineRGBScale > 1)
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = clamp(%.1f * vec4(texUnit%iArg0 + texUnit%iArg1 - vec4(0.5)), 0.0, 1.0);\n"), (float)combineRGBScale, i, i);
                            }
                            else
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = clamp(texUnit%iArg0 + texUnit%iArg1 - vec4(0.5), 0.0, 1.0);\n"), i, i);
                            }
                            break;
                        case GL_INTERPOLATE:
                            if(combineRGBScale > 1)
                            {
                               fragmentTextureString.Printf(Arg0 + Arg1 + Arg2 + wxT("    color = clamp(%.1f * (texUnit%iArg0 * texUnit%iArg2"
                               " + texUnit%iArg1 * (vec4(1.0) - texUnit%iArg2)), 0.0, 1.0);\n"), (float)combineRGBScale, i, i, i, i);
                            }
                            else
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + Arg2 + "    color = clamp(texUnit%iArg0 * texUnit%iArg2"
                                " + texUnit%iArg1 * (vec4(1.0) - texUnit%iArg2), 0.0, 1.0);\n", i, i, i, i);
                            }
                            break;
                        case GL_SUBTRACT:
                            if(combineRGBScale > 1)
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = clamp(%.1f * (texUnit%iArg0 - texUnit%iArg1), 0.0, 1.0);\n"), (float)combineRGBScale, i, i);
                            }
                            else
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = clamp(texUnit%iArg0 - texUnit%iArg1, 0.0, 1.0);\n"), i, i);
                            }
                            break;
                        case GL_DOT3_RGB:
                            if(combineRGBScale > 1)
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = vec4(clamp(%.1f * 4.0 * (( texUnit%iArg0.r - 0.5) * (texUnit%iArg1.r- 0.5)"
                                " + (texUnit%iArg0.g - 0.5) * (texUnit%iArg1.g - 0.5) + (texUnit%iArg0.b - 0.5) * "
                                "(texUnit%iArg1.b - 0.5), 0.0, 1.0));\n"), (float)combineRGBScale, i, i, i, i, i, i);
                            }
                            else
                            {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = vec4(clamp(4.0 * (( texUnit%iArg0.r - 0.5) * (texUnit%iArg1.r- 0.5)"
                                " + (texUnit%iArg0.g - 0.5) * (texUnit%iArg1.g - 0.5) + (texUnit%iArg0.b - 0.5) * "
                                "(texUnit%iArg1.b - 0.5)), 0.0, 1.0));\n"), i, i, i, i, i, i);
                            }
                            break;
                     case GL_DOT3_RGBA:
                          //Not implemented in ShaderGen GUI.
                          if(combineRGBScale > 1)
                          {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = vec4(clamp(%.1f * 4.0 * (( texUnit%iArg0.r - 0.5) * (texUnit%iArg1.r- 0.5)"
                                " + (texUnit%iArg0.g - 0.5) * (texUnit%iArg1.g - 0.5) + (texUnit%iArg0.b - 0.5) * "
                                "(texUnit%iArg1.b - 0.5)), 0.0, 1.0));\n"), (float)combineRGBScale, i, i, i, i, i, i);
                          }
                          else
                          {
                                fragmentTextureString.Printf(Arg0 + Arg1 + wxT("    color = vec4(clamp(4.0 * (( texUnit%iArg0.r - 0.5) * (texUnit%iArg1.r- 0.5)"
                                " + (texUnit%iArg0.g - 0.5) * (texUnit%iArg1.g - 0.5) + (texUnit%iArg0.b - 0.5) * "
                                "(texUnit%iArg1.b - 0.5)), 0.0, 1.0));\n"), i, i, i, i, i, i);
                          }
                          break;
                    }
                    break;
            }
            str += fragmentTextureString;
        }
    }
}

/*******************************************
* VERTEX SHADER
********************************************/

const char* SGShaderGenerator::BuildVertexShader()
{
    wxString topVertShader, bottomVertShader;
    InitTextures();

    topVertShader = wxT(""
                    "/*******************************************************\n"
                    "*  Fixed.vert Fixed Function Equivalent Vertex Shader  *\n"
                    "*   Automatically Generated by 3Dlabs GLSL ShaderGen   *\n"
                    "*             http://developer.3dlabs.com              *\n"
                    "*******************************************************/\n");    

    topVertShader += wxT("vec4 Ambient;\nvec4 Diffuse;\nvec4 Specular;\n\n");

    bottomVertShader = wxT("");

    fLightPoint = fLightSpot = fLightDir = fMapSphere = fMapReflection = fLightDirSpot = false;

    BuildFuncFnormal(bottomVertShader);

    //The for loop counts DOWN to find last texture unit in use.
    //This is necessary for the BuildTexCoord function to handle
    //  multiple texture coordinates correctly.
    for(int i=NUM_TEXTURES-1; i>=0; i--)
    {
        if(currentTexture[i].textureEnabled)
        {
           BuildTexCoord(bottomVertShader);
           texturesEnabled = true;
           break;
        }
    }
    if( glIsEnabled(GL_LIGHTING) )
        BuildLightCode( bottomVertShader );

    BuildVertMain( bottomVertShader );

    if(fLightPoint)
    {
        AddFuncLightPoint(topVertShader);
    }
    if(fLightSpot)
    {
        AddFuncLightSpot(topVertShader);
    }
    if(fLightDir)
    {
        AddFuncLightDirectional(topVertShader);
    }
    if(fLightDirSpot)
    {
        AddFuncLightSpotDirection(topVertShader);
    }
    if(fMapSphere)
    {
        AddFuncSphereMap(topVertShader);
    }
    if( fMapReflection)
    {
        AddFuncReflectionMap(topVertShader);
    }
    if(glIsEnabled(GL_FOG))
    {
        BuildFuncFog(topVertShader);
    }
    vertShader =  topVertShader  +  bottomVertShader;
    return vertShader.c_str();
}

void SGShaderGenerator::BuildLightCode(wxString &str)
{
    int separateSpecInt, lightModelTwoSidedInt, localViewInt;

    glGetIntegerv(GL_LIGHT_MODEL_COLOR_CONTROL, &separateSpecInt);
    glGetIntegerv(GL_LIGHT_MODEL_TWO_SIDE, &lightModelTwoSidedInt);
    glGetIntegerv(GL_LIGHT_MODEL_LOCAL_VIEWER, &localViewInt);

    if(glIsEnabled(GL_LIGHTING)){

        str +=  wxT("\nvoid flight(in vec3 normal, in vec4 ecPosition, float alphaFade)\n"
                "{\n"
                "    vec4 color;\n"
                "    vec3 ecPosition3;\n"
                "    vec3 eye;\n\n"
                "    ecPosition3 = (vec3 (ecPosition)) / ecPosition.w;\n");

        if(localViewInt)
        {
            str += wxT("    eye = -normalize(ecPosition3);\n\n");
        }
        else
        {
            str += wxT("    eye = vec3 (0.0, 0.0, 1.0);\n\n");
        }

        str +=  wxT("    // Clear the light intensity accumulators\n"
                "    Ambient  = vec4 (0.0);\n"
                "    Diffuse  = vec4 (0.0);\n"
                "    Specular = vec4 (0.0);\n\n");

        GLfloat v[4];
        GLfloat spotCut;

        for (int i = 0; i < NUM_LIGHTS; i++)
        {
            if(glIsEnabled(GL_LIGHT0 + i))
            {
                glGetLightfv( GL_LIGHT0 + i, GL_POSITION, v);
                glGetLightfv( GL_LIGHT0 + i, GL_SPOT_CUTOFF, &spotCut);
                if(v[3] == 0.0)
                {
                    if( spotCut != DEFAULT_SPOT_CUT)
                    {
                        str += wxString::Format(wxT("    infiniteSpotLight(%d, normal);\n\n"), i);
                        fLightDirSpot = true;
                    }
                    else
                    {
                        str += wxString::Format(wxT("    directionalLight(%d, normal);\n\n"), i);
                        fLightDir = true;
                    }
                }
                else
                {
                    if( spotCut == DEFAULT_SPOT_CUT)
                    {
                        str += wxString::Format(wxT("    pointLight(%d, normal, eye, ecPosition3);\n\n"), i);
                        fLightPoint = true;
                    }
                    else
                    {
                        str += wxString::Format(wxT("    spotLight(%d, normal, eye, ecPosition3);\n\n"), i);
                        fLightSpot = true;
                    }
                }
            }
        }

        str += wxT("    color = gl_FrontLightModelProduct.sceneColor +\n"
                   "      Ambient  * gl_FrontMaterial.ambient +\n"
                   "      Diffuse  * gl_FrontMaterial.diffuse;\n");

        if(separateSpecInt == GL_SEPARATE_SPECULAR_COLOR)
        {        
            str += wxT("    gl_FrontSecondaryColor = Specular * gl_FrontMaterial.specular;\n");
        }
        else
        {
            str += wxT("    color += Specular * gl_FrontMaterial.specular;\n");
        }

        str += wxT("    color = clamp( color, 0.0, 1.0 );\n");

        str += wxT("    gl_FrontColor = color;\n");

        if(lightModelTwoSidedInt)
        {
            str +=  wxT("    // Invert the normal for these lighting calculations\n"
                    "    normal = -normal;\n"
                    "    //Clear the light intensity accumulators\n"
                    "    Ambient  = vec4 (0.0);\n"
                    "    Diffuse  = vec4 (0.0);\n"
                    "    Specular = vec4 (0.0);\n\n");

            for (int i = 0; i < 8; i++)
            {
                if(glIsEnabled(GL_LIGHT0 + i))
                {
                    glGetLightfv( GL_LIGHT0 + i, GL_POSITION, v);

                    if(v[3] == 0.0)
                    {
                        str += wxString::Format(wxT("    directionalLight(%d, normal);\n\n"), i);
                        fLightDir = true;
                    }
                    else
                    {
                        glGetLightfv( GL_LIGHT0 + i, GL_SPOT_CUTOFF, &spotCut);
                        if( spotCut == 180.0)
                        {
                            str += wxString::Format(wxT("    pointLight(%d, normal, eye, ecPosition3);\n\n"), i);
                            fLightPoint = true;
                        }
                        else
                        {
                            str += wxString::Format(wxT("    spotLight(%d, normal, eye, ecPosition3);\n\n"), i);
                            fLightSpot = true;
                        }
                    }
                }
            }


            str +=  "\n"
                "    color = gl_BackLightModelProduct.sceneColor +\n"
                "        Ambient * gl_BackMaterial.ambient +\n"
                "        Diffuse * gl_BackMaterial.diffuse;\n";

            if(separateSpecInt == GL_SEPARATE_SPECULAR_COLOR)
                str += "    gl_BackSecondaryColor = Specular * gl_BackMaterial.specular;\n";
            else
                str += "    color += Specular * gl_BackMaterial.specular;\n";

            str += "    gl_BackColor = color;\n";

        }

        str += "\n"
                "    gl_FrontColor.a *= alphaFade;\n";

        if(lightModelTwoSidedInt)
        {     
            str += "    gl_FrontSecondaryColor.a *= alphaFade;\n"
                "    gl_BackColor.a *= alphaFade;\n"
                "    gl_BackSecondaryColor.a *= alphaFade;\n";
        }
        str += "}\n";
    }
}

void SGShaderGenerator::BuildFuncFnormal(wxString &str)
{
    if(glIsEnabled(GL_LIGHTING) || texturesEnabled)
    {
    str += wxT("\n"
        "vec3 fnormal(void)\n"
        "{\n"
        "    //Compute the normal \n"
        "    vec3 normal = gl_NormalMatrix * gl_Normal;\n");

    if(glIsEnabled(GL_NORMALIZE))
    {
        str +=wxT("    normal = normalize(normal);\n");
    }
    if(glIsEnabled(GL_RESCALE_NORMAL))
    {
        str += wxT("    normal = normal * gl_NormalScale;\n");
        str += wxT("    return normal;\n"
            "}\n");
    }
    else
    {
        str += wxT("    return normal;\n"
            "}\n");
    }
    }
}

void SGShaderGenerator::BuildFuncFog(wxString &str)
{
    int fogCoordSource;

    glGetIntegerv(GL_FOG_COORDINATE_SOURCE, &fogCoordSource);

    str +=  wxT("\n"
            "float ffog(in float ecDistance)\n"
            "{\n");

    if(fogCoordSource == GL_FOG_COORDINATE)
    {
        str += wxT("    return gl_FogCoord;\n");
    }
    else
    {
        str += wxT("    return(abs(ecDistance));\n");
    }
    str += wxT("}\n");
}

void SGShaderGenerator::BuildTexCoord(wxString &str)
{
    str += wxT("\nvoid ftexgen(in vec3 normal, in vec4 ecPosition)\n"
           "{\n");

    wxString texCoordString, tempStringA, tempStringB;

    for(int i=0;i<NUM_TEXTURES;i++)
    {
        if(currentTexture[i].textureEnabled == GL_TRUE)
        {
            if(currentTexture[i].textureCoordinateGenerationReflection)
            {
                if(!fMapReflection && !fMapSphere)
                {
                    tempStringA = wxT("    vec3 ecPosition3;"
                                  "    ecPosition3 = (vec3(ecPosition))/ecPosition.w;\n");
                }
                if(!fMapReflection)
                {
                    tempStringB = wxT("    vec3 reflection = reflectionMap( normal, ecPosition3 );\n");
                    if(!fMapSphere)
                    {
                        texCoordString.Printf(tempStringA + tempStringB + wxT("    gl_TexCoord[%i] = vec4( reflection, 1.0); \n"), i);
                    }
                    else
                    {
                        texCoordString.Printf(tempStringB + wxT("    gl_TexCoord[%i] = vec4( reflection, 1.0); \n"), i);
                    }
                    fMapReflection = true;
                }
                else
                {
                    texCoordString.Printf(wxT("    gl_TexCoord[%i] = vec4( reflection, 1.0); \n"), i);
                }
            }
            else if(currentTexture[i].textureCoordinateGenerationSphereMap)
            {
                if(!fMapSphere && !fMapReflection)
                {
                    tempStringA = wxT("    vec3 ecPosition3;\n"
                                  "    ecPosition3 = (vec3(ecPosition))/ecPosition.w;\n");
                }
                if(!fMapSphere)
                {
                    tempStringB = wxT("    vec2 sMap = sphereMap( normal, ecPosition3 );\n");
                 
                    if(!fMapReflection)
                    {
                        texCoordString.Printf(tempStringA + tempStringB + wxT("    gl_TexCoord[%i] = vec4(sMap, 0.0, 1.0 );\n"), i);
                    }
                    else
                    {
                        texCoordString.Printf(tempStringB + wxT("    gl_TexCoord[%i] = vec4(sMap, 0.0, 1.0 );\n"), i);
                    }
                    fMapSphere = true;
                }
                else
                {
                    texCoordString.Printf("    gl_TexCoord[%i] = vec4( sMap, 0.0, 1.0 );\n", i);
                }
            }
            else if(currentTexture[i].textureCoordinateGenerationEye)
            {
                texCoordString.Printf("\n    gl_TexCoord[%i].s = dot( ecPosition, gl_EyePlaneS[%i] );\n"
                    "    gl_TexCoord[%i].t = dot( ecPosition, gl_EyePlaneT[%i] );\n"
                    "    gl_TexCoord[%i].p = dot( ecPosition, gl_EyePlaneR[%i] );\n"
                    "    gl_TexCoord[%i].q = dot( ecPosition, gl_EyePlaneQ[%i] );\n", i,i,i,i,i,i,i,i);
            }
            else if(currentTexture[i].textureCoordinateGenerationObject)
            {
                texCoordString.Printf("\n    gl_TexCoord[%i].s = dot( gl_Vertex, gl_ObjectPlaneS[%i] );\n"
                                      "    gl_TexCoord[%i].t = dot( gl_Vertex, gl_ObjectPlaneT[%i] );\n"
                                      "    gl_TexCoord[%i].p = dot( gl_Vertex, gl_ObjectPlaneR[%i] );\n"
                                      "    gl_TexCoord[%i].q = dot( gl_Vertex, gl_ObjectPlaneQ[%i] );\n", i,i,i,i,i,i,i,i);
            }
            else if(currentTexture[i].textureCoordinateGenerationNormal)
            {
                texCoordString.Printf(wxT("\n    gl_TexCoord[%i] = vec4( normal, 1.0 );\n"), i);

            }
            else
            {
                texCoordString.Printf(wxT("\n"
                                      "    gl_TexCoord[%i] = gl_MultiTexCoord%i;\n"), i,i);
            }

            str += texCoordString;
        }
    }

    str += wxT("}\n");

}

//This functionality is not yet implemented in the GLSL ShaderGen GUI, it may
//  be added at a later date.  There has been no testing done with regards to
//  all "point" functions.
void SGShaderGenerator::BuildFuncPoint(wxString &str)
{
    str = wxT("    float fpoint(in float ecDistance, out float fadeFactor)\n"
          "    {\n"
          "        float derivedSize;\n"
          "        derivedSize = gl_Point.distanceConstantAttenuation +\n"
          "            (gl_Point.distanceLinearAttenuation * ecDistance) +\n"
          "            (gl_Point.distanceQuadraticAttenuation * ecDistance * ecDistance);\n"
          "        derivedSize = gl_Point.size * inversesqrt(derivedSize);\n"
          "        derivedSize = clamp(derivedSize, gl_Point.sizeMin, gl_Point.sizeMax);\n");

    if(glIsEnabled(GL_MULTISAMPLE))
    {
        str += wxT("        {\n"
               "            derivedSize = max(derivedSize, gl_Point.fadeThresholdSize);\n"
               "            if (derivedSize >= gl_Point.fadeThresholdSize)\n"
               "                fadeFactor = 1.0;\n"
               "            else\n"
               "            {\n"
               "                float ratio = derivedSize/gl_Point.fadeThresholdSize;\n"
               "                fadeFactor  = ratio * ratio;\n"
               "            }\n"
               "        }\n");
    }

    str += wxT("    return derivedSize;\n    }\n");
}

void SGShaderGenerator::BuildVertMain( wxString &str)
{    
    GLboolean lighting = glIsEnabled(GL_LIGHTING);

    str +=  wxT("\n\n"
            "void main (void)\n"
            "{\n");
    
    for(int i=0; i<NUM_TEXTURES; i++)
    {
        if(currentTexture[i].textureEnabled || lighting)
        {
            str+= wxT("    vec3  transformedNormal;\n"
                  "    float alphaFade = 1.0;\n\n");
            break;
        }
    }
    str +=  wxT("    // Eye-coordinate position of vertex, needed in various calculations\n"
            "    vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;\n\n"
            "    // Do fixed functionality vertex transform\n"
            "    gl_Position = ftransform();\n");

    for(int i=0; i<5; i++)
    {
        if(currentTexture[i].textureEnabled || lighting)
        {
            str += wxT("    transformedNormal = fnormal();\n");
            break;
        }
    }
    
    //
    //Query and support point parameters here.
    //
    
    if( glIsEnabled(GL_LIGHTING) )
    {
        str += wxT("    flight(transformedNormal, ecPosition, alphaFade);\n");
    }
    /*else 
    {
        //str += wxT("    gl_FrontColor = gl_Color;\n");
    }*/

    if(glIsEnabled(GL_FOG))
    {
        str += wxT("    gl_FogFragCoord = ffog(ecPosition.z);\n");
    }

    if(vTexGenEnable)            
    {
        str += wxT("    ftexgen(transformedNormal, ecPosition);\n");
    }

    if(glIsEnabled(GL_CLIP_PLANE0) || glIsEnabled(GL_CLIP_PLANE1) || glIsEnabled(GL_CLIP_PLANE2)
        || glIsEnabled(GL_CLIP_PLANE3) || glIsEnabled(GL_CLIP_PLANE4) || glIsEnabled(GL_CLIP_PLANE5))
    {
        str += wxT("    gl_ClipVertex = ecPosition;\n");
    }

    str+= wxT("}");
}


/*****************************************************************
* STATIC METHODS TO ADD PREDEFINED FUNCTIONS
*****************************************************************/

void SGShaderGenerator::AddFuncLightDirectional(wxString &str) const
{
    str +=  wxT("\nvoid directionalLight(in int i, in vec3 normal)\n"
            "{\n"
            "   float nDotVP;         // normal . light direction\n"
            "   float nDotHV;         // normal . light half vector\n"
            "   float pf;             // power factor\n\n"
            "   nDotVP = max(0.0, dot(normal, normalize(vec3 (gl_LightSource[i].position))));\n"
            "   nDotHV = max(0.0, dot(normal, vec3 (gl_LightSource[i].halfVector)));\n\n"
            "   if (nDotVP == 0.0)\n"
            "   {\n"
            "       pf = 0.0;\n"
            "   }\n"
            "   else\n"
            "   {\n"
            "       pf = pow(nDotHV, gl_FrontMaterial.shininess);\n\n"
            "   }\n"
            "   Ambient  += gl_LightSource[i].ambient;\n"
            "   Diffuse  += gl_LightSource[i].diffuse * nDotVP;\n"
            "   Specular += gl_LightSource[i].specular * pf;\n"
            "}\n");
}

void SGShaderGenerator::AddFuncLightPoint(wxString &str) const
{
    str +=  wxT("\nvoid pointLight(in int i, in vec3 normal, in vec3 eye, in vec3 ecPosition3)\n"
            "{\n"
            "   float nDotVP;       // normal . light direction\n"
            "   float nDotHV;       // normal . light half vector\n"
            "   float pf;           // power factor\n"
            "   float attenuation;  // computed attenuation factor\n"
            "   float d;            // distance from surface to light source\n"
            "   vec3  VP;           // direction from surface to light position\n"
            "   vec3  halfVector;   // direction of maximum highlights\n\n"
            "   // Compute vector from surface to light position\n"
            "   VP = vec3 (gl_LightSource[i].position) - ecPosition3;\n\n"
            "   // Compute distance between surface and light position\n"
            "   d = length(VP);\n\n"
            "   // Normalize the vector from surface to light position\n"
            "   VP = normalize(VP);\n\n"
            "   // Compute attenuation\n"
            "   attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +\n"
            "       gl_LightSource[i].linearAttenuation * d +\n"
            "       gl_LightSource[i].quadraticAttenuation * d * d);\n\n"
            "   halfVector = normalize(VP + eye);\n\n"
            "   nDotVP = max(0.0, dot(normal, VP));\n"
            "   nDotHV = max(0.0, dot(normal, halfVector));\n\n"
            "   if (nDotVP == 0.0)\n"
            "   {\n"
            "       pf = 0.0;\n"
            "   }\n"
            "   else\n"
            "   {\n"
            "       pf = pow(nDotHV, gl_FrontMaterial.shininess);\n\n"
            "   }\n"
            "   Ambient  += gl_LightSource[i].ambient * attenuation;\n"
            "   Diffuse  += gl_LightSource[i].diffuse * nDotVP * attenuation;\n"
            "   Specular += gl_LightSource[i].specular * pf * attenuation;\n"
            "}\n");
}

void SGShaderGenerator::AddFuncLightSpot(wxString &str) const
{
    str+=   wxT("\nvoid spotLight(in int i, in vec3 normal, in vec3 eye, in vec3 ecPosition3)\n"
            "{\n"
            "   float nDotVP;            // normal . light direction\n"
            "   float nDotHV;            // normal . light half vector\n"
            "   float pf;                // power factor\n"
            "   float spotDot;           // cosine of angle between spotlight\n"
            "   float spotAttenuation;   // spotlight attenuation factor\n"
            "   float attenuation;       // computed attenuation factor\n"
            "   float d;                 // distance from surface to light source\n"
            "   vec3  VP;                // direction from surface to light position\n"
            "   vec3  halfVector;        // direction of maximum highlights\n\n"
            "   // Compute vector from surface to light position\n"
            "   VP = vec3 (gl_LightSource[i].position) - ecPosition3;\n\n"
            "   // Compute distance between surface and light position\n"
            "   d = length(VP);\n\n"
            "   // Normalize the vector from surface to light position\n"
            "   VP = normalize(VP);\n\n"
            "   // Compute attenuation\n"
            "   attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +\n"
            "       gl_LightSource[i].linearAttenuation * d +\n"
            "       gl_LightSource[i].quadraticAttenuation * d * d);\n\n"
            "   // See if point on surface is inside cone of illumination\n"
            "   spotDot = dot(-VP, normalize(gl_LightSource[i].spotDirection));\n\n"
            "   if (spotDot < gl_LightSource[i].spotCosCutoff)\n"
            "   {\n"
            "       spotAttenuation = 0.0; // light adds no contribution\n"
            "   }\n"
            "   else\n"
            "   {\n"
            "       spotAttenuation = pow(spotDot, gl_LightSource[i].spotExponent);\n\n"
            "   }\n"
            "   // Combine the spotlight and distance attenuation.\n"
            "   attenuation *= spotAttenuation;\n\n"
            "   halfVector = normalize(VP + eye);\n\n"
            "   nDotVP = max(0.0, dot(normal, VP));\n"
            "   nDotHV = max(0.0, dot(normal, halfVector));\n\n"
            "   if (nDotVP == 0.0)\n"
            "   {\n"
            "       pf = 0.0;\n"
            "   }\n"
            "   else\n"
            "   {\n"
            "       pf = pow(nDotHV, gl_FrontMaterial.shininess);\n\n"
            "   }\n"
            "   Ambient  += gl_LightSource[i].ambient * attenuation;\n"
            "   Diffuse  += gl_LightSource[i].diffuse * nDotVP * attenuation;\n"
            "   Specular += gl_LightSource[i].specular * pf * attenuation;\n\n"
            "}\n");
}

void SGShaderGenerator::AddFuncLightSpotDirection(wxString &str) const
{
    str +=  wxT("\nvoid infiniteSpotLight(in int i, in vec3 normal)\n"
            "{\n"
            "   float nDotVP;         // normal . light direction\n"
            "   float nDotHV;         // normal . light half vector\n"
            "   float pf;             // power factor\n"
            "   float spotAttenuation;\n"
            "   vec3  Ppli;\n"
            "   vec3  Sdli;\n\n"
            "   nDotVP = max(0.0, dot(normal, normalize(vec3 (gl_LightSource[i].position))));\n"
            "   nDotHV = max(0.0, dot(normal, vec3 (gl_LightSource[i].halfVector)));\n\n"
            "   Ppli = -normalize(vec3(gl_LightSource[i].position));\n"
            "   Sdli = normalize(vec3(gl_LightSource[i].spotDirection));\n\n"
            "   spotAttenuation = pow(dot(Ppli, Sdli), gl_LightSource[i].spotExponent);\n"
            "   if (nDotVP == 0.0)\n"
            "   {\n"
            "       pf = 0.0;\n"
            "   }\n"
            "   else\n"
            "   {\n"
            "       pf = pow(nDotHV, gl_FrontMaterial.shininess);\n\n"
            "   }\n"
            "   Ambient  += gl_LightSource[i].ambient * spotAttenuation;\n"
            "   Diffuse  += gl_LightSource[i].diffuse * nDotVP * spotAttenuation;\n"
            "   Specular += gl_LightSource[i].specular * pf * spotAttenuation;\n"
            "}\n");
}

void SGShaderGenerator::AddFuncSphereMap(wxString &str) const
{
    str +=  wxT("\nvec2 sphereMap(in vec3 normal, in vec3 ecPosition3)\n"
            "{\n"
            "   float m;\n"
            "   vec3 r, u;\n"
            "   u = normalize(ecPosition3);\n"
            "   r = reflect(u, normal);\n"
            "   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0));\n"
            "   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
            "}\n");
}

void SGShaderGenerator::AddFuncReflectionMap(wxString &str) const
{
    str +=  wxT("\nvec3 reflectionMap(in vec3 normal, in vec3 ecPosition3)\n"
            "{\n"
            "   float NdotU, m;\n"
            "   vec3 u;\n"
            "   u = normalize(ecPosition3);\n"
            "   return (reflect(u, normal));\n"
            "}\n");
}