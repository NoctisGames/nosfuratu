//
//  DestructiblePhysicalEntity.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__DestructiblePhysicalEntity__
#define __nosfuratu__DestructiblePhysicalEntity__

#include "PhysicalEntity.h"

class DestructiblePhysicalEntity : public PhysicalEntity
{
public:
    DestructiblePhysicalEntity(float x, float y, float width, float height);
    
    virtual void triggerHit();
    
    virtual bool isDestroyed();
    
private:
    bool m_isDestroyed;
};

#endif /* defined(__nosfuratu__DestructiblePhysicalEntity__) */
