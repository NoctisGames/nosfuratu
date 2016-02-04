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

#include <memory>
#include <vector>

class Game;

class Jon : public PhysicalEntity
{
public:
    static Jon* create(float x, float y, int type);
    
    Jon(float x, float y, float width = 2.2f, float height = 2.2f);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    virtual void onDeletion();
    
    void triggerTransform();
    
    void triggerCancelTransform();
    
    void triggerJump();
    
    void triggerLeftAction();
    
    void triggerRightAction();
    
    void triggerUpAction();
    
    void triggerDownAction();
    
    std::vector<std::unique_ptr<DustCloud>>& getDustClouds();
    
    std::vector<std::unique_ptr<Jon>>& getAfterImages();
    
    JonState getState();
    
    JonPhysicalState getPhysicalState();
    
    JonActionState getActionState();
    
    JonAbilityState getAbilityState();
    
    Color getColor();
    
    float getActionStateTime();
    
    float getAbilityStateTime();
    
    float getTransformStateTime();
    
    float getDyingStateTime();
    
    bool isMoving();
    
    bool isPushedBack();
    
    bool isFalling();
    
    bool isLanding();
    
    bool isAlive();
    
    bool isDying();
    
    bool isDead();
    
    int getType();
    
    bool isRightFoot();
    
    void setRightFoot(bool isRightFoot);
    
    GroundSoundType getGroundSoundType();
    
    void setGroundSoundType(GroundSoundType groundSoundType);
    
    void setBoostVelocity(int boostVelocity);
    
    bool isVampire();
    
    bool isTransformingIntoVampire();
    
    bool isRevertingToRabbit();
    
    void setAllowedToMove(bool isAllowedToMove);
    
    bool isAllowedToMove();
    
    void setGame(Game* game);
    
    void beginWarmingUp();
    
private:
    std::unique_ptr<StateMachine<Jon>> m_formStateMachine;
    Game* m_game;
    std::vector<std::unique_ptr<DustCloud>> m_dustClouds;
    std::vector<std::unique_ptr<Jon>> m_afterImages;
    JonState m_state;
    JonPhysicalState m_physicalState;
    JonActionState m_actionState;
    JonAbilityState m_abilityState;
    GroundSoundType m_groundSoundType;
    Color m_color;
    float m_fDeltaTime;
    float m_fActionStateTime;
    float m_fAbilityStateTime;
    float m_fTransformStateTime;
    float m_fDyingStateTime;
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
    
    class JonFormState : public State<Jon>
    {
    public:
        virtual void enter(Jon* jon) = 0;
        virtual void execute(Jon* jon) = 0;
        virtual void exit(Jon* jon) = 0;
        
        virtual void triggerTransform(Jon* jon) = 0;
        virtual void triggerCancelTransform(Jon* jon) = 0;
        
        virtual void triggerJump(Jon* jon) = 0;
        virtual void triggerLeftAction(Jon* jon) = 0;
        virtual void triggerRightAction(Jon* jon) = 0;
        virtual void triggerUpAction(Jon* jon) = 0;
        virtual void triggerDownAction(Jon* jon) = 0;
        
        JonFormState() {};
        
    private:
        // ctor, copy ctor, and assignment should be private in a Singleton
        JonFormState(const JonFormState&);
        JonFormState& operator=(const JonFormState&);
    };
    
    class Rabbit : public JonFormState
    {
    public:
        static Rabbit* getInstance();
        
        virtual void enter(Jon* jon);
        virtual void execute(Jon* jon);
        virtual void exit(Jon* jon);
        
        virtual void triggerTransform(Jon* jon);
        virtual void triggerCancelTransform(Jon* jon) {};
        
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
        virtual void triggerCancelTransform(Jon* jon) {};
        
        virtual void triggerJump(Jon* jon);
        virtual void triggerLeftAction(Jon* jon);
        virtual void triggerRightAction(Jon* jon);
        virtual void triggerUpAction(Jon* jon);
        virtual void triggerDownAction(Jon* jon);
        
    private:
        std::unique_ptr<Vector2D> m_lastKnownVelocity;
        float m_fTimeSinceLastVelocityCheck;
        bool m_isFallingAfterGlide;
        
        // ctor, copy ctor, and assignment should be private in a Singleton
        Vampire();
        Vampire(const Vampire&);
        Vampire& operator=(const Vampire&);
    };
    
    class RabbitToVampire : public JonFormState
    {
    public:
        static RabbitToVampire* getInstance();
        
        virtual void enter(Jon* jon);
        virtual void execute(Jon* jon);
        virtual void exit(Jon* jon);
        
        virtual void triggerTransform(Jon* jon) {};
        virtual void triggerCancelTransform(Jon* jon);
        
        virtual void triggerJump(Jon* jon) {};
        virtual void triggerLeftAction(Jon* jon) {};
        virtual void triggerRightAction(Jon* jon) {};
        virtual void triggerUpAction(Jon* jon) {};
        virtual void triggerDownAction(Jon* jon) {};
        
    private:
        bool m_hasCompletedSlowMotion;
        
        // ctor, copy ctor, and assignment should be private in a Singleton
        RabbitToVampire();
        RabbitToVampire(const RabbitToVampire&);
        RabbitToVampire& operator=(const RabbitToVampire&);
    };
    
    class VampireToRabbit : public JonFormState
    {
    public:
        static VampireToRabbit* getInstance();
        
        virtual void enter(Jon* jon);
        virtual void execute(Jon* jon);
        virtual void exit(Jon* jon);
        
        virtual void triggerTransform(Jon* jon) {};
        virtual void triggerCancelTransform(Jon* jon);
        
        virtual void triggerJump(Jon* jon) {};
        virtual void triggerLeftAction(Jon* jon) {};
        virtual void triggerRightAction(Jon* jon) {};
        virtual void triggerUpAction(Jon* jon) {};
        virtual void triggerDownAction(Jon* jon) {};
        
    private:
        bool m_hasCompletedSlowMotion;
        
        // ctor, copy ctor, and assignment should be private in a Singleton
        VampireToRabbit();
        VampireToRabbit(const VampireToRabbit&);
        VampireToRabbit& operator=(const VampireToRabbit&);
    };
};

#endif /* defined(__nosfuratu__Jon__) */
