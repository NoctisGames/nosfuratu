//
//  Win81Sound.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/10/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "Win81Sound.h"

Win81Sound::Win81Sound(int soundId, DirectX::SoundEffect& sound, float volume) : ISound(soundId),
m_isLooping(false),
m_isPaused(false)
{
    m_soundEffectInstance = sound.CreateInstance();
}

Win81Sound::~Win81Sound()
{
    m_soundEffectInstance->Pause();
    
    m_soundEffectInstance.reset();
}

void Win81Sound::play(bool isLooping)
{
    m_isLooping = isLooping;
    m_isPaused = false;
 
	m_soundEffectInstance->Stop();
    m_soundEffectInstance->Play(isLooping);
}

void Win81Sound::resume()
{
    if (m_isPaused)
    {
        m_soundEffectInstance->Resume();
        
        m_isPaused = false;
    }
}

void Win81Sound::pause()
{
    if (isPlaying())
    {
        m_soundEffectInstance->Pause();
        
        m_isPaused = true;
    }
}

void Win81Sound::stop()
{
    m_isLooping = false;
    m_isPaused = false;
    
    m_soundEffectInstance->Stop();
}

void Win81Sound::setVolume(float volume)
{
    m_soundEffectInstance->SetVolume(volume);
}

bool Win81Sound::isLooping()
{
    return m_isLooping;
}

bool Win81Sound::isPlaying()
{
	return m_soundEffectInstance->GetState() == DirectX::SoundState::PLAYING;
}

bool Win81Sound::isPaused()
{
    return m_isPaused;
}
