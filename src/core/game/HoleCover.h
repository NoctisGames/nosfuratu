//
//  HoleCover.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__HoleCover__
#define __nosfuratu__HoleCover__

#include "PhysicalEntity.h"

typedef enum
{
    HoleCoverType_Grass,
    HoleCoverType_Cave
} HoleCoverType;

class HoleCover : public PhysicalEntity
{
public:
    HoleCover(float x, float y, float width, float height, HoleCoverType type);
    
    virtual void update(float deltaTime);
    
    void triggerHit();
    
    HoleCoverType getType();
    
private:
    HoleCoverType m_type;
    bool m_isBreaking;
};

#endif /* defined(__nosfuratu__HoleCover__) */
