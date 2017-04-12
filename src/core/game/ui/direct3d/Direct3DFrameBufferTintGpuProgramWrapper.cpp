//
//  Direct3DFramebufferTintGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DFramebufferTintGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "DeviceResources.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"

Direct3DFramebufferTintGpuProgramWrapper::Direct3DFramebufferTintGpuProgramWrapper() : GpuProgramWrapper(),
m_program(new Direct3DTextureProgram(L"FramebufferToScreenVertexShader.cso", L"FramebufferTintToScreenPixelShader.cso"))
{
    // Empty
}

Direct3DFramebufferTintGpuProgramWrapper::~Direct3DFramebufferTintGpuProgramWrapper()
{
    delete m_program;
}

void Direct3DFramebufferTintGpuProgramWrapper::bind()
{
    D3DManager->useScreenBlending();
    
    m_program->bindShaders();
    
    m_program->bindNormalSamplerState();

	m_program->mapVertices();
}

void Direct3DFramebufferTintGpuProgramWrapper::unbind()
{
	// Empty
}
