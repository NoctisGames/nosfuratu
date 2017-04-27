//
//  LevelCompletePanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/28/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "LevelCompletePanel.h"

#include "Game.h"
#include "GameButton.h"
#include "Text.h"

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
#include "EntityUtils.h"

#include <math.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

LevelCompletePanel::LevelCompletePanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_isLevelCompleted(false)
{
    float w = m_fWidth;
    float h = m_fHeight;
    
    {
        float iw = w * 0.09375f;
        float ih = h * 0.121621621621622f;
        
        m_replayButton = new GameButton(0, 0, iw, ih, GameButtonType_Replay);
        m_continueButton = new GameButton(0, 0, iw, ih, GameButtonType_ContinueToLevelSelect);
        m_leaderboardsButton = new GameButton(0, 0, iw, ih, GameButtonType_PostScore);
        
        m_clockIcon = new ScoreIcon(0, 0, iw, ih, 1);
        m_carrotIcon = new ScoreIcon(0, 0, iw, ih, 2);
        m_goldenCarrotIcon = new ScoreIcon(0, 0, iw, ih, 3);
        m_vialIcon = new ScoreIcon(0, 0, iw, ih, 4);
        m_enemyIcon = new ScoreIcon(0, 0, iw, ih, 5);
    }
    
    {
        float cw = w * 0.0390625f;
        float ch = cw * 1.171875f;
        
        m_clockValue = new Text("0:00.000", 0, 0, cw, ch, 0, 0, 0, 0);
        m_clockScore = new Text("0", 0, 0, cw, ch, 0, 0, 0, 0);
        
        m_carrotValue = new Text("0/100", 0, 0, cw, ch, 0, 0, 0, 0);
        m_carrotScore = new Text("0", 0, 0, cw, ch, 0, 0, 0, 0);
        
        m_goldenCarrotValue = new Text("0/4", 0, 0, cw, ch, 0, 0, 0, 0);
        m_goldenCarrotScore = new Text("0", 0, 0, cw, ch, 0, 0, 0, 0);
        
        m_vialValue = new Text("0/2", 0, 0, cw, ch, 0, 0, 0, 0);
        m_vialScore = new Text("0", 0, 0, cw, ch, 0, 0, 0, 0);
        
        m_enemyValue = new Text("0/0", 0, 0, cw, ch, 0, 0, 0, 0);
        m_enemyScore = new Text("0", 0, 0, cw, ch, 0, 0, 0, 0);
        
        m_finalScore = new Text("0", 0, 0, w * 0.046875f, w * 0.046875f * 1.171875f, 0, 0, 0, 0);
    }
    
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
    delete m_enemyIcon;
    
    delete m_clockValue;
    delete m_clockScore;
    
    delete m_carrotValue;
    delete m_carrotScore;
    
    delete m_goldenCarrotValue;
    delete m_goldenCarrotScore;
    
    delete m_vialValue;
    delete m_vialScore;
    
    delete m_enemyValue;
    delete m_enemyScore;
    
    delete m_finalScore;
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
        
        EntityUtils::updateAndClean(getSparkles(), deltaTime);
    }
    
    float w = m_fWidth;
    float h = m_fHeight;
    float l = m_position.getX() - w / 2;
    float b = m_position.getY() - (h / 2);
    
    float iw = w * 0.09375f;
    float ih = h * 0.121621621621622f;
    
    float time = m_fStateTime;
    if (m_fStateTime > 1)
    {
        m_clockIcon->setHidden(false);
        
        time -= 1;
        m_clockIcon->setWidth(iw * getInterpolationForIcon(time));
        m_clockIcon->setHeight(ih * getInterpolationForIcon(time));
        
        m_clockValue->setColor(0, 0, 0, getInterpolationForIcon(time));
        if (time > 0.10f)
        {
            m_clockScore->setColor(0, 0, 0, getInterpolationForIcon(time - 0.10f));
        }
    }
    
    if (m_fStateTime > 1.25f)
    {
        m_carrotIcon->setHidden(false);
        
        time -= 0.25f;
        m_carrotIcon->setWidth(iw * getInterpolationForIcon(time));
        m_carrotIcon->setHeight(ih * getInterpolationForIcon(time));
        
        m_carrotValue->setColor(0, 0, 0, getInterpolationForIcon(time));
        if (time > 0.10f)
        {
            m_carrotScore->setColor(0, 0, 0, getInterpolationForIcon(time - 0.10f));
        }
        
        if (m_game->getNumCarrotsCollected() >= 100)
        {
            // Bonus Golden Carrot Earned
            m_carrotValue->setCharColor(0, 1, 0.941176470588235f, 0.192156862745098f, m_carrotValue->getColor().alpha);
            m_carrotValue->setCharColor(1, 1, 0.941176470588235f, 0.192156862745098f, m_carrotValue->getColor().alpha);
            m_carrotValue->setCharColor(2, 1, 0.941176470588235f, 0.192156862745098f, m_carrotValue->getColor().alpha);
        }
    }
    
    if (m_fStateTime > 1.5f)
    {
        m_goldenCarrotIcon->setHidden(false);
        
        time -= 0.25f;
        m_goldenCarrotIcon->setWidth(iw * getInterpolationForIcon(time));
        m_goldenCarrotIcon->setHeight(ih * getInterpolationForIcon(time));
        
        m_goldenCarrotValue->setColor(0, 0, 0, getInterpolationForIcon(time));
        if (time > 0.10f)
        {
            m_goldenCarrotScore->setColor(0, 0, 0, getInterpolationForIcon(time - 0.10f));
        }
        
        if (m_game->getNumCarrotsCollected() >= 100)
        {
            // Bonus Golden Carrot Earned
            m_goldenCarrotValue->setCharColor(0, 1, 0.941176470588235f, 0.192156862745098f, m_goldenCarrotValue->getColor().alpha);
        }
    }
    
    if (m_fStateTime > 1.75f)
    {
        m_vialIcon->setHidden(false);
        
        time -= 0.25f;
        m_vialIcon->setWidth(iw * getInterpolationForIcon(time));
        m_vialIcon->setHeight(ih * getInterpolationForIcon(time));
        
        m_vialValue->setColor(0, 0, 0, getInterpolationForIcon(time));
        if (time > 0.10f)
        {
            m_vialScore->setColor(0, 0, 0, getInterpolationForIcon(time - 0.10f));
        }
    }
    
    if (m_fStateTime > 2.0f)
    {
        m_enemyIcon->setHidden(false);
        
        time -= 0.25f;
        m_enemyIcon->setWidth(iw * getInterpolationForIcon(time));
        m_enemyIcon->setHeight(ih * getInterpolationForIcon(time));
        
        m_enemyValue->setColor(0, 0, 0, getInterpolationForIcon(time));
        if (time > 0.10f)
        {
            m_enemyScore->setColor(0, 0, 0, getInterpolationForIcon(time - 0.10f));
        }
    }
    
    if (m_fStateTime > 2.25f)
    {
        time -= 0.25f;
        
        m_replayButton->setHidden(false);
        m_continueButton->setHidden(false);
        m_leaderboardsButton->setHidden(false);
        
        m_replayButton->setWidth(iw * getInterpolationForButton(time));
        m_replayButton->setHeight(ih * getInterpolationForButton(time));
        
        m_continueButton->setWidth(iw * getInterpolationForButton(time));
        m_continueButton->setHeight(ih * getInterpolationForButton(time));
        
        m_leaderboardsButton->setWidth(iw * getInterpolationForButton(time));
        m_leaderboardsButton->setHeight(ih * getInterpolationForButton(time));
        
        m_fSparkleStateTime += deltaTime;
        
        if (m_fSparkleStateTime > 0)
        {
            float randX = rand() / (float) RAND_MAX;
            float randY = rand() / (float) RAND_MAX;
            randX *= 0.403645833333333f * w;
            randY *= 0.1484375 * h;
            m_sparkles.push_back(new FinalScoreSparkle(l + w * 0.462239583333333f + randX, b + h * 0.032094594594595f + randY, w * 0.252604166666667f, h * 0.317567567567568f));
            
            m_fSparkleStateTime = -0.10f;
        }
        
        m_finalScore->setColor(0.12156862745098f, 0.713725490196078f, 0.96078431372549f, getInterpolationForIcon(time));
    }
    
    m_replayButton->getPosition().set(l + w * 0.010416666666667f + iw / 2, b + h - h * 0.013513513513514f - ih / 2);
    m_replayButton->updateBounds();
    m_continueButton->getPosition().set(l + w - w * 0.010416666666667f - iw / 2, b + h - h * 0.013513513513514f - ih / 2);
    m_continueButton->updateBounds();
    m_leaderboardsButton->getPosition().set(l + w * 0.92578125f, b + h * 0.091216216216216f);
    m_leaderboardsButton->updateBounds();
    
    m_clockIcon->getPosition().set(l + w * 0.072916666666667f, b + h * 0.77027027027027f);
    m_carrotIcon->getPosition().set(l + w * 0.072916666666667f, b + h * 0.635135135135135f);
    m_goldenCarrotIcon->getPosition().set(l + w * 0.072916666666667f, b + h * 0.5f);
    m_vialIcon->getPosition().set(l + w * 0.072916666666667f, b + h * 0.364864864864865f);
    m_enemyIcon->getPosition().set(l + w * 0.072916666666667f, b + h * 0.22972972972973f);
    
    m_clockValue->getPosition().set(l + w * 0.291666666666667f, b + h * 0.77027027027027f);
    m_clockScore->getPosition().set(l + w * 0.71875f, b + h * 0.77027027027027f);
    
    m_carrotValue->getPosition().set(l + w * 0.291666666666667f, b + h * 0.635135135135135f);
    m_carrotScore->getPosition().set(l + w * 0.71875f, b + h * 0.635135135135135f);
    
    m_goldenCarrotValue->getPosition().set(l + w * 0.291666666666667f, b + h * 0.5f);
    m_goldenCarrotScore->getPosition().set(l + w * 0.71875f, b + h * 0.5f);
    
    m_vialValue->getPosition().set(l + w * 0.291666666666667f, b + h * 0.364864864864865f);
    m_vialScore->getPosition().set(l + w * 0.71875f, b + h * 0.364864864864865f);
    
    m_enemyValue->getPosition().set(l + w * 0.291666666666667f, b + h * 0.22972972972973f);
    m_enemyScore->getPosition().set(l + w * 0.71875f, b + h * 0.22972972972973f);
    
    m_finalScore->getPosition().set(l + w * 0.662760416666667f, b + h * 0.094594594594595f);
}

int LevelCompletePanel::handleInput()
{
    for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); ++i)
    {
        switch ((*i)->getType())
        {
            case KeyboardEventType_SPACE:
            case KeyboardEventType_ENTER:
                if ((*i)->isUp())
                {
                    return LEVEL_COMPLETE_PANEL_RC_CONTINUE;
                }
                continue;
            case KeyboardEventType_BACK:
                if ((*i)->isUp())
                {
                    return LEVEL_COMPLETE_PANEL_RC_REPLAY;
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
            case GamePadEventType_A_BUTTON:
            case GamePadEventType_START_BUTTON:
                if ((*i)->isButtonPressed())
                {
                    return LEVEL_COMPLETE_PANEL_RC_CONTINUE;
                }
                continue;
            case GamePadEventType_BACK_BUTTON:
                if ((*i)->isButtonPressed())
                {
                    return LEVEL_COMPLETE_PANEL_RC_REPLAY;
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
    
    {
        float seconds = m_game->getStateTime();
        
        if (seconds > 599.999f)
        {
            seconds = 599.999f;
        }
        
        int minutesLeft = 0;
        while (seconds >= 60)
        {
            seconds -= 60;
            minutesLeft++;
        }
        
        std::stringstream ss;
        ss << minutesLeft << ":";
        if (seconds < 10)
        {
            ss << "0";
        }
        ss << std::fixed << std::setprecision(3) << seconds;
        std::string text = ss.str();
        
        m_clockValue->setText(text);
    }
    
    {
        std::stringstream ss;
        ss << m_game->getNumCarrotsCollected() << "/" << m_game->getNumCarrots();
        std::string text = ss.str();
        m_carrotValue->setText(text);
    }
    
    {
        std::stringstream ss;
        ss << m_game->getNumGoldenCarrotsCollected() << "/" << m_game->getNumGoldenCarrots();
        std::string text = ss.str();
        m_goldenCarrotValue->setText(text);
    }
    
    {
        std::stringstream ss;
        ss << m_game->getNumVialsCollected() << "/" << m_game->getNumVials();
        std::string text = ss.str();
        m_vialValue->setText(text);
    }
    
    {
        std::stringstream ss;
        ss << m_game->getNumEnemiesKilled() << "/" << m_game->getNumEnemies();
        std::string text = ss.str();
        m_enemyValue->setText(text);
    }
    
    {
        std::stringstream ss;
        
        // the number is converted to string with the help of stringstream
        ss << m_game->getScoreFromTime();
        std::string paddedScore;
        ss >> paddedScore;
        
        // Append zero chars
        unsigned long str_length = paddedScore.length();
        for (int i = 0; i < 6 - str_length; ++i)
        {
            paddedScore = "0" + paddedScore;
        }
        m_clockScore->setText(paddedScore);
    }
    
    {
        std::stringstream ss;
        
        // the number is converted to string with the help of stringstream
        ss << m_game->getScoreFromCarrots();
        std::string paddedScore;
        ss >> paddedScore;
        
        // Append zero chars
        unsigned long str_length = paddedScore.length();
        for (int i = 0; i < 6 - str_length; ++i)
        {
            paddedScore = "0" + paddedScore;
        }
        m_carrotScore->setText(paddedScore);
    }
    
    {
        std::stringstream ss;
        
        // the number is converted to string with the help of stringstream
        ss << m_game->getScoreFromGoldenCarrots();
        std::string paddedScore;
        ss >> paddedScore;
        
        // Append zero chars
        unsigned long str_length = paddedScore.length();
        for (int i = 0; i < 6 - str_length; ++i)
        {
            paddedScore = "0" + paddedScore;
        }
        m_goldenCarrotScore->setText(paddedScore);
    }
    
    {
        std::stringstream ss;
        
        // the number is converted to string with the help of stringstream
        ss << m_game->getScoreFromVials();
        std::string paddedScore;
        ss >> paddedScore;
        
        // Append zero chars
        unsigned long str_length = paddedScore.length();
        for (int i = 0; i < 6 - str_length; ++i)
        {
            paddedScore = "0" + paddedScore;
        }
        m_vialScore->setText(paddedScore);
    }
    
    {
        std::stringstream ss;
        
        // the number is converted to string with the help of stringstream
        ss << m_game->getScoreFromEnemies();
        std::string paddedScore;
        ss >> paddedScore;
        
        // Append zero chars
        unsigned long str_length = paddedScore.length();
        for (int i = 0; i < 6 - str_length; ++i)
        {
            paddedScore = "0" + paddedScore;
        }
        m_enemyScore->setText(paddedScore);
    }
    
    {
        std::stringstream ss;
        
        // the number is converted to string with the help of stringstream
        ss << m_game->getScore();
        std::string paddedScore;
        ss >> paddedScore;
        
        // Append zero chars
        unsigned long str_length = paddedScore.length();
        for (int i = 0; i < 6 - str_length; ++i)
        {
            paddedScore = "0" + paddedScore;
        }
        m_finalScore->setText(paddedScore);
    }
}

void LevelCompletePanel::reset()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_sparkles);
    
    m_position.setX(-CAM_WIDTH / 2);
    
    m_fStateTime = 0;
    m_fSparkleStateTime = 0;
    m_isLevelCompleted = false;
    
    m_clockIcon->setHidden(true);
    m_carrotIcon->setHidden(true);
    m_goldenCarrotIcon->setHidden(true);
    m_vialIcon->setHidden(true);
    m_enemyIcon->setHidden(true);
    
    m_replayButton->setHidden(true);
    m_continueButton->setHidden(true);
    m_leaderboardsButton->setHidden(true);
    
    m_clockValue->setColor(0, 0, 0, 0);
    m_clockScore->setColor(0, 0, 0, 0);
    
    m_carrotValue->setColor(0, 0, 0, 0);
    m_carrotScore->setColor(0, 0, 0, 0);
    
    m_goldenCarrotValue->setColor(0, 0, 0, 0);
    m_goldenCarrotScore->setColor(0, 0, 0, 0);
    
    m_vialValue->setColor(0, 0, 0, 0);
    m_vialScore->setColor(0, 0, 0, 0);
    
    m_enemyValue->setColor(0, 0, 0, 0);
    m_enemyScore->setColor(0, 0, 0, 0);
    
    m_finalScore->setColor(0, 0, 0, 0);
    
    srand (static_cast <unsigned> (time(0)));
    
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

ScoreIcon* LevelCompletePanel::getEnemyIcon()
{
    return m_enemyIcon;
}

Text* LevelCompletePanel::getClockValue()
{
    return m_clockValue;
}

Text* LevelCompletePanel::getClockScore()
{
    return m_clockScore;
}

Text* LevelCompletePanel::getCarrotValue()
{
    return m_carrotValue;
}

Text* LevelCompletePanel::getCarrotScore()
{
    return m_carrotScore;
}

Text* LevelCompletePanel::getGoldenCarrotValue()
{
    return m_goldenCarrotValue;
}

Text* LevelCompletePanel::getGoldenCarrotScore()
{
    return m_goldenCarrotScore;
}

Text* LevelCompletePanel::getVialValue()
{
    return m_vialValue;
}

Text* LevelCompletePanel::getVialScore()
{
    return m_vialScore;
}

Text* LevelCompletePanel::getEnemyValue()
{
    return m_enemyValue;
}

Text* LevelCompletePanel::getEnemyScore()
{
    return m_enemyScore;
}

Text* LevelCompletePanel::getFinalScore()
{
    return m_finalScore;
}

ScoreIcon::ScoreIcon(float x, float y, float width, float height, int type) : PhysicalEntity(x, y, width, height),
m_iType(type),
m_isHidden(false)
{
    // Empty
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

FinalScoreSparkle::FinalScoreSparkle(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

void FinalScoreSparkle::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_fStateTime > 0.5f)
    {
        m_isRequestingDeletion = true;
    }
}

RTTI_IMPL(LevelCompletePanel, PhysicalEntity);
RTTI_IMPL(ScoreIcon, PhysicalEntity);
RTTI_IMPL(FinalScoreSparkle, PhysicalEntity);
