//
//  EntityUtils.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/23/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__EntityUtils__
#define __nosfuratu__EntityUtils__

#include "OverlapTester.h"
#include "PhysicalEntity.h"
#include "GridLockedPhysicalEntity.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "Jon.h"

#include <math.h>
#include <vector>

class PhysicalEntity;

class EntityUtils
{
public:
    template<typename T>
    static bool isLanding(Jon& jon, std::vector<T>& items, float deltaTime)
    {
        float jonVelocityY = jon.getVelocity().getY();
        
        if (jonVelocityY <= 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if ((*i)->isJonLanding(jon, deltaTime))
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isFallingThroughHole(Jon& jon, std::vector<T>& items, float deltaTime)
    {
        if (jon.getVelocity().getY() <= 0)
        {
            float entityLeft = jon.getBounds().getLeft();
            float entityRight = jon.getBounds().getRight();
            
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (!(*i)->hasCover())
                {
                    float itemLowerLeftX = (*i)->getBounds().getLowerLeft().getX();
                    float itemRight = (*i)->getBounds().getRight();
                    
                    if (OverlapTester::doRectanglesOverlap(jon.getBounds(), (*i)->getBounds()))
                    {
                        if (entityLeft >= itemLowerLeftX && entityRight <= itemRight)
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
    static bool isBurrowingThroughHole(Jon& jon, std::vector<T>& items)
    {
        float entityLeft = jon.getBounds().getLeft();
        float entityRight = jon.getBounds().getRight();
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            float itemLeft = (*i)->getBounds().getLeft();
            float itemRight = (*i)->getBounds().getRight();
            
            if (OverlapTester::doRectanglesOverlap(jon.getBounds(), (*i)->getBounds()))
            {
                if (entityLeft >= itemLeft && entityRight <= itemRight)
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
        bool ret = false;
        
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
                        
                        float boost = fmaxf(fabsf(jonVelocityY) / 1.5f, 2);
                        
                        jon.setBoostVelocity(boost);
                        
                        ret = true;
                    }
                }
            }
        }
        
        return ret;
    }
    
    template<typename T>
    static bool isBlockedOnRight(Jon& jon, std::vector<T*>& items, float deltaTime)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            if ((*i)->isJonBlockedOnRight(jon, deltaTime))
            {
                return true;
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isBlockedAbove(Jon& jon, std::vector<T>& items, float deltaTime)
    {
        float entityVelocityY = jon.getVelocity().getY();
        
        if (entityVelocityY > 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if ((*i)->isJonBlockedAbove(jon, deltaTime))
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static void handleCollections(PhysicalEntity& entity, std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getBounds()))
            {
                (*i)->collect();
            }
        }
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
    static bool isHittingEnemyFromBelow(Jon& jon, std::vector<T>& items)
    {
        bool retval = false;
        
        Rectangle& bounds = jon.getBounds();
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (jon.getAbilityState() == ABILITY_UPWARD_THRUST && (*i)->canBeHitFromBelow() && OverlapTester::doRectanglesOverlap(bounds, (*i)->getBounds()))
            {
                (*i)->triggerHit();
                
                retval = true;
            }
        }
        
        return retval;
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
    static void updateBackgrounds(std::vector<T>& items, Vector2D& cameraPosition, float deltaTime)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            (*i)->update(cameraPosition, deltaTime);
        }
    }
    
    template<typename T>
    static void update(std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            (*i)->update(deltaTime);
        }
    }
    
    template<typename T>
    static void updateAndClean(std::vector<T*>& items, float deltaTime)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); )
        {
            (*i)->update(deltaTime);
            
            if ((*i)->isRequestingDeletion())
            {
                (*i)->onDeletion();
                
                delete *i;
                i = items.erase(i);
            }
            else
            {
                i++;
            }
        }
    }
    
    template<typename T>
    static void addAll(std::vector<T>& itemsFrom, std::vector<GridLockedPhysicalEntity*>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back((*i));
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