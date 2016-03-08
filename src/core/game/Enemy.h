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

class Game;
class Jon;

typedef enum
{
    EnemyType_MushroomGround,
    EnemyType_MushroomCeiling,
    EnemyType_SnakeGrunt,
    EnemyType_Sparrow,
    EnemyType_Toad,
    EnemyType_Fox
} EnemyType;

class Enemy : public GridLockedPhysicalEntity
{
public:
    static Enemy* create(int gridX, int gridY, int type);
    
    Enemy(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX, float boundsY, float boundsWidth, float boundsHeight, EnemyType type, EnemySpiritType enemySpiritType, short deathSoundId);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    void triggerHit();
    
    virtual bool isJonLanding(Jon& jon, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
    bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    bool isJonHittingFromBelow(Jon& jon, float deltaTime);
    
    virtual void onDeletion();
    
    bool hasSpirit();
    
    EnemySpirit& getSpirit();
    
    Color getColor();
    
    bool isDying();
    
    bool isDead();
    
    virtual bool canBeHitHorizontally();
    
    virtual bool canBeLandedOnToKill();
    
    virtual bool canBeHitFromBelow();
    
    void setGame(Game* game);
    
    EnemyType getType();

protected:
    Game* m_game;
    bool m_isDying;
    
    virtual void handleDead(float deltaTime);
    
    virtual void handleDying(float deltaTime);
    
    virtual void handleAlive(float deltaTime);
    
private:
    EnemySpirit* m_enemySpirit;
    EnemyType m_type;
    EnemySpiritType m_enemySpiritType;
    Color m_color;
    float m_fEnemySpiritStateTime;
    float m_fXOfDeath;
    float m_fYOfDeath;
    short m_deathSoundId;
    bool m_isDead;
};

class Mushroom : public Enemy
{
public:
    Mushroom(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX, float boundsY, float boundsWidth, float boundsHeight, EnemyType type) : Enemy(gridX, gridY, gridWidth, gridHeight, boundsX, boundsY, boundsWidth, boundsHeight, type, EnemySpiritType_None, NO_SOUND) {}
    
    virtual void handleAlive(float deltaTime);
};

class MushroomGround : public Mushroom
{
public:
    MushroomGround(int gridX, int gridY) : Mushroom(gridX, gridY, 7, 8, 0, 0, 1, 0.796875f, EnemyType_MushroomGround) {}
    
    virtual bool isJonLanding(Jon& jon, float deltaTime);
};

class MushroomCeiling : public Mushroom
{
public:
    MushroomCeiling(int gridX, int gridY) : Mushroom(gridX, gridY, 7, 8, 0, 0.203125f, 1, 0.796875f, EnemyType_MushroomCeiling) {}
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class SnakeGrunt : public Enemy
{
public:
    SnakeGrunt(int gridX, int gridY) : Enemy(gridX, gridY, 8, 6, 0, 0, 1, 0.79166666666667f, EnemyType_SnakeGrunt, EnemySpiritType_Snake, SOUND_SNAKE_DEATH) {}
};

class Sparrow : public Enemy
{
public:
    Sparrow(int gridX, int gridY) : Enemy(gridX, gridY, 10, 10, 0, 0, 1, 0.71875f, EnemyType_Sparrow, EnemySpiritType_Sparrow, SOUND_SPARROW_DEATH), m_isOnScreen(false) {}
    
    virtual void updateBounds();
    
private:
    bool m_isOnScreen;
};

class Toad : public Enemy
{
public:
    Toad(int gridX, int gridY) : Enemy(gridX, gridY, 32, 16, 0.34375f, 0.078125f, 0.59765625f, 0.546875f, EnemyType_Toad, EnemySpiritType_None, SOUND_TOAD_DEATH) {}
    
    virtual void handleAlive(float deltaTime);
};

class Fox : public Enemy
{
public:
    Fox(int gridX, int gridY) : Enemy(gridX, gridY, 16, 16, 0, 0, 1, 1, EnemyType_Fox, EnemySpiritType_None, SOUND_FOX_DEATH) {}
    
    virtual void handleAlive(float deltaTime);
};

#endif /* defined(__nosfuratu__Enemy__) */
