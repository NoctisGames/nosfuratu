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
#include "Platform.h"

Jon::Jon(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height, 0), m_iNumJumps(0), m_isGrounded(false), m_isSpeedBoost(false), m_isDead(false)
{
    resetBounds(width * 0.6796875f, height * 0.8203125f);
    
    m_acceleration->setX(8);
}

void Jon::update(float deltaTime, Game& game)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_position->getY() < 0)
    {
        m_isDead = true;
    }
    
    m_isGrounded = false;
    
    for (std::vector<Ground>::iterator itr = game.getGrounds().begin(); itr != game.getGrounds().end(); itr++)
    {
        if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr).getBounds()))
        {
            m_position->setY((*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight() + m_bounds->getHeight() / 2 * .99f);
            m_isGrounded = true;
            
            updateBounds();
        }
    }
    
    for (std::vector<Platform>::iterator itr = game.getPlatforms().begin(); itr != game.getPlatforms().end(); itr++)
    {
        if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr).getBounds()))
        {
            if (m_bounds->getLowerLeft().getY() > (*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight() / 2)
            {
                m_position->setY((*itr).getBounds().getLowerLeft().getY() + (*itr).getBounds().getHeight() + m_bounds->getHeight() / 2 * .99f);
                m_isGrounded = true;
                
                updateBounds();
            }
        }
    }
    
    for (std::vector<Carrot>::iterator itr = game.getCarrots().begin(); itr != game.getCarrots().end(); )
    {
        if (OverlapTester::doRectanglesOverlap(*m_bounds, (*itr).getBounds()))
        {
            itr = game.getCarrots().erase(itr);
            m_velocity->setX(m_velocity->getX() + 2);
            m_fStateTime = 0;
            m_isSpeedBoost = true;
        }
        else
        {
            itr++;
        }
    }
    
    if (m_isGrounded)
    {
        m_acceleration->setX(8);
        m_acceleration->setY(0);
        m_velocity->setY(0);
        m_iNumJumps = 0;
    }
    else if(m_iNumJumps == 0)
    {
        m_acceleration->setY(GRAVITY);
        m_iNumJumps = 1;
    }
    
    if (m_isSpeedBoost && m_fStateTime > 1)
    {
        m_isSpeedBoost = false;
    }
    
    if (!m_isSpeedBoost && m_velocity->getX() > 8)
    {
        m_velocity->setX(8);
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
    m_velocity->setY(8 - m_iNumJumps * 2);
    
    m_iNumJumps++;
    
    m_fStateTime = 0;
}