//
//  Ground.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Ground.h"
#include "GameConstants.h"
#include "EntityUtils.h"

#define GROUND_LARGE_WIDTH 47.90643274853801f
#define GROUND_MEDIUM_WIDTH 23.976608187134502f
#define GROUND_SMALL_WIDTH 12.0f

#define GROUND_GRASS_WITH_CAVE_HEIGHT 2.643847487001733f
#define GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR 0.54867256637168f
#define GROUND_GRASS_WITH_CAVE_Y 8.9025129982669

#define GROUND_GRASS_WITHOUT_CAVE_HEIGHT 10.224436741767764f
#define GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR 0.88329519450801f

#define GROUND_CAVE_HEIGHT 4.726169844020797f
#define GROUND_CAVE_BOUNDS_HEIGHT_FACTOR 0.25742574257426f

#define GROUND_CAVE_RAISED_HEIGHT 6.34055459272097f
#define GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR 0.84444444444444f

#define groundTypeKey "groundType"
#define boundsHeightFactorKey "boundsHeightFactor"

void Ground::create(std::vector<Ground>& items, float x, GroundType type)
{
    EntityAnchor anchor = ANCHOR_BOTTOM;
    switch (type)
    {
        case GroundType::GROUND_GRASS_WITH_CAVE_LARGE:
            items.push_back(Ground(x, GROUND_GRASS_WITH_CAVE_Y, GROUND_LARGE_WIDTH, GROUND_GRASS_WITH_CAVE_HEIGHT, type, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR));
            anchor = EntityAnchor::ANCHOR_NONE;
            break;
        case GroundType::GROUND_CAVE_LARGE:
            items.push_back(Ground(x, 0, GROUND_LARGE_WIDTH, GROUND_CAVE_HEIGHT, type, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_LARGE:
            items.push_back(Ground(x, 0, GROUND_LARGE_WIDTH, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, type, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_END_LEFT:
            items.push_back(Ground(x, 0, 1.5204678362573099f, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, type, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_MEDIUM:
            items.push_back(Ground(x, 0, GROUND_MEDIUM_WIDTH, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, type, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_SMALL:
            items.push_back(Ground(x, 0, GROUND_SMALL_WIDTH, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, type, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_END_RIGHT:
            items.push_back(Ground(x, 0, 1.6140350877192982f, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, type, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        
        case GroundType::GROUND_GRASS_WITH_CAVE_END_LEFT:
            items.push_back(Ground(x, GROUND_GRASS_WITH_CAVE_Y, 1.7309941520467835f, GROUND_GRASS_WITH_CAVE_HEIGHT, type, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR));
            anchor = EntityAnchor::ANCHOR_NONE;
            break;
        case GroundType::GROUND_GRASS_WITH_CAVE_MEDIUM:
            items.push_back(Ground(x, GROUND_GRASS_WITH_CAVE_Y, GROUND_MEDIUM_WIDTH, GROUND_GRASS_WITH_CAVE_HEIGHT, type, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR));
            anchor = EntityAnchor::ANCHOR_NONE;
            break;
        case GroundType::GROUND_GRASS_WITH_CAVE_SMALL:
            items.push_back(Ground(x, GROUND_GRASS_WITH_CAVE_Y, GROUND_SMALL_WIDTH, GROUND_GRASS_WITH_CAVE_HEIGHT, type, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR));
            anchor = EntityAnchor::ANCHOR_NONE;
            break;
        case GroundType::GROUND_GRASS_WITH_CAVE_END_RIGHT:
            items.push_back(Ground(x, GROUND_GRASS_WITH_CAVE_Y, 1.6140350877192982f, GROUND_GRASS_WITH_CAVE_HEIGHT, type, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR));
            anchor = EntityAnchor::ANCHOR_NONE;
            break;
        
        case GroundType::GROUND_CAVE_END_LEFT:
            items.push_back(Ground(x, 0, 1.567251461988304f, GROUND_CAVE_HEIGHT, type, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_CAVE_MEDIUM:
            items.push_back(Ground(x, 0, GROUND_MEDIUM_WIDTH, GROUND_CAVE_HEIGHT, type, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_CAVE_SMALL:
            items.push_back(Ground(x, 0, GROUND_SMALL_WIDTH, GROUND_CAVE_HEIGHT, type, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_CAVE_END_RIGHT:
            items.push_back(Ground(x, 0, 1.543859649122807f, GROUND_CAVE_HEIGHT, type, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR));
            break;
        
        case GroundType::GROUND_CAVE_RAISED_END_LEFT:
            items.push_back(Ground(x, 0, 2.198830409356725f, GROUND_CAVE_RAISED_HEIGHT, type, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_CAVE_RAISED_MEDIUM:
            items.push_back(Ground(x, 0, GROUND_MEDIUM_WIDTH, GROUND_CAVE_RAISED_HEIGHT, type, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_CAVE_RAISED_SMALL:
            items.push_back(Ground(x, 0, GROUND_SMALL_WIDTH, GROUND_CAVE_RAISED_HEIGHT, type, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR));
            break;
        case GroundType::GROUND_CAVE_RAISED_END_RIGHT:
            items.push_back(Ground(x, 0, 2.198830409356725f, GROUND_CAVE_RAISED_HEIGHT, type, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR));
            break;
        
        case GroundType::GROUND_CAVE_RAISED_LARGE:
        default:
            items.push_back(Ground(x, 0, GROUND_LARGE_WIDTH, GROUND_CAVE_RAISED_HEIGHT, type, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR));
            break;
    }
    
    EntityUtils::applyAnchor(items.at(items.size() - 1), anchor);
    
    items.at(items.size() - 1).updateBounds();
}

Ground::Ground(float x, float y, float width, float height, GroundType type, float boundsHeightFactor) : PhysicalEntity(x, y, width, height), m_groundType(type), m_fBoundsHeightFactor(boundsHeightFactor)
{
    updateBounds();
}

void Ground::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * m_fBoundsHeightFactor);
}

GroundType Ground::getGroundType()
{
    return m_groundType;
}

float Ground::getBoundsHeightFactor()
{
    return m_fBoundsHeightFactor;
}

Ground Ground::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    GroundType type = (GroundType) v[groundTypeKey].GetInt();
    float boundsHeightFactor = v[boundsHeightFactorKey].GetDouble();
    
    return Ground(x, y, width, height, type, boundsHeightFactor);
}

void Ground::serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w)
{
    w.String(groundTypeKey);
    w.Int(getGroundType());
    
    w.String(boundsHeightFactorKey);
    w.Double(m_fBoundsHeightFactor);
}