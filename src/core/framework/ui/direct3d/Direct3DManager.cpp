//
//  Direct3DManager.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 11/17/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "Direct3DManager.h"
#include "Direct3DTextureGpuProgramWrapper.h"
#include "Direct3DGeometryGpuProgramWrapper.h"
#include "Direct3DFrameBufferToScreenGpuProgramWrapper.h"
#include "DirectXHelper.h"

using namespace DirectX;

Direct3DManager * Direct3DManager::getInstance()
{
	static Direct3DManager *instance = new Direct3DManager();

	return instance;
}

void Direct3DManager::init(const std::shared_ptr<DX::DeviceResources>& deviceResources, int maxBatchSize, int numFramebuffers)
{
	m_deviceResources = deviceResources;
	m_iMaxBatchSize = maxBatchSize;
	m_iNumFramebuffers = numFramebuffers;
}

void Direct3DManager::createDeviceDependentResources()
{
	createBlendState();
	createSamplerState();
	createVertexBufferForSpriteBatcher();
	createVertexBufferForGeometryBatchers();
	createIndexBuffer();
	createConstantBuffer();

	m_textureProgram = std::unique_ptr<Direct3DTextureGpuProgramWrapper>(new Direct3DTextureGpuProgramWrapper(m_deviceResources));
	m_colorProgram = std::unique_ptr<Direct3DGeometryGpuProgramWrapper>(new Direct3DGeometryGpuProgramWrapper(m_deviceResources));
	m_fbToScreenProgram = std::unique_ptr<Direct3DFrameBufferToScreenGpuProgramWrapper>(new Direct3DFrameBufferToScreenGpuProgramWrapper(m_deviceResources));
}

void Direct3DManager::createWindowSizeDependentResources()
{
	m_offscreenRenderTargets.clear();
	m_offscreenRenderTargetViews.clear();
	m_offscreenShaderResourceViews.clear();

	for (int i = 0; i < m_iNumFramebuffers; i++)
	{
		ID3D11Texture2D* m_offscreenRenderTarget;
		ID3D11RenderTargetView* m_offscreenRenderTargetView;
		ID3D11ShaderResourceView* m_offscreenShaderResourceView;

		Windows::Foundation::Size renderTargetSize = m_deviceResources->GetRenderTargetSize();

		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = renderTargetSize.Width;
		textureDesc.Height = renderTargetSize.Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateTexture2D(&textureDesc, NULL, &m_offscreenRenderTarget));

		// Setup the description of the render target view.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateRenderTargetView(m_offscreenRenderTarget, &renderTargetViewDesc, &m_offscreenRenderTargetView));

		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateShaderResourceView(m_offscreenRenderTarget, &shaderResourceViewDesc, &m_offscreenShaderResourceView));

		m_offscreenRenderTargets.push_back(m_offscreenRenderTarget);
		m_offscreenRenderTargetViews.push_back(m_offscreenRenderTargetView);
		m_offscreenShaderResourceViews.push_back(m_offscreenShaderResourceView);
	}
}

void Direct3DManager::releaseDeviceDependentResources()
{
	for (std::vector<ID3D11Texture2D*>::iterator i = m_offscreenRenderTargets.begin(); i != m_offscreenRenderTargets.end(); )
	{
		(*i)->Release();
	}

	for (std::vector<ID3D11RenderTargetView*>::iterator i = m_offscreenRenderTargetViews.begin(); i != m_offscreenRenderTargetViews.end(); )
	{
		(*i)->Release();
	}

	for (std::vector<ID3D11ShaderResourceView*>::iterator i = m_offscreenShaderResourceViews.begin(); i != m_offscreenShaderResourceViews.end(); )
	{
		(*i)->Release();
	}

	m_blendState.Reset();
	m_screenBlendState.Reset();
	m_matrixConstantbuffer.Reset();
	m_indexbuffer.Reset();
	m_sbSamplerState.Reset();
	m_sbVertexBuffer.Reset();
	m_textureVertices.clear();
	m_gbVertexBuffer.Reset();
	m_colorVertices.clear();

	m_textureProgram->cleanUp();
	m_colorProgram->cleanUp();
	m_fbToScreenProgram->cleanUp();
}

void Direct3DManager::createMatrix(float left, float right, float bottom, float top)
{
	using namespace DirectX;

	XMVECTOR eye = XMVectorSet(0, 0, 1, 0);
	XMVECTOR center = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	XMMATRIX matProjection = XMMatrixOrthographicOffCenterRH(left, right, bottom, top, -1.0, 1.0);
	XMMATRIX matView = XMMatrixLookAtRH(eye, center, up);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	m_matFinal = matView * matProjection * orientationMatrix;
}

void Direct3DManager::addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v)
{
	TEXTURE_VERTEX tv = { x, y, z, r, g, b, a, u, v };
	D3DManager->m_textureVertices.push_back(tv);
}

void Direct3DManager::addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a)
{
	COLOR_VERTEX cv = { x, y, z, r, g, b, a };
	D3DManager->m_colorVertices.push_back(cv);
}

bool Direct3DManager::isLoaded()
{
	return m_textureProgram->isLoaded()
		&& m_colorProgram->isLoaded()
		&& m_fbToScreenProgram->isLoaded();
}

void Direct3DManager::createBlendState()
{
	{
		D3D11_BLEND_DESC bd;
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		bd.IndependentBlendEnable = FALSE;
		bd.AlphaToCoverageEnable = FALSE;

		m_deviceResources->GetD3DDevice()->CreateBlendState(&bd, &m_blendState);
	}

	{
		D3D11_BLEND_DESC bd;
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		bd.IndependentBlendEnable = FALSE;
		bd.AlphaToCoverageEnable = FALSE;

		m_deviceResources->GetD3DDevice()->CreateBlendState(&bd, &m_screenBlendState);
	}
}

void Direct3DManager::createSamplerState()
{
	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = 16;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.BorderColor[0] = 0.0f;
	sd.BorderColor[1] = 0.0f;
	sd.BorderColor[2] = 0.0f;
	sd.BorderColor[3] = 0.0f;
	sd.MinLOD = 0.0f;
	sd.MaxLOD = FLT_MAX;
	sd.MipLODBias = 0.0f;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // linear filtering
	sd.MinLOD = 5.0f; // mip level 5 will appear blurred

	m_deviceResources->GetD3DDevice()->CreateSamplerState(&sd, m_sbSamplerState.GetAddressOf());
	m_deviceResources->GetD3DDeviceContext()->PSSetSamplers(0, 1, m_sbSamplerState.GetAddressOf());
}

void Direct3DManager::createVertexBufferForSpriteBatcher()
{
	m_textureVertices.reserve(m_iMaxBatchSize * VERTICES_PER_RECTANGLE);
	TEXTURE_VERTEX tv = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < m_iMaxBatchSize * VERTICES_PER_RECTANGLE; i++)
	{
		m_textureVertices.push_back(tv);
	}

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(TEXTURE_VERTEX) * m_textureVertices.size();
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	vertexBufferData.pSysMem = &m_textureVertices[0];
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_sbVertexBuffer));
}

void Direct3DManager::createVertexBufferForGeometryBatchers()
{
	m_colorVertices.reserve(m_iMaxBatchSize * VERTICES_PER_RECTANGLE);
	COLOR_VERTEX cv = { 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < m_iMaxBatchSize * VERTICES_PER_RECTANGLE; i++)
	{
		m_colorVertices.push_back(cv);
	}

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(COLOR_VERTEX) * m_colorVertices.size();
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	vertexBufferData.pSysMem = &m_colorVertices[0];
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_gbVertexBuffer));
}

void Direct3DManager::createIndexBuffer()
{
	D3D11_BUFFER_DESC indexBufferDesc = { 0 };

	indexBufferDesc.ByteWidth = sizeof(short) * m_iMaxBatchSize * INDICES_PER_RECTANGLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	auto indexValues = createIndexValues(m_iMaxBatchSize);

	D3D11_SUBRESOURCE_DATA indexDataDesc = { 0 };

	indexDataDesc.pSysMem = &indexValues.front();

	m_deviceResources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &indexDataDesc, &m_indexbuffer);

	m_deviceResources->GetD3DDeviceContext()->IASetIndexBuffer(m_indexbuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

void Direct3DManager::createConstantBuffer()
{
	D3D11_BUFFER_DESC bd = { 0 };

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 64;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&bd, nullptr, &m_matrixConstantbuffer));
}

std::vector<short> Direct3DManager::createIndexValues(int maxBatchSize)
{
	std::vector<short> indices;

	indices.reserve(maxBatchSize * INDICES_PER_RECTANGLE);

	short j = 0;
	for (int i = 0; i < maxBatchSize * INDICES_PER_RECTANGLE; i += INDICES_PER_RECTANGLE, j += VERTICES_PER_RECTANGLE)
	{
		indices.push_back(j);
		indices.push_back(j + 1);
		indices.push_back(j + 2);
		indices.push_back(j + 2);
		indices.push_back(j + 3);
		indices.push_back(j + 0);
	}

	return indices;
}

Direct3DManager::Direct3DManager()
{
	// Hide Constructor for Singleton
}