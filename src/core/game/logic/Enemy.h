//
//  Enemy.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Enemy__
#define __nosfuratu__Enemy__

#include "GridLockedPhysicalEntity.h"

#include "EnemyType.h"
#include "EnemySpiritType.h"

#include "RTTI.h"

class Game;
class Jon;
class EnemySpirit;

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

    virtual int getEntityLandingPriority();
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime);
    
    bool hasSpirit();
    
    EnemySpirit& getSpirit();
    
    bool isDying();
    
    bool isDead();
    
    void setGame(Game* game);
    
    EnemyType getType();

protected:
    Game* m_game;
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
    Mushroom(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX, float boundsY, float boundsWidth, float boundsHeight, EnemyType type);
    
    virtual void handleAlive(float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime);
    
    bool isBeingBouncedOn();
    
    bool isBouncingBack();
    
protected:
    bool m_isBeingBouncedOn;
    bool m_isBouncingBack;
};

class MushroomGround : public Mushroom
{
    RTTI_DECL;
    
public:
    MushroomGround(int gridX, int gridY);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
};

class MushroomCeiling : public Mushroom
{
    RTTI_DECL;
    
public:
    MushroomCeiling(int gridX, int gridY);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class SnakeGrunt : public Enemy
{
    RTTI_DECL;
    
public:
    SnakeGrunt(int gridX, int gridY);
};

class Sparrow : public Enemy
{
    RTTI_DECL;
    
public:
    Sparrow(int gridX, int gridY);
    
    virtual void updateBounds();

    void onMoved();
    
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
    Toad(int gridX, int gridY);
    
    bool isDeadPart1();
    bool isEating();
    bool hasSwallowedJon();
    bool isJonVampire();

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
    Fox(int gridX, int gridY);

	virtual void updateBounds();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    bool isHitting();
    bool isLeft();
    bool isBeingHit();
    
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
    BigMushroomGround(int gridX, int gridY);
    
protected:
    virtual void handleAlive(float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
};

class BigMushroomCeiling : public Mushroom
{
    RTTI_DECL;
    
public:
    BigMushroomCeiling(int gridX, int gridY);

protected:
    virtual void handleAlive(float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class MovingSnakeGrunt : public Enemy
{
    RTTI_DECL;
    
public:
    MovingSnakeGrunt(int gridX, int gridY, float acceleration, float topSpeed, bool isAbleToJump, EnemyType type, float red, float green, float blue);
    
    virtual void updateBounds();
    
    bool isPreparingToJump();
    bool isLanding();
    bool isPausing();
    
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
