//
//  NGAudioEngine.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__NGAudioEngine__
#define __noctisgames__NGAudioEngine__

#define NG_AUDIO_ENGINE (NGAudioEngine::getInstance())

#include <map>

struct SoundWrapper;
class INGAudioEngineHelper;

class NGAudioEngine
{
public:
    static NGAudioEngine* getInstance();
    
    void update();
    
    void pause();
    
    void resume();
    
    void loadSound(int soundId, const char *path, int numInstances = 1);
    
    void playSound(int soundId, bool isLooping = false, float volume = 1.0f);
    
    void pauseSound(int soundId);
    
    void stopSound(int soundId);
    
    void pauseAllSounds();
    
    void stopAllSounds();
    
    void resumeAllSounds();
    
    void loadMusic(const char *path);
    
    void playMusic(bool isLooping = true, float volume = 1.0f);
    
    void setMusicVolume(float volume);
    
    void pauseMusic();
    
    void stopMusic();
    
    void resumeMusic();
    
    bool isMusicDisabled();
    
    void setMusicDisabled(bool isMusicDisabled);
    
    bool isSoundDisabled();
    
    void setSoundDisabled(bool isSoundDisabled);
    
private:
    std::map<int, SoundWrapper*> m_sounds;
    INGAudioEngineHelper* m_audioEngineHelper;
    int m_iNumSoundsPlayedThisFrame;
    bool m_isMusicDisabled;
    bool m_isSoundDisabled;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    NGAudioEngine();
    ~NGAudioEngine();
    NGAudioEngine(const NGAudioEngine&);
    NGAudioEngine& operator=(const NGAudioEngine&);
};

#endif /* defined(__noctisgames__NGAudioEngine__) */
