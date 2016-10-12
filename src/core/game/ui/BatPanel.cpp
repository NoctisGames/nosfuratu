//
//  BatPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 4/4/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "BatPanel.h"

#include "Game.h"
#include "GameScreen.h"
#include "OverlapTester.h"

BatGoalType calcBatGoalType(int world, int level)
{
    switch (world)
    {
        case 1:
        {
            switch (level)
            {
                case 1:
                    return BatGoalType_Jump;
                case 3:
                    return BatGoalType_DoubleJump;
                case 5:
                    return BatGoalType_Vampire;
                case 10:
                    return BatGoalType_Drill;
                case 12:
                    return BatGoalType_Stomp;
            }
        }
    }
    
    return BatGoalType_None;
}

BatPanel::BatPanel() :
m_game(nullptr),
m_type(BatGoalType_None),
m_fJonX(0),
m_isRequestingInput(false),
m_isAcknowledgedPart1(false),
m_isAcknowledgedPart2(false),
m_isAcknowledgedPart3(false),
m_isAcknowledgedPart4(false),
m_isAcknowledgedPart5(false),
m_hasTriggeredRequestedAction(false),
m_hasSwiped(false)
{
    m_bat = std::unique_ptr<Bat>(new Bat());
    m_batInstruction = std::unique_ptr<BatInstruction>(new BatInstruction());
}

void BatPanel::config(Game *game, int world, int level)
{
    reset();
    
    m_game = game;
    m_type = calcBatGoalType(world, level);
}

void BatPanel::config(Game* game, BatGoalType type)
{
    reset();
    
    m_game = game;
    m_type = type;
}

void BatPanel::reset()
{
    m_game = nullptr;
    m_type = BatGoalType_None;
    
    m_fJonX = 0;
    m_isRequestingInput = false;
    m_isAcknowledgedPart1 = false;
    m_isAcknowledgedPart2 = false;
    m_isAcknowledgedPart3 = false;
    m_isAcknowledgedPart4 = false;
    m_isAcknowledgedPart5 = false;
    m_hasTriggeredRequestedAction = false;
    m_hasSwiped = false;
    
    m_bat->reset();
    m_batInstruction->reset();
}

void BatPanel::update(GameScreen* gs)
{
    if (m_type == BatGoalType_None)
    {
        return;
    }
    
    switch (m_type)
    {
        case BatGoalType_Jump:
            updateJump(gs);
            break;
        case BatGoalType_DoubleJump:
            updateDoubleJump(gs);
            break;
        case BatGoalType_Vampire:
            updateVampire(gs);
            break;
        case BatGoalType_Drill:
            updateDrill(gs);
            break;
		case BatGoalType_DrillToDamageOwl:
            updateDrillToDamageOwl(gs);
			break;
        case BatGoalType_Stomp:
            updateStomp(gs);
            break;
        case BatGoalType_Dash:
            updateDash(gs);
            break;
        default:
            break;
    }
    
    m_bat->update(gs->m_fDeltaTime);
    m_batInstruction->update(gs->m_fDeltaTime);
    
    if (m_isRequestingInput)
    {
        bool isChaseCam = m_type == BatGoalType_DrillToDamageOwl;
        float paddingX = m_type == BatGoalType_DrillToDamageOwl ? 4 : 0;
        gs->m_renderer->updateCameraToFollowJon(*m_game, this, gs->m_fDeltaTime, paddingX, isChaseCam);
    }
}

void BatPanel::updateJump(GameScreen* gs)
{
    if (!m_isAcknowledgedPart1)
    {
        Jon& jon = m_game->getJon();
		jon.setUserActionPrevented(true);
        
        if (jon.getPosition().getX() > 17.7f
			&& jon.getPosition().getX() < 20)
        {
            if (!m_isRequestingInput)
            {
                jon.getPosition().setX(17.7f);
                
                showBatNearJon(jon);
                
                m_isRequestingInput = true;
            }
        }
		else if (jon.getPosition().getX() > 20)
		{
			jon.setUserActionPrevented(false);
		}
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            continue;
                        case DRAGGED:
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                jon.setUserActionPrevented(false);
                                jon.triggerJump();
                                
                                m_isAcknowledgedPart1 = true;
                                m_isRequestingInput = false;
                                
                                m_batInstruction->close();
                                
                                gs->processTouchEvents();
                                
                                return;
                            }
                            
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_Tap);
                }
            }
        }
    }
}

void BatPanel::updateDoubleJump(GameScreen* gs)
{
    if (!m_isAcknowledgedPart1)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (jon.getPosition().getX() > 19.2f
			&& jon.getPosition().getX() < 21)
        {
            if (!m_isRequestingInput)
            {
                jon.getPosition().setX(19.2f);
                
                showBatNearJon(jon);
                
                jon.enableAbility(FLAG_ABILITY_DOUBLE_JUMP);
                
                m_isRequestingInput = true;
            }
        }
		else if (jon.getPosition().getX() > 21)
		{
			jon.setUserActionPrevented(false);
		}
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            continue;
                        case DRAGGED:
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                jon.setUserActionPrevented(false);
                                jon.triggerJump();
                                jon.setUserActionPrevented(true);
                                
                                m_isAcknowledgedPart1 = true;
                                m_isRequestingInput = false;
                                
                                m_batInstruction->close();
                                
                                gs->processTouchEvents();
                                
                                return;
                            }
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_Tap);
                }
            }
        }
    }
    else if (!m_isAcknowledgedPart2)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (jon.getPosition().getX() > 29.1f
			&& jon.getPosition().getX() < 32)
        {
            if (!m_isRequestingInput)
            {
                jon.getPosition().setX(29.1f);
                
                showBatNearJon(jon);
                
                m_isRequestingInput = true;
            }
        }
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            continue;
                        case DRAGGED:
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                jon.setUserActionPrevented(false);
                                jon.triggerJump();
                                
                                m_isAcknowledgedPart2 = true;
                                m_isRequestingInput = false;
                                
                                m_batInstruction->close();
                                
                                gs->processTouchEvents();
                                
                                return;
                            }
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_Tap);
                }
            }
        }
    }
}

void BatPanel::updateVampire(GameScreen* gs)
{
    if (!m_isAcknowledgedPart1)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (jon.getPosition().getX() > 7
			&& jon.getPosition().getX() < 10)
        {
            if (!m_isRequestingInput)
            {
                jon.getPosition().setX(7);
                
                showBatNearJon(jon);
                
                jon.enableAbility(FLAG_ABILITY_TRANSFORM);
                
                m_isRequestingInput = true;
                
                m_fJonX = -1;
            }
        }
		else if (jon.getPosition().getX() > 10)
		{
			jon.setUserActionPrevented(false);
		}
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                if (jon.isTransformingIntoVampire() || jon.isRevertingToRabbit())
                {
                    if (jon.getTransformStateTime() < 0.0625f)
                    {
                        gs->m_fDeltaTime /= 8;
                    }
                    else
                    {
                        if (!gs->m_isReleasingShockwave)
                        {
                            gs->m_fShockwaveCenterX = jon.getPosition().getX();
                            gs->m_fShockwaveCenterY = jon.getPosition().getY();
                            gs->m_fShockwaveElapsedTime = 0.0f;
                            gs->m_isReleasingShockwave = true;
                            
                            m_isAcknowledgedPart1 = true;
                            m_isRequestingInput = false;
                            
                            m_batInstruction->close();
                            
                            gs->processTouchEvents();
                            
                            return;
                        }
                    }
                }
                
                if (gs->m_isReleasingShockwave)
                {
                    gs->m_fShockwaveElapsedTime += gs->m_fDeltaTime * 1.2f;
                    
                    if (gs->m_fShockwaveElapsedTime > 2)
                    {
                        gs->m_fShockwaveElapsedTime = 0;
                        gs->m_isReleasingShockwave = false;
                    }
                }
                
                if (gs->m_isScreenHeldDown)
                {
                    gs->m_fScreenHeldTime += gs->m_fDeltaTime;
                    
                    if (gs->m_fScreenHeldTime > 0.4f)
                    {
                        jon.setUserActionPrevented(false);
                        jon.triggerTransform();
                        jon.setUserActionPrevented(true);
                        
                        m_fJonX = jon.getPosition().getX();
                        
                        gs->m_isScreenHeldDown = false;
                        gs->m_fShockwaveElapsedTime = 0;
                        gs->m_isReleasingShockwave = false;
                    }
                }
                
                if (m_fJonX > 0)
                {
                    jon.update(gs->m_fDeltaTime);
                    jon.getPosition().setX(m_fJonX);
                }
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            if (isJonAlive)
                            {
                                gs->m_isScreenHeldDown = true;
                                gs->m_fScreenHeldTime = 0.0f;
                            }
                            continue;
                        case DRAGGED:
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                if (gs->m_fScreenHeldTime > 0.4f)
                                {
                                    jon.setUserActionPrevented(false);
                                    jon.triggerCancelTransform();
                                    jon.setUserActionPrevented(true);
                                }
                                
                                gs->m_isScreenHeldDown = false;
                                gs->m_fScreenHeldTime = 0;
                                
                                m_fJonX = -1;
                            }
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_TapHold);
                }
            }
        }
    }
    else if (!m_isAcknowledgedPart2)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (jon.getPosition().getX() > 34.2f
			&& jon.getPosition().getX() < 37)
        {
            if (!m_isRequestingInput)
            {
                jon.getPosition().setX(34.2f);
                
                showBatNearJon(jon);
                
                m_isRequestingInput = true;
            }
        }
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            continue;
                        case DRAGGED:
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                jon.setUserActionPrevented(false);
                                jon.triggerJump();
                                jon.setUserActionPrevented(true);
                                
                                m_isAcknowledgedPart2 = true;
                                m_isRequestingInput = false;
                                
                                m_batInstruction->close();
                                
                                gs->processTouchEvents();
                                
                                return;
                            }
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_Tap);
                }
            }
        }
    }
    else if (!m_isAcknowledgedPart3)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (jon.isFalling())
        {
            if (!m_isRequestingInput)
            {
				showBatNearJon(jon);
                
                m_isRequestingInput = true;
            }
        }
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            continue;
                        case DRAGGED:
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                jon.setUserActionPrevented(false);
                                jon.triggerJump();
                                jon.setUserActionPrevented(true);
                                
                                m_isAcknowledgedPart3 = true;
                                m_isRequestingInput = false;
                                
                                m_batInstruction->close();
                                
                                gs->processTouchEvents();
                                
                                return;
                            }
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_Tap);
                }
            }
        }
    }
    else if (!m_isAcknowledgedPart4)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (jon.getPosition().getX() > 83
			&& jon.getPosition().getX() < 86)
        {
            if (!m_isRequestingInput)
            {
                jon.getPosition().setX(83);
                
                showBatNearJon(jon);
                
                m_isRequestingInput = true;
                
                m_fJonX = -1;
            }
        }
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                if (jon.isTransformingIntoVampire() || jon.isRevertingToRabbit())
                {
                    if (jon.getTransformStateTime() < 0.0625f)
                    {
                        gs->m_fDeltaTime /= 8;
                    }
                    else
                    {
                        if (!gs->m_isReleasingShockwave)
                        {
                            gs->m_fShockwaveCenterX = jon.getPosition().getX();
                            gs->m_fShockwaveCenterY = jon.getPosition().getY();
                            gs->m_fShockwaveElapsedTime = 0.0f;
                            gs->m_isReleasingShockwave = true;
                            
							m_isAcknowledgedPart4 = true;
                            m_isRequestingInput = false;
                            
                            m_batInstruction->close();
                            
                            gs->processTouchEvents();
                            
                            return;
                        }
                    }
                }
                
                if (gs->m_isReleasingShockwave)
                {
                    gs->m_fShockwaveElapsedTime += gs->m_fDeltaTime * 1.2f;
                    
                    if (gs->m_fShockwaveElapsedTime > 2)
                    {
                        gs->m_fShockwaveElapsedTime = 0;
                        gs->m_isReleasingShockwave = false;
                    }
                }
                
                if (gs->m_isScreenHeldDown)
                {
                    gs->m_fScreenHeldTime += gs->m_fDeltaTime;
                    
                    if (gs->m_fScreenHeldTime > 0.4f)
                    {
                        jon.setUserActionPrevented(false);
                        jon.triggerTransform();
                        jon.setUserActionPrevented(true);
                        
                        m_fJonX = jon.getPosition().getX();
                        
                        gs->m_isScreenHeldDown = false;
                        gs->m_fShockwaveElapsedTime = 0;
                        gs->m_isReleasingShockwave = false;
                    }
                }
                
                if (m_fJonX > 0)
                {
                    jon.update(gs->m_fDeltaTime);
                    jon.getPosition().setX(m_fJonX);
                }
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            if (isJonAlive)
                            {
                                gs->m_isScreenHeldDown = true;
                                gs->m_fScreenHeldTime = 0.0f;
                            }
                            continue;
                        case DRAGGED:
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                if (gs->m_fScreenHeldTime > 0.4f)
                                {
                                    jon.setUserActionPrevented(false);
                                    jon.triggerCancelTransform();
                                    jon.setUserActionPrevented(true);
                                }
                                
                                gs->m_isScreenHeldDown = false;
                                gs->m_fScreenHeldTime = 0;
                                
                                m_fJonX = -1;
                            }
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_TapHold);
                }
            }
        }
    }
    else if (!m_isAcknowledgedPart5)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (jon.getPosition().getX() > 88
			&& jon.getPosition().getX() < 90)
        {
            if (!m_isRequestingInput)
            {
                jon.getPosition().setX(88);
                
                showBatNearJon(jon);
                
                m_isRequestingInput = true;
            }
        }
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            continue;
                        case DRAGGED:
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                jon.setUserActionPrevented(false);
                                jon.triggerJump();
                                
                                m_isAcknowledgedPart5 = true;
                                m_isRequestingInput = false;
                                
                                m_batInstruction->close();
                                
                                gs->processTouchEvents();
                                
                                return;
                            }
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_Tap);
                }
            }
        }
    }
}

void BatPanel::updateDrill(GameScreen* gs)
{
    if (!m_isAcknowledgedPart1)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (jon.getPosition().getX() > 17
			&& jon.getPosition().getX() < 20)
        {
            if (!m_isRequestingInput && !m_hasTriggeredRequestedAction)
            {
                jon.getPosition().setX(17);
                
                showBatNearJon(jon);
                
                jon.enableAbility(FLAG_ABILITY_RABBIT_DOWN);
                
                m_isRequestingInput = true;
            }
        }
		else if (jon.getPosition().getX() > 20)
		{
			jon.setUserActionPrevented(false);
		}
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            if (isJonAlive)
                            {
                                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                            }
                            continue;
                        case DRAGGED:
                            if (isJonAlive && !m_hasSwiped)
                            {
                                if (gs->m_touchPoint->getY() <= (gs->m_touchPointDown->getY() - SWIPE_HEIGHT))
                                {
                                    jon.setUserActionPrevented(false);
                                    
                                    // Swipe Down
                                    jon.setPhysicalState(PHYSICAL_GROUNDED);
                                    jon.triggerDownAction();
                                    
                                    jon.setUserActionPrevented(true);
                                    
                                    m_hasTriggeredRequestedAction = true;
                                    m_isRequestingInput = false;
                                    
                                    m_batInstruction->close();
                                    
                                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                                    
                                    gs->processTouchEvents();
                                    
                                    m_hasSwiped = true;
                                    
                                    return;
                                }
                            }
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                m_hasSwiped = false;
                                
                                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                            }
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_SwipeDown);
                }
            }
        }
        else if (m_hasTriggeredRequestedAction)
        {
            if (jon.getAbilityState() == ABILITY_NONE)
            {
                m_isAcknowledgedPart1 = true;
                
                jon.setUserActionPrevented(false);
            }
        }
    }
}

void BatPanel::updateDrillToDamageOwl(GameScreen* gs)
{
    if (!m_isAcknowledgedPart1)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (!m_isRequestingInput && !m_hasTriggeredRequestedAction)
        {
            showBatNearJon(jon);
            
            m_isRequestingInput = true;
        }
        
        if (m_isRequestingInput)
        {
            if (m_batInstruction->isOpen())
            {
                bool isJonAlive = jon.isAlive();
                
                for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
                {
                    gs->touchToWorld(*(*i));
                    
                    switch ((*i)->getTouchType())
                    {
                        case DOWN:
                            if (isJonAlive)
                            {
                                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                            }
                            continue;
                        case DRAGGED:
                            if (isJonAlive && !m_hasSwiped)
                            {
                                if (gs->m_touchPoint->getY() <= (gs->m_touchPointDown->getY() - SWIPE_HEIGHT))
                                {
                                    jon.setUserActionPrevented(false);
                                    
                                    // Swipe Down
                                    jon.setPhysicalState(PHYSICAL_GROUNDED);
                                    jon.triggerDownAction();
                                    
                                    jon.setUserActionPrevented(true);
                                    
                                    m_hasTriggeredRequestedAction = true;
                                    m_isRequestingInput = false;
                                    
                                    m_batInstruction->close();
                                    
                                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                                    
                                    gs->processTouchEvents();
                                    
                                    m_hasSwiped = true;
                                    
                                    return;
                                }
                            }
                            continue;
                        case UP:
                            if (isJonAlive)
                            {
                                m_hasSwiped = false;
                                
                                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                            }
                            break;
                    }
                }
            }
            else if (!m_batInstruction->isOpening())
            {
                if (m_bat->isInPosition())
                {
                    showBatInstruction(BatInstructionType_SwipeDown);
                }
            }
        }
        else if (m_hasTriggeredRequestedAction)
        {
            if (jon.getAbilityState() == ABILITY_NONE)
            {
                m_isAcknowledgedPart1 = true;
                
                jon.setUserActionPrevented(false);
            }
        }
    }
}

void BatPanel::updateStomp(GameScreen* gs)
{
	if (!m_isAcknowledgedPart1)
	{
		Jon& jon = m_game->getJon();

		if (jon.getPosition().getX() < 12)
		{
			jon.setUserActionPrevented(true);
		}

		if (jon.getPosition().getX() > 28
			&& jon.getPosition().getX() < 30)
		{
			if (!m_isRequestingInput && !m_hasTriggeredRequestedAction)
			{
				jon.getPosition().setX(28);

				showBatNearJon(jon);

				m_isRequestingInput = true;
			}
		}
		else if (jon.getPosition().getX() > 30)
		{
			jon.setUserActionPrevented(false);
		}

		if (m_isRequestingInput)
		{
			if (m_batInstruction->isOpen())
			{
				bool isJonAlive = jon.isAlive();

				for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
				{
					gs->touchToWorld(*(*i));

					switch ((*i)->getTouchType())
					{
					case DOWN:
						if (isJonAlive)
						{
							gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
						}
						continue;
					case DRAGGED:
						if (isJonAlive && !m_hasSwiped)
						{
							if (gs->m_touchPoint->getY() <= (gs->m_touchPointDown->getY() - SWIPE_HEIGHT))
							{
								jon.setUserActionPrevented(false);

								// Swipe Down
								jon.triggerDownAction();

								jon.setUserActionPrevented(true);

								m_hasTriggeredRequestedAction = true;
								m_isRequestingInput = false;

								m_batInstruction->close();

								gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());

								gs->processTouchEvents();

								m_hasSwiped = true;

								return;
							}
						}
						continue;
					case UP:
						if (isJonAlive)
						{
							m_hasSwiped = false;

							gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
						}
						break;
					}
				}
			}
			else if (!m_batInstruction->isOpening())
			{
				if (m_bat->isInPosition())
				{
					showBatInstruction(BatInstructionType_SwipeDown);
				}
			}
		}
		else if (m_hasTriggeredRequestedAction)
		{
			if (jon.getAbilityState() == ABILITY_NONE)
			{
				m_isAcknowledgedPart1 = true;

				jon.setUserActionPrevented(false);
			}
		}
	}
}

void BatPanel::updateDash(GameScreen* gs)
{
	if (!m_isAcknowledgedPart1)
	{
		Jon& jon = m_game->getJon();
		jon.setUserActionPrevented(true);

		if (jon.getPosition().getX() > 628
			&& jon.getPosition().getX() < 630)
		{
			if (!m_isRequestingInput && !m_hasTriggeredRequestedAction)
			{
				jon.getPosition().setX(628);

				showBatNearJon(jon);

				jon.enableAbility(FLAG_ABILITY_VAMPIRE_RIGHT);

				m_isRequestingInput = true;
			}
		}
		else if (jon.getPosition().getX() > 630)
		{
			jon.setUserActionPrevented(false);
		}

		if (m_isRequestingInput)
		{
			if (m_batInstruction->isOpen())
			{
				bool isJonAlive = jon.isAlive();

				for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
				{
					gs->touchToWorld(*(*i));

					switch ((*i)->getTouchType())
					{
					case DOWN:
						if (isJonAlive)
						{
							gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
						}
						continue;
					case DRAGGED:
						if (isJonAlive && !m_hasSwiped)
						{
							if (gs->m_touchPoint->getX() >= (gs->m_touchPointDown->getX() + SWIPE_WIDTH))
							{
								jon.setUserActionPrevented(false);

								// Swipe Right
								jon.triggerRightAction();

								jon.setUserActionPrevented(true);

								m_hasTriggeredRequestedAction = true;
								m_isRequestingInput = false;

								m_batInstruction->close();

								gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());

								gs->processTouchEvents();

								m_hasSwiped = true;

								return;
							}
						}
						continue;
					case UP:
						if (isJonAlive)
						{
							m_hasSwiped = false;

							gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
						}
						break;
					}
				}
			}
			else if (!m_batInstruction->isOpening())
			{
				if (m_bat->isInPosition())
				{
					showBatInstruction(BatInstructionType_SwipeRight);
				}
			}
		}
		else if (m_hasTriggeredRequestedAction)
		{
			if (jon.getAbilityState() == ABILITY_NONE)
			{
				m_isAcknowledgedPart1 = true;

				jon.setUserActionPrevented(false);
			}
		}
	}
}

void BatPanel::showBatNearJon(Jon& jon)
{
    float x = jon.getPosition().getX() + 1.6f;
    float y = jon.getPosition().getY() + 1.2f;
    
    if (m_bat->isInPosition())
    {
        m_bat->moveTo(x, y);
    }
    else
    {
        m_bat->naviPoof(x, y);
    }
}

void BatPanel::showBatInstruction(BatInstructionType type)
{
    m_batInstruction->open(type, m_bat->getPosition().getX() + 2.7f, m_bat->getPosition().getY() + 2);
}

void BatPanel::handleTouchInput(GameScreen* gs)
{
    Jon& jon = m_game->getJon();
    bool isJonAlive = jon.isAlive();
    
    for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld(*(*i));
        
        switch ((*i)->getTouchType())
        {
            case DOWN:
                if (isJonAlive)
                {
                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                    gs->m_isScreenHeldDown = true;
                    gs->m_fScreenHeldTime = 0.0f;
                }
                continue;
            case DRAGGED:
                if (isJonAlive && !m_hasSwiped)
                {
                    if (gs->m_touchPoint->getX() >= (gs->m_touchPointDown->getX() + SWIPE_WIDTH))
                    {
                        // Swipe Right
                        jon.triggerRightAction();
                        m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getX() <= (gs->m_touchPointDown->getX() - SWIPE_WIDTH))
                    {
                        // Swipe Left
                        jon.triggerLeftAction();
                        m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() >= (gs->m_touchPointDown->getY() + SWIPE_HEIGHT))
                    {
                        // Swipe Up
                        jon.triggerUpAction();
                        m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() <= (gs->m_touchPointDown->getY() - SWIPE_HEIGHT))
                    {
                        // Swipe Down
                        jon.triggerDownAction();
                        m_hasSwiped = true;
                    }
                    
                    if (m_hasSwiped)
                    {
                        gs->m_isScreenHeldDown = false;
                        gs->m_fScreenHeldTime = 0.0f;
                    }
                }
                continue;
            case UP:
                if (isJonAlive)
                {
                    if (!m_hasSwiped && gs->m_fScreenHeldTime < 0.4f)
                    {
                        jon.triggerJump();
                    }
                    
                    if (gs->m_fScreenHeldTime > 0.4f)
                    {
                        jon.triggerCancelTransform();
                    }
                    
                    gs->m_isScreenHeldDown = false;
                    gs->m_fScreenHeldTime = 0;
                    
                    m_hasSwiped = false;
                    
                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                }
                break;
        }
    }
}
