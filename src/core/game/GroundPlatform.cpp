//
//  GroundPlatform.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "GroundPlatform.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "EntityUtils.h"

#define GROUND_PLATFORM_HEIGHT 1.1698440207972272f

void GroundPlatform::createGrass(std::vector<GroundPlatform>& platforms, float x, float y, int length)
{
    create(platforms, x, y, length, GROUND_PLATFORM_GRASS_END_LEFT, GROUND_PLATFORM_GRASS_CENTER, GROUND_PLATFORM_GRASS_END_RIGHT);
}

void GroundPlatform::createCave(std::vector<GroundPlatform>& platforms, float x, float y, int length)
{
    create(platforms, x, y, length, GROUND_PLATFORM_CAVE_END_LEFT, GROUND_PLATFORM_CAVE_CENTER, GROUND_PLATFORM_CAVE_END_RIGHT);
}

GroundPlatform GroundPlatform::create(float x, float y, GroundPlatformType type)
{
    switch (type)
    {
        case GroundPlatformType::GROUND_PLATFORM_GRASS_DEFAULT:
            return GroundPlatform(x, y, 2.736842105263158f, type);
        case GroundPlatformType::GROUND_PLATFORM_GRASS_END_LEFT:
            return GroundPlatform(x, y, 0.7017543859649122f, type);
        case GroundPlatformType::GROUND_PLATFORM_GRASS_CENTER:
            return GroundPlatform(x, y, 2.7134502923976607f, type);
        case GroundPlatformType::GROUND_PLATFORM_GRASS_END_RIGHT:
            return GroundPlatform(x, y, 0.7017543859649122f, type);
        case GroundPlatformType::GROUND_PLATFORM_CAVE_END_LEFT:
            return GroundPlatform(x, y, 0.7953216374269005f, type);
        case GroundPlatformType::GROUND_PLATFORM_CAVE_CENTER:
            return GroundPlatform(x, y, 2.7134502923976607f, type);
        case GroundPlatformType::GROUND_PLATFORM_CAVE_END_RIGHT:
        default:
            return GroundPlatform(x, y, 0.7719298245614035f, type);
    }
}

GroundPlatform::GroundPlatform(float x, float y, float width, GroundPlatformType type) : PhysicalEntity(x + width / 2, y, width, GROUND_PLATFORM_HEIGHT), m_groundPlatformType(type)
{
    resetBounds(width, GROUND_PLATFORM_HEIGHT * 0.86f);
}

GroundPlatformType GroundPlatform::getGroundPlatformType()
{
    return m_groundPlatformType;
}

#pragma mark private

void GroundPlatform::create(std::vector<GroundPlatform>& platforms, float x, float y, int length, GroundPlatformType typeLeft, GroundPlatformType typeCenter, GroundPlatformType typeRight)
{
    platforms.push_back(GroundPlatform::create(x, y, typeLeft));
    
    for (int i = 0; i < length; i++)
    {
        platforms.push_back(GroundPlatform::create(0, y, typeCenter));
        EntityUtils::attach(platforms.at(platforms.size() - 1), platforms.at(platforms.size() - 2), false);
    }
    
    platforms.push_back(GroundPlatform::create(0, y, typeRight));
    
    EntityUtils::attach(platforms.at(platforms.size() - 1), platforms.at(platforms.size() - 2), false);
}