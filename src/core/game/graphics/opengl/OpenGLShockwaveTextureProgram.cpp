//
//  OpenGLShockwaveTextureProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLShockwaveTextureProgram.h"

#include "OpenGLManager.h"

OpenGLShockwaveTextureProgram::OpenGLShockwaveTextureProgram(const char* vertexShaderName, const char* fragmentShaderName) : OpenGLProgram(vertexShaderName, fragmentShaderName)
{
    u_mvp_matrix_location = glGetUniformLocation(m_programObjectId, "u_MvpMatrix");
    u_texture_unit_location = glGetUniformLocation(m_programObjectId, "u_TextureUnit");
    u_center_x_unit_location = glGetUniformLocation(m_programObjectId, "u_CenterX");
    u_center_y_unit_location = glGetUniformLocation(m_programObjectId, "u_CenterY");
    u_time_elapsed_unit_location = glGetUniformLocation(m_programObjectId, "u_TimeElapsed");
    u_is_transforming = glGetUniformLocation(m_programObjectId, "u_IsTransforming");
    a_position_location = glGetAttribLocation(m_programObjectId, "a_Position");
    a_color_location = glGetAttribLocation(m_programObjectId, "a_Color");
    a_texture_coordinates_location = glGetAttribLocation(m_programObjectId, "a_TextureCoordinates");
}

void OpenGLShockwaveTextureProgram::bind()
{
    OpenGLProgram::bind();
    
    glUniformMatrix4fv(u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)OGLManager->getViewProjectionMatrix());
    
    glUniform1i(u_texture_unit_location, 0);
    
    mapBuffer(OGLManager->getSbVboObject(), OGLManager->getTextureVertices());
    
    glVertexAttribPointer(a_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    glVertexAttribPointer(a_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
    glVertexAttribPointer(a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(7 * sizeof(GL_FLOAT)));
    
    glEnableVertexAttribArray(a_position_location);
    glEnableVertexAttribArray(a_color_location);
    glEnableVertexAttribArray(a_texture_coordinates_location);
}

void OpenGLShockwaveTextureProgram::unbind()
{
    unmapBuffer(OGLManager->getSbVboObject());
    
    OpenGLProgram::unbind();
}
