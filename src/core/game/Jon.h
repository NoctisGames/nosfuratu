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

class Game;

class Jon : public PhysicalEntity
{
public:
    Jon(float x, float y, float width, float height);
    
    virtual void update(float deltaTime, Game& game);
    
    void triggerJump();
    
    int getNumJumps();
    
    bool isFalling();
    
    bool isDead();
    
private:
	float m_fSpeedBoostTime;
    int m_iNumJumps;
    bool m_isGrounded;
    bool m_isSpeedBoost;
    bool m_isDead;
    
    void jump();
    
    bool isGrounded(Game& game);
};

#endif /* defined(__nosfuratu__Jon__) */
