//
//  Direct3DRenderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "pch.h"
#include "macros.h"
#include "Direct3DRenderer.h"
#include "PhysicalEntity.h"
#include "TextureRegion.h"
#include "Assets.h"
#include "Vector2D.h"
#include "Font.h"
#include "GpuTextureWrapper.h"
#include "Line.h"
#include "Rectangle.h"
#include "SpriteBatcher.h"
#include "Direct3DRectangleBatcher.h"
#include "Direct3DSpriteBatcher.h"
#include "Direct3DLineBatcher.h"
#include "Direct3DCircleBatcher.h"
#include "Direct3DLineBatcher.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Direct3DManager.h"
#include "GameConstants.h"
#include "DirectXTex.h"
#include "DirectXHelper.h"
#include "Direct3DTransScreenGpuProgramWrapper.h"
#include "Direct3DSinWaveTextureGpuProgramWrapper.h"
#include "Direct3DSnakeDeathTextureGpuProgramWrapper.h"
#include "Direct3DShockwaveTextureGpuProgramWrapper.h"
#include "Direct3DFramebufferTintGpuProgramWrapper.h"
#include "Direct3DTransDeathGpuProgramWrapper.h"
#include "Direct3DFramebufferRadialBlurGpuProgramWrapper.h"
#include "Direct3DBackgroundTextureGpuProgramWrapper.h"

#include <string>
#include <sstream>

using namespace DirectX;

std::vector<ID3D11ShaderResourceView *> g_shaderResourceViews;

Direct3DRenderer::Direct3DRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) : Renderer(), m_deviceResources(deviceResources), m_iNumTexturesLoaded(0)
{
	m_spriteBatcher = std::unique_ptr<Direct3DSpriteBatcher>(new Direct3DSpriteBatcher(m_deviceResources));
	m_boundsRectangleBatcher = std::unique_ptr<Direct3DRectangleBatcher>(new Direct3DRectangleBatcher(m_deviceResources));
	m_highlightRectangleBatcher = std::unique_ptr<Direct3DRectangleBatcher>(new Direct3DRectangleBatcher(m_deviceResources, true));
	m_lineBatcher = std::unique_ptr<Direct3DLineBatcher>(new Direct3DLineBatcher(m_deviceResources));
}

void Direct3DRenderer::beginFrame(float deltaTime)
{
    m_framebuffers.clear();
	addFramebuffers();
    
    Renderer::beginFrame(deltaTime);
}

void Direct3DRenderer::endFrame()
{
    // Empty
}

void Direct3DRenderer::loadShaderPrograms()
{
	m_transScreenGpuProgramWrapper = new Direct3DTransScreenGpuProgramWrapper(m_deviceResources);
	m_sinWaveTextureProgram = new Direct3DSinWaveTextureGpuProgramWrapper(m_deviceResources);
	m_backgroundGpuTextureProgramWrapper = new Direct3DBackgroundTextureGpuProgramWrapper(m_deviceResources);
	m_snakeDeathTextureProgram = new Direct3DSnakeDeathTextureGpuProgramWrapper(m_deviceResources);
	m_shockwaveTextureGpuProgramWrapper = new Direct3DShockwaveTextureGpuProgramWrapper(m_deviceResources);
	m_framebufferToScreenGpuProgramWrapper = D3DManager->m_fbToScreenProgram.get();
	m_framebufferTintGpuProgramWrapper = new Direct3DFramebufferTintGpuProgramWrapper(m_deviceResources);
	m_framebufferRadialBlurGpuProgramWrapper = new Direct3DFramebufferRadialBlurGpuProgramWrapper(m_deviceResources);
	m_transDeathInGpuProgramWrapper = new Direct3DTransDeathGpuProgramWrapper(m_deviceResources, true);
	m_transDeathOutGpuProgramWrapper = new Direct3DTransDeathGpuProgramWrapper(m_deviceResources, false);
}

bool Direct3DRenderer::areShadersLoaded()
{
    return D3DManager->isLoaded() &&
    m_transScreenGpuProgramWrapper->isLoaded() &&
    m_sinWaveTextureProgram->isLoaded() &&
    m_snakeDeathTextureProgram->isLoaded() &&
    m_shockwaveTextureGpuProgramWrapper->isLoaded() &&
    m_framebufferToScreenGpuProgramWrapper->isLoaded() &&
    m_framebufferTintGpuProgramWrapper->isLoaded() &&
    m_framebufferRadialBlurGpuProgramWrapper->isLoaded() &&
    m_transDeathInGpuProgramWrapper->isLoaded() &&
    m_transDeathOutGpuProgramWrapper->isLoaded();
}

void Direct3DRenderer::addFramebuffers()
{
	for (std::vector<ID3D11ShaderResourceView*>::iterator i = D3DManager->m_offscreenShaderResourceViews.begin(); i != D3DManager->m_offscreenShaderResourceViews.end(); i++)
	{
		m_framebuffers.push_back(GpuTextureWrapper((*i)));
	}
}

GpuTextureDataWrapper* Direct3DRenderer::loadTextureData(const char* textureName)
{
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
    
    TexMetadata info = TexMetadata();
    DX::ThrowIfFailed(GetMetadataFromDDSFile(wString, DDS_FLAGS_NONE, info));
    
    ScratchImage image;
    DX::ThrowIfFailed(LoadFromDDSFile(wString, DDS_FLAGS_NONE, &info, image));
    
	GpuTextureDataWrapper* tdw = new GpuTextureDataWrapper(image, info, m_iNumTexturesLoaded);

	m_iNumTexturesLoaded++;
	
	delete wString;
	delete textureFileName;

	return tdw;
}

GpuTextureWrapper* Direct3DRenderer::loadTexture(GpuTextureDataWrapper* textureData, int repeatS)
{
    UNUSED(repeatS);
    
    DX::ThrowIfFailed(CreateShaderResourceView(m_deviceResources->GetD3DDevice(), textureData->image.GetImages(), textureData->image.GetImageCount(), textureData->info, &g_shaderResourceViews[textureData->shaderResourceViewIndex]));
}

void Direct3DRenderer::updateMatrix(float left, float right, float bottom, float top)
{
	D3DManager->createMatrix(left, right, bottom, top);
}

void Direct3DRenderer::bindToOffscreenFramebuffer(int index)
{
	m_deviceResources->GetD3DDeviceContext()->OMSetRenderTargets(1, &D3DManager->m_offscreenRenderTargetViews.at(index), nullptr);
}

void Direct3DRenderer::clearFramebufferWithColor(float r, float g, float b, float a)
{
	float color[] = { r, g, b, a };

	m_deviceResources->GetD3DDeviceContext()->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), color);
	m_deviceResources->GetD3DDeviceContext()->ClearRenderTargetView(D3DManager->m_offscreenRenderTargetViews.at(m_iFramebufferIndex), color);
}

void Direct3DRenderer::bindToScreenFramebuffer()
{
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	m_deviceResources->GetD3DDeviceContext()->OMSetRenderTargets(1, targets, nullptr);
}

void Direct3DRenderer::destroyTexture(GpuTextureWrapper& textureWrapper)
{
	textureWrapper.texture->Release();
}

#pragma mark private

void Direct3DRenderer::loadTexture(LPCWSTR szFile, ID3D11ShaderResourceView **shaderResourceView)
{
	
	
}