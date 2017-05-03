//
//  Direct3DSnakeDeathTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/29/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DSnakeDeathTextureGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

Direct3DSnakeDeathTextureGpuProgramWrapper::Direct3DSnakeDeathTextureGpuProgramWrapper() : SnakeDeathTextureGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(TEXTURE_VERTEX_SHADER, SNAKE_DEATH_FRAGMENT_SHADER))
{
    // Empty
}

Direct3DSnakeDeathTextureGpuProgramWrapper::~Direct3DSnakeDeathTextureGpuProgramWrapper()
{
    delete m_program;
}

void Direct3DSnakeDeathTextureGpuProgramWrapper::bind()
{
    D3DManager->useNormalBlending();
    
    m_program->bindShaders();
    
    m_program->bindNormalSamplerState();

	m_program->bindMatrix();

	m_program->mapVertices();
}

void Direct3DSnakeDeathTextureGpuProgramWrapper::unbind()
{
	// Empty
}
