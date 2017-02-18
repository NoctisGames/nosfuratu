//
//  Direct3DTransDeathGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Direct3DTransDeathGpuProgramWrapper.h"

#include "Direct3DTextureProgram.h"
#include "DeviceResources.h"
#include "Direct3DManager.h"

Direct3DTransDeathGpuProgramWrapper::Direct3DTransDeathGpuProgramWrapper(bool isTransIn) : TransDeathGpuProgramWrapper(isTransIn),
m_program(new Direct3DTextureProgram(L"FramebufferToScreenVertexShader.cso", isTransIn ? L"TransDeathInTexturePixelShader.cso" : L"TransDeathOutTexturePixelShader.cso"))
{
    createConstantBuffers();
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

    DX::DeviceResources* deviceResources = Direct3DManager::getDeviceResources();
    
	// tell the GPU which texture to use
	deviceResources->GetD3DDeviceContext()->PSSetShaderResources(1, 1, &m_grayMap->texture);

	deviceResources->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, m_timeElapsedConstantBuffer.GetAddressOf());

	// send time elapsed to video memory
	deviceResources->GetD3DDeviceContext()->UpdateSubresource(m_timeElapsedConstantBuffer.Get(), 0, 0, &m_fTimeElapsed, 0, 0);

	m_program->mapVertices();
}

void Direct3DTransDeathGpuProgramWrapper::unbind()
{
	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	deviceResources->GetD3DDeviceContext()->PSSetShaderResources(1, 1, pSRV);
}

void Direct3DTransDeathGpuProgramWrapper::createConstantBuffers()
{
    D3D11_BUFFER_DESC bd = { 0 };
    
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = 16;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    
    deviceResources->GetD3DDevice()->CreateBuffer(&bd, nullptr, &m_timeElapsedConstantBuffer);
}
