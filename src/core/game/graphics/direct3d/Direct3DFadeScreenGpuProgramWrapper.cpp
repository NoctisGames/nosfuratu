//
//  Direct3DFadeScreenGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DFadeScreenGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

Direct3DFadeScreenGpuProgramWrapper::Direct3DFadeScreenGpuProgramWrapper() : TransitionGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(FRAMEBUFFER_TO_SCREEN_VERTEX_SHADER, FADE_SCREEN_FRAGMENT_SHADER))
{
	m_program->createConstantBuffer(&m_progressConstantBuffer);
	m_program->createConstantBuffer(&m_isWindowsMobileConstantBuffer);
    
    m_isWindowsMobile = D3DManager->isWindowsMobile();
}

Direct3DFadeScreenGpuProgramWrapper::~Direct3DFadeScreenGpuProgramWrapper()
{
    m_isWindowsMobileConstantBuffer.Reset();
    m_progressConstantBuffer.Reset();
    
    delete m_program;
}

void Direct3DFadeScreenGpuProgramWrapper::bind()
{
    D3DManager->useScreenBlending();
    
	m_program->bindShaders();
    
    m_program->bindNormalSamplerState();

    ID3D11DeviceContext* d3dContext = Direct3DManager::getD3dContext();
    
	// tell the GPU which texture to use
	d3dContext->PSSetShaderResources(1, 1, &m_to->texture);

	d3dContext->PSSetConstantBuffers(0, 1, m_progressConstantBuffer.GetAddressOf());
    d3dContext->PSSetConstantBuffers(1, 1, m_isWindowsMobileConstantBuffer.GetAddressOf());

	// send time elapsed to video memory
	d3dContext->UpdateSubresource(m_progressConstantBuffer.Get(), 0, 0, &m_fProgress, 0, 0);
    
    int isWindowsMobile = m_isWindowsMobile ? 1 : 0;
    d3dContext->UpdateSubresource(m_isWindowsMobileConstantBuffer.Get(), 0, 0, &isWindowsMobile, 0, 0);

	m_program->mapVertices();
}

void Direct3DFadeScreenGpuProgramWrapper::unbind()
{
	ID3D11DeviceContext* d3dContext = Direct3DManager::getD3dContext();

	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	d3dContext->PSSetShaderResources(1, 1, pSRV);
}
