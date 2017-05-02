//
//  MainScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "MainScreen.h"

#include "JsonFile.h"
#include "MainRenderer.h"
#include "Vector2D.h"

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
#include "BatPanel.h"
#include "ScreenEvent.h"
#include "NGAudioEngine.h"

#define FRAME_RATE 0.01666666666667f // 60 frames per second

MainScreen::MainScreen() : IScreen(),
m_saveData(new JsonFile("nosfuratu.sav", true)),
m_renderer(new MainRenderer(MAX_BATCH_SIZE)),
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
m_needsToResumeAudio(false),
m_isAuthenticated(false),
m_fShockwaveElapsedTime(0.0f),
m_fShockwaveCenterX(0.0f),
m_fShockwaveCenterY(0.0f),
m_fTimeUntilResume(0)
{
    initSounds();
    
    Title::getInstance()->enter(this);
    
    m_stateMachine.setCurrentState(Title::getInstance());
}

MainScreen::~MainScreen()
{
    delete m_saveData;
    delete m_renderer;
    delete m_touchPointDown;
    delete m_touchPointDown2;
}

void MainScreen::createDeviceDependentResources()
{
    m_renderer->createDeviceDependentResources();
    
    ((MainScreenState *)m_stateMachine.getCurrentState())->initRenderer(this);
}

void MainScreen::createWindowSizeDependentResources(int renderWidth, int renderHeight, int touchScreenWidth, int touchScreenHeight)
{
    TOUCH_CONVERTER->setTouchScreenSize(touchScreenWidth, touchScreenHeight);
    TOUCH_CONVERTER->setCamSize(CAM_WIDTH, CAM_HEIGHT);
    
    m_renderer->createWindowSizeDependentResources(renderWidth, renderHeight, NUM_FRAMEBUFFERS);
}

void MainScreen::releaseDeviceDependentResources()
{
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
        m_needsToResumeAudio = true;
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
    NG_AUDIO_ENGINE->pause();
    
    if (m_stateMachine.getCurrentState()->getRTTI().derivesFrom(Level::rtti))
    {
        Level* level = (Level*) m_stateMachine.getCurrentState();
        
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
    if (m_stateMachine.getCurrentState()->getRTTI().derivesFrom(Level::rtti))
    {
        Level* level = (Level*) m_stateMachine.getCurrentState();
        
        if (level->isDebug())
        {
            m_fFPSStateTime += deltaTime;
            m_iFrames++;
            if (m_fFPSStateTime >= 1.0f)
            {
                m_fFPSStateTime -= 1.0f;
                m_iFPS = m_iFrames;
                m_iFrames = 0;
            }
        }
    }
    
	m_isRequestingRender = false;
    m_iNumInternalUpdates = 0;
    m_fFrameStateTime += deltaTime;
    while (m_fFrameStateTime >= FRAME_RATE)
    {
        m_fFrameStateTime -= FRAME_RATE;
        
        internalUpdate();
    }
}

void MainScreen::render()
{
	if (!m_renderer->isReadyForRendering())
	{
		return;
	}
	
	m_isRequestingRender = true;

	m_stateMachine.execute();
}

void MainScreen::initSounds()
{
    NG_AUDIO_ENGINE->resetSounds();
    
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_COLLECT_CARROT, SOUND_COLLECT_CARROT, 6);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_COLLECT_GOLDEN_CARROT, SOUND_COLLECT_GOLDEN_CARROT, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_DEATH, SOUND_DEATH, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_FOOTSTEP_LEFT_GRASS, SOUND_FOOTSTEP_LEFT_GRASS, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_FOOTSTEP_RIGHT_GRASS, SOUND_FOOTSTEP_RIGHT_GRASS, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_FOOTSTEP_LEFT_CAVE, SOUND_FOOTSTEP_LEFT_CAVE, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_FOOTSTEP_RIGHT_CAVE, SOUND_FOOTSTEP_RIGHT_CAVE, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_JUMP_SPRING, SOUND_JUMP_SPRING, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_LANDING_GRASS, SOUND_LANDING_GRASS, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_LANDING_CAVE, SOUND_LANDING_CAVE, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_SNAKE_DEATH, SOUND_SNAKE_DEATH, 2);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_TRIGGER_TRANSFORM, SOUND_TRIGGER_TRANSFORM, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_CANCEL_TRANSFORM, SOUND_CANCEL_TRANSFORM, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_COMPLETE_TRANSFORM, SOUND_COMPLETE_TRANSFORM, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_JUMP_SPRING_HEAVY, SOUND_JUMP_SPRING_HEAVY, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_JON_RABBIT_JUMP, SOUND_JON_RABBIT_JUMP, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_JON_VAMPIRE_JUMP, SOUND_JON_VAMPIRE_JUMP, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_JON_RABBIT_DOUBLE_JUMP, SOUND_JON_RABBIT_DOUBLE_JUMP, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_JON_VAMPIRE_DOUBLE_JUMP, SOUND_JON_VAMPIRE_DOUBLE_JUMP, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_JON_VAMPIRE_GLIDE, SOUND_JON_VAMPIRE_GLIDE, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_MUSHROOM_BOUNCE, SOUND_MUSHROOM_BOUNCE, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_JON_BURROW_ROCKSFALL, SOUND_JON_BURROW_ROCKSFALL, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_SPARROW_FLY, SOUND_SPARROW_FLY, 4);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_SPARROW_DEATH, SOUND_SPARROW_DEATH, 2);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_TOAD_DEATH, SOUND_TOAD_DEATH, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_TOAD_EAT, SOUND_TOAD_EAT, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_SAW_GRIND, SOUND_SAW_GRIND, 4);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_FOX_BOUNCED_ON, SOUND_FOX_BOUNCED_ON, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_FOX_STRIKE, SOUND_FOX_STRIKE, 4);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_FOX_DEATH, SOUND_FOX_DEATH, 2);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_WORLD_1_LOOP_INTRO, SOUND_WORLD_1_LOOP_INTRO, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_MID_BOSS_LOOP_INTRO, SOUND_MID_BOSS_LOOP_INTRO, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_MID_BOSS_SWOOP_DOWN, SOUND_MID_BOSS_SWOOP_DOWN, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_MID_BOSS_TREE_SMASH, SOUND_MID_BOSS_TREE_SMASH, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_MID_BOSS_DEATH, SOUND_MID_BOSS_DEATH, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_SCREEN_TRANSITION, SOUND_SCREEN_TRANSITION, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_SCREEN_TRANSITION_2, SOUND_SCREEN_TRANSITION_2, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_LEVEL_COMPLETE, SOUND_LEVEL_COMPLETE, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_TITLE_LIGHTNING_1, SOUND_TITLE_LIGHTNING_1, 2);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_TITLE_LIGHTNING_2, SOUND_TITLE_LIGHTNING_2, 2);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_ABILITY_UNLOCK, SOUND_ABILITY_UNLOCK, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_BOSS_LEVEL_CLEAR, SOUND_BOSS_LEVEL_CLEAR, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_LEVEL_CLEAR, SOUND_LEVEL_CLEAR, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_LEVEL_SELECTED, SOUND_LEVEL_SELECTED, 2);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_RABBIT_DRILL, SOUND_RABBIT_DRILL, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_SNAKE_JUMP, SOUND_SNAKE_JUMP, 2);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_VAMPIRE_DASH, SOUND_VAMPIRE_DASH, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_BOSS_LEVEL_UNLOCK, SOUND_BOSS_LEVEL_UNLOCK, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_RABBIT_STOMP, SOUND_RABBIT_STOMP, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_END_BOSS_LOOP_INTRO, SOUND_END_BOSS_LOOP_INTRO, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_BUTTON_CLICK, SOUND_BUTTON_CLICK, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_LEVEL_CONFIRMED, SOUND_LEVEL_CONFIRMED, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_BAT_POOF, SOUND_BAT_POOF, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_CHAIN_SNAP, SOUND_CHAIN_SNAP, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_END_BOSS_SNAKE_MOUTH_OPEN, SOUND_END_BOSS_SNAKE_MOUTH_OPEN, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_END_BOSS_SNAKE_CHARGE_CUE, SOUND_END_BOSS_SNAKE_CHARGE_CUE, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_END_BOSS_SNAKE_CHARGE, SOUND_END_BOSS_SNAKE_CHARGE, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_END_BOSS_SNAKE_DAMAGED, SOUND_END_BOSS_SNAKE_DAMAGED, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_END_BOSS_SNAKE_DEATH, SOUND_END_BOSS_SNAKE_DEATH, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_SPIKED_BALL_ROLLING, SOUND_SPIKED_BALL_ROLLING, 2);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_ABSORB_DASH_ABILITY, SOUND_ABSORB_DASH_ABILITY, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_FOOTSTEP_LEFT_WOOD, SOUND_FOOTSTEP_LEFT_WOOD, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_FOOTSTEP_RIGHT_WOOD, SOUND_FOOTSTEP_RIGHT_WOOD, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_LANDING_WOOD, SOUND_LANDING_WOOD, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_COLLECT_BIG_CARROT, SOUND_COLLECT_BIG_CARROT, 1);
	NG_AUDIO_ENGINE->loadSound(SOUND_ID_COLLECT_VIAL, SOUND_COLLECT_VIAL, 1);
}

int MainScreen::getRequestedAction()
{
	return m_iRequestedAction;
}

void MainScreen::clearRequestedAction()
{
    m_iRequestedAction = REQUESTED_ACTION_UPDATE;
}

void MainScreen::setAuthenticated(bool isAuthenticated)
{
    m_isAuthenticated = isAuthenticated;
    
    if (m_stateMachine.getCurrentState()->getRTTI().derivesFrom(Level::rtti))
    {
        Level* level = (Level*) m_stateMachine.getCurrentState();
        
        Game& game = level->getGame();
        
        game.setAuthenticated(m_isAuthenticated);
    }
}

int MainScreen::getScore()
{
    if (m_stateMachine.getCurrentState()->getRTTI().derivesFrom(Level::rtti))
    {
        Level* level = (Level*) m_stateMachine.getCurrentState();
        
        Game& game = level->getGame();
        
        return game.getScore();
    }
    
    assert(false);
}

std::string MainScreen::getLeaderboardKey()
{
    if (m_stateMachine.getCurrentState()->getRTTI().derivesFrom(Level::rtti))
    {
        Level* level = (Level*) m_stateMachine.getCurrentState();
        
        Game& game = level->getGame();
        
        int levelIndex = game.getLevel();
        switch (levelIndex)
        {
            case 1:
                return "leaderboard_11";
            case 2:
                return "leaderboard_12";
            case 3:
                return "leaderboard_13";
            case 4:
                return "leaderboard_14";
            case 5:
                return "leaderboard_15";
            case 6:
                return "leaderboard_16";
            case 7:
                return "leaderboard_17";
            case 8:
                return "leaderboard_18";
            case 9:
                return "leaderboard_19";
            case 10:
                return "leaderboard_110";
            case 11:
                return "leaderboard_111";
            case 12:
                return "leaderboard_112";
            case 13:
                return "leaderboard_113";
            case 14:
                return "leaderboard_114";
            case 15:
                return "leaderboard_115";
            case 16:
                return "leaderboard_116";
            case 17:
                return "leaderboard_117";
            case 18:
                return "leaderboard_118";
            case 19:
                return "leaderboard_119";
            case 20:
                return "leaderboard_120";
            case 21:
                return "leaderboard_121";
            default:
                break;
        }
    }
    
    assert(false);
}

std::vector<std::string>& MainScreen::getUnlockedAchievementsKeys()
{
    if (m_stateMachine.getCurrentState()->getRTTI().derivesFrom(Level::rtti))
    {
        Level* level = (Level*) m_stateMachine.getCurrentState();
        
        Game& game = level->getGame();
        
        return game.getUnlockedAchievementsKeys();
    }
    
    assert(false);
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
        
        for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); ++i)
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
        
        for (std::vector<GamePadEvent *>::iterator i = GAME_PAD_INPUT_MANAGER->getEvents().begin(); i != GAME_PAD_INPUT_MANAGER->getEvents().end(); ++i)
        {
            switch ((*i)->getType())
            {
                case GamePadEventType_A_BUTTON:
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
        
        for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); ++i)
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
            m_fTimeUntilResume = 1;
        }
    }
    else if (m_fTimeUntilResume < 0)
    {
        if (m_needsToResumeAudio)
        {
            NG_AUDIO_ENGINE->resume();
            
            m_needsToResumeAudio = false;
        }
        
        m_stateMachine.execute();
    }
    
    NG_AUDIO_ENGINE->update();
}

RTTI_IMPL(MainScreen, IScreen);
