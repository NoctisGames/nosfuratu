//
//  GameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameScreen.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Renderer.h"
#include "SpriteBatcher.h"
#include "Circle.h"
#include "ResourceConstants.h"
#include "Rectangle.h"
#include "Assets.h"

GameScreen::GameScreen()
{
    init();
}

void GameScreen::init()
{
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
    
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    
    m_iScreenState = 0; // TODO
}

void GameScreen::onResume()
{
    Assets::getInstance()->setMusicId(MUSIC_PLAY_DEMO);
    
    platformResume();
}

void GameScreen::onPause()
{
    Assets::getInstance()->setMusicId(MUSIC_STOP);
    
    platformPause();
}

void GameScreen::update(float deltaTime)
{
    for (std::vector<TouchEvent>::iterator itr = m_touchEvents.begin(); itr != m_touchEvents.end(); itr++)
    {
        if(m_touchEventsPool.size() < 50)
        {
            m_touchEventsPool.push_back(*itr);
        }
    }
    
    m_touchEvents.clear();
    m_touchEvents.swap(m_touchEventsBuffer);
    m_touchEventsBuffer.clear();
}

void GameScreen::render()
{
	m_renderer->beginFrame();

	m_renderer->clearScreenWithColor(0, 0, 0, 1);
    
    m_renderer->renderBackground();
    
    m_renderer->endFrame();
}

int GameScreen::getState()
{
	return m_iScreenState;
}

void GameScreen::clearState()
{
    m_iScreenState = 0; // TODO
}

short GameScreen::getCurrentMusicId()
{
    short musicId = Assets::getInstance()->getMusicId();
    Assets::getInstance()->setMusicId(0);
    return musicId;
}

short GameScreen::getCurrentSoundId()
{
    short playThisSound = Assets::getInstance()->getFirstSoundId();
    Assets::getInstance()->eraseFirstSoundId();
    return playThisSound;
}

void GameScreen::onTouch(Touch_Type type, float raw_touch_x, float raw_touch_y)
{
    if (type == Touch_Type::DRAGGED && m_touchEventsBuffer.size() > 3)
    {
        return;
    }
    
    addTouchEventForType(type, raw_touch_x, raw_touch_y);
}

#pragma mark <private>

TouchEvent GameScreen::newTouchEvent()
{
    if(m_touchEventsPool.size() == 0)
    {
        return TouchEvent(0, 0, Touch_Type::DOWN);
    }
    else
    {
        TouchEvent touchEvent = m_touchEventsPool.back();
        m_touchEventsPool.pop_back();
        return touchEvent;
    }
}

void GameScreen::addTouchEventForType(Touch_Type touchType, float x, float y)
{
    TouchEvent touchEvent = newTouchEvent();
    touchEvent.setTouchType(touchType);
    touchEvent.setX(x);
    touchEvent.setY(y);
    
    m_touchEventsBuffer.push_back(touchEvent);
}
