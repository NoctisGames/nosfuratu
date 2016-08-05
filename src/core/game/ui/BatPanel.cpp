//
//  BatPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 4/4/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "BatPanel.h"

#include "Game.h"
#include "OverlapTester.h"

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
                    case BatPanelType_Burrow:
                        if (jon.getAbilityState() == ABILITY_BURROW)
                        {
                            dismiss();
                        }
                        break;
                    case BatPanelType_OwlDig:
                        if (jon.getPosition().getY() > 12)
                        {
                            dismiss();
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

void BatInstruction::dismiss()
{
    m_isShowing = false;
    m_isRequestingClose = true;
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
            m_fWidth = 2.091796875f;
            m_fHeight = 0.984375f;
            break;
        case BatPanelType_OwlDig:
            m_fWidth = 2.935546875f;
            m_fHeight = 1.037109375f;
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
m_isOpening(false),
m_isAcknowledged(true)
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
    else if (m_isOpen)
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

void BatPanel::handleTouch(Vector2D& touchPoint)
{
    if (m_isAcknowledged)
    {
        return;
    }
    
    if (OverlapTester::isPointInRectangle(touchPoint, getMainBounds()))
    {
        m_batInstruction->dismiss();
        
        m_isAcknowledged = true;
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
    m_isAcknowledged = m_type == BatPanelType_OwlDig || m_type == BatPanelType_Burrow;
}

bool BatPanel::isOpen()
{
    return m_isOpen;
}

bool BatPanel::isOpening()
{
    return m_isOpening;
}

bool BatPanel::isAcknowledged()
{
    return m_isAcknowledged;
}

BatInstruction& BatPanel::getBatInstruction()
{
    return *m_batInstruction;
}

Color& BatPanel::getColor()
{
    return m_color;
}
