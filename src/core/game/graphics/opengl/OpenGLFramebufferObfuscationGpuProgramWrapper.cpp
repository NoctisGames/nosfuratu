//
//  OpenGLFramebufferObfuscationGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/17/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLFramebufferObfuscationGpuProgramWrapper.h"

#include "OpenGLFramebufferToScreenProgram.h"
#include "OpenGLManager.h"
#include "GpuTextureWrapper.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

OpenGLFramebufferObfuscationGpuProgramWrapper::OpenGLFramebufferObfuscationGpuProgramWrapper() : GpuProgramWrapper(), m_program(new OpenGLFramebufferToScreenProgram(FRAMEBUFFER_TO_SCREEN_VERTEX_SHADER, OBFUSCATION_FRAGMENT_SHADER))
{
    // Empty
}

OpenGLFramebufferObfuscationGpuProgramWrapper::~OpenGLFramebufferObfuscationGpuProgramWrapper()
{
    delete m_program;
}

void OpenGLFramebufferObfuscationGpuProgramWrapper::bind()
{
    OGLManager->useNormalBlending();
    
    m_program->bind();
}

void OpenGLFramebufferObfuscationGpuProgramWrapper::unbind()
{
    m_program->unbind();
}
