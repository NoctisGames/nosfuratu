//
//  Jon.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Jon.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "EntityUtils.h"
#include "Game.h"
#include "GameConstants.h"
#include "OverlapTester.h"
#include "GroundPlatform.h"

Jon::Jon(float x, float y, float width, float height, EntityAnchor anchor) : PhysicalEntity(x, y, width, height), m_fAccelerationX(JON_DEFAULT_ACCELERATION), m_fMaxSpeed(JON_DEFAULT_MAX_SPEED), m_iNumJumps(0), m_isGrounded(false), m_isLanding(false), m_isDead(false)
{
    EntityUtils::applyAnchor(*this, anchor);
    
	resetBounds(width * 0.6796875f, height * 0.8203125f);

	m_acceleration->setX(m_fAccelerationX);
}

void Jon::update(float deltaTime, Game& game)
{
	PhysicalEntity::update(deltaTime);

	if (m_position->getY() < -m_fHeight / 2)
	{
		m_isDead = true;
	}
    
    bool isGrounded = m_isGrounded;
    
    m_isGrounded = game.isJonGrounded();
    
    if (!isGrounded && m_isGrounded)
    {
        m_isLanding = true;
        m_fStateTime = 0;
    }
    
    if (m_isLanding && m_fStateTime > 0.20f)
    {
        m_isLanding = false;
        m_fStateTime = 0.21f;
    }

	if (m_isGrounded)
	{
		m_acceleration->set(m_fAccelerationX, 0);
		m_velocity->setY(0);
		m_iNumJumps = 0;
	}
	else if (m_iNumJumps == 0)
	{
		m_acceleration->setY(GRAVITY);
		m_iNumJumps = 1;
	}
    
    if (game.isJonBlockedHorizontally())
    {
        m_position->sub(m_velocity->getX() * deltaTime, 0);
        updateBounds();
    }
    
    if (game.isJonBlockedVertically())
    {
        m_position->sub(0, m_velocity->getY() * deltaTime);
        updateBounds();
    }

	if (m_velocity->getX() > m_fMaxSpeed)
	{
		m_velocity->setX(m_fMaxSpeed);
	}
}

void Jon::triggerJump()
{
	if (m_iNumJumps < 2)
	{
		jump();
	}
}

int Jon::getNumJumps()
{
	return m_iNumJumps;
}

bool Jon::isGrounded()
{
    return m_isGrounded;
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
	return m_isDead;
}

#pragma mark private

void Jon::jump()
{
	m_acceleration->setY(GRAVITY);
	m_velocity->setY(13 - m_iNumJumps * 3);

	m_iNumJumps++;

	m_fStateTime = 0;
}