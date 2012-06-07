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
#include "Vector.h"
#include "AboutDialog.h"
#include "SGOglNotebook.h"
#include "SGCanvas.h"
#include "SGModels.h"
#include "SGShaderGenerator.h"

class SGCanvasWrapper;
class SGOglNotebook;
class SGShaderTextWindow;
class SGModels;
class SGCanvas;
class SGFixedGLState;
class SGShaderGenerator;
class SGTextures;

class SGFrame : public wxFrame 
{
public:
    SGFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void Clean();

    SGOglNotebook* GetOglNotebook(){ return oglNotebook; }
    SGShaderTextWindow* GetShaderTextWindow(){ return shaderText; }
    SGCanvasWrapper* GetCanvasWrapper(){ return canvas; }
    SGCanvas* GetCanvas() { return canvas->GetCanvas(); }

    SGFixedGLState* GetGLState() { return oglNotebook->GetGLState(); }
    SGTextures* GetTextures() { return oglNotebook->GetTextures(); }
    void Check(Id id);
    void Uncheck(Id id);
    bool IsChecked(Id id) const;
    void OnFileExit(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void DrawModel(Id model) { models->DrawModel(model); }
    void OnModel(wxCommandEvent& event);
    const char* GetVertexShader() { return shaderGen->BuildVertexShader(); }
    const char* GetFragmentShader() { return shaderGen->BuildFragmentShader(); }
    void OnClose(wxCloseEvent &event);
    void AboutCreate(wxCommandEvent &evt) { aboutDlg.Show(true); }
    void SetCanvasMode(int a);
    void UpdateCanvas(wxCommandEvent& event) { GetCanvas()->Update(); }

private:
    SGOglNotebook *oglNotebook;
    SGCanvasWrapper *canvas;
    SGShaderTextWindow* shaderText;
    SGModels *models;
    SGShaderGenerator *shaderGen;
    AboutDialog aboutDlg;

DECLARE_EVENT_TABLE()
};
