/************************************************************************
* Author(s):    Inderaj Bains and Joshua Doss                           *
* Last Modified: 11/02/2005                                            *
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

#include <QMessageBox>

#include "UtilityFunctions.h"
#include "SGShaderTextWindow.h"
#include "SGFrame.h"

// Returns 1 if an OpenGL error occurred, 0 otherwise.
int printOglError(char *file, int line)
{
    GLenum glErr;
    int    retCode = 0;

    QString str ;
    QTextEdit *text = SGFrame::instance->GetShaderTextWindow()->GetInfoBox();
    text->textCursor().movePosition(QTextCursor::End);

    glErr = glGetError();

    if(DEBUG_ON)
    {
        while (glErr != GL_NO_ERROR)
        {
            str = QString("\nglError in file %1 @ line %2: %3\n").arg(file).arg(line).arg( (const char*)gluErrorString(glErr));
            text->append(str);
            retCode = 1;
            glErr = glGetError();
        }
    }
    return retCode;
}

void IncorrectFormat(QString str, QWidget *errorWindow)
{
    QString errorString("Enter data in the correct format, you need ");
    errorString += str;

    QMessageBox::warning(errorWindow, QObject::tr("Incorrect Data Entry"), errorString);
}
