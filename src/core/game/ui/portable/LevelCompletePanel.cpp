//
//  LevelCompletePanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/28/17.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "LevelCompletePanel.h"

LevelCompletePanel::LevelCompletePanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_clockIcon(new ScoreIcon(0, 0, 1, 1, 0)),
m_carrotIcon(new ScoreIcon(0, 0, 1, 1, 1)),
m_goldenCarrotIcon(new ScoreIcon(0, 0, 1, 1, 2)),
m_vialIcon(new ScoreIcon(0, 0, 1, 1, 3))
{
    // Empty
}

LevelCompletePanel::~LevelCompletePanel()
{
    delete m_clockIcon;
    delete m_carrotIcon;
    delete m_goldenCarrotIcon;
    delete m_vialIcon;
}

void LevelCompletePanel::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
}

ScoreIcon::ScoreIcon(float x, float y, float width, float height, int type) : PhysicalEntity(x, y, width, height), m_type(type)
{
    // Empty
}

void ScoreIcon::update(float deltaTime)
{
    // TODO
}

void ScoreIcon::activate()
{
    // TODO
}

RTTI_IMPL(LevelCompletePanel, PhysicalEntity);
RTTI_IMPL(ScoreIcon, PhysicalEntity);
