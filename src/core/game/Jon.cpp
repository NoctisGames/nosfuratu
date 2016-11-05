//
//  Jon.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Jon.h"

#include "EntityUtils.h"
#include "Game.h"
#include "GameConstants.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "FlagUtil.h"
#include "MathUtil.h"
#include "GameTracker.h"

#include <math.h>

Jon* Jon::create(int gridX, int gridY, int type)
{
	return new Jon(gridX, gridY);
}

Jon::Jon(int gridX, int gridY, int gridWidth, int gridHeight) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight),
m_state(JON_ALIVE),
m_physicalState(PHYSICAL_GROUNDED),
m_actionState(ACTION_NONE),
m_abilityState(ABILITY_NONE),
m_groundSoundType(GROUND_SOUND_NONE),
m_color(1, 1, 1, 1),
m_fDeltaTime(0),
m_fAbilityStateTime(0),
m_iNumTriggeredJumps(0),
m_iNumRabbitJumps(0),
m_iNumVampireJumps(0),
m_iNumBoosts(0),
m_iNumEnemiesDestroyed(0),
m_iAbilityFlag(0),
m_isLanding(false),
m_isRollLanding(false),
m_isRightFoot(false),
m_isAllowedToMove(false),
m_isConsumed(false),
m_isIdle(false),
m_isUserActionPrevented(false),
m_isBurrowEffective(false),
m_shouldUseVampireFormForConsumeAnimation(false),
m_fFlashStateTime(0),
m_isFlashing(false)
{
	resetBounds(m_fWidth * 0.4f, m_fHeight * 0.8203125f);

	m_formStateMachine = std::unique_ptr<JonFormStateMachine>(new JonFormStateMachine(this));
	m_formStateMachine->setCurrentState(Rabbit::getInstance());
	m_formStateMachine->getCurrentState()->enter(this);
}

void Jon::update(float deltaTime)
{
	m_fDeltaTime = deltaTime;
    
    EntityUtils::updateAndClean(getDustClouds(), deltaTime);
    
    for (std::vector<Jon *>::iterator i = m_afterImages.begin(); i != m_afterImages.end(); )
    {
        (*i)->m_color.red -= m_fDeltaTime * 3;
        (*i)->m_color.green -= m_fDeltaTime * 3;
        (*i)->m_color.blue += m_fDeltaTime * 3;
        (*i)->m_color.alpha -= m_fDeltaTime * 3;
        
        if ((*i)->m_color.alpha < 0.0)
        {
            delete *i;
            i = m_afterImages.erase(i);
        }
        else
        {
            i++;
        }
    }
    
    if (m_isConsumed)
    {
        return;
    }

	if (m_state == JON_DEAD)
	{
		return;
	}

	if (m_state == JON_DYING)
	{
        m_fDyingStateTime += deltaTime;
		if (m_fDyingStateTime > 1)
		{
			setState(JON_DEAD);
		}

		return;
	}

	PhysicalEntity::update(deltaTime);

	if (m_position->getY() < -m_fHeight / 2)
	{
        kill();

		return;
	}

	if (m_isFlashing)
	{
		m_fFlashStateTime += deltaTime;

		if (m_fFlashStateTime > 2)
		{
			m_isFlashing = false;

			m_color.red = 1;
			m_color.green = 1;
			m_color.blue = 1;
		}
		else if (m_fFlashStateTime > 1)
		{
			float k = 2 - m_fFlashStateTime;
			k *= 3;
			k += 1;
			m_color.red = k;
			m_color.green = k;
			m_color.blue = k;
		}
		else
		{
			float k = m_fFlashStateTime;
			k *= 3;
			k += 1;
			m_color.red = k;
			m_color.green = k;
			m_color.blue = k;
		}
	}

	if (m_actionState != ACTION_NONE)
	{
		m_fActionStateTime += deltaTime;
	}

	if (m_abilityState != ABILITY_NONE)
	{
		m_fAbilityStateTime += deltaTime;
	}

	bool wasGrounded = m_physicalState == PHYSICAL_GROUNDED;

	setState(m_game->isEntityGrounded(this, deltaTime) ? PHYSICAL_GROUNDED : PHYSICAL_IN_AIR);

	if (m_physicalState == PHYSICAL_GROUNDED)
	{
		if (!wasGrounded && m_state == JON_ALIVE)
		{
			m_isLanding = true;
			m_fStateTime = 0;
			m_velocity->sub(1.5f, 0);
			if (m_velocity->getX() < 0)
			{
				m_velocity->setX(0);
			}

			m_dustClouds.push_back(new DustCloud(getPosition().getX(), getPosition().getY() - getHeight() / 2, fabsf(m_velocity->getY() / 12.6674061f)));

			if (m_groundSoundType == GROUND_SOUND_GRASS)
			{
				Assets::getInstance()->addSoundIdToPlayQueue(SOUND_LANDING_GRASS);
			}
			else if (m_groundSoundType == GROUND_SOUND_CAVE)
			{
				Assets::getInstance()->addSoundIdToPlayQueue(SOUND_LANDING_CAVE);
			}
		}
        
        if (m_isRollLanding)
        {
            if (m_fStateTime > 0.50f)
            {
                m_isRollLanding = false;
            }
        }

		if (m_isLanding)
		{
            if (m_fStateTime > 0.20f)
            {
                m_isLanding = false;
            }
		}

		m_acceleration->set(m_isAllowedToMove && !m_isIdle ? m_fAccelerationX : 0, 0);
		m_velocity->setY(0);
        m_iNumTriggeredJumps = 0;
		m_iNumRabbitJumps = 0;
        m_iNumVampireJumps = 0;
        
		setState(ACTION_NONE);
	}
    else
    {
		m_isRollLanding = false;
		m_isLanding = false;

        if (getNumJumps() == 0)
        {
            m_iNumRabbitJumps = m_iNumRabbitJumps == 0 ? 1 : m_iNumRabbitJumps;
            m_iNumVampireJumps = m_iNumVampireJumps == 0 ? 1 : m_iNumVampireJumps;

			m_acceleration->setX(0);
        }
        
        m_acceleration->setY(GAME_GRAVITY);
    }

	if (m_game->isJonBlockedVertically(deltaTime))
	{
		m_velocity->sub(0, 2);
	}
	else if (m_game->isJonBlockedOnRight(deltaTime))
	{
		m_velocity->setX(-5.0f);
        
        if (isFalling() || getNumJumps() > 0)
        {
            m_velocity->setX(0);
        }

		m_fStateTime = 0;
	}
	else if (m_game->isJonBlockedOnLeft(deltaTime))
	{
		m_velocity->setX(5);

		if (isFalling() || getNumJumps() > 0)
		{
			m_velocity->setX(0);
		}

		m_fStateTime = 0;
	}

	if (m_velocity->getX() > m_fMaxSpeed
        && !m_isRollLanding)
	{
		m_velocity->setX(m_fMaxSpeed);
	}

	m_formStateMachine->execute();
}

void Jon::updateBounds()
{
	Vector2D &lowerLeft = getMainBounds().getLowerLeft();
	float width = m_abilityState == ABILITY_DASH ? getMainBounds().getWidth() / 3 : getMainBounds().getWidth();
    float height = m_abilityState == ABILITY_UPWARD_THRUST ? getMainBounds().getHeight() / 2 : getMainBounds().getHeight();
	lowerLeft.set(m_position->getX() - width / 2, m_position->getY() - height / 2);
}

void Jon::onDeletion()
{
    EntityUtils::cleanUpVectorOfPointers(getDustClouds());
}

void Jon::triggerTransform()
{
	if (m_state != JON_ALIVE || m_isIdle || m_isConsumed || m_isUserActionPrevented)
	{
		return;
	}

	JonFormState* jfs = m_formStateMachine->getCurrentState();
	jfs->triggerTransform(this);
}

void Jon::triggerCancelTransform()
{
	JonFormState* jfs = m_formStateMachine->getCurrentState();
	jfs->triggerCancelTransform(this);
}

void Jon::triggerJump()
{
	if (m_state != JON_ALIVE || m_isIdle || m_isUserActionPrevented)
	{
		return;
	}

	JonFormState* jfs = m_formStateMachine->getCurrentState();
	jfs->triggerJump(this);
}

void Jon::triggerLeftAction()
{
	if (m_state != JON_ALIVE || m_isIdle || m_isConsumed || m_isUserActionPrevented)
	{
		return;
	}

	JonFormState* jfs = m_formStateMachine->getCurrentState();
	jfs->triggerLeftAction(this);
}

void Jon::triggerRightAction()
{
	if (m_state != JON_ALIVE || m_isIdle || m_isConsumed || m_isUserActionPrevented)
	{
		return;
	}

	JonFormState* jfs = m_formStateMachine->getCurrentState();
	jfs->triggerRightAction(this);
}

void Jon::triggerUpAction()
{
	if (m_state != JON_ALIVE || m_isIdle || m_isConsumed || m_isUserActionPrevented)
	{
		return;
	}

	JonFormState* jfs = m_formStateMachine->getCurrentState();
	jfs->triggerUpAction(this);
}

void Jon::triggerDownAction()
{
	if (m_state != JON_ALIVE || m_isIdle || m_isConsumed || m_isUserActionPrevented)
	{
		return;
	}

	JonFormState* jfs = m_formStateMachine->getCurrentState();
	jfs->triggerDownAction(this);
}

void Jon::triggerBoost(float boostVelocity)
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    JonFormState* jfs = m_formStateMachine->getCurrentState();
    jfs->triggerBoost(this, boostVelocity);
}

void Jon::triggerBoostOffEnemy(float boostVelocity)
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    JonFormState* jfs = m_formStateMachine->getCurrentState();
    jfs->triggerBoostOffEnemy(this, boostVelocity);
}

void Jon::triggerBounceDownardsOffEnemy(float bounceBackVelocity)
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    JonFormState* jfs = m_formStateMachine->getCurrentState();
    jfs->triggerBounceDownardsOffEnemy(this, bounceBackVelocity);
}

void Jon::triggerBounceBackOffEnemy(float bounceBackVelocity)
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    JonFormState* jfs = m_formStateMachine->getCurrentState();
    jfs->triggerBounceBackOffEnemy(this, bounceBackVelocity);
}

int Jon::getNumJumps()
{
    if (m_state != JON_ALIVE)
    {
        return 0;
    }
    
    JonFormState* jfs = m_formStateMachine->getCurrentState();
    return jfs->getNumJumps(this);
}

std::vector<DustCloud *>& Jon::getDustClouds()
{
	return m_dustClouds;
}

std::vector<Jon *>& Jon::getAfterImages()
{
    return m_afterImages;
}

JonState Jon::getState()
{
	return m_state;
}

JonPhysicalState Jon::getPhysicalState()
{
	return m_physicalState;
}

void Jon::setPhysicalState(JonPhysicalState state)
{
    m_physicalState = state;
}

JonActionState Jon::getActionState()
{
	return m_actionState;
}

JonAbilityState Jon::getAbilityState()
{
	return m_abilityState;
}

Color Jon::getColor()
{
	return m_color;
}

float Jon::getActionStateTime()
{
	return m_fActionStateTime;
}

float Jon::getAbilityStateTime()
{
	return m_fAbilityStateTime;
}

float Jon::getTransformStateTime()
{
	return m_fTransformStateTime;
}

float Jon::getDyingStateTime()
{
	return m_fDyingStateTime;
}

int Jon::getNumTriggeredJumps()
{
    return m_iNumTriggeredJumps;
}

int Jon::getNumBoosts()
{
    return m_iNumBoosts;
}

void Jon::setNumBoosts(int numBoosts)
{
    m_iNumBoosts = numBoosts;
}

int Jon::getNumEnemiesDestroyed()
{
    return m_iNumEnemiesDestroyed;
}

void Jon::onEnemyDestroyed()
{
    m_iNumEnemiesDestroyed++;
    
    GameTracker::getInstance()->onScored(SCORE_ENEMY);
}

bool Jon::isMoving()
{
	return m_velocity->getX() > 0;
}

bool Jon::isPushedBack()
{
	return m_velocity->getX() < 0;
}

bool Jon::isLanding()
{
	return m_isLanding;
}

bool Jon::isRollLanding()
{
    return m_isRollLanding;
}

bool Jon::isFalling()
{
	return m_velocity->getY() < 0;
}

bool Jon::isAlive()
{
    return m_state == JON_ALIVE;
}

bool Jon::isDying()
{
    return m_state == JON_DYING;
}

bool Jon::isDead()
{
	return m_state == JON_DEAD;
}

bool Jon::isConsumed()
{
    return m_isConsumed;
}

int Jon::getType()
{
	return -1;
}

bool Jon::isRightFoot()
{
	return m_isRightFoot;
}

void Jon::setRightFoot(bool isRightFoot)
{
	m_isRightFoot = isRightFoot;
}

GroundSoundType Jon::getGroundSoundType()
{
	return m_groundSoundType;
}

void Jon::setGroundSoundType(GroundSoundType groundSoundType)
{
	m_groundSoundType = groundSoundType;
}

bool Jon::isVampire()
{
	return m_formStateMachine->isInState(Jon::Vampire::getInstance());
}

bool Jon::isTransformingIntoVampire()
{
	return m_formStateMachine->isInState(Jon::RabbitToVampire::getInstance());
}

bool Jon::isRevertingToRabbit()
{
	return m_formStateMachine->isInState(Jon::VampireToRabbit::getInstance());
}

void Jon::setAllowedToMove(bool isAllowedToMove)
{
	m_isAllowedToMove = isAllowedToMove;
}

bool Jon::isAllowedToMove()
{
	return m_isAllowedToMove;
}

void Jon::setIdle(bool isIdle)
{
    m_fStateTime = 0;
    m_isIdle = isIdle;
}

bool Jon::isIdle()
{
    return m_isIdle;
}

void Jon::setUserActionPrevented(bool isUserActionPrevented)
{
    m_isUserActionPrevented = isUserActionPrevented;
}

bool Jon::isUserActionPrevented()
{
    return m_isUserActionPrevented;
}

void Jon::setGame(Game* game)
{
	m_game = game;
}

void Jon::setAbilityFlag(int abilityFlag)
{
    m_iAbilityFlag = abilityFlag;
}

void Jon::enableAbility(int abilityFlag)
{
    m_iAbilityFlag = FlagUtil::setFlag(m_iAbilityFlag, abilityFlag);
}

bool Jon::isAbilityEnabled(int abilityFlag)
{
    return FlagUtil::isFlagSet(m_iAbilityFlag, abilityFlag);
}

int Jon::getAbilityFlag()
{
    return m_iAbilityFlag;
}

bool Jon::isBurrowEffective()
{
    return m_isBurrowEffective;
}

void Jon::beginWarmingUp()
{
    m_fStateTime = 0;
}

void Jon::consume(bool vampireDies)
{
    // Used when Jon needs to be "transfered" i.e. grabbed by the owl or eaten by the toad
    m_isConsumed = true;
    m_fWidth = m_iGridWidth * GRID_CELL_SIZE;
	m_fHeight = m_iGridHeight * GRID_CELL_SIZE;
    m_isFatallyConsumed = vampireDies;
    m_iNumRabbitJumps = 0;
    m_iNumVampireJumps = 0;
	setState(ABILITY_NONE);
	Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
}

void Jon::kill()
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_DEATH);
    setState(m_isConsumed ? JON_DEAD : JON_DYING);
    m_fDyingStateTime = 0;
    m_velocity->set(0, 0);
    m_acceleration->set(0, 0);
    m_fWidth = m_iGridWidth * GRID_CELL_SIZE;
    m_fHeight = m_iGridHeight * GRID_CELL_SIZE;
    
    JonFormState* jfs = m_formStateMachine->getCurrentState();
    jfs->onDeath(this);
    
    setState(ABILITY_NONE);
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
}

bool Jon::shouldUseVampireFormForConsumeAnimation()
{
    return m_shouldUseVampireFormForConsumeAnimation;
}

void Jon::becomeVampire()
{
	m_formStateMachine->changeState(Jon::Vampire::getInstance());
	m_shouldUseVampireFormForConsumeAnimation = true;
}

void Jon::flash()
{
	m_fFlashStateTime = 0;
	m_isFlashing = true;
}

#pragma mark private

void Jon::setState(JonState state)
{
	m_state = state;
	m_fStateTime = 0;
}

void Jon::setState(JonPhysicalState state)
{
	m_physicalState = state;
}

void Jon::setState(JonActionState state)
{
	m_actionState = state;
	m_fActionStateTime = 0;
}

void Jon::setState(JonAbilityState state)
{
	m_abilityState = state;
	m_fAbilityStateTime = 0;
}

RTTI_IMPL(Jon, GridLockedPhysicalEntity);

/// Rabbit Form ///

Jon::Rabbit * Jon::Rabbit::getInstance()
{
	static Rabbit *instance = new Rabbit();

	return instance;
}

void Jon::Rabbit::enter(Jon* jon)
{
    jon->m_fActionStateTime = 0;
	jon->m_fDyingStateTime = 0;
    jon->m_isBurrowEffective = false;
    jon->m_shouldUseVampireFormForConsumeAnimation = false;
	jon->m_fDefaultMaxSpeed = RABBIT_DEFAULT_MAX_SPEED;
    jon->m_fMaxSpeed = RABBIT_DEFAULT_MAX_SPEED;
	jon->m_fAccelerationX = RABBIT_DEFAULT_ACCELERATION;
    
    jon->m_acceleration->setY(GAME_GRAVITY);

	m_isSpinningBackFistDelivered = false;
}

void Jon::Rabbit::execute(Jon* jon)
{
	switch (jon->m_abilityState)
	{
        case ABILITY_SPINNING_BACK_FIST:
        {
            if (jon->m_fAbilityStateTime > 0.48f)
            {
                jon->setState(ABILITY_NONE);
            }
            else if (!m_isSpinningBackFistDelivered && jon->m_fAbilityStateTime > 0.30f)
            {
                m_isSpinningBackFistDelivered = jon->m_game->isSpinningBackFistDelivered(jon->m_fDeltaTime);
            }
        }
            break;
        case ABILITY_BURROW:
        {
            jon->m_velocity->setX(0);
            jon->m_acceleration->setX(0);
            
            bool wasBurrowEffective = jon->m_isBurrowEffective;
            jon->m_isBurrowEffective = jon->m_game->isBurrowEffective(jon->m_fDeltaTime);
            
            if (jon->m_isBurrowEffective && !wasBurrowEffective)
            {
                Assets::getInstance()->addSoundIdToPlayQueue(SOUND_JON_BURROW_ROCKSFALL);
            }
            
            if (jon->isFalling())
            {
                jon->m_velocity->setY(fminf(-4, jon->m_velocity->getY()));
            }
            
            if (jon->m_fAbilityStateTime > 0.54f)
            {
                jon->setState(ABILITY_NONE);
            }
        }
            break;
        case ABILITY_STOMP:
        {
            jon->m_velocity->setX(0);
            jon->m_acceleration->setX(0);
            jon->m_acceleration->setY(0);
            
            if (jon->m_fAbilityStateTime > 0.35f)
            {
                jon->m_acceleration->setY(GAME_GRAVITY * 4);
                
                bool wasBurrowEffective = jon->m_isBurrowEffective;
                jon->m_isBurrowEffective = jon->m_game->isBurrowEffective(jon->m_fDeltaTime);
                
                if (jon->m_isBurrowEffective && !wasBurrowEffective)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_JON_BURROW_ROCKSFALL);
                    jon->setState(ABILITY_BURROW);
                    jon->m_fAbilityStateTime = 0.18f;
                    jon->m_velocity->setY(0);
                    jon->m_acceleration->setY(GAME_GRAVITY);
                }
                else
                {
                    if (jon->m_physicalState == PHYSICAL_GROUNDED)
                    {
                        jon->m_velocity->setX(VAMP_DEFAULT_MAX_SPEED);
                        
                        jon->m_isRollLanding = true;
                        
                        jon->setState(ABILITY_NONE);
                    }
                }
            }
        }
            break;
        default:
            break;
    }

	if (jon->isFalling())
	{
		jon->setState(ACTION_NONE);
	}
}

void Jon::Rabbit::exit(Jon* jon)
{
	// TODO
}

void Jon::Rabbit::triggerTransform(Jon* jon)
{
    if (!jon->isAbilityEnabled(FLAG_ABILITY_TRANSFORM))
    {
        return;
    }
    
	jon->m_formStateMachine->changeState(Jon::RabbitToVampire::getInstance());
}

void Jon::Rabbit::triggerJump(Jon* jon)
{
    if (jon->m_isConsumed)
    {
        return;
    }
    
    if (jon->m_iNumRabbitJumps == 1
        && !jon->isAbilityEnabled(FLAG_ABILITY_DOUBLE_JUMP))
    {
        return;
    }
    
	if (jon->m_iNumRabbitJumps < 2)
	{
		jon->m_fStateTime = 0;
        jon->m_isRollLanding = false;
        jon->m_isLanding = false;

		if (jon->m_velocity->getX() < 0)
		{
			jon->m_velocity->setX(0);
		}

		jon->m_acceleration->setX(0);
		jon->m_acceleration->setY(GAME_GRAVITY);
        
		jon->m_velocity->setY(13 - jon->m_iNumRabbitJumps * 3);

		jon->setState(jon->m_iNumRabbitJumps == 0 ? ACTION_JUMPING : ACTION_DOUBLE_JUMPING);
        jon->setState(ABILITY_NONE);

        jon->m_iNumTriggeredJumps++;
		jon->m_iNumRabbitJumps++;

		Assets::getInstance()->addSoundIdToPlayQueue(jon->m_iNumRabbitJumps == 1 ? SOUND_JON_RABBIT_JUMP : SOUND_JON_RABBIT_DOUBLE_JUMP);
	}
}

void Jon::Rabbit::triggerLeftAction(Jon* jon)
{
    if (!jon->isAbilityEnabled(FLAG_ABILITY_RABBIT_LEFT))
    {
        return;
    }
    
    // TODO
}

void Jon::Rabbit::triggerRightAction(Jon* jon)
{
    if (!jon->isAbilityEnabled(FLAG_ABILITY_RABBIT_RIGHT))
    {
        return;
    }
    
	if (jon->m_abilityState == ABILITY_SPINNING_BACK_FIST)
	{
		return;
	}

	jon->setState(ABILITY_SPINNING_BACK_FIST);

	m_isSpinningBackFistDelivered = false;
}

void Jon::Rabbit::triggerUpAction(Jon* jon)
{
    if (!jon->isAbilityEnabled(FLAG_ABILITY_RABBIT_UP))
    {
        return;
    }
    
    // TODO
}

void Jon::Rabbit::triggerDownAction(Jon* jon)
{
    if (!jon->isAbilityEnabled(FLAG_ABILITY_RABBIT_DOWN))
    {
        return;
    }
    
    if (jon->m_physicalState == PHYSICAL_IN_AIR)
    {
        if (jon->m_abilityState == ABILITY_STOMP)
        {
            return;
        }
        
        jon->setState(ABILITY_STOMP);
        jon->m_acceleration->setY(0);
        jon->m_velocity->setY(0);
        jon->m_isRollLanding = false;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_RABBIT_STOMP);
    }
    else
	{
        if (jon->m_abilityState == ABILITY_BURROW)
        {
            return;
        }
        
        jon->setState(ABILITY_BURROW);
        
        jon->m_isBurrowEffective = false;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_RABBIT_DRILL);
	}
    
    jon->m_velocity->setX(0);
    jon->m_acceleration->setX(0);
}

void Jon::Rabbit::triggerBoost(Jon* jon, float boostVelocity)
{
    jon->m_velocity->setX(jon->m_fDefaultMaxSpeed / 2);
    jon->m_acceleration->setX(jon->m_fAccelerationX);
    jon->m_acceleration->setY(GAME_GRAVITY);
    jon->m_velocity->setY(boostVelocity);
    
    jon->setState(ACTION_DOUBLE_JUMPING);
    jon->setState(ABILITY_NONE);
    
    jon->m_iNumRabbitJumps = 1;
    jon->m_iNumBoosts++;
    
    Assets::getInstance()->addSoundIdToPlayQueue(boostVelocity > 25 ? SOUND_JUMP_SPRING_HEAVY : SOUND_JUMP_SPRING);
}

void Jon::Rabbit::triggerBoostOffEnemy(Jon* jon, float boostVelocity)
{
    jon->m_velocity->setX(jon->m_fDefaultMaxSpeed / 2);
    jon->m_acceleration->setX(jon->m_fAccelerationX);
    jon->m_acceleration->setY(GAME_GRAVITY);
    jon->m_velocity->setY(boostVelocity);
    
    jon->setState(ACTION_DOUBLE_JUMPING);
    jon->setState(ABILITY_NONE);
    
    jon->m_iNumRabbitJumps = 1;
}

void Jon::Rabbit::triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity)
{
    jon->m_acceleration->setY(GAME_GRAVITY);
    jon->m_velocity->setY(bounceBackVelocity);
    
    jon->setState(ACTION_DOUBLE_JUMPING);
    jon->setState(ABILITY_NONE);
    
    jon->m_iNumRabbitJumps = 1;
}

void Jon::Rabbit::triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity)
{
    jon->m_velocity->setX(bounceBackVelocity);
    jon->m_acceleration->setX(0);
    
    jon->setState(ABILITY_NONE);
    
    jon->m_fStateTime = 0;
}

int Jon::Rabbit::getNumJumps(Jon* jon)
{
    return jon->m_iNumRabbitJumps;
}

Jon::Rabbit::Rabbit() : JonFormState(), m_isSpinningBackFistDelivered(false)
{
	// Empty
}

/// Vampire Form ///

Jon::Vampire * Jon::Vampire::getInstance()
{
	static Vampire *instance = new Vampire();

	return instance;
}

void Jon::Vampire::enter(Jon* jon)
{
    jon->m_fActionStateTime = 0;
	jon->m_fDyingStateTime = 0;
    jon->m_shouldUseVampireFormForConsumeAnimation = true;
	jon->m_fDefaultMaxSpeed = VAMP_DEFAULT_MAX_SPEED;
	jon->m_fMaxSpeed = VAMP_DEFAULT_MAX_SPEED;
    jon->m_fAccelerationX = VAMP_DEFAULT_ACCELERATION;
    
    jon->m_acceleration->setY(GAME_GRAVITY);
    
    m_isFallingAfterGlide = false;
}

void Jon::Vampire::execute(Jon* jon)
{
    m_fTimeSinceLastVelocityCheck += jon->m_fDeltaTime;
    bool createAfterImage = false;
    float vDist = 0;
    
    jon->m_fMaxSpeed = VAMP_DEFAULT_MAX_SPEED;
    jon->m_fWidth = jon->m_iGridWidth * GRID_CELL_SIZE;
    jon->m_fHeight = jon->m_iGridHeight * GRID_CELL_SIZE;
    
	switch (jon->m_abilityState)
	{
        case ABILITY_GLIDE:
        {
            if (jon->m_physicalState == PHYSICAL_GROUNDED)
            {
                jon->setState(ABILITY_NONE);
                
                Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
            }
            else
            {
                jon->m_acceleration->setY(GAME_GRAVITY / 36);
                jon->m_fMaxSpeed = VAMP_DEFAULT_MAX_SPEED - 2;
            }
        }
            break;
        case ABILITY_UPWARD_THRUST:
        {
            if (jon->m_fAbilityStateTime > 0.1818181818182f)
            {
                jon->m_velocity->setX(0);
            }
            
            if (jon->m_game->isUpwardThrustEffective(jon->m_fDeltaTime))
            {
                // TODO, maybe do something here?
            }
            
            if (jon->isFalling() || jon->m_iNumVampireJumps == 2)
            {
                jon->setState(ABILITY_NONE);
            }
            else
            {
                jon->m_fHeight = jon->m_iGridHeight * GRID_CELL_SIZE * 2;
            }
        }
            break;
        case ABILITY_DASH:
        {
            if (jon->m_fAbilityStateTime > 0.5f)
            {
				jon->m_acceleration->setX(0);
                jon->m_velocity->setX(0);
                jon->m_acceleration->setY(GAME_GRAVITY);
            }
            else
            {
                jon->m_acceleration->setY(0);
                
                if (jon->m_game->isDashEffective(jon->m_fDeltaTime))
                {
                    // TODO, maybe do something here?
                }
            }
            
            if (m_fTimeSinceLastVelocityCheck > 0.05f)
            {
                createAfterImage = true;
                vDist = 1.5f -jon->m_fAbilityStateTime;
            }
            
            if (jon->m_fAbilityStateTime > 1.2f)
            {
                jon->setState(ABILITY_NONE);
            }
            else
            {
                jon->m_fMaxSpeed = VAMP_DEFAULT_MAX_SPEED * 3;
                
                if (!(jon->m_fAbilityStateTime > 0.5f && jon->isFalling()))
                {
                    jon->m_fWidth = jon->m_iGridWidth * GRID_CELL_SIZE * 3;
                }
            }
        }
            break;
        default:
            break;
	}
    
    jon->updateBounds();

	if (jon->m_physicalState == PHYSICAL_GROUNDED)
	{
		m_isFallingAfterGlide = false;
	}

	if (jon->isFalling())
	{
        if (!m_isFallingAfterGlide
            && jon->m_iNumVampireJumps > 1
            && jon->m_abilityState != ABILITY_GLIDE
            && jon->m_abilityState != ABILITY_DASH)
		{
			jon->setState(ABILITY_GLIDE);
            
            jon->m_acceleration->setX(VAMP_DEFAULT_ACCELERATION);
            
            jon->m_velocity->setY(0);
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_JON_VAMPIRE_GLIDE);
		}

		jon->setState(ACTION_NONE);
	}
    
    if (!createAfterImage)
    {
        vDist = jon->m_velocity->dist(*m_lastKnownVelocity);
        createAfterImage = m_fTimeSinceLastVelocityCheck > 0.1 && vDist > 0;
    }
    
    if (createAfterImage)
    {
        m_lastKnownVelocity->set(*jon->m_velocity);
        m_fTimeSinceLastVelocityCheck = 0;
        
        Jon* afterImage = new Jon(jon->getGridX(), jon->getGridY());
        afterImage->m_formStateMachine = std::unique_ptr<JonFormStateMachine>(new JonFormStateMachine(afterImage));
        afterImage->m_formStateMachine->setCurrentState(jon->m_formStateMachine->getCurrentState());
        afterImage->m_velocity->set(*jon->m_velocity);
        afterImage->m_position->set(*jon->m_position);
        afterImage->m_state = jon->m_state;
        afterImage->m_physicalState = jon->m_physicalState;
        afterImage->m_actionState = jon->m_actionState;
        afterImage->m_abilityState = jon->m_abilityState;
        afterImage->m_groundSoundType = jon->m_groundSoundType;
        
        afterImage->m_color = jon->m_color;
        afterImage->m_fDeltaTime = jon->m_fDeltaTime;
        afterImage->m_fActionStateTime = jon->m_fActionStateTime;
        afterImage->m_fAbilityStateTime = jon->m_fAbilityStateTime;
        afterImage->m_fTransformStateTime = jon->m_fTransformStateTime;
        
        afterImage->m_fDyingStateTime = jon->m_fDyingStateTime;
        afterImage->m_fDefaultMaxSpeed = jon->m_fDefaultMaxSpeed;
        afterImage->m_fMaxSpeed = jon->m_fMaxSpeed;
        afterImage->m_fAccelerationX = jon->m_fAccelerationX;
        
        afterImage->m_iNumVampireJumps = jon->m_iNumVampireJumps;
        afterImage->m_isLanding = jon->m_isLanding;
        afterImage->m_isRightFoot = jon->m_isRightFoot;
        afterImage->m_isAllowedToMove = jon->m_isAllowedToMove;
        afterImage->m_fHeight = jon->m_fHeight;
        afterImage->m_fWidth = jon->m_fWidth;
        afterImage->m_fStateTime = jon->m_fStateTime;
        
        afterImage->m_color.red *= 0.9f;
        afterImage->m_color.green *= 0.9f;
        afterImage->m_color.blue *= 1.1f;
        afterImage->m_color.alpha *= (vDist + 0.25f);
        afterImage->m_color.alpha = clamp(afterImage->m_color.alpha, 1, 0);
        
        jon->m_afterImages.push_back(afterImage);
    }
}

void Jon::Vampire::exit(Jon* jon)
{
    jon->m_fWidth = jon->m_iGridWidth * GRID_CELL_SIZE;
	jon->m_fHeight = jon->m_iGridHeight * GRID_CELL_SIZE;

    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
}

void Jon::Vampire::triggerTransform(Jon* jon)
{
    jon->m_formStateMachine->changeState(Jon::VampireToRabbit::getInstance());
}

void Jon::Vampire::triggerJump(Jon* jon)
{
    if (jon->m_isConsumed && jon->m_isFatallyConsumed)
    {
        return;
    }

	if (jon->m_isConsumed)
	{
		jon->m_iNumVampireJumps = 0;
		jon->m_acceleration->setY(GAME_GRAVITY);
		m_isFallingAfterGlide = false;
	}
    
    if (jon->m_abilityState == ABILITY_DASH)
    {
        return;
    }
    
	if (jon->m_abilityState == ABILITY_GLIDE)
	{
		jon->setState(ABILITY_NONE);
		jon->m_acceleration->setY(GAME_GRAVITY);
		jon->m_iNumVampireJumps = 1;
		m_isFallingAfterGlide = true;
        
        Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
	}
	else if (jon->m_iNumVampireJumps < 2)
	{
		if (!m_isFallingAfterGlide)
		{
			jon->m_fStateTime = 0;
            jon->m_isLanding = false;

            jon->m_acceleration->setX(0);
			jon->m_acceleration->setY(GAME_GRAVITY);
			jon->m_velocity->setY(7 - jon->m_iNumVampireJumps);

			jon->setState(jon->m_iNumVampireJumps == 0 ? ACTION_JUMPING : ACTION_DOUBLE_JUMPING);

			if (jon->m_iNumVampireJumps == 0)
			{
				jon->setState(ABILITY_UPWARD_THRUST);
                
                jon->m_isConsumed = false;
			}
            
            Assets::getInstance()->addSoundIdToPlayQueue(jon->m_iNumVampireJumps == 1 ? SOUND_JON_VAMPIRE_JUMP : SOUND_JON_VAMPIRE_DOUBLE_JUMP);
		}

        jon->m_iNumTriggeredJumps++;
		jon->m_iNumVampireJumps++;
	}
}

void Jon::Vampire::triggerLeftAction(Jon* jon)
{
    if (!jon->isAbilityEnabled(FLAG_ABILITY_VAMPIRE_LEFT))
    {
        return;
    }
    
    // TODO
}

void Jon::Vampire::triggerRightAction(Jon* jon)
{
    if (!jon->isAbilityEnabled(FLAG_ABILITY_VAMPIRE_RIGHT))
    {
        return;
    }
    
    if (jon->m_abilityState == ABILITY_DASH
        || jon->m_abilityState == ABILITY_GLIDE)
    {
        return;
    }
    
    jon->m_fMaxSpeed = VAMP_DEFAULT_MAX_SPEED * 3;
    jon->m_velocity->setX(jon->m_fMaxSpeed);
    jon->m_velocity->setY(0);
    
    jon->setState(ABILITY_DASH);
    
    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_VAMPIRE_DASH);
}

void Jon::Vampire::triggerUpAction(Jon* jon)
{
    if (!jon->isAbilityEnabled(FLAG_ABILITY_VAMPIRE_UP))
    {
        return;
    }
    
    // TODO
}

void Jon::Vampire::triggerDownAction(Jon* jon)
{
    if (!jon->isAbilityEnabled(FLAG_ABILITY_VAMPIRE_DOWN))
    {
        return;
    }
    
    // TODO
}

void Jon::Vampire::triggerBoost(Jon* jon, float boostVelocity)
{
    jon->m_acceleration->setX(jon->m_fAccelerationX);
	jon->m_acceleration->setY(GAME_GRAVITY);
    
    jon->m_velocity->setX(jon->m_fDefaultMaxSpeed / 2);
    jon->m_velocity->setY(boostVelocity);
    
    m_isFallingAfterGlide = false;
    
    jon->setState(ACTION_DOUBLE_JUMPING);
	jon->setState(ABILITY_NONE);
    
    jon->m_iNumVampireJumps = 1;
    jon->m_iNumBoosts++;
    
    Assets::getInstance()->addSoundIdToPlayQueue(boostVelocity > 25 ? SOUND_JUMP_SPRING_HEAVY : SOUND_JUMP_SPRING);

	Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
}

void Jon::Vampire::triggerBoostOffEnemy(Jon* jon, float boostVelocity)
{
    jon->m_acceleration->setX(jon->m_fAccelerationX);
    jon->m_acceleration->setY(GAME_GRAVITY);
    
    jon->m_velocity->setX(jon->m_fDefaultMaxSpeed / 2);
    jon->m_velocity->setY(boostVelocity);
    
    m_isFallingAfterGlide = false;
    
    jon->setState(ACTION_DOUBLE_JUMPING);
    jon->setState(ABILITY_NONE);
    
    jon->m_iNumVampireJumps = 1;

	Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
}

void Jon::Vampire::triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity)
{
    jon->m_acceleration->setY(GAME_GRAVITY);
    jon->m_velocity->setY(bounceBackVelocity);
    
	m_isFallingAfterGlide = false;

	jon->setState(ACTION_DOUBLE_JUMPING);
	jon->setState(ABILITY_NONE);

	jon->m_iNumVampireJumps = 1;

	Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
}

void Jon::Vampire::triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity)
{
    jon->m_acceleration->setY(GAME_GRAVITY);
    jon->m_velocity->setX(bounceBackVelocity);
    jon->m_acceleration->setX(0);
    
    jon->setState(ABILITY_NONE);
    
    jon->m_fStateTime = 0;
    
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
}

int Jon::Vampire::getNumJumps(Jon* jon)
{
    return jon->m_iNumVampireJumps;
}

Jon::Vampire::Vampire() : JonFormState(), m_fTimeSinceLastVelocityCheck(0), m_isFallingAfterGlide(false)
{
    m_lastKnownVelocity = std::unique_ptr<Vector2D>(new Vector2D());
}

/// Transform to Vampire ///

Jon::RabbitToVampire * Jon::RabbitToVampire::getInstance()
{
	static RabbitToVampire *instance = new RabbitToVampire();

	return instance;
}

void Jon::RabbitToVampire::enter(Jon* jon)
{
	jon->m_fTransformStateTime = 0;
	jon->m_fDyingStateTime = 0;

	m_hasCompletedSlowMotion = false;

	Assets::getInstance()->addSoundIdToPlayQueue(SOUND_TRIGGER_TRANSFORM);
}

void Jon::RabbitToVampire::execute(Jon* jon)
{
	jon->m_fTransformStateTime += jon->m_fDeltaTime;

    if (jon->m_fTransformStateTime > 0.5025f)
    {
        jon->m_formStateMachine->changeState(Jon::Vampire::getInstance());
    }
    
    if (!m_hasCompletedSlowMotion && jon->m_fTransformStateTime > 0.0625f)
    {
		m_hasCompletedSlowMotion = true;
        jon->m_shouldUseVampireFormForConsumeAnimation = true;
        jon->setState(ABILITY_NONE);
        
		Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COMPLETE_TRANSFORM);
	}
}

void Jon::RabbitToVampire::exit(Jon* jon)
{
	// Empty
}

void Jon::RabbitToVampire::triggerCancelTransform(Jon* jon)
{
	if (!m_hasCompletedSlowMotion)
	{
		jon->m_formStateMachine->revertToPreviousState();

		Assets::getInstance()->addSoundIdToPlayQueue(SOUND_CANCEL_TRANSFORM);
	}
}

void Jon::RabbitToVampire::triggerJump(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerJump(jon);
}

void Jon::RabbitToVampire::triggerLeftAction(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerLeftAction(jon);
}

void Jon::RabbitToVampire::triggerRightAction(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerRightAction(jon);
}

void Jon::RabbitToVampire::triggerUpAction(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerUpAction(jon);
}

void Jon::RabbitToVampire::triggerDownAction(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerDownAction(jon);
}

void Jon::RabbitToVampire::triggerBoost(Jon* jon, float boostVelocity)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerBoost(jon, boostVelocity);
}

void Jon::RabbitToVampire::triggerBoostOffEnemy(Jon* jon, float boostVelocity)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerBoostOffEnemy(jon, boostVelocity);
}

void Jon::RabbitToVampire::triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerBounceDownardsOffEnemy(jon, bounceBackVelocity);
}

void Jon::RabbitToVampire::triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerBounceBackOffEnemy(jon, bounceBackVelocity);
}

void Jon::RabbitToVampire::onDeath(Jon* jon)
{
    handleTransformation(jon);
}

void Jon::RabbitToVampire::handleTransformation(Jon* jon)
{
    if (m_hasCompletedSlowMotion)
    {
        jon->m_formStateMachine->changeState(Jon::Vampire::getInstance());
    }
    else
    {
        jon->m_formStateMachine->revertToPreviousState();
    }
}

Jon::RabbitToVampire::RabbitToVampire() : JonFormState(), m_hasCompletedSlowMotion(false)
{
	// Empty
}

/// Transform to Rabbit ///

Jon::VampireToRabbit * Jon::VampireToRabbit::getInstance()
{
	static VampireToRabbit *instance = new VampireToRabbit();

	return instance;
}

void Jon::VampireToRabbit::enter(Jon* jon)
{
	jon->m_fTransformStateTime = 0;
	jon->m_fDyingStateTime = 0;

	m_hasCompletedSlowMotion = false;

	Assets::getInstance()->addSoundIdToPlayQueue(SOUND_TRIGGER_TRANSFORM);
}

void Jon::VampireToRabbit::execute(Jon* jon)
{
	jon->m_fTransformStateTime += jon->m_fDeltaTime;

	if (jon->m_fTransformStateTime > 0.5025f)
	{
		jon->m_formStateMachine->changeState(Jon::Rabbit::getInstance());
	}

	if (!m_hasCompletedSlowMotion && jon->m_fTransformStateTime > 0.0625f)
    {
		m_hasCompletedSlowMotion = true;
        jon->m_shouldUseVampireFormForConsumeAnimation = false;
        jon->setState(ABILITY_NONE);
        
		Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COMPLETE_TRANSFORM);
	}
}

void Jon::VampireToRabbit::exit(Jon* jon)
{
	// Empty
}

void Jon::VampireToRabbit::triggerCancelTransform(Jon* jon)
{
	if (!m_hasCompletedSlowMotion)
	{
		jon->m_formStateMachine->revertToPreviousState();

		Assets::getInstance()->addSoundIdToPlayQueue(SOUND_CANCEL_TRANSFORM);
	}
}

void Jon::VampireToRabbit::triggerJump(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerJump(jon);
}

void Jon::VampireToRabbit::triggerLeftAction(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerLeftAction(jon);
}

void Jon::VampireToRabbit::triggerRightAction(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerRightAction(jon);
}

void Jon::VampireToRabbit::triggerUpAction(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerUpAction(jon);
}

void Jon::VampireToRabbit::triggerDownAction(Jon* jon)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerDownAction(jon);
}

void Jon::VampireToRabbit::triggerBoost(Jon* jon, float boostVelocity)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerBoost(jon, boostVelocity);
}

void Jon::VampireToRabbit::triggerBoostOffEnemy(Jon* jon, float boostVelocity)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerBoostOffEnemy(jon, boostVelocity);
}

void Jon::VampireToRabbit::triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerBounceDownardsOffEnemy(jon, bounceBackVelocity);
}

void Jon::VampireToRabbit::triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity)
{
    handleTransformation(jon);
    jon->m_formStateMachine->getCurrentState()->triggerBounceBackOffEnemy(jon, bounceBackVelocity);
}

void Jon::VampireToRabbit::onDeath(Jon* jon)
{
    handleTransformation(jon);
}

void Jon::VampireToRabbit::handleTransformation(Jon* jon)
{
    if (m_hasCompletedSlowMotion)
    {
        jon->m_formStateMachine->changeState(Jon::Rabbit::getInstance());
    }
    else
    {
        jon->m_formStateMachine->revertToPreviousState();
    }
}

Jon::VampireToRabbit::VampireToRabbit() : JonFormState(), m_hasCompletedSlowMotion(false)
{
	// Empty
}
