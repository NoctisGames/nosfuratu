//
//  GameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameScreen.h"

GameScreen::GameScreen(bool isLevelEditor) : m_fDeltaTime(0), m_isRequestingRender(false), m_iRequestedAction(REQUESTED_ACTION_UPDATE)
{
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown = std::unique_ptr<Vector2D>(new Vector2D());
    
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
    
    m_stateMachine = std::unique_ptr<StateMachine<GameScreen>>(new StateMachine<GameScreen>(this));
    
    if (isLevelEditor)
    {
        m_stateMachine->setCurrentState(LevelEditor::getInstance());
    }
    else
    {
        m_stateMachine->setCurrentState(GamePlay::getInstance());
    }
}

void GameScreen::init()
{
    m_stateMachine->getCurrentState()->enter(this);
}

void GameScreen::onResume()
{
    // TODO
}

void GameScreen::onPause()
{
    // TODO
}

void GameScreen::update(float deltaTime)
{
    m_fDeltaTime = deltaTime;
    
    m_stateMachine->execute();
    
    m_isRequestingRender = true;
}

void GameScreen::render()
{
    m_stateMachine->execute();
    
    m_isRequestingRender = false;
}

int GameScreen::getRequestedAction()
{
	return m_iRequestedAction;
}

void GameScreen::clearRequestedAction()
{
    m_iRequestedAction = REQUESTED_ACTION_UPDATE;
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

void GameScreen::processTouchEvents()
{
    for (std::vector<TouchEvent>::iterator i = m_touchEvents.begin(); i != m_touchEvents.end(); i++)
    {
        if(m_touchEventsPool.size() < 50)
        {
            m_touchEventsPool.push_back(*i);
        }
    }
    
    m_touchEvents.clear();
    m_touchEvents.swap(m_touchEventsBuffer);
    m_touchEventsBuffer.clear();
}

#pragma mark private

TouchEvent GameScreen::newTouchEvent()
{
    if (m_touchEventsPool.size() == 0)
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