//
//  ForegroundObject.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "ForegroundObject.h"

#include "EntityUtils.h"
#include "Assets.h"
#include "Game.h"

ForegroundObject* ForegroundObject::create(int gridX, int gridY, int type)
{
    switch ((ForegroundObjectType)type)
    {
        case ForegroundObjectType_GrassPlatformLeft:
            return new GrassPlatformLeft(gridX, gridY);
        case ForegroundObjectType_GrassPlatformCenter:
            return new GrassPlatformCenter(gridX, gridY);
        case ForegroundObjectType_GrassPlatformRight:
            return new GrassPlatformRight(gridX, gridY);
            
        case ForegroundObjectType_CavePlatformLeft:
            return new CavePlatformLeft(gridX, gridY);
        case ForegroundObjectType_CavePlatformCenter:
            return new CavePlatformCenter(gridX, gridY);
        case ForegroundObjectType_CavePlatformRight:
            return new CavePlatformRight(gridX, gridY);
            
        case ForegroundObjectType_RockLarge:
            return new RockLarge(gridX, gridY);
        case ForegroundObjectType_RockMedium:
            return new RockMedium(gridX, gridY);
        case ForegroundObjectType_RockSmall:
        case ForegroundObjectType_RockSmallCracked:
            return new RockSmall(gridX, gridY);
            
        case ForegroundObjectType_StumpBig:
            return new StumpBig(gridX, gridY);
        case ForegroundObjectType_StumpSmall:
            return new StumpSmall(gridX, gridY);
            
        case ForegroundObjectType_EndSign:
            return new EndSign(gridX, gridY);
            
        case ForegroundObjectType_ThornsLeft:
            return new ThornsLeft(gridX, gridY);
        case ForegroundObjectType_ThornsCenterSmall:
            return new ThornsCenterSmall(gridX, gridY);
        case ForegroundObjectType_ThornsCenterBig:
            return new ThornsCenterBig(gridX, gridY);
        case ForegroundObjectType_ThornsRight:
            return new ThornsRight(gridX, gridY);
            
        case ForegroundObjectType_LogVerticalTall:
            return new LogVerticalTall(gridX, gridY);
        case ForegroundObjectType_LogVerticalShort:
            return new LogVerticalShort(gridX, gridY);
            
        case ForegroundObjectType_JumpSpringLightFlush:
            return new JumpSpringLightFlush(gridX, gridY);
        case ForegroundObjectType_JumpSpringLight:
            return new JumpSpringLight(gridX, gridY);
        case ForegroundObjectType_JumpSpringMedium:
            return new JumpSpringMedium(gridX, gridY);
        case ForegroundObjectType_JumpSpringHeavy:
            return new JumpSpringHeavy(gridX, gridY);
            
        case ForegroundObjectType_SpikeGrassSingle:
            return new SpikeGrassSingle(gridX, gridY);
        case ForegroundObjectType_SpikeGrassFour:
            return new SpikeGrassFour(gridX, gridY);
        case ForegroundObjectType_SpikeGrassEight:
            return new SpikeGrassEight(gridX, gridY);
            
        case ForegroundObjectType_SpikeCaveSingle:
            return new SpikeCaveSingle(gridX, gridY);
        case ForegroundObjectType_SpikeCaveFour:
            return new SpikeCaveFour(gridX, gridY);
        case ForegroundObjectType_SpikeCaveEight:
            return new SpikeCaveEight(gridX, gridY);
            
        case ForegroundObjectType_SpikeCaveCeilingSingle:
            return new SpikeCaveCeilingSingle(gridX, gridY);
        case ForegroundObjectType_SpikeCaveCeilingFour:
            return new SpikeCaveCeilingFour(gridX, gridY);
        case ForegroundObjectType_SpikeCaveCeilingEight:
            return new SpikeCaveCeilingEight(gridX, gridY);
            
        case ForegroundObjectType_SpikeWallSingle:
            return new SpikeWallSingle(gridX, gridY);
        case ForegroundObjectType_SpikeWallFour:
            return new SpikeWallFour(gridX, gridY);
        case ForegroundObjectType_SpikeWallEight:
            return new SpikeWallEight(gridX, gridY);
            
        case ForegroundObjectType_SpikeStar:
            return new SpikeStar(gridX, gridY);
            
        case ForegroundObjectType_VerticalSaw:
            return new VerticalSaw(gridX);
            
        case ForegroundObjectType_GiantTree:
            return new GiantTree(gridX, gridY);
        case ForegroundObjectType_GiantShakingTree:
            return new GiantShakingTree(gridX, gridY);
        case ForegroundObjectType_GiantPerchTree:
            return new GiantPerchTree(gridX, gridY);
            
        case ForegroundObjectType_SpikeTower:
            return new SpikeTower(gridX);
        case ForegroundObjectType_SpikeTowerBg:
            return new SpikeTowerBg(gridX);
            
        case ForegroundObjectType_SpikedBallRollingLeft:
            return new SpikedBallRollingLeft(gridX, gridY);
        case ForegroundObjectType_SpikedBallRollingRight:
            return new SpikedBallRollingRight(gridX, gridY);
            
        case ForegroundObjectType_SpikedBallChain:
            return new SpikedBallChain(gridX, gridY);
        case ForegroundObjectType_SpikedBall:
            return new SpikedBall(gridX, gridY);
    }
    
    assert(false);
}

ForegroundObject::ForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, boundsX, boundsY, boundsWidth, boundsHeight), m_type(type), m_groundSoundType(groundSoundType), m_game(nullptr), m_color(1, 1, 1, 1)
{
    // Empty
}

void ForegroundObject::updateBounds()
{
    GridLockedPhysicalEntity::updateBounds();
}

bool ForegroundObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return isEntityLanding(entity, getMainBounds(), deltaTime);
}

bool ForegroundObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return isEntityBlockedOnRight(entity, getMainBounds(), deltaTime);
}

bool ForegroundObject::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
	return isEntityBlockedOnLeft(entity, getMainBounds(), deltaTime);
}

bool ForegroundObject::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    return false;
}

bool ForegroundObject::isJonHittingHorizontally(Jon& jon, float deltaTime)
{
    return false;
}

bool ForegroundObject::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    return false;
}

bool ForegroundObject::canObjectBePlacedOn()
{
    return false;
}

ForegroundObjectType ForegroundObject::getType()
{
    return m_type;
}

GroundSoundType ForegroundObject::getGroundSoundType()
{
    return m_groundSoundType;
}

void ForegroundObject::setGame(Game* game)
{
    m_game = game;
}

#pragma mark protected

bool ForegroundObject::isEntityLanding(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
	float entityVelocityY = entity->getVelocity().getY();

	if (entityVelocityY <= 0
		&& entity->getMainBounds().getRight() > getMainBounds().getLeft())
	{
		float entityYDelta = fabsf(entityVelocityY * deltaTime);

		Rectangle tempBounds = Rectangle(getMainBounds().getLeft(), getMainBounds().getBottom(), getMainBounds().getWidth(), getMainBounds().getHeight());

		tempBounds.setHeight(tempBounds.getHeight() + entityYDelta);

		if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), tempBounds))
		{
			float jonLowerLeftY = entity->getMainBounds().getLowerLeft().getY();

			float itemTop = tempBounds.getTop();
			float padding = itemTop * .01f;
			padding += entityYDelta;
			float itemTopReq = itemTop - padding;

			if (jonLowerLeftY >= itemTopReq)
			{
				entity->getPosition().setY(getMainBounds().getTop() + entity->getMainBounds().getHeight() / 2 * .99f);
				entity->updateBounds();

				Jon *jon;
				if ((jon = dynamic_cast<Jon *>(entity)))
				{
					jon->setGroundSoundType(getGroundSoundType());
				}

				return true;
			}
		}
	}
    
    return false;
}

bool ForegroundObject::isEntityBlockedOnRight(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
    if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
    {
        float entityVelocityX = entity->getVelocity().getX();
        float entityBottom = entity->getMainBounds().getLowerLeft().getY();
        float entityRight = entity->getMainBounds().getRight();
        float entityXDelta = entityVelocityX * deltaTime;
        
        float itemTop = bounds.getTop();
        float itemTopReq = itemTop * 0.99f;
        
        float itemLeft = bounds.getLeft();
        float padding = itemLeft * .01f;
        padding += entityXDelta;
        float itemLeftReq = itemLeft + padding;
        
        if (entityRight <= itemLeftReq && entityBottom < itemTopReq)
        {
            entity->getPosition().setX(itemLeft - entity->getMainBounds().getWidth() / 2 * 1.01f);
            entity->updateBounds();
            
            return true;
        }
    }
    
    return false;
}

bool ForegroundObject::isEntityBlockedOnLeft(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
	if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
	{
		float entityVelocityX = entity->getVelocity().getX();
		float entityBottom = entity->getMainBounds().getLowerLeft().getY();
		float entityLeft = entity->getMainBounds().getLeft();
		float entityXDelta = entityVelocityX * deltaTime;

		float itemTop = bounds.getTop();
		float itemTopReq = itemTop * 0.99f;

		float itemRight = bounds.getRight();
		float padding = itemRight * .01f;
		padding -= entityXDelta;
		float itemRightReq = itemRight - padding;

		if (entityLeft >= itemRightReq && entityBottom < itemTopReq)
		{
			entity->getPosition().setX(itemRight + entity->getMainBounds().getWidth() / 2 * 1.01f);
			entity->updateBounds();

			return true;
		}
	}

	return false;
}

#pragma mark subclasses

bool PlatformObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool PlatformObject::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
	return false;
}

bool PlatformObject::canObjectBePlacedOn()
{
    return true;
}

bool DeadlyObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    float entityY = entity->getPosition().getY();
    
    if (ForegroundObject::isEntityLanding(entity, deltaTime))
    {
        Jon *jon;
        if ((jon = dynamic_cast<Jon *>(entity)))
        {
            jon->kill();
            
            if (entityY > getMainBounds().getBottom())
            {
                entity->getPosition().setY(entityY);
            }
        }
        
        return true;
    }
    
    return false;
}

bool DeadlyObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    if (ForegroundObject::isEntityBlockedOnRight(entity, deltaTime))
    {
        Jon *jon;
        if ((jon = dynamic_cast<Jon *>(entity)))
        {
            jon->kill();
        }
        
        return true;
    }
    
    return false;
}

bool DeadlyObject::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
    {
        jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
        jon.updateBounds();
        jon.kill();
    }
    
    return false;
}

bool LandingDeathObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    if (ForegroundObject::isEntityLanding(entity, deltaTime))
    {
        Jon *jon;
        if ((jon = dynamic_cast<Jon *>(entity)))
        {
            jon->kill();
        }
    }
    
    return false;
}

bool RunningIntoDeathObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    if (ForegroundObject::isEntityBlockedOnRight(entity, deltaTime))
    {
        Jon *jon;
        if ((jon = dynamic_cast<Jon *>(entity)))
        {
            jon->kill();
        }
    }
    
    return false;
}

bool DeathFromAboveObject::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
    {
        jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
        jon.updateBounds();
        jon.kill();
    }
    
    return false;
}

void ProvideBoostObject::update(float deltaTime)
{
    if (m_isBoosting)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.36f)
        {
            m_isBoosting = false;
            m_fStateTime = 0;
        }
    }
}

bool ProvideBoostObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    if (ForegroundObject::isEntityLanding(entity, deltaTime))
    {
        float itemTop = getMainBounds().getTop();
        entity->getPosition().setY(itemTop + entity->getMainBounds().getHeight() / 2 * 1.01f);
        
        Jon *jon;
        if ((jon = dynamic_cast<Jon *>(entity)))
        {
            jon->triggerBoost(m_fBoostVelocity);
        }
        
        m_isBoosting = true;
        m_fStateTime = 0;
    }
    
    return false;
}

bool JumpSpringLightFlush::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool JumpSpringLightFlush::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
	return false;
}

bool JumpSpringLightFlush::isEntityLanding(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
    float entityVelocityY = entity->getVelocity().getY();
    
    if (entityVelocityY <= 0
        && entity->getPosition().getX() > getMainBounds().getLeft()
        && (entity->getMainBounds().getBottom() + 0.01f) > getMainBounds().getBottom())
    {
        if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
        {
            float itemTop = bounds.getTop();
            
            entity->getPosition().setY(itemTop + entity->getMainBounds().getHeight() / 2 * .99f);
            entity->updateBounds();
            
            Jon *jon;
            if ((jon = dynamic_cast<Jon *>(entity)))
            {
                jon->setGroundSoundType(getGroundSoundType());
            }
            
            return true;
        }
    }
    
    return false;
}

void GiantShakingTree::update(float deltaTime)
{
    if (m_isShaking)
    {
        ForegroundObject::update(deltaTime);
        
        if (m_fStateTime > 0.70f)
        {
            m_fStateTime = 0.0f;
            m_isShaking = false;
        }
    }
}

void GiantShakingTree::triggerHit()
{
    m_isShaking = true;
}

void ExtraForegroundObject::update(float deltaTime)
{
    ForegroundObject::update(deltaTime);
    
    m_shadow->update(deltaTime);
    m_shadow->getPosition().set(getPosition());
    m_shadow->updateBounds();
}

void SpikeTower::updateBounds()
{
    ForegroundObject::updateBounds();
    
    Rectangle& bounds = getBounds().at(1);
    
    bounds.setWidth(getWidth());
    bounds.setHeight(getHeight());
    
    Vector2D &lowerLeft = bounds.getLowerLeft();
    lowerLeft.set(m_position->getX() - bounds.getWidth() / 2, m_position->getY() - bounds.getHeight() / 2);
    
    bounds.getLowerLeft().add(getWidth() * 0.0f, getHeight() * 0.79910714285714f);
    bounds.setWidth(getWidth() * 1.0f);
    bounds.setHeight(getHeight() * 0.20089285714286f);
}

bool SpikeTower::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    bool ret = false;
    
    Jon *jon;
    if ((jon = dynamic_cast<Jon *>(entity)))
    {
        jon->getMainBounds().setAngle(jon->getAbilityState() == ABILITY_GLIDE ? 90 : 0);
        
        if (ForegroundObject::isEntityLanding(jon, deltaTime)
            || ForegroundObject::isEntityLanding(jon, getBounds().at(1), deltaTime))
        {
            Jon *jon;
            if ((jon = dynamic_cast<Jon *>(entity)))
            {
                jon->kill();
            }
            
            ret = true;
        }
        
        jon->getMainBounds().setAngle(0);
    }
    
    return ret;
}

bool SpikeTower::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    bool ret = false;
    
    Jon *jon;
    if ((jon = dynamic_cast<Jon *>(entity)))
    {
        jon->getMainBounds().setAngle(jon->getAbilityState() == ABILITY_GLIDE ? 90 : 0);
    }
    
    if (ForegroundObject::isEntityBlockedOnRight(entity, deltaTime)
        || ForegroundObject::isEntityBlockedOnRight(entity, getBounds().at(1), deltaTime))
    {
        if (jon)
        {
            jon->kill();
        }
        
        ret = true;
    }
    
    if (jon)
    {
        jon->getMainBounds().setAngle(0);
    }
    
    return ret;
}

void VerticalSaw::updateBounds()
{
    DeadlyObject::updateBounds();
    
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
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SAW_GRIND);
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
        
        Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SAW_GRIND);
    }
}

void SpikedBallRollingLeft::update(float deltaTime)
{
    DeadlyObject::update(deltaTime);

	if (!m_isActivated)
	{
		Jon& jon = m_game->getJon();
		if (jon.getPosition().getX() > (getPosition().getX() - CAM_WIDTH * 2))
		{
			m_needsToPlaySound = true;

			m_isActivated = true;
		}
	}

	if (m_isActivated)
    {
        m_fAngle -= m_velocity->getX() * deltaTime * 15;
        
        if (m_velocity->getX() < (-VAMP_DEFAULT_MAX_SPEED * 2))
        {
            m_velocity->setX(-VAMP_DEFAULT_MAX_SPEED * 2);
        }

		if (!m_isStopped)
		{
			if (EntityUtils::isBlockedOnLeft(this, m_game->getEndBossForegroundObjects(), deltaTime))
			{
				stop();
			}
		}

		if (m_game->isEntityGrounded(this, deltaTime))
		{
			m_velocity->setY(0);
			m_acceleration->setY(0);

			if (!m_isStopped)
			{
				m_acceleration->setX(-VAMP_DEFAULT_ACCELERATION);

				if (m_needsToPlaySound)
				{
					Assets::getInstance()->forceAddSoundIdToPlayQueue(SOUND_SPIKED_BALL_ROLLING);

					m_needsToPlaySound = false;
				}
			}
		}
		else
		{
			if (!m_hasFallen)
			{
				if (EntityUtils::isBlockedOnLeft(this, m_game->getGrounds(), deltaTime))
				{
					m_velocity->setX(0);
					m_acceleration->setX(0);

					Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPIKED_BALL_ROLLING);

					m_hasFallen = true;
					m_needsToPlaySound = true;
				}

				m_acceleration->setY(GAME_GRAVITY);
			}
		}
    }
}

void SpikedBallRollingLeft::updateBounds()
{
    DeadlyObject::updateBounds();
    
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
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
    }
}

void SpikedBallRollingLeft::stop()
{
	m_isStopped = true;

	m_velocity->setX(0);
	m_acceleration->setX(0);

	Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPIKED_BALL_ROLLING);
}

bool SpikedBallRollingLeft::isEntityBlockedOnRight(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
	if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
	{
		float entityVelocityX = entity->getVelocity().getX();
		float entityBottom = entity->getMainBounds().getLowerLeft().getY();
		float entityRight = entity->getMainBounds().getRight();
		float entityXDelta = fabsf(entityVelocityX * deltaTime);

		float itemTop = bounds.getTop();
		float itemTopReq = itemTop * 0.99f;

		float itemLeft = bounds.getLeft();
		float padding = itemLeft * .01f;
		padding += entityXDelta;
		float itemLeftReq = itemLeft + padding;

		if (entityRight <= itemLeftReq && entityBottom < itemTopReq)
		{
			return true;
		}
	}

	return false;
}

void SpikedBallRollingRight::update(float deltaTime)
{
    DeadlyObject::update(deltaTime);
    
	if (!m_isActivated)
	{
		Jon& jon = m_game->getJon();
		if (jon.getMainBounds().getLeft() > (getMainBounds().getRight() + getMainBounds().getWidth() / 2))
		{
			m_needsToPlaySound = true;
			m_isActivated = true;
		}
	}

    if (m_isActivated)
    {
		m_fAngle -= m_velocity->getX() * deltaTime * 15;
        
        if (m_velocity->getX() > (VAMP_DEFAULT_MAX_SPEED * 2))
        {
            m_velocity->setX((VAMP_DEFAULT_MAX_SPEED * 2));
        }

		if (!m_isStopped)
		{
			if (EntityUtils::isBlockedOnRight(this, m_game->getEndBossForegroundObjects(), deltaTime))
			{
				stop();
			}
		}

		if (m_game->isEntityGrounded(this, deltaTime))
		{
			m_velocity->setY(0);
			m_acceleration->setY(0);

			if (!m_isStopped)
			{
				m_acceleration->setX(VAMP_DEFAULT_ACCELERATION * 1.4f);

				if (m_needsToPlaySound)
				{
					Assets::getInstance()->forceAddSoundIdToPlayQueue(SOUND_SPIKED_BALL_ROLLING);

					m_needsToPlaySound = false;
				}
			}
		}
		else
		{
			if (!m_hasFallen)
			{
				if (EntityUtils::isBlockedOnRight(this, m_game->getGrounds(), deltaTime))
				{
					m_velocity->setX(0);
					m_acceleration->setX(0);

					Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPIKED_BALL_ROLLING);

					m_hasFallen = true;
					m_needsToPlaySound = true;
				}

				m_acceleration->setY(GAME_GRAVITY);
			}
		}
    }
}

void SpikedBallRollingRight::updateBounds()
{
    DeadlyObject::updateBounds();
    
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
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
    }
}

void SpikedBallRollingRight::stop()
{
	m_isStopped = true;

	m_velocity->setX(0);
	m_acceleration->setX(0);

	Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPIKED_BALL_ROLLING);
}

bool SpikedBallRollingRight::isEntityBlockedOnRight(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
	if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
	{
		float entityVelocityX = entity->getVelocity().getX();
		float entityBottom = entity->getMainBounds().getLowerLeft().getY();
		float entityRight = entity->getMainBounds().getRight();
		float entityXDelta = fabsf(entityVelocityX * deltaTime);

		float itemTop = bounds.getTop();
		float itemTopReq = itemTop * 0.99f;

		float itemLeft = bounds.getLeft();
		float padding = itemLeft * .01f;
		padding += entityXDelta;
		float itemLeftReq = itemLeft + padding;

		if (entityRight <= itemLeftReq && entityBottom < itemTopReq)
		{
			return true;
		}
	}

	return false;
}

void SpikedBall::update(float deltaTime)
{
    if (m_isFalling)
    {
        DeadlyObject::update(deltaTime);
        
		EndBossSnake* snake = m_game->getEndBossSnakeP();

		if (snake)
		{
			if (!m_hasTriggeredSnakeHit
				&& OverlapTester::doRectanglesOverlap(snake->getMainBounds(), getMainBounds()))
			{
				snake->triggerHit();
				m_hasTriggeredSnakeHit = true;
			}
		}

		if (m_hasTriggeredSnakeHit)
		{
			m_acceleration->setY(0);
			m_velocity->setY(0);

			m_color.alpha -= deltaTime;
			if (m_color.alpha < 0)
			{
				m_color.alpha = 0;

				m_isRequestingDeletion = true;
			}
		}
		else
		{
			m_acceleration->setY(GAME_GRAVITY);
		}
    }
}

void SpikedBallChain::update(float deltaTime)
{
    if (m_isSnapping)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.30f)
        {
            if (!m_hasTriggeredSpikedBall)
            {
                if (m_spikedBall)
                {
                    m_spikedBall->fall();
                }
                
                m_hasTriggeredSpikedBall = true;
            }
            
            m_color.alpha -= deltaTime;
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
                
                m_isRequestingDeletion = true;
            }
        }
    }
}

bool SpikedBallChain::isJonHittingHorizontally(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getMainBounds();
    
    if (!m_isSnapping
        && OverlapTester::doRectanglesOverlap(bounds, getMainBounds()))
    {
        m_isSnapping = true;
        m_fStateTime = 0;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_CHAIN_SNAP);
        
        return true;
    }
    
    return false;
}

bool SpikedBallChain::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getMainBounds();
    
    if (!m_isSnapping
        && OverlapTester::doRectanglesOverlap(bounds, getMainBounds()))
    {
        m_isSnapping = true;
        m_fStateTime = 0;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_CHAIN_SNAP);
        
        return true;
    }
    
    return false;
}

void SpikedBallChain::trigger()
{
	m_color.alpha = 0;
}
