//
//  CaveSkeleton.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/30/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "CaveSkeleton.h"
#include "EntityUtils.h"

CaveSkeleton CaveSkeleton::createCaveSkeleton(float x, float y, CaveSkeletonType type)
{
    switch (type)
    {
        case CAVE_SKELETON_ONE:
            return CaveSkeleton(x, y, 2.9239766081871346f, 1.7781629116117852f, type);
        case CAVE_SKELETON_TWO:
            return CaveSkeleton(x, y, 3.064327485380117f, 2.3864818024263434f, type);
        case CAVE_SKELETON_THREE:
        default:
            return CaveSkeleton(x, y, 3.064327485380117f, 2.3864818024263434f, type);
    }
}

CaveSkeleton::CaveSkeleton(float x, float y, float width, float height, CaveSkeletonType type) : PhysicalEntity(x + width / 2, y, width, height), m_caveSkeletonType(type)
{
    // Empty
}

CaveSkeletonType CaveSkeleton::getCaveSkeletonType()
{
    return m_caveSkeletonType;
}