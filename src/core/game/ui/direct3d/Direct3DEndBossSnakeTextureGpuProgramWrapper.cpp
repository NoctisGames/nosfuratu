//
//  Direct3DEndBossSnakeTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/26/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DEndBossSnakeTextureGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "DeviceResources.h"
#include "Direct3DManager.h"

Direct3DEndBossSnakeTextureGpuProgramWrapper::Direct3DEndBossSnakeTextureGpuProgramWrapper() : EndBossSnakeTextureGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(L"TextureVertexShader.cso", L"EndBossSnakeTexturePixelShader.cso"))
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
