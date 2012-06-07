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
#include "SGCanvasMouseHandler.h"
#include "Vector.h"
#include "SGCanvasWrapper.h"
#include <wx/glcanvas.h>

class SGFrame;

class SGCanvas : public wxGLCanvas
{
public:
    SGCanvas(SGFrame* frame, SGCanvasWrapper* parent, wxWindowID id= -1, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize);
    SGFixedGLState* GetGLState() { return m_parent->GetGLState(); }
    SGFrame* GetFrame(){ return m_frame;}

    bool LinkShaders(const GLchar *vertexShader, const GLchar *fragmentShader);
    bool CompileShaders(const GLchar *vertexShader, const GLchar *fragmentShader);

    int GetMode() { return m_parent->GetMode();}
    int SwitchToShaderMode();

    static const float CameraZ;
    float GetZoom() { return m_zoom; }

    vec3 GetWorldSpace(int x, int y);

    void OnPaint(wxPaintEvent& event);
    void SetZoom(float zoom) { m_zoom = zoom;}
    void GLSetup();
    void Update() { Refresh(FALSE); }
    void SetModel(Id id) { modelCurrent = id;}
    void OnKey(wxKeyEvent& event);
    void PrintInfoLog(GLuint obj);
    void OnEventEraseBackground(wxEraseEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMouse(wxMouseEvent &event);

    GLuint logo;
    
private:
    SGCanvasWrapper *m_parent;
    SGCanvasMouseHandler mouse;
    Id modelCurrent;
    SGFrame* m_frame;
    int m_width, m_height;
    float m_left, m_right, m_bottom, m_top, m_znear, m_zfar, m_zoom;

    GLint GetUniLoc(unsigned int program, const GLchar *name);

    bool glReady, glCompiled, glLinked, gInit;
    unsigned int vertS, fragS, prog;

    void SetupFromFixedState();
    void WriteMessage(const wxString str);
    void SGCanvas::DrawLogo() const;
    void UnsupportedOpenGLVersion(void);
    void GetGlVersion(int *major, int *minor);
    void CheckGlImplementation();
    void NotEnoughTextureUnits(const int numTextures);

    void OnMouseLeftDown(wxMouseEvent &event);
    void OnMouseLeftUp(wxMouseEvent &event);
    void OnMouseRightDown(wxMouseEvent &event);
    void OnMouseRightUp(wxMouseEvent &event);

DECLARE_EVENT_TABLE()
};
