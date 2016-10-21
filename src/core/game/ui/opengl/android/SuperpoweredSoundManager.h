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

#include "SuperpoweredAndroidAudioIO.h"

#include <vector>

#define MAX_NUM_SOUND_PLAYERS 12
#define MAX_NUM_SOUNDS_PER_COLLECTION 4

class SuperpoweredSoundCollection
{
public:
    std::vector<SuperpoweredSound*> m_sounds;
    int m_iSoundIndex;
    int m_iRawResourceId;
    
    SuperpoweredSoundCollection(const char *apkPath, unsigned int sampleRate, unsigned int bufferSize, int rawResourceId, int fileOffset, int fileLength) : m_iSoundIndex(0), m_iRawResourceId(rawResourceId)
    {
        for (int i = 0; i < MAX_NUM_SOUNDS_PER_COLLECTION; i++)
        {
            m_sounds.push_back(new SuperpoweredSound(apkPath, sampleRate, bufferSize, rawResourceId, fileOffset, fileLength));
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
    SuperpoweredSoundManager(const char *apkPath, unsigned int sampleRate, unsigned int bufferSize);
	~SuperpoweredSoundManager();
    
    void loadSound(int rawResourceId, int fileOffset, int fileLength);
    
    void playSound(int rawResourceId, float volume, bool isLooping);
    
    void stopSound(int rawResourceId);
    
    void loadMusic(int rawResourceId, int fileOffset, int fileLength);
    
    void playMusic(int rawResourceId, float volume, bool isLooping);
    
    void setMusicVolume(float volume);
    
    void resumeMusic();
    
    void pauseMusic();
    
    bool processMusic(short int *output, unsigned int numberOfSamples);
    
    bool processSound1(short int *output, unsigned int numberOfSamples);
    bool processSound2(short int *output, unsigned int numberOfSamples);
    bool processSound3(short int *output, unsigned int numberOfSamples);
    bool processSound4(short int *output, unsigned int numberOfSamples);
    bool processSound5(short int *output, unsigned int numberOfSamples);
    bool processSound6(short int *output, unsigned int numberOfSamples);
    bool processSound7(short int *output, unsigned int numberOfSamples);
    bool processSound8(short int *output, unsigned int numberOfSamples);
    bool processSound9(short int *output, unsigned int numberOfSamples);
    bool processSound10(short int *output, unsigned int numberOfSamples);
    bool processSound11(short int *output, unsigned int numberOfSamples);
    
    bool processSound(short int *output, unsigned int numberOfSamples, SuperpoweredSound *sound, float *stereoBuffer);

private:
    SuperpoweredSound* m_music;
    std::vector<SuperpoweredSoundCollection*> m_sounds;
    std::vector<SuperpoweredAndroidAudioIO*> m_audioSystems;
    std::vector<float*> m_stereoBuffers;
    SuperpoweredSound *m_activeSounds[MAX_NUM_SOUND_PLAYERS - 1];
    const char *m_apkPath;
    int m_iSampleRate;
    int m_iBufferSize;
    int m_iSoundIndex;
};

#endif /* defined(__nosfuratu__SuperpoweredSound__) */
