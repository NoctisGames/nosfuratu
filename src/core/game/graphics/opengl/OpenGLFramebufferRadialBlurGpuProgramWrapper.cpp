//
//  OpenGLFramebufferRadialBlurGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/29/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLFramebufferRadialBlurGpuProgramWrapper.h"

#include "OpenGLFramebufferRadialBlurGpuProgram.h"
#include "OpenGLManager.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

OpenGLFramebufferRadialBlurGpuProgramWrapper::OpenGLFramebufferRadialBlurGpuProgramWrapper() : FramebufferRadialBlurGpuProgramWrapper(), m_program(new OpenGLFramebufferRadialBlurGpuProgram(FRAMEBUFFER_TO_SCREEN_VERTEX_SHADER, RADIAL_BLUR_FRAGMENT_SHADER))
{
    // Empty
}

OpenGLFramebufferRadialBlurGpuProgramWrapper::~OpenGLFramebufferRadialBlurGpuProgramWrapper()
{
    delete m_program;
}

void OpenGLFramebufferRadialBlurGpuProgramWrapper::bind()
{
    OGLManager->useScreenBlending();
    
    m_program->bind();
    
    glUniform1f(m_program->u_direction_location, m_fDirection);
}

void OpenGLFramebufferRadialBlurGpuProgramWrapper::unbind()
{
    m_program->unbind();
}
