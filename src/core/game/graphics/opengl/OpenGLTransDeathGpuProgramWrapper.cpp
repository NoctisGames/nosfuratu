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
#include "FrameworkConstants.h"
#include "GameConstants.h"

OpenGLTransDeathGpuProgramWrapper::OpenGLTransDeathGpuProgramWrapper(bool isTransIn) : TransDeathGpuProgramWrapper(isTransIn), m_program(new OpenGLTransDeathProgram(FRAMEBUFFER_TO_SCREEN_VERTEX_SHADER, isTransIn ? TRANS_DEATH_IN_FRAGMENT_SHADER : TRANS_DEATH_OUT_FRAGMENT_SHADER))
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
