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

void EntityUtils::applyAnchor(PhysicalEntity& entity, EntityAnchor anchor)
{
    switch (anchor)
    {
        case ANCHOR_TOP:
        {
            float halfHeight = entity.getHeight() / 2;
            double y = GAME_HEIGHT - halfHeight;
            entity.getPosition().setY(y);
        }
            break;
        case ANCHOR_BOTTOM:
        {
            float halfHeight = entity.getHeight() / 2;
            entity.getPosition().setY(halfHeight);
        }
            break;
        case ANCHOR_GROUND:
        {
            float halfHeight = entity.getHeight() / 2;
            double y = ANCHOR_GROUND_Y + halfHeight;
            entity.getPosition().setY(y);
        }
            break;
        case ANCHOR_GROUND_WITH_CAVE:
        {
            float halfHeight = entity.getHeight() / 2;
            double y = ANCHOR_GROUND_WITH_CAVE_Y + halfHeight;
            entity.getPosition().setY(y);
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
        float left = to.getPosition().getX() - to.getWidth() / 2;
        float x = left - entity.getWidth() / 2;
        
        entity.getPosition().setX(x);
    }
    else
    {
        float right = to.getPosition().getX() + to.getWidth() / 2;
        float x = right + entity.getWidth() / 2;
        
        entity.getPosition().setX(x);
    }
    
    Vector2D &lowerLeft = entity.getBounds().getLowerLeft();
    lowerLeft.setX(entity.getPosition().getX() - entity.getBounds().getWidth() / 2);
}