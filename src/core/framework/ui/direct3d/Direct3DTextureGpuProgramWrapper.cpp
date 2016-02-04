//
//  Direct3DTextureGpuProgramWrapper.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 8/30/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "Direct3DTextureGpuProgramWrapper.h"
#include "Direct3DManager.h"

Direct3DTextureGpuProgramWrapper::Direct3DTextureGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_iNumShadersLoaded(0), m_deviceResources(deviceResources)
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"TextureVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"TexturePixelShader.cso");

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
		m_iNumShadersLoaded++;
		m_isLoaded = m_iNumShadersLoaded == 2;

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

void Direct3DTextureGpuProgramWrapper::bind()
{
	m_deviceResources->GetD3DDeviceContext()->OMSetBlendState(D3DManager->m_blendState.Get(), 0, 0xffffffff);

	m_deviceResources->GetD3DDeviceContext()->IASetInputLayout(m_inputLayout.Get());

	// set the shader objects as the active shaders
	m_deviceResources->GetD3DDeviceContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_deviceResources->GetD3DDeviceContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	m_deviceResources->GetD3DDeviceContext()->PSSetSamplers(0, 1, D3DManager->m_sbSamplerState.GetAddressOf());

	m_deviceResources->GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, D3DManager->m_matrixConstantbuffer.GetAddressOf());

	// send the final matrix to video memory
	m_deviceResources->GetD3DDeviceContext()->UpdateSubresource(D3DManager->m_matrixConstantbuffer.Get(), 0, 0, &D3DManager->m_matFinal, 0, 0);

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

void Direct3DTextureGpuProgramWrapper::unbind()
{
	// Empty
}

void Direct3DTextureGpuProgramWrapper::cleanUp()
{
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
}