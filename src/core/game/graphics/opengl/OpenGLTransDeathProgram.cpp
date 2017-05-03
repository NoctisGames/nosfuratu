//
//  OpenGLTransDeathProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLTransDeathProgram.h"

#include "OpenGLManager.h"

OpenGLTransDeathProgram::OpenGLTransDeathProgram(const char* vertexShaderName, const char* fragmentShaderName) : OpenGLProgram(vertexShaderName, fragmentShaderName)
{
    u_texture_unit_location = glGetUniformLocation(m_programObjectId, "u_TextureUnit");
    u_texture_unit_gray_map_location = glGetUniformLocation(m_programObjectId, "u_TextureUnitGrayMap");
    u_time_elapsed_unit_location = glGetUniformLocation(m_programObjectId, "u_TimeElapsed");
    a_position_location = glGetAttribLocation(m_programObjectId, "a_Position");
}

void OpenGLTransDeathProgram::bind()
{
    OpenGLProgram::bind();
    
    glUniform1i(u_texture_unit_location, 0);
    glUniform1i(u_texture_unit_gray_map_location, 1);
    
    mapBuffer(OGLManager->getSbVboObject(), OGLManager->getTextureVertices());
    
    glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(a_position_location);
}

void OpenGLTransDeathProgram::unbind()
{
    unmapBuffer(OGLManager->getSbVboObject());
    
    OpenGLProgram::unbind();
}
