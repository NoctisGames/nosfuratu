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
#include "Game.h"
#include "Rectangle.h"
#include "Vector2D.h"

#include <math.h>
#include <vector>

class PhysicalEntity;

class EntityUtils
{
public:
    static void applyAnchor(PhysicalEntity& entity, EntityAnchor anchor, float offset = 0);
    
    static void attach(PhysicalEntity& entity, PhysicalEntity& to, bool leftOf);
    
    static void placeOn(PhysicalEntity& entity, PhysicalEntity& on);
    
    template<typename T>
    static bool isLanding(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        float entityVelocityY = entity.getVelocity().getY();
        float entityLowerLeftY = entity.getBounds().getLowerLeft().getY();
        float entityYDelta = fabsf(entityVelocityY * deltaTime);
        
        if (entityVelocityY <= 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i).getBounds()))
                {
                    float itemTop = (*i).getBounds().getLowerLeft().getY() + (*i).getBounds().getHeight();
                    float padding = itemTop * .01f;
                    padding += entityYDelta;
                    float itemTopReq = itemTop - padding;
                    
                    if (entityLowerLeftY >= itemTopReq)
                    {
                        entity.getPosition().setY(itemTop + entity.getBounds().getHeight() / 2 * .99f);
                        entity.updateBounds();
                        
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isLandingOnSpring(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        float entityVelocityY = entity.getVelocity().getY();
        float entityLowerLeftY = entity.getBounds().getLowerLeft().getY();
        float entityYDelta = fabsf(entityVelocityY * deltaTime);
        
        if (entityVelocityY <= 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i).getBounds()))
                {
                    float itemTop = (*i).getBounds().getLowerLeft().getY() + (*i).getBounds().getHeight();
                    float padding = itemTop * .01f;
                    padding += entityYDelta;
                    float itemTopReq = itemTop - padding;
                    
                    if (entityLowerLeftY >= itemTopReq)
                    {
                        (*i).trigger();
                        
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isBlockedOnRight(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        float entityBottom = entity.getBounds().getLowerLeft().getY();
        float entityRight = entity.getBounds().getLowerLeft().getX() + entity.getBounds().getWidth();
        float padding = fabsf(entity.getVelocity().getX() * deltaTime);
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i).getBounds()))
            {
                float itemLeft = (*i).getBounds().getLowerLeft().getX();
                float itemTop = (*i).getBounds().getLowerLeft().getY() + (*i).getBounds().getHeight();
                float itemTopReq = itemTop * 0.99f;
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
        float entityVelocityY = entity.getVelocity().getY();
        float entityLeft = entity.getBounds().getLowerLeft().getX();
        
        if (entityVelocityY > 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i).getBounds()))
                {
                    float itemLeft = (*i).getBounds().getLowerLeft().getX();
                    
                    if (itemLeft < entityLeft)
                    {
                        entity.getPosition().sub(0, entity.getVelocity().getY() * deltaTime);
                        entity.updateBounds();
                        
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isCollected(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        bool retval = false;
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); )
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i).getBounds()))
            {
                retval = true;
                i = items.erase(i);
            }
            else
            {
                i++;
            }
        }
        
        return retval;
    }
    
    template<typename T>
    static bool isHitting(PhysicalEntity& entity, std::vector<T>& items)
    {
        Rectangle& bounds = entity.getBounds();
        Rectangle hittingBounds = Rectangle(bounds.getLowerLeft().getX(), bounds.getLowerLeft().getY() + bounds.getHeight() / 2, bounds.getWidth() * 1.2f, bounds.getHeight());
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); )
        {
            if (OverlapTester::doRectanglesOverlap(hittingBounds, (*i).getBounds()))
            {
                (*i).triggerHit();
                
                if ((*i).isDestroyed())
                {
                    i = items.erase(i);
                }
                
                return true;
            }
            else
            {
                i++;
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isHit(PhysicalEntity& entity, std::vector<T>& items)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i).getBounds()))
            {
                return true;
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isFallingIntoDeath(PhysicalEntity& entity, std::vector<T>& items)
    {
        float entityVelocityY = entity.getVelocity().getY();
        if (entityVelocityY < 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i).getBounds()))
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static void updateBackgrounds(std::vector<T>& items, Game& game)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            i->update(game);
        }
    }
    
    template<typename T>
    static void update(std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            i->update(deltaTime);
        }
    }
    
    template<typename T>
    static void updateAndClean(std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); )
        {
            i->update(deltaTime);
            
            if (i->isDestroyed())
            {
                i = items.erase(i);
            }
            else
            {
                i++;
            }
        }
    }
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    EntityUtils();
    EntityUtils(const EntityUtils&);
    EntityUtils& operator=(const EntityUtils&);
};

#endif /* defined(__nosfuratu__EntityUtils__) */