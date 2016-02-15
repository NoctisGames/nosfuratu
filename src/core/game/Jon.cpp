//
//  Jon.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Jon.h"
#include "EntityUtils.h"
#include "Game.h"
#include "GameConstants.h"
#include "OverlapTester.h"
#include "Assets.h"

#include <math.h>

float clamp(float x, float upper, float lower)
{
    return fminf(upper, fmaxf(x, lower));
}

Jon* Jon::create(int gridX, int gridY, int type)
{
	return new Jon(gridX, gridY);
}

Jon::Jon(int gridX, int gridY, int gridWidth, int gridHeight) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_state(JON_ALIVE), m_physicalState(PHYSICAL_GROUNDED), m_actionState(ACTION_NONE), m_abilityState(ABILITY_NONE), m_groundSoundType(GROUND_SOUND_NONE), m_color(1, 1, 1, 1), m_fDeltaTime(0), m_iNumJumps(0), m_isLanding(false), m_isRightFoot(false), m_isAllowedToMove(false)
{
	resetBounds(m_fWidth * 0.6796875f, m_fHeight * 0.8203125f);

	m_formStateMachine = std::unique_ptr<StateMachine<Jon>>(new StateMachine<Jon>(this));
	m_formStateMachine->setCurrentState(Rabbit::getInstance());
	m_formStateMachine->getCurrentState()->enter(this);
}

void Jon::update(float deltaTime)
{
	m_fDeltaTime = deltaTime;

	for (std::vector<DustCloud *>::iterator i = getDustClouds().begin(); i != getDustClouds().end(); )
	{
		(*i)->update(deltaTime);

		if ((*i)->isRequestingDeletion())
		{
            delete *i;
			i = getDustClouds().erase(i);
		}
		else
		{
			i++;
		}
	}
    
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

	if (m_actionState != ACTION_NONE)
	{
		m_fActionStateTime += deltaTime;
	}

	if (m_abilityState != ABILITY_NONE)
	{
		m_fAbilityStateTime += deltaTime;
	}

	bool wasGrounded = m_physicalState == PHYSICAL_GROUNDED;

	setState(m_game->isJonGrounded(deltaTime) ? PHYSICAL_GROUNDED : PHYSICAL_IN_AIR);

	if (m_physicalState == PHYSICAL_GROUNDED)
	{
		if (!wasGrounded && m_state == JON_ALIVE)
		{
			m_isLanding = true;
			m_fStateTime = 0;
			m_fHeight = m_iGridHeight * GRID_CELL_SIZE;

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

		if (m_isLanding && m_fStateTime > 0.20f)
		{
			m_isLanding = false;
			m_fStateTime = 0.21f;
            m_velocity->setX(fminf(m_fMaxSpeed - 2, m_velocity->getX()));
		}

		m_acceleration->set(m_isAllowedToMove ? m_fAccelerationX : 0, 0);
		m_velocity->setY(0);
		m_iNumJumps = 0;
		m_fMaxSpeed = m_fDefaultMaxSpeed;
		setState(ACTION_NONE);
	}
	else if (m_iNumJumps == 0)
	{
		m_velocity->sub(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
		m_acceleration->setY(m_fGravity);
		m_iNumJumps = 1;
	}

	if (m_game->isJonBlockedVertically(deltaTime))
	{
		m_velocity->sub(0, 2);
	}
	else if (m_game->isJonBlockedHorizontally(deltaTime))
	{
		m_velocity->setX((m_fMaxSpeed - 2.5f) * -1.0f);
        m_acceleration->setX(0);
        
        if (isFalling() || m_iNumJumps > 0)
        {
            m_velocity->setX(0);
        }

		m_fStateTime = 0;
	}

	if (m_velocity->getX() > m_fMaxSpeed)
	{
		m_velocity->setX(m_fMaxSpeed);
	}

	m_formStateMachine->execute();
}

void Jon::updateBounds()
{
	Vector2D &lowerLeft = m_bounds->getLowerLeft();
	float height = m_abilityState == ABILITY_UPWARD_THRUST ? m_bounds->getHeight() / 2 : m_bounds->getHeight();
	lowerLeft.set(m_position->getX() - m_bounds->getWidth() / 2, m_position->getY() - height / 2);
}

void Jon::onDeletion()
{
	getDustClouds().clear();
}

void Jon::triggerTransform()
{
	if (m_state != JON_ALIVE)
	{
		return;
	}

	JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
	jfs->triggerTransform(this);
}

void Jon::triggerCancelTransform()
{
	JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
	jfs->triggerCancelTransform(this);
}

void Jon::triggerJump()
{
	if (m_state != JON_ALIVE)
	{
		return;
	}

	JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
	jfs->triggerJump(this);
}

void Jon::triggerLeftAction()
{
	if (m_state != JON_ALIVE)
	{
		return;
	}

	JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
	jfs->triggerLeftAction(this);
}

void Jon::triggerRightAction()
{
	if (m_state != JON_ALIVE)
	{
		return;
	}

	JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
	jfs->triggerRightAction(this);
}

void Jon::triggerUpAction()
{
	if (m_state != JON_ALIVE)
	{
		return;
	}

	JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
	jfs->triggerUpAction(this);
}

void Jon::triggerDownAction()
{
	if (m_state != JON_ALIVE)
	{
		return;
	}

	JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
	jfs->triggerDownAction(this);
}

void Jon::triggerBoost(float boostVelocity)
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
    jfs->triggerBoost(this, boostVelocity);
}

void Jon::triggerBoostOffEnemy(float boostVelocity)
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
    jfs->triggerBoostOffEnemy(this, boostVelocity);
}

void Jon::triggerBounceDownardsOffEnemy(float bounceBackVelocity)
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    JonFormState* jfs = dynamic_cast<JonFormState*>(m_formStateMachine->getCurrentState());
    jfs->triggerBounceDownardsOffEnemy(this, bounceBackVelocity);
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
	return m_formStateMachine->isInState(*Jon::Vampire::getInstance());
}

bool Jon::isTransformingIntoVampire()
{
	return m_formStateMachine->isInState(*Jon::RabbitToVampire::getInstance());
}

bool Jon::isRevertingToRabbit()
{
	return m_formStateMachine->isInState(*Jon::VampireToRabbit::getInstance());
}

void Jon::setAllowedToMove(bool isAllowedToMove)
{
	m_isAllowedToMove = isAllowedToMove;
}

bool Jon::isAllowedToMove()
{
	return m_isAllowedToMove;
}

void Jon::setGame(Game* game)
{
	m_game = game;
}

void Jon::beginWarmingUp()
{
    m_fStateTime = 0;
}

float Jon::getGravity()
{
    return m_fGravity;
}

void Jon::kill()
{
    if (m_state != JON_ALIVE)
    {
        return;
    }
    
    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_DEATH);
    setState(JON_DYING);
    m_fDyingStateTime = 0;
    m_fHeight = m_iGridHeight * GRID_CELL_SIZE;
    
    bool isTransforming = isTransformingIntoVampire() || isRevertingToRabbit();
    if (isTransforming)
    {
        m_formStateMachine->revertToPreviousState();
    }
    
    if (m_abilityState == ABILITY_GLIDE)
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_STOP_JON_VAMPIRE_GLIDE);
        setState(ABILITY_NONE);
    }
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

/// Rabbit Form ///

Jon::Rabbit * Jon::Rabbit::getInstance()
{
	static Rabbit *instance = new Rabbit();

	return instance;
}

void Jon::Rabbit::enter(Jon* jon)
{
	jon->m_fActionStateTime = 0;
	jon->m_fAbilityStateTime = 0;
	jon->m_fDyingStateTime = 0;
	jon->m_fMaxSpeed = RABBIT_DEFAULT_MAX_SPEED;
	jon->m_fDefaultMaxSpeed = RABBIT_DEFAULT_MAX_SPEED;
	jon->m_fAccelerationX = RABBIT_DEFAULT_ACCELERATION;
	jon->m_fGravity = RABBIT_GRAVITY;
	jon->m_acceleration->setY(RABBIT_GRAVITY);
	jon->m_abilityState = ABILITY_NONE;

	m_isSpinningBackFistDelivered = false;
	m_isBurrowEffective = false;
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
        
        m_isBurrowEffective = jon->m_game->isBurrowEffective();

		if (!m_isBurrowEffective || jon->m_physicalState != PHYSICAL_GROUNDED)
		{
            jon->setState(ABILITY_NONE);
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
	jon->m_formStateMachine->changeState(Jon::RabbitToVampire::getInstance());
}

void Jon::Rabbit::triggerJump(Jon* jon)
{
	if (jon->m_iNumJumps < 2)
	{
		jon->m_fStateTime = 0;

		if (jon->m_velocity->getX() < 0)
		{
			jon->m_velocity->setX(0);
		}

		jon->m_acceleration->setX(0);
		jon->m_acceleration->setY(jon->m_fGravity);
		jon->m_velocity->setY(13 - jon->m_iNumJumps * 3);

		jon->setState(jon->m_iNumJumps == 0 ? ACTION_JUMPING : ACTION_DOUBLE_JUMPING);

		jon->m_iNumJumps++;

		Assets::getInstance()->addSoundIdToPlayQueue(jon->m_iNumJumps == 1 ? SOUND_JON_RABBIT_JUMP : SOUND_JON_RABBIT_DOUBLE_JUMP);
	}
}

void Jon::Rabbit::triggerLeftAction(Jon* jon)
{
    if (jon->m_game->getLevel() < 31)
    {
        return;
    }
    
    // TODO
}

void Jon::Rabbit::triggerRightAction(Jon* jon)
{
    if (jon->m_game->getLevel() < 52)
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
    if (jon->m_game->getLevel() < 73)
    {
        return;
    }
    
	// TODO
}

void Jon::Rabbit::triggerDownAction(Jon* jon)
{
    if (jon->m_game->getLevel() < 10)
    {
        return;
    }
    
	if (jon->m_abilityState == ABILITY_BURROW || jon->m_physicalState == PHYSICAL_IN_AIR)
	{
		return;
	}

	jon->setState(ABILITY_BURROW);

	jon->m_velocity->setX(0);
	jon->m_acceleration->setX(0);

	m_isBurrowEffective = false;
}

void Jon::Rabbit::triggerBoost(Jon* jon, float boostVelocity)
{
    jon->m_velocity->setX(3);
    jon->m_acceleration->setX(0);
    jon->m_acceleration->setY(jon->m_fGravity);
    jon->m_velocity->setY(boostVelocity);
    
    if (jon->isVampire())
    {
        jon->setState(ACTION_DOUBLE_JUMPING);
        
        jon->m_iNumJumps = 2;
        
        jon->m_fHeight = jon->m_iGridHeight * GRID_CELL_SIZE;
    }
    else
    {
        jon->setState(ACTION_JUMPING);
        
        jon->m_iNumJumps = 1;
    }
    
    Assets::getInstance()->addSoundIdToPlayQueue(boostVelocity > 25 ? SOUND_JUMP_SPRING_HEAVY : SOUND_JUMP_SPRING);
}

void Jon::Rabbit::triggerBoostOffEnemy(Jon* jon, float boostVelocity)
{
    jon->m_velocity->setX(3);
    jon->m_acceleration->setX(fminf(jon->m_fAccelerationX - 7, jon->m_acceleration->getX()));
    jon->m_acceleration->setY(jon->m_fGravity);
    jon->m_velocity->setY(boostVelocity);
    
    jon->setState(ACTION_DOUBLE_JUMPING);
    
    jon->m_iNumJumps = 2;
}

void Jon::Rabbit::triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity)
{
    jon->m_acceleration->setY(jon->m_fGravity);
    jon->m_velocity->setY(bounceBackVelocity);
    
    jon->setState(ACTION_DOUBLE_JUMPING);
    
    jon->m_iNumJumps = 2;
}

Jon::Rabbit::Rabbit() : JonFormState(), m_isSpinningBackFistDelivered(false), m_isBurrowEffective(false)
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
	jon->m_fAbilityStateTime = 0;
	jon->m_fDyingStateTime = 0;
	jon->m_fMaxSpeed = VAMP_DEFAULT_MAX_SPEED;
	jon->m_fDefaultMaxSpeed = VAMP_DEFAULT_MAX_SPEED;
	jon->m_fAccelerationX = VAMP_DEFAULT_ACCELERATION;
	jon->m_fGravity = VAMP_GRAVITY;
	jon->m_acceleration->setY(VAMP_GRAVITY);
	jon->m_abilityState = ABILITY_NONE;
}

void Jon::Vampire::execute(Jon* jon)
{
	switch (jon->m_abilityState)
	{
	case ABILITY_GLIDE:
	{
		if (jon->m_physicalState == PHYSICAL_GROUNDED)
		{
			jon->setState(ABILITY_NONE);
			jon->m_fGravity = VAMP_GRAVITY;
			jon->m_acceleration->setY(jon->m_fGravity);
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_STOP_JON_VAMPIRE_GLIDE);
		}
	}
	break;
	case ABILITY_UPWARD_THRUST:
	{
		if (jon->m_game->isUpwardThrustEffective(jon->m_fDeltaTime))
		{
			// TODO, maybe do something here?
		}

		if (jon->isFalling() || jon->m_iNumJumps == 2)
		{
			jon->setState(ABILITY_NONE);
		}
	}
	break;
	default:
		break;
	}

	if (jon->m_physicalState == PHYSICAL_GROUNDED)
	{
		m_isFallingAfterGlide = false;
	}
	else if (jon->m_physicalState == PHYSICAL_IN_AIR && jon->m_velocity->getY() > 0 && jon->m_iNumJumps == 1)
	{
		jon->m_velocity->setX(0);
	}

	if (jon->isFalling())
	{
		jon->m_fHeight = jon->m_iGridHeight * GRID_CELL_SIZE;

		if (!m_isFallingAfterGlide && jon->m_iNumJumps > 1 && jon->m_abilityState != ABILITY_GLIDE)
		{
			jon->setState(ABILITY_GLIDE);
			jon->m_fGravity = VAMP_GRAVITY / 36;
			jon->m_acceleration->setY(jon->m_fGravity);
			jon->m_velocity->setY(0);
			jon->m_fMaxSpeed = VAMP_DEFAULT_MAX_SPEED - 2;
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_JON_VAMPIRE_GLIDE);
		}

		jon->setState(ACTION_NONE);
	}

	if (jon->m_iNumJumps == 2)
	{
		jon->m_fHeight = jon->m_iGridHeight * GRID_CELL_SIZE;
	}
    
    m_fTimeSinceLastVelocityCheck += jon->m_fDeltaTime;
    float dist = jon->m_velocity->dist(*m_lastKnownVelocity);
    if (m_fTimeSinceLastVelocityCheck > 0.10 && (dist > 0.01 || dist < -0.01))
    {
        m_lastKnownVelocity->set(*jon->m_velocity);
        m_fTimeSinceLastVelocityCheck = 0;
        
        Jon* afterImage = new Jon(jon->getGridX(), jon->getGridY());
        afterImage->m_formStateMachine = std::unique_ptr<StateMachine<Jon>>(new StateMachine<Jon>(afterImage));
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
        afterImage->m_fGravity = jon->m_fGravity;
        
        afterImage->m_iNumJumps = jon->m_iNumJumps;
        afterImage->m_isLanding = jon->m_isLanding;
        afterImage->m_isRightFoot = jon->m_isRightFoot;
        afterImage->m_isAllowedToMove = jon->m_isAllowedToMove;
        afterImage->m_fHeight = jon->m_fHeight;
        afterImage->m_fWidth = jon->m_fWidth;
        afterImage->m_fStateTime = jon->m_fStateTime;
        
        afterImage->m_color.red *= 0.9f;
        afterImage->m_color.green *= 0.9f;
        afterImage->m_color.blue *= 1.1f;
        afterImage->m_color.alpha *= (dist + 0.25f);
        afterImage->m_color.alpha = clamp(afterImage->m_color.alpha, 1, 0);
        
        jon->m_afterImages.push_back(afterImage);
    }
}

void Jon::Vampire::exit(Jon* jon)
{
	jon->m_fHeight = jon->m_iGridHeight * GRID_CELL_SIZE;

	if (jon->m_abilityState == ABILITY_GLIDE)
	{
		Assets::getInstance()->addSoundIdToPlayQueue(SOUND_STOP_JON_VAMPIRE_GLIDE);
	}
}

void Jon::Vampire::triggerTransform(Jon* jon)
{
	jon->m_formStateMachine->changeState(Jon::VampireToRabbit::getInstance());
}

void Jon::Vampire::triggerJump(Jon* jon)
{
	if (jon->m_abilityState == ABILITY_GLIDE)
	{
		jon->setState(ABILITY_NONE);
		jon->m_fGravity = VAMP_GRAVITY;
		jon->m_acceleration->setY(jon->m_fGravity);
		jon->m_iNumJumps = 1;
		m_isFallingAfterGlide = true;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_STOP_JON_VAMPIRE_GLIDE);
	}
	else if (jon->m_iNumJumps < 2)
	{
		if (!m_isFallingAfterGlide)
		{
			jon->m_fStateTime = 0;

			jon->m_acceleration->setY(jon->m_fGravity);
			jon->m_velocity->setY(11 - jon->m_iNumJumps);

			jon->setState(jon->m_iNumJumps == 0 ? ACTION_JUMPING : ACTION_DOUBLE_JUMPING);

			if (jon->m_iNumJumps == 0)
			{
				jon->m_fHeight = jon->m_iGridHeight * GRID_CELL_SIZE * 2;
				jon->setState(ABILITY_UPWARD_THRUST);
			}
		}

		jon->m_iNumJumps++;

		Assets::getInstance()->addSoundIdToPlayQueue(jon->m_iNumJumps == 1 ? SOUND_JON_VAMPIRE_JUMP : SOUND_JON_VAMPIRE_DOUBLE_JUMP);
	}
}

void Jon::Vampire::triggerLeftAction(Jon* jon)
{
    if (jon->m_game->getLevel() < 22)
    {
        return;
    }
    
	// TODO
}

void Jon::Vampire::triggerRightAction(Jon* jon)
{
    if (jon->m_game->getLevel() < 22)
    {
        return;
    }
    
	// TODO
}

void Jon::Vampire::triggerUpAction(Jon* jon)
{
    if (jon->m_game->getLevel() < 22)
    {
        return;
    }
    
	// TODO
}

void Jon::Vampire::triggerDownAction(Jon* jon)
{
    if (jon->m_game->getLevel() < 22)
    {
        return;
    }
    
	// TODO
}

void Jon::Vampire::triggerBoost(Jon* jon, float boostVelocity)
{
    jon->m_velocity->setX(3);
    jon->m_acceleration->setX(0);
    jon->m_acceleration->setY(jon->m_fGravity);
    jon->m_velocity->setY(boostVelocity);
    
    m_isFallingAfterGlide = false;
    
    jon->setState(ACTION_DOUBLE_JUMPING);
    
    jon->m_iNumJumps = 2;
    
    jon->m_fHeight = jon->m_iGridHeight * GRID_CELL_SIZE;
    
    Assets::getInstance()->addSoundIdToPlayQueue(boostVelocity > 25 ? SOUND_JUMP_SPRING_HEAVY : SOUND_JUMP_SPRING);
}

void Jon::Vampire::triggerBoostOffEnemy(Jon* jon, float boostVelocity)
{
    jon->m_velocity->setX(3);
    jon->m_acceleration->setX(fminf(jon->m_fAccelerationX - 7, jon->m_acceleration->getX()));
    jon->m_acceleration->setY(jon->m_fGravity);
    jon->m_velocity->setY(boostVelocity);
    
    m_isFallingAfterGlide = false;
    
    jon->setState(ACTION_DOUBLE_JUMPING);
    
    jon->m_iNumJumps = 2;
}

void Jon::Vampire::triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity)
{
    jon->m_acceleration->setY(jon->m_fGravity);
    jon->m_velocity->setY(bounceBackVelocity);
    
    jon->setState(ACTION_DOUBLE_JUMPING);
    
    jon->m_iNumJumps = 2;
    
    m_isFallingAfterGlide = false;
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
	jon->m_abilityState = ABILITY_NONE;

	m_hasCompletedSlowMotion = false;

	Assets::getInstance()->addSoundIdToPlayQueue(SOUND_TRIGGER_TRANSFORM);
}

void Jon::RabbitToVampire::execute(Jon* jon)
{
	jon->m_fTransformStateTime += jon->m_fDeltaTime;

	if (jon->m_fTransformStateTime > 0.565f)
	{
		jon->m_formStateMachine->changeState(Jon::Vampire::getInstance());
	}

	if (!m_hasCompletedSlowMotion && jon->m_fTransformStateTime > 0.125f) {
		m_hasCompletedSlowMotion = true;
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

void Jon::RabbitToVampire::triggerBoost(Jon* jon, float boostVelocity)
{
    jon->m_formStateMachine->revertToPreviousState();
    dynamic_cast<JonFormState *>(jon->m_formStateMachine->getCurrentState())->triggerBoost(jon, boostVelocity);
}

void Jon::RabbitToVampire::triggerBoostOffEnemy(Jon* jon, float boostVelocity)
{
    jon->m_formStateMachine->revertToPreviousState();
    dynamic_cast<JonFormState *>(jon->m_formStateMachine->getCurrentState())->triggerBoostOffEnemy(jon, boostVelocity);
}

void Jon::RabbitToVampire::triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity)
{
    jon->m_formStateMachine->revertToPreviousState();
    dynamic_cast<JonFormState *>(jon->m_formStateMachine->getCurrentState())->triggerBounceDownardsOffEnemy(jon, bounceBackVelocity);
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
	jon->m_abilityState = ABILITY_NONE;

	m_hasCompletedSlowMotion = false;

	Assets::getInstance()->addSoundIdToPlayQueue(SOUND_TRIGGER_TRANSFORM);
}

void Jon::VampireToRabbit::execute(Jon* jon)
{
	jon->m_fTransformStateTime += jon->m_fDeltaTime;

	if (jon->m_fTransformStateTime > 0.565f)
	{
		jon->m_formStateMachine->changeState(Jon::Rabbit::getInstance());
	}

	if (!m_hasCompletedSlowMotion && jon->m_fTransformStateTime > 0.125f) {
		m_hasCompletedSlowMotion = true;
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

void Jon::VampireToRabbit::triggerBoost(Jon* jon, float boostVelocity)
{
    jon->m_formStateMachine->revertToPreviousState();
    dynamic_cast<JonFormState *>(jon->m_formStateMachine->getCurrentState())->triggerBoost(jon, boostVelocity);
}

void Jon::VampireToRabbit::triggerBoostOffEnemy(Jon* jon, float boostVelocity)
{
    jon->m_formStateMachine->revertToPreviousState();
    dynamic_cast<JonFormState *>(jon->m_formStateMachine->getCurrentState())->triggerBoostOffEnemy(jon, boostVelocity);
}

void Jon::VampireToRabbit::triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity)
{
    jon->m_formStateMachine->revertToPreviousState();
    dynamic_cast<JonFormState *>(jon->m_formStateMachine->getCurrentState())->triggerBounceDownardsOffEnemy(jon, bounceBackVelocity);
}

Jon::VampireToRabbit::VampireToRabbit() : JonFormState(), m_hasCompletedSlowMotion(false)
{
	// Empty
}
