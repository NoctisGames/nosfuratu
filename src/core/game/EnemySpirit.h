//
//  EnemySpirit.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__EnemySpirit__
#define __nosfuratu__EnemySpirit__

#include "PhysicalEntity.h"

typedef enum
{
    EnemySpiritType_None,
    EnemySpiritType_Snake,
    EnemySpiritType_Sparrow
} EnemySpiritType;

class EnemySpirit : public PhysicalEntity
{
public:
    static EnemySpirit* create(float x, float y, EnemySpiritType type);
    
    EnemySpirit(float x, float y, float width, float height, EnemySpiritType type);
    
    virtual void update(float deltaTime);
    
    EnemySpiritType getType();
    
private:
    EnemySpiritType m_type;
};

class EnemySpiritSnake : public EnemySpirit
{
public:
    EnemySpiritSnake(float x, float y) : EnemySpirit(x, y, 1.828125f, 2.25f, EnemySpiritType_Snake) {}
};

class EnemySpiritSparrow : public EnemySpirit
{
public:
    EnemySpiritSparrow(float x, float y) : EnemySpirit(x, y + 1.546875f, 2.25f, 4.5f, EnemySpiritType_Sparrow) {}
};

#endif /* defined(__nosfuratu__EnemySpirit__) */
