//
//  GameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameScreen.h"
#include "Jon.h"

GameScreen::GameScreen()
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
    handleTouchInput();
    
    m_game->update(deltaTime);
    m_renderer->updateCameraToFollowJon(m_game->getJon(), *m_game, deltaTime);
    
    if (m_game->resetGame())
    {
        init();
    }
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

void GameScreen::handleTouchInput()
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
    
    for (std::vector<TouchEvent>::iterator i = m_touchEvents.begin(); i != m_touchEvents.end(); i++)
    {
        touchToWorld((*i));
        
        switch (i->getTouchType())
        {
            case DOWN:
                m_touchPointDown->set(m_touchPoint->getX(), m_touchPoint->getY());
                continue;
            case DRAGGED:
                continue;
            case UP:
                if (m_touchPointDown->getX() + SWIPE_WIDTH <= m_touchPoint->getX())
                {
                    // Swipe Right
                    m_game->getJon().triggerRightAction();
                }
                else if (m_touchPointDown->getX() - SWIPE_WIDTH >= m_touchPoint->getX())
                {
                    // Swipe Left
                    m_game->getJon().triggerLeftAction();
                }
                else if (m_touchPointDown->getY() + SWIPE_HEIGHT <= m_touchPoint->getY())
                {
                    // Swipe Up
                    m_game->getJon().triggerUpAction();
                }
                else if (m_touchPointDown->getY() - SWIPE_HEIGHT >= m_touchPoint->getY())
                {
                    // Swipe Down
                    m_game->getJon().triggerDownAction();
                }
                else
                {
                    m_game->getJon().triggerJump();
                }
                
                m_touchPointDown->set(m_touchPoint->getX(), m_touchPoint->getY());
                
                return;
        }
    }
}