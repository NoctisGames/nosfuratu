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
    m_fRegionWidth = (float) regionWidth;
    m_fRegionHeight = (float) regionHeight;
    
    u1 = x / (float) textureWidth;
    v1 = y / (float) textureHeight;
    u2 = u1 + m_fRegionWidth / (float) textureWidth;
    v2 = v1 + m_fRegionHeight / (float) textureHeight;
}

void TextureRegion::init(int x, int regionWidth, int textureWidth)
{
    m_fRegionWidth = (float) regionWidth;
    
    u1 = x / (float) textureWidth;
    u2 = u1 + m_fRegionWidth / (float) textureWidth;
}
