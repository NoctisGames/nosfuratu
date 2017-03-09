//
//  NGSuperpoweredSoundManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__NGSuperpoweredSoundCollection__
#define __noctisgames__NGSuperpoweredSoundCollection__

#include <vector>

class NGSuperpoweredSound;

class NGSuperpoweredSoundCollection
{
public:
    NGSuperpoweredSoundCollection(int soundId, const char *path, unsigned int sampleRate, int numInstances = 1, int fileOffset = -1, int fileLength = -1);
    
    ~NGSuperpoweredSoundCollection();
    
    NGSuperpoweredSound* getSound();
    
    std::vector<NGSuperpoweredSound *>& getSounds();
    
    int getSoundIndex();
    
    int getSoundId();
    
    int getNumInstances();
    
private:
    std::vector<NGSuperpoweredSound *> m_sounds;
    int m_iSoundIndex;
    int m_iSoundId;
    int m_iNumInstances;
};

#endif /* defined(__noctisgames__NGSuperpoweredSoundCollection__) */
