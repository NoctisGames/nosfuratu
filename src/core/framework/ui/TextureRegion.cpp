//
//  TextureRegion.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "TextureRegion.h"

TextureRegion::TextureRegion(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight)
{
    init(x, y, regionWidth, regionHeight, textureWidth, textureHeight);
}

void TextureRegion::init(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight)
{
    u1 = x / (float) textureWidth;
    v1 = y / (float) textureHeight;
    u2 = u1 + regionWidth / (float) textureWidth;
    v2 = v1 + regionHeight / (float) textureHeight;
}

void TextureRegion::init(int x, int regionWidth, int textureWidth)
{
    u1 = x / (float) textureWidth;
    u2 = u1 + regionWidth / (float) textureWidth;
}