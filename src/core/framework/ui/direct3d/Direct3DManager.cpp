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

void Direct3DManager::init(const std::shared_ptr<DX::DeviceResources>& deviceResources, int maxBatchSize)
{
	m_iMaxBatchSize = maxBatchSize;
	m_deviceResources = deviceResources;
}

void Direct3DManager::createDeviceDependentResources()
{
	createBlendState();
	createSamplerState();
	createInputLayoutForSpriteBatcher();
	createInputLayoutForGeometryBatcher();
	createIndexBuffer();
	createConstantBuffer();
	createOffsetBuffer();

	m_textureProgram = std::unique_ptr<Direct3DTextureGpuProgramWrapper>(new Direct3DTextureGpuProgramWrapper(m_deviceResources));
	m_colorProgram = std::unique_ptr<Direct3DGeometryGpuProgramWrapper>(new Direct3DGeometryGpuProgramWrapper(m_deviceResources));
	m_fbToScreenProgram = std::unique_ptr<Direct3DFrameBufferToScreenGpuProgramWrapper>(new Direct3DFrameBufferToScreenGpuProgramWrapper(m_deviceResources));
}

void Direct3DManager::createWindowSizeDependentResources()
{
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
	DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateRenderTargetView(m_offscreenRenderTarget.Get(), &renderTargetViewDesc, &m_offscreenRenderTargetView));

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateShaderResourceView(m_offscreenRenderTarget.Get(), &shaderResourceViewDesc, &m_offscreenShaderResourceView));
}

void Direct3DManager::releaseDeviceDependentResources()
{
	m_iNumShadersLoaded = 0;
	m_areSpriteBatcherResourcesCreated = false;
	m_areGeometryBatcherResourcesCreated = false;

	m_offscreenRenderTarget.Reset();
	m_offscreenRenderTargetView.Reset();
	m_offscreenShaderResourceView.Reset();
	m_blendState.Reset();
	m_screenBlendState.Reset();
	m_matrixConstantbuffer.Reset();
	m_offsetConstantBuffer.Reset();
	m_indexbuffer.Reset();
	m_sbSamplerState.Reset();
	m_sbVertexShader.Reset();
	m_sbPixelShader.Reset();
	m_sbSinWavePixelShader.Reset();
	m_sbInputLayout.Reset();
	m_sbVertexBuffer.Reset();
	m_textureVertices.clear();
	m_fbVertexShader.Reset();
	m_fbPixelShader.Reset();
	m_fbInputLayout.Reset();
	m_gbVertexShader.Reset();
	m_gbPixelShader.Reset();
	m_gbInputLayout.Reset();
	m_gbVertexBuffer.Reset();
	m_colorVertices.clear();
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
	return m_iNumShadersLoaded >= 7 && m_areSpriteBatcherResourcesCreated && m_areGeometryBatcherResourcesCreated;
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

void Direct3DManager::createInputLayoutForSpriteBatcher()
{
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
					&m_sbVertexShader
					)
				);
			m_iNumShadersLoaded++;

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
					&m_sbInputLayout
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
					&m_sbPixelShader
					)
				);
			m_iNumShadersLoaded++;
		});

		// Once both shaders are created, create SpriteBatcher resources
		auto createSpriteBatcherResources = (createPSTask && createVSTask).then([this]() {
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
		});

		createSpriteBatcherResources.then([this]() {
			m_areSpriteBatcherResourcesCreated = true;
		});
	}

	{
		// Load shaders asynchronously.
		auto loadPSTask = DX::ReadDataAsync(L"SinWaveTexturePixelShader.cso");

		// After the pixel shader file is loaded, create the shader
		auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreatePixelShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_sbSinWavePixelShader
					)
				);
			m_iNumShadersLoaded++;
		});
	}

	{
		// Load shaders asynchronously.
		auto loadVSTask = DX::ReadDataAsync(L"FrameBufferToScreenVertexShader.cso");
		auto loadPSTask = DX::ReadDataAsync(L"FrameBufferToScreenPixelShader.cso");

		// After the vertex shader file is loaded, create the shader and input layout.
		auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateVertexShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_fbVertexShader
					)
				);
			m_iNumShadersLoaded++;

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
					&m_fbInputLayout
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
					&m_fbPixelShader
					)
				);
			m_iNumShadersLoaded++;
		});
	}
}

void Direct3DManager::createInputLayoutForGeometryBatcher()
{
	{
		// Load shaders asynchronously.
		auto loadVSTask = DX::ReadDataAsync(L"ColorVertexShader.cso");
		auto loadPSTask = DX::ReadDataAsync(L"ColorPixelShader.cso");

		// After the vertex shader file is loaded, create the shader and input layout.
		auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateVertexShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_gbVertexShader
					)
				);
			m_iNumShadersLoaded++;

			static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateInputLayout(
					vertexDesc,
					ARRAYSIZE(vertexDesc),
					&fileData[0],
					fileData.size(),
					&m_gbInputLayout
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
					&m_gbPixelShader
					)
				);
			m_iNumShadersLoaded++;
		});

		// Once both shaders are created, create GeometryBatcher resources
		auto createGeometryBatcherResources = (createPSTask && createVSTask).then([this]() {
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
		});

		createGeometryBatcherResources.then([this]() {
			m_areGeometryBatcherResourcesCreated = true;
		});
	}
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

void Direct3DManager::createOffsetBuffer()
{
	D3D11_BUFFER_DESC bd = { 0 };

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 16;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	m_deviceResources->GetD3DDevice()->CreateBuffer(&bd, nullptr, &m_offsetConstantBuffer);
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

Direct3DManager::Direct3DManager() : m_iNumShadersLoaded(0), m_areSpriteBatcherResourcesCreated(false), m_areGeometryBatcherResourcesCreated(false)
{
	// Hide Constructor for Singleton
}