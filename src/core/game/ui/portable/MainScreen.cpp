//
//  MainScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreen.h"

#include "IDeviceHelper.h"
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
#include "DeviceHelperFactory.h"
#include "BatPanel.h"
#include "ScreenEvent.h"
#include "NGAudioEngine.h"
#include "SaveData.h"

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
m_needsToResumeAudio(false),
m_fShockwaveElapsedTime(0.0f),
m_fShockwaveCenterX(0.0f),
m_fShockwaveCenterY(0.0f),
m_fTimeUntilResume(0)
{
    NG_SAVE_DATA->config("nosfuratu");
    
    loadSounds();
    
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
    NG_AUDIO_ENGINE->update();
    
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
            m_fTimeUntilResume = 0.5f;
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
}

void MainScreen::loadSounds()
{
    NG_AUDIO_ENGINE->loadSound(SOUND_COLLECT_CARROT, "collect_carrot", 6);
    NG_AUDIO_ENGINE->loadSound(SOUND_COLLECT_GOLDEN_CARROT, "collect_golden_carrot", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_DEATH, "death", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_FOOTSTEP_LEFT_GRASS, "footstep_left_grass", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_FOOTSTEP_RIGHT_GRASS, "footstep_right_grass", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_FOOTSTEP_LEFT_CAVE, "footstep_left_cave", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_FOOTSTEP_RIGHT_CAVE, "footstep_right_cave", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_JUMP_SPRING, "jump_spring", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_LANDING_GRASS, "landing_grass", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_LANDING_CAVE, "landing_cave", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_SNAKE_DEATH, "snake_death", 2);
    NG_AUDIO_ENGINE->loadSound(SOUND_TRIGGER_TRANSFORM, "trigger_transform", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_CANCEL_TRANSFORM, "cancel_transform", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_COMPLETE_TRANSFORM, "complete_transform", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_JUMP_SPRING_HEAVY, "jump_spring_heavy", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_JON_RABBIT_JUMP, "jon_rabbit_jump", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_JON_VAMPIRE_JUMP, "jon_vampire_jump", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_JON_RABBIT_DOUBLE_JUMP, "jon_rabbit_double_jump", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_JON_VAMPIRE_DOUBLE_JUMP, "jon_vampire_double_jump", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_JON_VAMPIRE_GLIDE, "vampire_glide_loop", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_MUSHROOM_BOUNCE, "mushroom_bounce", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_JON_BURROW_ROCKSFALL, "jon_burrow_rocksfall", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_SPARROW_FLY, "sparrow_fly_loop", 4);
    NG_AUDIO_ENGINE->loadSound(SOUND_SPARROW_DEATH, "sparrow_die", 2);
    NG_AUDIO_ENGINE->loadSound(SOUND_TOAD_DEATH, "toad_die", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_TOAD_EAT, "toad_eat", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_SAW_GRIND, "saw_grind_loop", 4);
    NG_AUDIO_ENGINE->loadSound(SOUND_FOX_BOUNCED_ON, "fox_bounced_on", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_FOX_STRIKE, "fox_strike", 4);
    NG_AUDIO_ENGINE->loadSound(SOUND_FOX_DEATH, "fox_death", 2);
    NG_AUDIO_ENGINE->loadSound(SOUND_WORLD_1_LOOP_INTRO, "world_1_bgm_intro", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_MID_BOSS_LOOP_INTRO, "mid_boss_bgm_intro", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_MID_BOSS_SWOOP_DOWN, "mid_boss_owl_swoop", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_MID_BOSS_TREE_SMASH, "mid_boss_owl_tree_smash", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_MID_BOSS_DEATH, "mid_boss_owl_death", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_SCREEN_TRANSITION, "screen_transition", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_SCREEN_TRANSITION_2, "screen_transition_2", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_LEVEL_COMPLETE, "level_complete", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_TITLE_LIGHTNING_1, "title_lightning_1", 2);
    NG_AUDIO_ENGINE->loadSound(SOUND_TITLE_LIGHTNING_2, "title_lightning_2", 2);
    NG_AUDIO_ENGINE->loadSound(SOUND_ABILITY_UNLOCK, "ability_unlock", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_BOSS_LEVEL_CLEAR, "boss_level_clear", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_LEVEL_CLEAR, "level_clear", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_LEVEL_SELECTED, "level_selected", 2);
    NG_AUDIO_ENGINE->loadSound(SOUND_RABBIT_DRILL, "rabbit_drill", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_SNAKE_JUMP, "snake_jump", 2);
    NG_AUDIO_ENGINE->loadSound(SOUND_VAMPIRE_DASH, "vampire_dash", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_BOSS_LEVEL_UNLOCK, "boss_level_unlock", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_RABBIT_STOMP, "rabbit_stomp", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_END_BOSS_LOOP_INTRO, "final_boss_bgm_intro", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_BUTTON_CLICK, "button_click", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_LEVEL_CONFIRMED, "level_confirmed", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_BAT_POOF, "bat_poof", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_CHAIN_SNAP, "chain_snap", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_END_BOSS_SNAKE_MOUTH_OPEN, "end_boss_snake_mouth_open", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_END_BOSS_SNAKE_CHARGE_CUE, "end_boss_snake_charge_cue", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_END_BOSS_SNAKE_CHARGE, "end_boss_snake_charge", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_END_BOSS_SNAKE_DAMAGED, "end_boss_snake_damaged", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_END_BOSS_SNAKE_DEATH, "end_boss_snake_death", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_SPIKED_BALL_ROLLING, "spiked_ball_rolling_loop", 2);
    NG_AUDIO_ENGINE->loadSound(SOUND_ABSORB_DASH_ABILITY, "absorb_dash_ability", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_FOOTSTEP_LEFT_WOOD, "footstep_left_wood", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_FOOTSTEP_RIGHT_WOOD, "footstep_right_wood", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_LANDING_WOOD, "landing_wood", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_COLLECT_BIG_CARROT, "collect_big_carrot", 1);
    NG_AUDIO_ENGINE->loadSound(SOUND_COLLECT_VIAL, "collect_vial", 1);
}

RTTI_IMPL_NOPARENT(MainScreen);
