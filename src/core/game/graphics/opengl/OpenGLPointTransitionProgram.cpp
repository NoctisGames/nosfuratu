//
//  OpenGLPointTransitionProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLPointTransitionProgram.h"

#include "OpenGLManager.h"

OpenGLPointTransitionProgram::OpenGLPointTransitionProgram(const char* vertexShaderPath, const char* fragmentShaderPath) : OpenGLProgram(vertexShaderPath, fragmentShaderPath)
{
    u_mvp_matrix_location = glGetUniformLocation(m_programObjectId, "u_MvpMatrix");
    u_center_x_unit_location = glGetUniformLocation(m_programObjectId, "u_CenterX");
    u_center_y_unit_location = glGetUniformLocation(m_programObjectId, "u_CenterY");
    u_from_location = glGetUniformLocation(m_programObjectId, "u_from");
    u_to_location = glGetUniformLocation(m_programObjectId, "u_to");
    u_progress_location = glGetUniformLocation(m_programObjectId, "u_progress");
    a_position_location = glGetAttribLocation(m_programObjectId, "a_Position");
}

void OpenGLPointTransitionProgram::bind()
{
    OpenGLProgram::bind();
    
    glUniformMatrix4fv(u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)OGLManager->getViewProjectionMatrix());
    glUniform1i(u_from_location, 0);
    glUniform1i(u_to_location, 1);
    
    mapBuffer(OGLManager->getSbVboObject(), OGLManager->getTextureVertices());
    
    glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(a_position_location);
}

void OpenGLPointTransitionProgram::unbind()
{
    unmapBuffer(OGLManager->getSbVboObject());
    
    OpenGLProgram::unbind();
}
