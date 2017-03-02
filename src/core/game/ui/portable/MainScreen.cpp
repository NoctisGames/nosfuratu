//
//  MainScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreen.h"

#include "MainScreenTitle.h"
#include "MainScreenWorldMap.h"
#include "MainScreenLevels.h"
#include "MainScreenOpeningCutscene.h"
#include "MainScreenComingSoon.h"
#include "Game.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "GamePadInputManager.h"
#include "KeyboardEvent.h"
#include "GamePadEvent.h"
#include "TouchConverter.h"
#include "MainRenderer.h"
#include "DeviceHelperFactory.h"
#include "IDeviceHelper.h"
#include "MainRenderer.h"
#include "BatPanel.h"
#include "Vector2D.h"

#define FRAME_RATE 0.01666666666667f // 60 frames per second

MainScreen::MainScreen() : IScreen(),
m_deviceHelper(DEVICE_HELPER_FACTORY->createDeviceHelper()),
m_renderer(new MainRenderer()),
m_touchPointDown(new Vector2D()),
m_touchPointDown2(new Vector2D()),
m_stateMachine(this),
m_fFPSStateTime(0),
m_iFrames(0),
m_iFPS(0),
m_fDeltaTime(FRAME_RATE),
m_fScreenHeldTime(0),
m_isRequestingRender(false),
m_iRequestedAction(REQUESTED_ACTION_UPDATE),
m_iNumInternalUpdates(0),
m_isPaused(false),
m_isScreenHeldDown(false),
m_fFrameStateTime(0),
m_wasPaused(false),
m_hasSwiped(false),
m_isReleasingShockwave(false),
m_needsToResumeMusicAfterTexLoad(false),
m_needsToResumeAudio(false),
m_fShockwaveElapsedTime(0.0f),
m_fShockwaveCenterX(0.0f),
m_fShockwaveCenterY(0.0f),
m_fTimeUntilResume(0)
{
    Title::getInstance()->enter(this);
    
    m_stateMachine.setCurrentState(Title::getInstance());
}

MainScreen::~MainScreen()
{
    delete m_deviceHelper;
    delete m_renderer;
    delete m_touchPointDown;
    delete m_touchPointDown2;
}

void MainScreen::createDeviceDependentResources()
{
    m_deviceHelper->createDeviceDependentResources(MAX_BATCH_SIZE);
    
    m_renderer->createDeviceDependentResources();
    
    ((MainScreenState *)m_stateMachine.getCurrentState())->initRenderer(this);
}

void MainScreen::createWindowSizeDependentResources(int screenWidth, int screenHeight, int touchScreenWidth, int touchScreenHeight)
{
    TOUCH_CONVERTER->setTouchScreenSize(touchScreenWidth, touchScreenHeight);
    TOUCH_CONVERTER->setCamSize(CAM_WIDTH, CAM_HEIGHT);
    
    m_deviceHelper->createWindowSizeDependentResources(screenWidth, screenHeight, NUM_FRAMEBUFFERS);
}

void MainScreen::releaseDeviceDependentResources()
{
    m_deviceHelper->releaseDeviceDependentResources();
    
    m_renderer->releaseDeviceDependentResources();
}

void MainScreen::onResume()
{
    if (m_wasPaused &&
        (m_stateMachine.getCurrentState() == Title::getInstance()
         || m_stateMachine.getCurrentState() == WorldMap::getInstance()
         || m_stateMachine.getCurrentState() == OpeningCutscene::getInstance()
         || m_stateMachine.getCurrentState() == ComingSoon::getInstance()))
    {
        if (m_renderer->isLoadingData())
        {
            m_needsToResumeMusicAfterTexLoad = true;
        }
        else
        {
            SOUND_MANAGER->addMusicIdToPlayQueue(MUSIC_RESUME);
        }
    }
    
    if (m_stateMachine.getCurrentState() == OpeningCutscene::getInstance())
    {
        m_isPaused = false;
    }
    
    m_wasPaused = false;
    
    m_fTimeUntilResume = 0;
}

void MainScreen::onPause()
{
    if (m_stateMachine.getCurrentState()->getRTTI().derivesFrom(Level::rtti))
    {
        Level* level = (Level*) m_stateMachine.getCurrentState();
        level->pauseAllSounds();
        
        m_isPaused = !level->hasCompletedLevel();
    }
    else if (m_stateMachine.getCurrentState() == OpeningCutscene::getInstance())
    {
        m_isPaused = true;
    }
    
    m_wasPaused = true;
    
    m_fTimeUntilResume = 0;
}

void MainScreen::update(float deltaTime)
{
#if DEBUG || _DEBUG
    m_fFPSStateTime += deltaTime;
    m_iFrames++;
    if (m_fFPSStateTime >= 1.0f)
    {
        m_fFPSStateTime -= 1.0f;
        m_iFPS = m_iFrames;
        m_iFrames = 0;
    }
#endif
    
	m_isRequestingRender = false;
    m_iNumInternalUpdates = 0;
    m_fFrameStateTime += deltaTime;
    while (m_fFrameStateTime >= FRAME_RATE)
    {
        m_fFrameStateTime -= FRAME_RATE;
        
        internalUpdate();
    }
}

void MainScreen::internalUpdate()
{
    m_iNumInternalUpdates++;
	m_fDeltaTime = FRAME_RATE;

    SCREEN_INPUT_MANAGER->process();
    KEYBOARD_INPUT_MANAGER->process();
    GAME_PAD_INPUT_MANAGER->process();
    
    m_fTimeUntilResume -= m_fDeltaTime;
    
    if (m_isPaused)
    {
        bool unpause = false;
        
        for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); i++)
        {
            switch ((*i)->getType())
            {
                case KeyboardEventType_W:
                case KeyboardEventType_BACK:
                case KeyboardEventType_SPACE:
                case KeyboardEventType_ENTER:
                    if ((*i)->isUp())
                    {
                        unpause = true;
                        break;
                    }
                    continue;
                default:
                    continue;
            }
        }
        
        for (std::vector<GamePadEvent *>::iterator i = GAME_PAD_INPUT_MANAGER->getEvents().begin(); i != GAME_PAD_INPUT_MANAGER->getEvents().end(); i++)
        {
            switch ((*i)->getType())
            {
                case GamePadEventType_A_BUTTON:
                case GamePadEventType_B_BUTTON:
                case GamePadEventType_BACK_BUTTON:
                case GamePadEventType_START_BUTTON:
                    if ((*i)->isButtonPressed())
                    {
                        unpause = true;
                        break;
                    }
                    continue;
                default:
                    continue;
            }
        }
        
        for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); i++)
        {
            switch ((*i)->getType())
            {
                case ScreenEventType_UP:
                    unpause = true;
                    break;
                default:
                    continue;
            }
        }
        
        if (unpause)
        {
            m_isPaused = false;
            m_needsToResumeAudio = true;
            m_fTimeUntilResume = 0.5f;
        }
    }
    else if (m_fTimeUntilResume < 0)
    {
        if (m_needsToResumeAudio
            && m_stateMachine.getCurrentState()->getRTTI().derivesFrom(Level::rtti))
        {
            Level* level = (Level*) m_stateMachine.getCurrentState();
            level->resumeAllSounds();
            
            SOUND_MANAGER->addMusicIdToPlayQueue(MUSIC_RESUME);
            
            m_needsToResumeAudio = false;
        }
        
        if (!m_renderer->isLoadingData())
        {
            if (m_needsToResumeMusicAfterTexLoad)
            {
                SOUND_MANAGER->addMusicIdToPlayQueue(MUSIC_RESUME);
                
                m_needsToResumeMusicAfterTexLoad = false;
            }
            
            m_stateMachine.execute();
        }
    }
}

void MainScreen::render()
{
	m_isRequestingRender = true;

	m_stateMachine.execute();
}

int MainScreen::getRequestedAction()
{
	return m_iRequestedAction;
}

void MainScreen::clearRequestedAction()
{
    m_iRequestedAction = REQUESTED_ACTION_UPDATE;
}

int MainScreen::getScore()
{
    Level* level = (Level*) m_stateMachine.getCurrentState();
    
    return level->getScore();
}

int MainScreen::getOnlineScore()
{
    Level* level = (Level*) m_stateMachine.getCurrentState();
    
    return level->getOnlineScore();
}

int MainScreen::getLevelStatsFlag()
{
    Level* level = (Level*) m_stateMachine.getCurrentState();
    
    return level->getLevelStatsFlag();
}

int MainScreen::getLevelStatsFlagForUnlockedLevel()
{
    WorldMap *worldMap = WorldMap::getInstance();
    
    return worldMap->getUnlockedLevelStatsFlag();
}

int MainScreen::getNumGoldenCarrots()
{
    Level* level = (Level*) m_stateMachine.getCurrentState();
    
    return level->getNumGoldenCarrots();
}

int MainScreen::getNumGoldenCarrotsAfterUnlockingLevel()
{
    WorldMap *worldMap = WorldMap::getInstance();
    
    return worldMap->getNumCollectedGoldenCarrots();
}

int MainScreen::getJonAbilityFlag()
{
    Level* level = (Level*) m_stateMachine.getCurrentState();
    
    return level->getJonAbilityFlag();
}

RTTI_IMPL_NOPARENT(MainScreen);
