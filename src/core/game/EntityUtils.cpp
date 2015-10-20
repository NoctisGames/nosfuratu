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
            float y = GAME_HEIGHT - halfHeight;
            entity.getPosition().setY(y);
        }
            break;
        case ANCHOR_BOTTOM:
        {
            float halfHeight = entity.getHeight() / 2;
            entity.getPosition().setY(halfHeight);
        }
            break;
        case ANCHOR_NONE:
        default:
            break;
    }
}

void EntityUtils::attach(PhysicalEntity& entity, PhysicalEntity& to, bool leftOf, bool yCorrection)
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
    
    if (yCorrection)
    {
        float top = entity.getBounds().getTop();
        float topTo = to.getBounds().getTop();
        float yDelta = topTo - top;
        entity.getPosition().add(0, yDelta);
        entity.updateBounds();
    }
}

void EntityUtils::placeOn(PhysicalEntity& entity, PhysicalEntity& on, float yOffset)
{
    float halfHeight = entity.getBounds().getHeight() / 2;
    float top = on.getBounds().getTop();
    float y = top + halfHeight;
    
    entity.getPosition().setY(y);
    
    entity.updateBounds();
    
    float yDelta = top - entity.getBounds().getLowerLeft().getY();
    entity.getPosition().add(0, yDelta);
    
    entity.getPosition().add(0, yOffset);
    
    entity.updateBounds();
}