//
//  CaveSkeleton.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/30/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "CaveSkeleton.h"
#include "EntityUtils.h"

#define caveSkeletonTypeKey "caveSkeletonType"

CaveSkeleton CaveSkeleton::create(float x, float y, CaveSkeletonType type)
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

CaveSkeleton::CaveSkeleton(float x, float y, float width, float height, CaveSkeletonType type) : PhysicalEntity(x, y, width, height), m_caveSkeletonType(type)
{
    // Empty
}

CaveSkeletonType CaveSkeleton::getCaveSkeletonType()
{
    return m_caveSkeletonType;
}

CaveSkeleton CaveSkeleton::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    CaveSkeletonType type = (CaveSkeletonType) v[caveSkeletonTypeKey].GetInt();
    
    return CaveSkeleton(x, y, width, height, type);
}

void CaveSkeleton::serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w)
{
    w.String(caveSkeletonTypeKey);
    w.Int(getCaveSkeletonType());
}