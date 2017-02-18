//
//  TextureWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/9/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__TextureWrapper__
#define __noctisgames__TextureWrapper__

#include "GpuTextureDataWrapper.h"
#include "GpuTextureWrapper.h"

#include <string>

struct TextureWrapper
{
    std::string name;
    GpuTextureDataWrapper* gpuTextureDataWrapper;
    GpuTextureWrapper* gpuTextureWrapper;
    int repeat_s;
    bool isLoadingData;
    
    TextureWrapper(std::string inName) : name(inName), gpuTextureDataWrapper(nullptr), gpuTextureWrapper(nullptr), repeat_s(0), isLoadingData(false)
    {
        // Empty
    }
    
    TextureWrapper(std::string inName, int in_repeat_s) : name(inName), gpuTextureDataWrapper(nullptr), gpuTextureWrapper(nullptr), repeat_s(in_repeat_s), isLoadingData(false)
    {
        // Empty
    }
};

#endif /* defined(__noctisgames__TextureWrapper__) */
