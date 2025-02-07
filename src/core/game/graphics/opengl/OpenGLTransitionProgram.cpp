//
//  OpenGLTransitionProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLTransitionProgram.h"

#include "OpenGLManager.h"

OpenGLTransitionProgram::OpenGLTransitionProgram(const char* vertexShaderName, const char* fragmentShaderName) : OpenGLProgram(vertexShaderName, fragmentShaderName)
{
    u_from_location = glGetUniformLocation(m_programObjectId, "u_from");
    u_to_location = glGetUniformLocation(m_programObjectId, "u_to");
    u_progress_location = glGetUniformLocation(m_programObjectId, "u_progress");
    a_position_location = glGetAttribLocation(m_programObjectId, "a_Position");
}

void OpenGLTransitionProgram::bind()
{
    OpenGLProgram::bind();
    
    glUniform1i(u_from_location, 0);
    glUniform1i(u_to_location, 1);
    
    mapBuffer(OGLManager->getSbVboObject(), OGLManager->getTextureVertices());
    
    glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(a_position_location);
}

void OpenGLTransitionProgram::unbind()
{
    unmapBuffer(OGLManager->getSbVboObject());
    
    OpenGLProgram::unbind();
}
