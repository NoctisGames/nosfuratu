//
//  OpenGLFramebufferRadialBlurGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/29/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLFramebufferRadialBlurGpuProgramWrapper.h"
#include "OpenGLManager.h"
#include "macros.h"

extern "C"
{
#include "asset_utils.h"
}

OpenGLFramebufferRadialBlurGpuProgramWrapper::OpenGLFramebufferRadialBlurGpuProgramWrapper()
{
    m_program = FramebufferToScreenProgram::build(build_program_from_assets("frame_buffer_to_screen_shader.vsh", "pp_radial_blue_texture_shader.fsh"));
    m_isLoaded = true;
}

void OpenGLFramebufferRadialBlurGpuProgramWrapper::bind()
{
    OGLESManager->useScreenBlending();
    
    glUseProgram(m_program.program);
    
    glUniform1i(m_program.u_texture_unit_location, 0);
    glUniform1f(m_program.u_direction_location, m_fDirection);
    
    glGenBuffers(1, &OGLESManager->sb_vbo_object);
    glBindBuffer(GL_ARRAY_BUFFER, OGLESManager->sb_vbo_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * OGLESManager->m_textureVertices.size(), &OGLESManager->m_textureVertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(m_program.a_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(m_program.a_position_location);
}

void OpenGLFramebufferRadialBlurGpuProgramWrapper::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &OGLESManager->sb_vbo_object);
    
    glUseProgram(0);
}

void OpenGLFramebufferRadialBlurGpuProgramWrapper::cleanUp()
{
    glDeleteProgram(m_program.program);
}
