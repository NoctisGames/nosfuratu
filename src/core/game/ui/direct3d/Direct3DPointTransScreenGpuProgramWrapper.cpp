//
//  Direct3DPointTransScreenGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/31/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DPointTransScreenGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "DeviceResources.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"
#include "Vector2D.h"

Direct3DPointTransScreenGpuProgramWrapper::Direct3DPointTransScreenGpuProgramWrapper() : PointTransitionGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(L"PointTransScreenTextureVertexShader.cso", L"PointTransScreenTexturePixelShader.cso"))
{
	m_program->createConstantBuffer(&m_centerXConstantBuffer);
	m_program->createConstantBuffer(&m_centerYConstantBuffer);
	m_program->createConstantBuffer(&m_progressConstantBuffer);
}

Direct3DPointTransScreenGpuProgramWrapper::~Direct3DPointTransScreenGpuProgramWrapper()
{
    m_centerXConstantBuffer.Reset();
    m_centerYConstantBuffer.Reset();
    m_progressConstantBuffer.Reset();
    
    delete m_program;
}

void Direct3DPointTransScreenGpuProgramWrapper::bind()
{
    D3DManager->useScreenBlending();
    
	m_program->bindShaders();
    
    m_program->bindNormalSamplerState();

    DX::DeviceResources* deviceResources = Direct3DManager::getDeviceResources();
    
	// tell the GPU which texture to use
	deviceResources->GetD3DDeviceContext()->PSSetShaderResources(1, 1, &m_to->texture);

	deviceResources->GetD3DDeviceContext()->VSSetConstantBuffers(1, 1, m_centerXConstantBuffer.GetAddressOf());
	deviceResources->GetD3DDeviceContext()->VSSetConstantBuffers(2, 1, m_centerYConstantBuffer.GetAddressOf());
	deviceResources->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, m_progressConstantBuffer.GetAddressOf());

    m_program->bindMatrix();

	// send center and time elapsed to video memory
	float centerX = m_center->getX();
	float centerY = m_center->getY();
	deviceResources->GetD3DDeviceContext()->UpdateSubresource(m_centerXConstantBuffer.Get(), 0, 0, &centerX, 0, 0);
	deviceResources->GetD3DDeviceContext()->UpdateSubresource(m_centerYConstantBuffer.Get(), 0, 0, &centerY, 0, 0);
	deviceResources->GetD3DDeviceContext()->UpdateSubresource(m_progressConstantBuffer.Get(), 0, 0, &m_fProgress, 0, 0);

	m_program->mapVertices();
}

void Direct3DPointTransScreenGpuProgramWrapper::unbind()
{
	DX::DeviceResources* deviceResources = Direct3DManager::getDeviceResources();

	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	deviceResources->GetD3DDeviceContext()->PSSetShaderResources(1, 1, pSRV);
}
