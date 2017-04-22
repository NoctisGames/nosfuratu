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

OpenGLFramebufferObfuscationGpuProgramWrapper::OpenGLFramebufferObfuscationGpuProgramWrapper() : GpuProgramWrapper(), m_program(new OpenGLFramebufferToScreenProgram("frame_buffer_to_screen_shader.vsh", "pp_frame_buffer_obfuscation.fsh"))
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
