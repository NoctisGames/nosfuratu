//
//  GameSound.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#pragma once

#include "XAudio2SoundPlayer.h"

class GameSound
{
public:
	static XAudio2SoundPlayer * getSoundPlayerInstance();

	GameSound(Platform::String^ fileName);

	void play(bool isLoop = false);

	void stop();

	void clearFrame();

private:
	int m_sounds[4];
	int m_iSoundIndex;

	int loadSound(Platform::String^ fileName);
};