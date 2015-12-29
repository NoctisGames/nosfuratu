//
//  SnakeSpirit.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SnakeSpirit.h"

SnakeSpirit::SnakeSpirit(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height)
{
    // Empty
}

void SnakeSpirit::update(float deltaTime)
{
    Entity::update(deltaTime);
    
    if (m_fStateTime > 1.00f)
    {
        m_isRequestingDeletion = true;
    }
}