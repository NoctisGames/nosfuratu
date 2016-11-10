//
//  GameButton.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameButton.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "Assets.h"
#include "OverlapTester.h"

#include <assert.h>

GameButton* GameButton::create(GameButtonType type)
{
    switch (type)
    {
        case GameButtonType_ToggleMusic:
            return new GameButton(0.2f + CAM_WIDTH * 0.05f / 2,
                                  0.2f + CAM_HEIGHT * 0.08888888888889f / 2,
                                  CAM_WIDTH * 0.05f,
                                  CAM_HEIGHT * 0.08888888888889f,
                                  type);
        case GameButtonType_ToggleSound:
            return new GameButton(0.2f + 0.4f + CAM_WIDTH * 0.05f,
                                  0.2f + CAM_HEIGHT * 0.08888888888889f / 2,
                                  CAM_WIDTH * 0.05f,
                                  CAM_HEIGHT * 0.08888888888889f,
                                  type);
        case GameButtonType_BackToTitle:
        case GameButtonType_BackToLevelSelect:
            return new GameButton(1.51895680147063f / 2,
                                  CAM_HEIGHT - 1.52941176470592f / 2,
                                  1.51895680147063f,
                                  1.52941176470592f,
                                  type);
        case GameButtonType_ContinueToLevelSelect:
            return new GameButton(CAM_WIDTH - 1.51895680147063f / 2,
                                  1.52941176470592f / 2,
                                  1.51895680147063f,
                                  1.52941176470592f,
                                  type);
        case GameButtonType_Leaderboards:
            return new GameButton(CAM_WIDTH - 1.51895680147063f / 2,
                                  CAM_HEIGHT - 1.52941176470592f / 2,
                                  1.51895680147063f,
                                  1.52941176470592f,
                                  type);
        case GameButtonType_ViewOpeningCutscene:
            return new GameButton(CAM_WIDTH * 0.07552941176471f,
                                  CAM_HEIGHT * 0.56862745098039f,
                                  CAM_WIDTH * 0.08823529411765f,
                                  CAM_HEIGHT * 0.10457516339869f,
                                  type);
        case GameButtonType_LevelEditor:
            return new GameButton(CAM_WIDTH - 1,
                                  CAM_HEIGHT - 0.6456424857638f / 2,
                                  2,
                                  0.6456424857638f,
                                  type);
        case GameButtonType_SpriteTester:
            return new GameButton(CAM_WIDTH - 3,
                                  CAM_HEIGHT - 0.6456424857638f / 2,
                                  2,
                                  0.6456424857638f,
                                  type);
    }
    
    assert(false);
}

GameButton::GameButton(float x, float y, float width, float height, GameButtonType type) : PhysicalEntity(x, y, width, height), m_type(type), m_color(1, 1, 1, 1), m_fOriginalWidth(width), m_fOriginalHeight(height), m_isClicked(false)
{
    // Empty
}

void GameButton::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_isClicked)
    {
        m_fWidth -= deltaTime;
        m_fHeight -= deltaTime;
        
        if (m_fWidth < (m_fOriginalWidth * 0.84f)
            || m_fHeight < (m_fOriginalHeight * 0.84f))
        {
            m_isClicked = false;
        }
    }
    else
    {
        if (m_fWidth < m_fOriginalWidth
            || m_fHeight < m_fOriginalHeight)
        {
            m_fWidth += deltaTime;
            m_fHeight += deltaTime;
            
            if (m_fWidth > m_fOriginalWidth)
            {
                m_fWidth = m_fOriginalWidth;
            }
            
            if (m_fHeight > m_fOriginalHeight)
            {
                m_fHeight = m_fOriginalHeight;
            }
        }
    }
}

GameButtonType GameButton::getType()
{
    return m_type;
}

bool GameButton::handleClick(Vector2D& touchPoint)
{
    if (OverlapTester::isPointInRectangle(touchPoint, getMainBounds()))
    {
        m_isClicked = true;
        
        ASSETS->addSoundIdToPlayQueue(SOUND_BUTTON_CLICK);
        
        return true;
    }
    
    return false;
}

RTTI_IMPL(GameButton, PhysicalEntity);
