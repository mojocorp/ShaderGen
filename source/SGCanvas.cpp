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
#include "SGCanvas.h"
#include "SGFixedGLState.h"
#include "SGCanvasMouseHandler.h"
#include "UtilityFunctions.h"
#include "SGFrame.h"
#include "SGShaderTextWindow.h"
#include "SGTextures.h"
#include <wx/textctrl.h>
#include <png/png.h>

BEGIN_EVENT_TABLE(SGCanvas, wxGLCanvas)
EVT_PAINT(SGCanvas::OnPaint)
EVT_MOUSE_EVENTS(SGCanvas::OnMouse)
EVT_KEY_DOWN(SGCanvas::OnKey)
EVT_SIZE(SGCanvas::OnSize)
EVT_ERASE_BACKGROUND(SGCanvas::OnEventEraseBackground)
END_EVENT_TABLE()

const float SGCanvas::CameraZ = -5;

SGCanvas::SGCanvas(SGFrame *frame, SGCanvasWrapper *parent, wxWindowID id,const wxPoint& pos, const wxSize& size)
:wxGLCanvas(parent, id, pos, size, wxCLIP_CHILDREN)
{
    m_parent = parent;
    this->m_frame = frame;
    SetBackgroundColour(*wxBLUE);
    m_zoom= 0.8f;
    mouse.SetCanvas(this);
    modelCurrent = Id::ModelTorus;
    glReady = glCompiled = glLinked = gInit = false;
    prog = vertS = fragS = 0;

    float versionString;
    versionString = atof((char*)glGetString(GL_VERSION));

    CheckGlImplementation();
}

void SGCanvas::OnEventEraseBackground(wxEraseEvent& event)
{
    //
    // As this is an OpenGL window,
    // Deliberately do nothing on an erase background event.
    //
}

void SGCanvas::OnSize(wxSizeEvent& event)
{
    Update();
}

void SGCanvas::DrawLogo() const
{
    const int logoWidth = 75;
    const int logoHeight = 75;
    const int xPadding = 5;
    const int yPadding = 5;
    glDisable(GL_LIGHTING);
    glColor4f(1.0f,1.0f,1.0f,0.5f);

    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(xPadding + logoWidth, m_height - yPadding - logoHeight);    
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(xPadding, m_height - yPadding - logoHeight);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(xPadding, m_height - yPadding);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(xPadding + logoWidth, m_height - yPadding);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}


void SGCanvas::OnPaint(wxPaintEvent& event)
{
    PrintOpenGLError();
    wxPaintDC dc(this);
    if (!GetContext())
    {
        return;
    }
    SetCurrent();
    PrintOpenGLError();

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    PrintOpenGLError();

    GLSetup();
    PrintOpenGLError();
    glPushMatrix();
    mouse.LoadMatrix();
    m_frame->DrawModel(modelCurrent);
    PrintOpenGLError();
    glPopMatrix();

    PrintOpenGLError();
    glUseProgram(0);
    PrintOpenGLError();
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_FOG_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    if(glIsEnabled(GL_FOG)) 
    {
        glDisable(GL_FOG);
    }

    PrintOpenGLError();
    SGFixedGLState *glState = GetGLState();
    glViewport(0, 0, m_width, m_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_width, m_height, 0, -2, 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    m_frame->GetTextures()->ActivateLogo();

    DrawLogo();

    glPopAttrib();

    glFinish();
    SwapBuffers();
    event.Skip();
    PrintOpenGLError();
}

void SGCanvas::GLSetup()
{
    GetClientSize(&m_width, &m_height);
    float aspect = (float) m_width / (float) m_height;
    float vp = 0.8f;
    m_left = -vp;
    m_right = vp;
    m_bottom = -vp / aspect;
    m_top = vp / aspect;
    m_znear = 2;
    m_zfar = 10;

    if(!gInit)
    {
        int initSuccess = glewInit();
     
        if (initSuccess != GLEW_OK)
        {
            wxGetApp().Errorf(wxT("Unable to initialize GLEW.\n %s"), glewGetErrorString(initSuccess));
        }
        else
        {
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            gInit = true;
        }
    }

    SGFixedGLState *glState = GetGLState();
    glViewport(0, 0, m_width, m_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    PrintOpenGLError();
    if (wxGetApp().GetFrame()->IsChecked(Id::ViewPerspective))
    {
        glFrustum(m_left * m_zoom, m_right * m_zoom, m_bottom * m_zoom, m_top * m_zoom, m_znear, m_zfar);
    }
    else
    {
        glOrtho(3 * m_left * m_zoom, 3 * m_right * m_zoom, 3 * m_bottom * m_zoom, 3 * m_top * m_zoom, m_znear, m_zfar);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for(GLint i=0;i<5;i++)
    {
        if(glState->GetTexture(i)->textureEnabled)
        {
            m_frame->GetTextures()->Activate(glState->GetTexture(i)->textureCurrentSelection, i);
        }
        else
        {
            m_frame->GetTextures()->Deactivate(i);
        }
    }

    PrintOpenGLError();

    if(m_parent->GetMode() == Id::GLModeChoiceFixed)
    {
        glUseProgram(0);
        SetupFromFixedState();
    }
    else
    {
        if(glLinked)
        {
            glUseProgram(prog);
        }
        else
        {
            glUseProgram(0);
        }
    }
    glReady = true;
}

void SGCanvas::SetupFromFixedState()
{
    PrintOpenGLError();
    SGFixedGLState *glState = GetGLState();
    Light *light;
    Material* material;
    glEnable(GL_AUTO_NORMAL);

    glState->SetLightChanged(false);

    if(glState->GetLightingEnable())
    {
        glEnable(GL_LIGHTING);

        for(int i = 0; i < 8; i++)
        {
            light = glState->GetLight(i);
            if(light->lightEnabled)
            {
                glLightfv(GL_LIGHT0+i, GL_POSITION,       light->lightPositionVector );
                glLightfv(GL_LIGHT0+i, GL_AMBIENT,        light->lightAmbientColorVector );
                glLightfv(GL_LIGHT0+i, GL_DIFFUSE,        light->lightDiffuseColorVector );
                glLightfv(GL_LIGHT0+i, GL_SPECULAR,       light->lightSpecularColorVector );
                glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, light->lightSpotDirectionVector );
                glLightf (GL_LIGHT0+i, GL_SPOT_EXPONENT,  light->lightSpotExponent );
                glLightf (GL_LIGHT0+i, GL_SPOT_CUTOFF,    light->lightSpotCutoff );
                glLightf (GL_LIGHT0+i, GL_CONSTANT_ATTENUATION,  light->lightConstantAttenuation );
                glLightf (GL_LIGHT0+i, GL_LINEAR_ATTENUATION,    light->lightLinearAttenuation );
                glLightf (GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, light->lightQuadraticAttenuation );
                glEnable (GL_LIGHT0+i );
            }
            else
            {
                glDisable(GL_LIGHT0+i );
            }
        }
    }
    else
    {
        glDisable(GL_LIGHTING );
    }
    if(glState->GetNormalizeEnable())
    {
        glEnable(GL_NORMALIZE);
    }
    else
    {
        glDisable(GL_NORMALIZE);
    }

    if(glState->GetSeparateSpecularColorEnable())
    {
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    }
    else
    {
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
    }

    glState->SetMaterialChanged(false);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    material = glState->GetMaterial();
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  material->materialDiffuseColorVector);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material->materialSpecularColorVector);
    glMaterialfv(GL_FRONT, GL_AMBIENT,  material->materialAmbientColorVector);
    glMaterialf (GL_FRONT, GL_SHININESS,material->materialShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, material->materialEmissionColorVector);

    if(glState->GetFogChanged())
    {
        glState->SetFogChanged(false);
        if(glState->GetFogEnable())
        {
            glEnable(GL_FOG);
            glFogi (GL_FOG_MODE,    glState->GetFog()->fogMode);
            glFogfv(GL_FOG_COLOR,   glState->GetFog()->fogColorVector);
            glFogf (GL_FOG_DENSITY, glState->GetFog()->fogDensity);
            glFogf (GL_FOG_START,   glState->GetFog()->fogStart);
            glFogf (GL_FOG_END,     glState->GetFog()->fogEnd);
        }
        else
        {
            glDisable(GL_FOG);
        }
    }
}

vec3 SGCanvas::GetWorldSpace(int x, int y)
{
    vec3 v;
    v.x = (float) x / (float) m_width;
    v.y = 1 - (float) y / (float) m_height;
    v.x *= (m_right - m_left);
    v.y *= (m_top - m_bottom);
    v.x += m_left;
    v.y += m_bottom;
    v.z = CameraZ;
    return v;
}

void SGCanvas::OnMouse(wxMouseEvent &event)
{
    mouse.OnMouse(event);
    Update();
}

void SGCanvas::OnKey(wxKeyEvent &event)
{
    switch(event.GetKeyCode()){
        case 312: //page down
            m_zoom -= 0.1f;
            break;
        case 313: //page up
            m_zoom += 0.1f;
            break;
        case 348:
            m_parent->SwitchMode();
            break;
    }
    Update();
    event.Skip();
}

bool SGCanvas::LinkShaders(const GLchar *vertexShader, const GLchar *fragmentShader)
{
    GLint linked;
    SGFixedGLState *glState = GetGLState();

    if(glLinked)
    {
        return true;
    }

    if(!glCompiled)
    {
        CompileShaders(vertexShader, fragmentShader);
    }

    if(glCompiled)
    {
        WriteMessage(wxT("Attempting to link programs...."));
        if(glIsProgram(prog))
        {
            glDeleteProgram(prog);
        }
        prog = glCreateProgram();
        glAttachShader(prog, vertS);
        glAttachShader(prog, fragS);
        glLinkProgram(prog);
        glGetProgramiv(prog, GL_LINK_STATUS, &linked);
        PrintInfoLog(prog);
        if (!linked)
        {
            WriteMessage(wxT("Error in linking programs!!"));
            glLinked = false;
            return false;
        }
        else
        {
            glUseProgram(prog);
            for(int i=0;i<5;i++)
            {
                GLint uniformLocation;

                switch(i){
                    case 0: 
                        uniformLocation = glGetUniformLocation(prog, "texUnit0");
                        glUniform1i(uniformLocation, 0);
                        break;
                    case 1:
                        uniformLocation = glGetUniformLocation(prog, "texUnit1");
                        glUniform1i(uniformLocation, 1);
                        break;
                    case 2:
                        uniformLocation = glGetUniformLocation(prog, "texUnit2");
                        glUniform1i(uniformLocation, 2);
                        break;
                    case 3:
                        uniformLocation = glGetUniformLocation(prog, "texUnit3");
                        glUniform1i(uniformLocation, 3);
                        break;
                    case 4:
                        uniformLocation = glGetUniformLocation(prog, "texUnit4");
                        glUniform1i(uniformLocation, 4);
                        break;
                    default:
                        break;
                }
            }
        }

        WriteMessage(wxT("Linked programs successfully"));

        glLinked = true;
        return true;
    }
    else
    {
        WriteMessage(wxT("Compilation failed, not attempting link, check shader code"));
        glLinked = false;

        glDeleteProgram(prog);
        return false;
    }
}

bool SGCanvas::CompileShaders(const GLchar *vertexShader, const GLchar *fragmentShader)
{   

    GLint vertCompiled, fragCompiled;
    glLinked = false ;
    if(vertS || fragS)
    {
        if(glIsProgram(prog))
        {
            glDeleteProgram(prog);
        }
    }
    vertS = glCreateShader(GL_VERTEX_SHADER);
    fragS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertS, 1, (const GLchar **)&vertexShader, NULL);
    glShaderSource(fragS, 1, (const GLchar **)&fragmentShader, NULL);
    glCompileShader(vertS);
    glGetShaderiv(vertS, GL_COMPILE_STATUS, &vertCompiled);
    if(!vertCompiled)
    {
        WriteMessage(wxT("Vertex shader failed to compile"));
        PrintInfoLog(vertS);
        glDeleteShader(vertS);
    }
    else
    {
        WriteMessage(wxT("Vertex shader compiled successfully"));
    }
    PrintInfoLog(vertS);
    glCompileShader(fragS);
    glGetShaderiv(fragS, GL_COMPILE_STATUS, &fragCompiled);
    if(!fragCompiled)
    {
        WriteMessage(wxT("Fragment shader failed to compile"));
        PrintInfoLog(fragS);
        glDeleteShader(fragS);
    }
    else
    {
        WriteMessage(wxT("Fragment shader compiled successfully"));
    }
    PrintInfoLog(fragS);

    if (!vertCompiled || !fragCompiled)
    {
        glCompiled = false;
        return false;
    }
    glCompiled = true;

    return true;
}

void SGCanvas::PrintInfoLog(GLuint obj)
{
    GLint infologLength = 0;
    int charsWritten  = 0;
    wxString errors;
    GLchar *infoLog;
    wxTextCtrl *text = m_frame->GetShaderTextWindow()->GetInfoBox();

    if(glIsProgram(obj))
    {
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    }
    else if(glIsShader(obj))
    {
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    }
    else
    {
        text->SetValue(text->GetValue() + wxT("ERROR: No Shader or Program available"));
    }

    PrintOpenGLError(); 

    if (infologLength > 0)
    {
        infoLog = new GLchar[infologLength];
        if (infoLog == NULL)
        {
            text->SetValue(text->GetValue()+ wxT("ERROR: Could not allocate InfoLog buffer\n"));
            return;
        }
        if(glIsProgram(obj))
        {
            glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        }
        else if(glIsShader(obj))
        {
            glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        }
        else
        {
            text->SetValue(text->GetValue() + wxT("ERROR: No Shader or Program available"));
        }

        errors = wxString::Format(wxT("InfoLog:%s\n"), infoLog);

        text->AppendText(errors);

        delete[] infoLog;
    }
}

int SGCanvas::SwitchToShaderMode()
{   
    if( !m_frame->GetShaderTextWindow()->haveRefreshed)
    {
        m_frame->GetShaderTextWindow()->Refresh();
    }
    wxString vert= m_frame->GetShaderTextWindow()->GetVertexShaderBox()->GetValue();
    wxString frag= m_frame->GetShaderTextWindow()->GetFragmentShaderBox()->GetValue();
    LinkShaders(vert,frag);
    Update();
    return 0;
}

void SGCanvas::WriteMessage(const wxString str)
{
    m_frame->SetStatusText(str);
    wxTextCtrl *text = m_frame->GetShaderTextWindow()->GetInfoBox();
    text->SetValue(text->GetValue()+ str+wxT("\n"));
}

GLint SGCanvas::GetUniLoc(unsigned int program, const GLchar *name)
{
    GLint loc;
    loc = glGetUniformLocation(program, name);
    if (loc == -1)
    {
        WriteMessage(wxString::Format(wxT("No such uniform named \"%s\"\n"), name));
    }
    PrintOpenGLError();
    return loc;
}

void SGCanvas::CheckGlImplementation()
{
    int gl_major, gl_minor, gl_numTextures;
    GetGlVersion(&gl_major, &gl_minor);
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &gl_numTextures);
    if(gl_major < 2.0)
    {
        UnsupportedOpenGLVersion();
    }
    else if(gl_numTextures < 5)
    {
        NotEnoughTextureUnits(gl_numTextures);
    }
}

void SGCanvas::GetGlVersion(int *major, int* minor)
{
    const char* verstr = (const char*)glGetString(GL_VERSION);
    if( (verstr == NULL) || (sscanf(verstr, wxT("%d.%d"), major, minor) != 2))
    {
        *major = *minor = 0;
        fprintf(stderr, wxT("Invalid GL_VERSION format!!!\n"));
    }
}

void SGCanvas::UnsupportedOpenGLVersion()
{
    wxString errorString(wxString(wxT("You must have OpenGL 2.0 compliant drivers to run ShaderGen!")));
    wxMessageDialog badValues(this, errorString, wxT("OpenGL 2.0 Driver Not Found"), wxOK | wxICON_EXCLAMATION, wxDefaultPosition);
    if(badValues.ShowModal()==wxID_OK)
    {
        exit(1);
    }
}

void SGCanvas::NotEnoughTextureUnits(const int numTextures)
{
    wxString errorString;
    errorString.Printf(wxT("Your OpenGL Graphics Card Only Supports %i Texture Units, Some ShaderGen Features May Not Work As Expected!"), numTextures);
    wxMessageDialog insufficientTextures(this, errorString, wxT("Insufficient Texture Units"), wxOK | wxICON_EXCLAMATION, wxDefaultPosition);
    insufficientTextures.ShowModal();
}
