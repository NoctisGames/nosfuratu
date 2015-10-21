//
//  HoleCover.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__HoleCover__
#define __nosfuratu__HoleCover__

#include "DestructiblePhysicalEntity.h"

class HoleCover : public DestructiblePhysicalEntity
{
public:
    HoleCover(float x, float y = 8.095320623916809f, float width = 3.2046783625730995f, float height = 2.246100519930676f);
    
    virtual void update(float deltaTime);
    
    virtual void triggerHit();
    
private:
    bool m_isBreaking;
};

#endif /* defined(__nosfuratu__HoleCover__) */
