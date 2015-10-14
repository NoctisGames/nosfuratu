//
//  CaveSkeleton.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/30/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "CaveSkeleton.h"
#include "EntityUtils.h"

CaveSkeleton* CaveSkeleton::create(float x, float y, int type)
{
    switch ((CaveSkeletonType) type)
    {
        case CaveSkeletonType_One:
            return new CaveSkeletonOne(x, y);
        case CaveSkeletonType_Two:
            return new CaveSkeletonTwo(x, y);
        case CaveSkeletonType_Three:
        default:
            return new CaveSkeletonThree(x, y);
    }
}

CaveSkeleton::CaveSkeleton(float x, float y, float width, float height, CaveSkeletonType type) : PhysicalEntity(x, y, width, height), m_type(type)
{
    // Empty
}

CaveSkeletonType CaveSkeleton::getEnumType()
{
    return m_type;
}

int CaveSkeleton::getType()
{
    return m_type;
}