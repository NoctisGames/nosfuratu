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

#include <math.h>

Enemy* Enemy::create(int gridX, int gridY, int type)
{
    switch ((EnemyType)type)
    {
        case EnemyType_Mushroom:
            return new Mushroom(gridX, gridY);
        case EnemyType_MushroomCeiling:
            return new MushroomCeiling(gridX, gridY);
        case EnemyType_SnakeGrunt:
            return new SnakeGrunt(gridX, gridY);
        case EnemyType_Sparrow:
            return new Sparrow(gridX, gridY);
    }
    
    assert(false);
}

Enemy::Enemy(int gridX, int gridY, int gridWidth, int gridHeight, EnemyType type, EnemySpiritType enemySpiritType) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_enemySpirit(nullptr), m_type(type), m_enemySpiritType(enemySpiritType), m_color(0, 1, 1, 1), m_fEnemySpiritStateTime(0), m_fXOfDeath(0), m_fYOfDeath(0), m_isDying(false), m_isDead(false)
{
    // Empty
}

void Enemy::update(float deltaTime)
{
    updateBounds();
    
    if (m_isDead)
    {
        m_fEnemySpiritStateTime += deltaTime;
        m_color.alpha -= deltaTime * 2;
        
        if (m_fEnemySpiritStateTime > 1.00f)
        {
            m_isRequestingDeletion = true;
        }
        
        if (m_enemySpirit)
        {
            m_enemySpirit->update(deltaTime);
        }
    }
    else if (m_isDying)
    {
        m_color.red += deltaTime * 4;
        
        if (m_color.red > 1)
        {
            m_color.red = 1;
            m_isDead = true;
            
            m_enemySpirit = EnemySpirit::create(m_fXOfDeath, m_fYOfDeath, m_enemySpiritType);
        }
    }
    else
    {
        Entity::update(deltaTime);
        
        Jon& jon = m_game->getJon();
        
        if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
        {
            jon.kill();
        }
    }
}

void Enemy::updateBounds()
{
    GridLockedPhysicalEntity::updateBounds();
    
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
    
    m_fXOfDeath = getBounds().getLeft() + getBounds().getWidth() / 2;
    m_fYOfDeath = getBounds().getLowerLeft().getY() + getBounds().getHeight() / 2;
    
    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SNAKE_DEATH);
}

bool Enemy::isJonLanding(Jon& jon, float deltaTime)
{
	float jonVelocityY = jon.getVelocity().getY();

	if (jonVelocityY <= 0)
	{
		if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
		{
			float jonLowerLeftY = jon.getBounds().getLowerLeft().getY();
			float jonYDelta = fabsf(jonVelocityY * deltaTime);

			float itemTop = getBounds().getTop();
			float padding = itemTop * .01f;
			padding += jonYDelta;
			float itemTopReq = itemTop - padding;

			if (jonLowerLeftY >= itemTopReq)
			{
				triggerHit();

				float boost = fmaxf(fabsf(jonVelocityY) / 1.5f, 6);

				jon.triggerBoostOffEnemy(boost);
			}
		}
	}

	return false;
}

bool Enemy::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    return false;
}

bool Enemy::isJonHittingHorizontally(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getBounds();
    Rectangle hittingBounds = Rectangle(bounds.getLeft(), bounds.getLowerLeft().getY() + bounds.getHeight() / 2, bounds.getWidth() * 1.2f, bounds.getHeight());
    
    if (canBeHitHorizontally() && OverlapTester::doRectanglesOverlap(hittingBounds, getBounds()))
    {
        triggerHit();
        
        return true;
    }
    
    return false;
}

bool Enemy::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getBounds();
    
    if (canBeHitFromBelow() && OverlapTester::doRectanglesOverlap(bounds, getBounds()))
    {
        triggerHit();
        
        return true;
    }
    
    return false;
}

void Enemy::onDeletion()
{
    delete m_enemySpirit;
    m_enemySpirit = nullptr;
}

bool Enemy::hasSpirit()
{
    return m_enemySpirit ? true : false;
}

EnemySpirit& Enemy::getSpirit()
{
    return *m_enemySpirit;
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

bool Mushroom::isJonLanding(Jon& jon, float deltaTime)
{
	float jonVelocityY = jon.getVelocity().getY();

	if (jonVelocityY <= 0)
	{
		if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
		{
			float jonLowerLeftY = jon.getBounds().getLowerLeft().getY();
			float jonYDelta = fabsf(jonVelocityY * deltaTime);

			float itemTop = getBounds().getTop();
			float padding = itemTop * .01f;
			padding += jonYDelta;
			float itemTopReq = itemTop - padding;

			if (jonLowerLeftY >= itemTopReq)
			{
				jon.getPosition().setY(itemTop + jon.getBounds().getHeight() / 2 * 1.01f);
				jon.updateBounds();
				jon.triggerBoostOffEnemy(18);

				m_fStateTime = 0;
                
                Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MUSHROOM_BOUNCE);
			}
		}
	}

	return false;
}

bool MushroomCeiling::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
    {
        jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
        jon.updateBounds();
        
        jon.triggerBounceDownardsOffEnemy(-18);

		m_fStateTime = 0;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MUSHROOM_BOUNCE);
        
        return true;
    }
    
    return false;
}
