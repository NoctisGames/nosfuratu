//
//  OpenGLTransDeathGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLTransDeathGpuProgramWrapper.h"
#include "OpenGLManager.h"
#include "macros.h"

extern "C"
{
#include "asset_utils.h"
}

OpenGLTransDeathGpuProgramWrapper::OpenGLTransDeathGpuProgramWrapper(bool isTransIn) : TransDeathGpuProgramWrapper(isTransIn)
{
    m_program = TransDeathProgram::build(build_program_from_assets("frame_buffer_to_screen_shader.vsh", isTransIn ? "trans_in_death_shader.fsh" : "trans_out_death_shader.fsh"));
    m_isLoaded = true;
}

void OpenGLTransDeathGpuProgramWrapper::bind()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_grayMap->texture);
    
    glUseProgram(m_program.program);
    
    glUniform1i(m_program.u_texture_unit_location, 0);
    glUniform1i(m_program.u_texture_unit_gray_map_location, 1);
    glUniform1f(m_program.u_time_elapsed_unit_location, m_fTimeElapsed);
    
    glGenBuffers(1, &OGLESManager->sb_vbo_object);
    glBindBuffer(GL_ARRAY_BUFFER, OGLESManager->sb_vbo_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * OGLESManager->m_textureVertices.size(), &OGLESManager->m_textureVertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(m_program.a_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(m_program.a_position_location);
}

void OpenGLTransDeathGpuProgramWrapper::unbind()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &OGLESManager->sb_vbo_object);
    
    glUseProgram(0);
}

void OpenGLTransDeathGpuProgramWrapper::cleanUp()
{
    glDeleteProgram(m_program.program);
}
