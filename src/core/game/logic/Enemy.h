//
//  Enemy.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Enemy__
#define __nosfuratu__Enemy__

#include "GridLockedPhysicalEntity.h"
#include "Color.h"
#include "EnemySpirit.h"
#include "GameConstants.h"
#include "RTTI.h"

class Game;
class Jon;

typedef enum
{
    EnemyType_MushroomGround,
    EnemyType_MushroomCeiling,
    EnemyType_SnakeGrunt,
    EnemyType_Sparrow,
    EnemyType_Toad,
    EnemyType_Fox,
    EnemyType_BigMushroomGround,
    EnemyType_BigMushroomCeiling,
    EnemyType_MovingSnakeGruntV1,
    EnemyType_MovingSnakeGruntV2,
    EnemyType_MovingSnakeGruntV3,
    EnemyType_MovingSnakeGruntV4,
    EnemyType_MovingSnakeGruntV5
} EnemyType;

class Enemy : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static Enemy* create(int gridX, int gridY, int type);
    
    Enemy(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX, float boundsY, float boundsWidth, float boundsHeight, EnemyType type, EnemySpiritType enemySpiritType, short deathSoundId);
    
    virtual ~Enemy();
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    void triggerHit();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	virtual int getEntityLandingPriority() { return 0; }
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime);
    
    bool hasSpirit();
    
    EnemySpirit& getSpirit();
    
    Color getColor();
    
    bool isDying();
    
    bool isDead();
    
    void setGame(Game* game);
    
    EnemyType getType();

protected:
    Game* m_game;
    Color m_color;
    bool m_isDying;
    bool m_isDead;
    
    virtual void handleAlive(float deltaTime);
    
    virtual void handleDying(float deltaTime);
    
    virtual void handleDead(float deltaTime);
    
    virtual bool calcIsJonLanding(Jon* jon, float deltaTime);

    virtual void handleJonInKillRange(Jon& jon);
    
	void handleJon();
    
private:
    EnemySpirit* m_enemySpirit;
    EnemyType m_type;
    EnemySpiritType m_enemySpiritType;
    float m_fEnemySpiritStateTime;
    float m_fXOfDeath;
    float m_fYOfDeath;
    short m_deathSoundId;
};

class Mushroom : public Enemy
{
    RTTI_DECL;
    
public:
    Mushroom(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX, float boundsY, float boundsWidth, float boundsHeight, EnemyType type) : Enemy(gridX, gridY, gridWidth, gridHeight, boundsX, boundsY, boundsWidth, boundsHeight, type, EnemySpiritType_None, SOUND_NONE), m_isBeingBouncedOn(false), m_isBouncingBack(false) {}
    
    virtual void handleAlive(float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime) { return false; };
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime) { return false; };
    
    bool isBeingBouncedOn() { return m_isBeingBouncedOn; }
    bool isBouncingBack() { return m_isBouncingBack; }
    
protected:
    bool m_isBeingBouncedOn;
    bool m_isBouncingBack;
};

class MushroomGround : public Mushroom
{
    RTTI_DECL;
    
public:
    MushroomGround(int gridX, int gridY) : Mushroom(gridX, gridY, 7, 8, 0, 0, 1, 0.796875f, EnemyType_MushroomGround) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
};

class MushroomCeiling : public Mushroom
{
    RTTI_DECL;
    
public:
    MushroomCeiling(int gridX, int gridY) : Mushroom(gridX, gridY, 7, 8, 0, 0.203125f, 1, 0.796875f, EnemyType_MushroomCeiling) {}
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class SnakeGrunt : public Enemy
{
    RTTI_DECL;
    
public:
    SnakeGrunt(int gridX, int gridY) : Enemy(gridX, gridY, 8, 6, 0, 0, 1, 0.79166666666667f, EnemyType_SnakeGrunt, EnemySpiritType_Snake, SOUND_SNAKE_DEATH) {}
};

class Sparrow : public Enemy
{
    RTTI_DECL;
    
public:
    Sparrow(int gridX, int gridY) : Enemy(gridX, gridY, 10, 10, 0, 0, 1, 0.71875f, EnemyType_Sparrow, EnemySpiritType_Sparrow, SOUND_SPARROW_DEATH), m_fOriginalY(0), m_isOnScreen(false)
    {
        m_fOriginalY = m_position.getY();
    }
    
    virtual void updateBounds();

	void onMoved() { m_fOriginalY = m_position.getY(); }
    
protected:
    virtual void handleAlive(float deltaTime);
    
private:
    float m_fOriginalY;
    bool m_isOnScreen;
};

class Toad : public Enemy
{
    RTTI_DECL;
    
public:
    Toad(int gridX, int gridY) : Enemy(gridX, gridY, 32, 16, 0.58984375f, 0.109375f, 0.33984375f, 0.3359375f, EnemyType_Toad, EnemySpiritType_None, SOUND_TOAD_DEATH),
    m_isDeadPart1(false),
    m_isEating(false),
    m_hasSwallowedJon(false),
    m_isJonVampire(false) {}
    
    bool isDeadPart1() { return m_isDeadPart1; }
    bool isEating() { return m_isEating; }
    bool hasSwallowedJon() { return m_hasSwallowedJon; }
    bool isJonVampire() { return m_isJonVampire; }

protected:
    virtual void handleAlive(float deltaTime);
    virtual void handleDying(float deltaTime);
    virtual void handleDead(float deltaTime);
    virtual void handleJonInKillRange(Jon& jon);
    
private:
    bool m_isDeadPart1;
    bool m_isEating;
    bool m_hasSwallowedJon;
    bool m_isJonVampire;
};

class Fox : public Enemy
{
    RTTI_DECL;
    
public:
    Fox(int gridX, int gridY) : Enemy(gridX, gridY, 16, 16, 0.25f, 0.09375f, 0.50f, 0.734375f, EnemyType_Fox, EnemySpiritType_None, SOUND_FOX_DEATH),
		m_isHitting(false),
		m_isLeft(true),
		m_isBeingHit(false),
		m_isOnScreen(false) {}

	virtual void updateBounds();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    bool isHitting() { return m_isHitting; }
    bool isLeft() { return m_isLeft; }
    bool isBeingHit() { return m_isBeingHit; }
    
protected:
    virtual void handleAlive(float deltaTime);
    virtual void handleDying(float deltaTime);
    virtual void handleDead(float deltaTime);
    
private:
    bool m_isHitting;
    bool m_isLeft;
    bool m_isBeingHit;
	bool m_isOnScreen;
};

class BigMushroomGround : public Mushroom
{
    RTTI_DECL;
    
public:
    BigMushroomGround(int gridX, int gridY) : Mushroom(gridX, gridY, 16, 13, 0, 0.05f, 1, 0.95f, EnemyType_BigMushroomGround) {}
    
    virtual void handleAlive(float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
};

class BigMushroomCeiling : public Mushroom
{
    RTTI_DECL;
    
public:
    BigMushroomCeiling(int gridX, int gridY) : Mushroom(gridX, gridY, 16, 13, 0, 0, 1, 1, EnemyType_BigMushroomCeiling) {}
    
    virtual void handleAlive(float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class MovingSnakeGrunt : public Enemy
{
    RTTI_DECL;
    
public:
    MovingSnakeGrunt(int gridX, int gridY, float acceleration, float topSpeed, bool isAbleToJump, EnemyType type, float red, float green, float blue) : Enemy(gridX, gridY, 16, 8, 0.1f, 0, 0.8f, 1, type, EnemySpiritType_Snake, SOUND_SNAKE_DEATH), m_fAcceleration(-1 * acceleration), m_fTopSpeed(-1 * topSpeed), m_isAbleToJump(isAbleToJump), m_isPausing(false), m_isPreparingToJump(false), m_isLanding(false), m_isGrounded(false), m_isOnScreen(false)
    {
        m_color.red = red;
        m_color.green = green;
        m_color.blue = blue;
    }
    
    virtual void updateBounds();
    
    bool isPreparingToJump() { return m_isPreparingToJump; }
    bool isLanding() { return m_isLanding; }
    bool isPausing() { return m_isPausing; }
    
protected:
    virtual void handleAlive(float deltaTime);
    
private:
    float m_fAcceleration;
    float m_fTopSpeed;
    bool m_isPausing;
    bool m_isPreparingToJump;
    bool m_isLanding;
    bool m_isGrounded;
    bool m_isAbleToJump;
    bool m_isOnScreen;
};

#endif /* defined(__nosfuratu__Enemy__) */
