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
                case 2:
                    return BatGoalType_DoubleJump;
                case 3:
                    return BatGoalType_Vampire;
                case 10:
                    return BatGoalType_Drill;
                case 11:
                    return BatGoalType_Stomp;
                case 21:
                    return BatGoalType_Dash;
            }
        }
    }
    
    return BatGoalType_None;
}

BatPanel::BatPanel() :
m_game(nullptr),
m_type(BatGoalType_None),
m_isRequestingInput(false),
m_isAcknowledgedPart1(false),
m_isAcknowledgedPart2(false),
m_isAcknowledgedPart3(false),
m_hasSwiped(false)
{
    m_bat = std::unique_ptr<Bat>(new Bat());
    m_batInstruction = std::unique_ptr<BatInstruction>(new BatInstruction());
}

void BatPanel::config(Game *game, int world, int level)
{
    m_game = game;
    m_type = calcBatGoalType(world, level);
    
    m_isRequestingInput = false;
    m_isAcknowledgedPart1 = false;
    m_isAcknowledgedPart2 = false;
    m_isAcknowledgedPart3 = false;
    m_hasSwiped = false;
    
    m_bat->getPosition().set(1337, 1337);
    m_batInstruction->getPosition().set(1337, 1337);
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
}

void BatPanel::updateJump(GameScreen* gs)
{
    if (!m_isAcknowledgedPart1)
    {
        Jon& jon = m_game->getJon();
        jon.setUserActionPrevented(true);
        
        if (jon.getPosition().getX() > 17.7f)
        {
            if (!m_isRequestingInput)
            {
                jon.getPosition().setX(17.7f);
                
                float x = jon.getPosition().getX() + 1.6f;
                float y = jon.getPosition().getY() + 1.6f;
                m_bat->naviPoof(x, y);
                
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
                    m_batInstruction->open(BatInstructionType_Tap, m_bat->getPosition().getX() + 3, m_bat->getPosition().getY() + 2);
                }
            }
        }
    }
}

void BatPanel::updateDoubleJump(GameScreen* gs)
{
    
}

void BatPanel::updateVampire(GameScreen* gs)
{
    
}

void BatPanel::updateDrill(GameScreen* gs)
{
    
}

void BatPanel::updateStomp(GameScreen* gs)
{
    
}

void BatPanel::updateDash(GameScreen* gs)
{
    
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
