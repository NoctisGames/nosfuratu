//
//  Direct3DGpuProgramWrapper.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 8/30/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "Direct3DTextureGpuProgramWrapper.h"
#include "Direct3DManager.h"

Direct3DTextureGpuProgramWrapper::Direct3DTextureGpuProgramWrapper()
{
	m_matFinal = D3DManager->m_matFinal;
}

void Direct3DTextureGpuProgramWrapper::bind()
{
	D3DManager->m_d3dContext->IASetInputLayout(D3DManager->m_sbInputLayout);

	// set the shader objects as the active shaders
	D3DManager->m_d3dContext->VSSetShader(D3DManager->m_sbVertexShader, nullptr, 0);
	D3DManager->m_d3dContext->PSSetShader(D3DManager->m_sbPixelShader, nullptr, 0);

	D3DManager->m_d3dContext->VSSetConstantBuffers(0, 1, &D3DManager->m_matrixConstantbuffer);

	// send the final matrix to video memory
	D3DManager->m_d3dContext->UpdateSubresource(D3DManager->m_matrixConstantbuffer, 0, 0, &m_matFinal, 0, 0);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//	Disable GPU access to the vertex buffer data.
	D3DManager->m_d3dContext->Map(D3DManager->m_sbVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	int numTextureVertices = D3DManager->m_textureVertices.size();
	//	Update the vertex buffer here.
	memcpy(mappedResource.pData, &D3DManager->m_textureVertices[0], sizeof(TEXTURE_VERTEX)* numTextureVertices);

	//	Reenable GPU access to the vertex buffer data.
	D3DManager->m_d3dContext->Unmap(D3DManager->m_sbVertexBuffer, 0);

	// Set the vertex and index buffer
	UINT stride = sizeof(TEXTURE_VERTEX);
	UINT offset = 0;
	D3DManager->m_d3dContext->IASetVertexBuffers(0, 1, &D3DManager->m_sbVertexBuffer, &stride, &offset);
}

void Direct3DTextureGpuProgramWrapper::unbind()
{
	// Clear out shader resource, since we are going to be binding to it again for writing on the next frame
	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	D3DManager->m_d3dContext->PSSetShaderResources(0, 1, pSRV);
}