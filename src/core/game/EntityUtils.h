//
//  EntityUtils.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/23/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
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
    static void attach(GridLockedPhysicalEntity& entity, GridLockedPhysicalEntity& to, int gridCellSizeScalar, bool leftOf, bool yCorrection)
    {
        if (leftOf)
        {
            float left = to.getMainBounds().getLeft();
            float x = left - entity.getMainBounds().getWidth() / 2;
            
            entity.getPosition().setX(x);
        }
        else
        {
            float right = to.getMainBounds().getLeft() + to.getMainBounds().getWidth();
            float x = right + entity.getMainBounds().getWidth() / 2;
            
            entity.getPosition().setX(x);
        }
        
        entity.updateBounds();
        
        if (yCorrection)
        {
            float top = entity.getMainBounds().getTop();
            float topTo = to.getMainBounds().getTop();
            float yDelta = topTo - top;
            entity.getPosition().add(0, yDelta);
            entity.updateBounds();
        }
    }
    
    static void placeOn(GridLockedPhysicalEntity& entity, GridLockedPhysicalEntity& on, int gridCellSizeScalar)
    {
		float top = on.getMainBounds().getTop();

		entity.getMainBounds().getLowerLeft().setY(top);

		entity.snapToGrid(gridCellSizeScalar);
    }
    
    static void placeUnder(GridLockedPhysicalEntity& entity, GridLockedPhysicalEntity& under, int gridCellSizeScalar)
    {
        float boundsHeight = entity.getMainBounds().getHeight();
        float bottom = under.getMainBounds().getBottom();
        float lowerLeft = bottom - boundsHeight;
        
        entity.getMainBounds().getLowerLeft().setY(lowerLeft);

		entity.snapToGrid(gridCellSizeScalar);
    }
    
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
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (!(*i)->hasCover())
                {
                    if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), (*i)->getMainBounds()))
                    {
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isFallingThroughPit(Jon& jon, std::vector<T>& items, float deltaTime)
    {
        float entityLeft = jon.getMainBounds().getLeft();
        float entityRight = jon.getMainBounds().getRight();
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            float itemLowerLeftX = (*i)->getMainBounds().getLeft();
            float itemRight = (*i)->getMainBounds().getRight();
            
            if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), (*i)->getMainBounds()))
            {
                if (entityLeft >= itemLowerLeftX && entityRight <= itemRight)
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isBurrowingThroughHole(Jon& jon, std::vector<T>& items)
    {
        bool ret = false;
        if (jon.getVelocity().getY() <= 0)
        {
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), (*i)->getMainBounds()))
                {
                    if ((*i)->triggerBurrow(jon.getAcceleration().getY()))
                    {
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
    static bool isHittingFromBelow(Jon& jon, std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if ((*i)->isJonHittingFromBelow(jon, deltaTime))
            {
                return true;
            }
        }
        
        return false;
    }
    
    template<typename T>
    static bool isHorizontallyHitting(Jon& jon, std::vector<T>& items, float deltaTime)
    {
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            if ((*i)->isJonHittingHorizontally(jon, deltaTime))
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
            
            float width = entity.getMainBounds().getWidth();
            float height = entity.getMainBounds().getHeight();
            float x = entity.getMainBounds().getLeft();
            float y = entity.getMainBounds().getLowerLeft().getY();
            
            Rectangle tempBounds = Rectangle(x, y, width, height);
            
            entity.getPosition().setY(originalY);
            entity.updateBounds();
            
            for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (OverlapTester::doRectanglesOverlap(entity.getMainBounds(), (*i)->getMainBounds()))
                {
                    if (tempBounds.getLowerLeft().getY() > (*i)->getMainBounds().getTop())
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
    static int indexOfOverlappingObjectThatCanBePlacedOn(PhysicalEntity* pe, std::vector<T>& items)
    {
        int index = 0;
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++, index++)
        {
            if ((*i) == pe)
            {
                continue;
            }
            
            if ((*i)->canObjectBePlacedOn() && OverlapTester::doRectanglesOverlap(pe->getMainBounds(), (*i)->getMainBounds()))
            {
                return index;
            }
        }
        
        return -1;
    }
    
    template<typename T>
    static int indexOfOverlappingObjectThatCanBePlacedUnder(PhysicalEntity* pe, std::vector<T>& items)
    {
        int index = 0;
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++, index++)
        {
            if ((*i) == pe)
            {
                continue;
            }
            
            if ((*i)->canObjectBePlacedUnder() && OverlapTester::doRectanglesOverlap(pe->getMainBounds(), (*i)->getMainBounds()))
            {
                return index;
            }
        }
        
        return -1;
    }
    
    template<typename T>
    static void cleanUpVectorOfPointers(std::vector<T*>& items)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            delete (*i);
        }
        
        items.clear();
    }
    
    template<typename T>
    static void setGameToEntities(std::vector<T*>& items, Game* game)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            (*i)->setGame(game);
        }
    }
    
    template<typename T>
    static void copyAndOffset(std::vector<T*>& items, int beginGridX, int endGridX)
    {
        int gridSpacing = endGridX - beginGridX;
        float offset = gridSpacing * GRID_CELL_SIZE + GRID_CELL_SIZE / 3.0f;
        
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            if ((*i)->getGridX() >= endGridX)
            {
                (*i)->getPosition().add(offset, 0);
                (*i)->updateBounds();
                (*i)->snapToGrid(1);
            }
        }
        
		std::vector<T*> newItems;
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            if((*i)->getGridX() >= beginGridX && (*i)->getGridX() < endGridX)
            {
				newItems.push_back(T::create((*i)->getGridX() + gridSpacing, (*i)->getGridY(), (*i)->getType()));
            }
        }

		items.insert(items.end(), newItems.begin(), newItems.end());
    }
    
    template<typename T>
    static void offsetAll(std::vector<T*>& items, int beginGridX, int endGridX)
    {
        int gridSpacing = endGridX - beginGridX;
        float offset = gridSpacing * GRID_CELL_SIZE + GRID_CELL_SIZE / 3.0f;
        
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            (*i)->getPosition().add(offset, 0);
            (*i)->updateBounds();
            (*i)->snapToGrid(1);
        }
    }
    
    template<typename T>
    static void offsetAllInRangeOpenEnd(std::vector<T*>& items, int beginGridX, int endGridX, int gridOffset)
    {
        float offset = gridOffset * GRID_CELL_SIZE + GRID_CELL_SIZE / 3.0f;
        
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            if ((*i)->getGridX() >= beginGridX && (*i)->getGridX() < endGridX)
            {
                (*i)->getPosition().add(offset, 0);
                (*i)->updateBounds();
                (*i)->snapToGrid(1);
            }
        }
    }

	template<typename T>
	static void offsetAllInRangeClosedEnd(std::vector<T*>& items, int beginGridX, int endGridX, int gridOffset)
	{
		float offset = gridOffset * GRID_CELL_SIZE + GRID_CELL_SIZE / 3.0f;

		for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
		{
			if ((*i)->getGridX() >= beginGridX && (*i)->getGridX() <= endGridX)
			{
				(*i)->getPosition().add(offset, 0);
				(*i)->updateBounds();
				(*i)->snapToGrid(1);
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