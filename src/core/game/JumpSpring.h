//
//  JumpSpring.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__JumpSpring__
#define __nosfuratu__JumpSpring__

#include "PhysicalEntity.h"
#include "JumpSpringType.h"
#include "EntityAnchor.h"

class JumpSpring : public PhysicalEntity
{
public:
    static JumpSpring create(float x, JumpSpringType type);
    
    JumpSpring(float x, float width, float height, JumpSpringType type, EntityAnchor anchor = EntityAnchor::ANCHOR_NONE);
    
    virtual void update(float deltaTime);
    
    void trigger();
    
    JumpSpringType getJumpSpringType();
    
    bool isTriggered();
    
private:
    JumpSpringType m_jumpSpringType;
    bool m_isTriggered;
};

#endif /* defined(__nosfuratu__JumpSpring__) */
