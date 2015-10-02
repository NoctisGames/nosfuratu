//
//  CaveSkeleton.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/30/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__CaveSkeleton__
#define __nosfuratu__CaveSkeleton__

#include "PhysicalEntity.h"
#include "CaveSkeletonType.h"

class CaveSkeleton : public PhysicalEntity
{
public:
    static CaveSkeleton create(float x, float y, CaveSkeletonType caveSkeletonType);
    
    CaveSkeleton(float x, float y, float width, float height, CaveSkeletonType caveSkeletonType);
    
    CaveSkeletonType getCaveSkeletonType();
    
    static CaveSkeleton deserialize(rapidjson::Value& v);
    
    virtual void serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w);
    
private:
    CaveSkeletonType m_caveSkeletonType;
};

#endif /* defined(__nosfuratu__CaveSkeleton__) */
