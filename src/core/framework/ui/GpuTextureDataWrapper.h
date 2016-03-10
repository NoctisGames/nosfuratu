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
#include "DirectXTex.h"
using namespace DirectX;
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
	ID3D11ShaderResourceView *texture;

	GpuTextureDataWrapper(ID3D11ShaderResourceView *textureIn) : texture(textureIn)
	{
		// Empty
	}
#endif
};

#endif /* defined(__noctisgames__GpuTextureDataWrapper__) */
