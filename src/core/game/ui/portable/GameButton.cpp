//
//  GameButton.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameButton.h"

#include "NGRect.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "Assets.h"
#include "OverlapTester.h"
#include "NGAudioEngine.h"

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
            return new GameButton(1.518956801470588f / 2,
                                  CAM_HEIGHT - 1.518956801470588f / 2,
                                  1.518956801470588f,
                                  1.518956801470588f,
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
        case GameButtonType_GameController:
        case GameButtonType_Leaderboards:
        case GameButtonType_Achievements:
        case GameButtonType_SignInOut:
        case GameButtonType_ContinueToLevelSelect:
        case GameButtonType_Replay:
        case GameButtonType_PostScore:
        default:
            // These must be instantiated using the constructor, since the sizes are determined based on the panel they are in
            break;
    }
    
    assert(false);
}

GameButton::GameButton(float x, float y, float width, float height, GameButtonType type) : PhysicalEntity(x, y, width, height), m_type(type), m_color(1, 1, 1, 1), m_fOriginalWidth(width), m_fOriginalHeight(height), m_isSelected(false), m_isShrinking(false), m_isHidden(false), m_isAlt(false)
{
    // Empty
}

void GameButton::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_isSelected)
    {
        if (m_isShrinking)
        {
            m_fWidth -= deltaTime;
            m_fHeight -= deltaTime;
            
            if (m_fWidth < (m_fOriginalWidth * 0.84f)
                || m_fHeight < (m_fOriginalHeight * 0.84f))
            {
                m_isShrinking = false;
            }
        }
        else
        {
            m_fWidth += deltaTime;
            m_fHeight += deltaTime;
            
            if (m_fWidth > m_fOriginalWidth)
            {
                m_fWidth = m_fOriginalWidth;
                m_isShrinking = true;
            }
            
            if (m_fHeight > m_fOriginalHeight)
            {
                m_fHeight = m_fOriginalHeight;
                m_isShrinking = true;
            }
        }
    }
}

GameButtonType GameButton::getType()
{
    return m_type;
}

Color& GameButton::getColor()
{
    return m_color;
}

bool GameButton::handleClick(Vector2D& touchPoint)
{
    if (OverlapTester::isPointInNGRect(touchPoint, getMainBounds()))
    {
        m_isSelected = true;
        
        NG_AUDIO_ENGINE->playSound(SOUND_BUTTON_CLICK);
        
        return true;
    }
    
    return false;
}

void GameButton::select()
{
    m_fWidth = m_fOriginalWidth;
    m_fHeight = m_fOriginalHeight;
    m_isSelected = true;
    m_isShrinking = true;
}

void GameButton::deselect()
{
    m_fWidth = m_fOriginalWidth;
    m_fHeight = m_fOriginalHeight;
    m_isSelected = false;
    m_isShrinking = false;
}

void GameButton::click()
{
    m_fWidth = m_fOriginalWidth;
    m_fHeight = m_fOriginalHeight;
    m_isSelected = true;
    m_isShrinking = true;
    
    NG_AUDIO_ENGINE->playSound(SOUND_BUTTON_CLICK);
}

bool GameButton::isSelected()
{
    return m_isSelected;
}

bool GameButton::isHidden()
{
    return m_isHidden;
}

void GameButton::setHidden(bool isHidden)
{
    m_isHidden = isHidden;
}

bool GameButton::isAlt()
{
    return m_isAlt;
}

void GameButton::setAlt(bool isAlt)
{
    m_isAlt = isAlt;
}

RTTI_IMPL(GameButton, PhysicalEntity);
