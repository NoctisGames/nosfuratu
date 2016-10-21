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

SuperpoweredSound::SuperpoweredSound(const char *apkPath, unsigned int sampleRate, unsigned int bufferSize, int rawResourceId, int fileOffset, int fileLength) :
m_apkPath(apkPath),
m_fVolume(1.0f * headroom),
m_iRawResourceId(-1),
m_isLooping(false)
{
    m_iRawResourceId = rawResourceId;
    
    m_player = new SuperpoweredAdvancedAudioPlayer(this, playerEventCallback, sampleRate, 0);
    
    m_player->open(m_apkPath, fileOffset, fileLength);
}

SuperpoweredSound::~SuperpoweredSound()
{
    delete m_player;
}

void SuperpoweredSound::play(bool isLooping)
{
    m_isLooping = isLooping;
 
    m_player->seek(0);
    
    m_player->play(false);
}

void SuperpoweredSound::resume()
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

bool SuperpoweredSound::process(short int *output, float *stereoBuffer, unsigned int numberOfSamples)
{
    bool ret = m_player->process(stereoBuffer, false, numberOfSamples, m_fVolume, m_player->currentBpm, m_player->msElapsedSinceLastBeat);
    
    // The stereoBuffer is ready now, let's put the finished audio into the requested buffers.
    if (ret)
    {
        SuperpoweredFloatToShortInt(stereoBuffer, output, numberOfSamples);
    }
    
    return ret;
}
