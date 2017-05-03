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
#include "FrameworkConstants.h"
#include "GameConstants.h"

OpenGLSnakeDeathTextureGpuProgramWrapper::OpenGLSnakeDeathTextureGpuProgramWrapper() : SnakeDeathTextureGpuProgramWrapper(), m_program(new OpenGLTextureProgram(TEXTURE_VERTEX_SHADER, SNAKE_DEATH_FRAGMENT_SHADER))
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
