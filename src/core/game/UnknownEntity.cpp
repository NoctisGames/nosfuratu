//
//  UnknownEntity.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 11/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "UnknownEntity.h"
#include "Assets.h"

#define PIXEL_WIDTH_FOR_GAME 1808.0f
#define PIXEL_HEIGHT_FOR_GAME 4096.0f

UnknownEntity* UnknownEntity::create(std::string assetId, std::string textureName, float x, float y)
{
    TextureRegion& tr = ASSETS->findTextureRegion(assetId, 0);
    
    float pixelWidth = tr.m_fRegionWidth;
    float pixelHeight = tr.m_fRegionHeight;
    
    double width = pixelWidth / PIXEL_WIDTH_FOR_GAME * CAM_WIDTH;
    double height = pixelHeight / PIXEL_HEIGHT_FOR_GAME * GAME_HEIGHT;
    
    return new UnknownEntity(assetId, textureName, x, y, width, height);
}

UnknownEntity::UnknownEntity(std::string assetId, std::string textureName, float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_textureName(textureName)
{
    m_assetId = assetId;
}

RTTI_IMPL(UnknownEntity, PhysicalEntity);
