//
//  Jon.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Jon__
#define __nosfuratu__Jon__

#include "PhysicalEntity.h"
#include "EntityAnchor.h"

class Game;

class Jon : public PhysicalEntity
{
public:
    Jon(float x, float y, float width, float height, EntityAnchor anchor = ANCHOR_NONE);
    
    virtual void update(float deltaTime, Game& game);
    
    void triggerJump();
    
    int getNumJumps();
    
    bool isGrounded();
    
    bool isFalling();
    
    bool isLanding();
    
    bool isDead();
    
private:
	float m_fMaxSpeed;
	float m_fAccelerationX;
    int m_iNumJumps;
    bool m_isGrounded;
    bool m_isLanding;
    bool m_isDead;
    
    void jump();
};

#endif /* defined(__nosfuratu__Jon__) */
