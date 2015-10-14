//
//  Jon.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Jon.h"
#include "EntityUtils.h"
#include "Game.h"
#include "GameConstants.h"
#include "OverlapTester.h"

#include <math.h>

Jon* Jon::create(float x, float y, int type)
{
    return new Jon(x, y);
}

Jon::Jon(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_state(JON_ALIVE), m_physicalState(PHYSICAL_GROUNDED), m_actionState(ACTION_NONE), m_abilityState(ABILITY_NONE), m_color(1, 1, 1, 1), m_fActionStateTime(0), m_fAbilityStateTime(0), m_fMaxSpeed(JON_DEFAULT_MAX_SPEED), m_fAccelerationX(JON_DEFAULT_ACCELERATION), m_iNumJumps(0), m_isLanding(false), m_isSpinningBackFistDelivered(false)
{
    resetBounds(width * 0.6796875f, height * 0.8203125f);
}

void Jon::update(float deltaTime, Game& game)
{
    for (std::vector<std::unique_ptr<DustCloud>>::iterator i = getDustClouds().begin(); i != getDustClouds().end(); )
    {
        (*i)->update(deltaTime);
        
        if ((*i)->isRequestingDeletion())
        {
            i = getDustClouds().erase(i);
        }
        else
        {
            i++;
        }
    }
    
    if (m_state == JON_DEAD)
    {
        return;
    }
    
    if (m_state == JON_DYING_FADING)
    {
        m_color.alpha -= deltaTime / 2;
        if (m_color.alpha < 0)
        {
            m_color.alpha = 0;
            setState(JON_DEAD);
        }
        
        return;
    }
    
    PhysicalEntity::update(deltaTime);
    
    if (game.isJonHit(deltaTime))
    {
        setState(JON_DYING_FADING);
    }
    
    if (m_actionState != ACTION_NONE)
    {
        m_fActionStateTime += deltaTime;
    }
    
    if (m_abilityState != ABILITY_NONE)
    {
        m_fAbilityStateTime += deltaTime;
    }

	if (m_position->getY() < -m_fHeight / 2)
	{
        setState(JON_DEAD);
	}
    
    bool wasGrounded = m_physicalState == PHYSICAL_GROUNDED;
    
	setState(game.isJonGrounded(deltaTime) ? PHYSICAL_GROUNDED : PHYSICAL_IN_AIR);

	if (m_physicalState == PHYSICAL_GROUNDED)
	{
        if (!wasGrounded)
        {
            m_isLanding = true;
            m_fStateTime = 0;
            
            m_dustClouds.push_back(std::unique_ptr<DustCloud>(new DustCloud(getPosition().getX(), getPosition().getY() - getHeight() / 2, fabsf(m_velocity->getY() / 12.6674061f))));
        }
        
        if (m_isLanding && m_fStateTime > 0.20f)
        {
            m_isLanding = false;
            m_fStateTime = 0.21f;
        }
        
		m_acceleration->set(m_fAccelerationX, 0);
		m_velocity->setY(0);
		m_iNumJumps = 0; 
        setState(ACTION_NONE);
	}
	else if (m_iNumJumps == 0)
	{
        m_velocity->sub(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
		m_acceleration->setY(GRAVITY);
		m_iNumJumps = 1;
	}
    
    if (game.isJonLandingOnSpring(deltaTime))
    {
        m_acceleration->setY(GRAVITY);
        m_velocity->setY(28);
        
        setState(ACTION_JUMPING);
        
        m_iNumJumps = 1;
    }
    
    if (game.isJonBlockedVertically(deltaTime))
    {
        m_velocity->sub(0, 2);
    }
    else if (game.isJonBlockedHorizontally(deltaTime))
    {
        m_velocity->sub(2, 0);
    }

	if (m_velocity->getX() > m_fMaxSpeed)
	{
		m_velocity->setX(m_fMaxSpeed);
	}
    
    if (m_abilityState == ABILITY_SPINNING_BACK_FIST)
    {
        if (m_fAbilityStateTime > 0.48f)
        {
            setState(ABILITY_NONE);
        }
        else if (!m_isSpinningBackFistDelivered && m_fAbilityStateTime > 0.30f)
        {
            m_isSpinningBackFistDelivered = game.isSpinningBackFistDelivered(deltaTime);
        }
    }
    
    if (isFalling())
    {
        setState(ACTION_NONE);
    }
}

void Jon::triggerJump()
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
	if (m_iNumJumps < 2)
	{
		jump();
	}
}

void Jon::triggerLeftAction()
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    // TODO
}

void Jon::triggerRightAction()
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    if (m_abilityState == ABILITY_SPINNING_BACK_FIST)
    {
        return;
    }
    
    setState(ABILITY_SPINNING_BACK_FIST);
    
    m_isSpinningBackFistDelivered = false;
}

void Jon::triggerUpAction()
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    // TODO
}

void Jon::triggerDownAction()
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    // TODO
}

std::vector<std::unique_ptr<DustCloud>>& Jon::getDustClouds()
{
    return m_dustClouds;
}

JonState Jon::getState()
{
    return m_state;
}

JonPhysicalState Jon::getPhysicalState()
{
    return m_physicalState;
}

JonActionState Jon::getActionState()
{
    return m_actionState;
}

JonAbilityState Jon::getAbilityState()
{
    return m_abilityState;
}

Color Jon::getColor()
{
    return m_color;
}

float Jon::getActionStateTime()
{
    return m_fActionStateTime;
}

float Jon::getAbilityStateTime()
{
    return m_fAbilityStateTime;
}

bool Jon::isLanding()
{
    return m_isLanding;
}

bool Jon::isFalling()
{
    return m_velocity->getY() < 0;
}

bool Jon::isDead()
{
	return m_state == JON_DEAD;
}

int Jon::getType()
{
    return -1;
}

#pragma mark private

void Jon::jump()
{
	m_acceleration->setY(GRAVITY);
	m_velocity->setY(13 - m_iNumJumps * 3);

    setState(m_iNumJumps == 0 ? ACTION_JUMPING : ACTION_DOUBLE_JUMPING);
    
	m_iNumJumps++;
}

void Jon::setState(JonState state)
{
    m_state = state;
    m_fStateTime = 0;
}

void Jon::setState(JonPhysicalState state)
{
	m_physicalState = state;
}

void Jon::setState(JonActionState state)
{
    m_actionState = state;
    m_fActionStateTime = 0;
}

void Jon::setState(JonAbilityState state)
{
    m_abilityState = state;
    m_fAbilityStateTime = 0;
}