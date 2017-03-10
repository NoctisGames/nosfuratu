//
//  WinSoundWrapper.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "WinSoundWrapper.h"

#include "NGSTDUtil.h"

WinSoundWrapper::WinSoundWrapper(int soundId, const char *path, DirectX::AudioEngine* audioEngine, int numInstances) : SoundWrapper(soundId, numInstances)
{
    size_t len = strlen(path);
    
    char* wavFileName = new char[len + 5];
    
    strcpy(wavFileName, path);
    wavFileName[len] = '.';
    wavFileName[len + 1] = 'w';
    wavFileName[len + 2] = 'a';
    wavFileName[len + 3] = 'v';
    wavFileName[len + 4] = '\0';
    
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, wavFileName, -1, wString, 4096);
    
    m_sound = std::make_unique<SoundEffect>(audioEngine, waveFileName);
    
    delete wString;
    delete wavFileName;
    
    for (int i = 0; i < m_iNumInstances; ++i)
    {
        m_sounds.push_back(*m_sound);
    }
}

WinSoundWrapper::~WinSoundWrapper()
{
    if (m_sound)
    {
        m_sound.reset();
    }
}
