//
//  NGSuperpoweredSoundManager.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "NGSuperpoweredSoundManager.h"

#include "NGSuperpoweredSound.h"
#include "NGSuperpoweredSoundCollection.h"
#include "NGSTDUtil.h"

#include <SuperpoweredSimple.h>

#import <stdlib.h>

NGSuperpoweredSoundManager::NGSuperpoweredSoundManager(unsigned int sampleRate, unsigned int bufferSize) :
m_music(nullptr),
m_activeSounds(),
m_iSampleRate(sampleRate),
m_iSoundIndex(0)
{
    for (int i = 0; i < MAX_NUM_SOUND_PLAYERS; i++)
    {
        float* stereoBuffer;
#if defined __APPLE__
        if (posix_memalign((void **)&stereoBuffer, 16, 4096 + 128) != 0) abort(); // Allocating memory, aligned to 16.
#elif defined __ANDROID__
        stereoBuffer = (float *)memalign(16, (bufferSize + 16) * sizeof(float) * 2);
#endif
        m_stereoBuffers.push_back(stereoBuffer);
    }
}

NGSuperpoweredSoundManager::~NGSuperpoweredSoundManager()
{
    delete m_music;
    
    NGSTDUtil::cleanUpVectorOfPointers(m_sounds);
    
    for (int i = 0; i < MAX_NUM_SOUND_PLAYERS; i++)
    {
        free(m_stereoBuffers[i]);
    }
}

void NGSuperpoweredSoundManager::loadSound(int soundId, const char *path, int numInstances, int fileOffset, int fileLength)
{
    m_sounds.push_back(new NGSuperpoweredSoundCollection(soundId, path, m_iSampleRate, numInstances, fileOffset, fileLength));
}

void NGSuperpoweredSoundManager::playSound(int soundId, float volume, bool isLooping)
{
    for (std::vector<NGSuperpoweredSoundCollection *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getSoundId() == soundId)
        {
            NGSuperpoweredSound* sound = (*i)->getSound();
            
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

void NGSuperpoweredSoundManager::stopSound(int soundId)
{
    for (std::vector<NGSuperpoweredSoundCollection *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getSoundId() == soundId)
        {
            for (int j = 0; j < (*i)->getNumInstances(); j++)
            {
                NGSuperpoweredSound* sound = (*i)->getSound();
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

void NGSuperpoweredSoundManager::resumeAllSounds()
{
    for (std::vector<NGSuperpoweredSoundCollection *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        for (int j = 0; j < (*i)->getNumInstances(); j++)
        {
            NGSuperpoweredSound* sound = (*i)->getSound();
            sound->resume();
        }
    }
}

void NGSuperpoweredSoundManager::pauseAllSounds()
{
    for (std::vector<NGSuperpoweredSoundCollection *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        for (int j = 0; j < (*i)->getNumInstances(); j++)
        {
            NGSuperpoweredSound* sound = (*i)->getSound();
            sound->pause();
        }
    }
}

void NGSuperpoweredSoundManager::stopAllSounds(bool stopOnlyLoopingSounds)
{
    for (std::vector<NGSuperpoweredSoundCollection *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        for (int j = 0; j < (*i)->getNumInstances(); j++)
        {
            NGSuperpoweredSound* sound = (*i)->getSound();
            if (!stopOnlyLoopingSounds
                || (stopOnlyLoopingSounds
                    && sound->isLooping()))
            {
                sound->stop();
            }
        }
    }
}

void NGSuperpoweredSoundManager::stopAllLoopingSounds()
{
    stopAllSounds(true);
}

void NGSuperpoweredSoundManager::loadMusic(int soundId, const char *path, int fileOffset, int fileLength)
{
    if (m_music)
    {
        m_music->pause();
        
        delete m_music;
    }
    
    m_music = new NGSuperpoweredSound(soundId, path, m_iSampleRate, fileOffset, fileLength, 0.5f);
}

void NGSuperpoweredSoundManager::playMusic(float volume, bool isLooping)
{
    if (m_music)
    {
        m_music->play(isLooping);
        m_music->setVolume(volume);
    }
}

void NGSuperpoweredSoundManager::setMusicVolume(float volume)
{
    if (m_music)
    {
        m_music->setVolume(volume);
    }
}

void NGSuperpoweredSoundManager::resumeMusic()
{
    if (m_music && m_music->isPaused())
    {
        m_music->resume();
    }
}

void NGSuperpoweredSoundManager::pauseMusic()
{
    if (m_music)
    {
        m_music->pause();
    }
}

bool NGSuperpoweredSoundManager::processMusic(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return m_music && m_music->process(m_stereoBuffers[0], output, numberOfSamples, sampleRate);
}

bool NGSuperpoweredSoundManager::processSound1(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[0], m_stereoBuffers[1], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound2(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[1], m_stereoBuffers[2], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound3(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[2], m_stereoBuffers[3], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound4(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[3], m_stereoBuffers[4], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound5(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[4], m_stereoBuffers[5], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound6(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[5], m_stereoBuffers[6], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound7(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[6], m_stereoBuffers[7], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound8(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[7], m_stereoBuffers[8], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound9(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[8], m_stereoBuffers[9], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound10(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[9], m_stereoBuffers[10], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound11(void *output, unsigned int numberOfSamples, unsigned int sampleRate)
{
    return processSound(output, numberOfSamples, m_activeSounds[10], m_stereoBuffers[11], sampleRate);
}

bool NGSuperpoweredSoundManager::processSound(void *output, unsigned int numberOfSamples, NGSuperpoweredSound *sound, float *stereoBuffer, unsigned int sampleRate)
{
    return sound && sound->process(stereoBuffer, output, numberOfSamples, sampleRate);
}
