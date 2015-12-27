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

Jon* Jon::create(float x, float y, int type)
{
    return new Jon(x, y);
}

Jon::Jon(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_state(JON_ALIVE), m_physicalState(PHYSICAL_GROUNDED), m_actionState(ACTION_NONE), m_abilityState(ABILITY_NONE), m_groundSoundType(GROUND_SOUND_NONE), m_color(1, 1, 1, 1), m_fDeltaTime(0), m_iNumJumps(0), m_iBoostVelocity(0), m_isLanding(false), m_isRightFoot(false), m_isAllowedToMove(false)
{
    resetBounds(width * 0.6796875f, height * 0.8203125f);
    
    m_formStateMachine = std::unique_ptr<StateMachine<Jon>>(new StateMachine<Jon>(this));
    m_formStateMachine->setCurrentState(Rabbit::getInstance());
    m_formStateMachine->getCurrentState()->enter(this);
}

void Jon::update(float deltaTime)
{
    m_fDeltaTime = deltaTime;
    
    for (std::vector<std::unique_ptr<DustCloud>>::iterator i = getDustClouds().begin(); i != getDustClouds().end(); )
    {
        (*i)->update(deltaTime);
        
        if ((*i)->isRequestingDeletion())
        {
            i = getDustClouds().erase(i);
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
    
    if (m_state == JON_DYING_FADING)
    {
        m_color.alpha -= deltaTime / 2;
        if (m_color.alpha < 0)
        {
            m_color.alpha = 0;
            setState(JON_DEAD);
        }
        
        return;
    }
    
    PhysicalEntity::update(deltaTime);
    
    if (m_game->isJonHit() || m_position->getY() < -m_fHeight / 2)
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_DEATH);
        setState(JON_DYING_FADING);
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
        if (!wasGrounded)
        {
            m_isLanding = true;
            m_fStateTime = 0;
            
            m_dustClouds.push_back(std::unique_ptr<DustCloud>(new DustCloud(getPosition().getX(), getPosition().getY() - getHeight() / 2, fabsf(m_velocity->getY() / 12.6674061f))));
            
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
    
    if (m_game->isJonLandingOnSpring(deltaTime))
    {
        m_acceleration->setY(m_fGravity);
        m_velocity->setY(m_iBoostVelocity);
        
        setState(ACTION_JUMPING);
        
        m_iNumJumps = 1;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_JUMP_SPRING);
    }
    
    if (m_game->isJonBlockedVertically(deltaTime))
    {
        m_velocity->sub(0, 2);
    }
    else if (m_game->isJonBlockedHorizontally(deltaTime))
    {
        m_velocity->setX(-2.0f);
        
        m_fStateTime = 0;
    }
    
    if (m_velocity->getX() > m_fMaxSpeed)
    {
        m_velocity->setX(m_fMaxSpeed);
    }
    
    m_formStateMachine->execute();
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

std::vector<std::unique_ptr<DustCloud>>& Jon::getDustClouds()
{
    return m_dustClouds;
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

void Jon::setBoostVelocity(int boostVelocity)
{
    m_iBoostVelocity = boostVelocity;
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
    jon->m_fMaxSpeed = RABBIT_DEFAULT_MAX_SPEED;
    jon->m_fDefaultMaxSpeed = RABBIT_DEFAULT_MAX_SPEED;
    jon->m_fAccelerationX = RABBIT_DEFAULT_ACCELERATION;
    jon->m_fGravity = RABBIT_GRAVITY;
    jon->m_acceleration->setY(RABBIT_GRAVITY);
    
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
            
            if (!m_isBurrowEffective)
            {
                if (jon->m_fAbilityStateTime > 0.30f)
                {
                    jon->setState(ABILITY_NONE);
                    
                    jon->m_acceleration->setX(jon->m_fAccelerationX);
                }
                else if (jon->m_fAbilityStateTime > 0.06f)
                {
                    m_isBurrowEffective = jon->m_game->isBurrowEffective();
                }
            }
            
            if (jon->m_physicalState != PHYSICAL_GROUNDED)
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
        
        jon->m_acceleration->setY(jon->m_fGravity);
        jon->m_velocity->setY(12 - jon->m_iNumJumps * 3);
        
        jon->setState(jon->m_iNumJumps == 0 ? ACTION_JUMPING : ACTION_DOUBLE_JUMPING);
        
        jon->m_iNumJumps++;
    }
}

void Jon::Rabbit::triggerLeftAction(Jon* jon)
{
    // TODO
}

void Jon::Rabbit::triggerRightAction(Jon* jon)
{
    if (jon->m_abilityState == ABILITY_SPINNING_BACK_FIST)
    {
        return;
    }
    
    jon->setState(ABILITY_SPINNING_BACK_FIST);
    
    m_isSpinningBackFistDelivered = false;
}

void Jon::Rabbit::triggerUpAction(Jon* jon)
{
    // TODO
}

void Jon::Rabbit::triggerDownAction(Jon* jon)
{
    if (jon->m_abilityState == ABILITY_BURROW || jon->m_physicalState == PHYSICAL_IN_AIR)
    {
        return;
    }
    
    jon->setState(ABILITY_BURROW);
    
    jon->m_velocity->setX(0);
    jon->m_acceleration->setX(0);
    
    m_isBurrowEffective = false;
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
    jon->m_fMaxSpeed = VAMP_DEFAULT_MAX_SPEED;
    jon->m_fDefaultMaxSpeed = VAMP_DEFAULT_MAX_SPEED;
    jon->m_fAccelerationX = VAMP_DEFAULT_ACCELERATION;
    jon->m_fGravity = VAMP_GRAVITY;
    jon->m_acceleration->setY(VAMP_GRAVITY);
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
    
    if (jon->isFalling())
    {
        if (jon->m_fStateTime > 0.54f && jon->m_iNumJumps > 1 && jon->m_abilityState != ABILITY_GLIDE)
        {
            jon->setState(ABILITY_GLIDE);
            jon->m_fGravity = VAMP_GRAVITY / 8;
            jon->m_velocity->setY(0);
            jon->m_acceleration->setY(jon->m_fGravity);
            jon->m_fMaxSpeed = VAMP_DEFAULT_MAX_SPEED - 1.5f;
        }
        
        jon->setState(ACTION_NONE);
    }
}

void Jon::Vampire::exit(Jon* jon)
{
    // TODO
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
    }
    else if (jon->m_iNumJumps < 2)
    {
        if (m_isFallingAfterGlide)
        {
            jon->m_fStateTime = 0.54f;
        }
        else
        {
            jon->m_fStateTime = 0;
            
            jon->m_acceleration->setY(jon->m_fGravity);
            jon->m_velocity->setY(12 - jon->m_iNumJumps * 3);
            
            jon->setState(jon->m_iNumJumps == 0 ? ACTION_JUMPING : ACTION_DOUBLE_JUMPING);
        }
        
        jon->m_iNumJumps++;
    }
}

void Jon::Vampire::triggerLeftAction(Jon* jon)
{
    // TODO
}

void Jon::Vampire::triggerRightAction(Jon* jon)
{
    // TODO
}

void Jon::Vampire::triggerUpAction(Jon* jon)
{
    // TODO
}

void Jon::Vampire::triggerDownAction(Jon* jon)
{
    // TODO
}

Jon::Vampire::Vampire() : JonFormState(), m_isFallingAfterGlide(false)
{
    // Empty
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
    jon->m_fMaxSpeed = 0;
    jon->m_fDefaultMaxSpeed = 0;
    jon->m_fGravity = 0;
    jon->m_acceleration->setY(0);
    jon->m_velocity->setY(0);
}

void Jon::RabbitToVampire::execute(Jon* jon)
{
    jon->m_fTransformStateTime += jon->m_fDeltaTime;
    
    if (jon->m_fTransformStateTime > 0.95f)
    {
        jon->m_formStateMachine->changeState(Jon::Vampire::getInstance());
    }
}

void Jon::RabbitToVampire::exit(Jon* jon)
{
    // TODO
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
    jon->m_fMaxSpeed = 0;
    jon->m_fDefaultMaxSpeed = 0;
    jon->m_fGravity = 0;
    jon->m_acceleration->setY(0);
    jon->m_velocity->setY(0);
}

void Jon::VampireToRabbit::execute(Jon* jon)
{
    jon->m_fTransformStateTime += jon->m_fDeltaTime;
    
    jon->m_color.green = 1 - jon->m_fTransformStateTime;
    jon->m_color.blue = 1 - jon->m_fTransformStateTime;
    
    if (jon->m_fTransformStateTime > 0.95f)
    {
        jon->m_formStateMachine->changeState(Jon::Rabbit::getInstance());
    }
}

void Jon::VampireToRabbit::exit(Jon* jon)
{
    jon->m_color.green = 1;
    jon->m_color.blue = 1;
}
