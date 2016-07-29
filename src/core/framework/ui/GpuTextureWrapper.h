//
//  GpuTextureWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__GpuTextureWrapper__
#define __noctisgames__GpuTextureWrapper__

#ifdef NG_OPENGL_ES
extern "C"
{
#include "platform_gl.h"
}
#elif defined NG_DIRECT_3D
#include <d3d11_1.h>
#elif defined NG_3DS
#include <sf2d.h>
#endif

struct GpuTextureWrapper
{
#ifdef NG_OPENGL_ES
    GLuint texture;
    
    GpuTextureWrapper(GLuint textureIn) : texture(textureIn)
    {
        // Empty
    }
#elif defined NG_DIRECT_3D
    ID3D11ShaderResourceView *texture;
    
    GpuTextureWrapper(ID3D11ShaderResourceView *textureIn) : texture(textureIn)
    {
        // Empty
    }
#elif defined NG_3DS
    sf2d_texture *texture;
    
    GpuTextureWrapper(sf2d_texture *textureIn) : texture(textureIn)
    {
        // Empty
    }
#endif
};

#endif /* defined(__noctisgames__GpuTextureWrapper__) */
