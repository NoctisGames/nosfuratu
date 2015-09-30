//
//  Rock.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Rock__
#define __nosfuratu__Rock__

#include "DestructiblePhysicalEntity.h"
#include "EntityAnchor.h"
#include "Color.h"

class Rock : public DestructiblePhysicalEntity
{
public:
    Rock(float x, bool isCracked = false, EntityAnchor anchor = EntityAnchor::ANCHOR_GROUND);
    
    virtual void update(float deltaTime);
    
    virtual void triggerHit();
    
    virtual void updateBounds();
    
    Color getColor();
    
    bool isCracked();
    
    bool isBlowingUp();
    
private:
    Color m_color;
    bool m_isCracked;
    bool m_isBlowingUp;
};

#endif /* defined(__nosfuratu__Rock__) */
