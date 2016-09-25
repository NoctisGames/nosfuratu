//
//  EndBossSnake.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/24/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "EndBossSnake.h"
#include "Game.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "Jon.h"
#include "macros.h"

#include <math.h>

EndBossSnake::EndBossSnake(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_game(nullptr),
m_fTreeTopY(24.0f),
m_fGroundTopYWithPadding(14),
m_fTimeUnderTreeTop(0),
m_iDamage(0),
m_state(EndBossSnakeState_Sleeping),
m_didJonTransform(false)
{
    resetBounds(width * 0.5f, height * 0.5f);
}

void EndBossSnake::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    switch (m_state)
    {
        case EndBossSnakeState_Dying:
        {
            if (m_fStateTime > 1.7f)
            {
                setState(EndBossSnakeState_Dead);

				m_position->add(0, 1337);
            }
        }
            break;
        case EndBossSnakeState_SlammingIntoTree:
        {
            if (m_fStateTime > 0.55f)
            {
                m_velocity->set(0, 0);
                m_acceleration->set(0, 4);
                
                setState(EndBossSnakeState_FlyingOverTree);
            }
        }
            break;
        case EndBossSnakeState_Pursuing:
        {
            if (m_game->getJons().size() > 0)
            {
                Jon& jon = m_game->getJon();
                
                if (jon.isVampire())
                {
                    m_didJonTransform = true;
                }
                
                if (jon.getVelocity().getX() > 0)
                {
                    m_velocity->setX(jon.getVelocity().getX());
                }
                
                if (m_velocity->getX() > RABBIT_DEFAULT_MAX_SPEED)
                {
                    m_velocity->setX(RABBIT_DEFAULT_MAX_SPEED);
                }
                
                if (getPosition().getY() + getHeight() / 2 < m_fTreeTopY)
                {
                    if (getMainBounds().getBottom() < m_fGroundTopYWithPadding
                        || jon.getPosition().getY() < 9
                        || m_velocity->getX() < 0)
                    {
                        getPosition().setY(m_fGroundTopYWithPadding + getMainBounds().getHeight() / 2);
                        
                        m_velocity->set(0, 0);
                        m_acceleration->set(0, 4);
                        
                        setState(EndBossSnakeState_FlyingOverTree);
                        
                        return;
                    }
                    
                    m_velocity->setY(0);
                    
                    m_fTimeUnderTreeTop += deltaTime;
                    
                    if (m_fTimeUnderTreeTop > 2.4f)
                    {
                        Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());
                        
                        float angle = target.cpy().sub(m_position->getX(), m_position->getY()).angle();
                        float radians = DEGREES_TO_RADIANS(angle);
                        
                        m_velocity->add(cosf(radians) * 0.80f, sinf(radians) * 1.25f);
                        
                        if (target.dist(getMainBounds().getRight(), getMainBounds().getBottom()) < 6.0f)
                        {
                            m_velocity->add(cosf(radians) * 16, sinf(radians) * (jon.isVampire() ? 24 : 16));
                            
                            setState(EndBossSnakeState_SwoopingDown);
                            
                            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MID_BOSS_SWOOP_DOWN);
                            
                            break;
                        }
                    }
                }
                
                for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); i++)
                {
                    if ((*i)->getType() == ForegroundObjectType_GiantShakingTree)
                    {
                        if (OverlapTester::doRectanglesOverlap((*i)->getMainBounds(), getMainBounds()))
                        {
                            m_velocity->set(0, 0);
                            m_acceleration->set(0, 4);
                            
                            setState(EndBossSnakeState_FlyingOverTree);
                            
                            return;
                        }
                    }
                }
            }
        }
            break;
        case EndBossSnakeState_SwoopingDown:
        {
            if (m_game->getJons().size() > 0)
            {
                Jon& jon = m_game->getJon();
                
                if (jon.getVelocity().getX() > 0)
                {
                    m_velocity->setX(jon.getVelocity().getX());
                    
                    if (m_velocity->getX() > RABBIT_DEFAULT_MAX_SPEED)
                    {
                        m_velocity->setX(RABBIT_DEFAULT_MAX_SPEED);
                    }
                }
                
                m_velocity->setY(0);
                
                if (getMainBounds().getBottom() < m_fGroundTopYWithPadding
                    || jon.getPosition().getY() > 15
                    || jon.getPosition().getY() < 9
                    || m_velocity->getX() < 0)
                {
					if (!m_giveFreeHit)
					{
						getPosition().setY(m_fGroundTopYWithPadding + getMainBounds().getHeight() / 2);

						m_velocity->set(0, 0);
						m_acceleration->set(0, 4);

						setState(EndBossSnakeState_FlyingOverTree);
					}
                    
                    return;
                }
                
                Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());
                
                float angle = target.cpy().sub(m_position->getX(), m_position->getY()).angle();
                float radians = DEGREES_TO_RADIANS(angle);
                
                m_velocity->add(cosf(radians) * 0.80f, sinf(radians) * 1.25f);
                m_velocity->add(cosf(radians) * 16, sinf(radians) * (m_didJonTransform ? 40 : 16));
                
                if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
                {
                    if (jon.isVampire())
                    {
                        jon.kill();
                        
                        setState(EndBossSnakeState_FlyingOverTree);
                    }
                    else if (!jon.isBurrowEffective())
                    {
                        jon.consume(true);
                        
                        setState(EndBossSnakeState_FlyingAwayAfterCatchingJon);
                    }
                    
                    m_velocity->set(0, 0);
                    m_acceleration->set(0, 4);
                    
                    return;
                }
                
                for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); i++)
                {
                    if ((*i)->getType() == ForegroundObjectType_GiantShakingTree)
                    {
                        if (OverlapTester::doRectanglesOverlap((*i)->getMainBounds(), getMainBounds()))
                        {
                            if (m_giveFreeHit
								|| jon.getPosition().dist((*i)->getPosition()) < 7.0f)
                            {
                                m_iDamage++;
                                
                                m_acceleration->set(1.00f, 0);
                                m_velocity->set(-3, 0);
                                
                                setState(m_iDamage == 3 ? EndBossSnakeState_Dying : EndBossSnakeState_SlammingIntoTree);
                                
                                if (m_state == EndBossSnakeState_Dying)
                                {
                                    m_fWidth = MID_BOSS_OWL_DYING_WIDTH;
                                    m_fHeight = MID_BOSS_OWL_DYING_HEIGHT;
                                    
                                    jon.onEnemyDestroyed();
                                    
                                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MID_BOSS_DEATH);
                                }
                                else
                                {
                                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MID_BOSS_TREE_SMASH);
                                }
                                
                                GiantShakingTree* gst = (GiantShakingTree *) (*i);
                                gst->triggerHit();
                            }
                            else
                            {
                                m_velocity->set(0, 0);
                                m_acceleration->set(0, 4);
                                
                                setState(EndBossSnakeState_FlyingOverTree);
                                
                                return;
                            }
                        }
                    }
                }
            }
        }
            break;
        case EndBossSnakeState_FlyingAwayAfterCatchingJon:
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
        case EndBossSnakeState_Awakening:
        {
            if (m_fStateTime > 0.20f)
            {
                setState(EndBossSnakeState_Screeching);
            }
        }
            break;
        case EndBossSnakeState_Dead:
        case EndBossSnakeState_Screeching:
        case EndBossSnakeState_Sleeping:
        case EndBossSnakeState_FlyingOverTree:
        default:
            break;
    }
}

EndBossSnakeState EndBossSnake::getState()
{
    return m_state;
}

void EndBossSnake::goBackToSleep()
{
    m_velocity->set(0, 0);
    m_acceleration->set(0, 0);
    
    m_iDamage = 0;
    m_fWidth = MID_BOSS_OWL_SLEEPING_WIDTH;
    m_fHeight = MID_BOSS_OWL_SLEEPING_HEIGHT;
	m_giveFreeHit = false;
    
    setState(EndBossSnakeState_Sleeping);
}

void EndBossSnake::awaken()
{
    setState(EndBossSnakeState_Awakening);
}

void EndBossSnake::beginPursuit()
{
	m_giveFreeHit = false;
    m_fWidth = MID_BOSS_OWL_NORMAL_WIDTH;
    m_fHeight = MID_BOSS_OWL_NORMAL_HEIGHT;
    
    m_fTimeUnderTreeTop = 0;
    
    m_didJonTransform = false;
    
    m_acceleration->set(0, 0);
    
    m_position->setY(m_fTreeTopY + getHeight() / 2);
    
    if (m_game->getJons().size() > 0)
    {
        Jon& jon = m_game->getJon();
        
        m_velocity->set(jon.getVelocity().getX(), -0.8f);
        
        m_position->setX(jon.getPosition().getX() - CAM_WIDTH * 0.70f);
    }
    
    setState(EndBossSnakeState_Pursuing);
}

void EndBossSnake::setGame(Game* game)
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
            m_fGroundTopYWithPadding = (*i)->getMainBounds().getTop() + 0.5f;
            break;
        }
    }
}

int EndBossSnake::getDamage()
{
    return m_iDamage;
}

void EndBossSnake::setState(EndBossSnakeState state)
{
    m_state = state;
    m_fStateTime = 0.0f;
}
