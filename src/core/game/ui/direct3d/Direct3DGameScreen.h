//
//  Direct3DGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#pragma once

#include "GameScreen.h"
#include "DeviceResources.h"
#include "MediaEnginePlayer.h"

#include <memory>

class GameSound;

class Direct3DGameScreen : public GameScreen, DX::IDeviceNotify
{
public:
	Direct3DGameScreen(DX::DeviceResources* deviceResources);

	~Direct3DGameScreen();

	void onScreenSizeChanged(float screenDpWidth, float screenDpHeight);

	virtual void onResume();

	virtual void onPause();

	void handleSound();

	void handleMusic();

	// IDeviceNotify
	virtual void OnDeviceLost();
	virtual void OnDeviceRestored();

	virtual void touchToWorld(TouchEvent &touchEvent);

	virtual bool handleOnBackPressed();

private:
	DX::DeviceResources* m_deviceResources;
	std::unique_ptr<MediaEnginePlayer> m_mediaPlayer;
	std::unique_ptr<GameSound> m_collectCarrotSound;
	std::unique_ptr<GameSound> m_collectGoldenCarrotSound;

	float m_fScreenDpWidth;
	float m_fScreenDpHeight;
};