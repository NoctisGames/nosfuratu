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
#include "macros.h"

#include <math.h>

MidBossOwl::MidBossOwl(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_game(nullptr),
m_fTreeTopY(24.0f),
m_fGroundTopYWithPadding(14),
m_fTimeUnderTreeTop(0),
m_iDamage(0),
m_state(MidBossOwlState_Sleeping),
m_hasCaughtVampire(false)
{
    resetBounds(width * 0.5f, height * 0.5f);
}

void MidBossOwl::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    switch (m_state)
    {
        case MidBossOwlState_Dying:
        {
            // TODO
        }
            break;
        case MidBossOwlState_SlammingIntoTree:
        {
            if (m_fStateTime > 2)
            {
                m_velocity->set(0, 0);
                m_acceleration->set(0, 0);
                
                setState(MidBossOwlState_FlyingOverTree);
            }
        }
            break;
        case MidBossOwlState_Pursuing:
        {
            if (m_game->getJons().size() > 0)
            {
                Jon& jon = m_game->getJon();
                
                if (getPosition().getY() + getHeight() / 2 < m_fTreeTopY)
                {
                    m_velocity->setX(jon.getVelocity().getX());
                    m_velocity->setY(0);
                    
                    m_fTimeUnderTreeTop += deltaTime;
                    
                    if (m_fTimeUnderTreeTop > 3)
                    {
                        Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());
                        
                        float angle = target.cpy().sub(m_position->getX(), m_position->getY()).angle();
                        float radians = DEGREES_TO_RADIANS(angle);
                        
                        m_velocity->add(cosf(radians) * (1.0f + (0.15f * m_iDamage)), sinf(radians) * (1.0f + (0.15f * m_iDamage)));
                        
                        if (target.dist(getMainBounds().getRight(), getMainBounds().getBottom()) < (9.0f - m_iDamage))
                        {
                            m_velocity->add(cosf(radians) * (3.0f + m_iDamage), sinf(radians) * (3.0f + m_iDamage));
                            
                            setState(MidBossOwlState_SwoopingDown);
                            
                            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MID_BOSS_SWOOP_DOWN);
                        }
                        
                        for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); i++)
                        {
                            if ((*i)->getType() == ForegroundObjectType_GiantShakingTree)
                            {
                                if (OverlapTester::doRectanglesOverlap((*i)->getMainBounds(), getMainBounds()))
                                {
                                    m_velocity->set(0, 0);
                                    m_acceleration->set(0, 4);
                                    
                                    setState(MidBossOwlState_FlyingOverTree);
                                }
                            }
                        }
                        
                        if (getMainBounds().getBottom() < m_fGroundTopYWithPadding)
                        {
                            getPosition().setY(m_fGroundTopYWithPadding + getMainBounds().getHeight() / 2);
                        }
                    }
                }
                else
                {
                    m_velocity->setX(jon.getVelocity().getX());
                }
            }
        }
            break;
        case MidBossOwlState_SwoopingDown:
        {
            if (getMainBounds().getBottom() < m_fGroundTopYWithPadding)
            {
                getPosition().setY(m_fGroundTopYWithPadding + getMainBounds().getHeight() / 2);
            }
            
            for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); i++)
            {
                if ((*i)->getType() == ForegroundObjectType_GiantShakingTree)
                {
                    if (OverlapTester::doRectanglesOverlap((*i)->getMainBounds(), getMainBounds()))
                    {
                        m_iDamage++;
                        
                        m_acceleration->set(1.00f, 0);
                        m_velocity->set(-3, 0);
                        
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
                    m_acceleration->set(0, 4);
                    
                    setState(MidBossOwlState_FlyingAwayAfterCatchingJon);
                }
            }
        }
            break;
        case MidBossOwlState_FlyingAwayAfterCatchingJon:
        {
            if (m_fStateTime > 1.0f)
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
        case MidBossOwlState_FlyingOverTree:
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
    m_velocity->set(0, 0);
    m_acceleration->set(0, 0);
    
    m_iDamage = 0;
    m_hasCaughtVampire = false;
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
    
    m_fTimeUnderTreeTop = 0;
    
    m_acceleration->set(0, 0);
    
    m_position->setY(m_fTreeTopY + getHeight() / 2);
    
    if (m_game->getJons().size() > 0)
    {
        Jon& jon = m_game->getJon();
        
        m_velocity->set(jon.getVelocity().getX(), -0.8f);
        
        m_position->setX(jon.getPosition().getX() - CAM_WIDTH * 0.70f);
    }
    
    setState(MidBossOwlState_Pursuing);
}

void MidBossOwl::setGame(Game* game)
{
    m_game = game;
    
    for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); i++)
    {
        if ((*i)->getType() == ForegroundObjectType_GiantShakingTree
            || (*i)->getType() == ForegroundObjectType_GiantPerchTree)
        {
            m_fTreeTopY = (*i)->getMainBounds().getTop();
            break;
        }
    }
    
    for (std::vector<Ground*>::iterator i = m_game->getGrounds().begin(); i != m_game->getGrounds().end(); i++)
    {
        if ((*i)->getType() == GroundType_GrassWithCaveLarge)
        {
            m_fGroundTopYWithPadding = (*i)->getMainBounds().getTop() + 1;
            break;
        }
    }
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