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
                        if (jon.getNumJumps() >= 1)
                        {
                            m_isShowing = false;
                            m_isRequestingClose = true;
                        }
                        break;
                    case BatPanelType_DoubleJump:
                        if (jon.getNumJumps() >= 2)
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
    m_batInstruction = std::unique_ptr<BatInstruction>(new BatInstruction(x, y, width, height));
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
