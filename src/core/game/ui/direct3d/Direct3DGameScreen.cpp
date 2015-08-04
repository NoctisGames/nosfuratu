//
//  Direct3DGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "Direct3DGameScreen.h"
#include "Vector2D.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Assets.h"
#include "OverlapTester.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Font.h"
#include "Direct3DRenderer.h"
#include "GameSound.h"
#include "SpriteBatcher.h"
#include "LineBatcher.h"
#include "RectangleBatcher.h"
#include "CircleBatcher.h"
#include "Rectangle.h"
#include "Circle.h"
#include "GameConstants.h"
#include "ResourceConstants.h"
#include "Direct3DManager.h"

Direct3DGameScreen::Direct3DGameScreen() : GameScreen()
{
	// No further setup
}

void Direct3DGameScreen::load(float deviceScreenWidth, float deviceScreenHeight, int deviceScreenDpWidth, int deviceScreenDpHeight)
{
	m_iDeviceScreenWidth = deviceScreenWidth;
	m_iDeviceScreenHeight = deviceScreenHeight;
	m_fGameScreenToDeviceScreenWidthRatio = deviceScreenWidth / SCREEN_WIDTH;
	m_fGameScreenToDeviceScreenHeightRatio = deviceScreenHeight / SCREEN_HEIGHT;
	m_fDipToPixelRatio = (float)deviceScreenWidth / (float)deviceScreenDpWidth;

	D3DManager->init(deviceScreenWidth, deviceScreenHeight);

	m_renderer = std::unique_ptr<Direct3DRenderer>(new Direct3DRenderer());
}

void Direct3DGameScreen::updateForRenderResolutionChange(float width, float height)
{
	m_iDeviceScreenWidth = width;
	m_iDeviceScreenHeight = height;

	ID3D11RenderTargetView* nullViews[] = { nullptr };
	D3DManager->m_deviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	D3DManager->m_renderTarget = nullptr;
	D3DManager->m_renderTargetView = nullptr;
	D3DManager->m_offscreenRenderTarget = nullptr;
	D3DManager->m_offscreenRenderTargetView = nullptr;
	D3DManager->m_deviceContext->Flush();

	D3DManager->initWindowSizeDependentResources(width, height);
}

void Direct3DGameScreen::handleSound()
{
	// TODO
}

void Direct3DGameScreen::handleMusic()
{
	// TODO
}

void Direct3DGameScreen::unload()
{
	m_renderer->cleanUp();

	D3DManager->cleanUp();
}

ID3D11Texture2D* Direct3DGameScreen::getTexture()
{
	return D3DManager->m_renderTarget;
}

void Direct3DGameScreen::touchToWorld(TouchEvent &touchEvent)
{
	m_touchPoint->set(touchEvent.getX() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenWidthRatio, SCREEN_HEIGHT - (touchEvent.getY() * m_fDipToPixelRatio / m_fGameScreenToDeviceScreenHeightRatio));
}

void Direct3DGameScreen::platformResume()
{
	//GameSound::getSoundPlayerInstance()->Resume();
}

void Direct3DGameScreen::platformPause()
{
	//GameSound::getSoundPlayerInstance()->Suspend();
}

bool Direct3DGameScreen::handleOnBackPressed()
{
	return false;
}