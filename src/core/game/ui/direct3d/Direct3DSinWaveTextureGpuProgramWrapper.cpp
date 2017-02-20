//
//  Direct3DSinWaveTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/18/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DSinWaveTextureGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "DeviceResources.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"

Direct3DSinWaveTextureGpuProgramWrapper::Direct3DSinWaveTextureGpuProgramWrapper() : SinWaveTextureGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(L"TextureVertexShader.cso", L"SinWaveTexturePixelShader.cso"))
{
	m_program->createConstantBuffer(&m_offsetConstantBuffer);
}

Direct3DSinWaveTextureGpuProgramWrapper::~Direct3DSinWaveTextureGpuProgramWrapper()
{
    m_offsetConstantBuffer.Reset();
    
    delete m_program;
}

void Direct3DSinWaveTextureGpuProgramWrapper::bind()
{
    D3DManager->useNormalBlending();

	m_program->bindShaders();
    
    m_program->bindNormalSamplerState();

    DX::DeviceResources* deviceResources = Direct3DManager::getDeviceResources();
    
	deviceResources->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, m_offsetConstantBuffer.GetAddressOf());

    m_program->bindMatrix();

	// send the new offset to video memory
	deviceResources->GetD3DDeviceContext()->UpdateSubresource(m_offsetConstantBuffer.Get(), 0, 0, &m_fOffset, 0, 0);

	m_program->mapVertices();
}

void Direct3DSinWaveTextureGpuProgramWrapper::unbind()
{
	// Empty
}
