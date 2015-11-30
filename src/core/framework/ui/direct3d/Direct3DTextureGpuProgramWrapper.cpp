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

Direct3DTextureGpuProgramWrapper::Direct3DTextureGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources)
{
	// Empty
}

void Direct3DTextureGpuProgramWrapper::bind()
{
	m_deviceResources->GetD3DDeviceContext()->OMSetBlendState(D3DManager->m_blendState.Get(), 0, 0xffffffff);

	m_deviceResources->GetD3DDeviceContext()->IASetInputLayout(D3DManager->m_sbInputLayout.Get());

	// set the shader objects as the active shaders
	m_deviceResources->GetD3DDeviceContext()->VSSetShader(D3DManager->m_sbVertexShader.Get(), nullptr, 0);
	m_deviceResources->GetD3DDeviceContext()->PSSetShader(D3DManager->m_sbPixelShader.Get(), nullptr, 0);

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
	// Clear out shader resource, since we are going to be binding to it again for writing on the next frame
	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	m_deviceResources->GetD3DDeviceContext()->PSSetShaderResources(0, 1, pSRV);
}