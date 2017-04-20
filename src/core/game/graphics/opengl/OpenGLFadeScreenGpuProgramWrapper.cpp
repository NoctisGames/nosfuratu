//
//  OpenGLFadeScreenGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLFadeScreenGpuProgramWrapper.h"

#include "OpenGLTransitionProgram.h"
#include "OpenGLManager.h"
#include "GpuTextureWrapper.h"

OpenGLFadeScreenGpuProgramWrapper::OpenGLFadeScreenGpuProgramWrapper() : TransitionGpuProgramWrapper(), m_program(new OpenGLTransitionProgram("frame_buffer_to_screen_shader.vsh", "fade_screen_shader.fsh"))
{
    // Empty
}

OpenGLFadeScreenGpuProgramWrapper::~OpenGLFadeScreenGpuProgramWrapper()
{
    delete m_program;
}

void OpenGLFadeScreenGpuProgramWrapper::bind()
{
    OGLManager->useScreenBlending();
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_to->texture);
    
    m_program->bind();
    
    glUniform1f(m_program->u_progress_location, m_fProgress);
}

void OpenGLFadeScreenGpuProgramWrapper::unbind()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    m_program->unbind();
}
