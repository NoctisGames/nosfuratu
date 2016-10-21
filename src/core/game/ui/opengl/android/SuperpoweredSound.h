//
//  SuperpoweredSound.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__SuperpoweredSound__
#define __nosfuratu__SuperpoweredSound__

#include "SuperpoweredAdvancedAudioPlayer.h"
#include "SuperpoweredAndroidAudioIO.h"

class SuperpoweredSound
{
public:
    SuperpoweredSound(const char *apkPath, unsigned int sampleRate, unsigned int bufferSize);
	~SuperpoweredSound();

    void loadAndPlaySound(int rawResourceId, int fileOffset, int fileLength, bool isLooping);
    
	void play();
    void pause();
    void stop();
    void setVolume(float volume);
    
    bool process(short int *output, unsigned int numberOfSamples);
    
    SuperpoweredAdvancedAudioPlayer* getPlayer() { return m_player; }
    int getRawResourceId() { return m_iRawResourceId; }
    bool isLooping() { return m_isLooping; }
    bool isPlaying() { return m_player->playing; }

private:
    SuperpoweredAdvancedAudioPlayer *m_player;
    SuperpoweredAndroidAudioIO *m_audioSystem;
    float *m_stereoBuffer;
    const char *m_apkPath;
    float m_fVolume;
    int m_iRawResourceId;
    bool m_isLooping;
};

#endif /* defined(__nosfuratu__SuperpoweredSound__) */
