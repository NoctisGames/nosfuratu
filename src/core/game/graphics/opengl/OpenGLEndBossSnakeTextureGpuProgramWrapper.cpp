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
#include "FrameworkConstants.h"
#include "GameConstants.h"

OpenGLEndBossSnakeTextureGpuProgramWrapper::OpenGLEndBossSnakeTextureGpuProgramWrapper() : EndBossSnakeTextureGpuProgramWrapper(), m_program(new OpenGLTextureProgram(TEXTURE_VERTEX_SHADER, END_BOSS_FRAGMENT_SHADER))
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
