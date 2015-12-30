//
//  SnakeEnemy.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/27/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SnakeEnemy.h"
#include "SnakeGrunt.h"
#include "Game.h"
#include "OverlapTester.h"
#include "Assets.h"

SnakeEnemy::SnakeEnemy(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height), m_color(1, 1, 1, 1), m_fColorAdditive(0), m_fSpiritStateTime(0), m_fXOfDeath(0), m_fYOfDeath(0), m_isDying(false), m_isDead(false), m_hasKilledJon(false)
{
    // Empty
}

void SnakeEnemy::update(float deltaTime)
{
    updateBounds();
    
    if (m_isDead)
    {
        m_fSpiritStateTime += deltaTime;
        m_color.alpha -= deltaTime * 2;
        
        if (m_fSpiritStateTime > 1.00f)
        {
            m_isRequestingDeletion = true;
        }
        
        for (std::vector<std::unique_ptr<SnakeSpirit>>::iterator i = getSnakeSpirits().begin(); i != getSnakeSpirits().end(); i++)
        {
            (*i)->update(deltaTime);
        }
    }
    else if (m_isDying)
    {
        m_fColorAdditive += deltaTime * 4;
        
        if (m_fColorAdditive > 1)
        {
            m_fColorAdditive = 1;
            m_isDead = true;
            
            m_snakeSpirits.push_back(std::unique_ptr<SnakeSpirit>(new SnakeSpirit(m_fXOfDeath, m_fYOfDeath)));
        }
    }
    else
    {
        Entity::update(deltaTime);
        
        Jon& jon = m_game->getJon();
        
        if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
        {
            m_hasKilledJon = true;
        }
    }
}

void SnakeEnemy::triggerHit()
{
    m_isDying = true;
    
    m_fXOfDeath = getBounds().getLowerLeft().getX() + getBounds().getWidth() / 2;
    m_fYOfDeath = getBounds().getLowerLeft().getY() + getBounds().getHeight() / 2;
    
    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SNAKE_DEATH);
}

void SnakeEnemy::onDeletion()
{
    getSnakeSpirits().clear();
}

std::vector<std::unique_ptr<SnakeSpirit>>& SnakeEnemy::getSnakeSpirits()
{
    return m_snakeSpirits;
}

Color SnakeEnemy::getColor()
{
    return m_color;
}

float SnakeEnemy::getColorAdditive()
{
    return m_fColorAdditive;
}

bool SnakeEnemy::isDying()
{
    return m_isDying;
}

bool SnakeEnemy::isDead()
{
    return m_isDead;
}

bool SnakeEnemy::hasKilledJon()
{
    return m_hasKilledJon;
}

bool SnakeEnemy::canBeHitHorizontally()
{
    return true;
}

bool SnakeEnemy::canBeLandedOnToKill()
{
    return true;
}

bool SnakeEnemy::canBeHitFromBelow()
{
    return true;
}

void SnakeEnemy::setGame(Game* game)
{
    m_game = game;
}

int SnakeEnemy::getType()
{
    return -1;
}
