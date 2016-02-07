//
//  TextureRegion.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__TextureRegion__
#define __gowengamedev__TextureRegion__

class TextureRegion
{
public:
    TextureRegion(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight);
    
    void init(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight);
    
    void init(int x, int regionWidth, int textureWidth);
    
	float u1, v1, u2, v2;
};

#endif /* defined(__gowengamedev__TextureRegion__) */
