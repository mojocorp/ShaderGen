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

class SGOglNotebook;

class SGOglLightNBPage : public wxPanel
{
public:
    SGOglLightNBPage(SGOglNotebook*  parent, wxWindowID id);
    
    void OnButton(wxCommandEvent& event);
    void OnCheckbox(wxCommandEvent& event);
    void OnTextEnter(wxCommandEvent& event);
    void OnRadio(wxCommandEvent& event);

DECLARE_EVENT_TABLE()

private:
    //void IncorrectFormat(wxString str);
    SGOglNotebook *m_parent;

// The pattern in GUI is shown-----------

//.-----------------h1-------------------
//.-----------------h2-------------------
//.--h3v1--|--h3v2--|--h3v3--|--h3v4-----
//.--h3v1--|--h3v2--|--h3v3--|--h3v4-----
//.--h3v1--|--h3v2--|--h3v3--|--h3v4-----

//selection of lights----h2-------
wxRadioBox* lightSelectionBox;

//enabling lights--------h1-------
wxCheckBox *lcb0, *lcb1, *lcb2, *lcb3;
wxCheckBox *lcb4, *lcb5, *lcb6, *lcb7;
wxCheckBox *lightingCheckBox, *normalizeCheckBox, *separateSpecularColorCheckBox;

//-----------------------h3v1-----
wxTextCtrl* lightPosition;

//-spotlight controls----h3v3-----
wxTextCtrl* spotDirection;
wxTextCtrl* spotExponent;
wxTextCtrl* spotCutoff;  

//-attenuation controls--h3v4-----
wxTextCtrl* constantAttenuation;
wxTextCtrl* quadraticAttenuation;
wxTextCtrl* linearAttenuation;

//-color buttons---------h3v2-----
wxButton* ambientLight;
wxButton* specularLight;
wxButton* diffuseLight;
};
