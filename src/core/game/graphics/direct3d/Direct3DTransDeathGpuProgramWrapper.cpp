//
//  Direct3DTransDeathGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DTransDeathGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

Direct3DTransDeathGpuProgramWrapper::Direct3DTransDeathGpuProgramWrapper(bool isTransIn) : TransDeathGpuProgramWrapper(isTransIn),
m_program(new Direct3DTextureProgram(FRAMEBUFFER_TO_SCREEN_VERTEX_SHADER, isTransIn ? TRANS_DEATH_IN_FRAGMENT_SHADER : TRANS_DEATH_OUT_FRAGMENT_SHADER))
{
	m_program->createConstantBuffer(&m_timeElapsedConstantBuffer);
}

Direct3DTransDeathGpuProgramWrapper::~Direct3DTransDeathGpuProgramWrapper()
{
    m_timeElapsedConstantBuffer.Reset();
    
    delete m_program;
}

void Direct3DTransDeathGpuProgramWrapper::bind()
{
    D3DManager->useScreenBlending();
    
	m_program->bindShaders();
    
    m_program->bindNormalSamplerState();

    ID3D11DeviceContext* d3dContext = Direct3DManager::getD3dContext();
    
	// tell the GPU which texture to use
	d3dContext->PSSetShaderResources(1, 1, &m_grayMap->texture);

	d3dContext->PSSetConstantBuffers(0, 1, m_timeElapsedConstantBuffer.GetAddressOf());

	// send time elapsed to video memory
	d3dContext->UpdateSubresource(m_timeElapsedConstantBuffer.Get(), 0, 0, &m_fTimeElapsed, 0, 0);

	m_program->mapVertices();
}

void Direct3DTransDeathGpuProgramWrapper::unbind()
{
	ID3D11DeviceContext* d3dContext = Direct3DManager::getD3dContext();

	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	d3dContext->PSSetShaderResources(1, 1, pSRV);
}
