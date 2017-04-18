//
//  Direct3DFramebufferRadialBlurGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DFramebufferRadialBlurGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "DeviceResources.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"

Direct3DFramebufferRadialBlurGpuProgramWrapper::Direct3DFramebufferRadialBlurGpuProgramWrapper() : FramebufferRadialBlurGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(L"FramebufferToScreenVertexShader.cso", L"RadialBlurTexturePixelShader.cso"))
{
	m_program->createConstantBuffer(&m_isWindowsMobileConstantBuffer);
	m_program->createConstantBuffer(&m_directionConstantBuffer);
    
    m_isWindowsMobile = D3DManager->isWindowsMobile();
}

Direct3DFramebufferRadialBlurGpuProgramWrapper::~Direct3DFramebufferRadialBlurGpuProgramWrapper()
{
    m_isWindowsMobileConstantBuffer.Reset();
	m_directionConstantBuffer.Reset();
    
    delete m_program;
}

void Direct3DFramebufferRadialBlurGpuProgramWrapper::bind()
{
    D3DManager->useScreenBlending();
    
	m_program->bindShaders();
    
    m_program->bindNormalSamplerState();

    DX::DeviceResources* deviceResources = Direct3DManager::getDeviceResources();
    
	deviceResources->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, m_isWindowsMobileConstantBuffer.GetAddressOf());
	deviceResources->GetD3DDeviceContext()->PSSetConstantBuffers(1, 1, m_directionConstantBuffer.GetAddressOf());

	// send isWindowsMobile to video memory
	int isWindowsMobile = m_isWindowsMobile ? 1 : 0;
	deviceResources->GetD3DDeviceContext()->UpdateSubresource(m_isWindowsMobileConstantBuffer.Get(), 0, 0, &isWindowsMobile, 0, 0);
	deviceResources->GetD3DDeviceContext()->UpdateSubresource(m_directionConstantBuffer.Get(), 0, 0, &m_fDirection, 0, 0);

	m_program->mapVertices();
}

void Direct3DFramebufferRadialBlurGpuProgramWrapper::unbind()
{
    // Empty
}
