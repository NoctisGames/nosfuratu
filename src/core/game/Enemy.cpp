//
//  Enemy.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
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
        case EnemyType_MushroomGround:
            return new MushroomGround(gridX, gridY);
        case EnemyType_MushroomCeiling:
            return new MushroomCeiling(gridX, gridY);
        case EnemyType_SnakeGrunt:
            return new SnakeGrunt(gridX, gridY);
        case EnemyType_Sparrow:
            return new Sparrow(gridX, gridY);
        case EnemyType_Toad:
            return new Toad(gridX, gridY);
        case EnemyType_Fox:
            return new Fox(gridX, gridY);
    }
    
    assert(false);
}

Enemy::Enemy(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX, float boundsY, float boundsWidth, float boundsHeight, EnemyType type, EnemySpiritType enemySpiritType, short deathSoundId) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, boundsX, boundsY, boundsWidth, boundsHeight), m_enemySpirit(nullptr), m_type(type), m_enemySpiritType(enemySpiritType), m_color(0, 1, 1, 1), m_fEnemySpiritStateTime(0), m_fXOfDeath(0), m_fYOfDeath(0), m_deathSoundId(deathSoundId), m_isDying(false), m_isDead(false), m_game(nullptr)
{
    // Empty
}

void Enemy::update(float deltaTime)
{
    updateBounds();
    
    if (m_isDead)
    {
        handleDead(deltaTime);
    }
    else if (m_isDying)
    {
        handleDying(deltaTime);
    }
    else
    {
        handleAlive(deltaTime);
    }
}

void Enemy::updateBounds()
{
    GridLockedPhysicalEntity::updateBounds();
    
    if (m_isDying)
    {
        // Bye bye!
        Vector2D &lowerLeft = getMainBounds().getLowerLeft();
        lowerLeft.setY(1337);
    }
}

void Enemy::triggerHit()
{
    m_isDying = true;
    
    m_fXOfDeath = getMainBounds().getLeft() + getMainBounds().getWidth() / 2;
    m_fYOfDeath = getMainBounds().getLowerLeft().getY() + getMainBounds().getHeight() / 2;
    
    Assets::getInstance()->addSoundIdToPlayQueue(m_deathSoundId);
}

bool Enemy::isJonLanding(Jon& jon, float deltaTime)
{
	float jonVelocityY = jon.getVelocity().getY();

	if (jonVelocityY <= 0)
	{
		if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
		{
			float jonLowerLeftY = jon.getMainBounds().getLowerLeft().getY();
			float jonYDelta = fabsf(jonVelocityY * deltaTime);

			float itemTop = getMainBounds().getTop();
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
    Rectangle& bounds = jon.getMainBounds();
    Rectangle hittingBounds = Rectangle(bounds.getLeft(), bounds.getLowerLeft().getY() + bounds.getHeight() / 2, bounds.getWidth() * 1.2f, bounds.getHeight());
    
    if (canBeHitHorizontally() && OverlapTester::doRectanglesOverlap(hittingBounds, getMainBounds()))
    {
        triggerHit();
        
        return true;
    }
    
    return false;
}

bool Enemy::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getMainBounds();
    
    if (canBeHitFromBelow() && OverlapTester::doRectanglesOverlap(bounds, getMainBounds()))
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

#pragma mark protected

void Enemy::handleDead(float deltaTime)
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

void Enemy::handleDying(float deltaTime)
{
    m_color.red += deltaTime * 4;
    
    if (m_color.red > 1)
    {
        m_color.red = 1;
        m_isDead = true;
        
        m_enemySpirit = EnemySpirit::create(m_fXOfDeath, m_fYOfDeath, m_enemySpiritType);
    }
}

void Enemy::handleAlive(float deltaTime)
{
    Entity::update(deltaTime);
    
    Jon& jon = m_game->getJon();
    
    if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
    {
        jon.kill();
    }
}

#pragma mark subclasses

void Mushroom::handleAlive(float deltaTime)
{
    Entity::update(deltaTime);
    
    Jon& jon = m_game->getJon();
    
    if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
    {
        jon.triggerBounceBackOffEnemy(-9);
        
        m_fStateTime = 0;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MUSHROOM_BOUNCE);
    }
}

bool MushroomGround::isJonLanding(Jon& jon, float deltaTime)
{
	float jonVelocityY = jon.getVelocity().getY();

	if (jonVelocityY <= 0)
	{
		if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
		{
			float jonLowerLeftY = jon.getMainBounds().getLowerLeft().getY();
			float jonYDelta = fabsf(jonVelocityY * deltaTime);

			float itemTop = getMainBounds().getTop();
			float padding = itemTop * .01f;
			padding += jonYDelta;
			float itemTopReq = itemTop - padding;

			if (jonLowerLeftY >= itemTopReq)
			{
				jon.getPosition().setY(itemTop + jon.getMainBounds().getHeight() / 2 * 1.01f);
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
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
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

void Sparrow::updateBounds()
{
    Enemy::updateBounds();
    
	Rectangle& camBounds = *m_game->getCameraBounds();

	if (camBounds.getWidth() > CAM_WIDTH)
	{
		return;
	}

    if (OverlapTester::doRectanglesOverlap(camBounds, getMainBounds()))
    {
        if (!m_isOnScreen)
        {
            m_isOnScreen = true;
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SPARROW_FLY);
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
        
        Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPARROW_FLY);
    }
}

void Toad::handleAlive(float deltaTime)
{
    Entity::update(deltaTime);
    
    Jon& jon = m_game->getJon();
    
    if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
    {
        m_fStateTime = 0;
        
        // TODO try to eat Jon
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_TOAD_EAT);
    }
}

void Fox::handleAlive(float deltaTime)
{
    Entity::update(deltaTime);
    
    Jon& jon = m_game->getJon();
    
    if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
    {
        m_fStateTime = 0;
        
        // TODO swing at Jon
    }
}