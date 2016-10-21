//
//  SuperpoweredSound.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "SuperpoweredSound.h"

#include <jni.h>
#include <stdio.h>
#include <vector>
#include <malloc.h>
#include <math.h>

#include <SuperpoweredSimple.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#define HEADROOM_DECIBEL 3.0f
static const float headroom = powf(10.0f, -HEADROOM_DECIBEL * 0.025f);

static void playerEventCallback(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void * __unused value)
{
    SuperpoweredSound *sps = (SuperpoweredSound *)clientData;

    if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess)
    {
    	sps->getPlayer()->setFirstBeatMs(0);
        sps->getPlayer()->setPosition(sps->getPlayer()->firstBeatMs, false, false);
    }
    else if (event == SuperpoweredAdvancedAudioPlayerEvent_EOF
             && !sps->isLooping())
    {
        sps->getPlayer()->pause();
    }
}

static bool audioProcessing(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
	return ((SuperpoweredSound *)clientData)->process(audioIO, (unsigned int)numberOfSamples);
}

SuperpoweredSound::SuperpoweredSound(const char *apkPath, unsigned int sampleRate, unsigned int bufferSize) :
m_apkPath(apkPath),
m_fVolume(1.0f * headroom),
m_iRawResourceId(-1),
m_isLooping(false)
{
    m_stereoBuffer = (float *)memalign(16, (bufferSize + 16) * sizeof(float) * 2);

    m_player = new SuperpoweredAdvancedAudioPlayer(this, playerEventCallback, sampleRate, 0);

    m_audioSystem = new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessing, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2);
}

SuperpoweredSound::~SuperpoweredSound()
{
    delete m_audioSystem;
    delete m_player;
    
    free(m_stereoBuffer);
}

void SuperpoweredSound::loadAndPlaySound(int rawResourceId, int fileOffset, int fileLength, bool isLooping)
{
    m_iRawResourceId = rawResourceId;
    m_isLooping = isLooping;
    
    pause();
    
    m_player->open(m_apkPath, fileOffset, fileLength);
    
    play();
}

void SuperpoweredSound::play()
{
    m_player->play(false);
}

void SuperpoweredSound::pause()
{
    m_player->pause();
}

void SuperpoweredSound::stop()
{
    m_isLooping = false;
    
    pause();
}

void SuperpoweredSound::setVolume(float volume)
{
    m_fVolume = volume * headroom;
}

bool SuperpoweredSound::process(short int *output, unsigned int numberOfSamples)
{
    bool silence = !m_player->process(m_stereoBuffer, false, numberOfSamples, m_fVolume, m_player->currentBpm, m_player->msElapsedSinceLastBeat);
    
    // The m_stereoBuffer is ready now, let's put the finished audio into the requested buffers.
    if (!silence)
    {
        SuperpoweredFloatToShortInt(m_stereoBuffer, output, numberOfSamples);
    }
    
    return !silence;
}
