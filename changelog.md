<pre>
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
</pre>


# 3Dlabs GLSL ShaderGen v3.1

## Changes from ShaderGen v3.1.1 (in reverse chronological order)

* Fix the mirrored textures
* Update to glew 1.12.0
* Fix a texture name that couldn't be loaded on case sentive OS

## Changes from ShaderGen v3.1.0 (in reverse chronological order)

* Ported from WxWidget to Qt

## Changes from ShaderGen v3.0 (in reverse chronological order)

* Added ability to clear the infolog text window.
* Name Changed from ShaderGen to 3Dlabs GLSL ShaderGen
* Added Header to generated shaders
* Minor tuning to generated shaders
* Default values changed to be more visually appealing
* Place Unicode wrapper on string literals
* Updated About box to include community contributions and an updated screenshot
* Added functionality for COMBINE_RGB_SCALE
* Added functionality for GL_TEX_ENV_COLOR
* Added warning if number of texture units supported is less than 5
* Added functionality for GL_COMBINE_RGB with combinations not containing alpha
* Added TEXTURE COORDINATE SET notebook page
* Added TEXTURE ENVIRONMENT SET notebook page
* Added support for 5 texture sets
* Added multiple texture coordinate generation for up to 5 sets
* Added support for user defined eye and object plane coefficients
* Added support for user assignment of texture object to unit
* Added drop down menu for texture object selection
* Increased number of textures
* Integrated community contributed GL_SEPERATE_SPECULAR_COLOR support
* Removed TEXTURE notebook page
* Changed infolog, vertex and fragment shader text to fixed width
* Changed infolog text window to read only
* Optimized fog equation by offloading a portion of the calculation to host
