//
//  Game.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Game__
#define __nosfuratu__Game__

#include "BackgroundSky.h"
#include "BackgroundTrees.h"
#include "BackgroundCave.h"
#include "Tree.h"
#include "Ground.h"
#include "LogVerticalTall.h"
#include "GroundPlatform.h"
#include "EndSign.h"
#include "Carrot.h"
#include "GoldenCarrot.h"
#include "Jon.h"
#include "OverlapTester.h"

#include <math.h>
#include <memory>
#include <vector>

class Game
{
public:
    Game();
    
    void update(float deltaTime);
    
    bool isJonGrounded();
    
    bool isJonBlockedHorizontally();
    
    bool isJonBlockedVertically();
    
    BackgroundSky& getBackgroundSky();
    
    BackgroundTrees& getBackgroundTrees();
    
    BackgroundCave& getBackgroundCave();
    
    std::vector<Tree>& getTrees();
    
    std::vector<Ground>& getGrounds();
    
    std::vector<LogVerticalTall>& getLogVerticalTalls();
    
    std::vector<GroundPlatform>& getPlatforms();
    
    std::vector<EndSign>& getEndSigns();
    
    std::vector<Carrot>& getCarrots();
    
    std::vector<GoldenCarrot>& getGoldenCarrots();
    
    Jon& getJon();
    
    float getFarRight();
    
    bool resetGame();
    
private:
    std::unique_ptr<BackgroundSky> m_backgroundSky;
    std::unique_ptr<BackgroundTrees> m_backgroundTrees;
    std::unique_ptr<BackgroundCave> m_backgroundCave;
    std::unique_ptr<std::vector<Tree>> m_trees;
    std::unique_ptr<std::vector<Ground>> m_grounds;
    std::unique_ptr<std::vector<LogVerticalTall>> m_logVerticalTalls;
    std::unique_ptr<std::vector<GroundPlatform>> m_platforms;
    std::unique_ptr<std::vector<EndSign>> m_endSigns;
    std::unique_ptr<std::vector<Carrot>> m_carrots;
    std::unique_ptr<std::vector<GoldenCarrot>> m_goldenCarrots;
    std::unique_ptr<Jon> m_jon;
    
    float m_fDeltaTime;
    bool m_resetGame;
    
    template<typename T>
    bool isLanding(PhysicalEntity& entity, std::vector<T>& items)
    {
        for (typename std::vector<T>::iterator itr = items.begin(); itr != items.end(); itr++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*itr).getBounds()))
            {
                float entityVelocityY = entity.getVelocity().getY();
                float entityLowerLeftY = entity.getBounds().getLowerLeft().getY();
                float itemTop = (*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight();
                float padding = itemTop * .01f;
                float entityYDelta = fabsf(entityVelocityY * m_fDeltaTime);
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
    bool isBlockedOnRight(PhysicalEntity& entity, std::vector<T>& items)
    {
        for (typename std::vector<T>::iterator itr = items.begin(); itr != items.end(); itr++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*itr).getBounds()))
            {
                float entityBottomY = entity.getBounds().getLowerLeft().getY();
                float itemTopY = (*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight();
                itemTopY *= 0.95f;
                
                if (entityBottomY < itemTopY)
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    bool isBlockedAbove(PhysicalEntity& entity, std::vector<T>& items)
    {
        for (typename std::vector<T>::iterator itr = items.begin(); itr != items.end(); itr++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*itr).getBounds()))
            {
                float entityVelocityY = entity.getVelocity().getY();
                float entityTopY = entity.getBounds().getLowerLeft().getY() + entity.getBounds().getHeight();
                float entityBottom = (*itr).getBounds().getLowerLeft().getY();
                float entityBottomReq = entityBottom * 0.99f;
                
                if (entityVelocityY > 0 && entityTopY > entityBottomReq)
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<typename T>
    bool isCollected(PhysicalEntity& entity, std::vector<T>& items)
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
};

#endif /* defined(__nosfuratu__Game__) */
