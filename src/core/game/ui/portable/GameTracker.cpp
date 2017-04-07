//
//  GameTracker.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/18/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameTracker.h"

#include "MathUtil.h"
#include "NGSTDUtil.h"

#include <sstream>

DelayText::DelayText(float delay, std::string text, float x, float y, float width, float height) : Text(text, x, y, width, height, 1, 1, 1, 1), m_fDelay(delay)
{
    // Empty
}

void DelayText::update(float deltaTime)
{
    m_fDelay -= deltaTime;
    
    if (m_fDelay < 0)
    {
        Text::update(deltaTime);
    }
}

GameTracker * GameTracker::getInstance()
{
    static GameTracker instance = GameTracker();
    return &instance;
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
        
        if ((*i)->getPosition().getY() >= m_scoreY)
        {
            delete *i;
            i = m_texts.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

void GameTracker::reset()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_texts);
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

void GameTracker::config(float scoreX, float scoreY, float fontWidth, float fontHeight)
{
    m_scoreX = scoreX;
    m_scoreY = scoreY;
    m_fontWidth = fontWidth;
    m_fontHeight = fontHeight;
    
    NGSTDUtil::cleanUpVectorOfPointers(m_texts);
}

std::vector<DelayText *>& GameTracker::getTexts()
{
    return m_texts;
}

GameTracker::GameTracker() :
m_scoreX(0),
m_scoreY(0),
m_fontWidth(0),
m_fontHeight(0)
{
    // Hide Constructor for Singleton
}

RTTI_IMPL(DelayText, Text);
RTTI_IMPL_NOPARENT(GameTracker);
