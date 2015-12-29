//
//  SnakeHorned.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SnakeHorned.h"
#include "Game.h"
#include "OverlapTester.h"

#include <math.h>

SnakeHorned* SnakeHorned::create(float x, float y, int type)
{
    return new SnakeHorned(x, y);
}

SnakeHorned::SnakeHorned(float x, float y, float width, float height) : SnakeEnemy(x, y, width, height), m_isRising(false)
{
    updateBounds();
}

void SnakeHorned::update(float deltaTime)
{
    SnakeEnemy::update(deltaTime);
    
    if (m_isDying)
    {
        return;
    }
    
    updateBounds();
    
    Jon& jon = m_game->getJon();
    
    if (m_isRising)
    {
        float jonTop = jon.getBounds().getTop();
        
        if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
        {
            float itemTop = getBounds().getTop();
            
            if (jonTop > itemTop)
            {
                m_hasKilledJon = true;
            }
        }
        
        if (m_fStateTime > 1.10f)
        {
            m_isRising = false;
            m_fStateTime = 0;
        }
    }
    else if (jon.getBounds().getRight() > getBounds().getLowerLeft().getX() - 2
        && jon.getBounds().getLowerLeft().getY() > getBounds().getTop())
    {
        m_isRising = true;
        m_fStateTime = 0;
    }
}

void SnakeHorned::triggerHit()
{
    SnakeEnemy::triggerHit();
}

void SnakeHorned::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.171875f, getHeight() * 0.06388888888889f);
    
    m_bounds->setWidth(getWidth() * 0.3515625f);
    m_bounds->setHeight(getHeight() * 0.31944444444444f);
    
    if (m_isDying)
    {
        // Bye bye!
        lowerLeft.setY(1337);
        return;
    }
    
    if (m_isRising)
    {
        if (m_fStateTime > 1.00f)
        {
            m_bounds->setHeight(getHeight() * 0.39722222222222f);
        }
        else if (m_fStateTime > 0.90f)
        {
            m_bounds->setHeight(getHeight() * 0.52222222222222f);
        }
        else if (m_fStateTime > 0.80f)
        {
            m_bounds->setHeight(getHeight() * 0.63055555555556f);
        }
        else if (m_fStateTime > 0.70f)
        {
            m_bounds->setHeight(getHeight() * 0.86388888888889f);
        }
        else if (m_fStateTime > 0.60f)
        {
            m_bounds->setHeight(getHeight() * 0.85f);
        }
        else if (m_fStateTime > 0.50f)
        {
            m_bounds->setHeight(getHeight() * 0.86388888888889f);
        }
        else if (m_fStateTime > 0.40f)
        {
            m_bounds->setHeight(getHeight() * 0.85f);
        }
        else if (m_fStateTime > 0.30f)
        {
            m_bounds->setHeight(getHeight() * 0.63055555555556f);
        }
        else if (m_fStateTime > 0.20f)
        {
            m_bounds->setHeight(getHeight() * 0.52222222222222f);
        }
        else if (m_fStateTime > 0.10f)
        {
            m_bounds->setHeight(getHeight() * 0.39722222222222f);
        }
    }
}

bool SnakeHorned::canBeLandedOnToKill()
{
    return false;
}

bool SnakeHorned::isRising()
{
    return m_isRising;
}
