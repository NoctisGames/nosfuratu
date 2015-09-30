//
//  GameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameScreen.h"
#include "GameScreenStates.h"
#include "Jon.h"

GameScreen::GameScreen() : m_fDeltaTime(0)
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
    
    m_game = std::unique_ptr<Game>(new Game());
    
    m_stateMachine = std::unique_ptr<StateMachine<GameScreen>>(new StateMachine<GameScreen>(this));
    m_stateMachine->setCurrentState(GamePlay::getInstance());
}

void GameScreen::init()
{
    m_game->load();
    m_renderer->init();
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
}

void GameScreen::render()
{
    m_renderer->render(*m_game);
}

int GameScreen::getState()
{
	return 0;
}

void GameScreen::clearState()
{
	// TODO
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

Renderer& GameScreen::getRenderer()
{
    return *m_renderer;
}

Vector2D& GameScreen::getTouchPoint()
{
    return *m_touchPoint;
}

Vector2D& GameScreen::getTouchPointDown()
{
    return *m_touchPointDown;
}

Game& GameScreen::getGame()
{
    return *m_game;
}

StateMachine<GameScreen>& GameScreen::getStateMachine()
{
    return *m_stateMachine;
}

std::vector<TouchEvent>& GameScreen::getTouchEvents()
{
    return m_touchEvents;
}

std::vector<TouchEvent>& GameScreen::getTouchEventsPool()
{
    return m_touchEventsPool;
}

std::vector<TouchEvent>& GameScreen::getTouchEventsBuffer()
{
    return m_touchEventsBuffer;
}

float GameScreen::getDeltaTime()
{
    return m_fDeltaTime;
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