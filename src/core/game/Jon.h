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
#include "JonState.h"
#include "JonPhysicalState.h"
#include "JonActionState.h"
#include "JonAbilityState.h"
#include "Color.h"
#include "DustCloud.h"
#include "GroundSoundType.h"
#include "StateMachine.h"
#include "State.h"
#include "JonFormState.h"

#include <memory>
#include <vector>

class Game;

class Jon : public PhysicalEntity
{
public:
    static Jon* create(float x, float y, int type);
    
    Jon(float x, float y, float width = 2.2f, float height = 2.2f);
    
    void update(float deltaTime);
    
    void triggerTransform();
    
    void triggerJump();
    
    void triggerLeftAction();
    
    void triggerRightAction();
    
    void triggerUpAction();
    
    void triggerDownAction();
    
    std::vector<std::unique_ptr<DustCloud>>& getDustClouds();
    
    JonState getState();
    
    JonPhysicalState getPhysicalState();
    
    JonActionState getActionState();
    
    JonAbilityState getAbilityState();
    
    Color getColor();
    
    float getActionStateTime();
    
    float getAbilityStateTime();
    
    bool isMoving();
    
    bool isPushedBack();
    
    bool isFalling();
    
    bool isLanding();
    
    bool isDead();
    
    int getType();
    
    bool isRightFoot();
    
    void setRightFoot(bool isRightFoot);
    
    GroundSoundType getGroundSoundType();
    
    void setGroundSoundType(GroundSoundType groundSoundType);
    
    void setBoostVelocity(int boostVelocity);
    
    bool isVampire();
    
    void setAllowedToMove(bool isAllowedToMove);
    
    bool isAllowedToMove();
    
    void setGame(Game* game);
    
private:
    std::unique_ptr<StateMachine<Jon>> m_formStateMachine;
    Game* m_game;
    std::vector<std::unique_ptr<DustCloud>> m_dustClouds;
    JonState m_state;
    JonPhysicalState m_physicalState;
    JonActionState m_actionState;
    JonAbilityState m_abilityState;
    GroundSoundType m_groundSoundType;
    Color m_color;
    float m_fDeltaTime;
    float m_fActionStateTime;
    float m_fAbilityStateTime;
	float m_fDefaultMaxSpeed;
    float m_fMaxSpeed;
	float m_fAccelerationX;
    float m_fGravity;
    int m_iNumJumps;
    int m_iBoostVelocity;
    bool m_isLanding;
    bool m_isRightFoot;
    bool m_isAllowedToMove;
    
    void setState(JonState state);
    
    void setState(JonPhysicalState state);
    
    void setState(JonActionState state);
    
    void setState(JonAbilityState state);
    
    class Rabbit : public JonFormState
    {
    public:
        static Rabbit* getInstance();
        
        virtual void enter(Jon* jon);
        virtual void execute(Jon* jon);
        virtual void exit(Jon* jon);
        
        virtual void triggerTransform(Jon* jon);
        
        virtual void triggerJump(Jon* jon);
        virtual void triggerLeftAction(Jon* jon);
        virtual void triggerRightAction(Jon* jon);
        virtual void triggerUpAction(Jon* jon);
        virtual void triggerDownAction(Jon* jon);
        
    private:
        bool m_isSpinningBackFistDelivered;
        bool m_isBurrowEffective;
        
        // ctor, copy ctor, and assignment should be private in a Singleton
        Rabbit();
        Rabbit(const Rabbit&);
        Rabbit& operator=(const Rabbit&);
    };
    
    class Vampire : public JonFormState
    {
    public:
        static Vampire* getInstance();
        
        virtual void enter(Jon* jon);
        virtual void execute(Jon* jon);
        virtual void exit(Jon* jon);
        
        virtual void triggerTransform(Jon* jon);
        
        virtual void triggerJump(Jon* jon);
        virtual void triggerLeftAction(Jon* jon);
        virtual void triggerRightAction(Jon* jon);
        virtual void triggerUpAction(Jon* jon);
        virtual void triggerDownAction(Jon* jon);
        
    private:
        bool m_isFallingAfterGlide;
        
        // ctor, copy ctor, and assignment should be private in a Singleton
        Vampire();
        Vampire(const Vampire&);
        Vampire& operator=(const Vampire&);
    };
};

#endif /* defined(__nosfuratu__Jon__) */
