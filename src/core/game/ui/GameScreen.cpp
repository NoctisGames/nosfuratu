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
#include "ScreenInputManager.h"

#define FRAME_RATE 0.01666666666667f // 60 frames per second

GameScreen::GameScreen() :
m_fFPSStateTime(0),
m_iFrames(0),
m_iFPS(0),
m_fDeltaTime(FRAME_RATE),
m_fScreenHeldTime(0),
m_isRequestingRender(false),
m_iRequestedAction(REQUESTED_ACTION_UPDATE),
m_isPaused(false),
m_isScreenHeldDown(false),
m_fFrameStateTime(0),
m_iPoolIndex(0),
m_wasPaused(false),
m_hasSwiped(false),
m_isReleasingShockwave(false),
m_needsToResumeMusicAfterTexLoad(false),
m_fShockwaveElapsedTime(0.0f),
m_fShockwaveCenterX(0.0f),
m_fShockwaveCenterY(0.0f),
m_fTimeUntilResume(0)
{
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown2 = std::unique_ptr<Vector2D>(new Vector2D());
    
    m_stateMachine = std::unique_ptr<StateMachine<GameScreen, GameScreenState>>(new StateMachine<GameScreen, GameScreenState>(this));
    m_stateMachine->setCurrentState(Title::getInstance());
}

void GameScreen::onResume()
{
    if (m_wasPaused &&
        (m_stateMachine->getCurrentState() == Title::getInstance()
         || m_stateMachine->getCurrentState() == WorldMap::getInstance()
         || m_stateMachine->getCurrentState() == OpeningCutscene::getInstance()
         || m_stateMachine->getCurrentState() == ComingSoon::getInstance()))
    {
        if (m_renderer->isLoadingAdditionalTextures())
        {
            m_needsToResumeMusicAfterTexLoad = true;
        }
        else
        {
            ASSETS->addMusicIdToPlayQueue(MUSIC_RESUME);
        }
    }
    
    if (m_stateMachine->getCurrentState() == OpeningCutscene::getInstance())
    {
        m_isPaused = false;
    }
    
    m_wasPaused = false;
    
    m_fTimeUntilResume = 0;
}

void GameScreen::onPause()
{
    if (m_stateMachine->getCurrentState()->getRTTI().derivesFrom(Level::rtti))
    {
        Level* level = (Level*) m_stateMachine->getCurrentState();
        level->stopAllSounds();
        
        m_isPaused = !level->hasCompletedLevel();
    }
    else if (m_stateMachine->getCurrentState() == OpeningCutscene::getInstance())
    {
        m_isPaused = true;
    }
    
    m_wasPaused = true;
    
    m_fTimeUntilResume = 0;
}

void GameScreen::update(float deltaTime)
{
    m_fFPSStateTime += deltaTime;
    m_iFrames++;
    if (m_fFPSStateTime >= 1.0f)
    {
        m_fFPSStateTime -= 1.0f;
        m_iFPS = m_iFrames;
        m_iFrames = 0;
    }
    
    m_fDeltaTime = FRAME_RATE;
    
    m_fFrameStateTime += deltaTime;
    while (m_fFrameStateTime >= FRAME_RATE)
    {
        m_fFrameStateTime -= FRAME_RATE;
        
        internalUpdate();
    }
    
    m_isRequestingRender = m_renderer->isLoaded();
}

void GameScreen::internalUpdate()
{
    SCREEN_INPUT_MANAGER->process();
    
    m_fTimeUntilResume -= m_fDeltaTime;
    
    if (m_isPaused)
    {
        for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); i++)
        {
            switch ((*i)->getType())
            {
                case ScreenEventType_DOWN:
                continue;
                case ScreenEventType_DRAGGED:
                continue;
                case ScreenEventType_UP:
                m_isPaused = false;
                m_fTimeUntilResume = 0.5f;
                
                if (m_stateMachine->getCurrentState()->getRTTI().derivesFrom(Level::rtti))
                {
                    ASSETS->addMusicIdToPlayQueue(MUSIC_RESUME);
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
            if (m_needsToResumeMusicAfterTexLoad)
            {
                ASSETS->addMusicIdToPlayQueue(MUSIC_RESUME);
                
                m_needsToResumeMusicAfterTexLoad = false;
            }
            
            m_stateMachine->execute();
        }
    }
}

void GameScreen::render()
{
	// Loading may be asynchronous, so make sure we are loaded before rendering
	if (m_renderer->isLoaded()
        && m_isRequestingRender)
	{
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
    short musicId = ASSETS->getFirstMusicId();
    ASSETS->eraseFirstMusicId();
    
    return musicId;
}

short GameScreen::getCurrentSoundId()
{
    short playThisSound = ASSETS->getFirstSoundId();
    ASSETS->eraseFirstSoundId();
    
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

RTTI_IMPL(GameHudCarrot, PhysicalEntity);
RTTI_IMPL_NOPARENT(GameScreen);
