//
//  OpenGLESSnakeDeathTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESSnakeDeathTextureGpuProgramWrapper.h"
#include "OpenGLESManager.h"
#include "macros.h"

extern "C"
{
#include "asset_utils.h"
}

OpenGLESSnakeDeathTextureGpuProgramWrapper::OpenGLESSnakeDeathTextureGpuProgramWrapper()
{
    m_program = SnakeDeathTextureProgram::build(build_program_from_assets("texture_shader.vsh", "snake_death_texture_shader.fsh"));
    m_isLoaded = true;
}

void OpenGLESSnakeDeathTextureGpuProgramWrapper::bind()
{
    glUseProgram(m_program.program);
    
    glUniformMatrix4fv(m_program.u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)OGLESManager->m_viewProjectionMatrix);
    glUniform1i(m_program.u_texture_unit_location, 0);
    glUniform1f(m_program.u_color_additive_unit_location, m_fColorAdditive);
    
    glGenBuffers(1, &OGLESManager->sb_vbo_object);
    glBindBuffer(GL_ARRAY_BUFFER, OGLESManager->sb_vbo_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * OGLESManager->m_textureVertices.size(), &OGLESManager->m_textureVertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(m_program.a_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    glVertexAttribPointer(m_program.a_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
    glVertexAttribPointer(m_program.a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(7 * sizeof(GL_FLOAT)));
    
    glEnableVertexAttribArray(m_program.a_position_location);
    glEnableVertexAttribArray(m_program.a_color_location);
    glEnableVertexAttribArray(m_program.a_texture_coordinates_location);
}

void OpenGLESSnakeDeathTextureGpuProgramWrapper::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &OGLESManager->sb_vbo_object);
    
    glUseProgram(0);
}

void OpenGLESSnakeDeathTextureGpuProgramWrapper::cleanUp()
{
    glDeleteProgram(m_program.program);
}