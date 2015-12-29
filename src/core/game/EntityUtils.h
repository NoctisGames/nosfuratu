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
#include "Rectangle.h"
#include "Vector2D.h"
#include "Jon.h"

#include <math.h>
#include <vector>

class PhysicalEntity;

class EntityUtils
{
public:
    static void applyAnchor(PhysicalEntity& entity, EntityAnchor anchor);
    
    static void attach(PhysicalEntity& entity, PhysicalEntity& to, bool leftOf, bool yCorrection);
    
    static void placeOn(PhysicalEntity& entity, PhysicalEntity& on, float yOffset = 0);
    
    template<typename T>
    static bool isLanding(Jon& jon, std::vector<T>& items, float deltaTime)
    {
        float jonVelocityY = jon.getVelocity().getY();
        float jonLowerLeftY = jon.getBounds().getLowerLeft().getY();
        float jonYDelta = fabsf(jonVelocityY * deltaTime);
        
        if (jonVelocityY <= 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (OverlapTester::doRectanglesOverlap(jon.getBounds(), (*i)->getBounds()))
                {
                    float itemTop = (*i)->getBounds().getTop();
                    float padding = itemTop * .01f;
                    padding += jonYDelta;
                    float itemTopReq = itemTop - padding;
                    
                    if (jonLowerLeftY >= itemTopReq)
                    {
                        jon.getPosition().setY(itemTop + jon.getBounds().getHeight() / 2 * .99f);
                        jon.updateBounds();
                        jon.setGroundSoundType((*i)->getGroundSoundType());
                        
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isFallingThroughHole(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        float entityVelocityY = entity.getVelocity().getY();
        float entityLowerLeftX = entity.getBounds().getLowerLeft().getX();
        float entityRight = entity.getBounds().getRight();
        
        if (entityVelocityY <= 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (!(*i)->hasCover())
                {
                    float itemLowerLeftX = (*i)->getBounds().getLowerLeft().getX();
                    float itemRight = (*i)->getBounds().getRight();
                    
                    if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getBounds()))
                    {
                        if (entityLowerLeftX >= itemLowerLeftX && entityRight <= itemRight)
                        {
                            return true;
                        }
                    }
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isBurrowingThroughHole(PhysicalEntity& entity, std::vector<T>& items)
    {
        float entityLowerLeftX = entity.getBounds().getLowerLeft().getX();
        float entityRight = entity.getBounds().getRight();
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            float itemLowerLeftX = (*i)->getBounds().getLowerLeft().getX();
            float itemRight = (*i)->getBounds().getRight();
            
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getBounds()))
            {
                if (entityLowerLeftX >= itemLowerLeftX && entityRight <= itemRight)
                {
                    (*i)->triggerBurrow();
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isLandingOnSpring(Jon& jon, std::vector<T>& items, float deltaTime)
    {
        float jonVelocityY = jon.getVelocity().getY();
        float jonLowerLeftY = jon.getBounds().getLowerLeft().getY();
        float jonYDelta = fabsf(jonVelocityY * deltaTime);
        
        if (jonVelocityY <= 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (OverlapTester::doRectanglesOverlap(jon.getBounds(), (*i)->getBounds()))
                {
                    float itemTop = (*i)->getBounds().getTop();
                    float padding = itemTop * .01f;
                    padding += jonYDelta;
                    float itemTopReq = itemTop - padding;
                    
                    if (jonLowerLeftY >= itemTopReq)
                    {
                        (*i)->trigger();
                        
                        if ((*i)->isBoosting())
                        {
                            jon.setBoostVelocity((*i)->getBoostVelocity());
                            
                            return true;
                        }
                    }
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isLandingOnEnemy(Jon& jon, std::vector<T>& items, float deltaTime)
    {
        float jonVelocityY = jon.getVelocity().getY();
        float jonLowerLeftY = jon.getBounds().getLowerLeft().getY();
        float jonYDelta = fabsf(jonVelocityY * deltaTime);
        
        if (jonVelocityY <= 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if ((*i)->canBeLandedOnToKill() && OverlapTester::doRectanglesOverlap(jon.getBounds(), (*i)->getBounds()))
                {
                    float itemTop = (*i)->getBounds().getTop();
                    float padding = itemTop * .01f;
                    padding += jonYDelta;
                    float itemTopReq = itemTop - padding;
                    
                    if (jonLowerLeftY >= itemTopReq)
                    {
                        (*i)->triggerHit();
                        
                        jon.setBoostVelocity(fabsf(jonVelocityY) / 1.5f);
                        
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
        float entityVelocityX = entity.getVelocity().getX();
        float entityBottom = entity.getBounds().getLowerLeft().getY();
        float entityRight = entity.getBounds().getRight();
        float entityXDelta = fabsf(entityVelocityX * deltaTime);
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getBounds()))
            {
                float itemTop = (*i)->getBounds().getTop();
                float itemTopReq = itemTop * 0.99f;
                
                float itemLeft = (*i)->getBounds().getLowerLeft().getX();
                float padding = itemLeft * .01f;
                padding += entityXDelta;
                float itemLeftReq = itemLeft + padding;
                
                if (entityRight <= itemLeftReq && entityBottom < itemTopReq)
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
                if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getBounds()))
                {
                    float itemLeft = (*i)->getBounds().getLowerLeft().getX();
                    
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
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getBounds()))
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
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (OverlapTester::doRectanglesOverlap(hittingBounds, (*i)->getBounds()))
            {
                (*i)->triggerHit();
                
                return true;
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isHorizontallyHittingAnEnemy(PhysicalEntity& entity, std::vector<T>& items)
    {
        Rectangle& bounds = entity.getBounds();
        Rectangle hittingBounds = Rectangle(bounds.getLowerLeft().getX(), bounds.getLowerLeft().getY() + bounds.getHeight() / 2, bounds.getWidth() * 1.2f, bounds.getHeight());
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if ((*i)->canBeHitHorizontally() && OverlapTester::doRectanglesOverlap(hittingBounds, (*i)->getBounds()))
            {
                (*i)->triggerHit();
                
                return true;
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isHit(PhysicalEntity& entity, std::vector<T>& items)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getBounds()))
            {
                return true;
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isKilledByEnemy(PhysicalEntity& entity, std::vector<T>& items)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if ((*i)->hasKilledJon())
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
            float originalY = entity.getPosition().getY();
            entity.getPosition().sub(0, entity.getVelocity().getY() * 0.5f);
            entity.updateBounds();
            
            float width = entity.getBounds().getWidth();
            float height = entity.getBounds().getHeight();
            float x = entity.getBounds().getLowerLeft().getX();
            float y = entity.getBounds().getLowerLeft().getY();
            
            Rectangle tempBounds = Rectangle(x, y, width, height);
            
            entity.getPosition().setY(originalY);
            entity.updateBounds();
            
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getBounds()))
                {
                    if (tempBounds.getLowerLeft().getY() > (*i)->getBounds().getTop())
                    {
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static void updateBackgrounds(std::vector<T>& items, Vector2D& cameraPosition)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            (*i)->update(cameraPosition);
        }
    }
    
    template<typename T>
    static void updateAndClean(std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); )
        {
            (*i)->update(deltaTime);
            
            if ((*i)->isRequestingDeletion())
            {
                (*i)->onDeletion();
                
                i = items.erase(i);
            }
            else
            {
                i++;
            }
        }
    }
    
    template<typename T>
    static void addAll(std::vector<T>& itemsFrom, std::vector<PhysicalEntity*>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back((*i).get());
        }
    }
    
    template<typename T>
    static int isTouching(std::vector<T*>& items, Vector2D& touchPoint)
    {
        int last = ((int) items.size()) - 1;
        for (int i = last; i >= 0; i--)
        {
            T* item = items.at(i);
            float width = item->getWidth();
            float height = item->getHeight();
            float x = item->getPosition().getX() - width / 2;
            float y = item->getPosition().getY() - height / 2;
            
            Rectangle tempBounds = Rectangle(x, y, width, height);
            if (OverlapTester::isPointInRectangle(touchPoint, tempBounds))
            {
                return i;
            }
        }
        
        return -1;
    }
    
    template<typename T>
    static int doRectanglesOverlap(std::vector<T>& items, PhysicalEntity* pe)
    {
        int index = 0;
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++, index++)
        {
            if ((*i).get() == pe)
            {
                continue;
            }
            
            if (OverlapTester::doRectanglesOverlap(pe->getBounds(), (*i)->getBounds()))
            {
                return index;
            }
        }
        
        return -1;
    }
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    EntityUtils();
    EntityUtils(const EntityUtils&);
    EntityUtils& operator=(const EntityUtils&);
};

#endif /* defined(__nosfuratu__EntityUtils__) */