/************************************************************************
* Author(s):    Inderaj Bains and Joshua Doss                           *
* Last Modified: 11/04/2005                                             *
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

#include "App.h"
#include "SGFrame.h"
#include <wx/statline.h>
#include "SGShaderTextWindow.h"
#include <wx/image.h>
#include <wx/stattext.h>
#include "AboutDialog.h"

BEGIN_EVENT_TABLE(SGFrame, wxFrame) 
    EVT_MENU(Id::About, SGFrame::AboutCreate)
    EVT_MENU(Id::Help, SGFrame::OnHelp)
    EVT_MENU(Id::ViewPerspective, SGFrame::UpdateCanvas)
    EVT_MENU(Id::ModelTorus, SGFrame::OnModel)
    EVT_MENU(Id::ModelSphere, SGFrame::OnModel)
    EVT_MENU(Id::ModelKlein, SGFrame::OnModel)
    EVT_MENU(Id::ModelTrefoil, SGFrame::OnModel)
    EVT_MENU(Id::ModelConic, SGFrame::OnModel)
    EVT_MENU(Id::ModelPlane, SGFrame::OnModel)
    EVT_MENU(Id::FileExit, SGFrame::OnFileExit)
    EVT_CLOSE(SGFrame::OnClose)
END_EVENT_TABLE()

SGFrame::SGFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(0, wxID_ANY, title, pos, size, wxDEFAULT_FRAME_STYLE|wxSUNKEN_BORDER)
{
    models = new SGModels();
    shaderGen = new SGShaderGenerator();

    CreateStatusBar(1);

    wxMenu* menu;
    wxMenuBar* menuBar = new wxMenuBar;
    menu = new wxMenu;
    menu->Append(Id::FileExit, wxT("Exit"));
    menuBar->Append(menu, wxT("File"));

    menu = new wxMenu;
    menu->AppendCheckItem(Id::ViewPerspective, wxT("Perspective"));
    menuBar->Append(menu, wxT("View"));

    menu = new wxMenu;
    menu->AppendRadioItem(Id::ModelTorus, wxT("Torus"));
    menu->AppendRadioItem(Id::ModelSphere, wxT("Sphere"));
    menu->AppendRadioItem(Id::ModelTrefoil, wxT("Trefoil"));
    menu->AppendRadioItem(Id::ModelKlein, wxT("Klein"));
    menu->AppendRadioItem(Id::ModelConic, wxT("Conic"));
    menu->AppendRadioItem(Id::ModelPlane, wxT("Plane"));
    menuBar->Append(menu, wxT("Model"));

    menu = new wxMenu;
    menu->Append(Id::Help, wxT("Help"));
    menu->Append(Id::About, wxT("About"));
    menuBar->Append(menu, wxT("Help"));

    SetMenuBar(menuBar);

    Check(Id::ViewPerspective);
    Check(Id::ModelTorus);

    oglNotebook = new SGOglNotebook(this, wxID_ANY);
    canvas      = new SGCanvasWrapper(this, wxID_ANY, wxDefaultPosition, wxSize(400, 350) );
    shaderText  = new SGShaderTextWindow(this);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *horizSizer = new wxBoxSizer( wxHORIZONTAL );

    horizSizer->Add(
        canvas,        
        1,             
        wxEXPAND |     
        wxALL | wxADJUST_MINSIZE,      
        2);          

    horizSizer->Add(
        shaderText,    
        1,       
        wxEXPAND |   
        wxALL | wxADJUST_MINSIZE,     
        2);           

    topSizer->Add(
        horizSizer,
        1,           
        wxEXPAND |
        wxALIGN_CENTER | wxADJUST_MINSIZE);

    topSizer->Add(
        oglNotebook,    
        0,              
        wxEXPAND |
        wxALL | wxADJUST_MINSIZE,        
        2);          

    SetIcon(wxIcon(wxT("IDI_ICON1"), wxBITMAP_TYPE_ICO_RESOURCE));
    SetStatusText(title);
    SetAutoLayout(TRUE);
    SetSizer( topSizer );            
    topSizer->SetSizeHints( this );  
    CenterOnScreen();
    aboutDlg.Create(this);
}

void SGFrame::Clean()
{
    if(models != NULL)
    {
        delete models;
        models = NULL;
    }
    if(shaderGen != NULL)
    {
        delete shaderGen;
        shaderGen = NULL;
    }
}

void SGFrame::OnClose(wxCloseEvent &event)
{
    Clean();
    event.Skip();
}  

void SGFrame::OnFileExit(wxCommandEvent& event)
{
      Close();
      event.Skip();
}

void SGFrame::Check(Id id)
{
    wxMenuItem* item = GetMenuBar()->FindItem(id);
    if (!item)
    {
        return;
    }
    item->Check(true);
}

void SGFrame::Uncheck(Id id)
{
    wxMenuItem* item = GetMenuBar()->FindItem(id);
    if (!item)
    {
        return;
    }
    item->Check(false);
}

bool SGFrame::IsChecked(Id id) const
{
    wxMenuItem* item = GetMenuBar()->FindItem(id);
    if (!item)
    {
        return false;
    }
    return item->IsChecked();
}

void SGFrame::SetCanvasMode(int a)
{
    GetCanvasWrapper()->SetMode(a);
    GetCanvas()->Update();
}

void SGFrame::OnHelp(wxCommandEvent& event)
{
    wxMessageBox(wxT("\n"
        "        GLSL ShaderGen\n\n"
        "        INTENDED PURPOSE:\n\n"
        "        The purpose of ShaderGen is to show the user how to emulate fixed functionality\n"
        "        by using the OpenGL Shading Language.\n\n"
        "        ShaderGen is intended to be educational, with the focus being on clarity of\n"
        "        generated code rather than efficiency.\n"
        "         \n"
        "         \n"
        "        USAGE:\n\n"
        "        The tool has three major parts:\n"
        "        * OpenGL Window : Displays rendered output.\n"
        "        * OpenGL State : Configure the OpenGL State using the tabs.\n"
        "        * Shader Text Windows : Use the tabs to look through the generated shaders\n\n"
        "        Operation:\n\n"
        "        1. Using the tabs, setup the OpenGL state.  You will see your results updated\n"
        "             in the OpenGL Window as you make state changes.  If you make a change requiring text\n"
        "             entry, be sure to press the enter key to update the current state.\n"
        "        2. Click the \"GENERATE SHADERS\" button on the Shader Windows panel to generate Vertex and Fragment Shaders\n"
        "        3. Click \"COMPILE\" to compile the shaders, then check the \"Infolog\" text window for compilation results\n"
        "        4. Click \"LINK\" to link the compilers, then see the \"Infolog\" window for results\n"
        "        5. To switch the OpenGL Window to display the shaded model, click the \"Shader\n"
        "             Equivalent\" mode radio button.\n"
        "        6. Click \"BUILD\" to do steps 2 through 5 together.\n\n\n"
        "        Note: The text boxes only accept values when enter key is pressed\n"
        "        Note: You can edit the shaders in the text windows, and then press \"COMPILE\" and then \"LINK\" to\n"
        "                  to see the results of your edited shaders.\n"
        "        Note: The eye coordinate distance between the model center and the viewpoint is 6.0.  The model is\n"
        "                  drawn with it's center at (0.0,0.0,6.0) in eye-coordinates and diagonals at (-1.0, -1.0, 5.0)\n"
        "                  and (1.0, 1.0, 7.0).\n"
        "                 "), wxT("Help - ShaderGen"), wxOK);
}

void SGFrame::OnModel(wxCommandEvent& event)
{
    GetCanvas()->SetModel(event.GetId());
    GetCanvas()->Update();
}
