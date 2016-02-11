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
#include "Spirit.h"

class Game;

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
    
    Enemy(int gridX, int gridY, int gridWidth, int gridHeight, EnemyType type, SpiritType spiritType);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    void triggerHit();
    
    virtual void onDeletion();
    
    bool hasSpirit();
    
    Spirit& getSpirit();
    
    Color getColor();
    
    bool isDying();
    
    bool isDead();
    
    bool hasKilledJon();
    
    virtual bool canBeHitHorizontally();
    
    virtual bool canBeLandedOnToKill();
    
    virtual bool canBeHitFromBelow();
    
    void setGame(Game* game);
    
    EnemyType getType();

protected:
    Game* m_game;
    bool m_isDying;
    bool m_hasKilledJon;
    
private:
    Spirit* m_spirit;
    EnemyType m_type;
    SpiritType m_spiritType;
    Color m_color;
    float m_fSpiritStateTime;
    float m_fXOfDeath;
    float m_fYOfDeath;
    bool m_isDead;
};

class SnakeGrunt : public Enemy
{
public:
    SnakeGrunt(int gridX, int gridY) : Enemy(gridX, gridY, 8, 6, EnemyType_SnakeGrunt, SpiritType_Snake) {}
};

class Sparrow : public Enemy
{
public:
    Sparrow(int gridX, int gridY) : Enemy(gridX, gridY, 10, 10, EnemyType_Sparrow, SpiritType_Snake) {}
};

#endif /* defined(__nosfuratu__Enemy__) */
