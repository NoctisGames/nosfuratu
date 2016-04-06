//
//  MidBossOwl.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 4/1/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MidBossOwl.h"
#include "Game.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "Jon.h"

#include <math.h>

MidBossOwl::MidBossOwl(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_game(nullptr),
m_iDamage(0),
m_state(MidBossOwlState_Sleeping),
m_hasCaughtVampire(false),
m_iNumTreesPassed(0)
{
    // Empty
}

void MidBossOwl::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_velocity->getX() > RABBIT_DEFAULT_MAX_SPEED + 1)
    {
        m_velocity->setX(RABBIT_DEFAULT_MAX_SPEED + 1);
    }
    
    switch (m_state)
    {
        case MidBossOwlState_Dying:
        {
            // TODO
        }
            break;
        case MidBossOwlState_SlammingIntoTree:
        {
            if (m_fStateTime > 4.00f)
            {
                setState(MidBossOwlState_FlyingOverTree);
                
                m_velocity->set(0, 12);
            }
        }
            break;
        case MidBossOwlState_FlyingOverTree:
        {
            float treeTopY = 0.0;
            float treeRightX = 8000.0;
            int numTreesPassed = 0;
            for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); i++)
            {
                if ((*i)->getType() == ForegroundObjectType_GiantShakingTree)
                {
                    numTreesPassed++;
                    
                    treeTopY = (*i)->getMainBounds().getTop();
                    if (numTreesPassed > m_iNumTreesPassed)
                    {
                        treeRightX = (*i)->getMainBounds().getRight();
                        break;
                    }
                }
            }
            
            if (getMainBounds().getLeft() > treeRightX)
            {
                m_velocity->set(0, 0);
                m_acceleration->set(RABBIT_DEFAULT_ACCELERATION + 1, -0.005f);
                
                m_iNumTreesPassed++;
                
                setState(MidBossOwlState_Pursuing);
            }
            else if (getMainBounds().getBottom() > treeTopY)
            {
                m_velocity->set(0, 0);
                m_acceleration->set(RABBIT_DEFAULT_ACCELERATION + 1, 0);
            }
        }
            break;
        case MidBossOwlState_Pursuing:
        {
            if (m_game->getJons().size() > 0)
            {
                Jon& jon = m_game->getJon();
                
                if (getMainBounds().getBottom() < jon.getMainBounds().getTop())
                {
                    m_acceleration->setY(0);
                    
                    setState(MidBossOwlState_SwoopingDown);
                }
            }
        }
            break;
        case MidBossOwlState_SwoopingDown:
        {
            for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); i++)
            {
                if ((*i)->getType() == ForegroundObjectType_GiantShakingTree)
                {
                    if (OverlapTester::doRectanglesOverlap((*i)->getMainBounds(), getMainBounds()))
                    {
                        m_iDamage++;
                        
                        m_acceleration->set(1.6f, 0);
                        m_velocity->set(-8, 0);
                        
                        setState(m_iDamage == 3 ? MidBossOwlState_Dying : MidBossOwlState_SlammingIntoTree);
                        
                        GiantShakingTree* gst = (GiantShakingTree *) (*i);
                        gst->triggerHit();
                    }
                }
            }
            
            if (m_game->getJons().size() > 0)
            {
                Jon& jon = m_game->getJon();
                
                if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
                {
                    jon.consume(true);
                    
                    m_hasCaughtVampire = jon.isVampire();
                    
                    m_velocity->set(0, 0);
                    m_acceleration->set(0, 6);
                    
                    setState(MidBossOwlState_FlyingAwayAfterCatchingJon);
                }
            }
        }
            break;
        case MidBossOwlState_FlyingAwayAfterCatchingJon:
        {
            if (m_fStateTime > 3)
            {
                if (m_game->getJons().size() > 0)
                {
                    Jon& jon = m_game->getJon();
                    
                    jon.kill();
                }
            }
        }
            break;
        case MidBossOwlState_Awakening:
        {
            if (m_fStateTime > 0.20f)
            {
                setState(MidBossOwlState_Screeching);
            }
        }
            break;
        case MidBossOwlState_Dead:
        case MidBossOwlState_Screeching:
        case MidBossOwlState_Sleeping:
        default:
            break;
    }
}

MidBossOwlState MidBossOwl::getState()
{
    return m_state;
}

void MidBossOwl::goBackToSleep()
{
    m_fWidth = MID_BOSS_OWL_SLEEPING_WIDTH;
    m_fHeight = MID_BOSS_OWL_SLEEPING_HEIGHT;
    setState(MidBossOwlState_Sleeping);
}

void MidBossOwl::awaken()
{
    setState(MidBossOwlState_Awakening);
}

void MidBossOwl::beginPursuit()
{
    m_fWidth = MID_BOSS_OWL_NORMAL_WIDTH;
    m_fHeight = MID_BOSS_OWL_NORMAL_HEIGHT;
    
    m_velocity->set(0, 12);
    
    setState(MidBossOwlState_FlyingOverTree);
}

void MidBossOwl::setGame(Game* game)
{
    m_game = game;
}

int MidBossOwl::getDamage()
{
    return m_iDamage;
}

bool MidBossOwl::hasCaughtVampire()
{
    return m_hasCaughtVampire;
}

void MidBossOwl::setState(MidBossOwlState state)
{
    m_state = state;
    m_fStateTime = 0.0f;
}