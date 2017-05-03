//
//  OpenGLFramebufferTintGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/23/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLFramebufferTintGpuProgramWrapper.h"

#include "OpenGLFramebufferToScreenProgram.h"
#include "OpenGLManager.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

OpenGLFramebufferTintGpuProgramWrapper::OpenGLFramebufferTintGpuProgramWrapper() : GpuProgramWrapper(), m_program(new OpenGLFramebufferToScreenProgram(FRAMEBUFFER_TO_SCREEN_VERTEX_SHADER, TINT_TO_SCREEN_FRAGMENT_SHADER))
{
    // Empty
}

OpenGLFramebufferTintGpuProgramWrapper::~OpenGLFramebufferTintGpuProgramWrapper()
{
    delete m_program;
}

void OpenGLFramebufferTintGpuProgramWrapper::bind()
{
    OGLManager->useScreenBlending();
    
    m_program->bind();
}

void OpenGLFramebufferTintGpuProgramWrapper::unbind()
{
    m_program->unbind();
}
