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

#define groundPlatformTypeKey "groundPlatformType"

void GroundPlatform::create(std::vector<GroundPlatform>& items, float x, float y, GroundPlatformType type)
{
    switch (type)
    {
        case GroundPlatformType::GROUND_PLATFORM_GRASS_DEFAULT:
            items.push_back(GroundPlatform(x, y, 2.736842105263158f, GROUND_PLATFORM_HEIGHT, type));
            break;
        case GroundPlatformType::GROUND_PLATFORM_GRASS_END_LEFT:
            items.push_back(GroundPlatform(x, y, 0.7017543859649122f, GROUND_PLATFORM_HEIGHT, type));
            break;
        case GroundPlatformType::GROUND_PLATFORM_GRASS_CENTER:
            items.push_back(GroundPlatform(x, y, 2.7134502923976607f, GROUND_PLATFORM_HEIGHT, type));
            break;
        case GroundPlatformType::GROUND_PLATFORM_GRASS_END_RIGHT:
            items.push_back(GroundPlatform(x, y, 0.7017543859649122f, GROUND_PLATFORM_HEIGHT, type));
            break;
        case GroundPlatformType::GROUND_PLATFORM_CAVE_END_LEFT:
            items.push_back(GroundPlatform(x, y, 0.7953216374269005f, GROUND_PLATFORM_HEIGHT, type));
            break;
        case GroundPlatformType::GROUND_PLATFORM_CAVE_CENTER:
            items.push_back(GroundPlatform(x, y, 2.7134502923976607f, GROUND_PLATFORM_HEIGHT, type));
            break;
        case GroundPlatformType::GROUND_PLATFORM_CAVE_END_RIGHT:
        default:
            items.push_back(GroundPlatform(x, y, 0.7719298245614035f, GROUND_PLATFORM_HEIGHT, type));
            break;
    }
}

GroundPlatform::GroundPlatform(float x, float y, float width, float height, GroundPlatformType type) : PhysicalEntity(x, y, width, height), m_groundPlatformType(type)
{
    updateBounds();
}

void GroundPlatform::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * 0.80f);
}

GroundPlatformType GroundPlatform::getGroundPlatformType()
{
    return m_groundPlatformType;
}

GroundPlatform GroundPlatform::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    GroundPlatformType type = (GroundPlatformType) v[groundPlatformTypeKey].GetInt();
    
    return GroundPlatform(x, y, width, height, type);
}

void GroundPlatform::serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w)
{
    w.String(groundPlatformTypeKey);
    w.Int(getGroundPlatformType());
}