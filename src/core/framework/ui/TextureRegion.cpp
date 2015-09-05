//
//  TextureRegion.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
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