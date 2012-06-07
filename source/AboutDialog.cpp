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


#include "AboutDialog.h"
#include "SGShaderTextWindow.h"
#include <wx/statline.h>
#include <wx/image.h>
#include <wx/stattext.h>

BEGIN_EVENT_TABLE(AboutDialog, wxDialog)
    EVT_CLOSE(AboutDialog::OnClose)
END_EVENT_TABLE()

void AboutDialog::Create( wxWindow *parent)
{
    wxDialog::Create( parent,  Id::ModalAbout, wxT("About"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxCAPTION );
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sideSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("")), wxHORIZONTAL);
    wxImage img("../textures/BrickWall.bmp");
    wxBitmap splash(img);
    wxStaticBitmap* bmp = new wxStaticBitmap(this ,wxID_ANY, splash);
    sideSizer->Add(bmp);
    wxStaticLine *line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
    sideSizer->Add(line, 0, wxEXPAND | wxLEFT | wxRIGHT | wxADJUST_MINSIZE, 10);
    wxSize size(splash.GetWidth() * 1.5, splash.GetHeight());

    wxString textStr =  wxT("\n"
    "GLSL ShaderGen v3.0\n"
    "Copyright © 2005 3Dlabs. All rights reserved\n\n"
    "http://www.3dlabs.com/contact \n\n"
    "Lead Developer:\n"
    "   Joshua Alan Doss\n\n"
    "Past Developer(s):\n"
    "   Inderaj S. Baines\n\n"
    "Contributors:\n"
    "   Dave Baldwin\n"
    "   Philip Rideout\n"
    "   Randi Rost\n"
    "   Matthew Williams\n\n"
    "Community Contributor(s):\n"
    "   Ajoy Das\n"
    "   Michael M. Morrison\n\n"
    "3Dlabs thanks the developers of wxWidgets, Glew and Inno Setup.\n");

    wxStaticText *textt = new wxStaticText( this, wxID_ANY, textStr, wxDefaultPosition);
    sideSizer->Add(textt);
    topSizer->Add(sideSizer);
    SetSizer(topSizer);
    topSizer->SetSizeHints(this);
    Centre(wxBOTH);    
}
