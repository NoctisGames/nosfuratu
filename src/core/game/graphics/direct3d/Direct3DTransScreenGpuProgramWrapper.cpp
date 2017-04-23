//
//  Direct3DTransScreenGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DTransScreenGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"

Direct3DTransScreenGpuProgramWrapper::Direct3DTransScreenGpuProgramWrapper() : TransitionGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(L"FramebufferToScreenVertexShader.cso", L"TransScreenTexturePixelShader.cso"))
{
	m_program->createConstantBuffer(&m_progressConstantBuffer);
	m_program->createConstantBuffer(&m_isWindowsMobileConstantBuffer);
    
    m_isWindowsMobile = D3DManager->isWindowsMobile();
}

Direct3DTransScreenGpuProgramWrapper::~Direct3DTransScreenGpuProgramWrapper()
{
    m_isWindowsMobileConstantBuffer.Reset();
    m_progressConstantBuffer.Reset();
    
    delete m_program;
}

void Direct3DTransScreenGpuProgramWrapper::bind()
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

void Direct3DTransScreenGpuProgramWrapper::unbind()
{
	ID3D11DeviceContext* d3dContext = Direct3DManager::getD3dContext();

	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	d3dContext->PSSetShaderResources(1, 1, pSRV);
}
