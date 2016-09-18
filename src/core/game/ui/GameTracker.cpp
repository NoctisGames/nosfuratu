//
//  GameTracker.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/18/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameTracker.h"

#include "MathUtil.h"
#include "EntityUtils.h"

#include <sstream>

GameTracker * GameTracker::getInstance()
{
    static GameTracker *instance = new GameTracker();
    return instance;
}

void GameTracker::update(float deltaTime)
{
    for (std::vector<DelayText *>::iterator i = m_texts.begin(); i != m_texts.end(); )
    {
        (*i)->update(deltaTime);
        
        float r = (*i)->getColor().red;
        float g = (*i)->getColor().green;
        float b = (*i)->getColor().blue;
        float a = (*i)->getColor().alpha;
        
        a = clamp(m_scoreY - (*i)->getPosition().getY(), 1, 0);
        
        (*i)->setColor(r, g, b, a);
        
        float maxY = (*i)->getAcceleration().getY() < -0.5f && (*i)->getAcceleration().getY() > -1.5f ? m_bonusGoldenCarrotY : m_scoreY;
        
        if ((*i)->getPosition().getY() >= maxY)
        {
            delete *i;
            i = m_texts.erase(i);
        }
        else
        {
            i++;
        }
    }
}

void GameTracker::reset()
{
    EntityUtils::cleanUpVectorOfPointers(m_texts);
}

void GameTracker::onScored(int score)
{
    std::stringstream ss;
    
    // the number is converted to string with the help of stringstream
    ss << score;
    std::string scoreString;
    ss >> scoreString;
    
    float x = m_scoreX + m_fontWidth * 5;
    float y = m_scoreY - m_fontHeight;
    float w = m_fontWidth;
    float h = m_fontHeight;
    
    DelayText *text = new DelayText(0.25f, scoreString, x, y, w, h);
    text->getAcceleration().set(0, 4);
    
    m_texts.push_back(text);
}

void GameTracker::onBonusGoldenCarrotEarned()
{
    std::stringstream ss;
    
    // the number is converted to string with the help of stringstream
    ss << "+1";
    std::string plusOneGc;
    ss >> plusOneGc;
    
    float x = m_bonusGoldenCarrotX + m_fontWidth * 4;
    float y = m_bonusGoldenCarrotY - m_fontHeight;
    float w = m_fontWidth;
    float h = m_fontHeight;
    
    DelayText *text = new DelayText(1.25f, plusOneGc, x, y, w, h);
    text->getAcceleration().set(-1, 0.28f);
    
    m_texts.push_back(text);
}

void GameTracker::onTimeBonusScoreEarned(int score)
{
    std::stringstream ss;
    
    // the number is converted to string with the help of stringstream
    ss << score;
    std::string scoreString;
    ss >> scoreString;
    
    float x = m_scoreX + m_fontWidth * 12;
    float y = m_scoreY - m_fontHeight;
    float w = m_fontWidth;
    float h = m_fontHeight;
    
    DelayText *text = new DelayText(1.25f, scoreString, x, y, w, h);
    text->getAcceleration().set(-1.75, 0.28f);
    
    m_texts.push_back(text);
}

void GameTracker::config(float scoreX, float scoreY, float bonusGoldenCarrotX, float bonusGoldenCarrotY, float fontWidth, float fontHeight)
{
    m_scoreX = scoreX;
    m_scoreY = scoreY;
    m_bonusGoldenCarrotX = bonusGoldenCarrotX;
    m_bonusGoldenCarrotY = bonusGoldenCarrotY;
    m_fontWidth = fontWidth;
    m_fontHeight = fontHeight;
}

std::vector<DelayText *>& GameTracker::getTexts()
{
    return m_texts;
}

GameTracker::GameTracker() :
m_scoreX(0),
m_scoreY(0),
m_bonusGoldenCarrotX(0),
m_bonusGoldenCarrotY(0),
m_fontWidth(0),
m_fontHeight(0)
{
    // Hide Constructor for Singleton
}
