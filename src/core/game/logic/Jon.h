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

#include "StateMachine.h"
#include "Color.h"
#include "GroundSoundType.h"
#include "JonFormState.h"
#include "JonShadowState.h"
#include "JonStates.h"

#include "RTTI.h"

#include <vector>

class Game;
class DustCloud;

class JonShadow;

class Jon : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static Jon* create(int gridX, int gridY, int type);
    
    Jon(int gridX, int gridY, int gridWidth = 16, int gridHeight = 16);
    
    ~Jon();
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
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
    
    JonShadow* getJonShadow();
    
    std::vector<Jon *>& getAfterImages();
    
    JonState getState();
    
    JonPhysicalState getPhysicalState();
    
    void setPhysicalState(JonPhysicalState state);
    
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
    
    int getNumEnemiesDestroyed();
    
    void onEnemyDestroyed();
    
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
    
    void setAbilityFlag(int abilityFlag);
    
    void enableAbility(int abilityFlag);
    
    bool isAbilityEnabled(int abilityFlag);
    
    int getAbilityFlag();
    
    bool isBurrowEffective();
    
    void beginWarmingUp();
    
    void consume(bool vampireDies = false);
    
    void kill();
    
    bool shouldUseVampireFormForConsumeAnimation();

	void becomeVampire();

	void flash();
    
    bool isReleasingShockwave();
    
    bool isClimbingLedge();
    
private:
    StateMachine<Jon, JonFormState>* m_formStateMachine;
    Game* m_game;
    std::vector<DustCloud *> m_dustClouds;
    JonShadow* m_jonShadow;
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
	float m_fFlashStateTime;
    int m_iNumTriggeredJumps;
    int m_iNumRabbitJumps;
    int m_iNumVampireJumps;
    int m_iNumBoosts;
    int m_iNumEnemiesDestroyed;
    int m_iAbilityFlag;
    bool m_isLanding;
    bool m_isRollLanding;
    bool m_isRightFoot;
    bool m_isAllowedToMove;
    bool m_isConsumed;
    bool m_isFatallyConsumed;
    bool m_isIdle;
    bool m_isUserActionPrevented;
    bool m_isBurrowEffective;
    bool m_shouldUseVampireFormForConsumeAnimation;
	bool m_isFlashing;
    bool m_isReleasingShockwave;
    bool m_isClimbingLedge;
    bool m_isJumpingOverLedge;
    
    void setState(JonState state);
    
    void setState(JonPhysicalState state);
    
    void setState(JonActionState state);
    
    void setState(JonAbilityState state);
    
    class Rabbit : public JonFormState
    {
        RTTI_DECL;
        
    public:
        static Rabbit* getInstance();
        
        virtual void enter(Jon* jon);
        virtual void execute(Jon* jon);
        virtual void exit(Jon* jon);
        
        virtual void triggerTransform(Jon* jon);
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
        
        virtual int getNumJumps(Jon* jon);
        
    private:
        // ctor, copy ctor, and assignment should be private in a Singleton
        Rabbit();
        Rabbit(const Rabbit&);
        Rabbit& operator=(const Rabbit&);
    };
    
    class Vampire : public JonFormState
    {
        RTTI_DECL;
        
    public:
        static Vampire* getInstance();
        
        virtual void enter(Jon* jon);
        virtual void execute(Jon* jon);
        virtual void exit(Jon* jon);
        
        virtual void triggerTransform(Jon* jon);
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
        
        virtual int getNumJumps(Jon* jon);
        
    private:
        Vector2D* m_lastKnownVelocity;
        float m_fTimeSinceLastVelocityCheck;
        bool m_isFallingAfterGlide;
        
        // ctor, copy ctor, and assignment should be private in a Singleton
        Vampire();
        ~Vampire();
        Vampire(const Vampire&);
        Vampire& operator=(const Vampire&);
    };
    
    class RabbitToVampire : public JonFormState
    {
        RTTI_DECL;
        
    public:
        static RabbitToVampire* getInstance();
        
        virtual void enter(Jon* jon);
        virtual void execute(Jon* jon);
        virtual void exit(Jon* jon);
        
        virtual void triggerTransform(Jon* jon);
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
        
        virtual int getNumJumps(Jon* jon);
        
        void handleTransformation(Jon* jon);
        
    private:
        bool m_hasCompletedSlowMotion;
        
        // ctor, copy ctor, and assignment should be private in a Singleton
        RabbitToVampire();
        RabbitToVampire(const RabbitToVampire&);
        RabbitToVampire& operator=(const RabbitToVampire&);
    };
    
    class VampireToRabbit : public JonFormState
    {
        RTTI_DECL;
        
    public:
        static VampireToRabbit* getInstance();
        
        virtual void enter(Jon* jon);
        virtual void execute(Jon* jon);
        virtual void exit(Jon* jon);
        
        virtual void triggerTransform(Jon* jon);
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
        
        virtual int getNumJumps(Jon* jon);
        
        void handleTransformation(Jon* jon);
        
    private:
        bool m_hasCompletedSlowMotion;
        
        // ctor, copy ctor, and assignment should be private in a Singleton
        VampireToRabbit();
        VampireToRabbit(const VampireToRabbit&);
        VampireToRabbit& operator=(const VampireToRabbit&);
    };
};

class JonShadow : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    JonShadow();
    
    void onGrounded(float x, float y);
    
    void onJump();
    
    void onAir();
    
    void makeInvisible();
    
    JonShadowState getState();
    
private:
    JonShadowState m_state;
};

#endif /* defined(__nosfuratu__Jon__) */
