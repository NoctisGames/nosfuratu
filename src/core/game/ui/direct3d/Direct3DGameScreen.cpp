//
//  Direct3DGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "Direct3DGameScreen.h"
#include "Direct3DRenderer.h"
#include "Direct3DManager.h"
#include "GameSound.h"

Direct3DGameScreen::Direct3DGameScreen(DX::DeviceResources* deviceResources) : GameScreen()
{
	m_deviceResources = deviceResources;

	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	D3DManager->init(*m_deviceResources, m_deviceResources->m_d3dRenderTargetSize.Width, m_deviceResources->m_d3dRenderTargetSize.Height);

	m_renderer = std::unique_ptr<Direct3DRenderer>(new Direct3DRenderer());

	// Load Background Music
	m_mediaPlayer = std::unique_ptr<MediaEnginePlayer>(new MediaEnginePlayer);
	m_mediaPlayer->Initialize(D3DManager->m_d3dDevice, DXGI_FORMAT_B8G8R8A8_UNORM);

	// Load Sound Effects
	m_explosionSound = std::unique_ptr<GameSound>(new GameSound("assets\\explosion.wav"));
}

Direct3DGameScreen::~Direct3DGameScreen()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

void Direct3DGameScreen::onScreenSizeChanged(float screenDpWidth, float screenDpHeight)
{
	m_fScreenDpWidth = screenDpWidth;
	m_fScreenDpHeight = screenDpHeight;

	D3DManager->initWindowSizeDependentResources(*m_deviceResources, m_deviceResources->m_d3dRenderTargetSize.Width, m_deviceResources->m_d3dRenderTargetSize.Height);
}

void Direct3DGameScreen::handleSound()
{
	short soundId;
	while ((soundId = Assets::getInstance()->getFirstSoundId()) > 0)
	{
		Assets::getInstance()->eraseFirstSoundId();

		switch (soundId)
		{
		case SOUND_DEMO:
			m_explosionSound->play();
			break;
		default:
			continue;
		}
	}
}

void Direct3DGameScreen::handleMusic()
{
	short musicId = Assets::getInstance()->getMusicId();
	Assets::getInstance()->setMusicId(0);

	switch (musicId)
	{
	case MUSIC_STOP:
		m_mediaPlayer->Pause();
		break;
	case MUSIC_PLAY_DEMO:
		m_mediaPlayer->SetSource("assets\\bgm.wav");
		m_mediaPlayer->Play();
		break;
	default:
		break;
	}
}

// Notifies renderers that device resources need to be released.
void Direct3DGameScreen::OnDeviceLost()
{
	onPause();

	m_mediaPlayer->Shutdown();

	m_renderer->cleanUp();

	D3DManager->cleanUp();
}

// Notifies renderers that device resources may now be recreated.
void Direct3DGameScreen::OnDeviceRestored()
{
	onScreenSizeChanged(m_fScreenDpWidth, m_fScreenDpHeight);
	onResume();
}

void Direct3DGameScreen::touchToWorld(TouchEvent &touchEvent)
{
	m_touchPoint->set(touchEvent.getX() / m_fScreenDpWidth * CAM_WIDTH, CAM_HEIGHT - (touchEvent.getY() / m_fScreenDpHeight * CAM_HEIGHT));
}

void Direct3DGameScreen::onResume()
{
	GameScreen::onResume();

	GameSound::getSoundPlayerInstance()->Resume();
}

void Direct3DGameScreen::onPause()
{
	GameScreen::onPause();

	GameSound::getSoundPlayerInstance()->Suspend();
}

bool Direct3DGameScreen::handleOnBackPressed()
{
	return false;
}