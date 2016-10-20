//
//  TextureWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__TextureWrapper__
#define __noctisgames__TextureWrapper__

#include "GpuTextureDataWrapper.h"
#include "GpuTextureWrapper.h"

struct TextureWrapper
{
    GpuTextureDataWrapper* gpuTextureDataWrapper;
    GpuTextureWrapper* gpuTextureWrapper;
    int repeat_s;
    
    TextureWrapper() : gpuTextureDataWrapper(nullptr), gpuTextureWrapper(nullptr), repeat_s(0)
    {
        // Empty
    }
};

#endif /* defined(__noctisgames__TextureWrapper__) */
