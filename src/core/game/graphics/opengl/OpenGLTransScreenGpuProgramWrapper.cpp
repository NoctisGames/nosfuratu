//
//  OpenGLTransScreenGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLTransScreenGpuProgramWrapper.h"

#include "OpenGLTransitionProgram.h"
#include "OpenGLManager.h"
#include "GpuTextureWrapper.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

OpenGLTransScreenGpuProgramWrapper::OpenGLTransScreenGpuProgramWrapper() : TransitionGpuProgramWrapper(), m_program(new OpenGLTransitionProgram(FRAMEBUFFER_TO_SCREEN_VERTEX_SHADER, TRANS_SCREEN_FRAGMENT_SHADER))
{
    // Empty
}

OpenGLTransScreenGpuProgramWrapper::~OpenGLTransScreenGpuProgramWrapper()
{
    delete m_program;
}

void OpenGLTransScreenGpuProgramWrapper::bind()
{
    OGLManager->useScreenBlending();
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_to->texture);
    
    m_program->bind();
    
    glUniform1f(m_program->u_progress_location, m_fProgress);
}

void OpenGLTransScreenGpuProgramWrapper::unbind()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    m_program->unbind();
}
