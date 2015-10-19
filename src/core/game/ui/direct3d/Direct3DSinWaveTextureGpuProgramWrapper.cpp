//
//  Direct3DSinWaveTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/18/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "Direct3DSinWaveTextureGpuProgramWrapper.h"
#include "Direct3DManager.h"

Direct3DSinWaveTextureGpuProgramWrapper::Direct3DSinWaveTextureGpuProgramWrapper()
{
	// Empty
}

void Direct3DSinWaveTextureGpuProgramWrapper::bind()
{
	D3DManager->m_d3dContext->OMSetBlendState(D3DManager->m_blendState, 0, 0xffffffff);

	D3DManager->m_d3dContext->IASetInputLayout(D3DManager->m_sbInputLayout);

	// set the shader objects as the active shaders
	D3DManager->m_d3dContext->VSSetShader(D3DManager->m_sbVertexShader, nullptr, 0);
	D3DManager->m_d3dContext->PSSetShader(D3DManager->m_sbSinWavePixelShader, nullptr, 0);

	D3DManager->m_d3dContext->VSSetConstantBuffers(0, 1, &D3DManager->m_matrixConstantbuffer);
	D3DManager->m_d3dContext->PSSetConstantBuffers(0, 1, &D3DManager->m_offsetConstantBuffer);

	// send the final matrix to video memory
	D3DManager->m_d3dContext->UpdateSubresource(D3DManager->m_matrixConstantbuffer, 0, 0, &D3DManager->m_matFinal, 0, 0);

	// send the new offset to video memory
	D3DManager->m_d3dContext->UpdateSubresource(D3DManager->m_offsetConstantBuffer, 0, 0, &m_fOffset, 0, 0);

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

void Direct3DSinWaveTextureGpuProgramWrapper::unbind()
{
	// Clear out shader resource, since we are going to be binding to it again for writing on the next frame
	ID3D11ShaderResourceView *pSRV[1] = { NULL };
	D3DManager->m_d3dContext->PSSetShaderResources(0, 1, pSRV);
}