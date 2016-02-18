//
//  TextureWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__TextureWrapper__
#define __noctisgames__TextureWrapper__

#ifdef GGD_OPENGL_ES
extern "C"
{
#include "platform_gl.h"
}
#elif defined GGD_DIRECT_3D
#include <d3d11_1.h>
#elif defined GGD_3DS
#include <sf2d.h>
#endif

struct TextureWrapper
{
#ifdef GGD_OPENGL_ES
    GLuint texture;
    
    TextureWrapper(GLuint textureIn) : texture(textureIn)
    {
        // Empty
    }
#elif defined GGD_DIRECT_3D
    ID3D11ShaderResourceView *texture;
    
    TextureWrapper(ID3D11ShaderResourceView *textureIn) : texture(textureIn)
    {
        // Empty
    }
#elif defined GGD_3DS
    sf2d_texture *texture;
    
    TextureWrapper(sf2d_texture *textureIn) : texture(textureIn)
    {
        // Empty
    }
#endif
};

#endif /* defined(__noctisgames__TextureWrapper__) */
