//
//  Jon.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Jon__
#define __nosfuratu__Jon__

#include "GridLockedPhysicalEntity.h"
#include "Color.h"
#include "DustCloud.h"
#include "GroundSoundType.h"
#include "StateMachine.h"
#include "State.h"

#include <memory>
#include <vector>

class Game;

typedef enum
{
    ABILITY_NONE,
    ABILITY_SPINNING_BACK_FIST,
    ABILITY_BURROW,
    ABILITY_STOMP,
    ABILITY_GLIDE,
    ABILITY_UPWARD_THRUST
} JonAbilityState;

typedef enum
{
    ACTION_NONE,
    ACTION_JUMPING,
    ACTION_DOUBLE_JUMPING
} JonActionState;

typedef enum
{
    PHYSICAL_GROUNDED,
    PHYSICAL_IN_AIR
} JonPhysicalState;

typedef enum
{
    JON_ALIVE,
    JON_DYING,
    JON_DEAD
} JonState;

class Jon : public GridLockedPhysicalEntity
{
public:
    static Jon* create(int gridX, int gridY, int type);
    
    Jon(int gridX, int gridY, int gridWidth = 16, int gridHeight = 16);
    
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
    
    void triggerBoost(float boostVelocity);
    
    void triggerBoostOffEnemy(float boostVelocity);
    
    void triggerBounceDownardsOffEnemy(float bounceBackVelocity);
    
    void triggerBounceBackOffEnemy(float bounceBackVelocity);
    
    int getNumJumps();
    
    std::vector<DustCloud *>& getDustClouds();
    
    std::vector<Jon *>& getAfterImages();
    
    JonState getState();
    
    JonPhysicalState getPhysicalState();
    
    JonActionState getActionState();
    
    JonAbilityState getAbilityState();
    
    Color getColor();
    
    float getActionStateTime();
    
    float getAbilityStateTime();
    
    float getTransformStateTime();
    
    float getDyingStateTime();
    
    int getNumTriggeredJumps();
    
    int getNumBoosts();
    
    void setNumBoosts(int numBoosts);
    
    bool isMoving();
    
    bool isPushedBack();
    
    bool isFalling();
    
    bool isLanding();
    
    bool isRollLanding();
    
    bool isAlive();
    
    bool isDying();
    
    bool isDead();
    
    bool isConsumed();
    
    int getType();
    
    bool isRightFoot();
    
    void setRightFoot(bool isRightFoot);
    
    GroundSoundType getGroundSoundType();
    
    void setGroundSoundType(GroundSoundType groundSoundType);
    
    bool isVampire();
    
    bool isTransformingIntoVampire();
    
    bool isRevertingToRabbit();
    
    void setAllowedToMove(bool isAllowedToMove);
    
    bool isAllowedToMove();
    
    void setIdle(bool isIdle);
    
    bool isIdle();
    
    void setUserActionPrevented(bool isUserActionPrevented);
    
    bool isUserActionPrevented();
    
    void setGame(Game* game);
    
    void enableAbility(int abilityFlag);
    
    bool isAbilityEnabled(int abilityFlag);
    
    bool isBurrowEffective();
    
    void beginWarmingUp();
    
    float getGravity();
    
    void consume(bool vampireDies = false);
    
    void kill();
    
private:
    std::unique_ptr<StateMachine<Jon>> m_formStateMachine;
    Game* m_game;
    std::vector<DustCloud *> m_dustClouds;
    std::vector<Jon *> m_afterImages;
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
    int m_iNumTriggeredJumps;
    int m_iNumRabbitJumps;
    int m_iNumVampireJumps;
    int m_iNumBoosts;
    int m_iAbilitySet;
    bool m_isLanding;
    bool m_isRollLanding;
    bool m_isRightFoot;
    bool m_isAllowedToMove;
    bool m_isConsumed;
    bool m_isFatallyConsumed;
    bool m_isIdle;
    bool m_isUserActionPrevented;
    bool m_isBurrowEffective;
    
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
        
        virtual void triggerBoost(Jon* jon, float boostVelocity) = 0;
        virtual void triggerBoostOffEnemy(Jon* jon, float boostVelocity) = 0;
        virtual void triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity) = 0;
        virtual void triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity) = 0;
        
        virtual void onDeath(Jon* jon) = 0;
        
        virtual int getNumJumps(Jon* jon) = 0;
        
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
        virtual void triggerCancelTransform(Jon* jon) {}
        
        virtual void triggerJump(Jon* jon);
        virtual void triggerLeftAction(Jon* jon);
        virtual void triggerRightAction(Jon* jon);
        virtual void triggerUpAction(Jon* jon);
        virtual void triggerDownAction(Jon* jon);
        
        virtual void triggerBoost(Jon* jon, float boostVelocity);
        virtual void triggerBoostOffEnemy(Jon* jon, float boostVelocity);
        virtual void triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity);
        virtual void triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity);
        
        virtual void onDeath(Jon* jon) {};
        
        virtual int getNumJumps(Jon* jon);
        
    private:
        bool m_isSpinningBackFistDelivered;
        
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
        virtual void triggerCancelTransform(Jon* jon) {}
        
        virtual void triggerJump(Jon* jon);
        virtual void triggerLeftAction(Jon* jon);
        virtual void triggerRightAction(Jon* jon);
        virtual void triggerUpAction(Jon* jon);
        virtual void triggerDownAction(Jon* jon);
        
        virtual void triggerBoost(Jon* jon, float boostVelocity);
        virtual void triggerBoostOffEnemy(Jon* jon, float boostVelocity);
        virtual void triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity);
        virtual void triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity);
        
        virtual void onDeath(Jon* jon) {};
        
        virtual int getNumJumps(Jon* jon);
        
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
        
        virtual void triggerTransform(Jon* jon) {}
        virtual void triggerCancelTransform(Jon* jon);
        
        virtual void triggerJump(Jon* jon);
        virtual void triggerLeftAction(Jon* jon);
        virtual void triggerRightAction(Jon* jon);
        virtual void triggerUpAction(Jon* jon);
        virtual void triggerDownAction(Jon* jon);
        
        virtual void triggerBoost(Jon* jon, float boostVelocity);
        virtual void triggerBoostOffEnemy(Jon* jon, float boostVelocity);
        virtual void triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity);
        virtual void triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity);
        
        virtual void onDeath(Jon* jon);
        
        virtual int getNumJumps(Jon* jon) { return 0; };
        
    private:
        bool m_hasCompletedSlowMotion;
        
        void handleTransformation(Jon* jon);
        
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
        
        virtual void triggerTransform(Jon* jon) {}
        virtual void triggerCancelTransform(Jon* jon);
        
        virtual void triggerJump(Jon* jon);
        virtual void triggerLeftAction(Jon* jon);
        virtual void triggerRightAction(Jon* jon);
        virtual void triggerUpAction(Jon* jon);
        virtual void triggerDownAction(Jon* jon);
        
        virtual void triggerBoost(Jon* jon, float boostVelocity);
        virtual void triggerBoostOffEnemy(Jon* jon, float boostVelocity);
        virtual void triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity);
        virtual void triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity);
        
        virtual void onDeath(Jon* jon);
        
        virtual int getNumJumps(Jon* jon) { return 0; };
        
    private:
        bool m_hasCompletedSlowMotion;
        
        void handleTransformation(Jon* jon);
        
        // ctor, copy ctor, and assignment should be private in a Singleton
        VampireToRabbit();
        VampireToRabbit(const VampireToRabbit&);
        VampireToRabbit& operator=(const VampireToRabbit&);
    };
};

#endif /* defined(__nosfuratu__Jon__) */
