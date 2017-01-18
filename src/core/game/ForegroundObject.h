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
#include "RTTI.h"

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
    ForegroundObjectType_SpikedBallChain,
    
    ForegroundObjectType_MetalGrassPlatform,
    ForegroundObjectType_MetalGrassPlatformLeft,
    ForegroundObjectType_MetalGrassPlatformCenter,
    ForegroundObjectType_MetalGrassPlatformRight,
    
    ForegroundObjectType_WoodPlatform,
    ForegroundObjectType_WoodBoxTop,
    ForegroundObjectType_WoodBox,
    
    ForegroundObjectType_GreenThornsLeft,
    ForegroundObjectType_GreenThornsCenterSmall,
    ForegroundObjectType_GreenThornsCenterBig,
    ForegroundObjectType_GreenThornsRight,
    
    ForegroundObjectType_Logs,
    
    ForegroundObjectType_Stone_Bottom,
    ForegroundObjectType_Stone_Middle,
    ForegroundObjectType_Stone_Top,
    ForegroundObjectType_Stone_Platform,
    
    ForegroundObjectType_Floating_Platform,
    
    ForegroundObjectType_Stone_Square
} ForegroundObjectType;

class ForegroundObject : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static ForegroundObject* create(int gridX, int gridY, int type);
    
    ForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	virtual int getEntityLandingPriority() { return 0; }
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
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

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, Rectangle& bounds, float deltaTime);
    
private:
    ForegroundObjectType m_type;
    GroundSoundType m_groundSoundType;
};

class PlatformObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    PlatformObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
	virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);

    virtual bool canObjectBePlacedOn();
};

class FloatingPlatformObject : public PlatformObject
{
    RTTI_DECL;
    
public:
    FloatingPlatformObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : PlatformObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_fOriginalY(0), m_isIdle(true), m_isWeighted(false)
    {
        float x = m_position->getX();
        float y = m_position->getY() - m_fHeight / 2 + 0.1f;
        m_idlePoof = std::unique_ptr<PhysicalEntity>(new PhysicalEntity(x, y - 0.31640625f / 2, 0.4921875f, 0.31640625f));
        m_addedWeightPoof = std::unique_ptr<PhysicalEntity>(new PhysicalEntity(x, y - 1.51171875f / 2, 1.40625f, 1.51171875f));
        
        onMoved();
    }
    
    virtual void update(float deltaTime)
    {
        m_position->add(m_velocity->getX() * deltaTime, m_velocity->getY() * deltaTime);
        
        float x = m_position->getX();
        float y = m_position->getY() - m_fHeight / 2 + 0.1f;
        m_idlePoof->getPosition().set(x, y - 0.31640625f / 2);
        m_addedWeightPoof->getPosition().set(x, y - 1.51171875f / 2);
        
        m_idlePoof->update(deltaTime);
        m_addedWeightPoof->update(deltaTime);
        
        if (m_isIdle)
        {
            if (m_position->getY() > (m_fOriginalY + 0.1f))
            {
                m_velocity->setY(-0.2f);
            }
            else if (m_position->getY() < (m_fOriginalY - 0.1f))
            {
                m_velocity->setY(0.2f);
            }
        }
    }
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime)
    {
        if (PlatformObject::isEntityLanding(entity, deltaTime))
        {
            m_isIdle = false;
            m_isWeighted = true;
            
            m_position->setY(m_fOriginalY - 0.2f);
            m_velocity->setY(0);
            
            return true;
        }
        
        m_isIdle = true;
        m_isWeighted = false;
        
        return false;
    }
    
    void onMoved()
    {
        m_fOriginalY = m_position->getY();
        
        // One time
        getMainBounds().setWidth(getWidth());
        getMainBounds().setHeight(getHeight());
        
        PhysicalEntity::updateBounds();
        
        getMainBounds().getLowerLeft().add(getWidth() * m_fBoundsX, getHeight() * m_fBoundsY);
        getMainBounds().setWidth(getWidth() * m_fBoundsWidth);
        getMainBounds().setHeight(getHeight() * m_fBoundsHeight);
        
        m_velocity->setY(-0.25f);
    }
    
    PhysicalEntity& getIdlePoof() { return *m_idlePoof; }
    PhysicalEntity& getAddedWeightPoof() { return *m_addedWeightPoof; }
    bool isIdle() { return m_isIdle; }
    bool isWeighted() { return m_isWeighted; }
    
private:
    std::unique_ptr<PhysicalEntity> m_idlePoof;
    std::unique_ptr<PhysicalEntity> m_addedWeightPoof;
    float m_fOriginalY;
    bool m_isIdle;
    bool m_isWeighted;
};

class DeadlyObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    DeadlyObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, Rectangle& bounds, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);

	virtual int getEntityLandingPriority() { return 1; }
};

class LandingDeathObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    LandingDeathObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, GROUND_SOUND_NONE, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	virtual int getEntityLandingPriority() { return 1; }
};

class RunningIntoDeathObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    RunningIntoDeathObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
};

class DeathFromAboveObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    DeathFromAboveObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, GROUND_SOUND_NONE, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class ProvideBoostObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    ProvideBoostObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight, float boostVelocity) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_fBoostVelocity(boostVelocity), m_isBoosting(false) {}
    
    virtual void update(float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	virtual int getEntityLandingPriority() { return 1; }
    
private:
    float m_fBoostVelocity;
    bool m_isBoosting;
};

class ExtraForegroundObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    static ExtraForegroundObject* create(int gridX, int gridY, int type)
    {
        return reinterpret_cast<ExtraForegroundObject *>(ForegroundObject::create(gridX, gridY, type));
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

class EndSign : public ForegroundObject
{
    RTTI_DECL;
    
public:
    EndSign(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime) { return false; }
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime) { return false; }

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime) { return false; }
};

class JumpSpringLightFlush : public ProvideBoostObject
{
    RTTI_DECL;
    
public:
    JumpSpringLightFlush(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight, float boostVelocity) : ProvideBoostObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight, boostVelocity) {}
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, Rectangle& bounds, float deltaTime);
};

class VerticalSaw : public DeadlyObject
{
    RTTI_DECL;
    
public:
    VerticalSaw(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : DeadlyObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_isOnScreen(false) {}
    
    virtual void updateBounds();
    
private:
    bool m_isOnScreen;
};

class GiantShakingTree : public ForegroundObject
{
    RTTI_DECL;
    
public:
    GiantShakingTree(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_isShaking(false) {}
    
    virtual void update(float deltaTime);
    
    void triggerHit();
    
private:
    bool m_isShaking;
};

class SpikeTower : public ExtraForegroundObject
{
    RTTI_DECL;
    
public:
    SpikeTower(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, ForegroundObjectType shadowType, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ExtraForegroundObject(gridX, gridY, gridWidth, gridHeight, type, shadowType, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight)
    {
		m_bounds.push_back(std::unique_ptr<Rectangle>(new Rectangle(0, 0, 1, 1)));

        updateBounds();
    }
    
    virtual void updateBounds();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
};

class SpikedBallRollingLeft : public DeadlyObject
{
    RTTI_DECL;
    
public:
    SpikedBallRollingLeft(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : DeadlyObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight),
		m_isOnScreen(false),
		m_isStopped(false),
		m_needsToPlaySound(false),
		m_isActivated(false),
		m_hasFallen(false) {}
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();

	virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime) { return false; }

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
	SpikedBallRollingRight(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : DeadlyObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight),
		m_isOnScreen(false),
		m_isStopped(false),
		m_needsToPlaySound(false),
		m_isActivated(false),
		m_hasFallen(false) {}
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();

	virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime) { return false; }

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
    SpikedBall(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : DeadlyObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_isFalling(false), m_hasTriggeredSnakeHit(false) {}
    
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
    RTTI_DECL;
    
public:
    SpikedBallChain(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_spikedBall(nullptr), m_isSnapping(false), m_hasTriggeredSpikedBall(false) {}
    
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

class BlockingObject : public ForegroundObject
{
    RTTI_DECL;
    
public:
    BlockingObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

#endif /* defined(__nosfuratu__ForegroundObject__) */
