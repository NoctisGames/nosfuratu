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

OpenGLFramebufferTintGpuProgramWrapper::OpenGLFramebufferTintGpuProgramWrapper() : GpuProgramWrapper(), m_program(new OpenGLFramebufferToScreenProgram("frame_buffer_to_screen_shader.vsh", "frame_buffer_to_screen_purple_highlight_shader.fsh"))
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
