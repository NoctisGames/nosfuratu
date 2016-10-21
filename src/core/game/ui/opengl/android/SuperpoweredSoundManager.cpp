//
//  SuperpoweredSoundManager.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "SuperpoweredSoundManager.h"

#include <SuperpoweredSimple.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

static bool audioProcessingMusic(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processMusic(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound1(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound1(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound2(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound2(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound3(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound3(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound4(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound4(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound5(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound5(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound6(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound6(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound7(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound7(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound8(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound8(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound9(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound9(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound10(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound10(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound11(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound11(audioIO, (unsigned int)numberOfSamples);
}

SuperpoweredSoundManager::SuperpoweredSoundManager(const char *apkPath, unsigned int sampleRate, unsigned int bufferSize) :
m_music(nullptr),
m_activeSounds(),
m_apkPath(apkPath),
m_iSampleRate(sampleRate),
m_iBufferSize(bufferSize),
m_iSoundIndex(0)
{
    for (int i = 0; i < MAX_NUM_SOUND_PLAYERS; i++)
    {
        m_stereoBuffers.push_back((float *)memalign(16, (bufferSize + 16) * sizeof(float) * 2));
    }
    
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingMusic, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound1, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound2, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound3, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound4, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound5, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound6, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound7, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound8, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound9, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound10, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound11, this, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
}

SuperpoweredSoundManager::~SuperpoweredSoundManager()
{
    delete m_music;
    EntityUtils::cleanUpVectorOfPointers(m_sounds);
    
    for (int i = 0; i < MAX_NUM_SOUND_PLAYERS; i++)
    {
        free(m_stereoBuffers[i]);
    }
    
    EntityUtils::cleanUpVectorOfPointers(m_audioSystems);
}

void SuperpoweredSoundManager::loadSound(int rawResourceId, int fileOffset, int fileLength)
{
    m_sounds.push_back(new SuperpoweredSoundCollection(m_apkPath, m_iSampleRate, m_iBufferSize, rawResourceId, fileOffset, fileLength));
}

void SuperpoweredSoundManager::playSound(int rawResourceId, float volume, bool isLooping)
{
    for (std::vector<SuperpoweredSoundCollection *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getRawResourceId() == rawResourceId)
        {
            SuperpoweredSound* sound = (*i)->getSound();
            
            sound->play(isLooping);
            
            for (int j = 0; j < (MAX_NUM_SOUND_PLAYERS - 1); j++)
            {
                if (m_activeSounds[j] == sound)
                {
                    return;
                }
            }
            
            int count = 0;
            bool isGoodToBreak = false;
            while (true)
            {
                if (!m_activeSounds[m_iSoundIndex]
                    || !m_activeSounds[m_iSoundIndex]->isPlaying())
                {
                    m_activeSounds[m_iSoundIndex] = sound;
                    
                    isGoodToBreak = true;
                }
                
                m_iSoundIndex++;
                if (m_iSoundIndex > (MAX_NUM_SOUND_PLAYERS - 3))
                {
                    m_iSoundIndex = 0;
                }
                
                count++;
                if (isGoodToBreak
                    || count > (MAX_NUM_SOUND_PLAYERS - 1))
                {
                    break;
                }
            }
            
            return;
        }
    }
}

void SuperpoweredSoundManager::stopSound(int rawResourceId)
{
    for (std::vector<SuperpoweredSoundCollection *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getRawResourceId() == rawResourceId)
        {
            for (int j = 0; j < MAX_NUM_SOUNDS_PER_COLLECTION; j++)
            {
                SuperpoweredSound* sound = (*i)->getSound();
                if (sound->isPlaying())
                {
                    sound->stop();
                    
                    return;
                }
            }
            
            return;
        }
    }
}

void SuperpoweredSoundManager::loadMusic(int rawResourceId, int fileOffset, int fileLength)
{
    if (m_music)
    {
        m_music->pause();
        
        delete m_music;
    }
    
    m_music = new SuperpoweredSound(m_apkPath, m_iSampleRate, m_iBufferSize, rawResourceId, fileOffset, fileLength, 0.5f);
}

void SuperpoweredSoundManager::playMusic(int rawResourceId, float volume, bool isLooping)
{
    if (m_music)
    {
        m_music->play(isLooping);
        m_music->setVolume(volume);
    }
}

void SuperpoweredSoundManager::setMusicVolume(float volume)
{
    if (m_music)
    {
        m_music->setVolume(volume);
    }
}

void SuperpoweredSoundManager::resumeMusic()
{
    if (m_music)
    {
        m_music->resume();
    }
}

void SuperpoweredSoundManager::pauseMusic()
{
    if (m_music)
    {
        m_music->pause();
    }
}

bool SuperpoweredSoundManager::processMusic(short int *output, unsigned int numberOfSamples)
{
    return m_music && m_music->process(output, m_stereoBuffers[0], numberOfSamples);
}

bool SuperpoweredSoundManager::processSound1(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[0], m_stereoBuffers[1]);
}

bool SuperpoweredSoundManager::processSound2(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[1], m_stereoBuffers[2]);
}

bool SuperpoweredSoundManager::processSound3(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[2], m_stereoBuffers[3]);
}

bool SuperpoweredSoundManager::processSound4(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[3], m_stereoBuffers[4]);
}

bool SuperpoweredSoundManager::processSound5(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[4], m_stereoBuffers[5]);
}

bool SuperpoweredSoundManager::processSound6(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[5], m_stereoBuffers[6]);
}

bool SuperpoweredSoundManager::processSound7(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[6], m_stereoBuffers[7]);
}

bool SuperpoweredSoundManager::processSound8(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[7], m_stereoBuffers[8]);
}

bool SuperpoweredSoundManager::processSound9(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[8], m_stereoBuffers[9]);
}

bool SuperpoweredSoundManager::processSound10(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[9], m_stereoBuffers[10]);
}

bool SuperpoweredSoundManager::processSound11(short int *output, unsigned int numberOfSamples)
{
    return processSound(output, numberOfSamples, m_activeSounds[10], m_stereoBuffers[11]);
}

bool SuperpoweredSoundManager::processSound(short int *output, unsigned int numberOfSamples, SuperpoweredSound *sound, float *stereoBuffer)
{
    return sound && sound->process(output, stereoBuffer, numberOfSamples);
}
