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
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

Direct3DFramebufferRadialBlurGpuProgramWrapper::Direct3DFramebufferRadialBlurGpuProgramWrapper() : FramebufferRadialBlurGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(FRAMEBUFFER_TO_SCREEN_VERTEX_SHADER, RADIAL_BLUR_FRAGMENT_SHADER))
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

    ID3D11DeviceContext* d3dContext = Direct3DManager::getD3dContext();
    
	d3dContext->PSSetConstantBuffers(0, 1, m_isWindowsMobileConstantBuffer.GetAddressOf());
	d3dContext->PSSetConstantBuffers(1, 1, m_directionConstantBuffer.GetAddressOf());

	// send isWindowsMobile to video memory
	int isWindowsMobile = m_isWindowsMobile ? 1 : 0;
	d3dContext->UpdateSubresource(m_isWindowsMobileConstantBuffer.Get(), 0, 0, &isWindowsMobile, 0, 0);
	d3dContext->UpdateSubresource(m_directionConstantBuffer.Get(), 0, 0, &m_fDirection, 0, 0);

	m_program->mapVertices();
}

void Direct3DFramebufferRadialBlurGpuProgramWrapper::unbind()
{
    // Empty
}
