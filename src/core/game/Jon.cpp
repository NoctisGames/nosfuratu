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
#include "Game.h"
#include "GameConstants.h"
#include "OverlapTester.h"
#include "GamePlatform.h"

#define JON_DEFAULT_ACCELERATION_SPEED_BOOST JON_DEFAULT_MAX_SPEED * 4
#define JON_DEFAULT_ACCELERATION JON_DEFAULT_MAX_SPEED
#define JON_MAX_SPEED_WITH_BOOST JON_DEFAULT_MAX_SPEED * 2

Jon::Jon(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height, 0), m_fAccelerationX(JON_DEFAULT_ACCELERATION), m_fMaxSpeed(JON_DEFAULT_MAX_SPEED), m_fSpeedBoostTime(0.0f), m_iNumJumps(0), m_isGrounded(false), m_isSpeedBoost(false), m_isDead(false)
{
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

	m_isGrounded = testGrounded(game);

	for (std::vector<Carrot>::iterator itr = game.getCarrots().begin(); itr != game.getCarrots().end(); )
	{
		if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr).getBounds()))
		{
			itr = game.getCarrots().erase(itr);
			m_fMaxSpeed = JON_MAX_SPEED_WITH_BOOST;
			m_fAccelerationX = JON_DEFAULT_ACCELERATION_SPEED_BOOST;
			m_acceleration->setX(m_fAccelerationX);
			m_fSpeedBoostTime = 0;
			m_isSpeedBoost = true;
		}
		else
		{
			itr++;
		}
	}

	if (m_isGrounded)
	{
		updateBounds();
		m_acceleration->setX(m_fAccelerationX);
		m_acceleration->setY(0);
		m_velocity->setY(0);
		m_iNumJumps = 0;
	}
	else if (m_iNumJumps == 0)
	{
		m_acceleration->setY(GRAVITY);
		m_iNumJumps = 1;
	}

	if (m_isSpeedBoost)
	{
		m_fSpeedBoostTime += deltaTime;

		if (m_fSpeedBoostTime > 3)
		{
			m_isSpeedBoost = false;
			m_fSpeedBoostTime = 0;
			m_fMaxSpeed = JON_DEFAULT_MAX_SPEED;
			m_fAccelerationX = JON_DEFAULT_ACCELERATION;
			m_acceleration->setX(m_fAccelerationX);
		}
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
	m_velocity->setY(9 - m_iNumJumps * 2);

	m_iNumJumps++;

	m_fStateTime = 0;
}

bool Jon::testGrounded(Game& game)
{
	for (std::vector<Ground>::iterator itr = game.getGrounds().begin(); itr != game.getGrounds().end(); itr++)
	{
		if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr).getBounds()))
		{
			m_position->setY((*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight() + m_bounds->getHeight() / 2 * .99f);

			return true;
		}
	}

	for (std::vector<GamePlatform>::iterator itr = game.getPlatforms().begin(); itr != game.getPlatforms().end(); itr++)
	{
		if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr).getBounds()))
		{
            if(m_velocity->getY() <= 0)
            {
                // blah
            }
			
            if (m_bounds->getLowerLeft().getY() > (*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight() / 2)
			{
				m_position->setY((*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight() + m_bounds->getHeight() / 2 * .99f);

				return true;
			}
		}
	}

	return false;
}