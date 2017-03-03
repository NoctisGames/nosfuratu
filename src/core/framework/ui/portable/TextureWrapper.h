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
    bool repeatS;
    bool isLoadingData;
    
    TextureWrapper(std::string inName) : name(inName), gpuTextureDataWrapper(nullptr), gpuTextureWrapper(nullptr), repeatS(false), isLoadingData(false)
    {
        // Empty
    }
    
    TextureWrapper(std::string inName, bool in_repeatS) : name(inName), gpuTextureDataWrapper(nullptr), gpuTextureWrapper(nullptr), repeatS(in_repeatS), isLoadingData(false)
    {
        // Empty
    }
};

#endif /* defined(__noctisgames__TextureWrapper__) */
