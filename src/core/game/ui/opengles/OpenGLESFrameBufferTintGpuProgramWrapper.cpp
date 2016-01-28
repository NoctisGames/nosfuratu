//
//  OpenGLESFrameBufferTintGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/23/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESFrameBufferTintGpuProgramWrapper.h"
#include "OpenGLESManager.h"
#include "macros.h"

extern "C"
{
#include "asset_utils.h"
}

OpenGLESFrameBufferTintGpuProgramWrapper::OpenGLESFrameBufferTintGpuProgramWrapper()
{
    m_program = FrameBufferToScreenProgram::build(build_program_from_assets("frame_buffer_to_screen_shader.vsh", "frame_buffer_to_screen_purple_highlight_shader.fsh"));
    m_isLoaded = true;
}

void OpenGLESFrameBufferTintGpuProgramWrapper::bind()
{
    glUseProgram(m_program.program);
    
    glUniform1i(m_program.u_texture_unit_location, 0);
    
    glGenBuffers(1, &OGLESManager->sb_vbo_object);
    glBindBuffer(GL_ARRAY_BUFFER, OGLESManager->sb_vbo_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * OGLESManager->m_textureVertices.size(), &OGLESManager->m_textureVertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(m_program.a_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(m_program.a_position_location);
}

void OpenGLESFrameBufferTintGpuProgramWrapper::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &OGLESManager->sb_vbo_object);
    
    glUseProgram(0);
}

void OpenGLESFrameBufferTintGpuProgramWrapper::cleanUp()
{
    glDeleteProgram(m_program.program);
}