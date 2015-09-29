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

#include <string>
#include <sstream>

using namespace DirectX;

std::vector<ID3D11ShaderResourceView *> g_shaderResourceViews;

/* Prepends t into s. Assumes s has enough space allocated
** for the combined string.
*/
void prepend(char* s, const char* t)
{
	size_t len = strlen(t);
	size_t i;

	memmove(s + len, s, strlen(s) + 1);

	for (i = 0; i < len; ++i)
	{
		s[i] = t[i];
	}
}

Direct3DRenderer::Direct3DRenderer() : Renderer(), m_iNumTexturesLoaded(0)
{
	m_spriteBatcher = std::unique_ptr<Direct3DSpriteBatcher>(new Direct3DSpriteBatcher());
}

TextureWrapper* Direct3DRenderer::loadTexture(const char* textureName)
{
	size_t len = strlen(textureName);

	char* textureFileName = new char[8 + len + 5];

	strcpy(textureFileName, textureName);
	textureFileName[len] = '.';
	textureFileName[len + 1] = 'd';
	textureFileName[len + 2] = 'd';
	textureFileName[len + 3] = 's';
	textureFileName[len + 4] = '\0';

	prepend(textureFileName, "Assets\\");

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
	D3DManager->m_d3dContext->OMSetRenderTargets(1, &D3DManager->m_offscreenRenderTargetView, nullptr);

	m_framebuffer = std::unique_ptr<TextureWrapper>(new TextureWrapper(D3DManager->m_offscreenShaderResourceView));
}

void Direct3DRenderer::clearFrameBufferWithColor(float r, float g, float b, float a)
{
	float color[] = { r, g, b, a };

	D3DManager->m_d3dContext->ClearRenderTargetView(D3DManager->m_renderTargetView, color);
	D3DManager->m_d3dContext->ClearRenderTargetView(D3DManager->m_offscreenRenderTargetView, color);
}

void Direct3DRenderer::bindToScreenFramebuffer()
{
	D3DManager->m_d3dContext->OMSetRenderTargets(1, &D3DManager->m_renderTargetView, nullptr);
}

void Direct3DRenderer::beginFrame()
{
	D3DManager->m_d3dContext->OMSetRenderTargets(1, &D3DManager->m_offscreenRenderTargetView, nullptr);
}

void Direct3DRenderer::endFrame()
{
	// Empty
}

GpuProgramWrapper& Direct3DRenderer::getFramebufferToScreenGpuProgramWrapper()
{
	return *D3DManager->m_fbToScreenProgram;
}

void Direct3DRenderer::cleanUp()
{
	for (std::vector<ID3D11ShaderResourceView *>::iterator itr = g_shaderResourceViews.begin(); itr != g_shaderResourceViews.end(); itr++)
	{
		(*itr)->Release();
	}
}

#pragma mark private

void Direct3DRenderer::loadTexture(LPCWSTR szFile, ID3D11ShaderResourceView **shaderResourceView)
{
	TexMetadata info = TexMetadata();
	DX::ThrowIfFailed(GetMetadataFromDDSFile(szFile, DDS_FLAGS_NONE, info));

	ScratchImage image;
	DX::ThrowIfFailed(LoadFromDDSFile(szFile, DDS_FLAGS_NONE, &info, image));
	DX::ThrowIfFailed(CreateShaderResourceView(D3DManager->m_d3dDevice, image.GetImages(), image.GetImageCount(), info, shaderResourceView));
}