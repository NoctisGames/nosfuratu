//
//  ForegroundObject.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ForegroundObject__
#define __nosfuratu__ForegroundObject__

#include "GridLockedPhysicalEntity.h"

#include "GroundSoundType.h"
#include "ForegroundObjectType.h"
#include "Color.h"

#include "RTTI.h"

class Game;
class Jon;
class NGRect;

class ForegroundObject : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static ForegroundObject* create(int gridX, int gridY, int type);
    
    ForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

    virtual int getEntityLandingPriority();
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
    
    ForegroundObjectType getType();
    
    GroundSoundType getGroundSoundType();
    
    void setGame(Game* game);

    Color getColor();

protected:
    Game* m_game;
	Color m_color;
    
    virtual bool isEntityLanding(PhysicalEntity* entity, NGRect& bounds, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, NGRect& bounds, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, NGRect& bounds, float deltaTime);
    
private:
    ForegroundObjectType m_type;
    GroundSoundType m_groundSoundType;
};

class PlatformObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    PlatformObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
	virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);

    virtual bool canObjectBePlacedOn();
    
    bool shouldJonGrabLedge(PhysicalEntity* entity, float deltaTime);
};

class FloatingPlatformObject : public PlatformObject
{
    RTTI_DECL;
    
public:
    FloatingPlatformObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    ~FloatingPlatformObject();
    
    virtual void update(float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    void onMoved();
    
    PhysicalEntity& getIdlePoof();
    
    PhysicalEntity& getAddedWeightPoof();
    
    bool isIdle();
    
    bool isWeighted();
    
private:
    PhysicalEntity* m_idlePoof;
    PhysicalEntity* m_addedWeightPoof;
    float m_fOriginalY;
    bool m_isIdle;
    bool m_isWeighted;
};

class DeadlyObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    DeadlyObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual int getEntityLandingPriority();
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, NGRect& bounds, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class LandingDeathObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    LandingDeathObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

    virtual int getEntityLandingPriority();
};

class RunningIntoDeathObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    RunningIntoDeathObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
};

class DeathFromAboveObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    DeathFromAboveObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class ProvideBoostObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    ProvideBoostObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight, float boostVelocity);
    
    virtual void update(float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

    virtual int getEntityLandingPriority();
    
private:
    float m_fBoostVelocity;
    bool m_isBoosting;
};

class ExtraForegroundObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    static ExtraForegroundObject* create(int gridX, int gridY, int type);
    
    ExtraForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, ForegroundObjectType shadowType, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    ~ExtraForegroundObject();
    
    virtual void update(float deltaTime);
    
    ForegroundObject& getShadow();
    
protected:
    ForegroundObject* m_shadow;
};

class EndSign : public ForegroundObject
{
    RTTI_DECL;
    
public:
    EndSign(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

    virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
};

class JumpSpringLightFlush : public ProvideBoostObject
{
    RTTI_DECL;
    
public:
    JumpSpringLightFlush(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight, float boostVelocity);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, NGRect& bounds, float deltaTime);
};

class VerticalSaw : public DeadlyObject
{
    RTTI_DECL;
    
public:
    VerticalSaw(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual void updateBounds();
    
private:
    bool m_isOnScreen;
};

class GiantShakingTree : public ForegroundObject
{
    RTTI_DECL;
    
public:
    GiantShakingTree(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual void update(float deltaTime);
    
    void triggerHit();
    
private:
    bool m_isShaking;
};

class SpikeTower : public ExtraForegroundObject
{
    RTTI_DECL;
    
public:
    SpikeTower(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, ForegroundObjectType shadowType, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual void updateBounds();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
};

class SpikedBallRollingLeft : public DeadlyObject
{
    RTTI_DECL;
    
public:
    SpikedBallRollingLeft(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();

    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	void stop();
    
private:
    bool m_isOnScreen;
	bool m_isStopped;
	bool m_needsToPlaySound;
	bool m_isActivated;
	bool m_hasFallen;
};

class SpikedBallRollingRight : public DeadlyObject
{
    RTTI_DECL;
    
public:
    SpikedBallRollingRight(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();

    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	void stop();
    
private:
    bool m_isOnScreen;
	bool m_isStopped;
	bool m_needsToPlaySound;
	bool m_isActivated;
	bool m_hasFallen;
};

class SpikedBall : public DeadlyObject
{
    RTTI_DECL;
    
public:
    SpikedBall(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual void update(float deltaTime);
    
    void fall();
    
private:
    bool m_isFalling;
    bool m_hasTriggeredSnakeHit;
};

class SpikedBallChain : public ForegroundObject
{
    RTTI_DECL;
    
public:
    SpikedBallChain(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    void setSpikedBall(SpikedBall* spikedBall);
    
    virtual void update(float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime);

	void trigger();

private:
    SpikedBall* m_spikedBall;
    bool m_isSnapping;
    bool m_hasTriggeredSpikedBall;
};

class BlockingObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    BlockingObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual ~BlockingObject();
    
    virtual void updateBounds();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
private:
    std::vector<NGRect *> m_normalizedBounds;
};

#endif /* defined(__nosfuratu__ForegroundObject__) */
