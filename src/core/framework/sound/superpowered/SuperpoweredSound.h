//
//  SuperpoweredSound.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__SuperpoweredSound__
#define __noctisgames__SuperpoweredSound__

class SuperpoweredAdvancedAudioPlayer;

class SuperpoweredSound
{
public:
    SuperpoweredSound(const char *path, unsigned int sampleRate, int rawResourceId, int fileOffset = -1, int fileLength = -1, float volume = 1.0f);
    ~SuperpoweredSound();

    void play(bool isLooping = false);
    void resume();
    void pause();
    void stop();
    void setVolume(float volume);
    
    // output is float **buffers for iOS and Mac OS X and short int * for Android
    bool process(float *stereoBuffer, void *output, unsigned int numberOfSamples, unsigned int sampleRate = -1);
    
    SuperpoweredAdvancedAudioPlayer* getPlayer();
    int getRawResourceId();
    bool isLooping();
    bool isPlaying();
    bool isPaused();

private:
    SuperpoweredAdvancedAudioPlayer *m_player;
    const char *m_path;
    float m_fVolume;
    int m_iRawResourceId;
    unsigned int m_iLastSamplerate;
    bool m_isLooping;
    bool m_isPaused;
};

#endif /* defined(__noctisgames__SuperpoweredSound__) */
