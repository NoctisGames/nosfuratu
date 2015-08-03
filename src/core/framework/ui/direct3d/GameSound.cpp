//
//  GameSound.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2013 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "GameSound.h"
#include "Global.h"
#include "SoundFileReader.h"

GameSound::GameSound(Platform::String^ fileName)
{
	m_sounds[0] = loadSound(fileName);
	m_sounds[1] = loadSound(fileName);
	m_sounds[2] = loadSound(fileName);
	m_sounds[3] = loadSound(fileName);

	m_iSoundIndex = 0;
}

void GameSound::play()
{
	Global::getSoundPlayerInstance()->PlaySound(m_sounds[m_iSoundIndex++]);

	if (m_iSoundIndex > 3)
	{
		m_iSoundIndex = 0;
	}
}

int GameSound::loadSound(Platform::String^ fileName)
{
	SoundFileReader sound(fileName);
	return Global::getSoundPlayerInstance()->AddSound(sound.GetSoundFormat(), sound.GetSoundData());
}