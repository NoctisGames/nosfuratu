//
//  Direct3DGeometryGpuProgramWrapper.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 8/30/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "Direct3DGeometryGpuProgramWrapper.h"
#include "Direct3DManager.h"

Direct3DGeometryGpuProgramWrapper::Direct3DGeometryGpuProgramWrapper()
{
	// Empty
}

void Direct3DGeometryGpuProgramWrapper::bind()
{
	D3DManager->m_d3dContext->OMSetBlendState(D3DManager->m_blendState, 0, 0xffffffff);

	D3DManager->m_d3dContext->IASetInputLayout(D3DManager->m_gbInputLayout);

	// set the shader objects as the active shaders
	D3DManager->m_d3dContext->VSSetShader(D3DManager->m_gbVertexShader, nullptr, 0);
	D3DManager->m_d3dContext->PSSetShader(D3DManager->m_gbPixelShader, nullptr, 0);

	D3DManager->m_d3dContext->VSSetConstantBuffers(0, 1, &D3DManager->m_matrixConstantbuffer);

	// send the final matrix to video memory
	D3DManager->m_d3dContext->UpdateSubresource(D3DManager->m_matrixConstantbuffer, 0, 0, &D3DManager->m_matFinal, 0, 0);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//	Disable GPU access to the vertex buffer data.
	D3DManager->m_d3dContext->Map(D3DManager->m_gbVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	//	Update the vertex buffer here.
	memcpy(mappedResource.pData, &D3DManager->m_colorVertices.front(), sizeof(COLOR_VERTEX)* D3DManager->m_colorVertices.size());

	//	Reenable GPU access to the vertex buffer data.
	D3DManager->m_d3dContext->Unmap(D3DManager->m_gbVertexBuffer, 0);

	// Set the vertex buffer
	UINT stride = sizeof(COLOR_VERTEX);
	UINT offset = 0;
	D3DManager->m_d3dContext->IASetVertexBuffers(0, 1, &D3DManager->m_gbVertexBuffer, &stride, &offset);
}

void Direct3DGeometryGpuProgramWrapper::unbind()
{

}