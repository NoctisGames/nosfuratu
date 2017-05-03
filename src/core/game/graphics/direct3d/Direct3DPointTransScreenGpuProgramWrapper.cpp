//
//  Direct3DPointTransScreenGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/31/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DPointTransScreenGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "Direct3DManager.h"
#include "GpuTextureWrapper.h"
#include "Vector2D.h"
#include "GameConstants.h"

Direct3DPointTransScreenGpuProgramWrapper::Direct3DPointTransScreenGpuProgramWrapper() : PointTransitionGpuProgramWrapper(),
m_program(new Direct3DTextureProgram(POINT_TRANS_SCREEN_VERTEX_SHADER, POINT_TRANS_SCREEN_FRAGMENT_SHADER))
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

    ID3D11DeviceContext* d3dContext = Direct3DManager::getD3dContext();
    
	// tell the GPU which texture to use
	d3dContext->PSSetShaderResources(1, 1, &m_to->texture);

	d3dContext->VSSetConstantBuffers(1, 1, m_centerXConstantBuffer.GetAddressOf());
	d3dContext->VSSetConstantBuffers(2, 1, m_centerYConstantBuffer.GetAddressOf());
	d3dContext->PSSetConstantBuffers(0, 1, m_progressConstantBuffer.GetAddressOf());

    m_program->bindMatrix();

	// send center and time elapsed to video memory
	float centerX = m_center->getX();
	float centerY = m_center->getY();
	d3dContext->UpdateSubresource(m_centerXConstantBuffer.Get(), 0, 0, &centerX, 0, 0);
	d3dContext->UpdateSubresource(m_centerYConstantBuffer.Get(), 0, 0, &centerY, 0, 0);
	d3dContext->UpdateSubresource(m_progressConstantBuffer.Get(), 0, 0, &m_fProgress, 0, 0);

	m_program->mapVertices();
}

void Direct3DPointTransScreenGpuProgramWrapper::unbind()
{
	ID3D11DeviceContext* d3dContext = Direct3DManager::getD3dContext();

	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	d3dContext->PSSetShaderResources(1, 1, pSRV);
}
