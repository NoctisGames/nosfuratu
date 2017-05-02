//
//  Direct3DEndBossSnakeTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/26/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DEndBossSnakeTextureGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

Direct3DEndBossSnakeTextureGpuProgramWrapper::Direct3DEndBossSnakeTextureGpuProgramWrapper() : EndBossSnakeTextureGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(TEXTURE_VERTEX_SHADER, END_BOSS_FRAGMENT_SHADER))
{
    // Empty
}

Direct3DEndBossSnakeTextureGpuProgramWrapper::~Direct3DEndBossSnakeTextureGpuProgramWrapper()
{
    delete m_program;
}

void Direct3DEndBossSnakeTextureGpuProgramWrapper::bind()
{
    D3DManager->useNormalBlending();
    
    m_program->bindShaders();

    m_program->bindNormalSamplerState();
    
	m_program->bindMatrix();

	m_program->mapVertices();
}

void Direct3DEndBossSnakeTextureGpuProgramWrapper::unbind()
{
	// Empty
}
