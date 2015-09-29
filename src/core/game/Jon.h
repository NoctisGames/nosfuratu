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
#include "JonState.h"
#include "JonPhysicalState.h"
#include "JonActionState.h"
#include "JonAbilityState.h"
#include "Color.h"

class Game;

class Jon : public PhysicalEntity
{
public:
    Jon(float x, float y, float width, float height, EntityAnchor anchor = ANCHOR_NONE);
    
    void update(float deltaTime, Game& game);
    
    void triggerJump();
    
    void triggerLeftAction();
    
    void triggerRightAction();
    
    void triggerUpAction();
    
    void triggerDownAction();
    
    JonState getState();
    
    JonPhysicalState getPhysicalState();
    
    JonActionState getActionState();
    
    JonAbilityState getAbilityState();
    
    Color getColor();
    
    float getActionStateTime();
    
    float getAbilityStateTime();
    
    bool isFalling();
    
    bool isLanding();
    
    bool isDead();
    
private:
    JonState m_state;
    JonPhysicalState m_physicalState;
    JonActionState m_actionState;
    JonAbilityState m_abilityState;
    Color m_color;
    float m_fActionStateTime;
    float m_fAbilityStateTime;
	float m_fMaxSpeed;
	float m_fAccelerationX;
    int m_iNumJumps;
    bool m_isLanding;
    bool m_isSpinningBackFistDelivered;
    
    void jump();
    
    void setState(JonState state);
    
    void setState(JonPhysicalState state);
    
    void setState(JonActionState state);
    
    void setState(JonAbilityState state);
};

#endif /* defined(__nosfuratu__Jon__) */
