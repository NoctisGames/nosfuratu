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
#include "GameObject.h"
#include "TextureRegion.h"
#include "Assets.h"
#include "Vector2D.h"
#include "Font.h"
#include "TextureWrapper.h"
#include "Line.h"
#include "Rectangle.h"
#include "BasicReaderWriter.h"
#include "DDSTextureLoader.h"
#include "SpriteBatcher.h"
#include "Direct3DRectangleBatcher.h"
#include "Direct3DSpriteBatcher.h"
#include "Direct3DLineBatcher.h"
#include "Direct3DCircleBatcher.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Direct3DManager.h"

#include <string>
#include <sstream>

using namespace DirectX;

ID3D11ShaderResourceView *m_backgroundShaderResourceView;

Direct3DRenderer::Direct3DRenderer() : Renderer()
{
	m_spriteBatcher = std::unique_ptr<Direct3DSpriteBatcher>(new Direct3DSpriteBatcher());

	CreateDDSTextureFromFile(D3DManager->m_device, L"Assets\\background_texture.dds", NULL, &m_backgroundShaderResourceView, NULL);

	m_backgroundTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(m_backgroundShaderResourceView));
}

void Direct3DRenderer::clearScreenWithColor(float r, float g, float b, float a)
{
	float color[] = { r, g, b, a };

	// set our new render target object as the active render target
	D3DManager->m_deviceContext->OMSetRenderTargets(1, &D3DManager->m_renderTargetView, nullptr);

	D3DManager->m_deviceContext->ClearRenderTargetView(D3DManager->m_renderTargetView, color);
}

void Direct3DRenderer::beginFrame()
{
	// Not Needed Yet
}

void Direct3DRenderer::endFrame()
{
	// Not Needed Yet
}

void Direct3DRenderer::cleanUp()
{
	m_backgroundShaderResourceView->Release();
}