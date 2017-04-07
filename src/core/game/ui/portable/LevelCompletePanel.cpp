//
//  LevelCompletePanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/28/17.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "LevelCompletePanel.h"

#include "Game.h"
#include "GameButton.h"

#include "NGSTDUtil.h"
#include "OverlapTester.h"
#include "ScreenEvent.h"
#include "Vector2D.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "GamePadInputManager.h"
#include "KeyboardEvent.h"
#include "GamePadEvent.h"
#include "TouchConverter.h"

#include <math.h>

LevelCompletePanel::LevelCompletePanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_isLevelCompleted(false)
{
    float w = m_fWidth;
    float h = m_fHeight;
    
    float iw = w * 0.09375f;
    float ih = h * 0.140625f;
    
    m_replayButton = new GameButton(0, 0, iw, ih, GameButtonType_Replay);
    m_continueButton = new GameButton(0, 0, iw, ih, GameButtonType_ContinueToLevelSelect);
    m_leaderboardsButton = new GameButton(0, 0, iw, ih, GameButtonType_Leaderboards);
    
    m_clockIcon = new ScoreIcon(0, 0, iw, ih, 1);
    m_carrotIcon = new ScoreIcon(0, 0, iw, ih, 2);
    m_goldenCarrotIcon = new ScoreIcon(0, 0, iw, ih, 3);
    m_vialIcon = new ScoreIcon(0, 0, iw, ih, 4);
    
    reset();
}

LevelCompletePanel::~LevelCompletePanel()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_sparkles);
    
    delete m_replayButton;
    delete m_continueButton;
    delete m_leaderboardsButton;
    
    delete m_clockIcon;
    delete m_carrotIcon;
    delete m_goldenCarrotIcon;
    delete m_vialIcon;
}

float getInterpolation(float time)
{
    /**
     * The {@code double} value that is closer than any other to
     * <i>e</i>, the base of the natural logarithms.
     */
    static const float E = 2.7182818284590452354;
    
    static const float amp = 0.2f;
    static const float freq = 2.0f;
    
    return (float) (-1 * powf(E, -time / amp) * cosf(freq * time) + 1);
}

float getInterpolationForIcon(float time)
{
    /**
     * The {@code double} value that is closer than any other to
     * <i>e</i>, the base of the natural logarithms.
     */
    static const float E = 2.7182818284590452354;
    
    static const float amp = 0.2f;
    static const float freq = 20.0f;
    
    return (float) (-1 * powf(E, -time / amp) * cosf(freq * time) + 1);
}

float getInterpolationForButton(float time)
{
    /**
     * The {@code double} value that is closer than any other to
     * <i>e</i>, the base of the natural logarithms.
     */
    static const float E = 2.7182818284590452354;
    
    static const float amp = 0.4f;
    static const float freq = 2.0f;
    
    return (float) (-1 * powf(E, -time / amp) * cosf(freq * time) + 1);
}

void LevelCompletePanel::update(float deltaTime)
{
    if (m_isLevelCompleted)
    {
        PhysicalEntity::update(deltaTime);
        
        m_position.setX(-CAM_WIDTH / 2 + CAM_WIDTH * getInterpolation(m_fStateTime));
        
        updateBounds();
    }
    
    float w = m_fWidth;
    float h = m_fHeight;
    float l = m_position.getX() - w / 2;
    float b = m_position.getY() - (h / 2);
    
    float iw = w * 0.09375f;
    float ih = h * 0.140625f;
    
    float time = m_fStateTime;
    if (m_fStateTime > 1)
    {
        if (m_clockIcon->isHidden())
        {
            m_clockIcon->animateIn();
        }
        
        time -= 1;
        m_clockIcon->setWidth(iw * getInterpolationForIcon(time));
        m_clockIcon->setHeight(ih * getInterpolationForIcon(time));
    }
    
    if (m_fStateTime > 1.75f)
    {
        if (m_carrotIcon->isHidden())
        {
            m_carrotIcon->animateIn();
        }
        
        time -= 0.75f;
        m_carrotIcon->setWidth(iw * getInterpolationForIcon(time));
        m_carrotIcon->setHeight(ih * getInterpolationForIcon(time));
    }
    
    if (m_fStateTime > 2.5f)
    {
        if (m_goldenCarrotIcon->isHidden())
        {
            m_goldenCarrotIcon->animateIn();
        }
        
        time -= 0.75f;
        m_goldenCarrotIcon->setWidth(iw * getInterpolationForIcon(time));
        m_goldenCarrotIcon->setHeight(ih * getInterpolationForIcon(time));
    }
    
    if (m_fStateTime > 3.25f)
    {
        if (m_vialIcon->isHidden())
        {
            m_vialIcon->animateIn();
        }
        
        time -= 0.75f;
        m_vialIcon->setWidth(iw * getInterpolationForIcon(time));
        m_vialIcon->setHeight(ih * getInterpolationForIcon(time));
    }
    
    if (m_fStateTime > 4)
    {
        if (m_replayButton->isHidden())
        {
            m_replayButton->animateIn();
        }
        
        if (m_continueButton->isHidden())
        {
            m_continueButton->animateIn();
        }
        
        if (m_leaderboardsButton->isHidden())
        {
            m_leaderboardsButton->animateIn();
        }
        
        m_replayButton->setWidth(iw * getInterpolationForButton(time));
        m_replayButton->setHeight(ih * getInterpolationForButton(time));
        
        m_continueButton->setWidth(iw * getInterpolationForButton(time));
        m_continueButton->setHeight(ih * getInterpolationForButton(time));
        
        m_leaderboardsButton->setWidth(iw * getInterpolationForButton(time));
        m_leaderboardsButton->setHeight(ih * getInterpolationForButton(time));
    }
    
    m_replayButton->getPosition().set(l + w * 0.010416666666667f + iw / 2, b + h - h * 0.015625f - ih / 2);
    m_replayButton->updateBounds();
    m_continueButton->getPosition().set(l + w - w * 0.010416666666667f - iw / 2, b + h - h * 0.015625f - ih / 2);
    m_continueButton->updateBounds();
    m_leaderboardsButton->getPosition().set(l + w * 0.92578125f, b + h * 0.10546875f);
    m_leaderboardsButton->updateBounds();
    
    m_clockIcon->getPosition().set(l + w * 0.072916666666667f, b + h * 0.734375f);
    m_clockIcon->updateBounds();
    m_carrotIcon->getPosition().set(l + w * 0.072916666666667f, b + h * 0.578125f);
    m_carrotIcon->updateBounds();
    m_goldenCarrotIcon->getPosition().set(l + w * 0.072916666666667f, b + h * 0.421875f);
    m_goldenCarrotIcon->updateBounds();
    m_vialIcon->getPosition().set(l + w * 0.072916666666667f, b + h * 0.265625f);
    m_vialIcon->updateBounds();
}

int LevelCompletePanel::handleInput()
{
    for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); ++i)
    {
        switch ((*i)->getType())
        {
            case KeyboardEventType_ARROW_KEY_UP:
                // TODO
                break;
            case KeyboardEventType_ARROW_KEY_DOWN:
                // TODO
                break;
            case KeyboardEventType_SPACE:
            case KeyboardEventType_ENTER:
                if ((*i)->isUp())
                {
                    return LEVEL_COMPLETE_PANEL_RC_CONTINUE;
                }
                continue;
            default:
                continue;
        }
    }
    
    for (std::vector<GamePadEvent *>::iterator i = GAME_PAD_INPUT_MANAGER->getEvents().begin(); i != GAME_PAD_INPUT_MANAGER->getEvents().end(); ++i)
    {
        switch ((*i)->getType())
        {
            case GamePadEventType_D_PAD_UP:
                // TODO
                break;
            case GamePadEventType_D_PAD_DOWN:
                // TODO
                break;
            case GamePadEventType_A_BUTTON:
            case GamePadEventType_START_BUTTON:
                if ((*i)->isButtonPressed())
                {
                    return LEVEL_COMPLETE_PANEL_RC_CONTINUE;
                }
                continue;
            default:
                continue;
        }
    }
    
    for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); ++i)
    {
        Vector2D& touchPoint = TOUCH_CONVERTER->touchToWorld(*(*i));
        
        switch ((*i)->getType())
        {
            case ScreenEventType_DOWN:
            case ScreenEventType_DRAGGED:
                continue;
            case ScreenEventType_UP:
            {
                if (OverlapTester::isPointInNGRect(touchPoint, getMainBounds()))
                {
                    if (OverlapTester::isPointInNGRect(touchPoint, m_replayButton->getMainBounds()))
                    {
                        return LEVEL_COMPLETE_PANEL_RC_REPLAY;
                    }
                    
                    if (OverlapTester::isPointInNGRect(touchPoint, m_continueButton->getMainBounds()))
                    {
                        return LEVEL_COMPLETE_PANEL_RC_CONTINUE;
                    }
                    
#ifdef NG_GAME_SERVICES
                    if (OverlapTester::isPointInNGRect(touchPoint, m_leaderboardsButton->getMainBounds()))
                    {
                        return LEVEL_COMPLETE_PANEL_RC_SUBMIT_SCORE;
                    }
#endif
                    
                    return LEVEL_COMPLETE_PANEL_RC_HANDLED;
                }
            }
                break;
        }
    }
    
    return LEVEL_COMPLETE_PANEL_RC_UNHANDLED;
}

void LevelCompletePanel::onLevelCompleted(Game* game)
{
    m_game = game;
 
    m_fStateTime = 0;
    
    m_isLevelCompleted = true;
}

void LevelCompletePanel::reset()
{
    m_position.setX(-CAM_WIDTH / 2);
    
    m_fStateTime = 0;
    
    m_isLevelCompleted = false;
    
    m_clockIcon->setHidden(true);
    m_carrotIcon->setHidden(true);
    m_goldenCarrotIcon->setHidden(true);
    m_vialIcon->setHidden(true);
    
    m_replayButton->setHidden(true);
    m_continueButton->setHidden(true);
    m_leaderboardsButton->setHidden(true);
    
    update(0);
}

std::vector<FinalScoreSparkle*>& LevelCompletePanel::getSparkles()
{
    return m_sparkles;
}

GameButton* LevelCompletePanel::getReplayButton()
{
    return m_replayButton;
}

GameButton* LevelCompletePanel::getContinueButton()
{
    return m_continueButton;
}

GameButton* LevelCompletePanel::getLeaderboardsButton()
{
    return m_leaderboardsButton;
}

ScoreIcon* LevelCompletePanel::getClockIcon()
{
    return m_clockIcon;
}

ScoreIcon* LevelCompletePanel::getCarrotIcon()
{
    return m_carrotIcon;
}

ScoreIcon* LevelCompletePanel::getGoldenCarrotIcon()
{
    return m_goldenCarrotIcon;
}

ScoreIcon* LevelCompletePanel::getVialIcon()
{
    return m_vialIcon;
}

ScoreIcon::ScoreIcon(float x, float y, float width, float height, int type) : PhysicalEntity(x, y, width, height),
m_iType(type),
m_isAnimatingIn(false),
m_isHidden(false)
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
    m_isHidden = false;
}

int ScoreIcon::getType()
{
    return m_iType;
}

bool ScoreIcon::isHidden()
{
    return m_isHidden;
}

void ScoreIcon::setHidden(bool isHidden)
{
    m_isHidden = isHidden;
}

RTTI_IMPL(LevelCompletePanel, PhysicalEntity);
RTTI_IMPL(ScoreIcon, PhysicalEntity);
RTTI_IMPL(FinalScoreSparkle, PhysicalEntity);
