//
//  OpenGLFramebufferRadialBlurGpuProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/2/17.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLFramebufferRadialBlurGpuProgram.h"

#include "OpenGLManager.h"

OpenGLFramebufferRadialBlurGpuProgram::OpenGLFramebufferRadialBlurGpuProgram(const char* vertexShaderPath, const char* fragmentShaderPath) : OpenGLProgram(vertexShaderPath, fragmentShaderPath)
{
    u_texture_unit_location = glGetUniformLocation(m_programObjectId, "u_TextureUnit");
    u_direction_location = glGetUniformLocation(m_programObjectId, "u_Direction");
    a_position_location = glGetAttribLocation(m_programObjectId, "a_Position");
}

void OpenGLFramebufferRadialBlurGpuProgram::bind()
{
    OpenGLProgram::bind();
    
    glUniform1i(u_texture_unit_location, 0);
    
    mapBuffer(OGLManager->getSbVboObject(), OGLManager->getTextureVertices());
    
    glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(a_position_location);
}

void OpenGLFramebufferRadialBlurGpuProgram::unbind()
{
    unmapBuffer(OGLManager->getSbVboObject());
    
    OpenGLProgram::unbind();
}
