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

#include <vector>

class SuperpoweredSoundManager
{
public:
    SuperpoweredSoundManager(const char *apkPath, unsigned int sampleRate, unsigned int bufferSize);
	~SuperpoweredSoundManager();
    
    void loadAndPlaySound(int rawResourceId, int fileOffset, int fileLength, float volume, bool isLooping);
    
    void stopSound(int rawResourceId);
    
    void loadAndPlayMusic(int rawResourceId, int fileOffset, int fileLength, float volume, bool isLooping);
    
    void setMusicVolume(float volume);
    
    void resumeMusic();
    
    void pauseMusic();

private:
    SuperpoweredSound* m_music;
    std::vector<SuperpoweredSound*> m_sounds;
};

#endif /* defined(__nosfuratu__SuperpoweredSound__) */
