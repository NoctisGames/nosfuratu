//
//  Ground.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Ground__
#define __nosfuratu__Ground__

#include "PhysicalEntity.h"
#include "GroundType.h"
#include "EntityAnchor.h"

class Ground : public PhysicalEntity
{
public:
    static Ground createGround(float x, GroundType groundType);
    
    Ground(float x, float width, float height, GroundType groundType, float boundsHeightFactor, EntityAnchor anchor = ANCHOR_BOTTOM);
    
    GroundType getGroundType();
    
private:
    GroundType m_groundType;
};

#endif /* defined(__nosfuratu__Ground__) */
