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

#define GROUND_GRASS_WITHOUT_CAVE_HEIGHT 10.224436741767764f
#define GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR 0.88329519450801f

#define GROUND_CAVE_HEIGHT 4.726169844020797f
#define GROUND_CAVE_BOUNDS_HEIGHT_FACTOR 0.25742574257426f

#define GROUND_CAVE_RAISED_HEIGHT 6.34055459272097f
#define GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR 0.84444444444444f

void Ground::createGrassWithCave(std::vector<Ground>& grounds, float x, GroundSize gs, int length)
{
    GroundType gt = gs == GROUND_SIZE_LARGE ? GROUND_GRASS_WITH_CAVE_LARGE : gs == GROUND_SIZE_MEDIUM ? GROUND_GRASS_WITH_CAVE_MEDIUM : GROUND_GRASS_WITH_CAVE_SMALL;
    create(grounds, x, length, GROUND_GRASS_WITH_CAVE_END_LEFT, gt, GROUND_GRASS_WITH_CAVE_END_RIGHT);
}

void Ground::createGrassWithoutCave(std::vector<Ground>& grounds, float x, GroundSize gs, int length)
{
    GroundType gt = gs == GROUND_SIZE_LARGE ? GROUND_GRASS_WITHOUT_CAVE_LARGE : gs == GROUND_SIZE_MEDIUM ? GROUND_GRASS_WITHOUT_CAVE_MEDIUM : GROUND_GRASS_WITHOUT_CAVE_SMALL;
    create(grounds, x, length, GROUND_GRASS_WITHOUT_CAVE_END_LEFT, gt, GROUND_GRASS_WITHOUT_CAVE_END_RIGHT);
}

void Ground::createCave(std::vector<Ground>& grounds, float x, GroundSize gs, int length)
{
    GroundType gt = gs == GROUND_SIZE_LARGE ? GROUND_CAVE_LARGE : gs == GROUND_SIZE_MEDIUM ? GROUND_CAVE_MEDIUM : GROUND_CAVE_SMALL;
    create(grounds, x, length, GROUND_CAVE_END_LEFT, gt, GROUND_CAVE_END_RIGHT);
}

void Ground::createCaveRaised(std::vector<Ground>& grounds, float x, GroundSize gs, int length)
{
    GroundType gt = gs == GROUND_SIZE_LARGE ? GROUND_CAVE_RAISED_LARGE : gs == GROUND_SIZE_MEDIUM ? GROUND_CAVE_RAISED_MEDIUM : GROUND_CAVE_RAISED_SMALL;
    create(grounds, x, length, GROUND_CAVE_RAISED_END_LEFT, gt, GROUND_CAVE_RAISED_END_RIGHT);
}

Ground Ground::create(float x, GroundType groundType)
{
    switch (groundType)
    {
        case GroundType::GROUND_GRASS_WITH_CAVE_LARGE:
            return Ground(x, GROUND_LARGE_WIDTH, GROUND_GRASS_WITH_CAVE_HEIGHT, groundType, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, EntityAnchor::ANCHOR_GROUND_WITH_CAVE);
        case GroundType::GROUND_CAVE_LARGE:
            return Ground(x, GROUND_LARGE_WIDTH, GROUND_CAVE_HEIGHT, groundType, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_LARGE:
            return Ground(x, GROUND_LARGE_WIDTH, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, groundType, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR);
        
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_END_LEFT:
            return Ground(x, 1.5204678362573099f, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, groundType, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_MEDIUM:
            return Ground(x, GROUND_MEDIUM_WIDTH, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, groundType, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_SMALL:
            return Ground(x, GROUND_SMALL_WIDTH, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, groundType, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_END_RIGHT:
            return Ground(x, 1.6140350877192982f, GROUND_GRASS_WITHOUT_CAVE_HEIGHT, groundType, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR);
        
        case GroundType::GROUND_GRASS_WITH_CAVE_END_LEFT:
            return Ground(x, 1.7309941520467835f, GROUND_GRASS_WITH_CAVE_HEIGHT, groundType, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, EntityAnchor::ANCHOR_GROUND_WITH_CAVE);
        case GroundType::GROUND_GRASS_WITH_CAVE_MEDIUM:
            return Ground(x, GROUND_MEDIUM_WIDTH, GROUND_GRASS_WITH_CAVE_HEIGHT, groundType, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, EntityAnchor::ANCHOR_GROUND_WITH_CAVE);
        case GroundType::GROUND_GRASS_WITH_CAVE_SMALL:
            return Ground(x, GROUND_SMALL_WIDTH, GROUND_GRASS_WITH_CAVE_HEIGHT, groundType, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, EntityAnchor::ANCHOR_GROUND_WITH_CAVE);
        case GroundType::GROUND_GRASS_WITH_CAVE_END_RIGHT:
            return Ground(x, 1.6140350877192982f, GROUND_GRASS_WITH_CAVE_HEIGHT, groundType, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, EntityAnchor::ANCHOR_GROUND_WITH_CAVE);
        
        case GroundType::GROUND_CAVE_END_LEFT:
            return Ground(x, 1.567251461988304f, GROUND_CAVE_HEIGHT, groundType, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_CAVE_MEDIUM:
            return Ground(x, GROUND_MEDIUM_WIDTH, GROUND_CAVE_HEIGHT, groundType, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_CAVE_SMALL:
            return Ground(x, GROUND_SMALL_WIDTH, GROUND_CAVE_HEIGHT, groundType, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_CAVE_END_RIGHT:
            return Ground(x, 1.543859649122807f, GROUND_CAVE_HEIGHT, groundType, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR);
        
        case GroundType::GROUND_CAVE_RAISED_END_LEFT:
            return Ground(x, 2.198830409356725f, GROUND_CAVE_RAISED_HEIGHT, groundType, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_CAVE_RAISED_MEDIUM:
            return Ground(x, GROUND_MEDIUM_WIDTH, GROUND_CAVE_RAISED_HEIGHT, groundType, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_CAVE_RAISED_SMALL:
            return Ground(x, GROUND_SMALL_WIDTH, GROUND_CAVE_RAISED_HEIGHT, groundType, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR);
        case GroundType::GROUND_CAVE_RAISED_END_RIGHT:
            return Ground(x, 2.198830409356725f, GROUND_CAVE_RAISED_HEIGHT, groundType, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR);
        
        case GroundType::GROUND_CAVE_RAISED_LARGE:
        default:
            return Ground(x, GROUND_LARGE_WIDTH, GROUND_CAVE_RAISED_HEIGHT, groundType, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR);
    }
}

Ground::Ground(float x, float width, float height, GroundType groundType, float boundsHeightFactor, EntityAnchor anchor) : PhysicalEntity(x + width / 2, 0, width, height), m_groundType(groundType)
{
    EntityUtils::applyAnchor(*this, anchor);
    
    updateBounds();
    
    m_bounds->setHeight(height * boundsHeightFactor);
}

GroundType Ground::getGroundType()
{
    return m_groundType;
}

#pragma mark private

void Ground::create(std::vector<Ground>& grounds, float x, int length, GroundType typeLeft, GroundType typeCenter, GroundType typeRight)
{
    grounds.push_back(Ground::create(x, typeLeft));
    
    for (int i = 0; i < length; i++)
    {
        grounds.push_back(Ground::create(0, typeCenter));
        EntityUtils::attach(grounds.at(grounds.size() - 1), grounds.at(grounds.size() - 2), false);
    }
    
    grounds.push_back(Ground::create(0, typeRight));
    
    EntityUtils::attach(grounds.at(grounds.size() - 1), grounds.at(grounds.size() - 2), false);
}