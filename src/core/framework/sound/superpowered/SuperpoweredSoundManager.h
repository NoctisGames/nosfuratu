//
//  SuperpoweredSoundManager.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__SuperpoweredSoundManager__
#define __nosfuratu__SuperpoweredSoundManager__

#include "SuperpoweredSound.h"

#include "EntityUtils.h"

#include <vector>

#define MAX_NUM_SOUND_PLAYERS 12
#define MAX_NUM_SOUNDS_PER_COLLECTION 4

class SuperpoweredSoundCollection
{
public:
    std::vector<SuperpoweredSound*> m_sounds;
    int m_iSoundIndex;
    int m_iRawResourceId;
    
    SuperpoweredSoundCollection(const char *path, unsigned int sampleRate, int rawResourceId, int fileOffset = -1, int fileLength = -1) : m_iSoundIndex(0), m_iRawResourceId(rawResourceId)
    {
        for (int i = 0; i < MAX_NUM_SOUNDS_PER_COLLECTION; i++)
        {
            m_sounds.push_back(new SuperpoweredSound(path, sampleRate, rawResourceId, fileOffset, fileLength));
        }
    }
    
    ~SuperpoweredSoundCollection()
    {
        EntityUtils::cleanUpVectorOfPointers(m_sounds);
    }
    
    SuperpoweredSound* getSound()
    {
        SuperpoweredSound* ret = m_sounds[m_iSoundIndex++];
        if (m_iSoundIndex >= MAX_NUM_SOUNDS_PER_COLLECTION)
        {
            m_iSoundIndex = 0;
        }
        
        return ret;
    }
    
    int getRawResourceId() { return m_iRawResourceId; }
};

class SuperpoweredSoundManager
{
public:
    SuperpoweredSoundManager(unsigned int sampleRate, unsigned int bufferSize = 0);
	~SuperpoweredSoundManager();
    
    void loadSound(int rawResourceId, const char *path, int fileOffset = -1, int fileLength = -1);
    
    void playSound(int rawResourceId, float volume, bool isLooping);
    
    void stopSound(int rawResourceId);
    
    void loadMusic(int rawResourceId, const char *path, int fileOffset = -1, int fileLength = -1);
    
    void playMusic(int rawResourceId, float volume, bool isLooping);
    
    void setMusicVolume(float volume);
    
    void resumeMusic();
    
    void pauseMusic();
    
    bool processMusic(void *output, unsigned int numberOfSamples);
    
    bool processSound1(void *output, unsigned int numberOfSamples);
    bool processSound2(void *output, unsigned int numberOfSamples);
    bool processSound3(void *output, unsigned int numberOfSamples);
    bool processSound4(void *output, unsigned int numberOfSamples);
    bool processSound5(void *output, unsigned int numberOfSamples);
    bool processSound6(void *output, unsigned int numberOfSamples);
    bool processSound7(void *output, unsigned int numberOfSamples);
    bool processSound8(void *output, unsigned int numberOfSamples);
    bool processSound9(void *output, unsigned int numberOfSamples);
    bool processSound10(void *output, unsigned int numberOfSamples);
    bool processSound11(void *output, unsigned int numberOfSamples);
    
    bool processSound(void *output, unsigned int numberOfSamples, SuperpoweredSound *sound, float *stereoBuffer);

private:
    SuperpoweredSound* m_music;
    std::vector<SuperpoweredSoundCollection*> m_sounds;
    std::vector<float*> m_stereoBuffers;
    SuperpoweredSound *m_activeSounds[MAX_NUM_SOUND_PLAYERS - 1];
    int m_iSampleRate;
    int m_iSoundIndex;
};

#endif /* defined(__nosfuratu__SuperpoweredSound__) */
