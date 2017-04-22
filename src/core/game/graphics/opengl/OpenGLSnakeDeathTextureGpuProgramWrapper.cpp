//
//  OpenGLSnakeDeathTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLSnakeDeathTextureGpuProgramWrapper.h"

#include "OpenGLTextureProgram.h"
#include "OpenGLManager.h"

OpenGLSnakeDeathTextureGpuProgramWrapper::OpenGLSnakeDeathTextureGpuProgramWrapper() : SnakeDeathTextureGpuProgramWrapper(), m_program(new OpenGLTextureProgram("texture_shader.vsh", "snake_death_texture_shader.fsh"))
{
    // Empty
}

OpenGLSnakeDeathTextureGpuProgramWrapper::~OpenGLSnakeDeathTextureGpuProgramWrapper()
{
    delete m_program;
}

void OpenGLSnakeDeathTextureGpuProgramWrapper::bind()
{
    OGLManager->useNormalBlending();
    
    m_program->bind();
}

void OpenGLSnakeDeathTextureGpuProgramWrapper::unbind()
{
    m_program->unbind();
}
