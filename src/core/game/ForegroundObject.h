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
#include "Jon.h"
#include "GameConstants.h"

class Game;

typedef enum
{
    ForegroundObjectType_GrassPlatformLeft,
    ForegroundObjectType_GrassPlatformCenter,
    ForegroundObjectType_GrassPlatformRight,
    
    ForegroundObjectType_CavePlatformLeft,
    ForegroundObjectType_CavePlatformCenter,
    ForegroundObjectType_CavePlatformRight,
    
    ForegroundObjectType_RockLarge,
    ForegroundObjectType_RockMedium,
    ForegroundObjectType_RockSmall,
    ForegroundObjectType_RockSmallCracked,
    
    ForegroundObjectType_StumpBig,
    ForegroundObjectType_StumpSmall,
    
    ForegroundObjectType_EndSign,
    
    ForegroundObjectType_ThornsLeft,
    ForegroundObjectType_ThornsCenterSmall,
    ForegroundObjectType_ThornsCenterBig,
    ForegroundObjectType_ThornsRight,
    
    ForegroundObjectType_LogVerticalTall,
    ForegroundObjectType_LogVerticalShort,
    
    ForegroundObjectType_JumpSpringLight,
    ForegroundObjectType_JumpSpringMedium,
    ForegroundObjectType_JumpSpringHeavy,
    
    ForegroundObjectType_SpikeGrassSingle,
    ForegroundObjectType_SpikeGrassFour,
    ForegroundObjectType_SpikeGrassEight,
    
    ForegroundObjectType_SpikeCaveSingle,
    ForegroundObjectType_SpikeCaveFour,
    ForegroundObjectType_SpikeCaveEight,
    
    ForegroundObjectType_SpikeCaveCeilingSingle,
    ForegroundObjectType_SpikeCaveCeilingFour,
    ForegroundObjectType_SpikeCaveCeilingEight,
    
    ForegroundObjectType_SpikeWallSingle,
    ForegroundObjectType_SpikeWallFour,
    ForegroundObjectType_SpikeWallEight,
    
    ForegroundObjectType_SpikeStar,
    
    ForegroundObjectType_VerticalSaw,
    
    ForegroundObjectType_GiantTree,
    ForegroundObjectType_GiantShakingTree,
    ForegroundObjectType_GiantPerchTree,
    
    ForegroundObjectType_SpikeTower,
    ForegroundObjectType_SpikeTowerBg,

	ForegroundObjectType_JumpSpringLightFlush,
    
    ForegroundObjectType_SpikedBallRollingLeft,
    ForegroundObjectType_SpikedBallRollingRight,
    
    ForegroundObjectType_SpikedBall,
    ForegroundObjectType_SpikedBallChain
} ForegroundObjectType;

class ForegroundObject : public GridLockedPhysicalEntity
{
public:
    static ForegroundObject* create(int gridX, int gridY, int type);
    
    ForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual void updateBounds();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	virtual int getEntityLandingPriority() { return 0; }
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
    
    ForegroundObjectType getType();
    
    GroundSoundType getGroundSoundType();
    
    void setGame(Game* game);

	Color getColor() { return m_color; }

protected:
    Game* m_game;
	Color m_color;
    
    virtual bool isEntityLanding(PhysicalEntity* entity, Rectangle& bounds, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, Rectangle& bounds, float deltaTime);
    
private:
    ForegroundObjectType m_type;
    GroundSoundType m_groundSoundType;
};

class PlatformObject : public ForegroundObject
{
public:
    PlatformObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
	virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

    virtual bool canObjectBePlacedOn();
};

class DestructibleObject : public ForegroundObject
{
public:
    DestructibleObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_isDestructing(false) {}
    
    virtual void update(float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime);
    
private:
    bool m_isDestructing;
};

class DeadlyObject : public ForegroundObject
{
public:
    DeadlyObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);

	virtual int getEntityLandingPriority() { return 1; }
};

class LandingDeathObject : public ForegroundObject
{
public:
    LandingDeathObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, GROUND_SOUND_NONE, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	virtual int getEntityLandingPriority() { return 1; }
};

class RunningIntoDeathObject : public ForegroundObject
{
public:
    RunningIntoDeathObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
};

class DeathFromAboveObject : public ForegroundObject
{
public:
    DeathFromAboveObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, GROUND_SOUND_NONE, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class ProvideBoostObject : public ForegroundObject
{
public:
    ProvideBoostObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight, float boostVelocity) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_fBoostVelocity(boostVelocity), m_isBoosting(false) {}
    
    virtual void update(float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	virtual int getEntityLandingPriority() { return 1; }
    
private:
    float m_fBoostVelocity;
    bool m_isBoosting;
};

class GrassPlatformLeft : public PlatformObject
{
public:
    GrassPlatformLeft(int gridX, int gridY) : PlatformObject(gridX, gridY, 4, 6, ForegroundObjectType_GrassPlatformLeft, GROUND_SOUND_GRASS, 0, 0, 1, 0.83333333333333f) {}
};

class GrassPlatformCenter : public PlatformObject
{
public:
    GrassPlatformCenter(int gridX, int gridY) : PlatformObject(gridX, gridY, 14, 6, ForegroundObjectType_GrassPlatformCenter, GROUND_SOUND_GRASS, 0, 0, 1, 0.83333333333333f) {}
};

class GrassPlatformRight : public PlatformObject
{
public:
    GrassPlatformRight(int gridX, int gridY) : PlatformObject(gridX, gridY, 4, 6, ForegroundObjectType_GrassPlatformRight, GROUND_SOUND_GRASS, 0, 0, 1, 0.83333333333333f) {}
};

class CavePlatformLeft : public PlatformObject
{
public:
    CavePlatformLeft(int gridX, int gridY) : PlatformObject(gridX, gridY, 4, 6, ForegroundObjectType_CavePlatformLeft, GROUND_SOUND_CAVE, 0, 0, 1, 0.83333333333333f) {}
};

class CavePlatformCenter : public PlatformObject
{
public:
    CavePlatformCenter(int gridX, int gridY) : PlatformObject(gridX, gridY, 14, 6, ForegroundObjectType_CavePlatformCenter, GROUND_SOUND_CAVE, 0, 0, 1, 0.83333333333333f) {}
};

class CavePlatformRight : public PlatformObject
{
public:
    CavePlatformRight(int gridX, int gridY) : PlatformObject(gridX, gridY, 4, 6, ForegroundObjectType_CavePlatformRight, GROUND_SOUND_CAVE, 0, 0, 1, 0.83333333333333f) {}
};

class RockLarge : public ForegroundObject
{
public:
    RockLarge(int gridX, int gridY) : ForegroundObject(gridX, gridY, 44, 32, ForegroundObjectType_RockLarge, GROUND_SOUND_CAVE, 0.03551136363636f, 0.0078125f, 0.77840909090909f, 0.96875f) {}
};

class RockMedium : public ForegroundObject
{
public:
    RockMedium(int gridX, int gridY) : ForegroundObject(gridX, gridY, 24, 24, ForegroundObjectType_RockMedium, GROUND_SOUND_CAVE, 0.0625f, 0.015625f, 0.7734375f, 0.73958333333333f) {}
};

class RockSmall : public ForegroundObject
{
public:
    RockSmall(int gridX, int gridY) : ForegroundObject(gridX, gridY, 24, 23, ForegroundObjectType_RockSmall, GROUND_SOUND_CAVE, 0.328125f, 0.20923913043478f, 0.328125f, 0.27717391304348f) {}
};

class StumpBig : public ForegroundObject
{
public:
    StumpBig(int gridX, int gridY) : ForegroundObject(gridX, gridY, 16, 18, ForegroundObjectType_StumpBig, GROUND_SOUND_GRASS, 0.19140625f, 0.03125f, 0.5f, 0.94444444444444f) {}
};

class StumpSmall : public ForegroundObject
{
public:
    StumpSmall(int gridX, int gridY) : ForegroundObject(gridX, gridY, 20, 17, ForegroundObjectType_StumpSmall, GROUND_SOUND_GRASS, 0.215625f, 0.07352941176471f, 0.375f, 0.73897058823529f) {}
};

class EndSign : public ForegroundObject
{
public:
    EndSign(int gridX, int gridY) : ForegroundObject(gridX, gridY, 7, 10, ForegroundObjectType_EndSign, GROUND_SOUND_NONE, 0, 0, 1, 1) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime) { return false; }
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime) { return false; }
};

class ThornsLeft : public DeadlyObject
{
public:
    ThornsLeft(int gridX, int gridY) : DeadlyObject(gridX, gridY, 5, 7, ForegroundObjectType_ThornsLeft) {}
};

class ThornsCenterSmall : public DeadlyObject
{
public:
    ThornsCenterSmall(int gridX, int gridY) : DeadlyObject(gridX, gridY, 11, 7, ForegroundObjectType_ThornsCenterSmall) {}
};

class ThornsCenterBig : public DeadlyObject
{
public:
    ThornsCenterBig(int gridX, int gridY) : DeadlyObject(gridX, gridY, 22, 7, ForegroundObjectType_ThornsCenterBig) {}
};

class ThornsRight : public DeadlyObject
{
public:
    ThornsRight(int gridX, int gridY) : DeadlyObject(gridX, gridY, 5, 7, ForegroundObjectType_ThornsRight) {}
};

class LogVerticalTall : public ForegroundObject
{
public:
    LogVerticalTall(int gridX, int gridY) : ForegroundObject(gridX, gridY, 8, 11, ForegroundObjectType_LogVerticalTall, GROUND_SOUND_GRASS, 0.1484375f, 0.02840909090909f, 0.5f, 0.92613636363636f) {}
};

class LogVerticalShort : public ForegroundObject
{
public:
    LogVerticalShort(int gridX, int gridY) : ForegroundObject(gridX, gridY, 8, 6, ForegroundObjectType_LogVerticalShort, GROUND_SOUND_GRASS, 0.1462f, 0.05208333333333f, 0.4453125f, 0.875f) {}
};

class JumpSpringLightFlush : public ProvideBoostObject
{
public:
    JumpSpringLightFlush(int gridX, int gridY) : ProvideBoostObject(gridX, gridY, 10, 7, ForegroundObjectType_JumpSpringLightFlush, GROUND_SOUND_NONE, 0, 0.033f, 1, 0.1f, 18.0f) {}
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
};

class JumpSpringLight : public ProvideBoostObject
{
public:
    JumpSpringLight(int gridX, int gridY) : ProvideBoostObject(gridX, gridY, 6, 5, ForegroundObjectType_JumpSpringLight, GROUND_SOUND_NONE, 0, 0, 1, 0.525f, 18.0f) {}
};

class JumpSpringMedium : public ProvideBoostObject
{
public:
    JumpSpringMedium(int gridX, int gridY) : ProvideBoostObject(gridX, gridY, 17, 9, ForegroundObjectType_JumpSpringMedium, GROUND_SOUND_NONE, 0, 0.20138888888889f, 1, 0.33333333333333f, 24.0f) {}
};

class JumpSpringHeavy : public ProvideBoostObject
{
public:
    JumpSpringHeavy(int gridX, int gridY) : ProvideBoostObject(gridX, gridY, 17, 14, ForegroundObjectType_JumpSpringHeavy, GROUND_SOUND_NONE, 0, 0, 1, 0.52678571428571f, 32.0f) {}
};

class SpikeGrassSingle : public LandingDeathObject
{
public:
    SpikeGrassSingle(int gridX, int gridY) : LandingDeathObject(gridX, gridY, 6, 6, ForegroundObjectType_SpikeGrassSingle, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f) {}
};

class SpikeGrassFour : public LandingDeathObject
{
public:
    SpikeGrassFour(int gridX, int gridY) : LandingDeathObject(gridX, gridY, 18, 6, ForegroundObjectType_SpikeGrassFour, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f) {}
};

class SpikeGrassEight : public LandingDeathObject
{
public:
    SpikeGrassEight(int gridX, int gridY) : LandingDeathObject(gridX, gridY, 34, 6, ForegroundObjectType_SpikeGrassEight, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f) {}
};

class SpikeCaveSingle : public LandingDeathObject
{
public:
    SpikeCaveSingle(int gridX, int gridY) : LandingDeathObject(gridX, gridY, 6, 6, ForegroundObjectType_SpikeCaveSingle, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f) {}
};

class SpikeCaveFour : public LandingDeathObject
{
public:
    SpikeCaveFour(int gridX, int gridY) : LandingDeathObject(gridX, gridY, 18, 6, ForegroundObjectType_SpikeCaveFour, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f) {}
};

class SpikeCaveEight : public LandingDeathObject
{
public:
    SpikeCaveEight(int gridX, int gridY) : LandingDeathObject(gridX, gridY, 34, 6, ForegroundObjectType_SpikeCaveEight, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f) {}
};

class SpikeCaveCeilingSingle : public DeathFromAboveObject
{
public:
    SpikeCaveCeilingSingle(int gridX, int gridY) : DeathFromAboveObject(gridX, gridY, 6, 6, ForegroundObjectType_SpikeCaveCeilingSingle, 0.1f, 0.0f, 0.8f, 0.9f) {}
};

class SpikeCaveCeilingFour : public DeathFromAboveObject
{
public:
    SpikeCaveCeilingFour(int gridX, int gridY) : DeathFromAboveObject(gridX, gridY, 18, 6, ForegroundObjectType_SpikeCaveCeilingFour, 0.1f, 0.0f, 0.8f, 0.9f) {}
};

class SpikeCaveCeilingEight : public DeathFromAboveObject
{
public:
    SpikeCaveCeilingEight(int gridX, int gridY) : DeathFromAboveObject(gridX, gridY, 34, 6, ForegroundObjectType_SpikeCaveCeilingEight, 0.1f, 0.0f, 0.8f, 0.9f) {}
};

class SpikeWallSingle : public RunningIntoDeathObject
{
public:
    SpikeWallSingle(int gridX, int gridY) : RunningIntoDeathObject(gridX, gridY, 6, 4, ForegroundObjectType_SpikeWallSingle) {}
};

class SpikeWallFour : public RunningIntoDeathObject
{
public:
    SpikeWallFour(int gridX, int gridY) : RunningIntoDeathObject(gridX, gridY, 6, 16, ForegroundObjectType_SpikeWallFour) {}
};

class SpikeWallEight : public RunningIntoDeathObject
{
public:
    SpikeWallEight(int gridX, int gridY) : RunningIntoDeathObject(gridX, gridY, 6, 34, ForegroundObjectType_SpikeWallEight) {}
};

class SpikeStar : public DeadlyObject
{
public:
    SpikeStar(int gridX, int gridY) : DeadlyObject(gridX, gridY, 14, 14, ForegroundObjectType_SpikeStar, GROUND_SOUND_NONE, 0.18f, 0.18f, 0.64f, 0.64f) {}
};

class VerticalSaw : public DeadlyObject
{
public:
    VerticalSaw(int gridX) : DeadlyObject(gridX, 95, 15, 33, ForegroundObjectType_VerticalSaw, GROUND_SOUND_NONE, 0.3f, 0.33333333333333f, 0.3f, 0.66666666666667f), m_isOnScreen(false) {}
    
    virtual void updateBounds();
    
private:
    bool m_isOnScreen;
};

class GiantTree : public ForegroundObject
{
public:
    GiantTree(int gridX, int gridY) : ForegroundObject(gridX, gridY, 68, 65, ForegroundObjectType_GiantTree, GROUND_SOUND_NONE, 0.47265625f, 0.009765625f, 0.15625f, 0.990234375f) {}
};

class GiantShakingTree : public ForegroundObject
{
public:
    GiantShakingTree(int gridX, int gridY) : ForegroundObject(gridX, gridY, 68, 65, ForegroundObjectType_GiantShakingTree, GROUND_SOUND_NONE, 0.47265625f, 0.009765625f, 0.15625f, 0.990234375f), m_isShaking(false) {}
    
    virtual void update(float deltaTime);
    
    void triggerHit();
    
private:
    bool m_isShaking;
};

class GiantPerchTree : public ForegroundObject
{
public:
    GiantPerchTree(int gridX, int gridY) : ForegroundObject(gridX, gridY, 68, 65, ForegroundObjectType_GiantPerchTree, GROUND_SOUND_NONE, 0, 0.009765625f, 1, 0.990234375f) {}
};

class ExtraForegroundObject : public ForegroundObject
{
public:
    static ExtraForegroundObject* create(int gridX, int gridY, int type)
    {
        return dynamic_cast<ExtraForegroundObject*>(ForegroundObject::create(gridX, gridY, type));
    }
    
    ExtraForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, ForegroundObjectType shadowType, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_shadow(nullptr)
    {
        m_shadow = std::unique_ptr<ForegroundObject>(ForegroundObject::create(gridX, gridY, shadowType));
    }
    
    virtual void update(float deltaTime);
    
    ForegroundObject& getShadow() { return *m_shadow; }
    
protected:
    std::unique_ptr<ForegroundObject> m_shadow;
};

class SpikeTower : public ExtraForegroundObject
{
public:
    SpikeTower(int gridX) : ExtraForegroundObject(gridX, 95, 32, 56, ForegroundObjectType_SpikeTower, ForegroundObjectType_SpikeTowerBg, GROUND_SOUND_NONE, 0, 0, 1, 0.54017857142857f)
    {
        m_bounds.push_back(Rectangle(0, 0, 1, 1));
        updateBounds();
    }
    
    virtual void updateBounds();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
};

class SpikeTowerBg : public ForegroundObject
{
public:
    SpikeTowerBg(int gridX) : ForegroundObject(gridX, 95, 32, 56, ForegroundObjectType_SpikeTowerBg) {}
};

class SpikedBallRollingLeft : public DeadlyObject
{
public:
    SpikedBallRollingLeft(int gridX, int gridY) : DeadlyObject(gridX, gridY, 32, 30, ForegroundObjectType_SpikedBallRollingLeft, GROUND_SOUND_NONE, 0.1f, 0.1f, 0.8f, 0.8f),
		m_isOnScreen(false),
		m_isStopped(false),
		m_needsToPlaySound(false),
		m_isActivated(false),
		m_hasFallen(false) {}
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();

	virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime) { return false; }

	virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, Rectangle& bounds, float deltaTime);

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
public:
	SpikedBallRollingRight(int gridX, int gridY) : DeadlyObject(gridX, gridY, 32, 30, ForegroundObjectType_SpikedBallRollingRight, GROUND_SOUND_NONE, 0.1f, 0.1f, 0.8f, 0.8f),
		m_isOnScreen(false),
		m_isStopped(false),
		m_needsToPlaySound(false),
		m_isActivated(false),
		m_hasFallen(false) {}
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();

	virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime) { return false; }

	virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, Rectangle& bounds, float deltaTime);

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
public:
    SpikedBall(int gridX, int gridY) : DeadlyObject(gridX, gridY, 32, 30, ForegroundObjectType_SpikedBall, GROUND_SOUND_NONE, 0, 0.4f, 1, 0.8f), m_isFalling(false), m_hasTriggeredSnakeHit(false) {}
    
    virtual void update(float deltaTime);
    
    void fall()
    {
        m_isFalling = true;
        m_velocity->setY(GAME_GRAVITY);
    }
    
private:
    bool m_isFalling;
    bool m_hasTriggeredSnakeHit;
};

class SpikedBallChain : public ForegroundObject
{
public:
    SpikedBallChain(int gridX, int gridY) : ForegroundObject(gridX, gridY, 101, 82, ForegroundObjectType_SpikedBallChain, GROUND_SOUND_NONE, 0.72f, 0, 0.14f, 0.25f), m_spikedBall(nullptr), m_isSnapping(false), m_hasTriggeredSpikedBall(false) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime) { return false; }
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime) { return false; }
    
	void setSpikedBall(SpikedBall* spikedBall) { m_spikedBall = spikedBall; }
    
    virtual void update(float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime);

	void trigger();

private:
    SpikedBall* m_spikedBall;
    bool m_isSnapping;
    bool m_hasTriggeredSpikedBall;
};

#endif /* defined(__nosfuratu__ForegroundObject__) */
