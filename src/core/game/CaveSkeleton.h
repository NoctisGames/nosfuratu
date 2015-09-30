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
#include "EntityAnchor.h"

class CaveSkeleton : public PhysicalEntity
{
public:
    static CaveSkeleton createCaveSkeleton(float x, float y, CaveSkeletonType caveSkeletonType);
    
    CaveSkeleton(float x, float y, float width, float height, CaveSkeletonType caveSkeletonType);
    
    CaveSkeletonType getCaveSkeletonType();
    
private:
    CaveSkeletonType m_caveSkeletonType;
};

#endif /* defined(__nosfuratu__CaveSkeleton__) */
