//
//  GpuTextureDataWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/9/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__GpuTextureDataWrapper__
#define __noctisgames__GpuTextureDataWrapper__

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#if defined TARGET_OS_IPHONE || defined TARGET_OS_OSX || defined __ANDROID__
extern "C"
{
#include "PngImageData.h"
}
#elif defined _WIN32
#include "pch.h"
#endif

struct GpuTextureDataWrapper
{
#if defined TARGET_OS_IPHONE || defined TARGET_OS_OSX || defined __ANDROID__
    PngImageData raw_image_data;
    
    GpuTextureDataWrapper(PngImageData raw_image_data_in) : raw_image_data(raw_image_data_in)
    {
        // Empty
    }
#elif defined _WIN32
    DirectX::ID3D11ShaderResourceView *texture;

    GpuTextureDataWrapper(DirectX::ID3D11ShaderResourceView *textureIn) : texture(textureIn)
	{
		// Empty
	}
#endif
};

#endif /* defined(__noctisgames__GpuTextureDataWrapper__) */
