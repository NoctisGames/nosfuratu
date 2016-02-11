//
//  Enemy.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "Enemy.h"
#include "Game.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "Jon.h"

Enemy* Enemy::create(int gridX, int gridY, int type)
{
    switch ((EnemyType)type)
    {
        case EnemyType_Mushroom:
            return nullptr;
        case EnemyType_MushroomCeiling:
            return nullptr;
        case EnemyType_SnakeGrunt:
            return new SnakeGrunt(gridX, gridY);
        case EnemyType_Sparrow:
            return new Sparrow(gridX, gridY);
    }
    
    assert(false);
}

Enemy::Enemy(int gridX, int gridY, int gridWidth, int gridHeight, EnemyType type, SpiritType spiritType) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_spirit(nullptr), m_type(type), m_spiritType(spiritType), m_color(1, 1, 1, 1), m_fSpiritStateTime(0), m_fXOfDeath(0), m_fYOfDeath(0), m_isDying(false), m_isDead(false), m_hasKilledJon(false)
{
    // Empty
}

void Enemy::update(float deltaTime)
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
        
        m_spirit->update(deltaTime);
    }
    else if (m_isDying)
    {
        m_color.red += deltaTime * 4;
        
        if (m_color.red > 1)
        {
            m_color.red = 1;
            m_isDead = true;
            
            m_spirit = Spirit::create(m_fXOfDeath, m_fYOfDeath, m_spiritType);
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

void Enemy::updateBounds()
{
    PhysicalEntity::updateBounds();
    
    if (m_isDying)
    {
        // Bye bye!
        Vector2D &lowerLeft = m_bounds->getLowerLeft();
        lowerLeft.setY(1337);
    }
}

void Enemy::triggerHit()
{
    m_isDying = true;
    
    m_color.red = 0;
    
    m_fXOfDeath = getBounds().getLowerLeft().getX() + getBounds().getWidth() / 2;
    m_fYOfDeath = getBounds().getLowerLeft().getY() + getBounds().getHeight() / 2;
    
    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SNAKE_DEATH);
}

void Enemy::onDeletion()
{
    delete m_spirit;
    m_spirit = nullptr;
}

bool Enemy::hasSpirit()
{
    return m_spirit ? true : false;
}

Spirit& Enemy::getSpirit()
{
    return *m_spirit;
}

Color Enemy::getColor()
{
    return m_color;
}

bool Enemy::isDying()
{
    return m_isDying;
}

bool Enemy::isDead()
{
    return m_isDead;
}

bool Enemy::hasKilledJon()
{
    return m_hasKilledJon;
}

bool Enemy::canBeHitHorizontally()
{
    return true;
}

bool Enemy::canBeLandedOnToKill()
{
    return true;
}

bool Enemy::canBeHitFromBelow()
{
    return true;
}

void Enemy::setGame(Game* game)
{
    m_game = game;
}

EnemyType Enemy::getType()
{
    return m_type;
}
