//
//  GameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreen.h"

#include "GameScreenTitle.h"
#include "GameScreenWorldMap.h"
#include "GameScreenLevels.h"
#include "GameScreenOpeningCutscene.h"
#include "GameScreenComingSoon.h"
#include "Game.h"

GameScreen::GameScreen() :
m_fFPSStateTime(0),
m_iFrames(0),
m_iFPS(0),
m_fDeltaTime(0),
m_fScreenHeldTime(0),
m_isRequestingRender(false),
m_iRequestedAction(REQUESTED_ACTION_UPDATE),
m_isPaused(false),
m_isScreenHeldDown(false),
m_iPoolIndex(0),
m_wasPaused(false),
m_hasSwiped(false),
m_isReleasingShockwave(false),
m_fShockwaveElapsedTime(0.0f),
m_fShockwaveCenterX(0.0f),
m_fShockwaveCenterY(0.0f),
m_fTimeUntilResume(0)
{
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown2 = std::unique_ptr<Vector2D>(new Vector2D());
    
    for (int i = 0; i < 100; i++)
    {
		m_touchEventsPool.push_back(new TouchEvent(0, 0, Touch_Type::DOWN));
    }
    
    m_stateMachine = std::unique_ptr<StateMachine<GameScreen>>(new StateMachine<GameScreen>(this));
    m_stateMachine->setCurrentState(Title::getInstance());
}

void GameScreen::onResume()
{
    if (m_wasPaused &&
        (dynamic_cast<Title*>(m_stateMachine->getCurrentState())
         || dynamic_cast<WorldMap*>(m_stateMachine->getCurrentState())
         || dynamic_cast<OpeningCutscene*>(m_stateMachine->getCurrentState())
         || dynamic_cast<ComingSoon*>(m_stateMachine->getCurrentState())))
    {
        Assets::getInstance()->setMusicId(MUSIC_RESUME);
    }
    
    m_wasPaused = false;
    
    m_fTimeUntilResume = 0;
}

void GameScreen::onPause()
{
    if (dynamic_cast<Level*>(m_stateMachine->getCurrentState()))
    {
        Level* level = (Level*) m_stateMachine->getCurrentState();
        
        m_isPaused = !level->hasCompletedLevel();
    }
    
    m_wasPaused = true;
    
    m_fTimeUntilResume = 0;
}

void GameScreen::update(float deltaTime)
{
    m_fDeltaTime = deltaTime;
    
	m_fFPSStateTime += deltaTime;
	m_iFrames++;
	if (m_fFPSStateTime >= 1.0f)
	{
		m_fFPSStateTime -= 1.0f;
		m_iFPS = m_iFrames;
		m_iFrames = 0;
	}
    
    processTouchEvents();

    m_fTimeUntilResume -= deltaTime;
    
    if (m_isPaused)
    {
        for (std::vector<TouchEvent *>::iterator i = m_touchEvents.begin(); i != m_touchEvents.end(); i++)
        {
            switch ((*i)->getTouchType())
            {
                case DOWN:
                    continue;
                case DRAGGED:
                    continue;
                case UP:
                    m_isPaused = false;
                    m_fTimeUntilResume = 0.5f;
                    if (dynamic_cast<Level*>(m_stateMachine->getCurrentState()))
                    {
                        Assets::getInstance()->setMusicId(MUSIC_RESUME);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    else if (m_fTimeUntilResume < 0)
    {
        if (!m_renderer->isLoadingAdditionalTextures())
        {
            m_stateMachine->execute();
        }
    }
    
    m_isRequestingRender = m_renderer->isLoaded();
}

void GameScreen::render()
{
	// Loading may be asynchronous, so make sure we are loaded before rendering
	if (m_renderer->isLoaded())
	{
        m_isRequestingRender = true;
        
		m_stateMachine->execute();

		m_isRequestingRender = false;
	}
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

int GameScreen::getScore()
{
    Level* level = (Level*) m_stateMachine->getCurrentState();
    
    return level->getScore();
}

int GameScreen::getOnlineScore()
{
    Level* level = (Level*) m_stateMachine->getCurrentState();
    
    return level->getOnlineScore();
}

int GameScreen::getLevelStatsFlag()
{
    Level* level = (Level*) m_stateMachine->getCurrentState();
    
    return level->getLevelStatsFlag();
}

int GameScreen::getLevelStatsFlagForUnlockedLevel()
{
    WorldMap *worldMap = WorldMap::getInstance();
    
    return worldMap->getUnlockedLevelStatsFlag();
}

int GameScreen::getNumGoldenCarrots()
{
    Level* level = (Level*) m_stateMachine->getCurrentState();
    
    return level->getNumGoldenCarrots();
}

int GameScreen::getNumGoldenCarrotsAfterUnlockingLevel()
{
    WorldMap *worldMap = WorldMap::getInstance();
    
    return worldMap->getNumCollectedGoldenCarrots();
}

int GameScreen::getJonAbilityFlag()
{
    Level* level = (Level*) m_stateMachine->getCurrentState();
    
    return level->getJonAbilityFlag();
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
	m_touchEvents.clear();
    m_touchEvents.swap(m_touchEventsBuffer);
    m_touchEventsBuffer.clear();
}

#pragma mark private

TouchEvent* GameScreen::newTouchEvent()
{
	TouchEvent* touchEvent = m_touchEventsPool.at(m_iPoolIndex++);

	if (m_iPoolIndex > 99)
	{
		m_iPoolIndex = 0;
	}

	return touchEvent;
}

void GameScreen::addTouchEventForType(Touch_Type type, float x, float y)
{
	TouchEvent* touchEvent = newTouchEvent();
	touchEvent->setTouchType(type);
	touchEvent->setX(x);
	touchEvent->setY(y);

	m_touchEventsBuffer.push_back(touchEvent);
}
