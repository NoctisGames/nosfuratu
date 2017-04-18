//
//  OpenGLEndBossSnakeTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/26/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLEndBossSnakeTextureGpuProgramWrapper.h"

#include "OpenGLTextureProgram.h"
#include "OpenGLManager.h"

OpenGLEndBossSnakeTextureGpuProgramWrapper::OpenGLEndBossSnakeTextureGpuProgramWrapper() : EndBossSnakeTextureGpuProgramWrapper(), m_program(new OpenGLTextureProgram("texture_shader.vsh", "end_boss_snake_texture_shader.fsh"))
{
    // Empty
}

OpenGLEndBossSnakeTextureGpuProgramWrapper::~OpenGLEndBossSnakeTextureGpuProgramWrapper()
{
    delete m_program;
}

void OpenGLEndBossSnakeTextureGpuProgramWrapper::bind()
{
    OGLManager->useNormalBlending();
    
    m_program->bind();
}

void OpenGLEndBossSnakeTextureGpuProgramWrapper::unbind()
{
    m_program->unbind();
}
