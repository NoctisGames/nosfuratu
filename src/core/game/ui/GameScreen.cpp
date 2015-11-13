//
//  GameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameScreen.h"

GameScreen::GameScreen(bool isLevelEditor) : m_fDeltaTime(0), m_isRequestingRender(false), m_iRequestedAction(REQUESTED_ACTION_UPDATE), m_iNumFramesToDiscard(0), m_isPaused(false)
{
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown2 = std::unique_ptr<Vector2D>(new Vector2D());
    
    for (int i = 0; i < 50; i++)
    {
        m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
    }
    
    m_stateMachine = std::unique_ptr<StateMachine<GameScreen>>(new StateMachine<GameScreen>(this));
    
    if (isLevelEditor)
    {
        m_stateMachine->setCurrentState(LevelEditor::getInstance());
    }
    else
    {
        m_stateMachine->setCurrentState(Title::getInstance());
    }
}

void GameScreen::onResume()
{
    // Empty
}

void GameScreen::onPause()
{
    m_isPaused = true;
}

void GameScreen::update(float deltaTime)
{
    if (m_iNumFramesToDiscard > 0)
    {
        m_iNumFramesToDiscard--;
        processTouchEvents();
    }
    else if (m_isPaused)
    {
        processTouchEvents();
        
        for (std::vector<TouchEvent>::iterator i = m_touchEvents.begin(); i != m_touchEvents.end(); i++)
        {
            switch (i->getTouchType())
            {
                case DOWN:
                    continue;
                case DRAGGED:
                    continue;
                case UP:
                    m_isPaused = false;
                    if (m_stateMachine->isInState(*GamePlay::getInstance()))
                    {
                        Assets::getInstance()->setMusicId(MUSIC_PLAY_DEMO);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    else
    {
        m_fDeltaTime = deltaTime;
        
        m_stateMachine->execute();
    }
    
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

void GameScreen::addTouchEventForType(Touch_Type type, float x, float y)
{
    TouchEvent touchEvent = newTouchEvent();
    touchEvent.setTouchType(type);
    touchEvent.setX(x);
    touchEvent.setY(y);
    
    m_touchEventsBuffer.push_back(touchEvent);
}