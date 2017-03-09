//
//  NGSuperpoweredSound.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__NGSuperpoweredSound__
#define __noctisgames__NGSuperpoweredSound__

class SuperpoweredAdvancedAudioPlayer;

class NGSuperpoweredSound
{
public:
    NGSuperpoweredSound(int soundId, const char *path, unsigned int sampleRate, int fileOffset = -1, int fileLength = -1, float volume = 1.0f);
    
    ~NGSuperpoweredSound();

    void play(bool isLooping = false);
    void resume();
    void pause();
    void stop();
    void setVolume(float volume);
    
    // output is float **buffers for iOS and Mac OS X and short int * for Android
    bool process(float *stereoBuffer, void *output, unsigned int numberOfSamples, unsigned int sampleRate = -1);
    
    SuperpoweredAdvancedAudioPlayer* getPlayer();
    int getSoundId();
    bool isLooping();
    bool isPlaying();
    bool isPaused();

private:
    SuperpoweredAdvancedAudioPlayer *m_player;
    const char *m_path;
    float m_fVolume;
    int m_iSoundId;
    unsigned int m_iLastSamplerate;
    bool m_isLooping;
    bool m_isPaused;
};

#endif /* defined(__noctisgames__NGSuperpoweredSound__) */
