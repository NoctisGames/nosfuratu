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

ID3D11ShaderResourceView *m_backgroundShaderResourceView;
ID3D11ShaderResourceView *m_jonShaderResourceView;

Direct3DRenderer::Direct3DRenderer() : Renderer()
{
	m_spriteBatcher = std::unique_ptr<Direct3DSpriteBatcher>(new Direct3DSpriteBatcher());

	loadTexture(L"Assets\\level_1_background.dds", &m_backgroundShaderResourceView);
	m_background = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_backgroundShaderResourceView));

	loadTexture(L"Assets\\jon.dds", &m_jonShaderResourceView);
	m_jon = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_jonShaderResourceView));

	//m_framebuffer = std::unique_ptr<TextureWrapper>(new TextureWrapper(D3DManager->m_offscreenShaderResourceView));
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
	m_backgroundShaderResourceView->Release();
	m_jonShaderResourceView->Release();
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