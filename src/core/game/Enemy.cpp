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
	if (calcIsJonLanding(jon, deltaTime))
    {
        triggerHit();
        
        float boost = fmaxf(fabsf(jon.getVelocity().getY()) / 1.5f, 6);
        
        jon.onEnemyDestroyed();
        jon.triggerBoostOffEnemy(boost);
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
    
    if (OverlapTester::doRectanglesOverlap(hittingBounds, getMainBounds()))
    {
        triggerHit();
        
        jon.onEnemyDestroyed();
        
        return true;
    }
    
    return false;
}

bool Enemy::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getMainBounds();
    
    if (OverlapTester::doRectanglesOverlap(bounds, getMainBounds()))
    {
        triggerHit();
        
        jon.onEnemyDestroyed();
        
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

void Enemy::setGame(Game* game)
{
    m_game = game;
}

EnemyType Enemy::getType()
{
    return m_type;
}

#pragma mark protected

void Enemy::handleAlive(float deltaTime)
{
    Entity::update(deltaTime);
    
    Jon& jon = m_game->getJon();
    
    if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
    {
        jon.kill();
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

bool Enemy::calcIsJonLanding(Jon &jon, float deltaTime)
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
                return true;
            }
        }
    }
    
    return false;
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
    if (calcIsJonLanding(jon, deltaTime))
    {
        float itemTop = getMainBounds().getTop();
        jon.getPosition().setY(itemTop + jon.getMainBounds().getHeight() / 2 * 1.01f);
        jon.updateBounds();
        jon.triggerBoostOffEnemy(18);
        
        m_fStateTime = 0;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MUSHROOM_BOUNCE);
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
    PhysicalEntity::update(deltaTime);
    
    Jon& jon = m_game->getJon();
    
    if (m_isEating)
    {
        if (m_hasSwallowedJon)
        {
            // TODO ?
        }
        else
        {
            if (m_fStateTime > 0.40f)
            {
                m_hasSwallowedJon = true;
                
                jon.getPosition().set(getMainBounds().getLeft() + getMainBounds().getWidth() / 2, getPosition().getY());
                jon.updateBounds();
                
                if (!m_isJonVampire)
                {
                    jon.kill();
                }
            }
            else if (m_fStateTime > 0.10f)
            {
                if (jon.getMainBounds().getBottom() < getMainBounds().getTop()
                    && jon.getMainBounds().getRight() > getMainBounds().getLeft() - 1.2f
                    && jon.getMainBounds().getRight() < getMainBounds().getLeft())
                {
                    jon.consume();
                }
                else
                {
                    m_fStateTime = 0;
                    m_isEating = false;
                }
            }
        }
    }
    else
    {
        float jonPredictedRight = jon.getMainBounds().getRight() + jon.getVelocity().getX() * 0.10f;
        if (jon.getMainBounds().getBottom() > (getMainBounds().getBottom() - 1.0f)
			&& jon.getMainBounds().getTop() < (getMainBounds().getTop() + 2.0f)
            && jonPredictedRight > getMainBounds().getLeft() - 1.2f
            && jonPredictedRight < getMainBounds().getLeft())
        {
            m_fStateTime = 0;
            m_isEating = true;
            
            m_isJonVampire = jon.shouldUseVampireFormForConsumeAnimation();
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_TOAD_EAT);
        }
    }
}

void Toad::handleDying(float deltaTime)
{
    m_fStateTime = 0;
    m_isDead = true;
    m_isDeadPart1 = true;
}

void Toad::handleDead(float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if (m_isDeadPart1)
    {
        if (m_hasSwallowedJon && m_isJonVampire)
        {
            if (m_fStateTime > 0.10f)
            {
                m_fStateTime = 0;
                m_isDeadPart1 = false;
            }
        }
        else
        {
            if (m_fStateTime > 0.25f)
            {
                m_fStateTime = 0;
                m_isDeadPart1 = false;
            }
        }
    }
    else
    {
        if (m_fStateTime > 0.35f)
        {
            m_isRequestingDeletion = true;
        }
    }
}

bool Fox::isJonLanding(Jon& jon, float deltaTime)
{
    if (calcIsJonLanding(jon, deltaTime))
    {
        float jonVelocityY = jon.getVelocity().getY();
        float jonAccelY = jon.getAcceleration().getY();
        
        if (jonAccelY < JON_GRAVITY * 2)
        {
            triggerHit();
            
            jon.onEnemyDestroyed();
        }
        
        m_fStateTime = 0;
        m_isBeingHit = true;
        m_isHitting = false;
        m_velocity->setX(0);
        
        float itemTop = getMainBounds().getTop();
        jon.getPosition().setY(itemTop + jon.getMainBounds().getHeight() / 2 * 1.01f);
        jon.updateBounds();
        
        float boost = fmaxf(fabsf(jonVelocityY) / 1.5f, 6);
        
        jon.triggerBoostOffEnemy(boost);
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOX_BOUNCED_ON);
    }
    
    return false;
}

void Fox::handleAlive(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    bool isTooFarLeft = false;
    bool isTooFarRight = false;
    if (m_position->getX() - m_fStartingX > 8)
    {
        // Fox has wandered too far away from its starting position
        m_velocity->setX(0);
        isTooFarRight = true;
    }
    else if (m_position->getX() - m_fStartingX < -8)
    {
        // Fox has wandered too far away from its starting position
        m_velocity->setX(0);
        isTooFarLeft = true;
    }
    
    if (m_isBeingHit)
    {
        if (m_fStateTime > 0.30f)
        {
            m_isBeingHit = false;
        }
        
        return;
    }
    
    Jon& jon = m_game->getJon();
    
    if (m_isHitting)
    {
        if (m_fStateTime > 0.55f)
        {
            m_velocity->setX(0);
            m_isHitting = false;
        }
    }
    else
    {
        if (jon.getMainBounds().getBottom() < getMainBounds().getTop()
            && jon.getMainBounds().getRight() > getMainBounds().getLeft() - 4
            && jon.getMainBounds().getRight() < getMainBounds().getLeft())
        {
            m_fStateTime = 0;
            m_isHitting = true;
            m_isLeft = true;
            
            m_velocity->setX(isTooFarLeft ? 0 : -7);
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOX_STRIKE);
        }
        else if (jon.getMainBounds().getBottom() < getMainBounds().getTop()
                 && jon.getMainBounds().getLeft() < getMainBounds().getRight() + 4
                 && jon.getMainBounds().getLeft() > getMainBounds().getRight())
        {
            m_fStateTime = 0;
            m_isHitting = true;
            m_isLeft = false;
            
            m_velocity->setX(isTooFarRight ? 0 : 7);
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOX_STRIKE);
        }
        else
        {
            if (m_fStateTime > 0.45f)
            {
                m_fStateTime = 0;
                m_isLeft = !m_isLeft;
                m_velocity->setX(m_isLeft ? -4.0f : 4.0f);
            }
            else if (m_fStateTime > 0.30f)
            {
                m_velocity->setX(0);
            }
        }
    }
        
    if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
    {
        jon.kill();
    }
}

void Fox::handleDying(float deltaTime)
{
    m_fStateTime = 0;
    m_isDead = true;
    
    float halfHeight = m_fHeight / 2;
    m_fHeight *= 2;
    m_position->add(0, halfHeight);
    updateBounds();
}

void Fox::handleDead(float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if (m_fStateTime > 0.50f)
    {
        m_isRequestingDeletion = true;
    }
}