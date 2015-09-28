//
//  EntityUtils.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/23/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__EntityUtils__
#define __nosfuratu__EntityUtils__

#include "EntityAnchor.h"
#include "OverlapTester.h"
#include "PhysicalEntity.h"
#include "DestructiblePhysicalEntity.h"

#include <math.h>
#include <vector>

class PhysicalEntity;

class EntityUtils
{
public:
    static void applyAnchor(PhysicalEntity& entity, EntityAnchor anchor);
    
    static void attach(PhysicalEntity& entity, PhysicalEntity& to, bool leftOf);
    
    template<typename T>
    static bool isLanding(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator itr = items.begin(); itr != items.end(); itr++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*itr).getBounds()))
            {
                float entityVelocityY = entity.getVelocity().getY();
                float entityLowerLeftY = entity.getBounds().getLowerLeft().getY();
                float itemTop = (*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight();
                float padding = itemTop * .01f;
                float entityYDelta = fabsf(entityVelocityY * deltaTime);
                padding += entityYDelta;
                float itemTopReq = itemTop - padding;
                
                if (entityVelocityY <= 0 && entityLowerLeftY >= itemTopReq)
                {
                    entity.getPosition().setY(itemTop + entity.getBounds().getHeight() / 2 * .99f);
                    entity.updateBounds();
                    
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isBlockedOnRight(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator itr = items.begin(); itr != items.end(); itr++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*itr).getBounds()))
            {
                float entityBottom = entity.getBounds().getLowerLeft().getY();
                float entityRight = entity.getBounds().getLowerLeft().getX() + entity.getBounds().getWidth();
                float itemLeft = (*itr).getBounds().getLowerLeft().getX();
                float itemTop = (*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight();
                float itemTopReq = itemTop * 0.99f;
                float padding = fabsf(entity.getVelocity().getX() * deltaTime);
                float itemLeftReq = itemLeft - padding;
                
                if (entityRight > itemLeftReq && entityBottom < itemTopReq)
                {
                    entity.getPosition().setX(itemLeft - entity.getBounds().getWidth() / 2 * 1.01f);
                    entity.updateBounds();
                    
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isBlockedAbove(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator itr = items.begin(); itr != items.end(); itr++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*itr).getBounds()))
            {
                float entityVelocityY = entity.getVelocity().getY();
                float entityLeft = entity.getBounds().getLowerLeft().getX();
                float itemLeft = (*itr).getBounds().getLowerLeft().getX();
                
                if (entityVelocityY > 0 && itemLeft < entityLeft)
                {
                    entity.getPosition().sub(0, entity.getVelocity().getY() * deltaTime);
                    entity.updateBounds();
                    
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isCollected(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        bool retval = false;
        
        for (typename std::vector<T>::iterator itr = items.begin(); itr != items.end(); )
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*itr).getBounds()))
            {
                retval = true;
                itr = items.erase(itr);
            }
            else
            {
                itr++;
            }
        }
        
        return retval;
    }
    
    template<typename T>
    static bool isHit(PhysicalEntity& entity, std::vector<T>& items)
    {
        for (typename std::vector<T>::iterator itr = items.begin(); itr != items.end(); )
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*itr).getBounds()))
            {
                (*itr).triggerHit();
                
                if ((*itr).isDestroyed())
                {
                    itr = items.erase(itr);
                }
                
                return true;
            }
            else
            {
                itr++;
            }
        }
        
        return false;
    }
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    EntityUtils();
    EntityUtils(const EntityUtils&);
    EntityUtils& operator=(const EntityUtils&);
};

#endif /* defined(__nosfuratu__EntityUtils__) */