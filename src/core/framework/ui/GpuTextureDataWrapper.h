//
//  GpuTextureDataWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__GpuTextureDataWrapper__
#define __noctisgames__GpuTextureDataWrapper__

#ifdef GGD_OPENGL_ES
extern "C"
{
#include "image.h"
}
#elif defined GGD_DIRECT_3D
#include <d3d11_1.h>
#endif

struct GpuTextureDataWrapper
{
#ifdef GGD_OPENGL_ES
    RawImageData raw_image_data;
    
    GpuTextureDataWrapper(RawImageData raw_image_data_in) : raw_image_data(raw_image_data_in)
    {
        // Empty
    }
#elif defined GGD_DIRECT_3D
    ScratchImage image;
    TexMetadata info;
    int shaderResourceViewIndex;
    
    GpuTextureDataWrapper(ScratchImage imageIn, TexMetadata infoIn, int shaderResourceViewIndexIn) : image(imageIn), info(infoIn), shaderResourceViewIndex(shaderResourceViewIndexIn)
    {
        // Empty
    }
#endif
};

#endif /* defined(__noctisgames__GpuTextureDataWrapper__) */
