//
//  NGSuperpoweredSound.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "NGSuperpoweredSound.h"

#include "SuperpoweredAdvancedAudioPlayer.h"

#include <math.h>

#include <SuperpoweredSimple.h>

#define HEADROOM_DECIBEL 3.0f
static const float headroom = powf(10.0f, -HEADROOM_DECIBEL * 0.025f);

static void playerEventCallback(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value)
{
    NGSuperpoweredSound *sps = (NGSuperpoweredSound *)clientData;

    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess)
    {
    	sps->getPlayer()->setFirstBeatMs(0);
        sps->getPlayer()->setPosition(sps->getPlayer()->firstBeatMs, false, false);
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_EOF)
    {
        bool *pBoolValue = (bool *)value;
        *pBoolValue = !sps->isLooping();
    }
}

NGSuperpoweredSound::NGSuperpoweredSound(int soundId, const char *path, unsigned int sampleRate, int fileOffset, int fileLength, float volume) :
m_path(path),
m_fVolume(volume * headroom),
m_iSoundId(soundId),
m_iLastSamplerate(sampleRate),
m_isLooping(false),
m_isPaused(false)
{
    m_player = new SuperpoweredAdvancedAudioPlayer(this, playerEventCallback, sampleRate, 0);
    
    if (fileOffset > -1 && fileLength > -1)
    {
        m_player->open(m_path, fileOffset, fileLength);
    }
    else
    {
        m_player->open(m_path);
    }
}

NGSuperpoweredSound::~NGSuperpoweredSound()
{
    delete m_player;
}

void NGSuperpoweredSound::play(bool isLooping)
{
    m_isLooping = isLooping;
    m_isPaused = false;
 
    m_player->seek(0);
    
    m_player->play(false);
}

void NGSuperpoweredSound::resume()
{
    if (m_isPaused)
    {
        m_player->play(false);
        
        m_isPaused = false;
    }
}

void NGSuperpoweredSound::pause()
{
    if (isPlaying())
    {
        m_player->pause();
        
        m_isPaused = true;
    }
}

void NGSuperpoweredSound::stop()
{
    m_isLooping = false;
    m_isPaused = false;
    
    m_player->pause();
    m_player->setFirstBeatMs(0);
    m_player->setPosition(m_player->firstBeatMs, false, false);
}

void NGSuperpoweredSound::setVolume(float volume)
{
    m_fVolume = volume * headroom;
}

bool NGSuperpoweredSound::process(float *stereoBuffer, void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    if (m_fVolume <= 0)
    {
        return false;
    }
    
    if (sampleRate != -1
        && sampleRate != m_iLastSamplerate)
    {
        // Has samplerate changed?
        m_iLastSamplerate = sampleRate;
        m_player->setSamplerate(sampleRate);
    }
    
    bool ret = m_player->process(stereoBuffer, false, numberOfSamples, m_fVolume);
    
    // The stereoBuffer is ready now, let's put the finished audio into the requested buffers.
    if (ret)
    {
#if defined __APPLE__
        float **buffers = (float **)output;
        SuperpoweredDeInterleave(stereoBuffer, buffers[0], buffers[1], numberOfSamples); // The stereoBuffer is ready now, let's put the finished audio into the requested buffers.
#elif defined __ANDROID__
        short int *realOutput = (short int *)output;
        SuperpoweredFloatToShortInt(stereoBuffer, realOutput, numberOfSamples);
#endif
    }
    
    return ret;
}

SuperpoweredAdvancedAudioPlayer* NGSuperpoweredSound::getPlayer() { return m_player; }
int NGSuperpoweredSound::getSoundId() { return m_iSoundId; }
bool NGSuperpoweredSound::isLooping() { return m_isLooping; }
bool NGSuperpoweredSound::isPlaying() { return m_player->playing; }
bool NGSuperpoweredSound::isPaused() { return m_isPaused; }
