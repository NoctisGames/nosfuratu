//
//  Direct3DBackgroundTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/3/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DBackgroundTextureGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"

Direct3DBackgroundTextureGpuProgramWrapper::Direct3DBackgroundTextureGpuProgramWrapper() : GpuProgramWrapper(),
m_program(new Direct3DTextureProgram(L"TextureVertexShader.cso", L"TexturePixelShader.cso"))
{
    // Empty
}

Direct3DBackgroundTextureGpuProgramWrapper::~Direct3DBackgroundTextureGpuProgramWrapper()
{
    delete m_program;
}

void Direct3DBackgroundTextureGpuProgramWrapper::bind()
{
    D3DManager->useNormalBlending();
    
	m_program->bindShaders();
    
    m_program->bindWrapSamplerState();

    m_program->bindMatrix();

	m_program->mapVertices();
}

void Direct3DBackgroundTextureGpuProgramWrapper::unbind()
{
	// Empty
}
