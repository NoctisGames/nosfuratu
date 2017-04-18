//
//  MidBossOwl.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 4/1/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "MidBossOwl.h"
#include "Game.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "Jon.h"
#include "macros.h"
#include "NGAudioEngine.h"
#include "GameConstants.h"
#include "NGRect.h"
#include "ForegroundObject.h"
#include "Ground.h"

#include <math.h>

MidBossOwl::MidBossOwl(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_game(nullptr),
m_fTreeTopY(24.0f),
m_fGroundTopYWithPadding(14),
m_fTimeUnderTreeTop(0),
m_fHighestSwoopSpeedX(0),
m_iDamage(0),
m_state(MidBossOwlState_Sleeping),
m_didJonTransform(false)
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
            if (m_fStateTime > 1.7f)
            {
                setState(MidBossOwlState_Dead);

				m_position.add(0, 1337);
            }
        }
            break;
        case MidBossOwlState_SlammingIntoTree:
        {
            if (m_fStateTime > 0.55f)
            {
                m_velocity.set(0, 0);
                m_acceleration.set(0, 4);
                
                setState(MidBossOwlState_FlyingOverTree);
            }
        }
            break;
        case MidBossOwlState_Pursuing:
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
                    m_velocity.setX(jon.getVelocity().getX());
                }
                
                if (m_velocity.getX() > RABBIT_DEFAULT_MAX_SPEED)
                {
                    m_velocity.setX(RABBIT_DEFAULT_MAX_SPEED);
                }
                
                if (getPosition().getY() + getHeight() / 2 < m_fTreeTopY)
                {
                    if (getMainBounds().getBottom() < m_fGroundTopYWithPadding
                        || jon.getPosition().getY() < 9
                        || m_velocity.getX() < 0)
                    {
                        getPosition().setY(m_fGroundTopYWithPadding + getMainBounds().getHeight() / 2);
                        
                        m_velocity.set(0, 0);
                        m_acceleration.set(0, 4);
                        
                        setState(MidBossOwlState_FlyingOverTree);
                        
                        return;
                    }
                    
                    m_velocity.setY(0);
                    
                    m_fTimeUnderTreeTop += deltaTime;
                    
                    if (m_fTimeUnderTreeTop > 1.1f)
                    {
                        Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());
                        
                        float angle = target.cpy().sub(m_position.getX(), m_position.getY()).angle();
                        float radians = DEGREES_TO_RADIANS(angle);
                        
                        m_velocity.add(cosf(radians) * 0.92f, sinf(radians) * 1.20f);
                        
                        if (target.dist(getMainBounds().getRight(), getMainBounds().getBottom()) < 6.0f)
                        {
                            m_velocity.add(cosf(radians) * 16, sinf(radians) * (m_didJonTransform ? 44 : 14));
                            
                            setState(MidBossOwlState_SwoopingDown);
                            
                            NG_AUDIO_ENGINE->playSound(SOUND_MID_BOSS_SWOOP_DOWN);
                            
                            break;
                        }
                    }
                }
                
                for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); ++i)
                {
                    if ((*i)->getType() == ForegroundObjectType_GiantShakingTree)
                    {
                        if (OverlapTester::doNGRectsOverlap((*i)->getMainBounds(), getMainBounds()))
                        {
                            m_velocity.set(0, 0);
                            m_acceleration.set(0, 4);
                            
                            setState(MidBossOwlState_FlyingOverTree);
                            
                            return;
                        }
                    }
                }
            }
        }
            break;
        case MidBossOwlState_SwoopingDown:
        {
            if (m_game->getJons().size() > 0)
            {
                Jon& jon = m_game->getJon();
                
                if (jon.getVelocity().getX() > 0)
                {
                    m_velocity.setX(jon.getVelocity().getX());
                    
                    if (m_velocity.getX() > RABBIT_DEFAULT_MAX_SPEED)
                    {
                        m_velocity.setX(RABBIT_DEFAULT_MAX_SPEED);
                    }
                }
                
                m_velocity.setY(0);
                
                if (getMainBounds().getBottom() < m_fGroundTopYWithPadding
                    || jon.getPosition().getY() < 9
                    || m_velocity.getX() < 0)
                {
					if (!m_giveFreeHit)
					{
						getPosition().setY(m_fGroundTopYWithPadding + getMainBounds().getHeight() / 2);

						m_velocity.set(0, 0);
						m_acceleration.set(0, 4);

						setState(MidBossOwlState_FlyingOverTree);
                        
                        return;
					}
                }
				else if (jon.getPosition().getY() > 15)
				{
					getPosition().setY(jon.getMainBounds().getTop() - 0.1f);
				}
                
                Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());
                
                float angle = target.cpy().sub(m_position.getX(), m_position.getY()).angle();
                float radians = DEGREES_TO_RADIANS(angle);
                
                m_velocity.add(cosf(radians) * 0.92f, sinf(radians) * 1.20f);
                m_velocity.add(cosf(radians) * 16, sinf(radians) * (m_didJonTransform ? 44 : 14));
                
                if (m_velocity.getX() < m_fHighestSwoopSpeedX)
                {
                    m_velocity.setX(m_fHighestSwoopSpeedX);
                }
                
                m_fHighestSwoopSpeedX = m_velocity.getX();
                
                if (getMainBounds().getBottom() < (jon.getMainBounds().getTop() - 0.5f))
                {
                    m_velocity.setY(0);
                }
                
                if (OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds()))
                {
                    if (jon.isVampire())
                    {
                        jon.kill();
                        
                        setState(MidBossOwlState_FlyingOverTree);
                    }
                    else if (jon.getAbilityState() != ABILITY_BURROW)
                    {
                        jon.consume(true);
                        
                        setState(MidBossOwlState_FlyingAwayAfterCatchingJon);
                    }
                    
                    m_velocity.set(0, 0);
                    m_acceleration.set(0, 4);
                    
                    return;
                }
                
                for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); ++i)
                {
                    if ((*i)->getType() == ForegroundObjectType_GiantShakingTree)
                    {
                        if (OverlapTester::doNGRectsOverlap((*i)->getMainBounds(), getMainBounds()))
                        {
                            if (!m_didJonTransform
								&& (m_giveFreeHit
								|| jon.getPosition().dist((*i)->getPosition()) < 7.0f))
                            {
                                m_iDamage++;
                                
                                m_acceleration.set(1.00f, 0);
                                m_velocity.set(-3, 0);
                                
                                setState(m_iDamage == 3 ? MidBossOwlState_Dying : MidBossOwlState_SlammingIntoTree);
                                
                                if (m_state == MidBossOwlState_Dying)
                                {
                                    m_fWidth = MID_BOSS_OWL_DYING_WIDTH;
                                    m_fHeight = MID_BOSS_OWL_DYING_HEIGHT;
                                    
                                    jon.onEnemyDestroyed();
                                    
                                    NG_AUDIO_ENGINE->playSound(SOUND_MID_BOSS_DEATH);
                                }
                                else
                                {
                                    NG_AUDIO_ENGINE->playSound(SOUND_MID_BOSS_TREE_SMASH);
                                }
                                
                                GiantShakingTree* gst = (GiantShakingTree *) (*i);
                                gst->triggerHit();
                            }
                            else
                            {
                                m_velocity.set(0, 0);
                                m_acceleration.set(0, 4);
                                
                                setState(MidBossOwlState_FlyingOverTree);
                                
                                return;
                            }
                        }
                    }
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
    m_velocity.set(0, 0);
    m_acceleration.set(0, 0);
    
    m_iDamage = 0;
    m_fWidth = MID_BOSS_OWL_SLEEPING_WIDTH;
    m_fHeight = MID_BOSS_OWL_SLEEPING_HEIGHT;
	m_giveFreeHit = false;
    
    setState(MidBossOwlState_Sleeping);
}

void MidBossOwl::awaken()
{
    setState(MidBossOwlState_Awakening);
}

void MidBossOwl::beginPursuit()
{
	m_giveFreeHit = false;
    m_fWidth = MID_BOSS_OWL_NORMAL_WIDTH;
    m_fHeight = MID_BOSS_OWL_NORMAL_HEIGHT;
    
    m_fTimeUnderTreeTop = 0;
    m_fHighestSwoopSpeedX = 0;
    
    m_didJonTransform = false;
    
    m_acceleration.set(0, 0);
    
    m_position.setY(m_fTreeTopY + getHeight() / 2);
    
    if (m_game->getJons().size() > 0)
    {
        Jon& jon = m_game->getJon();
        
        m_velocity.set(jon.getVelocity().getX(), -0.8f);
        
        m_position.setX(jon.getPosition().getX() - CAM_WIDTH * 0.70f);
    }
    
    setState(MidBossOwlState_Pursuing);
}

void MidBossOwl::setGame(Game* game)
{
    m_game = game;
    
    for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); ++i)
    {
        if ((*i)->getType() == ForegroundObjectType_GiantShakingTree
            || (*i)->getType() == ForegroundObjectType_GiantPerchTree)
        {
            m_fTreeTopY = (*i)->getMainBounds().getTop();
            break;
        }
    }
    
    for (std::vector<Ground*>::iterator i = m_game->getGrounds().begin(); i != m_game->getGrounds().end(); ++i)
    {
        if ((*i)->getType() == GroundType_GrassWithCaveLarge)
        {
            m_fGroundTopYWithPadding = (*i)->getMainBounds().getTop() + 0.5f;
            break;
        }
    }
}

int MidBossOwl::getDamage()
{
    return m_iDamage;
}

bool MidBossOwl::didJonTransform()
{
    return m_didJonTransform;
}

void MidBossOwl::givePlayerAFreeHit()
{
    m_giveFreeHit = true;
}

void MidBossOwl::setState(MidBossOwlState state)
{
    m_state = state;
    m_fStateTime = 0.0f;
}

RTTI_IMPL(MidBossOwl, PhysicalEntity);
