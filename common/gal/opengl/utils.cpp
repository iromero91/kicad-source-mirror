/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2016-2017 CERN
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <confirm.h>    // DisplayError

#include <GL/glew.h>
#include <stdexcept>

int checkGlError( const std::string& aInfo, bool aThrow )
{
    int result = glGetError();
    wxString errorMsg;

    switch( result )
    {
        case GL_NO_ERROR:
            // all good
            break;

        case GL_INVALID_ENUM:
            errorMsg = wxString::Format( "Error: %s: invalid enum", aInfo );
            break;

        case GL_INVALID_VALUE:
            errorMsg = wxString::Format( "Error: %s: invalid value", aInfo );
            break;

        case GL_INVALID_OPERATION:
            errorMsg = wxString::Format( "Error: %s: invalid operation", aInfo );
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorMsg = wxString::Format( "Error: %s: invalid framebuffer operation", aInfo );
            break;

        case GL_OUT_OF_MEMORY:
            errorMsg = wxString::Format( "Error: %s: out of memory", aInfo );
            break;

        case GL_STACK_UNDERFLOW:
            errorMsg = wxString::Format( "Error: %s: stack underflow", aInfo );
            break;

        case GL_STACK_OVERFLOW:
            errorMsg = wxString::Format( "Error: %s: stack overflow", aInfo );
            break;

        default:
            errorMsg = wxString::Format( "Error: %s: unknown error", aInfo );
            break;
    }

    if( result != GL_NO_ERROR )
    {
        if( aThrow )
            throw std::runtime_error( (const char*) errorMsg.char_str() );
        else
            DisplayErrorMessage( nullptr, "OpenGL error occurred", errorMsg );
    }

    return result;
}


// debugMsgCallback is a callback function for glDebugMessageCallback.
// It must have the right type ( GLAPIENTRY )
static void GLAPIENTRY debugMsgCallback( GLenum aSource, GLenum aType, GLuint aId,
   GLenum aSeverity, GLsizei aLength, const GLchar* aMessage, const void* aUserParam )
{
    printf( "%s", aMessage );
}


void enableGlDebug( bool aEnable )
{
    if( aEnable )
    {
        glEnable( GL_DEBUG_OUTPUT );
        glDebugMessageCallback( (GLDEBUGPROC) debugMsgCallback, nullptr );
    }
    else
    {
        glDisable( GL_DEBUG_OUTPUT );
    }
}
