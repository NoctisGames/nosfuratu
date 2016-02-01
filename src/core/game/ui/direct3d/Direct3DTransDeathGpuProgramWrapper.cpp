//
//  Direct3DTransDeathGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "Direct3DTransDeathGpuProgramWrapper.h"
#include "Direct3DManager.h"
#include "macros.h"

Direct3DTransDeathGpuProgramWrapper::Direct3DTransDeathGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources, bool isTransIn) : TransDeathGpuProgramWrapper(isTransIn), m_iNumShadersLoaded(0), m_deviceResources(deviceResources)
{
	createConstantBuffers();

	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"FramebufferToScreenVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(isTransIn ? L"TransDeathInTexturePixelShader.cso" : L"TransDeathOutTexturePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
				)
			);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
				)
			);

		m_iNumShadersLoaded++;
		m_isLoaded = m_iNumShadersLoaded == 2;
	});

	// After the pixel shader file is loaded, create the shader
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
				)
			);

		m_iNumShadersLoaded++;
		m_isLoaded = m_iNumShadersLoaded == 2;
	});
}

void Direct3DTransDeathGpuProgramWrapper::bind()
{
	m_deviceResources->GetD3DDeviceContext()->OMSetBlendState(D3DManager->m_screenBlendState.Get(), 0, 0xffffffff);

	m_deviceResources->GetD3DDeviceContext()->IASetInputLayout(m_inputLayout.Get());

	// set the shader objects as the active shaders
	m_deviceResources->GetD3DDeviceContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_deviceResources->GetD3DDeviceContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// tell the GPU which texture to use
	m_deviceResources->GetD3DDeviceContext()->PSSetShaderResources(1, 1, &m_grayMap->texture);

	m_deviceResources->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, m_timeElapsedConstantBuffer.GetAddressOf());

	// send time elapsed to video memory
	m_deviceResources->GetD3DDeviceContext()->UpdateSubresource(m_timeElapsedConstantBuffer.Get(), 0, 0, &m_fTimeElapsed, 0, 0);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//	Disable GPU access to the vertex buffer data.
	m_deviceResources->GetD3DDeviceContext()->Map(D3DManager->m_sbVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	int numTextureVertices = D3DManager->m_textureVertices.size();
	//	Update the vertex buffer here.
	memcpy(mappedResource.pData, &D3DManager->m_textureVertices[0], sizeof(TEXTURE_VERTEX) * numTextureVertices);

	//	Reenable GPU access to the vertex buffer data.
	m_deviceResources->GetD3DDeviceContext()->Unmap(D3DManager->m_sbVertexBuffer.Get(), 0);

	// Set the vertex and index buffer
	UINT stride = sizeof(TEXTURE_VERTEX);
	UINT offset = 0;
	m_deviceResources->GetD3DDeviceContext()->IASetVertexBuffers(0, 1, D3DManager->m_sbVertexBuffer.GetAddressOf(), &stride, &offset);
}

void Direct3DTransDeathGpuProgramWrapper::unbind()
{
	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	m_deviceResources->GetD3DDeviceContext()->PSSetShaderResources(1, 1, pSRV);
}

void Direct3DTransDeathGpuProgramWrapper::cleanUp()
{
	m_timeElapsedConstantBuffer.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
	m_inputLayout.Reset();
}

void Direct3DTransDeathGpuProgramWrapper::createConstantBuffers()
{
	{
		D3D11_BUFFER_DESC bd = { 0 };

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = 16;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		m_deviceResources->GetD3DDevice()->CreateBuffer(&bd, nullptr, &m_timeElapsedConstantBuffer);
	}
}