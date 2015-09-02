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

#include <string>
#include <sstream>

using namespace DirectX;

ID3D11ShaderResourceView *m_backgroundShaderResourceView;

Direct3DRenderer::Direct3DRenderer() : Renderer()
{
	m_spriteBatcher = std::unique_ptr<Direct3DSpriteBatcher>(new Direct3DSpriteBatcher());

	loadTexture(L"Assets\\background_texture.dds", &m_backgroundShaderResourceView);
	m_backgroundTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_backgroundShaderResourceView));
}

void Direct3DRenderer::clearScreenWithColor(float r, float g, float b, float a)
{
	float color[] = { r, g, b, a };

	D3DManager->m_d3dContext->ClearRenderTargetView(D3DManager->m_renderTargetView, color);
	D3DManager->m_d3dContext->ClearRenderTargetView(D3DManager->m_offscreenRenderTargetView, color);
}

void Direct3DRenderer::beginFrame()
{
	D3DManager->m_d3dContext->OMSetRenderTargets(1, &D3DManager->m_offscreenRenderTargetView, nullptr);
}

void Direct3DRenderer::endFrame()
{
	D3DManager->m_d3dContext->OMSetRenderTargets(1, &D3DManager->m_renderTargetView, nullptr);

	m_spriteBatcher->beginBatch();

	static PhysicalEntity go = PhysicalEntity(GAME_WIDTH / 2, GAME_HEIGHT / 2, GAME_WIDTH, GAME_HEIGHT, 0);

	static TextureRegion screenTr = TextureRegion(0, 0, 1, 1, 1, 1);
	renderPhysicalEntity(go, screenTr);

	TextureWrapper tw = TextureWrapper(D3DManager->m_offscreenShaderResourceView);

	m_spriteBatcher->endBatch(tw);
}

void Direct3DRenderer::cleanUp()
{
	m_backgroundShaderResourceView->Release();
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