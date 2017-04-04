//
//  LevelCompletePanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/28/17.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "LevelCompletePanel.h"

#include "GameButton.h"

LevelCompletePanel::LevelCompletePanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    float w = m_fWidth;
    float h = m_fHeight;
    float l = x - w / 2;
    float b = y - (h / 2);
    
    float iw = w * 0.09375f;
    float ih = h * 0.140625f;
    
    m_replayButton = new GameButton(l + w * 0.010416666666667f + iw / 2, b + h - h * 0.015625f - ih / 2, iw, ih, GameButtonType_Replay);
    m_continueButton = new GameButton(l + w - w * 0.010416666666667f - w * iw / 2, b + h - h * 0.015625f - ih / 2, iw, ih, GameButtonType_ContinueToLevelSelect);
    m_leaderboardsButton = new GameButton(l + w * 0.92578125f, b + h * 0.10546875f, iw, ih, GameButtonType_Leaderboards);
    
    m_clockIcon = new ScoreIcon(l + w * 0.072916666666667f, b + h * 0.734375f, iw, ih, 1);
    m_carrotIcon = new ScoreIcon(l + w * 0.072916666666667f, b + h * 0.578125f, iw, ih, 2);
    m_goldenCarrotIcon = new ScoreIcon(l + w * 0.072916666666667f, b + h * 0.421875f, iw, ih, 3);
    m_vialIcon = new ScoreIcon(l + w * 0.072916666666667f, b + h * 0.265625f, iw, ih, 4);
}

LevelCompletePanel::~LevelCompletePanel()
{
    delete m_replayButton;
    delete m_continueButton;
    delete m_leaderboardsButton;
    
    delete m_clockIcon;
    delete m_carrotIcon;
    delete m_goldenCarrotIcon;
    delete m_vialIcon;
}

void LevelCompletePanel::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    // TODO, slide in
}

ScoreIcon::ScoreIcon(float x, float y, float width, float height, int type) : PhysicalEntity(x, y, width, height),
m_iType(type),
m_isAnimatingIn(false)
{
    // Emptys
}

void ScoreIcon::update(float deltaTime)
{
    if (m_isAnimatingIn)
    {
        // TODO
    }
}

void ScoreIcon::animateIn()
{
    m_isAnimatingIn = true;
}

int ScoreIcon::getType()
{
    return m_iType;
}

RTTI_IMPL(LevelCompletePanel, PhysicalEntity);
RTTI_IMPL(ScoreIcon, PhysicalEntity);
