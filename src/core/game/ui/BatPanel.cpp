//
//  BatPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 4/4/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "BatPanel.h"
#include "Game.h"

BatInstruction::BatInstruction(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_game(nullptr),
m_type(BatPanelType_None),
m_color(1, 1, 1, 0),
m_isShowing(false),
m_isRequestingClose(false)
{
    // Empty
}

void BatInstruction::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_isShowing)
    {
        if (m_color.alpha < 1)
        {
            m_color.alpha += deltaTime * 4;
            
            if (m_color.alpha > 1)
            {
                m_color.alpha = 1;
            }
        }
        else
        {
            if (m_game && m_game->getJons().size() > 0)
            {
                Jon& jon = m_game->getJon();
                
                switch (m_type)
                {
                    case BatPanelType_Jump:
                        if (jon.getNumTriggeredJumps() >= 1)
                        {
                            m_isShowing = false;
                            m_isRequestingClose = true;
                        }
                        break;
                    case BatPanelType_DoubleJump:
                        if (jon.getNumTriggeredJumps() >= 2)
                        {
                            m_isShowing = false;
                            m_isRequestingClose = true;
                        }
                        break;
                    case BatPanelType_Transform:
                        if (jon.isVampire())
                        {
                            show(BatPanelType_UpwardStrikeGlide);
                        }
                        break;
                    case BatPanelType_UpwardStrikeGlide:
                        if (jon.getNumTriggeredJumps() >= 2)
                        {
                            m_isShowing = false;
                            m_isRequestingClose = true;
                        }
                        break;
                    case BatPanelType_Burrow:
                        if (jon.getAbilityState() == ABILITY_BURROW)
                        {
                            m_isShowing = false;
                            m_isRequestingClose = true;
                        }
                        break;
                    case BatPanelType_OwlDig:
                        if (jon.getPosition().getY() > 12)
                        {
                            m_isShowing = false;
                            m_isRequestingClose = true;
                        }
                        break;
                    case BatPanelType_Stomp:
                        if (jon.getAbilityState() == ABILITY_STOMP)
                        {
                            m_isShowing = false;
                            m_isRequestingClose = true;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void BatInstruction::setGame(Game* game)
{
    m_game = game;
}

void BatInstruction::show(BatPanelType type)
{
    m_type = type;
    m_color = Color(1, 1, 1, 0);
    m_fStateTime = 0;
    m_isShowing = true;
    m_isRequestingClose = false;
    
    switch (m_type)
    {
        case BatPanelType_Jump:
            m_fWidth = 2.583984375f;
            m_fHeight = 0.861328125f;
            break;
        case BatPanelType_DoubleJump:
            m_fWidth = 3.05859375f;
            m_fHeight = 1.037109375f;
            break;
        case BatPanelType_Burrow:
            m_fWidth = 2.056640625f;
            m_fHeight = 0.73828125f;
            break;
        case BatPanelType_OwlDig:
            m_fWidth = 2.671875f;
            m_fHeight = 1.001953125f;
            break;
        case BatPanelType_Transform:
            m_fWidth = 1.96875f;
            m_fHeight = 0.685546875f;
            break;
        case BatPanelType_UpwardStrikeGlide:
            m_fWidth = 3.0234375f;
            m_fHeight = 1.001953125f;
            break;
        case BatPanelType_Stomp:
            m_fWidth = 2.267578125f;
            m_fHeight = 0.826171875f;
            break;
        case BatPanelType_None:
        default:
            break;
    }
}

bool BatInstruction::isShowing()
{
    return m_isShowing;
}

bool BatInstruction::isRequestingClose()
{
    return m_isRequestingClose;
}

BatPanelType BatInstruction::getBatPanelType()
{
    return m_type;
}

Color& BatInstruction::getColor()
{
    return m_color;
}

BatPanel::BatPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_batInstruction(nullptr),
m_type(BatPanelType_None),
m_color(1, 1, 1, 1),
m_isOpen(false),
m_isOpening(false)
{
    m_batInstruction = std::unique_ptr<BatInstruction>(new BatInstruction(x - 1.23046875f / 2, y, width, height));
}

void BatPanel::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_isOpening)
    {
        if (m_fStateTime > 0.40f)
        {
            m_fStateTime = 0;
            m_isOpening = false;
            m_isOpen = true;
            
            m_batInstruction->show(m_type);
        }
    }
    else
    {
        if (m_batInstruction->isRequestingClose())
        {
            if (m_color.alpha > 0)
            {
                m_color.alpha -= deltaTime * 2;
                m_batInstruction->getColor().alpha = m_color.alpha;
            }
            else
            {
                m_isOpen = false;
            }
        }
        else
        {
            m_batInstruction->update(deltaTime);
        }
    }
}

void BatPanel::setGame(Game* game)
{
    m_batInstruction->setGame(game);
}

void BatPanel::open(BatPanelType type)
{
    if (type == BatPanelType_None)
    {
        return;
    }
    
    m_type = type;
    m_color = Color(1, 1, 1, 1);
    m_fStateTime = 0;
    m_isOpening = true;
    m_isOpen = false;
}

bool BatPanel::isOpen()
{
    return m_isOpen;
}

bool BatPanel::isOpening()
{
    return m_isOpening;
}

BatInstruction& BatPanel::getBatInstruction()
{
    return *m_batInstruction;
}

Color& BatPanel::getColor()
{
    return m_color;
}
