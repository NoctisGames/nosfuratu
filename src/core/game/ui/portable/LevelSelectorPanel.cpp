//
//  LevelSelectorPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/28/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "LevelSelectorPanel.h"
#include "OverlapTester.h"

LevelSelectorPanel::LevelSelectorPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_iWorld(1), m_iLevel(1), m_isOpen(false)
{
    float l = x - width / 2;
    float b = y - height / 2;
    
    m_toggleWorldButton = new NGRect(l + width * 0.07647058823529f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f);
    m_toggleLevelButton = new NGRect(l + width * 0.55490196078431f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f);
    m_confirmButton = new NGRect(l + width * 0.12941176470588f, b + height * 0.07843137254902f, width * 0.74117647058824f, height * 0.23921568627451f);
    
    m_worldTextPosition = new Vector2D(m_toggleWorldButton->getLeft() + m_toggleWorldButton->getWidth() - m_toggleWorldButton->getWidth() / 3, m_toggleWorldButton->getBottom() + m_toggleWorldButton->getHeight() / 2);
    m_levelTextPosition = new Vector2D(m_toggleLevelButton->getLeft() + m_toggleLevelButton->getWidth() - m_toggleLevelButton->getWidth() / 3, m_toggleLevelButton->getBottom() + m_toggleLevelButton->getHeight() / 2);
}

LevelSelectorPanel::~LevelSelectorPanel()
{
    delete m_toggleWorldButton;
    delete m_toggleLevelButton;
    delete m_confirmButton;
    delete m_worldTextPosition;
    delete m_levelTextPosition;
}

void LevelSelectorPanel::open()
{
    m_isOpen = true;
}

void LevelSelectorPanel::close()
{
    m_isOpen = false;
}

int LevelSelectorPanel::handleTouch(ScreenEvent& te, Vector2D& touchPoint)
{
    if (te.getType() == ScreenEventType_UP)
    {
        if (OverlapTester::isPointInNGRect(touchPoint, *m_toggleWorldButton))
        {
            m_iWorld++;
            if (m_iWorld > 5)
            {
                m_iWorld = 1;
            }
            
            return LEVEL_SELECTOR_PANEL_RC_HANDLED;
        }
        else if (OverlapTester::isPointInNGRect(touchPoint, *m_toggleLevelButton))
        {
            m_iLevel++;
            if (m_iLevel > 21)
            {
                m_iLevel = 1;
            }
            
            return LEVEL_SELECTOR_PANEL_RC_HANDLED;
        }
        else if (OverlapTester::isPointInNGRect(touchPoint, *m_confirmButton))
        {
            m_isOpen = false;
            return LEVEL_SELECTOR_PANEL_RC_CONFIRM;
        }
    }
    
    return LEVEL_SELECTOR_PANEL_RC_UNHANDLED;
}

Vector2D& LevelSelectorPanel::getWorldTextPosition()
{
    return *m_worldTextPosition;
}

Vector2D& LevelSelectorPanel::getLevelTextPosition()
{
    return *m_levelTextPosition;
}

float LevelSelectorPanel::getTextSize()
{
    return m_toggleWorldButton->getWidth() / 3;
}

int LevelSelectorPanel::getWorld()
{
    return m_iWorld;
}

int LevelSelectorPanel::getLevel()
{
    return m_iLevel;
}

bool LevelSelectorPanel::isOpen()
{
    return m_isOpen;
}

RTTI_IMPL(LevelSelectorPanel, PhysicalEntity);
