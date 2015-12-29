//
//  Direct3DRenderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "macros.h"
#include "Direct3DRenderer.h"
#include "PhysicalEntity.h"
#include "TextureRegion.h"
#include "Assets.h"
#include "Vector2D.h"
#include "Font.h"
#include "TextureWrapper.h"
#include "Line.h"
#include "Rectangle.h"
#include "SpriteBatcher.h"
#include "Direct3DRectangleBatcher.h"
#include "Direct3DSpriteBatcher.h"
#include "Direct3DLineBatcher.h"
#include "Direct3DCircleBatcher.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Direct3DManager.h"
#include "GameConstants.h"
#include "DirectXTex.h"
#include "DirectXHelper.h"
#include "Direct3DTextureGpuProgramWrapper.h"
#include "Direct3DFrameBufferToScreenGpuProgramWrapper.h"
#include "Direct3DSinWaveTextureGpuProgramWrapper.h"
#include "Direct3DSnakeDeathTextureGpuProgramWrapper.h"

#include <string>
#include <sstream>

using namespace DirectX;

std::vector<ID3D11ShaderResourceView *> g_shaderResourceViews;

Direct3DRenderer::Direct3DRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) : Renderer(), m_deviceResources(deviceResources), m_iNumTexturesLoaded(0)
{
	m_spriteBatcher = std::unique_ptr<Direct3DSpriteBatcher>(new Direct3DSpriteBatcher(m_deviceResources));
	m_boundsRectangleBatcher = std::unique_ptr<Direct3DRectangleBatcher>(new Direct3DRectangleBatcher(m_deviceResources));
	m_highlightRectangleBatcher = std::unique_ptr<Direct3DRectangleBatcher>(new Direct3DRectangleBatcher(m_deviceResources, true));
}

void Direct3DRenderer::init(RendererType type)
{
	Renderer::init(type);

	if (!m_sinWaveTextureProgram)
	{
		m_sinWaveTextureProgram = std::unique_ptr<Direct3DSinWaveTextureGpuProgramWrapper>(new Direct3DSinWaveTextureGpuProgramWrapper(m_deviceResources));
	}
	
	if (!m_snakeDeathTextureProgram)
	{
		m_snakeDeathTextureProgram = std::unique_ptr<Direct3DSnakeDeathTextureGpuProgramWrapper>(new Direct3DSnakeDeathTextureGpuProgramWrapper(m_deviceResources));
	}
}

bool Direct3DRenderer::isLoaded()
{
	return D3DManager->isLoaded() && m_sinWaveTextureProgram->isLoaded() && m_snakeDeathTextureProgram->isLoaded();
}

TextureWrapper* Direct3DRenderer::loadTexture(const char* textureName, int repeatS)
{
	UNUSED(repeatS);

	size_t len = strlen(textureName);

	char* textureFileName = new char[8 + len + 5];

	strcpy(textureFileName, textureName);
	textureFileName[len] = '.';
	textureFileName[len + 1] = 'd';
	textureFileName[len + 2] = 'd';
	textureFileName[len + 3] = 's';
	textureFileName[len + 4] = '\0';

	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, textureFileName, -1, wString, 4096);

	ID3D11ShaderResourceView *pShaderResourceView;
	g_shaderResourceViews.push_back(pShaderResourceView);
	loadTexture(wString, &g_shaderResourceViews[m_iNumTexturesLoaded]);
	TextureWrapper* tw = new TextureWrapper(g_shaderResourceViews[m_iNumTexturesLoaded]);

	m_iNumTexturesLoaded++;
	
	delete wString;
	delete textureFileName;

	return tw;
}

void Direct3DRenderer::updateMatrix(float left, float right, float bottom, float top)
{
	D3DManager->createMatrix(left, right, bottom, top);
}

void Direct3DRenderer::bindToOffscreenFramebuffer()
{
	m_deviceResources->GetD3DDeviceContext()->OMSetRenderTargets(1, D3DManager->m_offscreenRenderTargetView.GetAddressOf(), nullptr);
}

void Direct3DRenderer::clearFrameBufferWithColor(float r, float g, float b, float a)
{
	float color[] = { r, g, b, a };

	m_deviceResources->GetD3DDeviceContext()->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), color);
	m_deviceResources->GetD3DDeviceContext()->ClearRenderTargetView(D3DManager->m_offscreenRenderTargetView.Get(), color);
}

void Direct3DRenderer::bindToScreenFramebuffer()
{
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	m_deviceResources->GetD3DDeviceContext()->OMSetRenderTargets(1, targets, nullptr);
}

void Direct3DRenderer::beginFrame()
{
	m_deviceResources->GetD3DDeviceContext()->OMSetRenderTargets(1, D3DManager->m_offscreenRenderTargetView.GetAddressOf(), nullptr);

	m_framebuffer = std::unique_ptr<TextureWrapper>(new TextureWrapper(D3DManager->m_offscreenShaderResourceView.Get()));
}

void Direct3DRenderer::endFrame()
{
	// Empty
}

GpuProgramWrapper& Direct3DRenderer::getFramebufferToScreenGpuProgramWrapper()
{
	return *D3DManager->m_fbToScreenProgram;
}

void Direct3DRenderer::destroyTexture(TextureWrapper& textureWrapper)
{
	textureWrapper.texture->Release();
}

#pragma mark private

void Direct3DRenderer::loadTexture(LPCWSTR szFile, ID3D11ShaderResourceView **shaderResourceView)
{
	TexMetadata info = TexMetadata();
	DX::ThrowIfFailed(GetMetadataFromDDSFile(szFile, DDS_FLAGS_NONE, info));

	ScratchImage image;
	DX::ThrowIfFailed(LoadFromDDSFile(szFile, DDS_FLAGS_NONE, &info, image));
	DX::ThrowIfFailed(CreateShaderResourceView(m_deviceResources->GetD3DDevice(), image.GetImages(), image.GetImageCount(), info, shaderResourceView));
}