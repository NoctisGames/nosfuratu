//
//  Spirit.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "Spirit.h"
#include "EntityUtils.h"

Spirit* Spirit::create(float x, float y, SpiritType type)
{
    switch (type)
    {
        case SpiritType_Snake:
            return new SpiritSnake(x, y);
    }
    
    assert(false);
}

Spirit::Spirit(float x, float y, float width, float height, SpiritType type) : PhysicalEntity(x, y, width, height), m_type(type)
{
    // Empty
}

void Spirit::update(float deltaTime)
{
    Entity::update(deltaTime);
    
    if (m_fStateTime > 0.5f)
    {
        m_isRequestingDeletion = true;
    }
}

SpiritType Spirit::getType()
{
    return m_type;
}