//
//  MacSound.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 4/24/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__MacSound__
#define __noctisgames__MacSound__

#include "ISound.h"

class AudioPlayerOsx;

class MacSound : public ISound
{
public:
    MacSound(int soundId, const char *path, float volume = 1.0f);
    
    virtual ~MacSound();

    virtual void play(bool isLooping = false);
    
    virtual void resume();
    
    virtual void pause();
    
    virtual void stop();
    
    virtual void setVolume(float volume);
    
    virtual bool isLooping();
    
    virtual bool isPlaying();
    
    virtual bool isPaused();

private:
    AudioPlayerOsx* m_player;
    float m_fVolume;
    bool m_isLooping;
    bool m_isPaused;
};

#endif /* defined(__noctisgames__MacSound__) */
