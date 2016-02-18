//
//  TextureRegion.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__TextureRegion__
#define __noctisgames__TextureRegion__

class TextureRegion
{
public:
    TextureRegion(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight);
    
    void init(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight);
    
    void init(int x, int regionWidth, int textureWidth);
    
	float u1, v1, u2, v2;
};

#endif /* defined(__noctisgames__TextureRegion__) */
