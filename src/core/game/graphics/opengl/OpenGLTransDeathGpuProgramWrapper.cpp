//
//  OpenGLTransDeathGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLTransDeathGpuProgramWrapper.h"

#include "OpenGLTransDeathProgram.h"
#include "OpenGLManager.h"
#include "GpuTextureWrapper.h"

OpenGLTransDeathGpuProgramWrapper::OpenGLTransDeathGpuProgramWrapper(bool isTransIn) : TransDeathGpuProgramWrapper(isTransIn), m_program(new OpenGLTransDeathProgram("frame_buffer_to_screen_shader.vsh", isTransIn ? "trans_in_death_shader.fsh" : "trans_out_death_shader.fsh"))
{
    // Empty
}

OpenGLTransDeathGpuProgramWrapper::~OpenGLTransDeathGpuProgramWrapper()
{
    delete m_program;
}

void OpenGLTransDeathGpuProgramWrapper::bind()
{
    OGLManager->useScreenBlending();
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_grayMap->texture);
    
    m_program->bind();
    
    glUniform1f(m_program->u_time_elapsed_unit_location, m_fTimeElapsed);
}

void OpenGLTransDeathGpuProgramWrapper::unbind()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    m_program->unbind();
}
