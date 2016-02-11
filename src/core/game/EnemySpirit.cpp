//
//  EnemySpirit.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "EnemySpirit.h"
#include "EntityUtils.h"

EnemySpirit* EnemySpirit::create(float x, float y, EnemySpiritType type)
{
    switch (type)
    {
        case EnemySpiritType_Snake:
            return new EnemySpiritSnake(x, y);
        case EnemySpiritType_Sparrow:
            return new EnemySpiritSparrow(x, y);
        case EnemySpiritType_None:
            return nullptr;
    }
    
    assert(false);
}

EnemySpirit::EnemySpirit(float x, float y, float width, float height, EnemySpiritType type) : PhysicalEntity(x, y, width, height), m_type(type)
{
    // Empty
}

void EnemySpirit::update(float deltaTime)
{
    Entity::update(deltaTime);
    
    if (m_fStateTime > 0.5f)
    {
        m_isRequestingDeletion = true;
    }
}

EnemySpiritType EnemySpirit::getType()
{
    return m_type;
}