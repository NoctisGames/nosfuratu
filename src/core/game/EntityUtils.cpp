//
//  EntityUtils.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/23/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "EntityUtils.h"
#include "PhysicalEntity.h"
#include "GameConstants.h"

void EntityUtils::applyAnchor(PhysicalEntity& entity, EntityAnchor anchor, float offset)
{
    switch (anchor)
    {
        case ANCHOR_TOP:
        {
            float halfHeight = entity.getHeight() / 2;
            float y = GAME_HEIGHT - halfHeight;
            entity.getPosition().setY(y + offset);
        }
            break;
        case ANCHOR_BOTTOM:
        {
            float halfHeight = entity.getHeight() / 2;
            entity.getPosition().setY(halfHeight + offset);
        }
            break;
        case ANCHOR_GROUND:
        {
            float halfHeight = entity.getHeight() / 2;
            float y = ANCHOR_GROUND_Y + halfHeight;
            entity.getPosition().setY(y + offset);
        }
            break;
        case ANCHOR_CAVE:
        {
            float halfHeight = entity.getHeight() / 2;
            float y = ANCHOR_CAVE_Y + halfHeight;
            entity.getPosition().setY(y + offset);
        }
            break;
        case ANCHOR_NONE:
        default:
            break;
    }
}

void EntityUtils::attach(PhysicalEntity& entity, PhysicalEntity& to, bool leftOf)
{
    if (leftOf)
    {
        float left = to.getBounds().getLowerLeft().getX();
        float x = left - entity.getBounds().getWidth() / 2;
        
        entity.getPosition().setX(x);
    }
    else
    {
        float right = to.getBounds().getLowerLeft().getX() + to.getBounds().getWidth();
        float x = right + entity.getBounds().getWidth() / 2;
        
        entity.getPosition().setX(x);
    }
    
    entity.updateBounds();
}

void EntityUtils::placeOn(PhysicalEntity& entity, PhysicalEntity& on)
{
    float halfHeight = entity.getBounds().getHeight() / 2;
    float top = on.getBounds().getLowerLeft().getY() + on.getBounds().getHeight();
    float y = top + halfHeight;
    
    entity.getPosition().setY(y);
    
    entity.updateBounds();
}