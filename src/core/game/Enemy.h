//
//  Enemy.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Enemy__
#define __nosfuratu__Enemy__

#include "GridLockedPhysicalEntity.h"
#include "Color.h"
#include "EnemySpirit.h"

class Game;
class Jon;

typedef enum
{
    EnemyType_Mushroom,
    EnemyType_MushroomCeiling,
    EnemyType_SnakeGrunt,
    EnemyType_Sparrow
} EnemyType;

class Enemy : public GridLockedPhysicalEntity
{
public:
    static Enemy* create(int gridX, int gridY, int type);
    
    Enemy(int gridX, int gridY, int gridWidth, int gridHeight, EnemyType type, EnemySpiritType enemySpiritType);
    
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
    
private:
    EnemySpirit* m_enemySpirit;
    EnemyType m_type;
    EnemySpiritType m_enemySpiritType;
    Color m_color;
    float m_fEnemySpiritStateTime;
    float m_fXOfDeath;
    float m_fYOfDeath;
    bool m_isDead;
};

class Mushroom : public Enemy
{
public:
    Mushroom(int gridX, int gridY) : Enemy(gridX, gridY, 7, 8, EnemyType_Mushroom, EnemySpiritType_None) {}
    
    virtual bool isJonLanding(Jon& jon, float deltaTime);
};

class MushroomCeiling : public Enemy
{
public:
    MushroomCeiling(int gridX, int gridY) : Enemy(gridX, gridY, 7, 8, EnemyType_MushroomCeiling, EnemySpiritType_None) {}
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

class SnakeGrunt : public Enemy
{
public:
    SnakeGrunt(int gridX, int gridY) : Enemy(gridX, gridY, 8, 6, EnemyType_SnakeGrunt, EnemySpiritType_Snake) {}
};

class Sparrow : public Enemy
{
public:
    Sparrow(int gridX, int gridY) : Enemy(gridX, gridY, 10, 10, EnemyType_Sparrow, EnemySpiritType_Sparrow) {}
};

#endif /* defined(__nosfuratu__Enemy__) */
