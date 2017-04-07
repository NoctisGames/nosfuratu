//
//  MainScreenLevels.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreenLevels.h"

#include "MainScreen.h"
#include "Game.h"
#include "GameButton.h"
#include "LevelCompletePanel.h"
#include "CollectibleItem.h"
#include "MidBossOwl.h"
#include "EndBossSnake.h"
#include "Hole.h"
#include "ForegroundObject.h"
#include "BatPanel.h"

#include "FlagUtil.h"
#include "OverlapTester.h"
#include "State.h"
#include "MainScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "MainScreenLevelEditor.h"
#include "MathUtil.h"
#include "GameTracker.h"
#include "BatPanel.h"
#include "MainScreenWorldMap.h"
#include "MainScreenTransitions.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "GamePadInputManager.h"
#include "KeyboardEvent.h"
#include "GamePadEvent.h"
#include "NGAudioEngine.h"
#include "TouchConverter.h"
#include "MainRenderer.h"
#include "GameMarker.h"
#include "ScreenEvent.h"
#include "SaveDataKeys.h"
#include "SaveData.h"
#include "StringUtil.h"

UserDemoAction::UserDemoAction(DemoAction action, float stateTimeToExecuteAction) : m_action(action), m_fStateTimeToExecuteAction(stateTimeToExecuteAction)
{
    // Empty
}

/// Level ///

void Level::enter(MainScreen* ms)
{
    m_fStateTime = 0;
    m_iLevelStatsFlag = m_iBestLevelStatsFlag;
    m_iNumGoldenCarrots = m_iLastKnownNumGoldenCarrots;
    m_hasCompletedLevel = false;
    m_isDisplayingResults = false;
    m_hasExited = false;
    ms->m_isReleasingShockwave = false;
    ms->m_isScreenHeldDown = false;
    ms->m_fScreenHeldTime = 0;
    
    if (!m_game->isLoaded())
    {
        if (!m_sourceGame)
        {
            m_sourceGame = new Game();
            m_sourceGame->load(m_json);
            
            m_createdOwnSourceGame = true;
        }
        
        m_game->copy(m_sourceGame);
        
        m_game->setBestLevelStatsFlag(m_iBestLevelStatsFlag);
        m_game->setCameraBounds(&ms->m_renderer->getCameraBounds());
        
        Jon& jon = m_game->getJon();
        jon.setAbilityFlag(m_iLastKnownJonAbilityFlag);
        
        m_levelCompletePanel->reset();
    }
    
    m_batPanel->reset();
    
    configBatPanel();

    ms->initSounds();
    initRenderer(ms);
    
    ms->m_renderer->beginOpeningPanningSequence(*m_game);
    
    onEnter(ms);
    
    m_game->updateBackgrounds(ms->m_renderer->getCameraPosition(), ms->m_fDeltaTime);
    
    static float fgWidth = CAM_WIDTH / 32;
    static float fgHeight = fgWidth * 1.171875f;
    
    static float textY = CAM_HEIGHT - fgHeight;
    
    GameTracker::getInstance()->config(CAM_WIDTH * 0.5f,
                                       textY,
                                       fgWidth,
                                       fgHeight);
    
    m_playLevelSelectMusicOnExit = ms->m_stateMachine.getPreviousState() == WorldMap::getInstance();
	m_stopMusicOnExit = ms->m_stateMachine.getPreviousState() == MainScreenLevelEditor::getInstance();
    
    {
        std::string key = std::string("ng_debug");
        std::string val = NG_SAVE_DATA->findValue(key);
        int isDebug = StringUtil::stringToInt(val);
        m_isDebug = isDebug == 1;
    }
    
    {
        std::string key = std::string("ng_show_bounds");
        std::string val = NG_SAVE_DATA->findValue(key);
        int isShowingBounds = StringUtil::stringToInt(val);
        m_isShowingBounds = isShowingBounds == 1;
    }
}

void Level::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        render(ms);
    }
    else
    {
        update(ms);
    }
}

void Level::exit(MainScreen* ms)
{
    m_iNumTimesBatPanelDisplayed = 0;
    m_iNumAttemptsSinceLastAdBreak = 0;
    
    NG_AUDIO_ENGINE->stopAllSounds();
    
    if (m_playLevelSelectMusicOnExit)
    {
        NG_AUDIO_ENGINE->loadMusic("level_select_bgm");
        NG_AUDIO_ENGINE->playMusic(true);
    }
	else if (m_stopMusicOnExit)
	{
		NG_AUDIO_ENGINE->stopMusic();
	}
    
    m_fStateTime = 0;
    ms->m_isReleasingShockwave = false;
    ms->m_fShockwaveElapsedTime = 0;
    ms->m_fShockwaveCenterX = 0;
    ms->m_fShockwaveCenterY = 0;
    m_hasShownOpeningSequence = false;
    m_hasOpeningSequenceCompleted = false;
    ms->m_hasSwiped = false;
    m_showDeathTransOut = false;
    m_hasCompletedLevel = false;
    m_isDisplayingResults = false;
    m_exitLoop = false;
    m_isDemoMode = false;
    m_hasExited = true;
    
    m_iBestScore = 0;
    m_iBestLevelStatsFlag = 0;
    m_iLastKnownNumGoldenCarrots = 0;
    m_iLastKnownJonAbilityFlag = 0;
}

void Level::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_TITLE);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_CUTSCENE);
    
    ms->m_renderer->load(calcRendererTypeFromLevel(m_game->getWorld(), m_game->getLevel()));
}

void Level::setSourceGame(Game* game)
{
    m_sourceGame = game;
}

void Level::setBestStats(int bestScore, int bestLevelStatsFlag, int numGoldenCarrots, int jonAbilityFlag)
{
    m_iBestScore = bestScore;
    m_iBestLevelStatsFlag = bestLevelStatsFlag;
    m_iLastKnownNumGoldenCarrots = numGoldenCarrots;
    m_iLastKnownJonAbilityFlag = jonAbilityFlag;
}

void Level::launchInDemoMode(std::vector<UserDemoAction> userDemoActions)
{
    m_userDemoActions = userDemoActions;
    m_isDemoMode = true;
    m_iBestLevelStatsFlag = FLAG_LEVEL_COMPLETE;
    m_iLastKnownJonAbilityFlag = FLAG_ABILITY_ALL;
}

bool Level::hasCompletedLevel()
{
    return m_hasCompletedLevel;
}

Game& Level::getGame()
{
    return *m_game;
}

LevelCompletePanel* Level::getLevelCompletePanel()
{
    return m_levelCompletePanel;
}

bool Level::isDebug()
{
    return m_isDebug;
}

void Level::onEnter(MainScreen* ms)
{
    if (m_hasOpeningSequenceCompleted)
    {
        CountHissWithMina& countHissWithMina = m_game->getCountHissWithMina();
        countHissWithMina.getPosition().setX(m_game->getFarRight() + CAM_WIDTH * 2);
        
        updateCamera(ms, 0, false, true);
    }
}

void Level::beginOpeningSequence(MainScreen* ms)
{
    CountHissWithMina& countHissWithMina = m_game->getCountHissWithMina();
    countHissWithMina.beginMovement();
    
    if (ms->m_stateMachine.getPreviousState() == MainScreenLevelEditor::getInstance())
	{
		m_hasShownOpeningSequence = true;
		m_hasOpeningSequenceCompleted = true;

		updateCamera(ms, 0, false, true);

        if (!NG_AUDIO_ENGINE->isMusicDisabled())
        {
            NG_AUDIO_ENGINE->loadMusic("world_1_bgm");
            NG_AUDIO_ENGINE->playMusic(true);
        }

		return;
	}

	ms->m_renderer->beginOpeningPanningSequence(*m_game);

	m_game->updateBackgrounds(ms->m_renderer->getCameraPosition(), ms->m_fDeltaTime);

	m_hasShownOpeningSequence = true;

	if (!NG_AUDIO_ENGINE->isMusicDisabled())
	{
		NG_AUDIO_ENGINE->playSound(SOUND_WORLD_1_LOOP_INTRO);
        NG_AUDIO_ENGINE->loadMusic("world_1_bgm");
	}
}

void Level::handleOpeningSequence(MainScreen* ms)
{
    m_game->updateAndClean(ms->m_fDeltaTime, true);

	int result = ms->m_renderer->updateCameraToFollowPathToJon(*m_game, ms->m_fDeltaTime);
	m_hasOpeningSequenceCompleted = result == 3;
	m_activateRadialBlur = result == 1;
	
    Jon& jon = m_game->getJon();
    jon.setAllowedToMove(m_hasOpeningSequenceCompleted);

	if (m_hasOpeningSequenceCompleted)
	{
        CountHissWithMina& countHissWithMina = m_game->getCountHissWithMina();
		countHissWithMina.getPosition().setX(m_game->getFarRight() + CAM_WIDTH * 2);

		NG_AUDIO_ENGINE->playMusic(true);
	}

	if (result == 2)
	{
		jon.beginWarmingUp();
	}

	m_game->updateBackgrounds(ms->m_renderer->getCameraPosition(), ms->m_fDeltaTime);
}

void Level::update(MainScreen* ms)
{
    if (!m_game->isLoaded())
    {
        return;
    }
    
    Jon& jon = m_game->getJon();
    jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
    
    if (!m_hasShownOpeningSequence)
    {
		beginOpeningSequence(ms);
    }
    else if (!m_hasOpeningSequenceCompleted)
    {
		handleOpeningSequence(ms);
    }
    else
    {
        GameTracker::getInstance()->update(ms->m_fDeltaTime);
        
        m_batPanel->update(ms);
        
        if (m_batPanel->isRequestingInput())
        {
            return;
        }
        
        if (m_hasCompletedLevel)
        {
            m_levelCompletePanel->update(ms->m_fDeltaTime);
        }
        
        if (handleInput(ms))
        {
            return;
        }
        
        if (m_showDeathTransOut)
        {
            // Starting new game after death
            
            m_fStateTime += ms->m_fDeltaTime * 2;
            
            if (m_fStateTime > 2.4f)
            {
                m_fStateTime = 0;
                m_showDeathTransOut = false;
                
                NG_AUDIO_ENGINE->stopAllSounds();
            }
        }
        
        if (jon.isDead())
        {
            // Starting death transition, when screen goes black, new game begins
            
            m_fStateTime += ms->m_fDeltaTime * 2;
            
            if (m_fStateTime > 1.6f)
            {
                m_game->reset();
                
                enter(ms);
                
                updateCamera(ms, 0, false, true);
                
                m_iNumAttemptsSinceLastAdBreak++;
                if (m_iNumAttemptsSinceLastAdBreak >= 6)
                {
                    ms->m_iRequestedAction = REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD;
                    m_iNumAttemptsSinceLastAdBreak = 0;
                }
                
                m_showDeathTransOut = true;
            }
            
            return;
        }
        else
        {
            m_game->updateScore();
            
            // Is Still Actively playing the Level
            
			if (!m_hasCompletedLevel)
			{
				m_game->update(ms->m_fDeltaTime);
			}
            
            if (isInSlowMotionMode())
            {
                ms->m_fDeltaTime /= 8;
            }
            
            if (jon.isTransformingIntoVampire() || jon.isRevertingToRabbit())
            {
                if (jon.isReleasingShockwave())
                {
                    if (!ms->m_isReleasingShockwave)
                    {
                        ms->m_fShockwaveCenterX = jon.getPosition().getX();
                        ms->m_fShockwaveCenterY = jon.getPosition().getY();
                        ms->m_fShockwaveElapsedTime = 0.0f;
                        ms->m_isReleasingShockwave = true;
                    }
                }
                else
                {
                    if (!isInSlowMotionMode())
                    {
                        ms->m_fDeltaTime /= 8;
                    }
                }
            }
            
            if (ms->m_isReleasingShockwave)
            {
                ms->m_fShockwaveElapsedTime += ms->m_fDeltaTime * 1.2f;
                
                if (ms->m_fShockwaveElapsedTime > 2)
                {
                    ms->m_fShockwaveElapsedTime = 0;
                    ms->m_isReleasingShockwave = false;
                }
            }
            
            m_game->updateAndClean(ms->m_fDeltaTime);
            
            handleCollections(jon, m_game->getCollectibleItems(), ms->m_fDeltaTime);
            
            m_game->updateScore();
            
            if (ms->m_isScreenHeldDown)
            {
                ms->m_fScreenHeldTime += ms->m_fDeltaTime;
                
                if (ms->m_fScreenHeldTime > 0.4f)
                {
                    jon.triggerTransform();
                    ms->m_isScreenHeldDown = false;
                    ms->m_fShockwaveElapsedTime = 0;
                    ms->m_isReleasingShockwave = false;
                }
            }
        }

        updateCamera(ms, (jon.isAlive() && jon.getVelocity().getX() < 0) ? jon.getVelocity().getX() : 0);
        
        if (m_game->getMarkers().size() > 1)
        {
            GameMarker* nextEndLoopMarker = m_game->getMarkers().at(1);
            if (ms->m_renderer->getCameraBounds().getRight() > nextEndLoopMarker->getGridX() * GRID_CELL_SIZE)
            {
                if (m_exitLoop)
                {
                    m_game->getMarkers().erase(m_game->getMarkers().begin());
                    m_game->getMarkers().erase(m_game->getMarkers().begin());
                    
                    m_exitLoop = false;
                }
                else
                {
                    GameMarker* beginLoopMarker = m_game->getMarkers().at(0);
                    
                    int beginGridX = beginLoopMarker->getGridX();
                    int endGridX = nextEndLoopMarker->getGridX();
                    
                    EntityUtils::copyAndOffset(m_game->getMidgrounds(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getGrounds(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getPits(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getExitGrounds(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getHoles(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getForegroundObjects(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getMidBossForegroundObjects(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getEndBossForegroundObjects(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getCountHissWithMinas(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getEndBossSnakes(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getEnemies(), beginGridX, endGridX);
                    EntityUtils::offsetOnly(m_game->getCollectibleItems(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getExtraForegroundObjects(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getForegroundCoverObjects(), beginGridX, endGridX);
                    
                    EntityUtils::offsetAll(m_game->getMarkers(), beginGridX, endGridX);
                    
                    EntityUtils::setGameToEntities(m_game->getGrounds(), m_game);
                    EntityUtils::setGameToEntities(m_game->getPits(), m_game);
                    EntityUtils::setGameToEntities(m_game->getForegroundObjects(), m_game);
                    EntityUtils::setGameToEntities(m_game->getMidBossForegroundObjects(), m_game);
                    EntityUtils::setGameToEntities(m_game->getEndBossForegroundObjects(), m_game);
                    EntityUtils::setGameToEntities(m_game->getCollectibleItems(), m_game);
                    EntityUtils::setGameToEntities(m_game->getEnemies(), m_game);
                    EntityUtils::setGameToEntities(m_game->getExtraForegroundObjects(), m_game);
                    EntityUtils::setGameToEntities(m_game->getEndBossSnakes(), m_game);
                    
                    m_game->calcFarRight();
                }
            }
        }
        
        if (m_hasCompletedLevel)
        {
            m_fStateTime += ms->m_fDeltaTime / 2;
            if (m_fStateTime > 1)
            {
                m_fStateTime = 1;
                
                m_isDisplayingResults = true;
            }
            
            if (m_game->getLevel() != 10
                && m_game->getLevel() != 21)
            {
                float musicVolume = 1 - m_fStateTime;
                musicVolume = clamp(musicVolume, 1, 0);
                
                NG_AUDIO_ENGINE->setMusicVolume(musicVolume);
            }
        }
        else if (jon.getMainBounds().getLeft() > m_game->getFarRight())
        {
            // Has Cleared the Level
            
            jon.setUserActionPrevented(true);
            
            m_iLevelStatsFlag = FlagUtil::setFlag(m_iLevelStatsFlag, FLAG_LEVEL_COMPLETE);
            
            if (m_game->getNumCarrotsCollected() >= 100)
            {
                m_game->setNumGoldenCarrotsCollected(m_game->getNumGoldenCarrotsCollected() + 1);
                
                m_iLevelStatsFlag = FlagUtil::setFlag(m_iLevelStatsFlag, FLAG_BONUS_GOLDEN_CARROT_COLLECTED);
            }
            
            m_game->updateScoreFromTime();
            
            m_game->updateScore();
            
            int finalScore = m_game->getScore();
            if (finalScore < m_iBestScore)
            {
                finalScore = m_iBestScore;
            }
            
            if (FlagUtil::isFlagSet(m_iLevelStatsFlag, FLAG_FIRST_GOLDEN_CARROT_COLLECTED)
                && !FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_FIRST_GOLDEN_CARROT_COLLECTED))
            {
                m_iNumGoldenCarrots++;
            }
            
            if (FlagUtil::isFlagSet(m_iLevelStatsFlag, FLAG_SECOND_GOLDEN_CARROT_COLLECTED)
                && !FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_SECOND_GOLDEN_CARROT_COLLECTED))
            {
                m_iNumGoldenCarrots++;
            }
            
            if (FlagUtil::isFlagSet(m_iLevelStatsFlag, FLAG_THIRD_GOLDEN_CARROT_COLLECTED)
                && !FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_THIRD_GOLDEN_CARROT_COLLECTED))
            {
                m_iNumGoldenCarrots++;
            }
            
            if (FlagUtil::isFlagSet(m_iLevelStatsFlag, FLAG_BONUS_GOLDEN_CARROT_COLLECTED)
                && !FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_BONUS_GOLDEN_CARROT_COLLECTED))
            {
                m_iNumGoldenCarrots++;
            }
            
            {
                std::string key = getKeyForLevelScore(m_game->getWorld(), m_game->getLevel());
                std::string val = StringUtil::toString(finalScore);
                NG_SAVE_DATA->getKeyValues()[key] = val;
            }
            
            {
                std::string key = getKeyForLevelStats(m_game->getWorld(), m_game->getLevel());
                std::string val = StringUtil::toString(m_iLevelStatsFlag);
                NG_SAVE_DATA->getKeyValues()[key] = val;
            }
            
            {
                std::string key = getKeyForNumGoldenCarrots();
                std::string val = StringUtil::toString(m_iNumGoldenCarrots);
                NG_SAVE_DATA->getKeyValues()[key] = val;
            }
            
            {
                std::string key = getKeyForJonUnlockedAbilitiesFlag();
                std::string val = StringUtil::toString(m_game->getJon().getAbilityFlag());
                NG_SAVE_DATA->getKeyValues()[key] = val;
            }
            
            NG_SAVE_DATA->save();
  
            m_fStateTime = 0;
			ms->m_renderer->stopCamera();
            m_hasCompletedLevel = true;
            
            m_levelCompletePanel->onLevelCompleted(m_game);
            
            NG_AUDIO_ENGINE->stopAllSounds();
            NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_COMPLETE);
        }
        
        m_game->updateBackgrounds(ms->m_renderer->getCameraPosition(), ms->m_fDeltaTime);
    }
}

void Level::render(MainScreen* ms)
{
    Jon& jon = m_game->getJon();
    
    ms->m_renderer->beginFrame();
    
    ms->m_renderer->renderWorld(*m_game);
    
    if (ms->m_isReleasingShockwave)
    {
        ms->m_renderer->renderToSecondFramebufferWithShockwave(ms->m_fShockwaveCenterX, ms->m_fShockwaveCenterY, ms->m_fShockwaveElapsedTime, jon.isTransformingIntoVampire() || jon.isVampire());
    }
    else
    {
        ms->m_renderer->renderToSecondFramebuffer(*m_game);
    }
    
    ms->m_renderer->renderJonAndExtraForegroundObjects(*m_game);
    
    additionalRenderingBeforeHud(ms);
    
    if (m_hasCompletedLevel)
    {
        ms->m_renderer->renderBlackOverlay(m_fStateTime);
    }
    
    ms->m_renderer->renderBatPanel(*m_batPanel);
    
    if (ms->m_isPaused)
    {
        ms->m_renderer->renderToThirdFramebufferWithObfuscation();
    }
    
    if (m_hasOpeningSequenceCompleted
        && !m_hasCompletedLevel)
    {
        ms->m_renderer->renderHud(*m_game, m_hasCompletedLevel ? nullptr : m_backButton);
    }

    if (m_isDebug)
    {
        ms->m_renderer->renderDebugInfo(*m_game, ms->m_iFPS);
    }
    
    if (m_isShowingBounds)
    {
        ms->m_renderer->renderBounds(*m_game);
    }
    
    if (m_hasCompletedLevel)
    {
        ms->m_renderer->renderLevelCompletePanel(m_levelCompletePanel);
    }
    
    if (ms->m_isPaused)
    {
		m_fStateTime = 0;
		m_showDeathTransOut = false;

        ms->m_renderer->renderResumeButtonOverlay();
    }
    
    if (jon.isDead())
    {
        ms->m_renderer->renderToThirdFramebufferWithTransDeathIn(m_fStateTime);
    }
    else if (m_showDeathTransOut)
    {
        ms->m_renderer->renderToThirdFramebufferWithTransDeathOut(m_fStateTime);
    }
    else if (m_activateRadialBlur)
    {
        ms->m_renderer->renderToThirdFramebufferWithRadialBlur();
    }
    
    ms->m_renderer->renderToScreen();
    
    ms->m_renderer->endFrame();
}

void Level::updateCamera(MainScreen* ms, float paddingX, bool ignoreY, bool instant)
{
    ms->m_renderer->updateCameraToFollowJon(*m_game, m_batPanel, ms->m_fDeltaTime, paddingX, false, ignoreY, instant);
}

void Level::additionalRenderingBeforeHud(MainScreen* ms)
{
    // Empty, override in subclass
}

bool Level::isInSlowMotionMode()
{
    return false;
}

void Level::configBatPanel()
{
    if (!FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_LEVEL_COMPLETE))
    {
        if (m_iNumTimesBatPanelDisplayed < 2)
        {
            m_batPanel->config(m_game, m_game->getWorld(), m_game->getLevel());
        }
        else
        {
            m_batPanel->configWithoutUi(m_game, m_game->getWorld(), m_game->getLevel());
        }
        
        m_iNumTimesBatPanelDisplayed++;
    }
}

bool Level::handleInput(MainScreen* ms)
{
    if (m_isDemoMode)
    {
        bool endDemo = false;
        
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
                        endDemo = true;
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
                        endDemo = true;
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
                case ScreenEventType_DOWN:
                case ScreenEventType_DRAGGED:
                    continue;
                case ScreenEventType_UP:
                    endDemo = true;
                    break;
            }
        }
        
        if (m_userDemoActions.size() > 0)
        {
            UserDemoAction userDemoAction = m_userDemoActions.at(0);
            if (m_game->getStateTime() >= userDemoAction.m_fStateTimeToExecuteAction)
            {
                Jon& jon = m_game->getJon();
                
                switch (userDemoAction.m_action)
                {
                    case DemoAction_Exit:
                        endDemo = true;
                        break;
                    case DemoAction_TriggerJump:
                        jon.triggerJump();
                        break;
                    case DemoAction_TriggerTransform:
                        jon.triggerTransform();
                        break;
                    case DemoAction_TriggerRight:
                        jon.triggerRightAction();
                        break;
                    case DemoAction_TriggerUp:
                        jon.triggerUpAction();
                        break;
                    case DemoAction_TriggerLeft:
                        jon.triggerLeftAction();
                        break;
                    case DemoAction_TriggerDown:
                        jon.triggerDownAction();
                        break;
                    default:
                        break;
                }
                
                m_userDemoActions.erase(m_userDemoActions.begin());
            }
        }
        
        if (endDemo)
        {
            ms->m_stateMachine.revertToPreviousState();
            
            return true;
        }
        
        return false;
    }
    
    if (m_isDisplayingResults)
    {
        bool goToNextState = false;
        
        int result = m_levelCompletePanel->handleInput();
        
        if (result == LEVEL_COMPLETE_PANEL_RC_CONTINUE)
        {
            goToNextState = true;
        }
        else if (result == LEVEL_COMPLETE_PANEL_RC_REPLAY)
        {
            NG_AUDIO_ENGINE->stopAllSounds();
            
            m_game->reset();
            
            m_hasCompletedLevel = false;
            m_hasShownOpeningSequence = false;
            m_hasOpeningSequenceCompleted = false;
            m_isDisplayingResults = false;
            m_exitLoop = false;
            
            enter(ms);
            
            updateCamera(ms, 0, false, true);
            
            m_iNumAttemptsSinceLastAdBreak++;
            if (m_iNumAttemptsSinceLastAdBreak >= 6)
            {
                ms->m_iRequestedAction = REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD;
                m_iNumAttemptsSinceLastAdBreak = 0;
            }
        }
        else if (result == LEVEL_COMPLETE_PANEL_RC_REPLAY)
        {
            ms->m_iRequestedAction = REQUESTED_ACTION_SUBMIT_SCORE_TO_LEADERBOARD;
        }
        
        if (goToNextState)
        {
            if (m_game->getWorld() == 1
                && m_game->getLevel() == 21)
            {
                m_playLevelSelectMusicOnExit = false;
                LevelToComingSoon::getInstance()->setLevelComingFrom(this);
                ms->m_stateMachine.changeState(LevelToComingSoon::getInstance());
            }
            else
            {
                ms->m_stateMachine.revertToPreviousState();
            }
        }
        
        return true;
    }
    
    Jon& jon = m_game->getJon();
    bool isJonAlive = jon.isAlive();
    if (!isJonAlive)
    {
        return false;
    }
    
    for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); ++i)
    {
        switch ((*i)->getType())
        {
            case KeyboardEventType_ARROW_KEY_RIGHT:
            {
                if ((*i)->isUp())
                {
                    jon.triggerRightAction();
                    return false;
                }
            }
                continue;
            case KeyboardEventType_ARROW_KEY_UP:
            {
                if ((*i)->isUp())
                {
                    jon.triggerUpAction();
                    return false;
                }
            }
                continue;
            case KeyboardEventType_ARROW_KEY_LEFT:
            {
                if ((*i)->isUp())
                {
                    jon.triggerLeftAction();
                    return false;
                }
            }
                continue;
            case KeyboardEventType_ARROW_KEY_DOWN:
            {
                if ((*i)->isUp())
                {
                    jon.triggerDownAction();
                    return false;
                }
            }
                continue;
            case KeyboardEventType_W:
            {
                if ((*i)->isUp())
                {
                    ms->m_fScreenHeldTime = 0.0f;
                    
                    jon.triggerJump();
                    
                    return false;
                }
            }
                continue;
            case KeyboardEventType_S:
            {
                if ((*i)->isUp())
                {
                    if (ms->m_fScreenHeldTime > 0.4f)
                    {
                        jon.triggerCancelTransform();
                    }
                    
                    ms->m_isScreenHeldDown = false;
                    ms->m_fScreenHeldTime = 0;
                }
                else
                {
                    ms->m_isScreenHeldDown = true;
                    ms->m_fScreenHeldTime = 0.0f;
                }
            }
                return false;
            case KeyboardEventType_BACK:
                if (!m_hasCompletedLevel
                    && (*i)->isUp())
                {
                    m_exitLoop = true;
                    
                    ms->m_renderer->stopCamera();
                    
                    ms->m_stateMachine.revertToPreviousState();
                    
                    return true;
                }
                continue;
            default:
                break;
        }
    }
    
    for (std::vector<GamePadEvent *>::iterator i = GAME_PAD_INPUT_MANAGER->getEvents().begin(); i != GAME_PAD_INPUT_MANAGER->getEvents().end(); ++i)
    {
        switch ((*i)->getType())
        {
            case GamePadEventType_STICK_LEFT:
            case GamePadEventType_STICK_RIGHT:
            {
                float x = (*i)->getX();
                float y = (*i)->getY();
                
                if (x > 0.8f)
                {
                    jon.triggerRightAction();
                    return false;
                }
                else if (x < -0.8f)
                {
                    jon.triggerLeftAction();
                    return false;
                }
                
                if (y > 0.8f)
                {
                    jon.triggerUpAction();
                    return false;
                }
                else if (y < -0.8f)
                {
                    jon.triggerDownAction();
                    return false;
                }
            }
                continue;
            case GamePadEventType_D_PAD_RIGHT:
            {
                if ((*i)->isButtonPressed())
                {
                    jon.triggerRightAction();
                    return false;
                }
            }
                continue;
            case GamePadEventType_D_PAD_UP:
            {
                if ((*i)->isButtonPressed())
                {
                    jon.triggerUpAction();
                    return false;
                }
            }
                continue;
            case GamePadEventType_D_PAD_LEFT:
            {
                if ((*i)->isButtonPressed())
                {
                    jon.triggerLeftAction();
                    return false;
                }
            }
                continue;
            case GamePadEventType_D_PAD_DOWN:
            {
                if ((*i)->isButtonPressed())
                {
                    jon.triggerDownAction();
                    return false;
                }
            }
                continue;
            case GamePadEventType_A_BUTTON:
            {
                if ((*i)->isButtonPressed())
                {
                    ms->m_fScreenHeldTime = 0.0f;
                    
                    jon.triggerJump();
                    
                    return false;
                }
            }
                continue;
            case GamePadEventType_X_BUTTON:
            {
                if ((*i)->isButtonPressed())
                {
                    ms->m_isScreenHeldDown = true;
                    ms->m_fScreenHeldTime = 0.0f;
                }
                else
                {
                    if (ms->m_fScreenHeldTime > 0.4f)
                    {
                        jon.triggerCancelTransform();
                    }
                    
                    ms->m_isScreenHeldDown = false;
                    ms->m_fScreenHeldTime = 0;
                }
            }
                return false;
            case GamePadEventType_BACK_BUTTON:
                if (!m_hasCompletedLevel
                    && (*i)->isButtonPressed())
                {
                    m_exitLoop = true;
                    
                    ms->m_renderer->stopCamera();
                    
                    ms->m_stateMachine.revertToPreviousState();
                    
                    return true;
                }
                continue;
            default:
                break;
        }
    }
    
    for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); ++i)
    {
        Vector2D& touchPoint = TOUCH_CONVERTER->touchToWorld(*(*i));
        
        switch ((*i)->getType())
        {
            case ScreenEventType_DOWN:
            {
                ms->m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
                ms->m_isScreenHeldDown = true;
                ms->m_fScreenHeldTime = 0.0f;
            }
                continue;
            case ScreenEventType_DRAGGED:
                if (!ms->m_hasSwiped)
                {
                    if (touchPoint.getX() >= (ms->m_touchPointDown->getX() + SWIPE_WIDTH))
                    {
                        // Swipe Right
                        jon.triggerRightAction();
                        ms->m_hasSwiped = true;
                    }
                    else if (touchPoint.getX() <= (ms->m_touchPointDown->getX() - SWIPE_WIDTH))
                    {
                        // Swipe Left
                        jon.triggerLeftAction();
                        ms->m_hasSwiped = true;
                    }
                    else if (touchPoint.getY() >= (ms->m_touchPointDown->getY() + SWIPE_HEIGHT))
                    {
                        // Swipe Up
                        jon.triggerUpAction();
                        ms->m_hasSwiped = true;
                    }
                    else if (touchPoint.getY() <= (ms->m_touchPointDown->getY() - SWIPE_HEIGHT))
                    {
                        // Swipe Down
                        jon.triggerDownAction();
                        ms->m_hasSwiped = true;
                    }
                    
                    if (ms->m_hasSwiped)
                    {
                        ms->m_isScreenHeldDown = false;
                        ms->m_fScreenHeldTime = 0.0f;
                    }
                }
                continue;
            case ScreenEventType_UP:
                if (!m_hasCompletedLevel
                    && m_backButton->handleClick(touchPoint))
                {
//                    m_game->setNumCarrotsCollected(112);
//                    m_game->setNumGoldenCarrotsCollected(4);
//                    m_game->setNumVialsCollected(1);
//                    
//                    m_game->updateScoreFromTime();
//                    
//                    m_game->updateScore();
//                    
//                    m_hasCompletedLevel = true;
//                    m_isDisplayingResults = true;
//                    
//                    m_levelCompletePanel->onLevelCompleted(m_game);
//                    
//                    return false;
                    
                    m_exitLoop = true;
                    
                    ms->m_renderer->stopCamera();
                    
                    ms->m_stateMachine.revertToPreviousState();
                    
                    return true;
                }
                
                if (!ms->m_hasSwiped && ms->m_fScreenHeldTime < 0.4f)
                {
                    jon.triggerJump();
                }
                
                if (ms->m_fScreenHeldTime > 0.4f)
                {
                    jon.triggerCancelTransform();
                }
                
                ms->m_isScreenHeldDown = false;
                ms->m_fScreenHeldTime = 0;
                
                ms->m_hasSwiped = false;
                
                ms->m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
                
                break;
        }
    }
    
    return false;
}

void Level::handleCollections(PhysicalEntity& entity, std::vector<CollectibleItem *>& items, float deltaTime)
{
    for (std::vector<CollectibleItem *>::iterator i = items.begin(); i != items.end(); ++i)
    {
        if ((*i)->isCollected())
        {
            continue;
        }
        
        if (OverlapTester::doNGRectsOverlap(entity.getMainBounds(), (*i)->getMainBounds()))
        {
            (*i)->collect();
            
            if ((*i)->getType() == CollectibleItemType_GoldenCarrot)
            {
                GoldenCarrot* gc = reinterpret_cast<GoldenCarrot *>((*i));
                
                m_game->setNumGoldenCarrotsCollected(m_game->getNumGoldenCarrotsCollected() + 1);
                
                GameTracker::getInstance()->onScored(SCORE_GOLDEN_CARROT);
                
                switch (gc->getIndex())
                {
                    case 0:
                        m_iLevelStatsFlag = FlagUtil::setFlag(m_iLevelStatsFlag, FLAG_FIRST_GOLDEN_CARROT_COLLECTED);
                        break;
                    case 1:
                        m_iLevelStatsFlag = FlagUtil::setFlag(m_iLevelStatsFlag, FLAG_SECOND_GOLDEN_CARROT_COLLECTED);
                        break;
                    case 2:
                        m_iLevelStatsFlag = FlagUtil::setFlag(m_iLevelStatsFlag, FLAG_THIRD_GOLDEN_CARROT_COLLECTED);
                        break;
                    default:
                        break;
                }
            }
            else if ((*i)->getType() == CollectibleItemType_BigCarrot)
            {
                m_game->setNumCarrotsCollected(m_game->getNumCarrotsCollected() + 10);
                
                GameTracker::getInstance()->onScored(SCORE_BIG_CARROT);
            }
            else if ((*i)->getType() == CollectibleItemType_Vial)
            {
                m_game->setNumVialsCollected(m_game->getNumVialsCollected() + 1);
                
                GameTracker::getInstance()->onScored(SCORE_VIAL);
            }
            else
            {
                m_game->setNumCarrotsCollected(m_game->getNumCarrotsCollected() + 1);
                
                GameTracker::getInstance()->onScored(SCORE_CARROT);
            }
        }
    }
}

Level::Level(const char* json) :
m_json(json),
m_game(new Game()),
m_sourceGame(nullptr),
m_batPanel(new BatPanel()),
m_backButton(GameButton::create(GameButtonType_BackToLevelSelect)),
m_levelCompletePanel(new LevelCompletePanel()),
m_fStateTime(0.0f),
m_iLevelStatsFlag(0),
m_iNumGoldenCarrots(0),
m_iNumTimesBatPanelDisplayed(0),
m_iNumAttemptsSinceLastAdBreak(0),
m_hasShownOpeningSequence(false),
m_hasOpeningSequenceCompleted(false),
m_activateRadialBlur(false),
m_showDeathTransOut(false),
m_exitLoop(false),
m_hasCompletedLevel(false),
m_isDisplayingResults(false),
m_createdOwnSourceGame(false),
m_iBestScore(0),
m_iBestLevelStatsFlag(0),
m_iLastKnownNumGoldenCarrots(0),
m_iLastKnownJonAbilityFlag(0),
m_playLevelSelectMusicOnExit(false),
m_stopMusicOnExit(false),
m_isDemoMode(false),
m_isDebug(false),
m_isShowingBounds(false),
m_hasExited(false)
{
    // Empty
}

Level::~Level()
{
    delete m_batPanel;
    delete m_backButton;
    delete m_levelCompletePanel;
    
    delete m_game;
    m_game = nullptr;
    
    if (m_createdOwnSourceGame)
    {
        delete m_sourceGame;
        m_sourceGame = nullptr;
    }
}

Level* LevelUtil::getInstanceForWorldAndLevel(int world, int level)
{
    switch (world)
    {
        case 1:
        {
            switch (level)
            {
                case 1:
                {
                    if (Chapter1Level1::getInstance() != nullptr)
                    {
                        Chapter1Level1::destroy();
                    }
                    
                    Chapter1Level1::create();
                    
                    return Chapter1Level1::getInstance();
                }
                case 2:
                {
                    if (Chapter1Level2::getInstance() != nullptr)
                    {
                        Chapter1Level2::destroy();
                    }
                    
                    Chapter1Level2::create();
                    
                    return Chapter1Level2::getInstance();
                }
                case 3:
                {
                    if (Chapter1Level3::getInstance() != nullptr)
                    {
                        Chapter1Level3::destroy();
                    }
                    
                    Chapter1Level3::create();
                    
                    return Chapter1Level3::getInstance();
                }
                case 4:
                {
                    if (Chapter1Level4::getInstance() != nullptr)
                    {
                        Chapter1Level4::destroy();
                    }
                    
                    Chapter1Level4::create();
                    
                    return Chapter1Level4::getInstance();
                }
                case 5:
                {
                    if (Chapter1Level5::getInstance() != nullptr)
                    {
                        Chapter1Level5::destroy();
                    }
                    
                    Chapter1Level5::create();
                    
                    return Chapter1Level5::getInstance();
                }
                case 6:
                {
                    if (Chapter1Level6::getInstance() != nullptr)
                    {
                        Chapter1Level6::destroy();
                    }
                    
                    Chapter1Level6::create();
                    
                    return Chapter1Level6::getInstance();
                }
                case 7:
                {
                    if (Chapter1Level7::getInstance() != nullptr)
                    {
                        Chapter1Level7::destroy();
                    }
                    
                    Chapter1Level7::create();
                    
                    return Chapter1Level7::getInstance();
                }
                case 8:
                {
                    if (Chapter1Level8::getInstance() != nullptr)
                    {
                        Chapter1Level8::destroy();
                    }
                    
                    Chapter1Level8::create();
                    
                    return Chapter1Level8::getInstance();
                }
                case 9:
                {
                    if (Chapter1Level9::getInstance() != nullptr)
                    {
                        Chapter1Level9::destroy();
                    }
                    
                    Chapter1Level9::create();
                    
                    return Chapter1Level9::getInstance();
                }
                case 10:
                {
                    if (Chapter1Level10::getInstance() != nullptr)
                    {
                        Chapter1Level10::destroy();
                    }
                    
                    Chapter1Level10::create();
                    
                    return Chapter1Level10::getInstance();
                }
                case 11:
                {
                    if (Chapter1Level11::getInstance() != nullptr)
                    {
                        Chapter1Level11::destroy();
                    }
                    
                    Chapter1Level11::create();
                    
                    return Chapter1Level11::getInstance();
                }
                case 12:
                {
                    if (Chapter1Level12::getInstance() != nullptr)
                    {
                        Chapter1Level12::destroy();
                    }
                    
                    Chapter1Level12::create();
                    
                    return Chapter1Level12::getInstance();
                }
                case 13:
                {
                    if (Chapter1Level13::getInstance() != nullptr)
                    {
                        Chapter1Level13::destroy();
                    }
                    
                    Chapter1Level13::create();
                    
                    return Chapter1Level13::getInstance();
                }
                case 14:
                {
                    if (Chapter1Level14::getInstance() != nullptr)
                    {
                        Chapter1Level14::destroy();
                    }
                    
                    Chapter1Level14::create();
                    
                    return Chapter1Level14::getInstance();
                }
                case 15:
                {
                    if (Chapter1Level15::getInstance() != nullptr)
                    {
                        Chapter1Level15::destroy();
                    }
                    
                    Chapter1Level15::create();
                    
                    return Chapter1Level15::getInstance();
                }
                case 16:
                {
                    if (Chapter1Level16::getInstance() != nullptr)
                    {
                        Chapter1Level16::destroy();
                    }
                    
                    Chapter1Level16::create();
                    
                    return Chapter1Level16::getInstance();
                }
                case 17:
                {
                    if (Chapter1Level17::getInstance() != nullptr)
                    {
                        Chapter1Level17::destroy();
                    }
                    
                    Chapter1Level17::create();
                    
                    return Chapter1Level17::getInstance();
                }
                case 18:
                {
                    if (Chapter1Level18::getInstance() != nullptr)
                    {
                        Chapter1Level18::destroy();
                    }
                    
                    Chapter1Level18::create();
                    
                    return Chapter1Level18::getInstance();
                }
                case 19:
                {
                    if (Chapter1Level19::getInstance() != nullptr)
                    {
                        Chapter1Level19::destroy();
                    }
                    
                    Chapter1Level19::create();
                    
                    return Chapter1Level19::getInstance();
                }
                case 20:
                {
                    if (Chapter1Level20::getInstance() != nullptr)
                    {
                        Chapter1Level20::destroy();
                    }
                    
                    Chapter1Level20::create();
                    
                    return Chapter1Level20::getInstance();
                }
                case 21:
                {
                    if (Chapter1Level21::getInstance() != nullptr)
                    {
                        Chapter1Level21::destroy();
                    }
                    
                    Chapter1Level21::create();
                    
                    return Chapter1Level21::getInstance();
                }
            }
        }
    }
    
    assert(false);
}

std::vector<UserDemoAction> LevelUtil::getUserDemoActionsForWorldAndLevel(int world, int level)
{
    std::vector<UserDemoAction> userDemoActions;
    
    switch (world)
    {
        case 1:
        {
            switch (level)
            {
                case 1:
                {
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 2.4));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 5.2f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 6.4f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 9.8f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 11.4f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_Exit, 13));
                }
                    break;
                case 2:
                {
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 3.18f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 4.32f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 6.78f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 9.95f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 11.38f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 13.24f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 14.80f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 16.9f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_Exit, 22.8f));
                }
                    break;
                case 6:
                {
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 1.40f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 3.40f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerTransform, 3.90f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 4.70f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 6.35f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerTransform, 8.2f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 9.2f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 10.9f));
                    
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 13.9f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 16.4f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 17.6f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 18.0f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerTransform, 18.2f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 18.8f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 21.7f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_Exit, 22.4f));
                }
                    break;
                case 9:
                {
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 2.3f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 2.7f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 8.8f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 12.05f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 13.5f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 17.8f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_Exit, 25.0f));
                }
                    break;
                case 10:
                {
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 2.4f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 5.2f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 19.4f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 33.5f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_Exit, 36.0f));
                }
                    break;
                case 14:
                {
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 2.60f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerTransform, 4.13f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 5.76f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 6.10f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 7.40f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerTransform, 7.90f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 10.15f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 11.08f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 11.49f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 12.48f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 15.03f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 17.38f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 23.31f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 25.01f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 26.64f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerTransform, 27.16f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 27.84f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_Exit, 29));
                }
                    break;
                case 15:
                {
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 1.0f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 2.5f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 4.98f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 5.40f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 8.60f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 8.86f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 9.90f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 12.78f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 13.01f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 15.21f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 17.43f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 17.80f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 19.05f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_Exit, 21.8f));
                }
                    break;
                case 16:
                {
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 1.5f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 3.1f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 5.35f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerTransform, 5.7f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerRight, 6.86f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerTransform, 9.90f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 10.56f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 11.74f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerDown, 14.1f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 15.55f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 17.3f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 17.7f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerTransform, 18.2f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_TriggerJump, 18.9f));
                    userDemoActions.push_back(UserDemoAction(DemoAction_Exit, 20.5f));
                }
                    break;
                default:
                    assert(false);
            }
        }
    }
    
    return userDemoActions;
}

RTTI_IMPL(Level, MainScreenState);
RTTI_IMPL(Chapter1Level1, Level);
RTTI_IMPL(Chapter1Level2, Level);
RTTI_IMPL(Chapter1Level3, Level);
RTTI_IMPL(Chapter1Level4, Level);
RTTI_IMPL(Chapter1Level5, Level);
RTTI_IMPL(Chapter1Level6, Level);
RTTI_IMPL(Chapter1Level7, Level);
RTTI_IMPL(Chapter1Level8, Level);
RTTI_IMPL(Chapter1Level9, Level);
RTTI_IMPL(Chapter1Level11, Level);
RTTI_IMPL(Chapter1Level12, Level);
RTTI_IMPL(Chapter1Level13, Level);
RTTI_IMPL(Chapter1Level14, Level);
RTTI_IMPL(Chapter1Level15, Level);
RTTI_IMPL(Chapter1Level16, Level);
RTTI_IMPL(Chapter1Level17, Level);
RTTI_IMPL(Chapter1Level18, Level);
RTTI_IMPL(Chapter1Level19, Level);
RTTI_IMPL(Chapter1Level20, Level);
RTTI_IMPL_NOPARENT(LevelUtil);

#include <sstream>

/// Chapter 1 Level 1 ///

Chapter1Level1 * Chapter1Level1::s_pInstance = nullptr;

void Chapter1Level1::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":1,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":0},{\"gridX\":32,\"gridY\":96,\"type\":1},{\"gridX\":72,\"gridY\":96,\"type\":2},{\"gridX\":196,\"gridY\":96,\"type\":0},{\"gridX\":228,\"gridY\":96,\"type\":1},{\"gridX\":272,\"gridY\":96,\"type\":2},{\"gridX\":368,\"gridY\":96,\"type\":2},{\"gridX\":568,\"gridY\":96,\"type\":0},{\"gridX\":640,\"gridY\":96,\"type\":2},{\"gridX\":688,\"gridY\":96,\"type\":0},{\"gridX\":760,\"gridY\":96,\"type\":1},{\"gridX\":800,\"gridY\":96,\"type\":2},{\"gridX\":912,\"gridY\":96,\"type\":2},{\"gridX\":964,\"gridY\":96,\"type\":1},{\"gridX\":1075,\"gridY\":96,\"type\":0},{\"gridX\":1124,\"gridY\":96,\"type\":2},{\"gridX\":1196,\"gridY\":96,\"type\":2},{\"gridX\":1232,\"gridY\":96,\"type\":0},{\"gridX\":1328,\"gridY\":96,\"type\":0},{\"gridX\":1384,\"gridY\":96,\"type\":2},{\"gridX\":1541,\"gridY\":96,\"type\":2},{\"gridX\":1712,\"gridY\":96,\"type\":2},{\"gridX\":1772,\"gridY\":96,\"type\":0},{\"gridX\":1804,\"gridY\":96,\"type\":1},{\"gridX\":1844,\"gridY\":96,\"type\":1},{\"gridX\":1884,\"gridY\":96,\"type\":2},{\"gridX\":390,\"gridY\":96,\"type\":11},{\"gridX\":89,\"gridY\":96,\"type\":6},{\"gridX\":565,\"gridY\":96,\"type\":11},{\"gridX\":1563,\"gridY\":96,\"type\":11},{\"gridX\":1554,\"gridY\":96,\"type\":6},{\"gridX\":1219,\"gridY\":96,\"type\":6},{\"gridX\":1034,\"gridY\":96,\"type\":15},{\"gridX\":1059,\"gridY\":96,\"type\":16},{\"gridX\":617,\"gridY\":96,\"type\":15},{\"gridX\":575,\"gridY\":96,\"type\":16},{\"gridX\":898,\"gridY\":147,\"type\":17},{\"gridX\":898,\"gridY\":160,\"type\":18},{\"gridX\":945,\"gridY\":163,\"type\":18},{\"gridX\":959,\"gridY\":162,\"type\":18},{\"gridX\":971,\"gridY\":126,\"type\":17},{\"gridX\":966,\"gridY\":125,\"type\":17},{\"gridX\":956,\"gridY\":144,\"type\":18},{\"gridX\":969,\"gridY\":146,\"type\":17},{\"gridX\":985,\"gridY\":113,\"type\":18},{\"gridX\":955,\"gridY\":157,\"type\":18},{\"gridX\":980,\"gridY\":154,\"type\":18},{\"gridX\":943,\"gridY\":156,\"type\":18},{\"gridX\":907,\"gridY\":167,\"type\":17},{\"gridX\":931,\"gridY\":164,\"type\":18},{\"gridX\":919,\"gridY\":169,\"type\":18},{\"gridX\":1184,\"gridY\":216,\"type\":7},{\"gridX\":1184,\"gridY\":204,\"type\":7},{\"gridX\":1184,\"gridY\":192,\"type\":7},{\"gridX\":1184,\"gridY\":180,\"type\":7},{\"gridX\":1184,\"gridY\":168,\"type\":7},{\"gridX\":1184,\"gridY\":156,\"type\":7},{\"gridX\":1184,\"gridY\":144,\"type\":7},{\"gridX\":1184,\"gridY\":132,\"type\":7},{\"gridX\":1184,\"gridY\":120,\"type\":7},{\"gridX\":1184,\"gridY\":108,\"type\":7},{\"gridX\":1184,\"gridY\":96,\"type\":7},{\"gridX\":1184,\"gridY\":240,\"type\":7},{\"gridX\":1184,\"gridY\":228,\"type\":7},{\"gridX\":907,\"gridY\":187,\"type\":17},{\"gridX\":973,\"gridY\":179,\"type\":18},{\"gridX\":973,\"gridY\":165,\"type\":17},{\"gridX\":967,\"gridY\":101,\"type\":17},{\"gridX\":979,\"gridY\":96,\"type\":17},{\"gridX\":1332,\"gridY\":240,\"type\":7},{\"gridX\":1457,\"gridY\":100,\"type\":18},{\"gridX\":1445,\"gridY\":96,\"type\":17},{\"gridX\":1468,\"gridY\":107,\"type\":17},{\"gridX\":1504,\"gridY\":96,\"type\":18},{\"gridX\":1453,\"gridY\":107,\"type\":18},{\"gridX\":1496,\"gridY\":106,\"type\":18},{\"gridX\":1485,\"gridY\":107,\"type\":17},{\"gridX\":1499,\"gridY\":84,\"type\":18},{\"gridX\":1604,\"gridY\":204,\"type\":7},{\"gridX\":1604,\"gridY\":192,\"type\":7},{\"gridX\":1604,\"gridY\":180,\"type\":7},{\"gridX\":1604,\"gridY\":168,\"type\":7},{\"gridX\":1604,\"gridY\":156,\"type\":7},{\"gridX\":1604,\"gridY\":144,\"type\":7},{\"gridX\":1604,\"gridY\":132,\"type\":7},{\"gridX\":1604,\"gridY\":120,\"type\":7},{\"gridX\":1604,\"gridY\":108,\"type\":7},{\"gridX\":1604,\"gridY\":96,\"type\":7},{\"gridX\":1604,\"gridY\":228,\"type\":7},{\"gridX\":1604,\"gridY\":216,\"type\":7},{\"gridX\":1595,\"gridY\":96,\"type\":1},{\"gridX\":1677,\"gridY\":96,\"type\":0},{\"gridX\":1575,\"gridY\":96,\"type\":16},{\"gridX\":1506,\"gridY\":96,\"type\":0},{\"gridX\":1518,\"gridY\":96,\"type\":6},{\"gridX\":1535,\"gridY\":96,\"type\":6},{\"gridX\":1457,\"gridY\":125,\"type\":17},{\"gridX\":1474,\"gridY\":177,\"type\":18},{\"gridX\":1479,\"gridY\":128,\"type\":17},{\"gridX\":1478,\"gridY\":148,\"type\":17},{\"gridX\":1485,\"gridY\":170,\"type\":18},{\"gridX\":1477,\"gridY\":194,\"type\":17},{\"gridX\":1485,\"gridY\":187,\"type\":18},{\"gridX\":1477,\"gridY\":206,\"type\":18},{\"gridX\":1436,\"gridY\":156,\"type\":7},{\"gridX\":1436,\"gridY\":168,\"type\":7},{\"gridX\":1436,\"gridY\":180,\"type\":7},{\"gridX\":1436,\"gridY\":192,\"type\":7},{\"gridX\":1436,\"gridY\":204,\"type\":7},{\"gridX\":1436,\"gridY\":216,\"type\":7},{\"gridX\":1436,\"gridY\":228,\"type\":7}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":132,\"gridY\":0,\"type\":31},{\"gridX\":512,\"gridY\":0,\"type\":30},{\"gridX\":780,\"gridY\":0,\"type\":30},{\"gridX\":836,\"gridY\":0,\"type\":31},{\"gridX\":360,\"gridY\":0,\"type\":30},{\"gridX\":1024,\"gridY\":0,\"type\":31},{\"gridX\":1112,\"gridY\":0,\"type\":31},{\"gridX\":1324,\"gridY\":0,\"type\":30},{\"gridX\":926,\"gridY\":0,\"type\":30},{\"gridX\":1372,\"gridY\":0,\"type\":31},{\"gridX\":1466,\"gridY\":0,\"type\":30}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1664,\"gridY\":96,\"type\":12},{\"gridX\":323,\"gridY\":96,\"type\":58},{\"gridX\":318,\"gridY\":97,\"type\":60},{\"gridX\":698,\"gridY\":96,\"type\":58},{\"gridX\":718,\"gridY\":96,\"type\":58},{\"gridX\":666,\"gridY\":97,\"type\":60},{\"gridX\":739,\"gridY\":96,\"type\":58},{\"gridX\":689,\"gridY\":97,\"type\":60},{\"gridX\":708,\"gridY\":97,\"type\":60},{\"gridX\":812,\"gridY\":96,\"type\":58},{\"gridX\":808,\"gridY\":105,\"type\":59},{\"gridX\":806,\"gridY\":118,\"type\":60},{\"gridX\":821,\"gridY\":118,\"type\":60},{\"gridX\":632,\"gridY\":96,\"type\":58},{\"gridX\":656,\"gridY\":96,\"type\":58},{\"gridX\":624,\"gridY\":97,\"type\":60},{\"gridX\":677,\"gridY\":96,\"type\":58},{\"gridX\":644,\"gridY\":97,\"type\":60},{\"gridX\":900,\"gridY\":96,\"type\":58},{\"gridX\":895,\"gridY\":119,\"type\":59},{\"gridX\":895,\"gridY\":129,\"type\":60},{\"gridX\":895,\"gridY\":106,\"type\":59},{\"gridX\":958,\"gridY\":96,\"type\":58},{\"gridX\":994,\"gridY\":96,\"type\":58},{\"gridX\":951,\"gridY\":105,\"type\":60},{\"gridX\":993,\"gridY\":105,\"type\":60},{\"gridX\":1301,\"gridY\":96,\"type\":58},{\"gridX\":1306,\"gridY\":103,\"type\":60},{\"gridX\":972,\"gridY\":122,\"type\":61},{\"gridX\":893,\"gridY\":214,\"type\":60},{\"gridX\":928,\"gridY\":38,\"type\":50},{\"gridX\":929,\"gridY\":41,\"type\":21},{\"gridX\":1172,\"gridY\":248,\"type\":47},{\"gridX\":1176,\"gridY\":248,\"type\":48},{\"gridX\":1192,\"gridY\":248,\"type\":48},{\"gridX\":1320,\"gridY\":248,\"type\":48},{\"gridX\":1364,\"gridY\":236,\"type\":48},{\"gridX\":1380,\"gridY\":236,\"type\":48},{\"gridX\":1304,\"gridY\":248,\"type\":48},{\"gridX\":1288,\"gridY\":248,\"type\":48},{\"gridX\":1272,\"gridY\":248,\"type\":48},{\"gridX\":1256,\"gridY\":248,\"type\":48},{\"gridX\":1240,\"gridY\":248,\"type\":48},{\"gridX\":1224,\"gridY\":248,\"type\":48},{\"gridX\":1208,\"gridY\":248,\"type\":48},{\"gridX\":1332,\"gridY\":236,\"type\":48},{\"gridX\":1348,\"gridY\":236,\"type\":48},{\"gridX\":1396,\"gridY\":236,\"type\":48},{\"gridX\":903,\"gridY\":203,\"type\":59},{\"gridX\":987,\"gridY\":178,\"type\":63},{\"gridX\":975,\"gridY\":185,\"type\"" << ":59},{\"gridX\":989,\"gridY\":205,\"type\":60},{\"gridX\":914,\"gridY\":204,\"type\":59},{\"gridX\":912,\"gridY\":220,\"type\":63},{\"gridX\":903,\"gridY\":230,\"type\":60},{\"gridX\":917,\"gridY\":226,\"type\":59},{\"gridX\":982,\"gridY\":199,\"type\":59},{\"gridX\":989,\"gridY\":165,\"type\":59},{\"gridX\":1336,\"gridY\":248,\"type\":49},{\"gridX\":1412,\"gridY\":236,\"type\":48},{\"gridX\":1428,\"gridY\":236,\"type\":48},{\"gridX\":1328,\"gridY\":236,\"type\":47},{\"gridX\":1620,\"gridY\":236,\"type\":48},{\"gridX\":1588,\"gridY\":236,\"type\":48},{\"gridX\":1604,\"gridY\":236,\"type\":48},{\"gridX\":1584,\"gridY\":236,\"type\":47},{\"gridX\":1636,\"gridY\":236,\"type\":49},{\"gridX\":1548,\"gridY\":96,\"type\":58},{\"gridX\":1528,\"gridY\":117,\"type\":61},{\"gridX\":1540,\"gridY\":98,\"type\":59},{\"gridX\":1438,\"gridY\":96,\"type\":58},{\"gridX\":1468,\"gridY\":30,\"type\":4},{\"gridX\":1438,\"gridY\":105,\"type\":63},{\"gridX\":1438,\"gridY\":119,\"type\":59},{\"gridX\":1444,\"gridY\":130,\"type\":59},{\"gridX\":1449,\"gridY\":135,\"type\":59},{\"gridX\":1431,\"gridY\":136,\"type\":60},{\"gridX\":1455,\"gridY\":136,\"type\":60},{\"gridX\":1475,\"gridY\":156,\"type\":63},{\"gridX\":1473,\"gridY\":163,\"type\":60},{\"gridX\":1467,\"gridY\":214,\"type\":60},{\"gridX\":1483,\"gridY\":211,\"type\":59},{\"gridX\":1490,\"gridY\":214,\"type\":60},{\"gridX\":1476,\"gridY\":144,\"type\":59},{\"gridX\":1499,\"gridY\":163,\"type\":60},{\"gridX\":1491,\"gridY\":160,\"type\":59},{\"gridX\":1444,\"gridY\":236,\"type\":49},{\"gridX\":1450,\"gridY\":231,\"type\":61},{\"gridX\":1510,\"gridY\":231,\"type\":61},{\"gridX\":1464,\"gridY\":30,\"type\":3},{\"gridX\":1482,\"gridY\":30,\"type\":5},{\"gridX\":728,\"gridY\":97,\"type\":60},{\"gridX\":755,\"gridY\":96,\"type\":58},{\"gridX\":751,\"gridY\":97,\"type\":60},{\"gridX\":1032,\"gridY\":222,\"type\":61},{\"gridX\":992,\"gridY\":191,\"type\":59},{\"gridX\":999,\"gridY\":201,\"type\":63},{\"gridX\":1009,\"gridY\":205,\"type\":60}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1632,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":1092,\"gridY\":96,\"type\":6},{\"gridX\":1356,\"gridY\":96,\"type\":6},{\"gridX\":1616,\"gridY\":241,\"type\":6},{\"gridX\":1466,\"gridY\":35,\"type\":6},{\"gridX\":1524,\"gridY\":96,\"type\":6}],\"collectibles\":[{\"gridX\":148,\"gridY\":132,\"type\":0},{\"gridX\":140,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":164,\"gridY\":136,\"type\":1},{\"gridX\":188,\"gridY\":116,\"type\":0},{\"gridX\":180,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":104,\"type\":0},{\"gridX\":184,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":104,\"type\":0},{\"gridX\":240,\"gridY\":104,\"type\":0},{\"gridX\":248,\"gridY\":104,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":0},{\"gridX\":264,\"gridY\":104,\"type\":0},{\"gridX\":332,\"gridY\":130,\"type\":0},{\"gridX\":355,\"gridY\":148,\"type\":0},{\"gridX\":272,\"gridY\":104,\"type\":0},{\"gridX\":445,\"gridY\":105,\"type\":0},{\"gridX\":395,\"gridY\":152,\"type\":0},{\"gridX\":383,\"gridY\":156,\"type\":0},{\"gridX\":435,\"gridY\":105,\"type\":0},{\"gridX\":425,\"gridY\":105,\"type\":0},{\"gridX\":417,\"gridY\":106,\"type\":0},{\"gridX\":512,\"gridY\":104,\"type\":0},{\"gridX\":516,\"gridY\":112,\"type\":0},{\"gridX\":520,\"gridY\":120,\"type\":0},{\"gridX\":524,\"gridY\":128,\"type\":0},{\"gridX\":560,\"gridY\":128,\"type\":0},{\"gridX\":564,\"gridY\":120,\"type\":0},{\"gridX\":568,\"gridY\":112,\"type\":0},{\"gridX\":572,\"gridY\":104,\"type\":0},{\"gridX\":580,\"gridY\":104,\"type\":0},{\"gridX\":828,\"gridY\":148,\"type\":0},{\"gridX\":596,\"gridY\":112,\"type\":0},{\"gridX\":588,\"gridY\":104,\"type\":0},{\"gridX\":768,\"gridY\":136,\"type\":0},{\"gridX\":820,\"gridY\":148,\"type\":0},{\"gridX\":836,\"gridY\":148,\"type\":0},{\"gridX\":977,\"gridY\":132,\"type\":0},{\"gridX\":728,\"gridY\":128,\"type\":0},{\"gridX\":712,\"gridY\":128,\"type\":0},{\"gridX\":736,\"gridY\":128,\"type\":0},{\"gridX\":704,\"gridY\":128,\"type\":0},{\"gridX\":744,\"gridY\":128,\"type\":0},{\"gridX\":604,\"gridY\":118,\"type\":0},{\"gridX\":720,\"gridY\":128,\"type\":0},{\"gridX\":1026,\"gridY\":151,\"type\":0},{\"gridX\":852,\"gridY\":164,\"type\":0},{\"gridX\":844,\"gridY\":148,\"type\":0},{\"gridX\":848,\"gridY\":156,\"type\":0},{\"gridX\":752,\"gridY\":128,\"type\":0},{\"gridX\":760,\"gridY\":128,\"type\":0},{\"gridX\":776,\"gridY\":144,\"type\":0},{\"gridX\":868,\"gridY\":176,\"type\":0},{\"gridX\":1412,\"gridY\":180,\"type\":0},{\"gridX\":1392,\"gridY\":172,\"type\":0},{\"gridX\":876,\"gridY\":176,\"type\":0},{\"gridX\":1108,\"gridY\":132,\"type\":0},{\"gridX\":1016,\"gridY\":142,\"type\":0},{\"gridX\":1009,\"gridY\":132,\"type\":0},{\"gridX\":988,\"gridY\":132,\"type\":0},{\"gridX\":999,\"gridY\":132,\"type\":0},{\"gridX\":1128,\"gridY\":172,\"type\":0},{\"gridX\":860,\"gridY\":172,\"type\":0},{\"gridX\":1124,\"gridY\":164,\"type\":0},{\"gridX\":1120,\"gridY\":156,\"type\":0},{\"gridX\":1132,\"gridY\":180,\"type\":0},{\"gridX\":1112,\"gridY\":140,\"type\":0},{\"gridX\":1116,\"gridY\":148,\"type\":0},{\"gridX\":1421,\"gridY\":174,\"type\":1},{\"gridX\":528,\"gridY\":136,\"type\":0},{\"gridX\":556,\"gridY\":136,\"type\":0},{\"gridX\":1272,\"gridY\":120,\"type\":0},{\"gridX\":1268,\"gridY\":112,\"type\":0},{\"gridX\":1240,\"gridY\":104,\"type\":0},{\"gridX\":1248,\"gridY\":104,\"type\":0},{\"gridX\":1256,\"gridY\":104,\"type\":0},{\"gridX\":1264,\"gridY\":104,\"type\":0},{\"gridX\":1308,\"gridY\":132,\"type\":0},{\"gridX\":1316,\"gridY\":132,\"type\":0},{\"gridX\":1324,\"gridY\":132,\"type\":0},{\"gridX\":1332,\"gridY\":132,\"type\":0},{\"gridX\":1384,\"gridY\":156,\"type\":0},{\"gridX\":1388,\"gridY\":164,\"type\":0},{\"gridX\":1372,\"gridY\":132,\"type\":0},{\"gridX\":1376,\"gridY\":140,\"type\":0},{\"gridX\":1380,\"gridY\":148,\"type\":0},{\"gridX\":1426,\"gridY\":168,\"type\":0},{\"gridX\":1434,\"gridY\":163,\"type\":0},{\"gridX\":1484,\"gridY\":192,\"type\":0},{\"gridX\":1476,\"gridY\":192,\"type\":0},{\"gridX\":1492,\"gridY\":192,\"type\":0},{\"gridX\":1500,\"gridY\":192,\"type\":0},{\"gridX\":1508,\"gridY\":192,\"type\":0},{\"gridX\":1436,\"gridY\":244,\"type\":0},{\"gridX\":1516,\"gridY\":192,\"type\":0},{\"gridX\":1428,\"gridY\":244,\"type\":0},{\"gridX\":1540,\"gridY\":180,\"type\":1},{\"gridX\":342,\"gridY\":139,\"type\":0},{\"gridX\":369,\"gridY\":154,\"type\":0},{\"gridX\":1401,\"gridY\":177,\"type\":0},{\"gridX\":1363,\"gridY\":111,\"type\":0},{\"gridX\":1104,\"gridY\":124,\"type\":0},{\"gridX\":1366,\"gridY\":122,\"type\":0},{\"gridX\":1488,\"gridY\":240,\"type\":2},{\"gridX\":1420,\"gridY\":244,\"type\":0},{\"gridX\":1412,\"gridY\":244,\"type\":0},{\"gridX\":1532,\"gridY\":188,\"type\":0},{\"gridX\":1404,\"gridY\":244,\"type\":0},{\"gridX\":1524,\"gridY\":192,\"type\":0},{\"gridX\":1520,\"gridY\":240,\"type\":2},{\"gridX\":1472,\"gridY\":240,\"type\":2},{\"gridX\":1456,\"gridY\":240,\"type\":2},{\"gridX\":1504,\"gridY\":240,\"type\":2},{\"gridX\":1396,\"gridY\":244,\"type\":0},{\"gridX\":1388,\"gridY\":244,\"type\":0},{\"gridX\":1380,\"gridY\":244,\"type\":0},{\"gridX\":1372,\"gridY\":244,\"type\":0},{\"gridX\":1364,\"gridY\":244,\"type\":0},{\"gridX\":1047,\"gridY\":232,\"type\":3},{\"gridX\":1472,\"gridY\":56,\"type\":3}],\"jons\":[{\"gridX\":31,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":309,\"gridY\":92,\"type\":3},{\"gridX\":1213,\"gridY\":92,\"type\":3},{\"gridX\":414,\"gridY\":92,\"type\":1},{\"gridX\":209,\"gridY\":85,\"type\":0},{\"gridX\":41,\"gridY\":85,\"type\":0},{\"gridX\":68,\"gridY\":92,\"type\":1},{\"gridX\":1519,\"gridY\":92,\"type\":3},{\"gridX\":1527,\"gridY\":92,\"type\":1}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level1(text.c_str());
}

void Chapter1Level1::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level1 * Chapter1Level1::getInstance()
{
    return s_pInstance;
}

Chapter1Level1::Chapter1Level1(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 2 ///

Chapter1Level2 * Chapter1Level2::s_pInstance = nullptr;

void Chapter1Level2::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":2,\"midgrounds\":[{\"gridX\":87,\"gridY\":96,\"type\":17},{\"gridX\":73,\"gridY\":109,\"type\":18},{\"gridX\":74,\"gridY\":117,\"type\":18},{\"gridX\":86,\"gridY\":117,\"type\":17},{\"gridX\":109,\"gridY\":96,\"type\":17},{\"gridX\":108,\"gridY\":117,\"type\":17},{\"gridX\":129,\"gridY\":117,\"type\":17},{\"gridX\":131,\"gridY\":96,\"type\":17},{\"gridX\":150,\"gridY\":116,\"type\":17},{\"gridX\":170,\"gridY\":116,\"type\":17},{\"gridX\":173,\"gridY\":96,\"type\":17},{\"gridX\":151,\"gridY\":96,\"type\":17},{\"gridX\":171,\"gridY\":134,\"type\":17},{\"gridX\":184,\"gridY\":113,\"type\":18},{\"gridX\":194,\"gridY\":97,\"type\":17},{\"gridX\":199,\"gridY\":119,\"type\":17},{\"gridX\":185,\"gridY\":130,\"type\":18},{\"gridX\":200,\"gridY\":141,\"type\":17},{\"gridX\":182,\"gridY\":147,\"type\":18},{\"gridX\":218,\"gridY\":139,\"type\":17},{\"gridX\":197,\"gridY\":149,\"type\":17},{\"gridX\":217,\"gridY\":159,\"type\":17},{\"gridX\":201,\"gridY\":161,\"type\":18},{\"gridX\":239,\"gridY\":139,\"type\":17},{\"gridX\":239,\"gridY\":159,\"type\":17},{\"gridX\":248,\"gridY\":157,\"type\":18},{\"gridX\":253,\"gridY\":168,\"type\":18},{\"gridX\":215,\"gridY\":118,\"type\":17},{\"gridX\":162,\"gridY\":106,\"type\":18},{\"gridX\":12,\"gridY\":96,\"type\":0},{\"gridX\":69,\"gridY\":96,\"type\":6},{\"gridX\":56,\"gridY\":96,\"type\":2},{\"gridX\":712,\"gridY\":96,\"type\":11},{\"gridX\":680,\"gridY\":96,\"type\":2},{\"gridX\":584,\"gridY\":96,\"type\":1},{\"gridX\":716,\"gridY\":96,\"type\":0},{\"gridX\":803,\"gridY\":96,\"type\":6},{\"gridX\":753,\"gridY\":96,\"type\":1},{\"gridX\":822,\"gridY\":96,\"type\":16},{\"gridX\":700,\"gridY\":96,\"type\":15},{\"gridX\":979,\"gridY\":96,\"type\":6},{\"gridX\":1003,\"gridY\":96,\"type\":6},{\"gridX\":1034,\"gridY\":96,\"type\":6},{\"gridX\":941,\"gridY\":97,\"type\":17},{\"gridX\":946,\"gridY\":102,\"type\":18},{\"gridX\":959,\"gridY\":112,\"type\":18},{\"gridX\":966,\"gridY\":109,\"type\":18},{\"gridX\":958,\"gridY\":96,\"type\":17},{\"gridX\":978,\"gridY\":119,\"type\":18},{\"gridX\":970,\"gridY\":130,\"type\":17},{\"gridX\":1030,\"gridY\":96,\"type\":17},{\"gridX\":1030,\"gridY\":109,\"type\":18},{\"gridX\":1039,\"gridY\":115,\"type\":18},{\"gridX\":1033,\"gridY\":126,\"type\":18},{\"gridX\":1027,\"gridY\":132,\"type\":18},{\"gridX\":1005,\"gridY\":136,\"type\":18},{\"gridX\":1014,\"gridY\":138,\"type\":18},{\"gridX\":1054,\"gridY\":128,\"type\":17},{\"gridX\":1072,\"gridY\":136,\"type\":18},{\"gridX\":1042,\"gridY\":129,\"type\":18},{\"gridX\":1088,\"gridY\":133,\"type\":17},{\"gridX\":1098,\"gridY\":140,\"type\":18},{\"gridX\":1105,\"gridY\":128,\"type\":18},{\"gridX\":1117,\"gridY\":142,\"type\":18},{\"gridX\":1222,\"gridY\":96,\"type\":17},{\"gridX\":1203,\"gridY\":96,\"type\":17},{\"gridX\":1213,\"gridY\":109,\"type\":18},{\"gridX\":1195,\"gridY\":111,\"type\":18},{\"gridX\":1205,\"gridY\":122,\"type\":18},{\"gridX\":1181,\"gridY\":120,\"type\":18},{\"gridX\":1193,\"gridY\":126,\"type\":17},{\"gridX\":1144,\"gridY\":129,\"type\":18},{\"gridX\":1169,\"gridY\":125,\"type\":17},{\"gridX\":1157,\"gridY\":134,\"type\":18},{\"gridX\":1192,\"gridY\":97,\"type\":18},{\"gridX\":1231,\"gridY\":96,\"type\":18},{\"gridX\":1068,\"gridY\":123,\"type\":18},{\"gridX\":324,\"gridY\":96,\"type\":1},{\"gridX\":268,\"gridY\":96,\"type\":0},{\"gridX\":835,\"gridY\":96,\"type\":0},{\"gridX\":918,\"gridY\":96,\"type\":1},{\"gridX\":1367,\"gridY\":96,\"type\":1},{\"gridX\":1328,\"gridY\":96,\"type\":0},{\"gridX\":1367,\"gridY\":96,\"type\":6},{\"gridX\":1518,\"gridY\":96,\"type\":0},{\"gridX\":1447,\"gridY\":96,\"type\":15},{\"gridX\":1474,\"gridY\":96,\"type\":16},{\"gridX\":1610,\"gridY\":96,\"type\":15},{\"gridX\":1579,\"gridY\":96,\"type\":16},{\"gridX\":150,\"gridY\":133,\"type\":17},{\"gridX\":477,\"gridY\":96,\"type\":17},{\"gridX\":484,\"gridY\":107,\"type\":18},{\"gridX\":476,\"gridY\":109,\"type\":18},{\"gridX\":563,\"gridY\":118,\"type\":18},{\"gridX\":572,\"gridY\":97,\"type\":17},{\"gridX\":574,\"gridY\":107,\"type\":18},{\"gridX\":563,\"gridY\":84,\"type\":18},{\"gridX\":1106,\"gridY\":159,\"type\":17},{\"gridX\":1128,\"gridY\":160,\"type\":17},{\"gridX\":1132,\"gridY\":160,\"type\":17},{\"gridX\":1107,\"gridY\":171,\"type\":18},{\"gridX\":1105,\"gridY\":189,\"type\":18},{\"gridX\":1117,\"gridY\":181,\"type\":18},{\"gridX\":1130,\"gridY\":179,\"type\":17},{\"gridX\":1143,\"gridY\":178,\"type\":18},{\"gridX\":1128,\"gridY\":196,\"type\":17},{\"gridX\":1142,\"gridY\":191,\"type\":18},{\"gridX\":1133,\"gridY\":96,\"type\":17},{\"gridX\":1118,\"gridY\":116,\"type\":18},{\"gridX\":1148,\"gridY\":88,\"type\":18}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28}],\"pits\":[{\"gridX\":413,\"gridY\":0,\"type\":31},{\"gridX\":500,\"gridY\":0,\"type\":31},{\"gridX\":733,\"gridY\":0,\"type\":30},{\"gridX\":1044,\"gridY\":0,\"type\":33}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1567,\"gridY\":96,\"type\":12},{\"gridX\":86,\"gridY\":123,\"type\":59},{\"gridX\":65,\"gridY\":127,\"type\":60},{\"gridX\":91,\"gridY\":131,\"type\":59},{\"gridX\":94,\"gridY\":137,\"type\":60},{\"gridX\":111,\"gridY\":125,\"type\":59},{\"gridX\":111,\"gridY\":131,\"type\":59},{\"gridX\":112,\"gridY\":137,\"type\":60},{\"gridX\":127,\"gridY\":130,\"type\":59},{\"gridX\":134,\"gridY\":137,\"type\":60},{\"gridX\":226,\"gridY\":98,\"type\":59},{\"gridX\":243,\"gridY\":96,\"type\":58},{\"gridX\":233,\"gridY\":96,\"type\":58},{\"gridX\":229,\"gridY\":117,\"type\":60},{\"gridX\":235,\"gridY\":111,\"type\":59},{\"gridX\":216,\"gridY\":180,\"type\":60},{\"gridX\":208,\"gridY\":176,\"type\":59},{\"gridX\":239,\"gridY\":180,\"type\":60},{\"gridX\":232,\"gridY\":173,\"type\":59},{\"gridX\":253,\"gridY\":176,\"type\":59},{\"gridX\":157,\"gridY\":154,\"type\":61},{\"gridX\":253,\"gridY\":117,\"type\":60},{\"gridX\":193,\"gridY\":96,\"type\":54},{\"gridX\":186,\"gridY\":96,\"type\":53},{\"gridX\":198,\"gridY\":96,\"type\":59},{\"gridX\":211,\"gridY\":96,\"type\":58},{\"gridX\":208,\"gridY\":98,\"type\":60},{\"gridX\":186,\"gridY\":115,\"type\":61},{\"gridX\":388,\"gridY\":96,\"type\":58},{\"gridX\":388,\"gridY\":131,\"type\":60},{\"gridX\":393,\"gridY\":106,\"type\":59},{\"gridX\":396,\"gridY\":119,\"type\":59},{\"gridX\":358,\"gridY\":96,\"type\":55},{\"gridX\":350,\"gridY\":96,\"type\":53},{\"gridX\":381,\"gridY\":96,\"type\":56},{\"gridX\":586,\"gridY\":96,\"type\":58},{\"gridX\":477,\"gridY\":119,\"type\":59},{\"gridX\":482,\"gridY\":133,\"type\":59},{\"gridX\":471,\"gridY\":142,\"type\":60},{\"gridX\":568,\"gridY\":143,\"type\":59},{\"gridX\":565,\"gridY\":131,\"type\":59},{\"gridX\":555,\"gridY\":136,\"type\":59},{\"gridX\":552,\"gridY\":144,\"type\":59},{\"gridX\":554,\"gridY\":155,\"type\":60},{\"gridX\":664,\"gridY\":96,\"type\":54},{\"gridX\":657,\"gridY\":96,\"type\":53},{\"gridX\":676,\"gridY\":96,\"type\":56},{\"gridX\":846,\"gridY\":96,\"type\":58},{\"gridX\":868,\"gridY\":96,\"type\":58},{\"gridX\":912,\"gridY\":96,\"type\":58},{\"gridX\":890,\"gridY\":96,\"type\":58},{\"gridX\":933,\"gridY\":96,\"type\":58},{\"gridX\":899,\"gridY\":100,\"type\":60},{\"gridX\":919,\"gridY\":100,\"type\":60},{\"gridX\":874,\"gridY\":100,\"type\":60},{\"gridX\":852,\"gridY\":100,\"type\":60},{\"gridX\":835,\"gridY\":100,\"type\":60},{\"gridX\":1405,\"gridY\":96,\"type\":55},{\"gridX\":1398,\"gridY\":96,\"type\":53},{\"gridX\":1427,\"gridY\":96,\"type\":56},{\"gridX\":1499,\"gr" << "idY\":96,\"type\":54},{\"gridX\":1492,\"gridY\":96,\"type\":53},{\"gridX\":1510,\"gridY\":96,\"type\":56},{\"gridX\":1072,\"gridY\":72,\"type\":50},{\"gridX\":1088,\"gridY\":72,\"type\":50},{\"gridX\":1132,\"gridY\":16,\"type\":50},{\"gridX\":1148,\"gridY\":16,\"type\":50},{\"gridX\":1164,\"gridY\":16,\"type\":50},{\"gridX\":1250,\"gridY\":51,\"type\":50},{\"gridX\":969,\"gridY\":131,\"type\":60},{\"gridX\":947,\"gridY\":148,\"type\":61},{\"gridX\":987,\"gridY\":129,\"type\":59},{\"gridX\":990,\"gridY\":140,\"type\":60},{\"gridX\":1028,\"gridY\":140,\"type\":60},{\"gridX\":1012,\"gridY\":157,\"type\":61},{\"gridX\":1100,\"gridY\":139,\"type\":60},{\"gridX\":1120,\"gridY\":139,\"type\":60},{\"gridX\":1113,\"gridY\":130,\"type\":59},{\"gridX\":1145,\"gridY\":139,\"type\":60},{\"gridX\":1127,\"gridY\":123,\"type\":59},{\"gridX\":1136,\"gridY\":135,\"type\":59},{\"gridX\":1186,\"gridY\":129,\"type\":60},{\"gridX\":1212,\"gridY\":129,\"type\":60},{\"gridX\":1204,\"gridY\":126,\"type\":59},{\"gridX\":1233,\"gridY\":146,\"type\":61},{\"gridX\":304,\"gridY\":134,\"type\":61},{\"gridX\":280,\"gridY\":117,\"type\":60},{\"gridX\":247,\"gridY\":113,\"type\":59},{\"gridX\":278,\"gridY\":97,\"type\":58},{\"gridX\":271,\"gridY\":112,\"type\":59},{\"gridX\":399,\"gridY\":131,\"type\":60},{\"gridX\":1097,\"gridY\":211,\"type\":59},{\"gridX\":1108,\"gridY\":194,\"type\":59},{\"gridX\":1112,\"gridY\":208,\"type\":59},{\"gridX\":1106,\"gridY\":218,\"type\":60},{\"gridX\":1146,\"gridY\":201,\"type\":59},{\"gridX\":1146,\"gridY\":217,\"type\":59},{\"gridX\":1148,\"gridY\":227,\"type\":60},{\"gridX\":1154,\"gridY\":190,\"type\":59},{\"gridX\":1136,\"gridY\":116,\"type\":59},{\"gridX\":1130,\"gridY\":102,\"type\":59},{\"gridX\":1164,\"gridY\":96,\"type\":61},{\"gridX\":1181,\"gridY\":96,\"type\":61},{\"gridX\":85,\"gridY\":133,\"type\":59}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1535,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":638,\"gridY\":96,\"type\":8},{\"gridX\":800,\"gridY\":96,\"type\":9}],\"collectibles\":[{\"gridX\":400,\"gridY\":160,\"type\":1},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":108,\"gridY\":100,\"type\":0},{\"gridX\":116,\"gridY\":100,\"type\":0},{\"gridX\":124,\"gridY\":100,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":144,\"type\":0},{\"gridX\":264,\"gridY\":144,\"type\":0},{\"gridX\":280,\"gridY\":144,\"type\":0},{\"gridX\":304,\"gridY\":144,\"type\":0},{\"gridX\":296,\"gridY\":144,\"type\":0},{\"gridX\":288,\"gridY\":144,\"type\":0},{\"gridX\":485,\"gridY\":168,\"type\":0},{\"gridX\":477,\"gridY\":173,\"type\":0},{\"gridX\":634,\"gridY\":111,\"type\":0},{\"gridX\":840,\"gridY\":128,\"type\":0},{\"gridX\":580,\"gridY\":183,\"type\":0},{\"gridX\":590,\"gridY\":179,\"type\":0},{\"gridX\":598,\"gridY\":171,\"type\":0},{\"gridX\":604,\"gridY\":163,\"type\":0},{\"gridX\":610,\"gridY\":154,\"type\":0},{\"gridX\":647,\"gridY\":111,\"type\":0},{\"gridX\":654,\"gridY\":120,\"type\":0},{\"gridX\":663,\"gridY\":129,\"type\":0},{\"gridX\":710,\"gridY\":104,\"type\":0},{\"gridX\":936,\"gridY\":164,\"type\":0},{\"gridX\":1092,\"gridY\":183,\"type\":0},{\"gridX\":1087,\"gridY\":189,\"type\":0},{\"gridX\":1097,\"gridY\":175,\"type\":0},{\"gridX\":1016,\"gridY\":168,\"type\":0},{\"gridX\":1024,\"gridY\":168,\"type\":0},{\"gridX\":1032,\"gridY\":168,\"type\":0},{\"gridX\":1040,\"gridY\":168,\"type\":0},{\"gridX\":1044,\"gridY\":176,\"type\":0},{\"gridX\":1048,\"gridY\":184,\"type\":0},{\"gridX\":1056,\"gridY\":192,\"type\":0},{\"gridX\":928,\"gridY\":160,\"type\":0},{\"gridX\":1064,\"gridY\":196,\"type\":0},{\"gridX\":1080,\"gridY\":192,\"type\":1},{\"gridX\":1135,\"gridY\":199,\"type\":1},{\"gridX\":848,\"gridY\":128,\"type\":0},{\"gridX\":856,\"gridY\":128,\"type\":0},{\"gridX\":864,\"gridY\":128,\"type\":0},{\"gridX\":872,\"gridY\":128,\"type\":0},{\"gridX\":880,\"gridY\":128,\"type\":0},{\"gridX\":896,\"gridY\":128,\"type\":0},{\"gridX\":904,\"gridY\":128,\"type\":0},{\"gridX\":1368,\"gridY\":104,\"type\":0},{\"gridX\":1376,\"gridY\":112,\"type\":0},{\"gridX\":1384,\"gridY\":120,\"type\":0},{\"gridX\":1392,\"gridY\":128,\"type\":0},{\"gridX\":1464,\"gridY\":104,\"type\":0},{\"gridX\":1472,\"gridY\":112,\"type\":0},{\"gridX\":1480,\"gridY\":120,\"type\":0},{\"gridX\":1488,\"gridY\":128,\"type\":0},{\"gridX\":617,\"gridY\":143,\"type\":0},{\"gridX\":623,\"gridY\":131,\"type\":0},{\"gridX\":628,\"gridY\":121,\"type\":0},{\"gridX\":684,\"gridY\":125,\"type\":0},{\"gridX\":693,\"gridY\":116,\"type\":0},{\"gridX\":701,\"gridY\":108,\"type\":0},{\"gridX\":494,\"gridY\":179,\"type\":0},{\"gridX\":500,\"gridY\":189,\"type\":0},{\"gridX\":511,\"gridY\":198,\"type\":0},{\"gridX\":525,\"gridY\":204,\"type\":0},{\"gridX\":540,\"gridY\":204,\"type\":0},{\"gridX\":408,\"gridY\":172,\"type\":0},{\"gridX\":416,\"gridY\":182,\"type\":0},{\"gridX\":428,\"gridY\":189,\"type\":0},{\"gridX\":443,\"gridY\":194,\"type\":0},{\"gridX\":456,\"gridY\":191,\"type\":0},{\"gridX\":467,\"gridY\":183,\"type\":0},{\"gridX\":551,\"gridY\":197,\"type\":0},{\"gridX\":675,\"gridY\":130,\"type\":0},{\"gridX\":920,\"gridY\":152,\"type\":0},{\"gridX\":912,\"gridY\":144,\"type\":0},{\"gridX\":908,\"gridY\":136,\"type\":0},{\"gridX\":888,\"gridY\":128,\"type\":0},{\"gridX\":1103,\"gridY\":170,\"type\":0},{\"gridX\":1072,\"gridY\":196,\"type\":0},{\"gridX\":1109,\"gridY\":167,\"type\":0},{\"gridX\":1157,\"gridY\":166,\"type\":0},{\"gridX\":1148,\"gridY\":166,\"type\":0},{\"gridX\":1165,\"gridY\":166,\"type\":0},{\"gridX\":1175,\"gridY\":166,\"type\":0},{\"gridX\":1185,\"gridY\":162,\"type\":0},{\"gridX\":1200,\"gridY\":160,\"type\":0},{\"gridX\":1208,\"gridY\":160,\"type\":0},{\"gridX\":1216,\"gridY\":160,\"type\":0},{\"gridX\":1224,\"gridY\":160,\"type\":0},{\"gridX\":1232,\"gridY\":160,\"type\":0},{\"gridX\":1240,\"gridY\":160,\"type\":0},{\"gridX\":1248,\"gridY\":160,\"type\":0},{\"gridX\":1354,\"gridY\":102,\"type\":0},{\"gridX\":720,\"gridY\":104,\"type\":0},{\"gridX\":726,\"gridY\":115,\"type\":0},{\"gridX\":735,\"gridY\":123,\"type\":0},{\"gridX\":746,\"gridY\":125,\"type\":0},{\"gridX\":758,\"gridY\":125,\"type\":0},{\"gridX\":765,\"gridY\":120,\"type\":0},{\"gridX\":772,\"gridY\":113,\"type\":0},{\"gridX\":779,\"gridY\":105,\"type\":0},{\"gridX\":280,\"gridY\":228,\"type\":3},{\"gridX\":132,\"gridY\":164,\"type\":2},{\"gridX\":100,\"gridY\":164,\"type\":0},{\"gridX\":108,\"gridY\":164,\"type\":0},{\"gridX\":116,\"gridY\":164,\"type\":0},{\"gridX\":124,\"gridY\":164,\"type\":0},{\"gridX\":144,\"gridY\":164,\"type\":0},{\"gridX\":152,\"gridY\":164,\"type\":0},{\"gridX\":160,\"gridY\":164,\"type\":0},{\"gridX\":168,\"gridY\":164,\"type\":0},{\"gridX\":576,\"gridY\":112,\"type\":2},{\"gridX\":484,\"gridY\":100,\"type\":2},{\"gridX\":1084,\"gridY\":80,\"type\":2},{\"gridX\":1251,\"gridY\":62,\"type\":2},{\"gridX\":1176,\"gridY\":108,\"type\":3},{\"gridX\":1144,\"gridY\":24,\"type\":2},{\"gridX\":1160,\"gridY\":24,\"type\":2}],\"jons\":[{\"gridX\":15,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":36,\"gridY\":92,\"type\":2},{\"gridX\":53,\"gridY\":92,\"type\":3},{\"gridX\":77,\"gridY\":92,\"type\":1},{\"gridX\":842,\"gridY\":92,\"type\":2},{\"gridX\":935,\"gridY\":85,\"type\":0},{\"gridX\":913,\"gridY\":92,\"type\":3},{\"gridX\":967,\"gridY\":92,\"type\":1},{\"gridX\":960,\"gridY\":92,\"type\":1},{\"gridX\":1028,\"gridY\":92,\"type\":3},{\"gridX\":1309,\"gridY\":92,\"type\":3},{\"gridX\":995,\"gridY\":92,\"type\":1}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level2(text.c_str());
}

void Chapter1Level2::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level2 * Chapter1Level2::getInstance()
{
    return s_pInstance;
}

Chapter1Level2::Chapter1Level2(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 3 ///

Chapter1Level3 * Chapter1Level3::s_pInstance = nullptr;

void Chapter1Level3::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":3,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":56,\"gridY\":96,\"type\":0},{\"gridX\":92,\"gridY\":96,\"type\":1},{\"gridX\":256,\"gridY\":96,\"type\":2},{\"gridX\":373,\"gridY\":96,\"type\":1},{\"gridX\":316,\"gridY\":96,\"type\":0},{\"gridX\":412,\"gridY\":96,\"type\":2},{\"gridX\":441,\"gridY\":96,\"type\":1},{\"gridX\":580,\"gridY\":96,\"type\":2},{\"gridX\":648,\"gridY\":96,\"type\":1},{\"gridX\":601,\"gridY\":96,\"type\":0},{\"gridX\":864,\"gridY\":96,\"type\":0},{\"gridX\":940,\"gridY\":96,\"type\":2},{\"gridX\":958,\"gridY\":96,\"type\":0},{\"gridX\":1011,\"gridY\":96,\"type\":1},{\"gridX\":1064,\"gridY\":96,\"type\":0},{\"gridX\":1140,\"gridY\":96,\"type\":0},{\"gridX\":1208,\"gridY\":96,\"type\":1},{\"gridX\":1260,\"gridY\":96,\"type\":2},{\"gridX\":1324,\"gridY\":96,\"type\":2},{\"gridX\":1384,\"gridY\":96,\"type\":0},{\"gridX\":1416,\"gridY\":96,\"type\":1},{\"gridX\":1496,\"gridY\":96,\"type\":1},{\"gridX\":1668,\"gridY\":96,\"type\":2},{\"gridX\":1915,\"gridY\":96,\"type\":1},{\"gridX\":1757,\"gridY\":96,\"type\":0},{\"gridX\":1824,\"gridY\":96,\"type\":2},{\"gridX\":1884,\"gridY\":96,\"type\":2},{\"gridX\":1944,\"gridY\":96,\"type\":2},{\"gridX\":2012,\"gridY\":96,\"type\":0},{\"gridX\":2056,\"gridY\":96,\"type\":2},{\"gridX\":2116,\"gridY\":96,\"type\":2},{\"gridX\":2176,\"gridY\":96,\"type\":1},{\"gridX\":2220,\"gridY\":96,\"type\":0},{\"gridX\":1460,\"gridY\":96,\"type\":0},{\"gridX\":285,\"gridY\":96,\"type\":15},{\"gridX\":434,\"gridY\":96,\"type\":17},{\"gridX\":445,\"gridY\":96,\"type\":18},{\"gridX\":434,\"gridY\":116,\"type\":17},{\"gridX\":422,\"gridY\":107,\"type\":18},{\"gridX\":518,\"gridY\":96,\"type\":17},{\"gridX\":507,\"gridY\":96,\"type\":18},{\"gridX\":524,\"gridY\":104,\"type\":18},{\"gridX\":432,\"gridY\":136,\"type\":17},{\"gridX\":441,\"gridY\":130,\"type\":18},{\"gridX\":736,\"gridY\":96,\"type\":17},{\"gridX\":835,\"gridY\":96,\"type\":17},{\"gridX\":744,\"gridY\":107,\"type\":18},{\"gridX\":751,\"gridY\":122,\"type\":18},{\"gridX\":761,\"gridY\":135,\"type\":17},{\"gridX\":781,\"gridY\":124,\"type\":18},{\"gridX\":844,\"gridY\":96,\"type\":18},{\"gridX\":823,\"gridY\":106,\"type\":18},{\"gridX\":810,\"gridY\":116,\"type\":17},{\"gridX\":797,\"gridY\":128,\"type\":18},{\"gridX\":767,\"gridY\":143,\"type\":18},{\"gridX\":800,\"gridY\":137,\"type\":17},{\"gridX\":1713,\"gridY\":96,\"type\":17},{\"gridX\":1718,\"gridY\":106,\"type\":18},{\"gridX\":1735,\"gridY\":118,\"type\":17},{\"gridX\":1720,\"gridY\":119,\"type\":18},{\"gridX\":1745,\"gridY\":127,\"type\":18},{\"gridX\":1756,\"gridY\":137,\"type\":18},{\"gridX\":1767,\"gridY\":137,\"type\":17},{\"gridX\":1781,\"gridY\":136,\"type\":18},{\"gridX\":1807,\"gridY\":96,\"type\":17},{\"gridX\":1793,\"gridY\":107,\"type\":18},{\"gridX\":1786,\"gridY\":122,\"type\":18},{\"gridX\":1747,\"gridY\":165,\"type\":18},{\"gridX\":1765,\"gridY\":146,\"type\":18},{\"gridX\":1754,\"gridY\":154,\"type\":18},{\"gridX\":28,\"gridY\":96,\"type\":15},{\"gridX\":551,\"gridY\":96,\"type\":16},{\"gridX\":1857,\"gridY\":96,\"type\":6},{\"gridX\":1744,\"gridY\":96,\"type\":6}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":28},{\"gridX\":2056,\"gridY\":0,\"type\":28},{\"gridX\":2184,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":2312,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":136,\"gridY\":0,\"type\":32},{\"gridX\":1534,\"gridY\":0,\"type\":32},{\"gridX\":1340,\"gridY\":0,\"type\":30},{\"gridX\":1364,\"gridY\":31,\"type\":34},{\"gridX\":1388,\"gridY\":31,\"type\":35},{\"gridX\":1420,\"gridY\":31,\"type\":35},{\"gridX\":1452,\"gridY\":31,\"type\":35},{\"gridX\":1484,\"gridY\":31,\"type\":35},{\"gridX\":1516,\"gridY\":31,\"type\":36}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":2056,\"gridY\":96,\"type\":12},{\"gridX\":916,\"gridY\":96,\"type\":10},{\"gridX\":1208,\"gridY\":96,\"type\":11},{\"gridX\":1328,\"gridY\":96,\"type\":8},{\"gridX\":1373,\"gridY\":96,\"type\":8},{\"gridX\":347,\"gridY\":96,\"type\":58},{\"gridX\":349,\"gridY\":119,\"type\":60},{\"gridX\":348,\"gridY\":105,\"type\":59},{\"gridX\":525,\"gridY\":114,\"type\":60},{\"gridX\":500,\"gridY\":114,\"type\":60},{\"gridX\":516,\"gridY\":111,\"type\":59},{\"gridX\":423,\"gridY\":153,\"type\":60},{\"gridX\":440,\"gridY\":153,\"type\":60},{\"gridX\":434,\"gridY\":145,\"type\":59},{\"gridX\":447,\"gridY\":140,\"type\":59},{\"gridX\":414,\"gridY\":96,\"type\":58},{\"gridX\":732,\"gridY\":115,\"type\":61},{\"gridX\":833,\"gridY\":115,\"type\":61},{\"gridX\":775,\"gridY\":145,\"type\":60},{\"gridX\":798,\"gridY\":145,\"type\":60},{\"gridX\":778,\"gridY\":131,\"type\":59},{\"gridX\":789,\"gridY\":140,\"type\":59},{\"gridX\":894,\"gridY\":96,\"type\":54},{\"gridX\":887,\"gridY\":96,\"type\":53},{\"gridX\":906,\"gridY\":96,\"type\":56},{\"gridX\":1816,\"gridY\":96,\"type\":58},{\"gridX\":1809,\"gridY\":98,\"type\":60},{\"gridX\":1728,\"gridY\":115,\"type\":61},{\"gridX\":1740,\"gridY\":167,\"type\":60},{\"gridX\":1810,\"gridY\":96,\"type\":56},{\"gridX\":1802,\"gridY\":96,\"type\":53},{\"gridX\":1523,\"gridY\":96,\"type\":41},{\"gridX\":1561,\"gridY\":7,\"type\":50},{\"gridX\":1563,\"gridY\":10,\"type\":21}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":2024,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":400,\"gridY\":96,\"type\":8},{\"gridX\":488,\"gridY\":96,\"type\":8},{\"gridX\":592,\"gridY\":96,\"type\":8},{\"gridX\":800,\"gridY\":96,\"type\":8},{\"gridX\":1129,\"gridY\":96,\"type\":9},{\"gridX\":1168,\"gridY\":96,\"type\":9},{\"gridX\":1744,\"gridY\":121,\"type\":8},{\"gridX\":1812,\"gridY\":121,\"type\":9},{\"gridX\":886,\"gridY\":208,\"type\":3}],\"collectibles\":[{\"gridX\":144,\"gridY\":112,\"type\":0},{\"gridX\":148,\"gridY\":120,\"type\":0},{\"gridX\":152,\"gridY\":128,\"type\":0},{\"gridX\":202,\"gridY\":109,\"type\":0},{\"gridX\":212,\"gridY\":120,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":474,\"gridY\":227,\"type\":1},{\"gridX\":344,\"gridY\":148,\"type\":0},{\"gridX\":352,\"gridY\":148,\"type\":0},{\"gridX\":360,\"gridY\":148,\"type\":0},{\"gridX\":640,\"gridY\":124,\"type\":0},{\"gridX\":536,\"gridY\":144,\"type\":0},{\"gridX\":544,\"gridY\":144,\"type\":0},{\"gridX\":560,\"gridY\":136,\"type\":0},{\"gridX\":568,\"gridY\":128,\"type\":0},{\"gridX\":576,\"gridY\":120,\"type\":0},{\"gridX\":632,\"gridY\":116,\"type\":0},{\"gridX\":648,\"gridY\":128,\"type\":0},{\"gridX\":664,\"gridY\":116,\"type\":0},{\"gridX\":584,\"gridY\":112,\"type\":0},{\"gridX\":656,\"gridY\":124,\"type\":0},{\"gridX\":452,\"gridY\":184,\"type\":0},{\"gridX\":436,\"gridY\":184,\"type\":0},{\"gridX\":444,\"gridY\":184,\"type\":0},{\"gridX\":368,\"gridY\":148,\"type\":0},{\"gridX\":456,\"gridY\":192,\"type\":0},{\"gridX\":553,\"gridY\":144,\"type\":0},{\"gridX\":220,\"gridY\":128,\"type\":0},{\"gridX\":244,\"gridY\":128,\"type\":0},{\"gridX\":688,\"gridY\":104,\"type\":0},{\"gridX\":696,\"gridY\":104,\"type\":0},{\"gridX\":704,\"gridY\":104,\"type\":0},{\"gridX\":732,\"gridY\":128,\"type\":0},{\"gridX\":748,\"gridY\":128,\"type\":0},{\"gridX\":740,\"gridY\":128,\"type\":0},{\"gridX\":756,\"gridY\":128,\"type\":0},{\"gridX\":832,\"gridY\":128,\"type\":0},{\"gridX\":840,\"gridY\":128,\"type\":0},{\"gridX\":848,\"gridY\":128,\"type\":0},{\"gridX\":856,\"gridY\":128,\"type\":0}," << "{\"gridX\":932,\"gridY\":132,\"type\":0},{\"gridX\":936,\"gridY\":140,\"type\":0},{\"gridX\":940,\"gridY\":148,\"type\":0},{\"gridX\":944,\"gridY\":156,\"type\":0},{\"gridX\":975,\"gridY\":172,\"type\":1},{\"gridX\":460,\"gridY\":200,\"type\":0},{\"gridX\":1024,\"gridY\":104,\"type\":0},{\"gridX\":1032,\"gridY\":104,\"type\":0},{\"gridX\":1040,\"gridY\":104,\"type\":0},{\"gridX\":1100,\"gridY\":120,\"type\":0},{\"gridX\":1104,\"gridY\":112,\"type\":0},{\"gridX\":1108,\"gridY\":104,\"type\":0},{\"gridX\":1120,\"gridY\":104,\"type\":0},{\"gridX\":1128,\"gridY\":112,\"type\":0},{\"gridX\":1136,\"gridY\":116,\"type\":0},{\"gridX\":1144,\"gridY\":112,\"type\":0},{\"gridX\":1152,\"gridY\":104,\"type\":0},{\"gridX\":1096,\"gridY\":128,\"type\":0},{\"gridX\":1172,\"gridY\":120,\"type\":0},{\"gridX\":1168,\"gridY\":112,\"type\":0},{\"gridX\":1164,\"gridY\":104,\"type\":0},{\"gridX\":1180,\"gridY\":124,\"type\":0},{\"gridX\":1188,\"gridY\":128,\"type\":0},{\"gridX\":1256,\"gridY\":104,\"type\":0},{\"gridX\":1264,\"gridY\":104,\"type\":0},{\"gridX\":1272,\"gridY\":104,\"type\":0},{\"gridX\":1328,\"gridY\":120,\"type\":0},{\"gridX\":1321,\"gridY\":111,\"type\":0},{\"gridX\":1337,\"gridY\":128,\"type\":0},{\"gridX\":1349,\"gridY\":133,\"type\":0},{\"gridX\":1372,\"gridY\":131,\"type\":0},{\"gridX\":1361,\"gridY\":134,\"type\":0},{\"gridX\":1380,\"gridY\":125,\"type\":0},{\"gridX\":1444,\"gridY\":104,\"type\":0},{\"gridX\":1460,\"gridY\":104,\"type\":0},{\"gridX\":1452,\"gridY\":104,\"type\":0},{\"gridX\":1528,\"gridY\":114,\"type\":0},{\"gridX\":1607,\"gridY\":133,\"type\":0},{\"gridX\":1526,\"gridY\":104,\"type\":0},{\"gridX\":1712,\"gridY\":104,\"type\":0},{\"gridX\":1584,\"gridY\":158,\"type\":0},{\"gridX\":1544,\"gridY\":147,\"type\":0},{\"gridX\":1536,\"gridY\":136,\"type\":0},{\"gridX\":1595,\"gridY\":152,\"type\":0},{\"gridX\":1570,\"gridY\":159,\"type\":0},{\"gridX\":1612,\"gridY\":119,\"type\":1},{\"gridX\":1532,\"gridY\":125,\"type\":0},{\"gridX\":1602,\"gridY\":144,\"type\":0},{\"gridX\":1720,\"gridY\":104,\"type\":0},{\"gridX\":1728,\"gridY\":104,\"type\":0},{\"gridX\":1556,\"gridY\":154,\"type\":0},{\"gridX\":1816,\"gridY\":124,\"type\":0},{\"gridX\":1824,\"gridY\":124,\"type\":0},{\"gridX\":1832,\"gridY\":124,\"type\":0},{\"gridX\":1988,\"gridY\":104,\"type\":0},{\"gridX\":1996,\"gridY\":104,\"type\":0},{\"gridX\":1980,\"gridY\":104,\"type\":0},{\"gridX\":1704,\"gridY\":104,\"type\":0},{\"gridX\":1696,\"gridY\":104,\"type\":0},{\"gridX\":1688,\"gridY\":104,\"type\":0},{\"gridX\":1916,\"gridY\":104,\"type\":0},{\"gridX\":1924,\"gridY\":104,\"type\":0},{\"gridX\":1932,\"gridY\":104,\"type\":0},{\"gridX\":1392,\"gridY\":44,\"type\":2},{\"gridX\":1758,\"gridY\":199,\"type\":2},{\"gridX\":1508,\"gridY\":44,\"type\":2},{\"gridX\":1480,\"gridY\":44,\"type\":2},{\"gridX\":1454,\"gridY\":47,\"type\":3},{\"gridX\":935,\"gridY\":243,\"type\":3},{\"gridX\":1420,\"gridY\":44,\"type\":2},{\"gridX\":1746,\"gridY\":199,\"type\":2}],\"jons\":[{\"gridX\":19,\"gridY\":95}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":937,\"gridY\":85,\"type\":0},{\"gridX\":710,\"gridY\":92,\"type\":2},{\"gridX\":823,\"gridY\":92,\"type\":1},{\"gridX\":779,\"gridY\":92,\"type\":3},{\"gridX\":40,\"gridY\":92,\"type\":3},{\"gridX\":523,\"gridY\":92,\"type\":2},{\"gridX\":501,\"gridY\":92,\"type\":1},{\"gridX\":505,\"gridY\":92,\"type\":3}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level3(text.c_str());
}

void Chapter1Level3::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level3 * Chapter1Level3::getInstance()
{
    return s_pInstance;
}

Chapter1Level3::Chapter1Level3(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 4 ///

Chapter1Level4 * Chapter1Level4::s_pInstance = nullptr;

void Chapter1Level4::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":4,\"midgrounds\":[{\"gridX\":276,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1},{\"gridX\":428,\"gridY\":96,\"type\":0},{\"gridX\":716,\"gridY\":96,\"type\":1},{\"gridX\":549,\"gridY\":96,\"type\":0},{\"gridX\":537,\"gridY\":96,\"type\":2},{\"gridX\":1220,\"gridY\":96,\"type\":1},{\"gridX\":1264,\"gridY\":96,\"type\":0},{\"gridX\":1543,\"gridY\":96,\"type\":2},{\"gridX\":1444,\"gridY\":96,\"type\":1},{\"gridX\":94,\"gridY\":96,\"type\":2},{\"gridX\":1,\"gridY\":96,\"type\":0},{\"gridX\":100,\"gridY\":96,\"type\":0},{\"gridX\":2048,\"gridY\":96,\"type\":2},{\"gridX\":2108,\"gridY\":96,\"type\":1},{\"gridX\":2152,\"gridY\":96,\"type\":0},{\"gridX\":2188,\"gridY\":96,\"type\":2},{\"gridX\":1956,\"gridY\":96,\"type\":0},{\"gridX\":464,\"gridY\":96,\"type\":1},{\"gridX\":664,\"gridY\":96,\"type\":0},{\"gridX\":776,\"gridY\":96,\"type\":0},{\"gridX\":836,\"gridY\":96,\"type\":1},{\"gridX\":920,\"gridY\":96,\"type\":2},{\"gridX\":1084,\"gridY\":96,\"type\":2},{\"gridX\":1512,\"gridY\":96,\"type\":0},{\"gridX\":1572,\"gridY\":96,\"type\":0},{\"gridX\":2012,\"gridY\":96,\"type\":0},{\"gridX\":1000,\"gridY\":96,\"type\":1},{\"gridX\":1106,\"gridY\":96,\"type\":0},{\"gridX\":1602,\"gridY\":96,\"type\":11},{\"gridX\":961,\"gridY\":96,\"type\":15},{\"gridX\":985,\"gridY\":96,\"type\":16},{\"gridX\":369,\"gridY\":96,\"type\":13},{\"gridX\":77,\"gridY\":96,\"type\":16},{\"gridX\":55,\"gridY\":96,\"type\":15}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":27},{\"gridX\":1608,\"gridY\":0,\"type\":27},{\"gridX\":1928,\"gridY\":0,\"type\":27},{\"gridX\":72,\"gridY\":0,\"type\":28},{\"gridX\":200,\"gridY\":0,\"type\":28},{\"gridX\":328,\"gridY\":0,\"type\":28},{\"gridX\":456,\"gridY\":0,\"type\":28},{\"gridX\":584,\"gridY\":0,\"type\":28},{\"gridX\":712,\"gridY\":0,\"type\":28},{\"gridX\":840,\"gridY\":0,\"type\":28},{\"gridX\":968,\"gridY\":0,\"type\":28},{\"gridX\":1096,\"gridY\":0,\"type\":28},{\"gridX\":1224,\"gridY\":0,\"type\":28},{\"gridX\":1352,\"gridY\":0,\"type\":28},{\"gridX\":1480,\"gridY\":0,\"type\":28},{\"gridX\":1992,\"gridY\":0,\"type\":28},{\"gridX\":2120,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":2248,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":1064,\"gridY\":0,\"type\":30},{\"gridX\":1159,\"gridY\":0,\"type\":31},{\"gridX\":1320,\"gridY\":0,\"type\":32},{\"gridX\":132,\"gridY\":0,\"type\":32},{\"gridX\":1701,\"gridY\":0,\"type\":33},{\"gridX\":1088,\"gridY\":30,\"type\":34},{\"gridX\":1112,\"gridY\":30,\"type\":35},{\"gridX\":1144,\"gridY\":30,\"type\":36},{\"gridX\":1216,\"gridY\":36,\"type\":34},{\"gridX\":1240,\"gridY\":36,\"type\":35},{\"gridX\":1272,\"gridY\":36,\"type\":35},{\"gridX\":1303,\"gridY\":36,\"type\":36},{\"gridX\":1439,\"gridY\":31,\"type\":34},{\"gridX\":1463,\"gridY\":31,\"type\":35},{\"gridX\":1495,\"gridY\":31,\"type\":35},{\"gridX\":1527,\"gridY\":31,\"type\":35},{\"gridX\":1559,\"gridY\":31,\"type\":35},{\"gridX\":1591,\"gridY\":31,\"type\":35},{\"gridX\":1623,\"gridY\":31,\"type\":35},{\"gridX\":1655,\"gridY\":31,\"type\":35},{\"gridX\":1687,\"gridY\":31,\"type\":36}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":2016,\"gridY\":96,\"type\":12},{\"gridX\":336,\"gridY\":96,\"type\":10},{\"gridX\":416,\"gridY\":96,\"type\":8},{\"gridX\":851,\"gridY\":96,\"type\":24},{\"gridX\":1492,\"gridY\":96,\"type\":24},{\"gridX\":1567,\"gridY\":96,\"type\":23},{\"gridX\":1624,\"gridY\":96,\"type\":22},{\"gridX\":560,\"gridY\":96,\"type\":24},{\"gridX\":528,\"gridY\":96,\"type\":24},{\"gridX\":915,\"gridY\":96,\"type\":24},{\"gridX\":947,\"gridY\":96,\"type\":24},{\"gridX\":978,\"gridY\":96,\"type\":23},{\"gridX\":605,\"gridY\":96,\"type\":51},{\"gridX\":621,\"gridY\":96,\"type\":51},{\"gridX\":637,\"gridY\":96,\"type\":51},{\"gridX\":687,\"gridY\":128,\"type\":51},{\"gridX\":687,\"gridY\":96,\"type\":52},{\"gridX\":687,\"gridY\":112,\"type\":52},{\"gridX\":826,\"gridY\":96,\"type\":52},{\"gridX\":826,\"gridY\":111,\"type\":52},{\"gridX\":810,\"gridY\":127,\"type\":50},{\"gridX\":826,\"gridY\":127,\"type\":50},{\"gridX\":721,\"gridY\":96,\"type\":24},{\"gridX\":753,\"gridY\":96,\"type\":24},{\"gridX\":785,\"gridY\":96,\"type\":24},{\"gridX\":818,\"gridY\":96,\"type\":24},{\"gridX\":688,\"gridY\":96,\"type\":24},{\"gridX\":592,\"gridY\":96,\"type\":24},{\"gridX\":625,\"gridY\":96,\"type\":24},{\"gridX\":656,\"gridY\":96,\"type\":24},{\"gridX\":508,\"gridY\":96,\"type\":52},{\"gridX\":508,\"gridY\":112,\"type\":51},{\"gridX\":492,\"gridY\":124,\"type\":50},{\"gridX\":481,\"gridY\":96,\"type\":53},{\"gridX\":488,\"gridY\":96,\"type\":54},{\"gridX\":498,\"gridY\":96,\"type\":56},{\"gridX\":881,\"gridY\":96,\"type\":52},{\"gridX\":897,\"gridY\":96,\"type\":52},{\"gridX\":881,\"gridY\":112,\"type\":50},{\"gridX\":897,\"gridY\":112,\"type\":50},{\"gridX\":1736,\"gridY\":0,\"type\":52},{\"gridX\":1736,\"gridY\":80,\"type\":52},{\"gridX\":1736,\"gridY\":96,\"type\":52},{\"gridX\":1736,\"gridY\":112,\"type\":51},{\"gridX\":1800,\"gridY\":0,\"type\":52},{\"gridX\":1800,\"gridY\":16,\"type\":52},{\"gridX\":1800,\"gridY\":64,\"type\":52},{\"gridX\":1800,\"gridY\":80,\"type\":52},{\"gridX\":1800,\"gridY\":96,\"type\":52},{\"gridX\":1800,\"gridY\":112,\"type\":52},{\"gridX\":1800,\"gridY\":128,\"type\":52},{\"gridX\":1800,\"gridY\":144,\"type\":52},{\"gridX\":1796,\"gridY\":158,\"type\":50},{\"gridX\":1888,\"gridY\":0,\"type\":52},{\"gridX\":1888,\"gridY\":16,\"type\":52},{\"gridX\":1888,\"gridY\":32,\"type\":52},{\"gridX\":1888,\"gridY\":96,\"type\":52},{\"gridX\":1888,\"gridY\":128,\"type\":52},{\"gridX\":1888,\"gridY\":112,\"type\":52},{\"gridX\":1888,\"gridY\":144,\"type\":52},{\"gridX\":1812,\"gridY\":158,\"type\":50},{\"gridX\":1828,\"gridY\":158,\"type\":50},{\"gridX\":1876,\"gridY\":158,\"type\":50},{\"gridX\":1860,\"gridY\":158,\"type\":50},{\"gridX\":1844,\"gridY\":158,\"type\":50},{\"gridX\":1892,\"gridY\":158,\"type\":50},{\"gridX\":1320,\"gridY\":40,\"type\":50},{\"gridX\":1736,\"gridY\":16,\"type\":50},{\"gridX\":1792,\"gridY\":32,\"type\":50},{\"gridX\":1808,\"gridY\":32,\"type\":50},{\"gridX\":1880,\"gridY\":48,\"type\":50},{\"gridX\":1896,\"gridY\":48,\"type\":50}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1984,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":1712,\"gridY\":116,\"type\":3}],\"collectibles\":[{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":256,\"gridY\":108,\"type\":0},{\"gridX\":312,\"gridY\":104,\"type\":0},{\"gridX\":320,\"gridY\":112,\"type\":0},{\"gridX\":328,\"gridY\":120,\"type\":0},{\"gridX\":336,\"gridY\":128,\"type\":0},{\"gridX\":344,\"gridY\":128,\"type\":0},{\"gridX\":352,\"gridY\":128,\"type\":0},{\"gridX\":416,\"gridY\":136,\"type\":0},{\"gridX\":408,\"gridY\":128,\"type\":0},{\"gridX\":424,\"gridY\":136,\"type\":0},{\"gridX\":400,\"gridY\":120,\"type\":0},{\"gridX\":416,\"gridY\":128,\"type\":0},{\"gridX\":590,\"gridY\":150,\"type\":0},{\"gridX\":572,\"gridY\":162,\"type\":0},{\"gridX\":529,\"gridY\":147,\"type\":0},{\"gridX\":561,\"gridY\":165,\"type\":1},{\"gridX\":408,\"gridY\":120,\"type\":0},{\"gridX\":536,\"gridY\":158,\"type\":0},{\"gridX\":581,\"gridY\":156,\"type\":0},{\"gridX\":624,\"gridY\":116,\"type\":0},{\"gridX\":632,\"gridY\":116,\"type\":0},{\"gridX\":640,\"gridY\":116,\"type\":0},{\"gridX\":648,\"gridY\":124,\"type\":0},{\"gridX\":616,\"gridY\":116,\"type\":0},{\"gridX\":688,\"gridY\":148,\"type\":0},{\"gridX\":696,\"gridY\":148,\"type\":0},{\"gridX\":694,\"gridY\":155,\"type\":0},{\"gridX\":824,\"gridY\":140,\"type\":0},{\"gridX\":828,\"gridY\":148,\"type\":0},{\"gridX\":816,\"gridY\":140,\"type\":0},{\"gridX\":912,\"gridY\":132,\"type\":0},{\"gridX\":897,\"gridY\":121,\"type\":0}," << "{\"gridX\":906,\"gridY\":121,\"type\":0},{\"gridX\":748,\"gridY\":164,\"type\":0},{\"gridX\":740,\"gridY\":168,\"type\":0},{\"gridX\":756,\"gridY\":160,\"type\":0},{\"gridX\":1076,\"gridY\":95,\"type\":1},{\"gridX\":1064,\"gridY\":104,\"type\":0},{\"gridX\":1032,\"gridY\":104,\"type\":0},{\"gridX\":1040,\"gridY\":104,\"type\":0},{\"gridX\":1048,\"gridY\":104,\"type\":0},{\"gridX\":1056,\"gridY\":104,\"type\":0},{\"gridX\":1120,\"gridY\":104,\"type\":0},{\"gridX\":1128,\"gridY\":104,\"type\":0},{\"gridX\":1136,\"gridY\":104,\"type\":0},{\"gridX\":1154,\"gridY\":105,\"type\":0},{\"gridX\":1157,\"gridY\":114,\"type\":0},{\"gridX\":1160,\"gridY\":122,\"type\":0},{\"gridX\":1165,\"gridY\":129,\"type\":0},{\"gridX\":1173,\"gridY\":136,\"type\":0},{\"gridX\":1392,\"gridY\":122,\"type\":0},{\"gridX\":1403,\"gridY\":131,\"type\":0},{\"gridX\":1415,\"gridY\":137,\"type\":0},{\"gridX\":1248,\"gridY\":104,\"type\":0},{\"gridX\":1256,\"gridY\":104,\"type\":0},{\"gridX\":1264,\"gridY\":104,\"type\":0},{\"gridX\":1304,\"gridY\":104,\"type\":0},{\"gridX\":1288,\"gridY\":104,\"type\":0},{\"gridX\":1296,\"gridY\":104,\"type\":0},{\"gridX\":1536,\"gridY\":104,\"type\":0},{\"gridX\":1532,\"gridY\":108,\"type\":0},{\"gridX\":1592,\"gridY\":108,\"type\":0},{\"gridX\":1596,\"gridY\":104,\"type\":0},{\"gridX\":1500,\"gridY\":124,\"type\":0},{\"gridX\":1508,\"gridY\":124,\"type\":0},{\"gridX\":1504,\"gridY\":128,\"type\":0},{\"gridX\":1560,\"gridY\":124,\"type\":0},{\"gridX\":1568,\"gridY\":124,\"type\":0},{\"gridX\":1564,\"gridY\":128,\"type\":0},{\"gridX\":1620,\"gridY\":124,\"type\":0},{\"gridX\":1636,\"gridY\":132,\"type\":0},{\"gridX\":1628,\"gridY\":128,\"type\":0},{\"gridX\":1737,\"gridY\":136,\"type\":0},{\"gridX\":1745,\"gridY\":136,\"type\":0},{\"gridX\":1812,\"gridY\":164,\"type\":0},{\"gridX\":1820,\"gridY\":164,\"type\":0},{\"gridX\":1828,\"gridY\":164,\"type\":0},{\"gridX\":1992,\"gridY\":128,\"type\":0},{\"gridX\":1836,\"gridY\":164,\"type\":0},{\"gridX\":1844,\"gridY\":164,\"type\":0},{\"gridX\":1852,\"gridY\":164,\"type\":0},{\"gridX\":2000,\"gridY\":124,\"type\":0},{\"gridX\":1750,\"gridY\":145,\"type\":0},{\"gridX\":1860,\"gridY\":164,\"type\":0},{\"gridX\":1868,\"gridY\":164,\"type\":0},{\"gridX\":1876,\"gridY\":164,\"type\":0},{\"gridX\":1952,\"gridY\":124,\"type\":0},{\"gridX\":1984,\"gridY\":132,\"type\":0},{\"gridX\":1912,\"gridY\":164,\"type\":0},{\"gridX\":1920,\"gridY\":156,\"type\":0},{\"gridX\":1928,\"gridY\":148,\"type\":0},{\"gridX\":1936,\"gridY\":140,\"type\":0},{\"gridX\":1944,\"gridY\":132,\"type\":0},{\"gridX\":1976,\"gridY\":136,\"type\":1},{\"gridX\":547,\"gridY\":164,\"type\":0},{\"gridX\":522,\"gridY\":137,\"type\":0},{\"gridX\":1540,\"gridY\":108,\"type\":0},{\"gridX\":1600,\"gridY\":108,\"type\":0},{\"gridX\":1846,\"gridY\":70,\"type\":3},{\"gridX\":1415,\"gridY\":27,\"type\":3},{\"gridX\":1103,\"gridY\":43,\"type\":2},{\"gridX\":1122,\"gridY\":43,\"type\":2},{\"gridX\":1141,\"gridY\":43,\"type\":2},{\"gridX\":1284,\"gridY\":50,\"type\":2},{\"gridX\":1251,\"gridY\":50,\"type\":2},{\"gridX\":1266,\"gridY\":49,\"type\":2}],\"jons\":[{\"gridX\":28,\"gridY\":100}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":334,\"gridY\":92,\"type\":3},{\"gridX\":423,\"gridY\":92,\"type\":1},{\"gridX\":1025,\"gridY\":85,\"type\":0},{\"gridX\":1112,\"gridY\":92,\"type\":2}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level4(text.c_str());
}

void Chapter1Level4::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level4 * Chapter1Level4::getInstance()
{
    return s_pInstance;
}

Chapter1Level4::Chapter1Level4(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 5 ///

Chapter1Level5 * Chapter1Level5::s_pInstance = nullptr;

void Chapter1Level5::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":5,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":60,\"gridY\":96,\"type\":0},{\"gridX\":92,\"gridY\":96,\"type\":2},{\"gridX\":212,\"gridY\":96,\"type\":0},{\"gridX\":244,\"gridY\":96,\"type\":1},{\"gridX\":540,\"gridY\":96,\"type\":2},{\"gridX\":612,\"gridY\":96,\"type\":1},{\"gridX\":672,\"gridY\":96,\"type\":0},{\"gridX\":736,\"gridY\":96,\"type\":2},{\"gridX\":832,\"gridY\":96,\"type\":0},{\"gridX\":876,\"gridY\":96,\"type\":1},{\"gridX\":916,\"gridY\":96,\"type\":2},{\"gridX\":1196,\"gridY\":96,\"type\":2},{\"gridX\":1252,\"gridY\":96,\"type\":1},{\"gridX\":1300,\"gridY\":96,\"type\":2},{\"gridX\":1376,\"gridY\":96,\"type\":0},{\"gridX\":1428,\"gridY\":96,\"type\":0},{\"gridX\":1488,\"gridY\":96,\"type\":2},{\"gridX\":1584,\"gridY\":96,\"type\":1},{\"gridX\":1548,\"gridY\":96,\"type\":0},{\"gridX\":1960,\"gridY\":96,\"type\":2},{\"gridX\":2124,\"gridY\":96,\"type\":0},{\"gridX\":2160,\"gridY\":96,\"type\":0},{\"gridX\":564,\"gridY\":96,\"type\":0},{\"gridX\":724,\"gridY\":96,\"type\":17},{\"gridX\":725,\"gridY\":112,\"type\":18},{\"gridX\":733,\"gridY\":114,\"type\":18},{\"gridX\":754,\"gridY\":122,\"type\":17},{\"gridX\":743,\"gridY\":122,\"type\":18},{\"gridX\":782,\"gridY\":140,\"type\":17},{\"gridX\":770,\"gridY\":132,\"type\":18},{\"gridX\":793,\"gridY\":148,\"type\":18},{\"gridX\":763,\"gridY\":121,\"type\":18},{\"gridX\":805,\"gridY\":96,\"type\":17},{\"gridX\":797,\"gridY\":102,\"type\":18},{\"gridX\":809,\"gridY\":111,\"type\":18},{\"gridX\":816,\"gridY\":117,\"type\":18},{\"gridX\":826,\"gridY\":119,\"type\":17},{\"gridX\":844,\"gridY\":111,\"type\":18},{\"gridX\":843,\"gridY\":133,\"type\":17},{\"gridX\":833,\"gridY\":131,\"type\":18},{\"gridX\":810,\"gridY\":153,\"type\":18},{\"gridX\":822,\"gridY\":143,\"type\":18},{\"gridX\":908,\"gridY\":126,\"type\":17},{\"gridX\":898,\"gridY\":111,\"type\":17},{\"gridX\":888,\"gridY\":123,\"type\":18},{\"gridX\":908,\"gridY\":109,\"type\":18},{\"gridX\":897,\"gridY\":96,\"type\":17},{\"gridX\":865,\"gridY\":136,\"type\":17},{\"gridX\":887,\"gridY\":136,\"type\":17},{\"gridX\":884,\"gridY\":144,\"type\":18},{\"gridX\":917,\"gridY\":96,\"type\":17},{\"gridX\":922,\"gridY\":113,\"type\":18},{\"gridX\":875,\"gridY\":96,\"type\":17},{\"gridX\":881,\"gridY\":108,\"type\":18},{\"gridX\":860,\"gridY\":104,\"type\":18},{\"gridX\":799,\"gridY\":162,\"type\":18},{\"gridX\":1543,\"gridY\":96,\"type\":17},{\"gridX\":1538,\"gridY\":108,\"type\":18},{\"gridX\":1549,\"gridY\":121,\"type\":17},{\"gridX\":1545,\"gridY\":96,\"type\":18},{\"gridX\":1553,\"gridY\":109,\"type\":18},{\"gridX\":1566,\"gridY\":122,\"type\":17},{\"gridX\":1558,\"gridY\":131,\"type\":18},{\"gridX\":1581,\"gridY\":117,\"type\":18},{\"gridX\":1591,\"gridY\":108,\"type\":17},{\"gridX\":1592,\"gridY\":96,\"type\":17},{\"gridX\":1582,\"gridY\":101,\"type\":18},{\"gridX\":1600,\"gridY\":112,\"type\":18},{\"gridX\":1608,\"gridY\":122,\"type\":18},{\"gridX\":1617,\"gridY\":130,\"type\":18},{\"gridX\":1620,\"gridY\":118,\"type\":18},{\"gridX\":1546,\"gridY\":138,\"type\":18},{\"gridX\":1873,\"gridY\":96,\"type\":17},{\"gridX\":1863,\"gridY\":112,\"type\":18},{\"gridX\":1880,\"gridY\":114,\"type\":17},{\"gridX\":1868,\"gridY\":128,\"type\":18},{\"gridX\":1893,\"gridY\":96,\"type\":17},{\"gridX\":1898,\"gridY\":113,\"type\":18},{\"gridX\":1884,\"gridY\":129,\"type\":18},{\"gridX\":1875,\"gridY\":141,\"type\":18},{\"gridX\":1899,\"gridY\":125,\"type\":18},{\"gridX\":1893,\"gridY\":138,\"type\":18},{\"gridX\":761,\"gridY\":135,\"type\":17},{\"gridX\":1913,\"gridY\":96,\"type\":17},{\"gridX\":1933,\"gridY\":97,\"type\":17},{\"gridX\":1955,\"gridY\":96,\"type\":17},{\"gridX\":1976,\"gridY\":96,\"type\":17},{\"gridX\":1998,\"gridY\":96,\"type\":17},{\"gridX\":2017,\"gridY\":96,\"type\":17},{\"gridX\":1935,\"gridY\":118,\"type\":17},{\"gridX\":1924,\"gridY\":198,\"type\":17},{\"gridX\":1955,\"gridY\":118,\"type\":17},{\"gridX\":1976,\"gridY\":118,\"type\":17},{\"gridX\":1997,\"gridY\":118,\"type\":17},{\"gridX\":2018,\"gridY\":118,\"type\":17},{\"gridX\":2038,\"gridY\":96,\"type\":17},{\"gridX\":2038,\"gridY\":118,\"type\":17},{\"gridX\":2059,\"gridY\":96,\"type\":17},{\"gridX\":2059,\"gridY\":118,\"type\":17},{\"gridX\":1865,\"gridY\":96,\"type\":6},{\"gridX\":1893,\"gridY\":96,\"type\":6},{\"gridX\":619,\"gridY\":96,\"type\":16},{\"gridX\":210,\"gridY\":96,\"type\":16},{\"gridX\":49,\"gridY\":96,\"type\":15},{\"gridX\":170,\"gridY\":96,\"type\":14},{\"gridX\":395,\"gridY\":96,\"type\":13},{\"gridX\":469,\"gridY\":96,\"type\":14},{\"gridX\":492,\"gridY\":96,\"type\":13},{\"gridX\":1898,\"gridY\":140,\"type\":18},{\"gridX\":1941,\"gridY\":132,\"type\":18},{\"gridX\":1942,\"gridY\":193,\"type\":17},{\"gridX\":1943,\"gridY\":142,\"type\":18},{\"gridX\":1726,\"gridY\":101,\"type\":18},{\"gridX\":1714,\"gridY\":96,\"type\":17},{\"gridX\":1720,\"gridY\":112,\"type\":17},{\"gridX\":1705,\"gridY\":96,\"type\":18},{\"gridX\":1701,\"gridY\":140,\"type\":18},{\"gridX\":1714,\"gridY\":128,\"type\":17},{\"gridX\":1727,\"gridY\":144,\"type\":18},{\"gridX\":1732,\"gridY\":128,\"type\":18},{\"gridX\":1736,\"gridY\":142,\"type\":17},{\"gridX\":1851,\"gridY\":138,\"type\":17},{\"gridX\":1789,\"gridY\":158,\"type\":18},{\"gridX\":1806,\"gridY\":143,\"type\":18},{\"gridX\":1795,\"gridY\":138,\"type\":17},{\"gridX\":1835,\"gridY\":137,\"type\":18},{\"gridX\":1832,\"gridY\":148,\"type\":18},{\"gridX\":1813,\"gridY\":152,\"type\":18},{\"gridX\":1823,\"gridY\":142,\"type\":18},{\"gridX\":1779,\"gridY\":145,\"type\":17},{\"gridX\":1765,\"gridY\":149,\"type\":18},{\"gridX\":1746,\"gridY\":144,\"type\":18},{\"gridX\":1755,\"gridY\":153,\"type\":18},{\"gridX\":1795,\"gridY\":124,\"type\":18},{\"gridX\":1800,\"gridY\":103,\"type\":18},{\"gridX\":1795,\"gridY\":111,\"type\":18},{\"gridX\":1799,\"gridY\":96,\"type\":17},{\"gridX\":1736,\"gridY\":154,\"type\":18},{\"gridX\":1886,\"gridY\":178,\"type\":17},{\"gridX\":1907,\"gridY\":178,\"type\":17},{\"gridX\":1929,\"gridY\":178,\"type\":17},{\"gridX\":1893,\"gridY\":156,\"type\":17},{\"gridX\":1931,\"gridY\":156,\"type\":17},{\"gridX\":1912,\"gridY\":161,\"type\":17},{\"gridX\":1902,\"gridY\":198,\"type\":17},{\"gridX\":1924,\"gridY\":216,\"type\":17},{\"gridX\":1882,\"gridY\":189,\"type\":18},{\"gridX\":1889,\"gridY\":201,\"type\":18},{\"gridX\":1884,\"gridY\":214,\"type\":18},{\"gridX\":1902,\"gridY\":216,\"type\":17},{\"gridX\":1887,\"gridY\":226,\"type\":18}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":648,\"gridY\":0,\"type\":26},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":680,\"gridY\":0,\"type\":28},{\"gridX\":808,\"gridY\":0,\"type\":28},{\"gridX\":1832,\"gridY\":0,\"type\":28},{\"gridX\":1960,\"gridY\":0,\"type\":28},{\"gridX\":2088,\"gridY\":0,\"type\":28},{\"gridX\":936,\"gridY\":0,\"type\":28},{\"gridX\":1064,\"gridY\":0,\"type\":28},{\"gridX\":1192,\"gridY\":0,\"type\":28},{\"gridX\":1320,\"gridY\":0,\"type\":28},{\"gridX\":1448,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":1576,\"gridY\":0,\"type\":28},{\"gridX\":1704,\"gridY\":0,\"type\":28},{\"gridX\":2216,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":292,\"gridY\":0,\"type\":33},{\"gridX\":954,\"gridY\":0,\"type\":33},{\"gridX\":1405,\"gridY\":0,\"type\":30},{\"gridX\":1629,\"gridY\":0,\"type\":33},{\"gridX\":1429,\"gridY\":37,\"type\":34},{\"gridX\":1453,\"gridY\":37,\"type\":35},{\"gridX\":1485,\"gridY\":37,\"type\":35},{\"gridX\":1517,\"gridY\":37,\"type\":35},{\"gridX\":1549,\"gridY\":37,\"type\":35},{\"gridX\":1581,\"gridY\":37,\"type\":35},{\"gridX\":1613,\"gridY\":37,\"type\":36}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1952,\"gridY\":96,\"type\":12},{\"gridX\":128,\"gridY\":96,\"type\":41},{\"gridX\":184,\"gridY\":96,\"type\":20},{\"gridX\":710,\"gridY\":96,\"type\":58},{\"gridX\":705,\"gridY\":101,\"type\":60},{\"gridX\":8" << "03,\"gridY\":118,\"type\":61},{\"gridX\":842,\"gridY\":122,\"type\":60},{\"gridX\":895,\"gridY\":146,\"type\":60},{\"gridX\":918,\"gridY\":146,\"type\":60},{\"gridX\":930,\"gridY\":163,\"type\":61},{\"gridX\":910,\"gridY\":138,\"type\":59},{\"gridX\":918,\"gridY\":96,\"type\":55},{\"gridX\":894,\"gridY\":96,\"type\":55},{\"gridX\":887,\"gridY\":96,\"type\":53},{\"gridX\":941,\"gridY\":96,\"type\":56},{\"gridX\":220,\"gridY\":144,\"type\":51},{\"gridX\":204,\"gridY\":156,\"type\":50},{\"gridX\":236,\"gridY\":156,\"type\":50},{\"gridX\":252,\"gridY\":144,\"type\":51},{\"gridX\":202,\"gridY\":96,\"type\":53},{\"gridX\":210,\"gridY\":96,\"type\":56},{\"gridX\":220,\"gridY\":96,\"type\":52},{\"gridX\":220,\"gridY\":112,\"type\":52},{\"gridX\":220,\"gridY\":128,\"type\":52},{\"gridX\":252,\"gridY\":96,\"type\":52},{\"gridX\":252,\"gridY\":112,\"type\":52},{\"gridX\":252,\"gridY\":128,\"type\":52},{\"gridX\":268,\"gridY\":120,\"type\":50},{\"gridX\":284,\"gridY\":120,\"type\":50},{\"gridX\":1369,\"gridY\":96,\"type\":52},{\"gridX\":1369,\"gridY\":112,\"type\":51},{\"gridX\":1353,\"gridY\":124,\"type\":50},{\"gridX\":1352,\"gridY\":96,\"type\":53},{\"gridX\":1359,\"gridY\":96,\"type\":55},{\"gridX\":1382,\"gridY\":96,\"type\":54},{\"gridX\":1393,\"gridY\":96,\"type\":56},{\"gridX\":1454,\"gridY\":96,\"type\":52},{\"gridX\":1454,\"gridY\":112,\"type\":51},{\"gridX\":1448,\"gridY\":96,\"type\":55},{\"gridX\":1440,\"gridY\":96,\"type\":53},{\"gridX\":1471,\"gridY\":96,\"type\":56},{\"gridX\":1625,\"gridY\":134,\"type\":60},{\"gridX\":1537,\"gridY\":149,\"type\":60},{\"gridX\":1536,\"gridY\":96,\"type\":54},{\"gridX\":1528,\"gridY\":96,\"type\":53},{\"gridX\":1548,\"gridY\":96,\"type\":56},{\"gridX\":1869,\"gridY\":136,\"type\":59},{\"gridX\":1870,\"gridY\":144,\"type\":60},{\"gridX\":1885,\"gridY\":141,\"type\":59},{\"gridX\":1874,\"gridY\":166,\"type\":58},{\"gridX\":1868,\"gridY\":175,\"type\":60},{\"gridX\":1954,\"gridY\":215,\"type\":59},{\"gridX\":1887,\"gridY\":152,\"type\":59},{\"gridX\":1799,\"gridY\":160,\"type\":59},{\"gridX\":1863,\"gridY\":139,\"type\":59},{\"gridX\":1023,\"gridY\":74,\"type\":62},{\"gridX\":1103,\"gridY\":80,\"type\":62},{\"gridX\":1955,\"gridY\":204,\"type\":60},{\"gridX\":1990,\"gridY\":142,\"type\":60},{\"gridX\":1950,\"gridY\":184,\"type\":59},{\"gridX\":1950,\"gridY\":138,\"type\":59},{\"gridX\":1948,\"gridY\":154,\"type\":59},{\"gridX\":1947,\"gridY\":198,\"type\":59},{\"gridX\":2006,\"gridY\":137,\"type\":59},{\"gridX\":1964,\"gridY\":158,\"type\":59},{\"gridX\":1964,\"gridY\":129,\"type\":59},{\"gridX\":1997,\"gridY\":130,\"type\":59},{\"gridX\":1957,\"gridY\":186,\"type\":60},{\"gridX\":1961,\"gridY\":145,\"type\":59},{\"gridX\":2015,\"gridY\":126,\"type\":59},{\"gridX\":2027,\"gridY\":119,\"type\":59},{\"gridX\":2035,\"gridY\":130,\"type\":59},{\"gridX\":2020,\"gridY\":133,\"type\":60},{\"gridX\":2043,\"gridY\":123,\"type\":59},{\"gridX\":2054,\"gridY\":125,\"type\":59},{\"gridX\":2063,\"gridY\":133,\"type\":59},{\"gridX\":2045,\"gridY\":137,\"type\":60},{\"gridX\":753,\"gridY\":145,\"type\":60},{\"gridX\":795,\"gridY\":168,\"type\":60},{\"gridX\":1978,\"gridY\":136,\"type\":63},{\"gridX\":1952,\"gridY\":170,\"type\":63},{\"gridX\":1976,\"gridY\":150,\"type\":59},{\"gridX\":1944,\"gridY\":211,\"type\":59},{\"gridX\":1941,\"gridY\":223,\"type\":59},{\"gridX\":1946,\"gridY\":227,\"type\":59},{\"gridX\":1693,\"gridY\":0,\"type\":63},{\"gridX\":1715,\"gridY\":0,\"type\":63},{\"gridX\":1688,\"gridY\":8,\"type\":59},{\"gridX\":1705,\"gridY\":20,\"type\":63},{\"gridX\":1690,\"gridY\":26,\"type\":60},{\"gridX\":1711,\"gridY\":26,\"type\":60},{\"gridX\":1724,\"gridY\":13,\"type\":59},{\"gridX\":1704,\"gridY\":74,\"type\":60},{\"gridX\":1702,\"gridY\":149,\"type\":60},{\"gridX\":1714,\"gridY\":143,\"type\":59},{\"gridX\":1743,\"gridY\":200,\"type\":60},{\"gridX\":1796,\"gridY\":166,\"type\":60},{\"gridX\":1794,\"gridY\":146,\"type\":59},{\"gridX\":1832,\"gridY\":7,\"type\":4},{\"gridX\":1600,\"gridY\":96,\"type\":53},{\"gridX\":1608,\"gridY\":96,\"type\":54},{\"gridX\":1620,\"gridY\":96,\"type\":56},{\"gridX\":1828,\"gridY\":7,\"type\":3},{\"gridX\":1846,\"gridY\":7,\"type\":5},{\"gridX\":1687,\"gridY\":91,\"type\":61},{\"gridX\":1719,\"gridY\":155,\"type\":59},{\"gridX\":1722,\"gridY\":166,\"type\":63},{\"gridX\":1739,\"gridY\":192,\"type\":59},{\"gridX\":1731,\"gridY\":179,\"type\":59},{\"gridX\":1833,\"gridY\":12,\"type\":21},{\"gridX\":1884,\"gridY\":232,\"type\":63},{\"gridX\":1906,\"gridY\":232,\"type\":63},{\"gridX\":1924,\"gridY\":232,\"type\":63},{\"gridX\":1938,\"gridY\":232,\"type\":59},{\"gridX\":1911,\"gridY\":219,\"type\":59},{\"gridX\":1873,\"gridY\":232,\"type\":59}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1920,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":848,\"gridY\":96,\"type\":8},{\"gridX\":940,\"gridY\":188,\"type\":3},{\"gridX\":1272,\"gridY\":96,\"type\":9},{\"gridX\":367,\"gridY\":96,\"type\":3},{\"gridX\":500,\"gridY\":110,\"type\":3},{\"gridX\":1933,\"gridY\":96,\"type\":4},{\"gridX\":455,\"gridY\":108,\"type\":3},{\"gridX\":1508,\"gridY\":96,\"type\":8},{\"gridX\":1568,\"gridY\":96,\"type\":6},{\"gridX\":408,\"gridY\":103,\"type\":3},{\"gridX\":251,\"gridY\":176,\"type\":3},{\"gridX\":256,\"gridY\":164,\"type\":8}],\"collectibles\":[{\"gridX\":704,\"gridY\":128,\"type\":0},{\"gridX\":1484,\"gridY\":124,\"type\":0},{\"gridX\":1492,\"gridY\":116,\"type\":0},{\"gridX\":1500,\"gridY\":108,\"type\":0},{\"gridX\":712,\"gridY\":128,\"type\":0},{\"gridX\":720,\"gridY\":128,\"type\":0},{\"gridX\":284,\"gridY\":220,\"type\":1},{\"gridX\":728,\"gridY\":128,\"type\":0},{\"gridX\":756,\"gridY\":116,\"type\":0},{\"gridX\":764,\"gridY\":112,\"type\":0},{\"gridX\":772,\"gridY\":108,\"type\":0},{\"gridX\":808,\"gridY\":128,\"type\":0},{\"gridX\":816,\"gridY\":128,\"type\":0},{\"gridX\":824,\"gridY\":128,\"type\":0},{\"gridX\":862,\"gridY\":152,\"type\":0},{\"gridX\":846,\"gridY\":152,\"type\":0},{\"gridX\":854,\"gridY\":152,\"type\":0},{\"gridX\":1637,\"gridY\":163,\"type\":0},{\"gridX\":911,\"gridY\":176,\"type\":0},{\"gridX\":919,\"gridY\":176,\"type\":0},{\"gridX\":928,\"gridY\":176,\"type\":0},{\"gridX\":936,\"gridY\":176,\"type\":0},{\"gridX\":944,\"gridY\":176,\"type\":0},{\"gridX\":1786,\"gridY\":113,\"type\":1},{\"gridX\":960,\"gridY\":184,\"type\":0},{\"gridX\":968,\"gridY\":184,\"type\":0},{\"gridX\":1000,\"gridY\":180,\"type\":0},{\"gridX\":1040,\"gridY\":172,\"type\":0},{\"gridX\":1048,\"gridY\":172,\"type\":0},{\"gridX\":1088,\"gridY\":164,\"type\":0},{\"gridX\":1080,\"gridY\":164,\"type\":0},{\"gridX\":1120,\"gridY\":156,\"type\":0},{\"gridX\":1128,\"gridY\":156,\"type\":0},{\"gridX\":1168,\"gridY\":148,\"type\":0},{\"gridX\":1160,\"gridY\":148,\"type\":0},{\"gridX\":612,\"gridY\":104,\"type\":0},{\"gridX\":596,\"gridY\":104,\"type\":0},{\"gridX\":604,\"gridY\":104,\"type\":0},{\"gridX\":668,\"gridY\":112,\"type\":0},{\"gridX\":672,\"gridY\":120,\"type\":0},{\"gridX\":676,\"gridY\":128,\"type\":0},{\"gridX\":133,\"gridY\":111,\"type\":0},{\"gridX\":137,\"gridY\":120,\"type\":0},{\"gridX\":142,\"gridY\":129,\"type\":0},{\"gridX\":200,\"gridY\":164,\"type\":0},{\"gridX\":206,\"gridY\":172,\"type\":0},{\"gridX\":1508,\"gridY\":106,\"type\":0},{\"gridX\":1442,\"gridY\":148,\"type\":0},{\"gridX\":1434,\"gridY\":154,\"type\":0},{\"gridX\":1448,\"gridY\":138,\"type\":0},{\"gridX\":1410,\"gridY\":159,\"type\":0},{\"gridX\":1455,\"gridY\":131,\"type\":0},{\"gridX\":1464,\"gridY\":131,\"type\":0},{\"gridX\":1472,\"gridY\":131,\"type\":0},{\"gridX\":188,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":196,\"gridY\":140,\"type\":0},{\"gridX\":1517,\"gridY\":111,\"type\":0},{\"gridX\":1526,\"gridY\":121,\"type\":0},{\"gridX\":1536,\"gridY\":125,\"type\":0},{\"gridX\":1547,\"gridY\":126,\"type\":0},{\"gridX\":1558,\"gridY\":124,\"type\":0},{\"gridX\":1399,\"gridY\":157,\"type\":0},{\"gridX\":1423,\"gridY\":159,\"type\"" << ":0},{\"gridX\":1573,\"gridY\":116,\"type\":0},{\"gridX\":1575,\"gridY\":129,\"type\":0},{\"gridX\":1578,\"gridY\":140,\"type\":0},{\"gridX\":1580,\"gridY\":149,\"type\":0},{\"gridX\":1584,\"gridY\":159,\"type\":0},{\"gridX\":1623,\"gridY\":181,\"type\":0},{\"gridX\":1611,\"gridY\":182,\"type\":0},{\"gridX\":1631,\"gridY\":172,\"type\":0},{\"gridX\":1008,\"gridY\":180,\"type\":0},{\"gridX\":976,\"gridY\":184,\"type\":0},{\"gridX\":1016,\"gridY\":180,\"type\":0},{\"gridX\":1056,\"gridY\":172,\"type\":0},{\"gridX\":1096,\"gridY\":164,\"type\":0},{\"gridX\":1136,\"gridY\":156,\"type\":0},{\"gridX\":1176,\"gridY\":148,\"type\":0},{\"gridX\":1590,\"gridY\":169,\"type\":0},{\"gridX\":1599,\"gridY\":176,\"type\":0},{\"gridX\":1676,\"gridY\":142,\"type\":0},{\"gridX\":1709,\"gridY\":101,\"type\":0},{\"gridX\":1814,\"gridY\":112,\"type\":0},{\"gridX\":1682,\"gridY\":135,\"type\":0},{\"gridX\":1688,\"gridY\":127,\"type\":0},{\"gridX\":1699,\"gridY\":112,\"type\":0},{\"gridX\":1743,\"gridY\":117,\"type\":0},{\"gridX\":1716,\"gridY\":100,\"type\":0},{\"gridX\":1724,\"gridY\":108,\"type\":0},{\"gridX\":1732,\"gridY\":116,\"type\":0},{\"gridX\":1754,\"gridY\":116,\"type\":0},{\"gridX\":1772,\"gridY\":116,\"type\":0},{\"gridX\":1917,\"gridY\":106,\"type\":0},{\"gridX\":1794,\"gridY\":113,\"type\":0},{\"gridX\":1888,\"gridY\":106,\"type\":0},{\"gridX\":1804,\"gridY\":112,\"type\":0},{\"gridX\":1908,\"gridY\":106,\"type\":0},{\"gridX\":1764,\"gridY\":116,\"type\":0},{\"gridX\":1898,\"gridY\":106,\"type\":0},{\"gridX\":1703,\"gridY\":105,\"type\":0},{\"gridX\":1214,\"gridY\":100,\"type\":1},{\"gridX\":1694,\"gridY\":120,\"type\":0},{\"gridX\":1452,\"gridY\":50,\"type\":2},{\"gridX\":1496,\"gridY\":49,\"type\":2},{\"gridX\":1536,\"gridY\":49,\"type\":2},{\"gridX\":1572,\"gridY\":48,\"type\":2},{\"gridX\":1606,\"gridY\":49,\"type\":2},{\"gridX\":1836,\"gridY\":27,\"type\":3},{\"gridX\":1879,\"gridY\":202,\"type\":3},{\"gridX\":1749,\"gridY\":229,\"type\":2},{\"gridX\":1760,\"gridY\":230,\"type\":0},{\"gridX\":1769,\"gridY\":230,\"type\":0},{\"gridX\":1784,\"gridY\":223,\"type\":0},{\"gridX\":1777,\"gridY\":227,\"type\":0},{\"gridX\":1795,\"gridY\":213,\"type\":0},{\"gridX\":1799,\"gridY\":207,\"type\":0},{\"gridX\":1802,\"gridY\":200,\"type\":0},{\"gridX\":1804,\"gridY\":192,\"type\":0},{\"gridX\":1812,\"gridY\":192,\"type\":0},{\"gridX\":1811,\"gridY\":201,\"type\":0},{\"gridX\":1790,\"gridY\":218,\"type\":0}],\"jons\":[{\"gridX\":20,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":1886,\"gridY\":92,\"type\":3}],\"markers\":[{\"gridX\":2228,\"gridY\":0,\"type\":1}]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level5(text.c_str());
}

void Chapter1Level5::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level5 * Chapter1Level5::getInstance()
{
    return s_pInstance;
}

Chapter1Level5::Chapter1Level5(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 6 ///

Chapter1Level6 * Chapter1Level6::s_pInstance = nullptr;

void Chapter1Level6::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":6,\"midgrounds\":[{\"gridX\":0,\"gridY\":97,\"type\":17},{\"gridX\":20,\"gridY\":96,\"type\":17},{\"gridX\":39,\"gridY\":96,\"type\":17},{\"gridX\":58,\"gridY\":96,\"type\":17},{\"gridX\":79,\"gridY\":96,\"type\":17},{\"gridX\":85,\"gridY\":109,\"type\":18},{\"gridX\":92,\"gridY\":122,\"type\":18},{\"gridX\":120,\"gridY\":120,\"type\":18},{\"gridX\":102,\"gridY\":126,\"type\":17},{\"gridX\":0,\"gridY\":117,\"type\":17},{\"gridX\":21,\"gridY\":117,\"type\":17},{\"gridX\":43,\"gridY\":117,\"type\":17},{\"gridX\":64,\"gridY\":117,\"type\":17},{\"gridX\":75,\"gridY\":122,\"type\":18},{\"gridX\":82,\"gridY\":135,\"type\":18},{\"gridX\":92,\"gridY\":146,\"type\":17},{\"gridX\":112,\"gridY\":146,\"type\":17},{\"gridX\":122,\"gridY\":132,\"type\":18},{\"gridX\":134,\"gridY\":146,\"type\":17},{\"gridX\":136,\"gridY\":203,\"type\":18},{\"gridX\":149,\"gridY\":140,\"type\":18},{\"gridX\":156,\"gridY\":152,\"type\":17},{\"gridX\":168,\"gridY\":161,\"type\":18},{\"gridX\":79,\"gridY\":146,\"type\":18},{\"gridX\":162,\"gridY\":131,\"type\":18},{\"gridX\":171,\"gridY\":140,\"type\":18},{\"gridX\":184,\"gridY\":151,\"type\":17},{\"gridX\":197,\"gridY\":140,\"type\":18},{\"gridX\":208,\"gridY\":131,\"type\":18},{\"gridX\":218,\"gridY\":120,\"type\":17},{\"gridX\":225,\"gridY\":133,\"type\":18},{\"gridX\":212,\"gridY\":143,\"type\":18},{\"gridX\":200,\"gridY\":153,\"type\":18},{\"gridX\":96,\"gridY\":167,\"type\":17},{\"gridX\":116,\"gridY\":167,\"type\":17},{\"gridX\":137,\"gridY\":168,\"type\":17},{\"gridX\":159,\"gridY\":168,\"type\":17},{\"gridX\":108,\"gridY\":188,\"type\":17},{\"gridX\":97,\"gridY\":181,\"type\":18},{\"gridX\":129,\"gridY\":189,\"type\":17},{\"gridX\":150,\"gridY\":189,\"type\":17},{\"gridX\":167,\"gridY\":190,\"type\":17},{\"gridX\":152,\"gridY\":208,\"type\":17},{\"gridX\":171,\"gridY\":206,\"type\":18},{\"gridX\":191,\"gridY\":169,\"type\":18},{\"gridX\":178,\"gridY\":170,\"type\":17},{\"gridX\":181,\"gridY\":181,\"type\":18},{\"gridX\":364,\"gridY\":97,\"type\":17},{\"gridX\":364,\"gridY\":118,\"type\":17},{\"gridX\":364,\"gridY\":139,\"type\":17},{\"gridX\":363,\"gridY\":161,\"type\":17},{\"gridX\":350,\"gridY\":112,\"type\":18},{\"gridX\":383,\"gridY\":96,\"type\":17},{\"gridX\":349,\"gridY\":130,\"type\":18},{\"gridX\":343,\"gridY\":144,\"type\":17},{\"gridX\":344,\"gridY\":155,\"type\":18},{\"gridX\":348,\"gridY\":165,\"type\":18},{\"gridX\":402,\"gridY\":96,\"type\":17},{\"gridX\":384,\"gridY\":115,\"type\":17},{\"gridX\":402,\"gridY\":110,\"type\":18},{\"gridX\":392,\"gridY\":123,\"type\":18},{\"gridX\":379,\"gridY\":132,\"type\":18},{\"gridX\":380,\"gridY\":148,\"type\":18},{\"gridX\":379,\"gridY\":166,\"type\":18},{\"gridX\":381,\"gridY\":162,\"type\":18},{\"gridX\":443,\"gridY\":96,\"type\":17},{\"gridX\":465,\"gridY\":96,\"type\":17},{\"gridX\":449,\"gridY\":108,\"type\":18},{\"gridX\":461,\"gridY\":113,\"type\":18},{\"gridX\":476,\"gridY\":110,\"type\":18},{\"gridX\":450,\"gridY\":125,\"type\":18},{\"gridX\":422,\"gridY\":96,\"type\":17},{\"gridX\":445,\"gridY\":112,\"type\":17},{\"gridX\":460,\"gridY\":127,\"type\":18},{\"gridX\":245,\"gridY\":96,\"type\":1},{\"gridX\":583,\"gridY\":96,\"type\":0},{\"gridX\":537,\"gridY\":96,\"type\":1},{\"gridX\":411,\"gridY\":96,\"type\":6},{\"gridX\":431,\"gridY\":96,\"type\":6},{\"gridX\":460,\"gridY\":96,\"type\":6},{\"gridX\":144,\"gridY\":96,\"type\":2},{\"gridX\":2,\"gridY\":96,\"type\":6},{\"gridX\":53,\"gridY\":96,\"type\":6},{\"gridX\":422,\"gridY\":96,\"type\":11},{\"gridX\":198,\"gridY\":96,\"type\":15},{\"gridX\":169,\"gridY\":96,\"type\":16},{\"gridX\":325,\"gridY\":96,\"type\":15},{\"gridX\":640,\"gridY\":96,\"type\":17},{\"gridX\":660,\"gridY\":96,\"type\":17},{\"gridX\":681,\"gridY\":96,\"type\":17},{\"gridX\":703,\"gridY\":96,\"type\":17},{\"gridX\":721,\"gridY\":96,\"type\":17},{\"gridX\":743,\"gridY\":96,\"type\":17},{\"gridX\":636,\"gridY\":116,\"type\":17},{\"gridX\":658,\"gridY\":118,\"type\":17},{\"gridX\":680,\"gridY\":117,\"type\":17},{\"gridX\":701,\"gridY\":117,\"type\":17},{\"gridX\":723,\"gridY\":117,\"type\":17},{\"gridX\":744,\"gridY\":117,\"type\":17},{\"gridX\":626,\"gridY\":122,\"type\":18},{\"gridX\":618,\"gridY\":130,\"type\":18},{\"gridX\":626,\"gridY\":139,\"type\":17},{\"gridX\":611,\"gridY\":149,\"type\":18},{\"gridX\":646,\"gridY\":138,\"type\":17},{\"gridX\":598,\"gridY\":150,\"type\":18},{\"gridX\":667,\"gridY\":139,\"type\":17},{\"gridX\":688,\"gridY\":139,\"type\":17},{\"gridX\":708,\"gridY\":139,\"type\":17},{\"gridX\":729,\"gridY\":139,\"type\":17},{\"gridX\":749,\"gridY\":139,\"type\":17},{\"gridX\":753,\"gridY\":106,\"type\":18},{\"gridX\":759,\"gridY\":119,\"type\":18},{\"gridX\":767,\"gridY\":132,\"type\":18},{\"gridX\":778,\"gridY\":133,\"type\":17},{\"gridX\":766,\"gridY\":144,\"type\":18},{\"gridX\":620,\"gridY\":160,\"type\":17},{\"gridX\":641,\"gridY\":159,\"type\":17},{\"gridX\":662,\"gridY\":160,\"type\":17},{\"gridX\":684,\"gridY\":160,\"type\":17},{\"gridX\":695,\"gridY\":149,\"type\":18},{\"gridX\":747,\"gridY\":151,\"type\":18},{\"gridX\":744,\"gridY\":167,\"type\":18},{\"gridX\":756,\"gridY\":162,\"type\":17},{\"gridX\":807,\"gridY\":128,\"type\":17},{\"gridX\":792,\"gridY\":127,\"type\":18},{\"gridX\":828,\"gridY\":123,\"type\":17},{\"gridX\":833,\"gridY\":111,\"type\":18},{\"gridX\":833,\"gridY\":101,\"type\":18},{\"gridX\":840,\"gridY\":96,\"type\":17},{\"gridX\":799,\"gridY\":141,\"type\":18},{\"gridX\":817,\"gridY\":142,\"type\":18},{\"gridX\":827,\"gridY\":144,\"type\":17},{\"gridX\":834,\"gridY\":156,\"type\":18},{\"gridX\":561,\"gridY\":96,\"type\":2},{\"gridX\":431,\"gridY\":108,\"type\":18},{\"gridX\":463,\"gridY\":139,\"type\":18},{\"gridX\":1256,\"gridY\":112,\"type\":19},{\"gridX\":1272,\"gridY\":112,\"type\":19},{\"gridX\":1256,\"gridY\":128,\"type\":20},{\"gridX\":1272,\"gridY\":128,\"type\":20},{\"gridX\":1288,\"gridY\":112,\"type\":19},{\"gridX\":1304,\"gridY\":112,\"type\":19},{\"gridX\":1144,\"gridY\":96,\"type\":13},{\"gridX\":1179,\"gridY\":96,\"type\":14},{\"gridX\":1158,\"gridY\":96,\"type\":15},{\"gridX\":1408,\"gridY\":96,\"type\":11},{\"gridX\":1384,\"gridY\":112,\"type\":19},{\"gridX\":1400,\"gridY\":112,\"type\":19},{\"gridX\":1416,\"gridY\":112,\"type\":19},{\"gridX\":1288,\"gridY\":128,\"type\":19},{\"gridX\":1304,\"gridY\":128,\"type\":19},{\"gridX\":1288,\"gridY\":144,\"type\":19},{\"gridX\":1304,\"gridY\":144,\"type\":19},{\"gridX\":1801,\"gridY\":96,\"type\":13},{\"gridX\":1765,\"gridY\":96,\"type\":15},{\"gridX\":1690,\"gridY\":96,\"type\":16},{\"gridX\":63,\"gridY\":137,\"type\":17},{\"gridX\":74,\"gridY\":163,\"type\":17},{\"gridX\":70,\"gridY\":151,\"type\":17},{\"gridX\":222,\"gridY\":96,\"type\":16},{\"gridX\":1464,\"gridY\":112,\"type\":19},{\"gridX\":1480,\"gridY\":112,\"type\":19}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":28},{\"gridX\":128,\"gridY\":0,\"type\":28},{\"gridX\":256,\"gridY\":0,\"type\":28},{\"gridX\":384,\"gridY\":0,\"type\":28},{\"gridX\":512,\"gridY\":0,\"type\":28},{\"gridX\":640,\"gridY\":0,\"type\":28},{\"gridX\":768,\"gridY\":0,\"type\":28},{\"gridX\":896,\"gridY\":0,\"type\":28},{\"gridX\":1024,\"gridY\":0,\"type\":28},{\"gridX\":1152,\"gridY\":0,\"type\":28},{\"gridX\":1280,\"gridY\":0,\"type\":28},{\"gridX\":1408,\"gridY\":0,\"type\":28},{\"gridX\":1536,\"gridY\":0,\"type\":28},{\"gridX\":1664,\"gridY\":0,\"type\":28},{\"gridX\":1792,\"gridY\":0,\"type\":28}],\"pits\":[{\"gridX\":866,\"gridY\":0,\"type\":33},{\"gridX\":1497,\"gridY\":0,\"type\":31},{\"gridX\":1599,\"gridY\":0,\"type\":31},{\"gridX\":580,\"gridY\":0,\"type\":30},{\"gridX\":604,\"gridY\":47,\"type\":34},{\"gridX\":628,\"gridY\":47,\"type\":35},{\"gridX\":660,\"gridY\":47,\"type\":35},{\"gridX\":692,\"gridY\":47,\"type\":35},{\"gridX\":756,\"gridY\":47,\"type\":35},{\"gridX\":788,\"gridY\":47,\"type\":35},{\"gridX\":724,\"gridY\":47,\"type\":35},{\"gridX\":820,\"gridY\":47,\"type\":35},{\"gridX\":852,\"gridY\":47,\"type\":36},{\"gridX\":1115,\"gridY\":15,\"type\":34},{\"gr" << "idX\":1138,\"gridY\":15,\"type\":35},{\"gridX\":1433,\"gridY\":0,\"type\":30},{\"gridX\":1170,\"gridY\":15,\"type\":35},{\"gridX\":1202,\"gridY\":15,\"type\":35},{\"gridX\":1234,\"gridY\":15,\"type\":35},{\"gridX\":1354,\"gridY\":27,\"type\":35},{\"gridX\":1386,\"gridY\":27,\"type\":35},{\"gridX\":1418,\"gridY\":27,\"type\":36},{\"gridX\":1280,\"gridY\":0,\"type\":31},{\"gridX\":1330,\"gridY\":27,\"type\":34},{\"gridX\":1266,\"gridY\":15,\"type\":36}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1738,\"gridY\":96,\"type\":12},{\"gridX\":93,\"gridY\":96,\"type\":58},{\"gridX\":93,\"gridY\":104,\"type\":60},{\"gridX\":115,\"gridY\":104,\"type\":60},{\"gridX\":114,\"gridY\":96,\"type\":58},{\"gridX\":107,\"gridY\":99,\"type\":59},{\"gridX\":22,\"gridY\":138,\"type\":59},{\"gridX\":70,\"gridY\":171,\"type\":59},{\"gridX\":15,\"gridY\":126,\"type\":59},{\"gridX\":33,\"gridY\":135,\"type\":59},{\"gridX\":52,\"gridY\":166,\"type\":60},{\"gridX\":31,\"gridY\":146,\"type\":60},{\"gridX\":135,\"gridY\":96,\"type\":53},{\"gridX\":142,\"gridY\":96,\"type\":55},{\"gridX\":165,\"gridY\":96,\"type\":55},{\"gridX\":216,\"gridY\":96,\"type\":58},{\"gridX\":212,\"gridY\":96,\"type\":56},{\"gridX\":188,\"gridY\":96,\"type\":55},{\"gridX\":216,\"gridY\":103,\"type\":60},{\"gridX\":140,\"gridY\":166,\"type\":61},{\"gridX\":54,\"gridY\":152,\"type\":59},{\"gridX\":83,\"gridY\":180,\"type\":59},{\"gridX\":95,\"gridY\":188,\"type\":59},{\"gridX\":98,\"gridY\":199,\"type\":60},{\"gridX\":117,\"gridY\":203,\"type\":59},{\"gridX\":131,\"gridY\":209,\"type\":59},{\"gridX\":138,\"gridY\":217,\"type\":59},{\"gridX\":147,\"gridY\":222,\"type\":60},{\"gridX\":229,\"gridY\":96,\"type\":58},{\"gridX\":161,\"gridY\":221,\"type\":59},{\"gridX\":222,\"gridY\":193,\"type\":50},{\"gridX\":238,\"gridY\":193,\"type\":50},{\"gridX\":197,\"gridY\":174,\"type\":60},{\"gridX\":355,\"gridY\":96,\"type\":58},{\"gridX\":348,\"gridY\":98,\"type\":60},{\"gridX\":355,\"gridY\":175,\"type\":59},{\"gridX\":362,\"gridY\":181,\"type\":59},{\"gridX\":360,\"gridY\":185,\"type\":60},{\"gridX\":443,\"gridY\":191,\"type\":60},{\"gridX\":250,\"gridY\":96,\"type\":53},{\"gridX\":257,\"gridY\":96,\"type\":55},{\"gridX\":280,\"gridY\":96,\"type\":55},{\"gridX\":303,\"gridY\":96,\"type\":55},{\"gridX\":326,\"gridY\":96,\"type\":55},{\"gridX\":349,\"gridY\":96,\"type\":56},{\"gridX\":372,\"gridY\":175,\"type\":59},{\"gridX\":454,\"gridY\":180,\"type\":59},{\"gridX\":449,\"gridY\":165,\"type\":59},{\"gridX\":456,\"gridY\":150,\"type\":59},{\"gridX\":444,\"gridY\":148,\"type\":59},{\"gridX\":452,\"gridY\":141,\"type\":59},{\"gridX\":592,\"gridY\":164,\"type\":60},{\"gridX\":612,\"gridY\":164,\"type\":60},{\"gridX\":604,\"gridY\":159,\"type\":59},{\"gridX\":635,\"gridY\":181,\"type\":61},{\"gridX\":657,\"gridY\":164,\"type\":60},{\"gridX\":669,\"gridY\":154,\"type\":59},{\"gridX\":679,\"gridY\":168,\"type\":59},{\"gridX\":684,\"gridY\":181,\"type\":59},{\"gridX\":681,\"gridY\":191,\"type\":60},{\"gridX\":687,\"gridY\":167,\"type\":59},{\"gridX\":754,\"gridY\":193,\"type\":60},{\"gridX\":750,\"gridY\":181,\"type\":59},{\"gridX\":760,\"gridY\":174,\"type\":59},{\"gridX\":827,\"gridY\":170,\"type\":60},{\"gridX\":837,\"gridY\":159,\"type\":59},{\"gridX\":824,\"gridY\":160,\"type\":59},{\"gridX\":843,\"gridY\":96,\"type\":58},{\"gridX\":843,\"gridY\":97,\"type\":60},{\"gridX\":729,\"gridY\":96,\"type\":58},{\"gridX\":727,\"gridY\":100,\"type\":60},{\"gridX\":617,\"gridY\":114,\"type\":60},{\"gridX\":502,\"gridY\":96,\"type\":58},{\"gridX\":677,\"gridY\":144,\"type\":59},{\"gridX\":676,\"gridY\":130,\"type\":59},{\"gridX\":787,\"gridY\":127,\"type\":59},{\"gridX\":786,\"gridY\":141,\"type\":59},{\"gridX\":793,\"gridY\":135,\"type\":59},{\"gridX\":648,\"gridY\":96,\"type\":53},{\"gridX\":656,\"gridY\":96,\"type\":54},{\"gridX\":667,\"gridY\":96,\"type\":56},{\"gridX\":702,\"gridY\":96,\"type\":53},{\"gridX\":710,\"gridY\":96,\"type\":54},{\"gridX\":722,\"gridY\":96,\"type\":56},{\"gridX\":497,\"gridY\":96,\"type\":53},{\"gridX\":625,\"gridY\":96,\"type\":58},{\"gridX\":501,\"gridY\":97,\"type\":60},{\"gridX\":504,\"gridY\":96,\"type\":55},{\"gridX\":736,\"gridY\":96,\"type\":53},{\"gridX\":744,\"gridY\":96,\"type\":55},{\"gridX\":768,\"gridY\":96,\"type\":56},{\"gridX\":815,\"gridY\":96,\"type\":53},{\"gridX\":822,\"gridY\":96,\"type\":54},{\"gridX\":833,\"gridY\":96,\"type\":56},{\"gridX\":1208,\"gridY\":96,\"type\":52},{\"gridX\":1224,\"gridY\":96,\"type\":52},{\"gridX\":1240,\"gridY\":96,\"type\":52},{\"gridX\":1256,\"gridY\":96,\"type\":51},{\"gridX\":1272,\"gridY\":96,\"type\":51},{\"gridX\":1288,\"gridY\":96,\"type\":51},{\"gridX\":1304,\"gridY\":96,\"type\":51},{\"gridX\":1320,\"gridY\":96,\"type\":51},{\"gridX\":1336,\"gridY\":96,\"type\":51},{\"gridX\":1352,\"gridY\":96,\"type\":51},{\"gridX\":1368,\"gridY\":96,\"type\":51},{\"gridX\":1464,\"gridY\":128,\"type\":52},{\"gridX\":1464,\"gridY\":144,\"type\":51},{\"gridX\":1368,\"gridY\":134,\"type\":50},{\"gridX\":1352,\"gridY\":134,\"type\":50},{\"gridX\":1565,\"gridY\":96,\"type\":52},{\"gridX\":1565,\"gridY\":112,\"type\":52},{\"gridX\":1565,\"gridY\":127,\"type\":52},{\"gridX\":1565,\"gridY\":143,\"type\":51},{\"gridX\":1663,\"gridY\":96,\"type\":52},{\"gridX\":1663,\"gridY\":112,\"type\":52},{\"gridX\":1663,\"gridY\":128,\"type\":52},{\"gridX\":1663,\"gridY\":144,\"type\":51},{\"gridX\":1679,\"gridY\":156,\"type\":50},{\"gridX\":574,\"gridY\":96,\"type\":56},{\"gridX\":526,\"gridY\":96,\"type\":55},{\"gridX\":598,\"gridY\":114,\"type\":60},{\"gridX\":606,\"gridY\":96,\"type\":58},{\"gridX\":1583,\"gridY\":96,\"type\":23},{\"gridX\":1208,\"gridY\":112,\"type\":52},{\"gridX\":1288,\"gridY\":160,\"type\":52},{\"gridX\":1288,\"gridY\":176,\"type\":52},{\"gridX\":1304,\"gridY\":160,\"type\":52},{\"gridX\":1304,\"gridY\":176,\"type\":52},{\"gridX\":1384,\"gridY\":128,\"type\":52},{\"gridX\":1384,\"gridY\":144,\"type\":52},{\"gridX\":1400,\"gridY\":128,\"type\":52},{\"gridX\":1416,\"gridY\":128,\"type\":52},{\"gridX\":1400,\"gridY\":144,\"type\":52},{\"gridX\":1416,\"gridY\":144,\"type\":52},{\"gridX\":45,\"gridY\":138,\"type\":63},{\"gridX\":0,\"gridY\":131,\"type\":63},{\"gridX\":765,\"gridY\":162,\"type\":59},{\"gridX\":773,\"gridY\":150,\"type\":63},{\"gridX\":1117,\"gridY\":53,\"type\":33},{\"gridX\":1464,\"gridY\":96,\"type\":51},{\"gridX\":1432,\"gridY\":108,\"type\":50},{\"gridX\":1448,\"gridY\":108,\"type\":50},{\"gridX\":1496,\"gridY\":108,\"type\":50},{\"gridX\":1480,\"gridY\":96,\"type\":51},{\"gridX\":1443,\"gridY\":24,\"type\":50},{\"gridX\":1445,\"gridY\":27,\"type\":21},{\"gridX\":577,\"gridY\":131,\"type\":61},{\"gridX\":833,\"gridY\":114,\"type\":61},{\"gridX\":616,\"gridY\":107,\"type\":59},{\"gridX\":600,\"gridY\":103,\"type\":59},{\"gridX\":550,\"gridY\":96,\"type\":55},{\"gridX\":1581,\"gridY\":155,\"type\":50},{\"gridX\":1384,\"gridY\":96,\"type\":51},{\"gridX\":1400,\"gridY\":96,\"type\":51},{\"gridX\":1416,\"gridY\":96,\"type\":51},{\"gridX\":1106,\"gridY\":46,\"type\":4},{\"gridX\":1120,\"gridY\":46,\"type\":5},{\"gridX\":1102,\"gridY\":46,\"type\":3}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1704,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":688,\"gridY\":96,\"type\":8},{\"gridX\":793,\"gridY\":96,\"type\":8},{\"gridX\":316,\"gridY\":123,\"type\":3},{\"gridX\":429,\"gridY\":96,\"type\":10},{\"gridX\":362,\"gridY\":121,\"type\":8}],\"collectibles\":[{\"gridX\":62,\"gridY\":104,\"type\":0},{\"gridX\":68,\"gridY\":115,\"type\":0},{\"gridX\":73,\"gridY\":125,\"type\":0},{\"gridX\":104,\"gridY\":136,\"type\":0},{\"gridX\":107,\"gridY\":149,\"type\":0},{\"gridX\":112,\"gridY\":160,\"type\":0},{\"gridX\":119,\"gridY\":171,\"type\":0},{\"gridX\":128,\"gridY\":180,\"type\":0},{\"gridX\":162,\"gridY\":178,\"type\":0},{\"gridX\":168,\"gridY\":189,\"type\":0},{\"gridX\":178,\"gridY\":197,\"type\":0},{\"gridX\":191,\"gridY\":203,\"type\":0},{\"gridX\":201,\"gridY" << "\":203,\"type\":0},{\"gridX\":211,\"gridY\":203,\"type\":0},{\"gridX\":223,\"gridY\":203,\"type\":0},{\"gridX\":494,\"gridY\":247,\"type\":1},{\"gridX\":247,\"gridY\":203,\"type\":0},{\"gridX\":252,\"gridY\":215,\"type\":0},{\"gridX\":263,\"gridY\":223,\"type\":0},{\"gridX\":276,\"gridY\":229,\"type\":0},{\"gridX\":290,\"gridY\":232,\"type\":0},{\"gridX\":303,\"gridY\":229,\"type\":0},{\"gridX\":312,\"gridY\":224,\"type\":0},{\"gridX\":320,\"gridY\":217,\"type\":0},{\"gridX\":377,\"gridY\":215,\"type\":0},{\"gridX\":384,\"gridY\":225,\"type\":0},{\"gridX\":397,\"gridY\":233,\"type\":0},{\"gridX\":328,\"gridY\":209,\"type\":0},{\"gridX\":409,\"gridY\":237,\"type\":0},{\"gridX\":424,\"gridY\":234,\"type\":0},{\"gridX\":436,\"gridY\":228,\"type\":0},{\"gridX\":447,\"gridY\":220,\"type\":0},{\"gridX\":235,\"gridY\":203,\"type\":0},{\"gridX\":563,\"gridY\":173,\"type\":0},{\"gridX\":569,\"gridY\":184,\"type\":0},{\"gridX\":580,\"gridY\":195,\"type\":0},{\"gridX\":594,\"gridY\":200,\"type\":0},{\"gridX\":608,\"gridY\":198,\"type\":0},{\"gridX\":619,\"gridY\":193,\"type\":0},{\"gridX\":632,\"gridY\":193,\"type\":0},{\"gridX\":693,\"gridY\":218,\"type\":0},{\"gridX\":693,\"gridY\":226,\"type\":0},{\"gridX\":767,\"gridY\":226,\"type\":0},{\"gridX\":765,\"gridY\":218,\"type\":0},{\"gridX\":838,\"gridY\":197,\"type\":0},{\"gridX\":839,\"gridY\":206,\"type\":0},{\"gridX\":918,\"gridY\":163,\"type\":0},{\"gridX\":931,\"gridY\":162,\"type\":0},{\"gridX\":944,\"gridY\":161,\"type\":0},{\"gridX\":958,\"gridY\":160,\"type\":0},{\"gridX\":1043,\"gridY\":152,\"type\":0},{\"gridX\":1071,\"gridY\":148,\"type\":0},{\"gridX\":1083,\"gridY\":146,\"type\":0},{\"gridX\":1095,\"gridY\":143,\"type\":0},{\"gridX\":972,\"gridY\":159,\"type\":0},{\"gridX\":989,\"gridY\":158,\"type\":0},{\"gridX\":1058,\"gridY\":150,\"type\":0},{\"gridX\":1030,\"gridY\":154,\"type\":0},{\"gridX\":1018,\"gridY\":155,\"type\":0},{\"gridX\":1003,\"gridY\":156,\"type\":0},{\"gridX\":1211,\"gridY\":135,\"type\":0},{\"gridX\":1221,\"gridY\":135,\"type\":0},{\"gridX\":1230,\"gridY\":135,\"type\":0},{\"gridX\":1239,\"gridY\":135,\"type\":0},{\"gridX\":1249,\"gridY\":135,\"type\":0},{\"gridX\":1259,\"gridY\":132,\"type\":0},{\"gridX\":1268,\"gridY\":124,\"type\":0},{\"gridX\":1275,\"gridY\":117,\"type\":0},{\"gridX\":1286,\"gridY\":117,\"type\":0},{\"gridX\":1297,\"gridY\":117,\"type\":0},{\"gridX\":1308,\"gridY\":125,\"type\":0},{\"gridX\":831,\"gridY\":200,\"type\":0},{\"gridX\":846,\"gridY\":201,\"type\":0},{\"gridX\":773,\"gridY\":221,\"type\":0},{\"gridX\":758,\"gridY\":222,\"type\":0},{\"gridX\":701,\"gridY\":223,\"type\":0},{\"gridX\":685,\"gridY\":221,\"type\":0},{\"gridX\":1384,\"gridY\":167,\"type\":0},{\"gridX\":1394,\"gridY\":167,\"type\":0},{\"gridX\":1405,\"gridY\":168,\"type\":0},{\"gridX\":1411,\"gridY\":177,\"type\":0},{\"gridX\":1418,\"gridY\":186,\"type\":0},{\"gridX\":1430,\"gridY\":191,\"type\":0},{\"gridX\":1443,\"gridY\":192,\"type\":0},{\"gridX\":1456,\"gridY\":188,\"type\":0},{\"gridX\":1474,\"gridY\":172,\"type\":0},{\"gridX\":1483,\"gridY\":183,\"type\":0},{\"gridX\":1494,\"gridY\":194,\"type\":0},{\"gridX\":1507,\"gridY\":199,\"type\":0},{\"gridX\":1524,\"gridY\":192,\"type\":0},{\"gridX\":1538,\"gridY\":195,\"type\":0},{\"gridX\":1550,\"gridY\":188,\"type\":0},{\"gridX\":1571,\"gridY\":171,\"type\":0},{\"gridX\":1579,\"gridY\":184,\"type\":0},{\"gridX\":1590,\"gridY\":192,\"type\":0},{\"gridX\":1603,\"gridY\":199,\"type\":0},{\"gridX\":1618,\"gridY\":189,\"type\":0},{\"gridX\":1632,\"gridY\":194,\"type\":0},{\"gridX\":1644,\"gridY\":188,\"type\":0},{\"gridX\":1659,\"gridY\":167,\"type\":0},{\"gridX\":1652,\"gridY\":180,\"type\":0},{\"gridX\":1105,\"gridY\":137,\"type\":1},{\"gridX\":334,\"gridY\":198,\"type\":1},{\"gridX\":1309,\"gridY\":47,\"type\":3},{\"gridX\":1447,\"gridY\":81,\"type\":3},{\"gridX\":1340,\"gridY\":40,\"type\":2},{\"gridX\":1352,\"gridY\":40,\"type\":2},{\"gridX\":1364,\"gridY\":40,\"type\":2},{\"gridX\":1376,\"gridY\":40,\"type\":2},{\"gridX\":1388,\"gridY\":40,\"type\":2},{\"gridX\":1400,\"gridY\":40,\"type\":2},{\"gridX\":1412,\"gridY\":40,\"type\":2},{\"gridX\":1424,\"gridY\":40,\"type\":2}],\"jons\":[{\"gridX\":19,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":470,\"gridY\":85,\"type\":0},{\"gridX\":418,\"gridY\":92,\"type\":1},{\"gridX\":382,\"gridY\":92,\"type\":3},{\"gridX\":65,\"gridY\":92,\"type\":1},{\"gridX\":70,\"gridY\":92,\"type\":3},{\"gridX\":1208,\"gridY\":128,\"type\":10},{\"gridX\":1224,\"gridY\":128,\"type\":10},{\"gridX\":1240,\"gridY\":128,\"type\":11},{\"gridX\":1208,\"gridY\":112,\"type\":6},{\"gridX\":1224,\"gridY\":112,\"type\":6},{\"gridX\":1240,\"gridY\":112,\"type\":6},{\"gridX\":1288,\"gridY\":176,\"type\":4},{\"gridX\":1288,\"gridY\":160,\"type\":6},{\"gridX\":1304,\"gridY\":160,\"type\":6},{\"gridX\":1304,\"gridY\":176,\"type\":4},{\"gridX\":1288,\"gridY\":192,\"type\":8},{\"gridX\":1304,\"gridY\":192,\"type\":9},{\"gridX\":1384,\"gridY\":128,\"type\":4},{\"gridX\":1400,\"gridY\":128,\"type\":4},{\"gridX\":1416,\"gridY\":128,\"type\":4},{\"gridX\":1384,\"gridY\":144,\"type\":6},{\"gridX\":1416,\"gridY\":144,\"type\":6},{\"gridX\":1400,\"gridY\":144,\"type\":6},{\"gridX\":1384,\"gridY\":160,\"type\":10},{\"gridX\":1400,\"gridY\":160,\"type\":10},{\"gridX\":1416,\"gridY\":160,\"type\":10},{\"gridX\":1286,\"gridY\":92,\"type\":2},{\"gridX\":1464,\"gridY\":128,\"type\":4},{\"gridX\":1464,\"gridY\":144,\"type\":6},{\"gridX\":1464,\"gridY\":160,\"type\":10},{\"gridX\":1480,\"gridY\":144,\"type\":6},{\"gridX\":1480,\"gridY\":128,\"type\":4},{\"gridX\":1426,\"gridY\":92,\"type\":1},{\"gridX\":1440,\"gridY\":92,\"type\":2},{\"gridX\":1450,\"gridY\":92,\"type\":3},{\"gridX\":1273,\"gridY\":92,\"type\":1},{\"gridX\":1319,\"gridY\":92,\"type\":2},{\"gridX\":1318,\"gridY\":92,\"type\":3},{\"gridX\":1308,\"gridY\":92,\"type\":1},{\"gridX\":1291,\"gridY\":92,\"type\":3},{\"gridX\":554,\"gridY\":92,\"type\":3},{\"gridX\":601,\"gridY\":92,\"type\":3},{\"gridX\":1480,\"gridY\":160,\"type\":11}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level6(text.c_str());
}

void Chapter1Level6::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level6 * Chapter1Level6::getInstance()
{
    return s_pInstance;
}

Chapter1Level6::Chapter1Level6(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 7 ///

Chapter1Level7 * Chapter1Level7::s_pInstance = nullptr;

void Chapter1Level7::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":7,\"midgrounds\":[{\"gridX\":2216,\"gridY\":96,\"type\":2},{\"gridX\":2276,\"gridY\":96,\"type\":1},{\"gridX\":2316,\"gridY\":96,\"type\":0},{\"gridX\":2348,\"gridY\":96,\"type\":2},{\"gridX\":2408,\"gridY\":96,\"type\":1},{\"gridX\":2448,\"gridY\":96,\"type\":0},{\"gridX\":0,\"gridY\":96,\"type\":1},{\"gridX\":66,\"gridY\":96,\"type\":0},{\"gridX\":107,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":272,\"gridY\":96,\"type\":1},{\"gridX\":540,\"gridY\":96,\"type\":0},{\"gridX\":432,\"gridY\":96,\"type\":2},{\"gridX\":856,\"gridY\":96,\"type\":2},{\"gridX\":953,\"gridY\":96,\"type\":1},{\"gridX\":728,\"gridY\":96,\"type\":1},{\"gridX\":805,\"gridY\":96,\"type\":0},{\"gridX\":1036,\"gridY\":96,\"type\":2},{\"gridX\":1008,\"gridY\":96,\"type\":0},{\"gridX\":1168,\"gridY\":96,\"type\":0},{\"gridX\":1529,\"gridY\":96,\"type\":2},{\"gridX\":1628,\"gridY\":96,\"type\":0},{\"gridX\":1668,\"gridY\":96,\"type\":1},{\"gridX\":1720,\"gridY\":96,\"type\":2},{\"gridX\":1840,\"gridY\":96,\"type\":2},{\"gridX\":1772,\"gridY\":96,\"type\":0},{\"gridX\":2136,\"gridY\":96,\"type\":1},{\"gridX\":2180,\"gridY\":96,\"type\":0},{\"gridX\":595,\"gridY\":96,\"type\":17},{\"gridX\":598,\"gridY\":112,\"type\":18},{\"gridX\":589,\"gridY\":99,\"type\":18},{\"gridX\":601,\"gridY\":102,\"type\":18},{\"gridX\":697,\"gridY\":96,\"type\":17},{\"gridX\":684,\"gridY\":108,\"type\":18},{\"gridX\":700,\"gridY\":102,\"type\":18},{\"gridX\":607,\"gridY\":126,\"type\":18},{\"gridX\":614,\"gridY\":116,\"type\":17},{\"gridX\":622,\"gridY\":128,\"type\":18},{\"gridX\":689,\"gridY\":114,\"type\":18},{\"gridX\":676,\"gridY\":124,\"type\":18},{\"gridX\":630,\"gridY\":140,\"type\":17},{\"gridX\":648,\"gridY\":138,\"type\":18},{\"gridX\":662,\"gridY\":130,\"type\":18},{\"gridX\":672,\"gridY\":141,\"type\":18},{\"gridX\":1244,\"gridY\":96,\"type\":7},{\"gridX\":1244,\"gridY\":108,\"type\":7},{\"gridX\":1244,\"gridY\":120,\"type\":7},{\"gridX\":1244,\"gridY\":132,\"type\":7},{\"gridX\":1244,\"gridY\":144,\"type\":7},{\"gridX\":1244,\"gridY\":156,\"type\":7},{\"gridX\":1244,\"gridY\":168,\"type\":7},{\"gridX\":1244,\"gridY\":180,\"type\":7},{\"gridX\":1260,\"gridY\":132,\"type\":7},{\"gridX\":1260,\"gridY\":144,\"type\":7},{\"gridX\":1260,\"gridY\":167,\"type\":7},{\"gridX\":1260,\"gridY\":156,\"type\":7},{\"gridX\":1260,\"gridY\":96,\"type\":7},{\"gridX\":1260,\"gridY\":108,\"type\":7},{\"gridX\":1260,\"gridY\":120,\"type\":7},{\"gridX\":1276,\"gridY\":96,\"type\":7},{\"gridX\":1276,\"gridY\":108,\"type\":7},{\"gridX\":1276,\"gridY\":120,\"type\":7},{\"gridX\":1276,\"gridY\":132,\"type\":7},{\"gridX\":1276,\"gridY\":144,\"type\":7},{\"gridX\":1276,\"gridY\":156,\"type\":7},{\"gridX\":1228,\"gridY\":96,\"type\":7},{\"gridX\":1228,\"gridY\":108,\"type\":7},{\"gridX\":1228,\"gridY\":120,\"type\":7},{\"gridX\":1228,\"gridY\":132,\"type\":7},{\"gridX\":1228,\"gridY\":144,\"type\":7},{\"gridX\":1228,\"gridY\":156,\"type\":7},{\"gridX\":1228,\"gridY\":168,\"type\":7},{\"gridX\":1228,\"gridY\":180,\"type\":7},{\"gridX\":1228,\"gridY\":192,\"type\":7},{\"gridX\":1224,\"gridY\":96,\"type\":2},{\"gridX\":1206,\"gridY\":96,\"type\":1},{\"gridX\":1101,\"gridY\":96,\"type\":6},{\"gridX\":1114,\"gridY\":96,\"type\":2},{\"gridX\":168,\"gridY\":96,\"type\":7},{\"gridX\":192,\"gridY\":96,\"type\":7},{\"gridX\":192,\"gridY\":108,\"type\":7},{\"gridX\":192,\"gridY\":120,\"type\":7},{\"gridX\":787,\"gridY\":96,\"type\":17},{\"gridX\":778,\"gridY\":100,\"type\":18},{\"gridX\":787,\"gridY\":111,\"type\":18},{\"gridX\":784,\"gridY\":128,\"type\":18},{\"gridX\":776,\"gridY\":118,\"type\":18},{\"gridX\":797,\"gridY\":97,\"type\":18},{\"gridX\":779,\"gridY\":152,\"type\":18},{\"gridX\":795,\"gridY\":110,\"type\":17},{\"gridX\":790,\"gridY\":141,\"type\":18},{\"gridX\":793,\"gridY\":120,\"type\":18},{\"gridX\":795,\"gridY\":158,\"type\":18},{\"gridX\":786,\"gridY\":165,\"type\":18},{\"gridX\":634,\"gridY\":153,\"type\":18},{\"gridX\":1704,\"gridY\":96,\"type\":7},{\"gridX\":1704,\"gridY\":108,\"type\":7},{\"gridX\":1696,\"gridY\":112,\"type\":7},{\"gridX\":1712,\"gridY\":113,\"type\":7},{\"gridX\":261,\"gridY\":96,\"type\":6},{\"gridX\":333,\"gridY\":96,\"type\":15},{\"gridX\":305,\"gridY\":96,\"type\":11},{\"gridX\":346,\"gridY\":96,\"type\":16},{\"gridX\":93,\"gridY\":96,\"type\":15},{\"gridX\":63,\"gridY\":96,\"type\":16},{\"gridX\":83,\"gridY\":96,\"type\":11},{\"gridX\":168,\"gridY\":108,\"type\":7},{\"gridX\":168,\"gridY\":120,\"type\":7},{\"gridX\":168,\"gridY\":132,\"type\":7},{\"gridX\":192,\"gridY\":132,\"type\":7},{\"gridX\":160,\"gridY\":144,\"type\":8}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":776,\"gridY\":0,\"type\":26},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":808,\"gridY\":0,\"type\":28},{\"gridX\":936,\"gridY\":0,\"type\":28},{\"gridX\":1064,\"gridY\":0,\"type\":28},{\"gridX\":1192,\"gridY\":0,\"type\":28},{\"gridX\":1320,\"gridY\":0,\"type\":28},{\"gridX\":2216,\"gridY\":0,\"type\":28},{\"gridX\":2344,\"gridY\":0,\"type\":28},{\"gridX\":1448,\"gridY\":0,\"type\":28},{\"gridX\":1576,\"gridY\":0,\"type\":28},{\"gridX\":1704,\"gridY\":0,\"type\":28},{\"gridX\":1832,\"gridY\":0,\"type\":28},{\"gridX\":1960,\"gridY\":0,\"type\":28},{\"gridX\":2088,\"gridY\":0,\"type\":28},{\"gridX\":2472,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":144,\"gridY\":0,\"type\":31},{\"gridX\":480,\"gridY\":0,\"type\":31},{\"gridX\":568,\"gridY\":0,\"type\":32},{\"gridX\":1268,\"gridY\":0,\"type\":33},{\"gridX\":1824,\"gridY\":0,\"type\":30},{\"gridX\":1892,\"gridY\":0,\"type\":33},{\"gridX\":929,\"gridY\":0,\"type\":30},{\"gridX\":953,\"gridY\":6,\"type\":34},{\"gridX\":977,\"gridY\":6,\"type\":35},{\"gridX\":1009,\"gridY\":6,\"type\":35},{\"gridX\":1041,\"gridY\":6,\"type\":35},{\"gridX\":1073,\"gridY\":6,\"type\":35},{\"gridX\":1105,\"gridY\":6,\"type\":35},{\"gridX\":1137,\"gridY\":6,\"type\":35},{\"gridX\":1169,\"gridY\":6,\"type\":35},{\"gridX\":1201,\"gridY\":6,\"type\":35},{\"gridX\":1265,\"gridY\":6,\"type\":36},{\"gridX\":1233,\"gridY\":6,\"type\":35}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":2216,\"gridY\":96,\"type\":12},{\"gridX\":192,\"gridY\":124,\"type\":34},{\"gridX\":168,\"gridY\":96,\"type\":34},{\"gridX\":980,\"gridY\":96,\"type\":19},{\"gridX\":1256,\"gridY\":96,\"type\":41},{\"gridX\":1228,\"gridY\":200,\"type\":34},{\"gridX\":1276,\"gridY\":164,\"type\":34},{\"gridX\":1260,\"gridY\":174,\"type\":34},{\"gridX\":1244,\"gridY\":188,\"type\":34},{\"gridX\":1592,\"gridY\":96,\"type\":7},{\"gridX\":1696,\"gridY\":120,\"type\":34},{\"gridX\":1712,\"gridY\":120,\"type\":34},{\"gridX\":1816,\"gridY\":96,\"type\":19},{\"gridX\":651,\"gridY\":147,\"type\":59},{\"gridX\":656,\"gridY\":150,\"type\":60},{\"gridX\":642,\"gridY\":163,\"type\":59},{\"gridX\":640,\"gridY\":181,\"type\":60},{\"gridX\":657,\"gridY\":172,\"type\":58},{\"gridX\":655,\"gridY\":181,\"type\":60},{\"gridX\":690,\"gridY\":124,\"type\":61},{\"gridX\":668,\"gridY\":124,\"type\":61},{\"gridX\":647,\"gridY\":124,\"type\":61},{\"gridX\":624,\"gridY\":124,\"type\":61},{\"gridX\":898,\"gridY\":96,\"type\":51},{\"gridX\":914,\"gridY\":96,\"type\":51},{\"gridX\":788,\"gridY\":174,\"type\":60},{\"gridX\":783,\"gridY\":115,\"type\":61},{\"gridX\":738,\"gridY\":96,\"type\":41},{\"gridX\":1000,\"gridY\":96,\"type\":53},{\"gridX\":1008,\"gridY\":96,\"type\":55},{\"gridX\":1032,\"gridY\":96,\"type\":55},{\"gridX\":1056,\"gridY\":96,\"type\":55},{\"gridX\":1080,\"gridY\":96,\"type\":54},{\"gridX\":1092,\"gridY\":96,\"type\":56},{\"gridX\":1132,\"gridY\":96,\"type\":53},{\"gridX\":1140,\"gridY\":96,\"type\":55},{\"gridX\":1164,\"gridY\":96,\"type\":55},{\"gridX\":1188,\"gridY\":96,\"type\":56},{\"gridX\":160,\"gridY\":140,\"type\":50},{\"gridX\":176,\"gridY\":140,\"type\"" << ":50},{\"gridX\":192,\"gridY\":140,\"type\":50},{\"gridX\":297,\"gridY\":174,\"type\":62},{\"gridX\":430,\"gridY\":188,\"type\":62},{\"gridX\":1348,\"gridY\":28,\"type\":4},{\"gridX\":1432,\"gridY\":52,\"type\":4},{\"gridX\":1344,\"gridY\":28,\"type\":3},{\"gridX\":1428,\"gridY\":52,\"type\":3},{\"gridX\":1362,\"gridY\":28,\"type\":5},{\"gridX\":1446,\"gridY\":52,\"type\":5}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":2184,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":132,\"gridY\":96,\"type\":4},{\"gridX\":376,\"gridY\":96,\"type\":9},{\"gridX\":540,\"gridY\":96,\"type\":9},{\"gridX\":884,\"gridY\":96,\"type\":4},{\"gridX\":901,\"gridY\":111,\"type\":4},{\"gridX\":914,\"gridY\":111,\"type\":4},{\"gridX\":1108,\"gridY\":96,\"type\":6},{\"gridX\":1640,\"gridY\":96,\"type\":4},{\"gridX\":1780,\"gridY\":96,\"type\":4},{\"gridX\":848,\"gridY\":96,\"type\":10},{\"gridX\":1880,\"gridY\":96,\"type\":4},{\"gridX\":555,\"gridY\":205,\"type\":3}],\"collectibles\":[{\"gridX\":169,\"gridY\":112,\"type\":0},{\"gridX\":177,\"gridY\":112,\"type\":0},{\"gridX\":185,\"gridY\":110,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":228,\"gridY\":104,\"type\":0},{\"gridX\":244,\"gridY\":104,\"type\":0},{\"gridX\":236,\"gridY\":104,\"type\":0},{\"gridX\":262,\"gridY\":134,\"type\":1},{\"gridX\":300,\"gridY\":104,\"type\":0},{\"gridX\":308,\"gridY\":104,\"type\":0},{\"gridX\":316,\"gridY\":104,\"type\":0},{\"gridX\":360,\"gridY\":116,\"type\":0},{\"gridX\":368,\"gridY\":116,\"type\":0},{\"gridX\":376,\"gridY\":116,\"type\":0},{\"gridX\":384,\"gridY\":112,\"type\":0},{\"gridX\":392,\"gridY\":112,\"type\":0},{\"gridX\":400,\"gridY\":112,\"type\":0},{\"gridX\":408,\"gridY\":108,\"type\":0},{\"gridX\":416,\"gridY\":108,\"type\":0},{\"gridX\":424,\"gridY\":108,\"type\":0},{\"gridX\":440,\"gridY\":104,\"type\":0},{\"gridX\":432,\"gridY\":104,\"type\":0},{\"gridX\":448,\"gridY\":104,\"type\":0},{\"gridX\":488,\"gridY\":104,\"type\":0},{\"gridX\":496,\"gridY\":104,\"type\":0},{\"gridX\":504,\"gridY\":104,\"type\":0},{\"gridX\":572,\"gridY\":106,\"type\":0},{\"gridX\":580,\"gridY\":106,\"type\":0},{\"gridX\":588,\"gridY\":105,\"type\":0},{\"gridX\":668,\"gridY\":100,\"type\":0},{\"gridX\":676,\"gridY\":100,\"type\":0},{\"gridX\":684,\"gridY\":98,\"type\":0},{\"gridX\":763,\"gridY\":143,\"type\":0},{\"gridX\":771,\"gridY\":150,\"type\":0},{\"gridX\":782,\"gridY\":155,\"type\":0},{\"gridX\":794,\"gridY\":154,\"type\":0},{\"gridX\":803,\"gridY\":148,\"type\":0},{\"gridX\":932,\"gridY\":128,\"type\":0},{\"gridX\":924,\"gridY\":128,\"type\":0},{\"gridX\":940,\"gridY\":128,\"type\":0},{\"gridX\":964,\"gridY\":124,\"type\":0},{\"gridX\":948,\"gridY\":124,\"type\":0},{\"gridX\":956,\"gridY\":124,\"type\":0},{\"gridX\":996,\"gridY\":124,\"type\":0},{\"gridX\":1004,\"gridY\":132,\"type\":0},{\"gridX\":1012,\"gridY\":140,\"type\":0},{\"gridX\":1020,\"gridY\":148,\"type\":0},{\"gridX\":1028,\"gridY\":156,\"type\":0},{\"gridX\":848,\"gridY\":104,\"type\":0},{\"gridX\":856,\"gridY\":104,\"type\":0},{\"gridX\":840,\"gridY\":104,\"type\":0},{\"gridX\":1072,\"gridY\":156,\"type\":0},{\"gridX\":1080,\"gridY\":148,\"type\":0},{\"gridX\":1088,\"gridY\":140,\"type\":0},{\"gridX\":1096,\"gridY\":132,\"type\":0},{\"gridX\":1104,\"gridY\":124,\"type\":0},{\"gridX\":1196,\"gridY\":108,\"type\":1},{\"gridX\":1288,\"gridY\":140,\"type\":0},{\"gridX\":1296,\"gridY\":140,\"type\":0},{\"gridX\":1304,\"gridY\":140,\"type\":0},{\"gridX\":1336,\"gridY\":132,\"type\":0},{\"gridX\":1344,\"gridY\":132,\"type\":0},{\"gridX\":1352,\"gridY\":132,\"type\":0},{\"gridX\":1384,\"gridY\":124,\"type\":0},{\"gridX\":1392,\"gridY\":124,\"type\":0},{\"gridX\":1400,\"gridY\":124,\"type\":0},{\"gridX\":1432,\"gridY\":116,\"type\":0},{\"gridX\":1440,\"gridY\":116,\"type\":0},{\"gridX\":1448,\"gridY\":116,\"type\":0},{\"gridX\":1480,\"gridY\":108,\"type\":0},{\"gridX\":1488,\"gridY\":108,\"type\":0},{\"gridX\":1496,\"gridY\":108,\"type\":0},{\"gridX\":620,\"gridY\":102,\"type\":0},{\"gridX\":628,\"gridY\":102,\"type\":0},{\"gridX\":636,\"gridY\":101,\"type\":0},{\"gridX\":732,\"gridY\":104,\"type\":0},{\"gridX\":740,\"gridY\":104,\"type\":0},{\"gridX\":724,\"gridY\":104,\"type\":0},{\"gridX\":1568,\"gridY\":104,\"type\":0},{\"gridX\":1552,\"gridY\":104,\"type\":0},{\"gridX\":1560,\"gridY\":104,\"type\":0},{\"gridX\":1592,\"gridY\":120,\"type\":0},{\"gridX\":1600,\"gridY\":128,\"type\":0},{\"gridX\":1608,\"gridY\":136,\"type\":0},{\"gridX\":1616,\"gridY\":128,\"type\":0},{\"gridX\":1624,\"gridY\":120,\"type\":0},{\"gridX\":1656,\"gridY\":120,\"type\":0},{\"gridX\":1660,\"gridY\":128,\"type\":0},{\"gridX\":1664,\"gridY\":136,\"type\":0},{\"gridX\":750,\"gridY\":126,\"type\":0},{\"gridX\":756,\"gridY\":135,\"type\":0},{\"gridX\":745,\"gridY\":117,\"type\":0},{\"gridX\":1716,\"gridY\":140,\"type\":1},{\"gridX\":1812,\"gridY\":104,\"type\":0},{\"gridX\":1796,\"gridY\":112,\"type\":0},{\"gridX\":1804,\"gridY\":108,\"type\":0},{\"gridX\":2041,\"gridY\":147,\"type\":0},{\"gridX\":2034,\"gridY\":144,\"type\":0},{\"gridX\":2040,\"gridY\":141,\"type\":0},{\"gridX\":2047,\"gridY\":147,\"type\":0},{\"gridX\":2046,\"gridY\":141,\"type\":0},{\"gridX\":659,\"gridY\":212,\"type\":3},{\"gridX\":802,\"gridY\":203,\"type\":3},{\"gridX\":1008,\"gridY\":20,\"type\":2},{\"gridX\":1060,\"gridY\":20,\"type\":2},{\"gridX\":1084,\"gridY\":20,\"type\":2},{\"gridX\":1032,\"gridY\":20,\"type\":2},{\"gridX\":1108,\"gridY\":20,\"type\":2},{\"gridX\":1156,\"gridY\":20,\"type\":2},{\"gridX\":1132,\"gridY\":20,\"type\":2}],\"jons\":[{\"gridX\":24,\"gridY\":108}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":394,\"gridY\":92,\"type\":3},{\"gridX\":546,\"gridY\":92,\"type\":1},{\"gridX\":1582,\"gridY\":92,\"type\":1},{\"gridX\":1598,\"gridY\":92,\"type\":2},{\"gridX\":1108,\"gridY\":92,\"type\":3},{\"gridX\":278,\"gridY\":92,\"type\":2},{\"gridX\":207,\"gridY\":85,\"type\":0},{\"gridX\":8,\"gridY\":92,\"type\":2},{\"gridX\":52,\"gridY\":92,\"type\":1},{\"gridX\":943,\"gridY\":92,\"type\":1},{\"gridX\":919,\"gridY\":92,\"type\":3}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level7(text.c_str());
}

void Chapter1Level7::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level7 * Chapter1Level7::getInstance()
{
    return s_pInstance;
}

Chapter1Level7::Chapter1Level7(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 8 ///

Chapter1Level8 * Chapter1Level8::s_pInstance = nullptr;

void Chapter1Level8::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":8,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":56,\"gridY\":96,\"type\":0},{\"gridX\":84,\"gridY\":96,\"type\":2},{\"gridX\":276,\"gridY\":96,\"type\":0},{\"gridX\":212,\"gridY\":96,\"type\":2},{\"gridX\":312,\"gridY\":96,\"type\":1},{\"gridX\":600,\"gridY\":96,\"type\":2},{\"gridX\":660,\"gridY\":96,\"type\":1},{\"gridX\":704,\"gridY\":96,\"type\":0},{\"gridX\":760,\"gridY\":96,\"type\":2},{\"gridX\":817,\"gridY\":96,\"type\":0},{\"gridX\":860,\"gridY\":96,\"type\":1},{\"gridX\":908,\"gridY\":96,\"type\":2},{\"gridX\":1224,\"gridY\":96,\"type\":2},{\"gridX\":1288,\"gridY\":96,\"type\":1},{\"gridX\":168,\"gridY\":96,\"type\":1},{\"gridX\":1356,\"gridY\":96,\"type\":1},{\"gridX\":1400,\"gridY\":96,\"type\":0},{\"gridX\":1436,\"gridY\":96,\"type\":1},{\"gridX\":1592,\"gridY\":96,\"type\":1},{\"gridX\":1508,\"gridY\":96,\"type\":0},{\"gridX\":1652,\"gridY\":96,\"type\":0},{\"gridX\":1764,\"gridY\":96,\"type\":2},{\"gridX\":1816,\"gridY\":96,\"type\":1},{\"gridX\":1856,\"gridY\":96,\"type\":2},{\"gridX\":1912,\"gridY\":96,\"type\":0},{\"gridX\":1940,\"gridY\":96,\"type\":1},{\"gridX\":1980,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":92,\"type\":7},{\"gridX\":264,\"gridY\":104,\"type\":7},{\"gridX\":264,\"gridY\":116,\"type\":7},{\"gridX\":264,\"gridY\":128,\"type\":7},{\"gridX\":264,\"gridY\":140,\"type\":7},{\"gridX\":316,\"gridY\":96,\"type\":7},{\"gridX\":316,\"gridY\":108,\"type\":7},{\"gridX\":316,\"gridY\":120,\"type\":7},{\"gridX\":316,\"gridY\":132,\"type\":7},{\"gridX\":316,\"gridY\":144,\"type\":7},{\"gridX\":316,\"gridY\":156,\"type\":7},{\"gridX\":316,\"gridY\":168,\"type\":7},{\"gridX\":316,\"gridY\":180,\"type\":7},{\"gridX\":368,\"gridY\":188,\"type\":7},{\"gridX\":368,\"gridY\":200,\"type\":7},{\"gridX\":368,\"gridY\":212,\"type\":7},{\"gridX\":440,\"gridY\":108,\"type\":7},{\"gridX\":440,\"gridY\":96,\"type\":7},{\"gridX\":440,\"gridY\":120,\"type\":7},{\"gridX\":440,\"gridY\":132,\"type\":7},{\"gridX\":440,\"gridY\":144,\"type\":7},{\"gridX\":440,\"gridY\":156,\"type\":7},{\"gridX\":440,\"gridY\":168,\"type\":7},{\"gridX\":440,\"gridY\":180,\"type\":7},{\"gridX\":440,\"gridY\":192,\"type\":7},{\"gridX\":440,\"gridY\":204,\"type\":7},{\"gridX\":836,\"gridY\":96,\"type\":7},{\"gridX\":836,\"gridY\":108,\"type\":7},{\"gridX\":836,\"gridY\":168,\"type\":7},{\"gridX\":836,\"gridY\":156,\"type\":7},{\"gridX\":836,\"gridY\":144,\"type\":7},{\"gridX\":836,\"gridY\":132,\"type\":7},{\"gridX\":836,\"gridY\":120,\"type\":7},{\"gridX\":916,\"gridY\":168,\"type\":7},{\"gridX\":916,\"gridY\":156,\"type\":7},{\"gridX\":916,\"gridY\":144,\"type\":7},{\"gridX\":916,\"gridY\":132,\"type\":7},{\"gridX\":916,\"gridY\":120,\"type\":7},{\"gridX\":916,\"gridY\":108,\"type\":7},{\"gridX\":916,\"gridY\":96,\"type\":7},{\"gridX\":1245,\"gridY\":96,\"type\":7},{\"gridX\":1245,\"gridY\":108,\"type\":7},{\"gridX\":1339,\"gridY\":96,\"type\":7},{\"gridX\":1339,\"gridY\":108,\"type\":7},{\"gridX\":1432,\"gridY\":96,\"type\":7},{\"gridX\":1432,\"gridY\":108,\"type\":7},{\"gridX\":1432,\"gridY\":120,\"type\":7},{\"gridX\":1432,\"gridY\":132,\"type\":7},{\"gridX\":1432,\"gridY\":144,\"type\":7},{\"gridX\":1432,\"gridY\":156,\"type\":7},{\"gridX\":1432,\"gridY\":168,\"type\":7},{\"gridX\":1512,\"gridY\":96,\"type\":7},{\"gridX\":1512,\"gridY\":108,\"type\":7},{\"gridX\":1512,\"gridY\":120,\"type\":7},{\"gridX\":1512,\"gridY\":132,\"type\":7},{\"gridX\":1512,\"gridY\":144,\"type\":7},{\"gridX\":1528,\"gridY\":96,\"type\":7},{\"gridX\":1528,\"gridY\":108,\"type\":7},{\"gridX\":1528,\"gridY\":156,\"type\":7},{\"gridX\":1528,\"gridY\":144,\"type\":7},{\"gridX\":1528,\"gridY\":132,\"type\":7},{\"gridX\":1528,\"gridY\":120,\"type\":7},{\"gridX\":1384,\"gridY\":96,\"type\":7},{\"gridX\":1384,\"gridY\":108,\"type\":7},{\"gridX\":1384,\"gridY\":120,\"type\":7},{\"gridX\":1384,\"gridY\":132,\"type\":7},{\"gridX\":368,\"gridY\":96,\"type\":7},{\"gridX\":368,\"gridY\":156,\"type\":7},{\"gridX\":368,\"gridY\":144,\"type\":7},{\"gridX\":368,\"gridY\":132,\"type\":7},{\"gridX\":368,\"gridY\":120,\"type\":7},{\"gridX\":368,\"gridY\":108,\"type\":7},{\"gridX\":368,\"gridY\":180,\"type\":7},{\"gridX\":368,\"gridY\":168,\"type\":7},{\"gridX\":720,\"gridY\":96,\"type\":19},{\"gridX\":1357,\"gridY\":96,\"type\":6},{\"gridX\":1549,\"gridY\":96,\"type\":2},{\"gridX\":1598,\"gridY\":96,\"type\":6},{\"gridX\":1579,\"gridY\":96,\"type\":15},{\"gridX\":1445,\"gridY\":96,\"type\":16}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":136,\"gridY\":0,\"type\":27},{\"gridX\":1992,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":200,\"gridY\":0,\"type\":28},{\"gridX\":328,\"gridY\":0,\"type\":28},{\"gridX\":456,\"gridY\":0,\"type\":28},{\"gridX\":584,\"gridY\":0,\"type\":28},{\"gridX\":712,\"gridY\":0,\"type\":28},{\"gridX\":840,\"gridY\":0,\"type\":28},{\"gridX\":1608,\"gridY\":0,\"type\":28},{\"gridX\":1736,\"gridY\":0,\"type\":28},{\"gridX\":1864,\"gridY\":0,\"type\":28},{\"gridX\":968,\"gridY\":0,\"type\":28},{\"gridX\":1096,\"gridY\":0,\"type\":28},{\"gridX\":1224,\"gridY\":0,\"type\":28},{\"gridX\":1352,\"gridY\":0,\"type\":28},{\"gridX\":1480,\"gridY\":0,\"type\":28},{\"gridX\":2056,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":356,\"gridY\":0,\"type\":33},{\"gridX\":964,\"gridY\":0,\"type\":33},{\"gridX\":948,\"gridY\":28,\"type\":36},{\"gridX\":916,\"gridY\":28,\"type\":35},{\"gridX\":884,\"gridY\":28,\"type\":35},{\"gridX\":852,\"gridY\":28,\"type\":35},{\"gridX\":820,\"gridY\":28,\"type\":35},{\"gridX\":788,\"gridY\":28,\"type\":35},{\"gridX\":756,\"gridY\":28,\"type\":35},{\"gridX\":708,\"gridY\":0,\"type\":30},{\"gridX\":732,\"gridY\":28,\"type\":34}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1772,\"gridY\":96,\"type\":12},{\"gridX\":192,\"gridY\":96,\"type\":41},{\"gridX\":244,\"gridY\":96,\"type\":24},{\"gridX\":276,\"gridY\":96,\"type\":24},{\"gridX\":308,\"gridY\":96,\"type\":24},{\"gridX\":212,\"gridY\":96,\"type\":24},{\"gridX\":340,\"gridY\":96,\"type\":23},{\"gridX\":836,\"gridY\":176,\"type\":34},{\"gridX\":1512,\"gridY\":152,\"type\":34},{\"gridX\":1528,\"gridY\":164,\"type\":34},{\"gridX\":1500,\"gridY\":96,\"type\":24},{\"gridX\":916,\"gridY\":176,\"type\":34},{\"gridX\":1348,\"gridY\":96,\"type\":11},{\"gridX\":1688,\"gridY\":96,\"type\":24},{\"gridX\":1720,\"gridY\":96,\"type\":24},{\"gridX\":1432,\"gridY\":176,\"type\":34},{\"gridX\":260,\"gridY\":148,\"type\":48},{\"gridX\":256,\"gridY\":148,\"type\":47},{\"gridX\":276,\"gridY\":148,\"type\":48},{\"gridX\":292,\"gridY\":148,\"type\":48},{\"gridX\":308,\"gridY\":148,\"type\":48},{\"gridX\":324,\"gridY\":148,\"type\":49},{\"gridX\":316,\"gridY\":188,\"type\":48},{\"gridX\":312,\"gridY\":188,\"type\":47},{\"gridX\":332,\"gridY\":188,\"type\":48},{\"gridX\":348,\"gridY\":188,\"type\":48},{\"gridX\":364,\"gridY\":188,\"type\":48},{\"gridX\":380,\"gridY\":188,\"type\":49},{\"gridX\":368,\"gridY\":220,\"type\":48},{\"gridX\":364,\"gridY\":220,\"type\":47},{\"gridX\":384,\"gridY\":220,\"type\":48},{\"gridX\":400,\"gridY\":220,\"type\":48},{\"gridX\":416,\"gridY\":220,\"type\":48},{\"gridX\":440,\"gridY\":212,\"type\":48},{\"gridX\":456,\"gridY\":212,\"type\":48},{\"gridX\":436,\"gridY\":212,\"type\":47},{\"gridX\":432,\"gridY\":220,\"type\":49},{\"gridX\":828,\"gridY\":96,\"type\":55},{\"gridX\":821,\"gridY\":96,\"type\":53},{\"gridX\":850,\"gridY\":96,\"type\":56},{\"gridX\":850,\"gridY\":141,\"type\":48},{\"gridX\":834,\"gridY\":141,\"type\":48},{\"gridX\":866,\"gridY\":141,\"type\":48},{\"gridX\":882,\"gridY\":141,\"type\":48},{\"gridX\":898,\"gridY\":141,\"type\":48},{\"gridX\":914,\"gridY\":141,\"type\":48},{\"gridX\":830,\"gridY\":141,\"type\":47},{\"gridX\":930,\"gridY\":141,\"type\":49},{\"gridX\":876,\"gridY\":146,\"type\":23},{\"gridX\":859,\"gridY\":146,\"type\":23},{\"gridX\":1382,\"gridY\":141,\"type\":48},{\"gridX\":1378,\"gridY\":141,\"type\":47},{\"gridX\":1434,\"gridY\":145,\"type\":48},{\"gridX\":1430," << "\"gridY\":145,\"type\":47},{\"gridX\":1450,\"gridY\":145,\"type\":48},{\"gridX\":1466,\"gridY\":145,\"type\":49},{\"gridX\":1380,\"gridY\":121,\"type\":48},{\"gridX\":1376,\"gridY\":121,\"type\":47},{\"gridX\":1411,\"gridY\":121,\"type\":49},{\"gridX\":1395,\"gridY\":121,\"type\":48},{\"gridX\":1240,\"gridY\":116,\"type\":46},{\"gridX\":1335,\"gridY\":117,\"type\":46},{\"gridX\":720,\"gridY\":112,\"type\":52},{\"gridX\":791,\"gridY\":96,\"type\":19},{\"gridX\":752,\"gridY\":96,\"type\":52},{\"gridX\":736,\"gridY\":96,\"type\":52},{\"gridX\":704,\"gridY\":112,\"type\":52},{\"gridX\":974,\"gridY\":31,\"type\":20},{\"gridX\":972,\"gridY\":28,\"type\":50},{\"gridX\":1381,\"gridY\":96,\"type\":10},{\"gridX\":1366,\"gridY\":96,\"type\":21},{\"gridX\":1484,\"gridY\":236,\"type\":1},{\"gridX\":1480,\"gridY\":236,\"type\":0},{\"gridX\":1498,\"gridY\":236,\"type\":1},{\"gridX\":1512,\"gridY\":236,\"type\":1},{\"gridX\":1526,\"gridY\":236,\"type\":1},{\"gridX\":1540,\"gridY\":236,\"type\":2}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1740,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":344,\"gridY\":140,\"type\":3},{\"gridX\":960,\"gridY\":128,\"type\":3},{\"gridX\":1024,\"gridY\":112,\"type\":3},{\"gridX\":1088,\"gridY\":112,\"type\":3},{\"gridX\":1176,\"gridY\":96,\"type\":3},{\"gridX\":380,\"gridY\":132,\"type\":3},{\"gridX\":429,\"gridY\":128,\"type\":3},{\"gridX\":484,\"gridY\":120,\"type\":3},{\"gridX\":524,\"gridY\":100,\"type\":3},{\"gridX\":568,\"gridY\":100,\"type\":3},{\"gridX\":152,\"gridY\":96,\"type\":4},{\"gridX\":1328,\"gridY\":96,\"type\":4},{\"gridX\":1468,\"gridY\":96,\"type\":10},{\"gridX\":1575,\"gridY\":96,\"type\":8},{\"gridX\":1622,\"gridY\":96,\"type\":8},{\"gridX\":1667,\"gridY\":96,\"type\":8},{\"gridX\":334,\"gridY\":193,\"type\":4},{\"gridX\":842,\"gridY\":37,\"type\":8},{\"gridX\":878,\"gridY\":37,\"type\":9},{\"gridX\":901,\"gridY\":36,\"type\":8},{\"gridX\":951,\"gridY\":36,\"type\":8}],\"collectibles\":[{\"gridX\":732,\"gridY\":137,\"type\":0},{\"gridX\":794,\"gridY\":117,\"type\":0},{\"gridX\":797,\"gridY\":128,\"type\":0},{\"gridX\":1060,\"gridY\":148,\"type\":0},{\"gridX\":996,\"gridY\":160,\"type\":0},{\"gridX\":1136,\"gridY\":148,\"type\":0},{\"gridX\":1220,\"gridY\":136,\"type\":0},{\"gridX\":1208,\"gridY\":128,\"type\":0},{\"gridX\":976,\"gridY\":148,\"type\":0},{\"gridX\":1016,\"gridY\":148,\"type\":0},{\"gridX\":1080,\"gridY\":136,\"type\":0},{\"gridX\":1116,\"gridY\":136,\"type\":0},{\"gridX\":1156,\"gridY\":136,\"type\":0},{\"gridX\":1196,\"gridY\":120,\"type\":0},{\"gridX\":1292,\"gridY\":104,\"type\":0},{\"gridX\":1284,\"gridY\":104,\"type\":0},{\"gridX\":1300,\"gridY\":104,\"type\":0},{\"gridX\":932,\"gridY\":124,\"type\":1},{\"gridX\":741,\"gridY\":138,\"type\":0},{\"gridX\":748,\"gridY\":137,\"type\":0},{\"gridX\":801,\"gridY\":139,\"type\":0},{\"gridX\":896,\"gridY\":104,\"type\":0},{\"gridX\":904,\"gridY\":104,\"type\":0},{\"gridX\":268,\"gridY\":156,\"type\":0},{\"gridX\":276,\"gridY\":156,\"type\":0},{\"gridX\":284,\"gridY\":156,\"type\":0},{\"gridX\":308,\"gridY\":156,\"type\":0},{\"gridX\":300,\"gridY\":156,\"type\":0},{\"gridX\":292,\"gridY\":156,\"type\":0},{\"gridX\":340,\"gridY\":156,\"type\":0},{\"gridX\":348,\"gridY\":152,\"type\":0},{\"gridX\":356,\"gridY\":156,\"type\":0},{\"gridX\":384,\"gridY\":152,\"type\":0},{\"gridX\":388,\"gridY\":144,\"type\":0},{\"gridX\":392,\"gridY\":152,\"type\":0},{\"gridX\":428,\"gridY\":148,\"type\":0},{\"gridX\":432,\"gridY\":140,\"type\":0},{\"gridX\":436,\"gridY\":148,\"type\":0},{\"gridX\":524,\"gridY\":120,\"type\":0},{\"gridX\":528,\"gridY\":112,\"type\":0},{\"gridX\":532,\"gridY\":120,\"type\":0},{\"gridX\":484,\"gridY\":140,\"type\":0},{\"gridX\":488,\"gridY\":132,\"type\":0},{\"gridX\":148,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":112,\"type\":0},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":140,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":492,\"gridY\":140,\"type\":0},{\"gridX\":568,\"gridY\":120,\"type\":0},{\"gridX\":572,\"gridY\":112,\"type\":0},{\"gridX\":576,\"gridY\":120,\"type\":0},{\"gridX\":632,\"gridY\":104,\"type\":0},{\"gridX\":640,\"gridY\":104,\"type\":0},{\"gridX\":648,\"gridY\":104,\"type\":0},{\"gridX\":912,\"gridY\":104,\"type\":0},{\"gridX\":1028,\"gridY\":124,\"type\":0},{\"gridX\":1092,\"gridY\":124,\"type\":0},{\"gridX\":1180,\"gridY\":108,\"type\":0},{\"gridX\":1040,\"gridY\":136,\"type\":0},{\"gridX\":1333,\"gridY\":128,\"type\":0},{\"gridX\":1340,\"gridY\":128,\"type\":0},{\"gridX\":1346,\"gridY\":128,\"type\":0},{\"gridX\":1380,\"gridY\":152,\"type\":0},{\"gridX\":1388,\"gridY\":152,\"type\":0},{\"gridX\":1396,\"gridY\":152,\"type\":0},{\"gridX\":1452,\"gridY\":152,\"type\":0},{\"gridX\":1460,\"gridY\":152,\"type\":0},{\"gridX\":1444,\"gridY\":152,\"type\":0},{\"gridX\":1528,\"gridY\":144,\"type\":1},{\"gridX\":848,\"gridY\":116,\"type\":0},{\"gridX\":856,\"gridY\":112,\"type\":0},{\"gridX\":864,\"gridY\":108,\"type\":0},{\"gridX\":872,\"gridY\":104,\"type\":0},{\"gridX\":1500,\"gridY\":132,\"type\":0},{\"gridX\":1484,\"gridY\":148,\"type\":0},{\"gridX\":1492,\"gridY\":140,\"type\":0},{\"gridX\":1403,\"gridY\":148,\"type\":0},{\"gridX\":1418,\"gridY\":137,\"type\":0},{\"gridX\":1412,\"gridY\":144,\"type\":0},{\"gridX\":1592,\"gridY\":128,\"type\":0},{\"gridX\":1544,\"gridY\":136,\"type\":0},{\"gridX\":1552,\"gridY\":128,\"type\":0},{\"gridX\":1560,\"gridY\":120,\"type\":0},{\"gridX\":1748,\"gridY\":120,\"type\":0},{\"gridX\":1580,\"gridY\":120,\"type\":0},{\"gridX\":1604,\"gridY\":120,\"type\":0},{\"gridX\":1628,\"gridY\":120,\"type\":0},{\"gridX\":1568,\"gridY\":112,\"type\":0},{\"gridX\":1616,\"gridY\":112,\"type\":0},{\"gridX\":1696,\"gridY\":128,\"type\":0},{\"gridX\":1640,\"gridY\":128,\"type\":0},{\"gridX\":1676,\"gridY\":120,\"type\":0},{\"gridX\":1756,\"gridY\":120,\"type\":0},{\"gridX\":1664,\"gridY\":112,\"type\":0},{\"gridX\":1652,\"gridY\":120,\"type\":0},{\"gridX\":1508,\"gridY\":124,\"type\":0},{\"gridX\":1764,\"gridY\":120,\"type\":0},{\"gridX\":1688,\"gridY\":128,\"type\":0},{\"gridX\":1704,\"gridY\":128,\"type\":0},{\"gridX\":1340,\"gridY\":107,\"type\":1},{\"gridX\":757,\"gridY\":136,\"type\":0},{\"gridX\":724,\"gridY\":100,\"type\":3},{\"gridX\":776,\"gridY\":40,\"type\":2},{\"gridX\":816,\"gridY\":40,\"type\":2},{\"gridX\":860,\"gridY\":40,\"type\":2},{\"gridX\":912,\"gridY\":40,\"type\":2},{\"gridX\":1491,\"gridY\":243,\"type\":2},{\"gridX\":1509,\"gridY\":246,\"type\":3},{\"gridX\":1522,\"gridY\":243,\"type\":2}],\"jons\":[{\"gridX\":20,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":736,\"gridY\":96,\"type\":5},{\"gridX\":752,\"gridY\":96,\"type\":7},{\"gridX\":736,\"gridY\":112,\"type\":4},{\"gridX\":720,\"gridY\":112,\"type\":6},{\"gridX\":752,\"gridY\":112,\"type\":9},{\"gridX\":704,\"gridY\":128,\"type\":8},{\"gridX\":736,\"gridY\":128,\"type\":9},{\"gridX\":720,\"gridY\":128,\"type\":10},{\"gridX\":704,\"gridY\":112,\"type\":4},{\"gridX\":704,\"gridY\":96,\"type\":4},{\"gridX\":696,\"gridY\":92,\"type\":3},{\"gridX\":719,\"gridY\":92,\"type\":1},{\"gridX\":727,\"gridY\":92,\"type\":3},{\"gridX\":1361,\"gridY\":92,\"type\":1},{\"gridX\":1355,\"gridY\":92,\"type\":3}],\"markers\":[{\"gridX\":2088,\"gridY\":0,\"type\":1}]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level8(text.c_str());
}

void Chapter1Level8::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level8 * Chapter1Level8::getInstance()
{
    return s_pInstance;
}

Chapter1Level8::Chapter1Level8(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 9 ///

Chapter1Level9 * Chapter1Level9::s_pInstance = nullptr;

void Chapter1Level9::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":56,\"gridY\":96,\"type\":0},{\"gridX\":88,\"gridY\":96,\"type\":1},{\"gridX\":572,\"gridY\":96,\"type\":2},{\"gridX\":492,\"gridY\":96,\"type\":0},{\"gridX\":992,\"gridY\":96,\"type\":2},{\"gridX\":1204,\"gridY\":96,\"type\":2},{\"gridX\":1553,\"gridY\":96,\"type\":1},{\"gridX\":1528,\"gridY\":96,\"type\":2},{\"gridX\":1758,\"gridY\":96,\"type\":1},{\"gridX\":1711,\"gridY\":96,\"type\":0},{\"gridX\":1640,\"gridY\":96,\"type\":2},{\"gridX\":1784,\"gridY\":96,\"type\":2},{\"gridX\":1844,\"gridY\":96,\"type\":0},{\"gridX\":1880,\"gridY\":96,\"type\":1},{\"gridX\":140,\"gridY\":96,\"type\":0},{\"gridX\":688,\"gridY\":96,\"type\":1},{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":273,\"gridY\":96,\"type\":7},{\"gridX\":273,\"gridY\":108,\"type\":7},{\"gridX\":367,\"gridY\":143,\"type\":7},{\"gridX\":367,\"gridY\":132,\"type\":7},{\"gridX\":367,\"gridY\":120,\"type\":7},{\"gridX\":367,\"gridY\":108,\"type\":7},{\"gridX\":367,\"gridY\":97,\"type\":7},{\"gridX\":793,\"gridY\":121,\"type\":7},{\"gridX\":793,\"gridY\":109,\"type\":7},{\"gridX\":793,\"gridY\":97,\"type\":7},{\"gridX\":1068,\"gridY\":96,\"type\":7},{\"gridX\":1068,\"gridY\":120,\"type\":7},{\"gridX\":1116,\"gridY\":96,\"type\":7},{\"gridX\":1068,\"gridY\":132,\"type\":7},{\"gridX\":1068,\"gridY\":108,\"type\":7},{\"gridX\":1032,\"gridY\":120,\"type\":7},{\"gridX\":1032,\"gridY\":96,\"type\":7},{\"gridX\":1032,\"gridY\":108,\"type\":7},{\"gridX\":1134,\"gridY\":133,\"type\":18},{\"gridX\":1141,\"gridY\":146,\"type\":18},{\"gridX\":1129,\"gridY\":118,\"type\":18},{\"gridX\":1141,\"gridY\":121,\"type\":18},{\"gridX\":1139,\"gridY\":105,\"type\":18},{\"gridX\":1116,\"gridY\":108,\"type\":7},{\"gridX\":1116,\"gridY\":120,\"type\":7},{\"gridX\":1116,\"gridY\":180,\"type\":7},{\"gridX\":1116,\"gridY\":168,\"type\":7},{\"gridX\":1116,\"gridY\":156,\"type\":7},{\"gridX\":1116,\"gridY\":144,\"type\":7},{\"gridX\":1116,\"gridY\":132,\"type\":7},{\"gridX\":1116,\"gridY\":204,\"type\":7},{\"gridX\":1116,\"gridY\":192,\"type\":7},{\"gridX\":1116,\"gridY\":216,\"type\":7},{\"gridX\":1116,\"gridY\":228,\"type\":7},{\"gridX\":1116,\"gridY\":240,\"type\":7},{\"gridX\":1068,\"gridY\":144,\"type\":7},{\"gridX\":1032,\"gridY\":132,\"type\":7},{\"gridX\":1032,\"gridY\":144,\"type\":7},{\"gridX\":1032,\"gridY\":156,\"type\":7},{\"gridX\":1032,\"gridY\":168,\"type\":7},{\"gridX\":1068,\"gridY\":156,\"type\":7},{\"gridX\":1068,\"gridY\":168,\"type\":7},{\"gridX\":1030,\"gridY\":180,\"type\":8},{\"gridX\":952,\"gridY\":180,\"type\":9},{\"gridX\":972,\"gridY\":96,\"type\":7},{\"gridX\":972,\"gridY\":144,\"type\":7},{\"gridX\":972,\"gridY\":132,\"type\":7},{\"gridX\":972,\"gridY\":120,\"type\":7},{\"gridX\":972,\"gridY\":108,\"type\":7},{\"gridX\":972,\"gridY\":168,\"type\":7},{\"gridX\":972,\"gridY\":156,\"type\":7},{\"gridX\":701,\"gridY\":96,\"type\":6},{\"gridX\":583,\"gridY\":96,\"type\":11},{\"gridX\":594,\"gridY\":96,\"type\":16},{\"gridX\":643,\"gridY\":96,\"type\":15},{\"gridX\":1512,\"gridY\":96,\"type\":7},{\"gridX\":1452,\"gridY\":132,\"type\":7},{\"gridX\":1512,\"gridY\":108,\"type\":7},{\"gridX\":1452,\"gridY\":120,\"type\":7},{\"gridX\":1452,\"gridY\":108,\"type\":7},{\"gridX\":1452,\"gridY\":96,\"type\":7},{\"gridX\":1512,\"gridY\":240,\"type\":7},{\"gridX\":1512,\"gridY\":228,\"type\":7},{\"gridX\":1512,\"gridY\":216,\"type\":7},{\"gridX\":1512,\"gridY\":204,\"type\":7},{\"gridX\":1512,\"gridY\":192,\"type\":7},{\"gridX\":1512,\"gridY\":180,\"type\":7},{\"gridX\":1512,\"gridY\":168,\"type\":7},{\"gridX\":1512,\"gridY\":156,\"type\":7},{\"gridX\":1512,\"gridY\":144,\"type\":7},{\"gridX\":1512,\"gridY\":132,\"type\":7},{\"gridX\":1512,\"gridY\":120,\"type\":7},{\"gridX\":1452,\"gridY\":188,\"type\":7},{\"gridX\":1452,\"gridY\":176,\"type\":7},{\"gridX\":1452,\"gridY\":168,\"type\":7},{\"gridX\":1452,\"gridY\":156,\"type\":7},{\"gridX\":1452,\"gridY\":144,\"type\":7},{\"gridX\":1352,\"gridY\":96,\"type\":7},{\"gridX\":1352,\"gridY\":144,\"type\":7},{\"gridX\":1352,\"gridY\":132,\"type\":7},{\"gridX\":1352,\"gridY\":120,\"type\":7},{\"gridX\":1352,\"gridY\":108,\"type\":7},{\"gridX\":1312,\"gridY\":96,\"type\":7},{\"gridX\":1312,\"gridY\":144,\"type\":7},{\"gridX\":1312,\"gridY\":132,\"type\":7},{\"gridX\":1312,\"gridY\":120,\"type\":7},{\"gridX\":1312,\"gridY\":108,\"type\":7},{\"gridX\":1300,\"gridY\":132,\"type\":7},{\"gridX\":1300,\"gridY\":144,\"type\":7},{\"gridX\":1300,\"gridY\":156,\"type\":7},{\"gridX\":1300,\"gridY\":120,\"type\":7},{\"gridX\":1300,\"gridY\":108,\"type\":7},{\"gridX\":1300,\"gridY\":96,\"type\":7},{\"gridX\":1264,\"gridY\":96,\"type\":7},{\"gridX\":1300,\"gridY\":192,\"type\":7},{\"gridX\":1300,\"gridY\":180,\"type\":7},{\"gridX\":1300,\"gridY\":168,\"type\":7},{\"gridX\":1264,\"gridY\":108,\"type\":7},{\"gridX\":1264,\"gridY\":120,\"type\":7},{\"gridX\":1312,\"gridY\":157,\"type\":9},{\"gridX\":1390,\"gridY\":155,\"type\":7},{\"gridX\":1390,\"gridY\":143,\"type\":7},{\"gridX\":1390,\"gridY\":131,\"type\":7},{\"gridX\":1390,\"gridY\":119,\"type\":7},{\"gridX\":1390,\"gridY\":107,\"type\":7},{\"gridX\":1390,\"gridY\":96,\"type\":7},{\"gridX\":1390,\"gridY\":167,\"type\":7},{\"gridX\":876,\"gridY\":118,\"type\":17},{\"gridX\":874,\"gridY\":138,\"type\":17},{\"gridX\":874,\"gridY\":159,\"type\":17},{\"gridX\":867,\"gridY\":105,\"type\":18},{\"gridX\":863,\"gridY\":163,\"type\":18},{\"gridX\":874,\"gridY\":176,\"type\":17},{\"gridX\":892,\"gridY\":135,\"type\":17},{\"gridX\":907,\"gridY\":106,\"type\":18},{\"gridX\":900,\"gridY\":147,\"type\":18},{\"gridX\":911,\"gridY\":157,\"type\":18},{\"gridX\":887,\"gridY\":153,\"type\":18},{\"gridX\":895,\"gridY\":165,\"type\":17},{\"gridX\":908,\"gridY\":174,\"type\":18},{\"gridX\":896,\"gridY\":183,\"type\":17},{\"gridX\":876,\"gridY\":194,\"type\":17},{\"gridX\":866,\"gridY\":186,\"type\":18},{\"gridX\":889,\"gridY\":194,\"type\":18},{\"gridX\":882,\"gridY\":211,\"type\":17},{\"gridX\":866,\"gridY\":214,\"type\":18},{\"gridX\":879,\"gridY\":220,\"type\":18},{\"gridX\":865,\"gridY\":227,\"type\":17},{\"gridX\":892,\"gridY\":113,\"type\":17},{\"gridX\":907,\"gridY\":96,\"type\":17},{\"gridX\":928,\"gridY\":95,\"type\":17},{\"gridX\":1154,\"gridY\":96,\"type\":17},{\"gridX\":1174,\"gridY\":96,\"type\":17},{\"gridX\":1195,\"gridY\":96,\"type\":17},{\"gridX\":1150,\"gridY\":133,\"type\":18},{\"gridX\":1156,\"gridY\":116,\"type\":17},{\"gridX\":1178,\"gridY\":116,\"type\":17},{\"gridX\":1197,\"gridY\":116,\"type\":17},{\"gridX\":1155,\"gridY\":146,\"type\":18},{\"gridX\":1163,\"gridY\":125,\"type\":18},{\"gridX\":1192,\"gridY\":123,\"type\":18},{\"gridX\":929,\"gridY\":109,\"type\":18}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":136,\"gridY\":0,\"type\":26},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":168,\"gridY\":0,\"type\":28},{\"gridX\":296,\"gridY\":0,\"type\":28},{\"gridX\":424,\"gridY\":0,\"type\":28},{\"gridX\":552,\"gridY\":0,\"type\":28},{\"gridX\":680,\"gridY\":0,\"type\":28},{\"gridX\":808,\"gridY\":0,\"type\":28},{\"gridX\":1576,\"gridY\":0,\"type\":28},{\"gridX\":1704,\"gridY\":0,\"type\":28},{\"gridX\":1832,\"gridY\":0,\"type\":28},{\"gridX\":936,\"gridY\":0,\"type\":28},{\"gridX\":1064,\"gridY\":0,\"type\":28},{\"gridX\":1192,\"gridY\":0,\"type\":28},{\"gridX\":1320,\"gridY\":0,\"type\":28},{\"gridX\":1448,\"gridY\":0,\"type\":28},{\"gridX\":1960,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":224,\"gridY\":0,\"type\":33},{\"gridX\":668,\"gridY\":0,\"type\":30},{\"gridX\":1272,\"gridY\":0,\"type\":33},{\"gridX\":1004,\"gridY\":0,\"type\":32},{\"gridX\":1176,\"gridY\":0,\"type\":30},{\"gridX\":544,\"gridY\":0,\"type\":30},{\"gridX\":726,\"gridY\":0,\"type\":33},{\"gridX\":1256,\"gridY\":16,\"type\":36},{\"gridX\":1224,\"gridY\":16,\"type\":35},{\"gridX\":1200,\"gridY\":16,\"type\":34},{\"gridX\":1519,\"gridY\":28,\"type\":34},{\"gridX\":1543,\"gridY\":28,\"type\":35},{\"gridX\":1575,\"gridY\":28,\"type\":35},{\"gridX\":1607,\"gridY\":28,\"type\":35},{\"gridX\":" << "1639,\"gridY\":28,\"type\":35},{\"gridX\":1671,\"gridY\":28,\"type\":35},{\"gridX\":1703,\"gridY\":28,\"type\":35},{\"gridX\":1735,\"gridY\":28,\"type\":35},{\"gridX\":1767,\"gridY\":28,\"type\":35},{\"gridX\":1799,\"gridY\":28,\"type\":35},{\"gridX\":1831,\"gridY\":28,\"type\":35},{\"gridX\":1863,\"gridY\":28,\"type\":35},{\"gridX\":1895,\"gridY\":28,\"type\":35},{\"gridX\":1927,\"gridY\":28,\"type\":35}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1696,\"gridY\":96,\"type\":12},{\"gridX\":128,\"gridY\":104,\"type\":34},{\"gridX\":140,\"gridY\":116,\"type\":34},{\"gridX\":152,\"gridY\":128,\"type\":34},{\"gridX\":652,\"gridY\":96,\"type\":23},{\"gridX\":1512,\"gridY\":196,\"type\":34},{\"gridX\":1512,\"gridY\":180,\"type\":34},{\"gridX\":1512,\"gridY\":164,\"type\":34},{\"gridX\":1512,\"gridY\":148,\"type\":34},{\"gridX\":1512,\"gridY\":132,\"type\":34},{\"gridX\":1512,\"gridY\":116,\"type\":34},{\"gridX\":1512,\"gridY\":212,\"type\":34},{\"gridX\":1512,\"gridY\":228,\"type\":34},{\"gridX\":269,\"gridY\":115,\"type\":46},{\"gridX\":363,\"gridY\":152,\"type\":48},{\"gridX\":359,\"gridY\":152,\"type\":47},{\"gridX\":379,\"gridY\":152,\"type\":49},{\"gridX\":789,\"gridY\":128,\"type\":46},{\"gridX\":1028,\"gridY\":128,\"type\":46},{\"gridX\":1134,\"gridY\":96,\"type\":58},{\"gridX\":1122,\"gridY\":106,\"type\":60},{\"gridX\":1146,\"gridY\":154,\"type\":60},{\"gridX\":1064,\"gridY\":139,\"type\":46},{\"gridX\":1110,\"gridY\":215,\"type\":46},{\"gridX\":179,\"gridY\":97,\"type\":55},{\"gridX\":156,\"gridY\":97,\"type\":55},{\"gridX\":202,\"gridY\":97,\"type\":54},{\"gridX\":213,\"gridY\":96,\"type\":56},{\"gridX\":149,\"gridY\":97,\"type\":53},{\"gridX\":1166,\"gridY\":171,\"type\":61},{\"gridX\":1312,\"gridY\":153,\"type\":50},{\"gridX\":1328,\"gridY\":153,\"type\":50},{\"gridX\":1344,\"gridY\":153,\"type\":50},{\"gridX\":1304,\"gridY\":200,\"type\":48},{\"gridX\":1320,\"gridY\":200,\"type\":49},{\"gridX\":1284,\"gridY\":200,\"type\":47},{\"gridX\":1382,\"gridY\":177,\"type\":47},{\"gridX\":1386,\"gridY\":177,\"type\":48},{\"gridX\":1401,\"gridY\":177,\"type\":49},{\"gridX\":1430,\"gridY\":198,\"type\":48},{\"gridX\":1426,\"gridY\":198,\"type\":47},{\"gridX\":1446,\"gridY\":198,\"type\":48},{\"gridX\":1462,\"gridY\":198,\"type\":48},{\"gridX\":1478,\"gridY\":198,\"type\":48},{\"gridX\":1494,\"gridY\":198,\"type\":49},{\"gridX\":1259,\"gridY\":130,\"type\":48},{\"gridX\":1255,\"gridY\":130,\"type\":47},{\"gridX\":1274,\"gridY\":130,\"type\":48},{\"gridX\":1289,\"gridY\":130,\"type\":49},{\"gridX\":1288,\"gridY\":200,\"type\":48},{\"gridX\":864,\"gridY\":99,\"type\":60},{\"gridX\":891,\"gridY\":99,\"type\":60},{\"gridX\":869,\"gridY\":142,\"type\":61},{\"gridX\":868,\"gridY\":231,\"type\":60},{\"gridX\":850,\"gridY\":248,\"type\":61},{\"gridX\":915,\"gridY\":173,\"type\":60},{\"gridX\":857,\"gridY\":179,\"type\":60},{\"gridX\":935,\"gridY\":116,\"type\":60},{\"gridX\":931,\"gridY\":21,\"type\":62},{\"gridX\":965,\"gridY\":41,\"type\":50},{\"gridX\":969,\"gridY\":44,\"type\":20},{\"gridX\":1207,\"gridY\":96,\"type\":58},{\"gridX\":1196,\"gridY\":104,\"type\":59},{\"gridX\":1198,\"gridY\":126,\"type\":60},{\"gridX\":1205,\"gridY\":116,\"type\":59},{\"gridX\":1379,\"gridY\":2,\"type\":4},{\"gridX\":1393,\"gridY\":2,\"type\":4},{\"gridX\":1407,\"gridY\":2,\"type\":4},{\"gridX\":1421,\"gridY\":2,\"type\":5},{\"gridX\":1375,\"gridY\":2,\"type\":3},{\"gridX\":1512,\"gridY\":99,\"type\":34},{\"gridX\":1312,\"gridY\":138,\"type\":34},{\"gridX\":1312,\"gridY\":123,\"type\":34},{\"gridX\":1312,\"gridY\":105,\"type\":34},{\"gridX\":1390,\"gridY\":106,\"type\":34},{\"gridX\":1390,\"gridY\":123,\"type\":34},{\"gridX\":1390,\"gridY\":140,\"type\":34},{\"gridX\":1390,\"gridY\":158,\"type\":34},{\"gridX\":1264,\"gridY\":102,\"type\":34},{\"gridX\":1264,\"gridY\":118,\"type\":34},{\"gridX\":1452,\"gridY\":184,\"type\":34},{\"gridX\":1452,\"gridY\":168,\"type\":34},{\"gridX\":1452,\"gridY\":152,\"type\":34},{\"gridX\":1452,\"gridY\":136,\"type\":34},{\"gridX\":1452,\"gridY\":120,\"type\":34},{\"gridX\":1452,\"gridY\":104,\"type\":34},{\"gridX\":880,\"gridY\":96,\"type\":59},{\"gridX\":877,\"gridY\":82,\"type\":59},{\"gridX\":888,\"gridY\":83,\"type\":59},{\"gridX\":942,\"gridY\":92,\"type\":63},{\"gridX\":881,\"gridY\":67,\"type\":63},{\"gridX\":882,\"gridY\":55,\"type\":59},{\"gridX\":942,\"gridY\":80,\"type\":59},{\"gridX\":942,\"gridY\":103,\"type\":59}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1664,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":269,\"gridY\":121,\"type\":6},{\"gridX\":484,\"gridY\":96,\"type\":6},{\"gridX\":712,\"gridY\":96,\"type\":6},{\"gridX\":789,\"gridY\":134,\"type\":6},{\"gridX\":913,\"gridY\":196,\"type\":6},{\"gridX\":876,\"gridY\":148,\"type\":6},{\"gridX\":984,\"gridY\":96,\"type\":6},{\"gridX\":1064,\"gridY\":145,\"type\":6},{\"gridX\":1108,\"gridY\":221,\"type\":6},{\"gridX\":1028,\"gridY\":133,\"type\":6},{\"gridX\":1124,\"gridY\":129,\"type\":6},{\"gridX\":1144,\"gridY\":177,\"type\":6},{\"gridX\":632,\"gridY\":96,\"type\":6},{\"gridX\":184,\"gridY\":96,\"type\":6}],\"collectibles\":[{\"gridX\":456,\"gridY\":112,\"type\":0},{\"gridX\":464,\"gridY\":116,\"type\":0},{\"gridX\":480,\"gridY\":116,\"type\":0},{\"gridX\":680,\"gridY\":92,\"type\":1},{\"gridX\":885,\"gridY\":173,\"type\":1},{\"gridX\":876,\"gridY\":204,\"type\":0},{\"gridX\":945,\"gridY\":219,\"type\":0},{\"gridX\":952,\"gridY\":210,\"type\":0},{\"gridX\":960,\"gridY\":200,\"type\":0},{\"gridX\":964,\"gridY\":188,\"type\":0},{\"gridX\":968,\"gridY\":176,\"type\":0},{\"gridX\":868,\"gridY\":204,\"type\":0},{\"gridX\":1004,\"gridY\":152,\"type\":0},{\"gridX\":974,\"gridY\":166,\"type\":0},{\"gridX\":978,\"gridY\":152,\"type\":0},{\"gridX\":983,\"gridY\":140,\"type\":0},{\"gridX\":860,\"gridY\":204,\"type\":0},{\"gridX\":1032,\"gridY\":155,\"type\":0},{\"gridX\":1012,\"gridY\":160,\"type\":0},{\"gridX\":1020,\"gridY\":168,\"type\":0},{\"gridX\":1028,\"gridY\":176,\"type\":0},{\"gridX\":1060,\"gridY\":200,\"type\":0},{\"gridX\":1092,\"gridY\":216,\"type\":0},{\"gridX\":1083,\"gridY\":204,\"type\":0},{\"gridX\":132,\"gridY\":124,\"type\":0},{\"gridX\":140,\"gridY\":132,\"type\":0},{\"gridX\":148,\"gridY\":140,\"type\":0},{\"gridX\":124,\"gridY\":116,\"type\":0},{\"gridX\":180,\"gridY\":124,\"type\":0},{\"gridX\":172,\"gridY\":132,\"type\":0},{\"gridX\":164,\"gridY\":140,\"type\":0},{\"gridX\":156,\"gridY\":148,\"type\":0},{\"gridX\":204,\"gridY\":132,\"type\":0},{\"gridX\":212,\"gridY\":148,\"type\":0},{\"gridX\":188,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":140,\"type\":0},{\"gridX\":367,\"gridY\":163,\"type\":0},{\"gridX\":358,\"gridY\":163,\"type\":0},{\"gridX\":488,\"gridY\":112,\"type\":0},{\"gridX\":472,\"gridY\":120,\"type\":0},{\"gridX\":580,\"gridY\":104,\"type\":0},{\"gridX\":568,\"gridY\":128,\"type\":0},{\"gridX\":576,\"gridY\":112,\"type\":0},{\"gridX\":572,\"gridY\":120,\"type\":0},{\"gridX\":748,\"gridY\":168,\"type\":0},{\"gridX\":752,\"gridY\":176,\"type\":0},{\"gridX\":732,\"gridY\":136,\"type\":0},{\"gridX\":736,\"gridY\":144,\"type\":0},{\"gridX\":740,\"gridY\":152,\"type\":0},{\"gridX\":1069,\"gridY\":168,\"type\":0},{\"gridX\":1071,\"gridY\":180,\"type\":0},{\"gridX\":1076,\"gridY\":192,\"type\":0},{\"gridX\":1096,\"gridY\":204,\"type\":0},{\"gridX\":1104,\"gridY\":192,\"type\":0},{\"gridX\":1112,\"gridY\":180,\"type\":0},{\"gridX\":1120,\"gridY\":168,\"type\":0},{\"gridX\":1128,\"gridY\":156,\"type\":0},{\"gridX\":1052,\"gridY\":184,\"type\":0},{\"gridX\":1056,\"gridY\":192,\"type\":0},{\"gridX\":1285,\"gridY\":136,\"type\":1},{\"gridX\":1402,\"gridY\":199,\"type\":0},{\"gridX\":1380,\"gridY\":194,\"type\":0},{\"gridX\":1352,\"gridY\":165,\"type\":0},{\"gridX\":1409,\"gridY\":210,\"type\":0},{\"gridX\":1368,\"gridY\":192,\"type\":0},{\"gridX\":1360,\"gridY\":184,\"type\":0},{\"gridX\":1468,\"gridY\":208,\"type\":0},{\"gridX\":1460,\"gridY\":208,\"type\":0},{\"" << "gridX\":1476,\"gridY\":208,\"type\":0},{\"gridX\":1556,\"gridY\":212,\"type\":0},{\"gridX\":1526,\"gridY\":244,\"type\":0},{\"gridX\":1532,\"gridY\":236,\"type\":0},{\"gridX\":1540,\"gridY\":228,\"type\":0},{\"gridX\":1548,\"gridY\":220,\"type\":0},{\"gridX\":990,\"gridY\":117,\"type\":0},{\"gridX\":1355,\"gridY\":175,\"type\":0},{\"gridX\":1390,\"gridY\":189,\"type\":0},{\"gridX\":812,\"gridY\":196,\"type\":0},{\"gridX\":816,\"gridY\":204,\"type\":0},{\"gridX\":744,\"gridY\":160,\"type\":0},{\"gridX\":492,\"gridY\":124,\"type\":0},{\"gridX\":820,\"gridY\":212,\"type\":0},{\"gridX\":800,\"gridY\":172,\"type\":0},{\"gridX\":804,\"gridY\":180,\"type\":0},{\"gridX\":808,\"gridY\":188,\"type\":0},{\"gridX\":937,\"gridY\":223,\"type\":0},{\"gridX\":920,\"gridY\":227,\"type\":0},{\"gridX\":929,\"gridY\":225,\"type\":0},{\"gridX\":987,\"gridY\":130,\"type\":0},{\"gridX\":1564,\"gridY\":204,\"type\":0},{\"gridX\":564,\"gridY\":136,\"type\":0},{\"gridX\":496,\"gridY\":132,\"type\":0},{\"gridX\":500,\"gridY\":140,\"type\":0},{\"gridX\":504,\"gridY\":148,\"type\":0},{\"gridX\":508,\"gridY\":156,\"type\":0},{\"gridX\":1030,\"gridY\":147,\"type\":0},{\"gridX\":376,\"gridY\":163,\"type\":0},{\"gridX\":1450,\"gridY\":208,\"type\":0},{\"gridX\":1181,\"gridY\":181,\"type\":0},{\"gridX\":1184,\"gridY\":192,\"type\":0},{\"gridX\":1191,\"gridY\":203,\"type\":0},{\"gridX\":1199,\"gridY\":213,\"type\":0},{\"gridX\":1210,\"gridY\":220,\"type\":0},{\"gridX\":939,\"gridY\":37,\"type\":3},{\"gridX\":890,\"gridY\":125,\"type\":2},{\"gridX\":1208,\"gridY\":28,\"type\":2},{\"gridX\":1220,\"gridY\":28,\"type\":2},{\"gridX\":1232,\"gridY\":28,\"type\":2},{\"gridX\":1268,\"gridY\":28,\"type\":2},{\"gridX\":1244,\"gridY\":28,\"type\":2},{\"gridX\":1256,\"gridY\":28,\"type\":2},{\"gridX\":1695,\"gridY\":43,\"type\":3}],\"jons\":[{\"gridX\":25,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":696,\"gridY\":92,\"type\":3}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level9(text.c_str());
}

void Chapter1Level9::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level9 * Chapter1Level9::getInstance()
{
    return s_pInstance;
}

Chapter1Level9::Chapter1Level9(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 11 ///

Chapter1Level11 * Chapter1Level11::s_pInstance = nullptr;

void Chapter1Level11::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":11,\"midgrounds\":[{\"gridX\":2108,\"gridY\":96,\"type\":2},{\"gridX\":2228,\"gridY\":96,\"type\":1},{\"gridX\":2268,\"gridY\":96,\"type\":2},{\"gridX\":88,\"gridY\":96,\"type\":2},{\"gridX\":444,\"gridY\":96,\"type\":2},{\"gridX\":764,\"gridY\":96,\"type\":2},{\"gridX\":1180,\"gridY\":96,\"type\":2},{\"gridX\":1232,\"gridY\":96,\"type\":2},{\"gridX\":1428,\"gridY\":96,\"type\":2},{\"gridX\":2020,\"gridY\":96,\"type\":2},{\"gridX\":256,\"gridY\":96,\"type\":2},{\"gridX\":344,\"gridY\":96,\"type\":2},{\"gridX\":145,\"gridY\":96,\"type\":13},{\"gridX\":50,\"gridY\":96,\"type\":14},{\"gridX\":68,\"gridY\":96,\"type\":15},{\"gridX\":226,\"gridY\":96,\"type\":14},{\"gridX\":327,\"gridY\":96,\"type\":16},{\"gridX\":255,\"gridY\":96,\"type\":11},{\"gridX\":607,\"gridY\":96,\"type\":13},{\"gridX\":636,\"gridY\":96,\"type\":14},{\"gridX\":738,\"gridY\":96,\"type\":13},{\"gridX\":642,\"gridY\":96,\"type\":15},{\"gridX\":575,\"gridY\":96,\"type\":6},{\"gridX\":654,\"gridY\":96,\"type\":2},{\"gridX\":846,\"gridY\":96,\"type\":13},{\"gridX\":941,\"gridY\":96,\"type\":14},{\"gridX\":867,\"gridY\":96,\"type\":15},{\"gridX\":824,\"gridY\":96,\"type\":16},{\"gridX\":915,\"gridY\":96,\"type\":17},{\"gridX\":935,\"gridY\":96,\"type\":17},{\"gridX\":904,\"gridY\":105,\"type\":18},{\"gridX\":935,\"gridY\":105,\"type\":18},{\"gridX\":911,\"gridY\":123,\"type\":17},{\"gridX\":911,\"gridY\":133,\"type\":18},{\"gridX\":946,\"gridY\":96,\"type\":18},{\"gridX\":921,\"gridY\":112,\"type\":18},{\"gridX\":1185,\"gridY\":96,\"type\":17},{\"gridX\":1206,\"gridY\":96,\"type\":17},{\"gridX\":1180,\"gridY\":108,\"type\":18},{\"gridX\":1197,\"gridY\":112,\"type\":18},{\"gridX\":1190,\"gridY\":123,\"type\":17},{\"gridX\":1207,\"gridY\":105,\"type\":18},{\"gridX\":1185,\"gridY\":134,\"type\":18},{\"gridX\":1197,\"gridY\":142,\"type\":18},{\"gridX\":1201,\"gridY\":128,\"type\":18},{\"gridX\":1324,\"gridY\":120,\"type\":10},{\"gridX\":1372,\"gridY\":120,\"type\":10},{\"gridX\":588,\"gridY\":96,\"type\":7},{\"gridX\":588,\"gridY\":108,\"type\":7},{\"gridX\":588,\"gridY\":120,\"type\":7},{\"gridX\":588,\"gridY\":132,\"type\":7},{\"gridX\":1366,\"gridY\":96,\"type\":14},{\"gridX\":1355,\"gridY\":96,\"type\":15},{\"gridX\":1259,\"gridY\":96,\"type\":16},{\"gridX\":1312,\"gridY\":96,\"type\":13},{\"gridX\":1756,\"gridY\":96,\"type\":16},{\"gridX\":1733,\"gridY\":96,\"type\":15},{\"gridX\":1867,\"gridY\":96,\"type\":14},{\"gridX\":1721,\"gridY\":96,\"type\":13},{\"gridX\":1949,\"gridY\":96,\"type\":15},{\"gridX\":1921,\"gridY\":96,\"type\":13},{\"gridX\":1328,\"gridY\":96,\"type\":7},{\"gridX\":1328,\"gridY\":108,\"type\":7},{\"gridX\":1408,\"gridY\":96,\"type\":7},{\"gridX\":1408,\"gridY\":108,\"type\":7},{\"gridX\":902,\"gridY\":136,\"type\":18}],\"grounds\":[{\"gridX\":4,\"gridY\":50,\"type\":10},{\"gridX\":488,\"gridY\":50,\"type\":10},{\"gridX\":1816,\"gridY\":50,\"type\":10},{\"gridX\":688,\"gridY\":50,\"type\":11},{\"gridX\":268,\"gridY\":50,\"type\":12},{\"gridX\":624,\"gridY\":50,\"type\":12},{\"gridX\":12,\"gridY\":50,\"type\":13},{\"gridX\":140,\"gridY\":50,\"type\":13},{\"gridX\":332,\"gridY\":50,\"type\":13},{\"gridX\":496,\"gridY\":50,\"type\":13},{\"gridX\":1824,\"gridY\":50,\"type\":13},{\"gridX\":1952,\"gridY\":50,\"type\":13},{\"gridX\":320,\"gridY\":48,\"type\":15},{\"gridX\":328,\"gridY\":48,\"type\":18},{\"gridX\":456,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":494,\"gridY\":90,\"type\":20},{\"gridX\":264,\"gridY\":90,\"type\":21},{\"gridX\":392,\"gridY\":90,\"type\":21},{\"gridX\":488,\"gridY\":90,\"type\":22},{\"gridX\":328,\"gridY\":90,\"type\":22},{\"gridX\":552,\"gridY\":90,\"type\":22},{\"gridX\":616,\"gridY\":90,\"type\":22},{\"gridX\":424,\"gridY\":90,\"type\":22},{\"gridX\":1964,\"gridY\":90,\"type\":22},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":1836,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":2052,\"gridY\":0,\"type\":25},{\"gridX\":706,\"gridY\":0,\"type\":25},{\"gridX\":1418,\"gridY\":0,\"type\":26},{\"gridX\":1354,\"gridY\":0,\"type\":27},{\"gridX\":2060,\"gridY\":0,\"type\":27},{\"gridX\":2124,\"gridY\":0,\"type\":28},{\"gridX\":842,\"gridY\":0,\"type\":28},{\"gridX\":970,\"gridY\":0,\"type\":28},{\"gridX\":1226,\"gridY\":0,\"type\":28},{\"gridX\":2252,\"gridY\":0,\"type\":28},{\"gridX\":714,\"gridY\":0,\"type\":28},{\"gridX\":1450,\"gridY\":0,\"type\":28},{\"gridX\":1578,\"gridY\":0,\"type\":28},{\"gridX\":1706,\"gridY\":0,\"type\":28},{\"gridX\":1098,\"gridY\":0,\"type\":28},{\"gridX\":2380,\"gridY\":0,\"type\":29},{\"gridX\":1834,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":919,\"gridY\":0,\"type\":30},{\"gridX\":942,\"gridY\":20,\"type\":34},{\"gridX\":822,\"gridY\":0,\"type\":31},{\"gridX\":878,\"gridY\":6,\"type\":34},{\"gridX\":902,\"gridY\":6,\"type\":36},{\"gridX\":981,\"gridY\":0,\"type\":31},{\"gridX\":966,\"gridY\":20,\"type\":36},{\"gridX\":1035,\"gridY\":30,\"type\":34},{\"gridX\":1073,\"gridY\":0,\"type\":31},{\"gridX\":1059,\"gridY\":30,\"type\":36},{\"gridX\":1468,\"gridY\":0,\"type\":33}],\"exitGrounds\":[{\"gridX\":296,\"gridY\":88,\"type\":0},{\"gridX\":680,\"gridY\":88,\"type\":0},{\"gridX\":2028,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":160,\"gridY\":80,\"type\":3},{\"gridX\":620,\"gridY\":80,\"type\":3},{\"gridX\":1844,\"gridY\":73,\"type\":0},{\"gridX\":1860,\"gridY\":73,\"type\":1},{\"gridX\":1876,\"gridY\":73,\"type\":1},{\"gridX\":1892,\"gridY\":73,\"type\":1},{\"gridX\":1908,\"gridY\":73,\"type\":1},{\"gridX\":1924,\"gridY\":73,\"type\":1},{\"gridX\":1940,\"gridY\":73,\"type\":1},{\"gridX\":1956,\"gridY\":73,\"type\":1},{\"gridX\":1972,\"gridY\":73,\"type\":2}],\"foregroundObjects\":[{\"gridX\":2116,\"gridY\":96,\"type\":12},{\"gridX\":428,\"gridY\":95,\"type\":35},{\"gridX\":600,\"gridY\":95,\"type\":35},{\"gridX\":1300,\"gridY\":95,\"type\":35},{\"gridX\":1348,\"gridY\":95,\"type\":35},{\"gridX\":1400,\"gridY\":95,\"type\":35},{\"gridX\":1781,\"gridY\":96,\"type\":24},{\"gridX\":1813,\"gridY\":96,\"type\":24},{\"gridX\":588,\"gridY\":140,\"type\":34},{\"gridX\":181,\"gridY\":96,\"type\":36},{\"gridX\":204,\"gridY\":55,\"type\":26},{\"gridX\":272,\"gridY\":55,\"type\":26},{\"gridX\":324,\"gridY\":75,\"type\":27},{\"gridX\":310,\"gridY\":55,\"type\":41},{\"gridX\":232,\"gridY\":84,\"type\":30},{\"gridX\":320,\"gridY\":96,\"type\":22},{\"gridX\":324,\"gridY\":96,\"type\":22},{\"gridX\":708,\"gridY\":96,\"type\":23},{\"gridX\":2040,\"gridY\":55,\"type\":41},{\"gridX\":1992,\"gridY\":96,\"type\":36},{\"gridX\":951,\"gridY\":96,\"type\":58},{\"gridX\":948,\"gridY\":104,\"type\":60},{\"gridX\":918,\"gridY\":138,\"type\":60},{\"gridX\":981,\"gridY\":167,\"type\":62},{\"gridX\":1183,\"gridY\":143,\"type\":60},{\"gridX\":1191,\"gridY\":136,\"type\":59},{\"gridX\":1205,\"gridY\":143,\"type\":60},{\"gridX\":1199,\"gridY\":137,\"type\":59},{\"gridX\":1107,\"gridY\":159,\"type\":62},{\"gridX\":1537,\"gridY\":89,\"type\":62},{\"gridX\":1624,\"gridY\":0,\"type\":52},{\"gridX\":1624,\"gridY\":16,\"type\":52},{\"gridX\":1624,\"gridY\":32,\"type\":52},{\"gridX\":1624,\"gridY\":48,\"type\":52},{\"gridX\":1624,\"gridY\":64,\"type\":52},{\"gridX\":1624,\"gridY\":80,\"type\":52},{\"gridX\":1616,\"gridY\":96,\"type\":52},{\"gridX\":1632,\"gridY\":96,\"type\":52},{\"gridX\":1616,\"gridY\":94,\"type\":50},{\"gridX\":1632,\"gridY\":94,\"type\":50},{\"gridX\":694,\"gridY\":55,\"type\":41},{\"gridX\":895,\"gridY\":96,\"type\":58},{\"gridX\":887,\"gridY\":105,\"type\":59},{\"gridX\":946,\"gridY\":96,\"type\":59},{\"gridX\":885,\"gridY\":96,\"type\":58},{\"gridX\":903,\"gridY\":155,\"type\":61},{\"gridX\":1046,\"gridY\":96,\"type\":58},{\"gridX\":1039,\"gridY\":104,\"type\":59},{\"gridX\":1046,\"gridY\":117,\"type\":59},{\"gridX\":1050,\"gridY\":131,\"type\":59},{\"gridX\":1049,\"gridY\":142,\"type\":63},{\"gridX\":1049,\"gridY\":152,\"type\":59},{\"gridX\":1055,\"gridY\":163,\"type\":59},{\"gridX\":1048,\"gridY\":171,\"type\":60},{\"gridX\":1052,\"gridY\":" << "108,\"type\":59}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":2084,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":496,\"gridY\":96,\"type\":10},{\"gridX\":564,\"gridY\":96,\"type\":10},{\"gridX\":803,\"gridY\":96,\"type\":10},{\"gridX\":812,\"gridY\":116,\"type\":3},{\"gridX\":844,\"gridY\":128,\"type\":3},{\"gridX\":876,\"gridY\":140,\"type\":3},{\"gridX\":624,\"gridY\":55,\"type\":10},{\"gridX\":668,\"gridY\":55,\"type\":8},{\"gridX\":2076,\"gridY\":96,\"type\":5},{\"gridX\":1381,\"gridY\":96,\"type\":5},{\"gridX\":390,\"gridY\":96,\"type\":10}],\"collectibles\":[{\"gridX\":392,\"gridY\":104,\"type\":0},{\"gridX\":396,\"gridY\":112,\"type\":0},{\"gridX\":400,\"gridY\":120,\"type\":0},{\"gridX\":404,\"gridY\":128,\"type\":0},{\"gridX\":408,\"gridY\":136,\"type\":0},{\"gridX\":412,\"gridY\":144,\"type\":0},{\"gridX\":452,\"gridY\":124,\"type\":0},{\"gridX\":460,\"gridY\":116,\"type\":0},{\"gridX\":481,\"gridY\":119,\"type\":0},{\"gridX\":492,\"gridY\":126,\"type\":0},{\"gridX\":503,\"gridY\":128,\"type\":0},{\"gridX\":514,\"gridY\":126,\"type\":0},{\"gridX\":521,\"gridY\":117,\"type\":0},{\"gridX\":600,\"gridY\":120,\"type\":1},{\"gridX\":699,\"gridY\":107,\"type\":0},{\"gridX\":706,\"gridY\":115,\"type\":0},{\"gridX\":716,\"gridY\":118,\"type\":0},{\"gridX\":725,\"gridY\":115,\"type\":0},{\"gridX\":745,\"gridY\":121,\"type\":0},{\"gridX\":735,\"gridY\":120,\"type\":0},{\"gridX\":877,\"gridY\":158,\"type\":1},{\"gridX\":988,\"gridY\":184,\"type\":0},{\"gridX\":911,\"gridY\":170,\"type\":0},{\"gridX\":981,\"gridY\":190,\"type\":0},{\"gridX\":752,\"gridY\":114,\"type\":0},{\"gridX\":931,\"gridY\":170,\"type\":0},{\"gridX\":921,\"gridY\":170,\"type\":0},{\"gridX\":943,\"gridY\":190,\"type\":0},{\"gridX\":937,\"gridY\":181,\"type\":0},{\"gridX\":953,\"gridY\":195,\"type\":0},{\"gridX\":974,\"gridY\":196,\"type\":0},{\"gridX\":964,\"gridY\":199,\"type\":0},{\"gridX\":999,\"gridY\":197,\"type\":0},{\"gridX\":1022,\"gridY\":215,\"type\":0},{\"gridX\":1008,\"gridY\":208,\"type\":0},{\"gridX\":1037,\"gridY\":215,\"type\":0},{\"gridX\":1052,\"gridY\":203,\"type\":0},{\"gridX\":1046,\"gridY\":210,\"type\":0},{\"gridX\":1060,\"gridY\":198,\"type\":0},{\"gridX\":1082,\"gridY\":192,\"type\":0},{\"gridX\":1071,\"gridY\":197,\"type\":0},{\"gridX\":1204,\"gridY\":104,\"type\":0},{\"gridX\":1188,\"gridY\":104,\"type\":0},{\"gridX\":1196,\"gridY\":104,\"type\":0},{\"gridX\":1220,\"gridY\":104,\"type\":0},{\"gridX\":1212,\"gridY\":104,\"type\":0},{\"gridX\":1228,\"gridY\":104,\"type\":0},{\"gridX\":1276,\"gridY\":104,\"type\":0},{\"gridX\":1280,\"gridY\":112,\"type\":0},{\"gridX\":1284,\"gridY\":120,\"type\":0},{\"gridX\":1288,\"gridY\":128,\"type\":0},{\"gridX\":1292,\"gridY\":136,\"type\":0},{\"gridX\":1316,\"gridY\":128,\"type\":0},{\"gridX\":1320,\"gridY\":120,\"type\":0},{\"gridX\":1329,\"gridY\":125,\"type\":0},{\"gridX\":1365,\"gridY\":132,\"type\":0},{\"gridX\":1334,\"gridY\":120,\"type\":0},{\"gridX\":1342,\"gridY\":129,\"type\":0},{\"gridX\":1392,\"gridY\":128,\"type\":0},{\"gridX\":1373,\"gridY\":127,\"type\":0},{\"gridX\":1382,\"gridY\":122,\"type\":0},{\"gridX\":1354,\"gridY\":133,\"type\":0},{\"gridX\":1403,\"gridY\":130,\"type\":0},{\"gridX\":1430,\"gridY\":115,\"type\":0},{\"gridX\":1629,\"gridY\":124,\"type\":0},{\"gridX\":1422,\"gridY\":123,\"type\":0},{\"gridX\":1413,\"gridY\":128,\"type\":0},{\"gridX\":1628,\"gridY\":118,\"type\":0},{\"gridX\":1548,\"gridY\":107,\"type\":0},{\"gridX\":1544,\"gridY\":101,\"type\":0},{\"gridX\":1540,\"gridY\":106,\"type\":0},{\"gridX\":1714,\"gridY\":94,\"type\":0},{\"gridX\":1714,\"gridY\":79,\"type\":0},{\"gridX\":1714,\"gridY\":86,\"type\":0},{\"gridX\":1784,\"gridY\":112,\"type\":0},{\"gridX\":1792,\"gridY\":112,\"type\":0},{\"gridX\":1800,\"gridY\":112,\"type\":0},{\"gridX\":1820,\"gridY\":108,\"type\":0},{\"gridX\":1828,\"gridY\":108,\"type\":0},{\"gridX\":1836,\"gridY\":108,\"type\":0},{\"gridX\":468,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":64,\"type\":0},{\"gridX\":184,\"gridY\":64,\"type\":0},{\"gridX\":192,\"gridY\":64,\"type\":0},{\"gridX\":236,\"gridY\":64,\"type\":0},{\"gridX\":244,\"gridY\":64,\"type\":0},{\"gridX\":252,\"gridY\":64,\"type\":0},{\"gridX\":296,\"gridY\":64,\"type\":0},{\"gridX\":304,\"gridY\":64,\"type\":0},{\"gridX\":312,\"gridY\":64,\"type\":0},{\"gridX\":696,\"gridY\":77,\"type\":1},{\"gridX\":2012,\"gridY\":64,\"type\":0},{\"gridX\":2020,\"gridY\":64,\"type\":0},{\"gridX\":2028,\"gridY\":64,\"type\":0},{\"gridX\":1892,\"gridY\":64,\"type\":0},{\"gridX\":1900,\"gridY\":64,\"type\":0},{\"gridX\":1908,\"gridY\":64,\"type\":0},{\"gridX\":1936,\"gridY\":64,\"type\":0},{\"gridX\":1944,\"gridY\":64,\"type\":0},{\"gridX\":1952,\"gridY\":64,\"type\":0},{\"gridX\":1976,\"gridY\":64,\"type\":0},{\"gridX\":1984,\"gridY\":64,\"type\":0},{\"gridX\":1992,\"gridY\":64,\"type\":0},{\"gridX\":932,\"gridY\":86,\"type\":3},{\"gridX\":959,\"gridY\":33,\"type\":2},{\"gridX\":1054,\"gridY\":42,\"type\":2},{\"gridX\":1849,\"gridY\":70,\"type\":3},{\"gridX\":896,\"gridY\":17,\"type\":2}],\"jons\":[{\"gridX\":23,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":199,\"gridY\":85,\"type\":0},{\"gridX\":276,\"gridY\":85,\"type\":0},{\"gridX\":289,\"gridY\":92,\"type\":3},{\"gridX\":269,\"gridY\":92,\"type\":2},{\"gridX\":1616,\"gridY\":97,\"type\":6},{\"gridX\":1632,\"gridY\":97,\"type\":6},{\"gridX\":1616,\"gridY\":113,\"type\":8},{\"gridX\":1632,\"gridY\":113,\"type\":9}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level11(text.c_str());
}

void Chapter1Level11::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level11 * Chapter1Level11::getInstance()
{
    return s_pInstance;
}

Chapter1Level11::Chapter1Level11(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 12 ///

Chapter1Level12 * Chapter1Level12::s_pInstance = nullptr;

void Chapter1Level12::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":12,\"midgrounds\":[{\"gridX\":1596,\"gridY\":96,\"type\":2},{\"gridX\":1238,\"gridY\":96,\"type\":0},{\"gridX\":72,\"gridY\":96,\"type\":2},{\"gridX\":276,\"gridY\":96,\"type\":2},{\"gridX\":395,\"gridY\":96,\"type\":2},{\"gridX\":540,\"gridY\":96,\"type\":2},{\"gridX\":684,\"gridY\":96,\"type\":2},{\"gridX\":812,\"gridY\":96,\"type\":2},{\"gridX\":884,\"gridY\":96,\"type\":2},{\"gridX\":1028,\"gridY\":96,\"type\":2},{\"gridX\":1116,\"gridY\":96,\"type\":2},{\"gridX\":1180,\"gridY\":96,\"type\":2},{\"gridX\":1308,\"gridY\":96,\"type\":2},{\"gridX\":333,\"gridY\":96,\"type\":7},{\"gridX\":333,\"gridY\":108,\"type\":7},{\"gridX\":1221,\"gridY\":96,\"type\":13},{\"gridX\":1161,\"gridY\":96,\"type\":14},{\"gridX\":981,\"gridY\":96,\"type\":13},{\"gridX\":926,\"gridY\":96,\"type\":14},{\"gridX\":937,\"gridY\":96,\"type\":15},{\"gridX\":831,\"gridY\":96,\"type\":16},{\"gridX\":778,\"gridY\":96,\"type\":15},{\"gridX\":616,\"gridY\":96,\"type\":15},{\"gridX\":383,\"gridY\":96,\"type\":14},{\"gridX\":256,\"gridY\":96,\"type\":13},{\"gridX\":126,\"gridY\":96,\"type\":13},{\"gridX\":151,\"gridY\":96,\"type\":14},{\"gridX\":171,\"gridY\":96,\"type\":15},{\"gridX\":50,\"gridY\":96,\"type\":16},{\"gridX\":1044,\"gridY\":96,\"type\":7},{\"gridX\":1044,\"gridY\":204,\"type\":7},{\"gridX\":1044,\"gridY\":192,\"type\":7},{\"gridX\":1044,\"gridY\":180,\"type\":7},{\"gridX\":1044,\"gridY\":168,\"type\":7},{\"gridX\":1044,\"gridY\":156,\"type\":7},{\"gridX\":1044,\"gridY\":144,\"type\":7},{\"gridX\":1044,\"gridY\":132,\"type\":7},{\"gridX\":1044,\"gridY\":120,\"type\":7},{\"gridX\":1044,\"gridY\":108,\"type\":7},{\"gridX\":1044,\"gridY\":240,\"type\":7},{\"gridX\":1044,\"gridY\":228,\"type\":7},{\"gridX\":1044,\"gridY\":216,\"type\":7},{\"gridX\":1428,\"gridY\":0,\"type\":4},{\"gridX\":1506,\"gridY\":0,\"type\":5},{\"gridX\":1452,\"gridY\":0,\"type\":5}],\"grounds\":[{\"gridX\":444,\"gridY\":0,\"type\":0},{\"gridX\":1145,\"gridY\":0,\"type\":0},{\"gridX\":452,\"gridY\":0,\"type\":2},{\"gridX\":1153,\"gridY\":0,\"type\":3},{\"gridX\":516,\"gridY\":0,\"type\":4},{\"gridX\":1281,\"gridY\":0,\"type\":4},{\"gridX\":552,\"gridY\":20,\"type\":5},{\"gridX\":560,\"gridY\":20,\"type\":6},{\"gridX\":624,\"gridY\":20,\"type\":7},{\"gridX\":720,\"gridY\":20,\"type\":7},{\"gridX\":784,\"gridY\":20,\"type\":9},{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":264,\"gridY\":50,\"type\":13},{\"gridX\":136,\"gridY\":50,\"type\":13},{\"gridX\":392,\"gridY\":50,\"type\":13},{\"gridX\":520,\"gridY\":50,\"type\":14},{\"gridX\":1405,\"gridY\":48,\"type\":15},{\"gridX\":1413,\"gridY\":48,\"type\":18},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":488,\"gridY\":90,\"type\":21},{\"gridX\":872,\"gridY\":90,\"type\":21},{\"gridX\":264,\"gridY\":90,\"type\":21},{\"gridX\":1416,\"gridY\":90,\"type\":21},{\"gridX\":808,\"gridY\":90,\"type\":22},{\"gridX\":424,\"gridY\":90,\"type\":22},{\"gridX\":328,\"gridY\":90,\"type\":22},{\"gridX\":1448,\"gridY\":90,\"type\":22},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":520,\"gridY\":90,\"type\":23},{\"gridX\":648,\"gridY\":90,\"type\":23},{\"gridX\":904,\"gridY\":90,\"type\":23},{\"gridX\":1032,\"gridY\":90,\"type\":23},{\"gridX\":1160,\"gridY\":90,\"type\":23},{\"gridX\":1288,\"gridY\":90,\"type\":23},{\"gridX\":1672,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":1800,\"gridY\":90,\"type\":24},{\"gridX\":1512,\"gridY\":90,\"type\":24},{\"gridX\":1536,\"gridY\":0,\"type\":25},{\"gridX\":1544,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[{\"gridX\":296,\"gridY\":88,\"type\":0},{\"gridX\":592,\"gridY\":0,\"type\":3},{\"gridX\":688,\"gridY\":0,\"type\":3},{\"gridX\":776,\"gridY\":88,\"type\":0},{\"gridX\":392,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":144,\"gridY\":80,\"type\":3},{\"gridX\":348,\"gridY\":80,\"type\":3},{\"gridX\":512,\"gridY\":73,\"type\":0},{\"gridX\":528,\"gridY\":73,\"type\":1},{\"gridX\":544,\"gridY\":73,\"type\":1},{\"gridX\":560,\"gridY\":73,\"type\":2},{\"gridX\":458,\"gridY\":32,\"type\":4}],\"foregroundObjects\":[{\"gridX\":1545,\"gridY\":96,\"type\":12},{\"gridX\":116,\"gridY\":96,\"type\":41},{\"gridX\":200,\"gridY\":84,\"type\":30},{\"gridX\":232,\"gridY\":84,\"type\":30},{\"gridX\":232,\"gridY\":55,\"type\":27},{\"gridX\":264,\"gridY\":84,\"type\":30},{\"gridX\":264,\"gridY\":55,\"type\":27},{\"gridX\":300,\"gridY\":55,\"type\":20},{\"gridX\":772,\"gridY\":27,\"type\":20},{\"gridX\":1044,\"gridY\":244,\"type\":34},{\"gridX\":1044,\"gridY\":228,\"type\":34},{\"gridX\":1044,\"gridY\":212,\"type\":34},{\"gridX\":1044,\"gridY\":196,\"type\":34},{\"gridX\":1044,\"gridY\":180,\"type\":34},{\"gridX\":1044,\"gridY\":164,\"type\":34},{\"gridX\":956,\"gridY\":96,\"type\":10},{\"gridX\":860,\"gridY\":96,\"type\":6},{\"gridX\":1069,\"gridY\":96,\"type\":6},{\"gridX\":329,\"gridY\":115,\"type\":46},{\"gridX\":1381,\"gridY\":96,\"type\":58},{\"gridX\":1380,\"gridY\":103,\"type\":60},{\"gridX\":580,\"gridY\":96,\"type\":53},{\"gridX\":588,\"gridY\":96,\"type\":55},{\"gridX\":612,\"gridY\":96,\"type\":55},{\"gridX\":636,\"gridY\":96,\"type\":55},{\"gridX\":660,\"gridY\":96,\"type\":55},{\"gridX\":684,\"gridY\":96,\"type\":55},{\"gridX\":708,\"gridY\":96,\"type\":55},{\"gridX\":732,\"gridY\":96,\"type\":55},{\"gridX\":756,\"gridY\":96,\"type\":56},{\"gridX\":1488,\"gridY\":128,\"type\":52},{\"gridX\":1488,\"gridY\":112,\"type\":52},{\"gridX\":1488,\"gridY\":144,\"type\":51},{\"gridX\":1440,\"gridY\":96,\"type\":52},{\"gridX\":1440,\"gridY\":112,\"type\":51},{\"gridX\":1504,\"gridY\":156,\"type\":50},{\"gridX\":1600,\"gridY\":156,\"type\":50},{\"gridX\":1584,\"gridY\":156,\"type\":50},{\"gridX\":1568,\"gridY\":156,\"type\":50},{\"gridX\":1552,\"gridY\":156,\"type\":50},{\"gridX\":1536,\"gridY\":156,\"type\":50},{\"gridX\":1520,\"gridY\":156,\"type\":50},{\"gridX\":1664,\"gridY\":156,\"type\":50},{\"gridX\":1648,\"gridY\":156,\"type\":50},{\"gridX\":1632,\"gridY\":156,\"type\":50},{\"gridX\":1616,\"gridY\":156,\"type\":50},{\"gridX\":1456,\"gridY\":124,\"type\":50},{\"gridX\":1472,\"gridY\":124,\"type\":50},{\"gridX\":401,\"gridY\":55,\"type\":41},{\"gridX\":1342,\"gridY\":27,\"type\":62},{\"gridX\":1488,\"gridY\":96,\"type\":52}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1506,\"gridY\":159}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":196,\"gridY\":96,\"type\":5},{\"gridX\":329,\"gridY\":121,\"type\":6},{\"gridX\":592,\"gridY\":77,\"type\":7},{\"gridX\":688,\"gridY\":77,\"type\":7},{\"gridX\":448,\"gridY\":96,\"type\":5},{\"gridX\":504,\"gridY\":96,\"type\":5},{\"gridX\":840,\"gridY\":96,\"type\":5},{\"gridX\":912,\"gridY\":96,\"type\":5},{\"gridX\":1004,\"gridY\":96,\"type\":5},{\"gridX\":1148,\"gridY\":96,\"type\":9},{\"gridX\":1180,\"gridY\":96,\"type\":10},{\"gridX\":1232,\"gridY\":96,\"type\":10},{\"gridX\":1280,\"gridY\":96,\"type\":8},{\"gridX\":1312,\"gridY\":96,\"type\":8},{\"gridX\":1348,\"gridY\":96,\"type\":5},{\"gridX\":1421,\"gridY\":96,\"type\":5},{\"gridX\":1458,\"gridY\":127,\"type\":5}],\"collectibles\":[{\"gridX\":212,\"gridY\":128,\"type\":0},{\"gridX\":208,\"gridY\":120,\"type\":0},{\"gridX\":204,\"gridY\":112,\"type\":0},{\"gridX\":228,\"gridY\":132,\"type\":0},{\"gridX\":244,\"gridY\":128,\"type\":0},{\"gridX\":236,\"gridY\":132,\"type\":0},{\"gridX\":220,\"gridY\":132,\"type\":0},{\"gridX\":276,\"gridY\":66,\"type\":1},{\"gridX\":348,\"gridY\":72,\"type\":0},{\"gridX\":356,\"gridY\":72,\"type\":0},{\"gridX\":348,\"gridY\":64,\"type\":0},{\"gridX\":356,\"gridY\":64,\"type\":0},{\"gridX\":316,\"gridY\":104,\"type\":0},{\"gridX\":312,\"gridY\":96,\"type\":0},{\"gridX\":320,\"gridY\":112,\"type\":0},{\"gridX\":372,\"gridY\":64,\"type\":0},{\"gridX\":380,\"gridY\":64,\"type\":0},{\"gridX\":364,\"gridY\":64,\"type\":0},{\"gridX\":120,\"gridY\":112,\"type\":0},{\"gridX\":124,\"gridY\":120,\"type\":0},{\"gridX\":128,\"gridY\":128,\"type\":0},{\"gridX\":132,\"gridY" << "\":136,\"type\":0},{\"gridX\":136,\"gridY\":144,\"type\":0},{\"gridX\":140,\"gridY\":152,\"type\":0},{\"gridX\":144,\"gridY\":160,\"type\":0},{\"gridX\":148,\"gridY\":168,\"type\":0},{\"gridX\":424,\"gridY\":64,\"type\":0},{\"gridX\":440,\"gridY\":64,\"type\":0},{\"gridX\":432,\"gridY\":64,\"type\":0},{\"gridX\":456,\"gridY\":64,\"type\":0},{\"gridX\":496,\"gridY\":16,\"type\":0},{\"gridX\":534,\"gridY\":24,\"type\":1},{\"gridX\":504,\"gridY\":16,\"type\":0},{\"gridX\":512,\"gridY\":16,\"type\":0},{\"gridX\":488,\"gridY\":16,\"type\":0},{\"gridX\":588,\"gridY\":64,\"type\":0},{\"gridX\":596,\"gridY\":68,\"type\":0},{\"gridX\":604,\"gridY\":64,\"type\":0},{\"gridX\":684,\"gridY\":64,\"type\":0},{\"gridX\":692,\"gridY\":68,\"type\":0},{\"gridX\":700,\"gridY\":64,\"type\":0},{\"gridX\":644,\"gridY\":36,\"type\":0},{\"gridX\":652,\"gridY\":36,\"type\":0},{\"gridX\":660,\"gridY\":36,\"type\":0},{\"gridX\":780,\"gridY\":60,\"type\":0},{\"gridX\":784,\"gridY\":68,\"type\":0},{\"gridX\":788,\"gridY\":76,\"type\":0},{\"gridX\":800,\"gridY\":108,\"type\":0},{\"gridX\":804,\"gridY\":116,\"type\":0},{\"gridX\":808,\"gridY\":124,\"type\":0},{\"gridX\":1000,\"gridY\":152,\"type\":1},{\"gridX\":867,\"gridY\":152,\"type\":0},{\"gridX\":876,\"gridY\":156,\"type\":0},{\"gridX\":881,\"gridY\":152,\"type\":0},{\"gridX\":1020,\"gridY\":104,\"type\":0},{\"gridX\":1028,\"gridY\":104,\"type\":0},{\"gridX\":1036,\"gridY\":104,\"type\":0},{\"gridX\":1068,\"gridY\":136,\"type\":0},{\"gridX\":1076,\"gridY\":144,\"type\":0},{\"gridX\":1084,\"gridY\":152,\"type\":0},{\"gridX\":1092,\"gridY\":144,\"type\":0},{\"gridX\":1100,\"gridY\":136,\"type\":0},{\"gridX\":1116,\"gridY\":120,\"type\":0},{\"gridX\":1108,\"gridY\":128,\"type\":0},{\"gridX\":1124,\"gridY\":112,\"type\":0},{\"gridX\":1044,\"gridY\":112,\"type\":0},{\"gridX\":1052,\"gridY\":120,\"type\":0},{\"gridX\":1060,\"gridY\":128,\"type\":0},{\"gridX\":1232,\"gridY\":112,\"type\":0},{\"gridX\":1240,\"gridY\":112,\"type\":0},{\"gridX\":1184,\"gridY\":116,\"type\":0},{\"gridX\":1180,\"gridY\":112,\"type\":0},{\"gridX\":1184,\"gridY\":108,\"type\":0},{\"gridX\":1188,\"gridY\":112,\"type\":0},{\"gridX\":1236,\"gridY\":108,\"type\":0},{\"gridX\":1236,\"gridY\":116,\"type\":0},{\"gridX\":1264,\"gridY\":112,\"type\":0},{\"gridX\":1268,\"gridY\":108,\"type\":0},{\"gridX\":1272,\"gridY\":112,\"type\":0},{\"gridX\":1268,\"gridY\":116,\"type\":0},{\"gridX\":1308,\"gridY\":112,\"type\":0},{\"gridX\":1312,\"gridY\":108,\"type\":0},{\"gridX\":1316,\"gridY\":112,\"type\":0},{\"gridX\":1312,\"gridY\":116,\"type\":0},{\"gridX\":1392,\"gridY\":128,\"type\":0},{\"gridX\":1384,\"gridY\":136,\"type\":0},{\"gridX\":1400,\"gridY\":128,\"type\":0},{\"gridX\":1384,\"gridY\":128,\"type\":0},{\"gridX\":1400,\"gridY\":136,\"type\":0},{\"gridX\":1465,\"gridY\":152,\"type\":0},{\"gridX\":1496,\"gridY\":168,\"type\":0},{\"gridX\":1392,\"gridY\":136,\"type\":0},{\"gridX\":1528,\"gridY\":168,\"type\":0},{\"gridX\":1472,\"gridY\":160,\"type\":0},{\"gridX\":1421,\"gridY\":116,\"type\":0},{\"gridX\":1512,\"gridY\":168,\"type\":0},{\"gridX\":1504,\"gridY\":168,\"type\":0},{\"gridX\":1460,\"gridY\":143,\"type\":0},{\"gridX\":1496,\"gridY\":153,\"type\":0},{\"gridX\":1520,\"gridY\":168,\"type\":0},{\"gridX\":1422,\"gridY\":125,\"type\":0},{\"gridX\":448,\"gridY\":64,\"type\":0},{\"gridX\":812,\"gridY\":132,\"type\":0},{\"gridX\":1172,\"gridY\":12,\"type\":2},{\"gridX\":1188,\"gridY\":12,\"type\":2},{\"gridX\":1204,\"gridY\":12,\"type\":2},{\"gridX\":1220,\"gridY\":12,\"type\":2},{\"gridX\":1349,\"gridY\":43,\"type\":3},{\"gridX\":1526,\"gridY\":98,\"type\":3}],\"jons\":[{\"gridX\":18,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level12(text.c_str());
}

void Chapter1Level12::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level12 * Chapter1Level12::getInstance()
{
    return s_pInstance;
}

Chapter1Level12::Chapter1Level12(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 13 ///

Chapter1Level13 * Chapter1Level13::s_pInstance = nullptr;

void Chapter1Level13::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":13,\"midgrounds\":[{\"gridX\":89,\"gridY\":96,\"type\":2},{\"gridX\":108,\"gridY\":96,\"type\":6},{\"gridX\":301,\"gridY\":96,\"type\":0},{\"gridX\":556,\"gridY\":0,\"type\":5},{\"gridX\":643,\"gridY\":0,\"type\":4},{\"gridX\":618,\"gridY\":0,\"type\":5},{\"gridX\":668,\"gridY\":0,\"type\":5},{\"gridX\":877,\"gridY\":0,\"type\":4},{\"gridX\":891,\"gridY\":0,\"type\":5},{\"gridX\":924,\"gridY\":0,\"type\":4},{\"gridX\":806,\"gridY\":96,\"type\":2},{\"gridX\":874,\"gridY\":96,\"type\":6},{\"gridX\":954,\"gridY\":96,\"type\":7},{\"gridX\":980,\"gridY\":96,\"type\":7},{\"gridX\":954,\"gridY\":108,\"type\":7},{\"gridX\":980,\"gridY\":108,\"type\":7},{\"gridX\":947,\"gridY\":132,\"type\":8},{\"gridX\":954,\"gridY\":120,\"type\":7},{\"gridX\":980,\"gridY\":120,\"type\":7},{\"gridX\":1410,\"gridY\":96,\"type\":11},{\"gridX\":1482,\"gridY\":96,\"type\":6},{\"gridX\":84,\"gridY\":96,\"type\":15},{\"gridX\":121,\"gridY\":96,\"type\":16},{\"gridX\":785,\"gridY\":96,\"type\":15},{\"gridX\":828,\"gridY\":96,\"type\":16},{\"gridX\":1446,\"gridY\":96,\"type\":13},{\"gridX\":937,\"gridY\":96,\"type\":14},{\"gridX\":1107,\"gridY\":96,\"type\":13},{\"gridX\":1068,\"gridY\":96,\"type\":14},{\"gridX\":1032,\"gridY\":96,\"type\":13},{\"gridX\":2052,\"gridY\":0,\"type\":7},{\"gridX\":2052,\"gridY\":96,\"type\":7},{\"gridX\":2052,\"gridY\":84,\"type\":7},{\"gridX\":2052,\"gridY\":72,\"type\":7},{\"gridX\":2052,\"gridY\":60,\"type\":7},{\"gridX\":2052,\"gridY\":48,\"type\":7},{\"gridX\":2052,\"gridY\":36,\"type\":7},{\"gridX\":2052,\"gridY\":24,\"type\":7},{\"gridX\":2052,\"gridY\":12,\"type\":7},{\"gridX\":2052,\"gridY\":108,\"type\":7},{\"gridX\":2052,\"gridY\":120,\"type\":7},{\"gridX\":2168,\"gridY\":120,\"type\":7},{\"gridX\":2168,\"gridY\":108,\"type\":7},{\"gridX\":2168,\"gridY\":72,\"type\":7},{\"gridX\":2168,\"gridY\":60,\"type\":7},{\"gridX\":2168,\"gridY\":48,\"type\":7},{\"gridX\":2168,\"gridY\":36,\"type\":7},{\"gridX\":2168,\"gridY\":24,\"type\":7},{\"gridX\":2168,\"gridY\":12,\"type\":7},{\"gridX\":2168,\"gridY\":0,\"type\":7},{\"gridX\":2168,\"gridY\":84,\"type\":7},{\"gridX\":2168,\"gridY\":96,\"type\":7},{\"gridX\":1220,\"gridY\":0,\"type\":4},{\"gridX\":1187,\"gridY\":0,\"type\":5},{\"gridX\":1354,\"gridY\":0,\"type\":5},{\"gridX\":1055,\"gridY\":0,\"type\":7},{\"gridX\":1055,\"gridY\":12,\"type\":7},{\"gridX\":1055,\"gridY\":24,\"type\":7},{\"gridX\":44,\"gridY\":96,\"type\":11},{\"gridX\":420,\"gridY\":120,\"type\":7},{\"gridX\":420,\"gridY\":108,\"type\":7},{\"gridX\":420,\"gridY\":96,\"type\":7},{\"gridX\":420,\"gridY\":84,\"type\":7},{\"gridX\":420,\"gridY\":72,\"type\":7},{\"gridX\":420,\"gridY\":60,\"type\":7},{\"gridX\":420,\"gridY\":48,\"type\":7},{\"gridX\":420,\"gridY\":36,\"type\":7},{\"gridX\":420,\"gridY\":24,\"type\":7},{\"gridX\":420,\"gridY\":12,\"type\":7},{\"gridX\":420,\"gridY\":0,\"type\":7},{\"gridX\":420,\"gridY\":216,\"type\":7},{\"gridX\":420,\"gridY\":204,\"type\":7},{\"gridX\":420,\"gridY\":192,\"type\":7},{\"gridX\":420,\"gridY\":180,\"type\":7},{\"gridX\":420,\"gridY\":168,\"type\":7},{\"gridX\":420,\"gridY\":156,\"type\":7},{\"gridX\":420,\"gridY\":144,\"type\":7},{\"gridX\":420,\"gridY\":132,\"type\":7},{\"gridX\":420,\"gridY\":240,\"type\":7},{\"gridX\":420,\"gridY\":228,\"type\":7},{\"gridX\":356,\"gridY\":204,\"type\":7},{\"gridX\":356,\"gridY\":216,\"type\":7},{\"gridX\":356,\"gridY\":228,\"type\":7},{\"gridX\":356,\"gridY\":240,\"type\":7},{\"gridX\":364,\"gridY\":240,\"type\":7},{\"gridX\":372,\"gridY\":240,\"type\":7},{\"gridX\":380,\"gridY\":240,\"type\":7},{\"gridX\":388,\"gridY\":240,\"type\":7},{\"gridX\":396,\"gridY\":240,\"type\":7},{\"gridX\":412,\"gridY\":240,\"type\":7},{\"gridX\":404,\"gridY\":240,\"type\":7},{\"gridX\":1472,\"gridY\":0,\"type\":4},{\"gridX\":1444,\"gridY\":0,\"type\":5},{\"gridX\":1337,\"gridY\":75,\"type\":17},{\"gridX\":1359,\"gridY\":75,\"type\":17},{\"gridX\":1327,\"gridY\":93,\"type\":18},{\"gridX\":1309,\"gridY\":118,\"type\":18},{\"gridX\":1361,\"gridY\":96,\"type\":17},{\"gridX\":1381,\"gridY\":96,\"type\":17},{\"gridX\":1371,\"gridY\":75,\"type\":18},{\"gridX\":1339,\"gridY\":96,\"type\":17},{\"gridX\":1384,\"gridY\":109,\"type\":18},{\"gridX\":1347,\"gridY\":114,\"type\":17},{\"gridX\":1368,\"gridY\":114,\"type\":17},{\"gridX\":1386,\"gridY\":122,\"type\":18},{\"gridX\":1318,\"gridY\":107,\"type\":18},{\"gridX\":1326,\"gridY\":114,\"type\":17},{\"gridX\":1341,\"gridY\":133,\"type\":17},{\"gridX\":1362,\"gridY\":133,\"type\":17},{\"gridX\":1374,\"gridY\":134,\"type\":18},{\"gridX\":1346,\"gridY\":153,\"type\":17},{\"gridX\":1337,\"gridY\":142,\"type\":18},{\"gridX\":1344,\"gridY\":167,\"type\":18},{\"gridX\":1347,\"gridY\":181,\"type\":18},{\"gridX\":1367,\"gridY\":152,\"type\":17},{\"gridX\":1388,\"gridY\":153,\"type\":17},{\"gridX\":1403,\"gridY\":174,\"type\":17},{\"gridX\":1398,\"gridY\":162,\"type\":18},{\"gridX\":1421,\"gridY\":172,\"type\":18},{\"gridX\":1431,\"gridY\":184,\"type\":18},{\"gridX\":1439,\"gridY\":184,\"type\":18},{\"gridX\":1362,\"gridY\":174,\"type\":17},{\"gridX\":1382,\"gridY\":174,\"type\":17},{\"gridX\":1368,\"gridY\":196,\"type\":17},{\"gridX\":1385,\"gridY\":192,\"type\":17},{\"gridX\":1405,\"gridY\":190,\"type\":18},{\"gridX\":1379,\"gridY\":212,\"type\":18}],\"grounds\":[{\"gridX\":364,\"gridY\":0,\"type\":0},{\"gridX\":975,\"gridY\":0,\"type\":0},{\"gridX\":1097,\"gridY\":0,\"type\":0},{\"gridX\":1537,\"gridY\":0,\"type\":0},{\"gridX\":372,\"gridY\":0,\"type\":1},{\"gridX\":983,\"gridY\":0,\"type\":1},{\"gridX\":1105,\"gridY\":0,\"type\":1},{\"gridX\":1545,\"gridY\":0,\"type\":1},{\"gridX\":134,\"gridY\":0,\"type\":3},{\"gridX\":262,\"gridY\":0,\"type\":4},{\"gridX\":404,\"gridY\":0,\"type\":4},{\"gridX\":1014,\"gridY\":0,\"type\":4},{\"gridX\":1137,\"gridY\":0,\"type\":4},{\"gridX\":555,\"gridY\":50,\"type\":10},{\"gridX\":859,\"gridY\":50,\"type\":10},{\"gridX\":1184,\"gridY\":50,\"type\":10},{\"gridX\":1440,\"gridY\":50,\"type\":10},{\"gridX\":563,\"gridY\":50,\"type\":11},{\"gridX\":1448,\"gridY\":50,\"type\":11},{\"gridX\":595,\"gridY\":50,\"type\":12},{\"gridX\":867,\"gridY\":50,\"type\":12},{\"gridX\":1192,\"gridY\":50,\"type\":13},{\"gridX\":931,\"gridY\":50,\"type\":14},{\"gridX\":1480,\"gridY\":50,\"type\":14},{\"gridX\":644,\"gridY\":48,\"type\":15},{\"gridX\":1305,\"gridY\":48,\"type\":15},{\"gridX\":652,\"gridY\":48,\"type\":16},{\"gridX\":1313,\"gridY\":48,\"type\":17},{\"gridX\":684,\"gridY\":48,\"type\":19},{\"gridX\":1377,\"gridY\":48,\"type\":19},{\"gridX\":296,\"gridY\":90,\"type\":20},{\"gridX\":728,\"gridY\":90,\"type\":20},{\"gridX\":1372,\"gridY\":90,\"type\":20},{\"gridX\":1120,\"gridY\":90,\"type\":21},{\"gridX\":304,\"gridY\":90,\"type\":21},{\"gridX\":1476,\"gridY\":90,\"type\":21},{\"gridX\":1380,\"gridY\":90,\"type\":22},{\"gridX\":736,\"gridY\":90,\"type\":23},{\"gridX\":864,\"gridY\":90,\"type\":23},{\"gridX\":992,\"gridY\":90,\"type\":23},{\"gridX\":336,\"gridY\":90,\"type\":24},{\"gridX\":1152,\"gridY\":90,\"type\":24},{\"gridX\":1508,\"gridY\":90,\"type\":24},{\"gridX\":440,\"gridY\":0,\"type\":25},{\"gridX\":1574,\"gridY\":0,\"type\":25},{\"gridX\":1646,\"gridY\":0,\"type\":25},{\"gridX\":1716,\"gridY\":0,\"type\":25},{\"gridX\":1804,\"gridY\":0,\"type\":25},{\"gridX\":2330,\"gridY\":0,\"type\":25},{\"gridX\":448,\"gridY\":0,\"type\":27},{\"gridX\":0,\"gridY\":0,\"type\":28},{\"gridX\":2338,\"gridY\":0,\"type\":28},{\"gridX\":2466,\"gridY\":0,\"type\":28},{\"gridX\":1654,\"gridY\":0,\"type\":29},{\"gridX\":2594,\"gridY\":0,\"type\":29},{\"gridX\":1724,\"gridY\":0,\"type\":29},{\"gridX\":1812,\"gridY\":0,\"type\":29},{\"gridX\":1582,\"gridY\":0,\"type\":29},{\"gridX\":512,\"gridY\":0,\"type\":29},{\"gridX\":128,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":1444,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":876,\"gridY\":80,\"type\":3},{\"gridX\":875,\"gridY\":32,\"type\":4}],\"foregroundObjects\":[{\"gridX\":64,\"gridY\":96,\"type\":52},{\"gridX\":64,\"gridY\":112,\"type\"" << ":52},{\"gridX\":64,\"gridY\":128,\"type\":52},{\"gridX\":254,\"gridY\":7,\"type\":21},{\"gridX\":316,\"gridY\":96,\"type\":52},{\"gridX\":316,\"gridY\":112,\"type\":52},{\"gridX\":316,\"gridY\":128,\"type\":52},{\"gridX\":316,\"gridY\":144,\"type\":52},{\"gridX\":316,\"gridY\":160,\"type\":52},{\"gridX\":316,\"gridY\":176,\"type\":52},{\"gridX\":332,\"gridY\":204,\"type\":50},{\"gridX\":348,\"gridY\":204,\"type\":50},{\"gridX\":364,\"gridY\":204,\"type\":50},{\"gridX\":388,\"gridY\":7,\"type\":21},{\"gridX\":223,\"gridY\":7,\"type\":27},{\"gridX\":448,\"gridY\":95,\"type\":35},{\"gridX\":506,\"gridY\":95,\"type\":35},{\"gridX\":478,\"gridY\":95,\"type\":35},{\"gridX\":420,\"gridY\":228,\"type\":34},{\"gridX\":420,\"gridY\":214,\"type\":34},{\"gridX\":420,\"gridY\":200,\"type\":34},{\"gridX\":371,\"gridY\":7,\"type\":26},{\"gridX\":399,\"gridY\":7,\"type\":25},{\"gridX\":404,\"gridY\":7,\"type\":25},{\"gridX\":736,\"gridY\":95,\"type\":35},{\"gridX\":64,\"gridY\":144,\"type\":52},{\"gridX\":64,\"gridY\":160,\"type\":51},{\"gridX\":420,\"gridY\":242,\"type\":34},{\"gridX\":48,\"gridY\":172,\"type\":50},{\"gridX\":32,\"gridY\":172,\"type\":50},{\"gridX\":16,\"gridY\":172,\"type\":50},{\"gridX\":0,\"gridY\":172,\"type\":50},{\"gridX\":872,\"gridY\":26,\"type\":4},{\"gridX\":869,\"gridY\":26,\"type\":3},{\"gridX\":942,\"gridY\":26,\"type\":5},{\"gridX\":886,\"gridY\":26,\"type\":4},{\"gridX\":900,\"gridY\":26,\"type\":4},{\"gridX\":914,\"gridY\":26,\"type\":4},{\"gridX\":928,\"gridY\":26,\"type\":4},{\"gridX\":916,\"gridY\":96,\"type\":24},{\"gridX\":948,\"gridY\":96,\"type\":24},{\"gridX\":980,\"gridY\":96,\"type\":24},{\"gridX\":1012,\"gridY\":96,\"type\":24},{\"gridX\":1044,\"gridY\":96,\"type\":24},{\"gridX\":1820,\"gridY\":92,\"type\":50},{\"gridX\":1852,\"gridY\":92,\"type\":50},{\"gridX\":1836,\"gridY\":92,\"type\":50},{\"gridX\":1868,\"gridY\":92,\"type\":50},{\"gridX\":1884,\"gridY\":92,\"type\":50},{\"gridX\":1900,\"gridY\":92,\"type\":50},{\"gridX\":1916,\"gridY\":92,\"type\":50},{\"gridX\":2378,\"gridY\":96,\"type\":12},{\"gridX\":1055,\"gridY\":32,\"type\":34},{\"gridX\":1075,\"gridY\":96,\"type\":23},{\"gridX\":1094,\"gridY\":96,\"type\":24},{\"gridX\":1127,\"gridY\":96,\"type\":24},{\"gridX\":300,\"gridY\":96,\"type\":53},{\"gridX\":308,\"gridY\":96,\"type\":55},{\"gridX\":332,\"gridY\":96,\"type\":56},{\"gridX\":2164,\"gridY\":130,\"type\":48},{\"gridX\":2164,\"gridY\":74,\"type\":48},{\"gridX\":2148,\"gridY\":74,\"type\":48},{\"gridX\":2132,\"gridY\":74,\"type\":48},{\"gridX\":2116,\"gridY\":74,\"type\":48},{\"gridX\":2100,\"gridY\":74,\"type\":48},{\"gridX\":2084,\"gridY\":74,\"type\":48},{\"gridX\":2068,\"gridY\":74,\"type\":48},{\"gridX\":2052,\"gridY\":74,\"type\":48},{\"gridX\":2048,\"gridY\":74,\"type\":47},{\"gridX\":2180,\"gridY\":74,\"type\":49},{\"gridX\":2148,\"gridY\":130,\"type\":48},{\"gridX\":2132,\"gridY\":130,\"type\":48},{\"gridX\":2116,\"gridY\":130,\"type\":48},{\"gridX\":2100,\"gridY\":130,\"type\":48},{\"gridX\":2084,\"gridY\":130,\"type\":48},{\"gridX\":2068,\"gridY\":130,\"type\":48},{\"gridX\":2052,\"gridY\":130,\"type\":48},{\"gridX\":2048,\"gridY\":130,\"type\":47},{\"gridX\":2180,\"gridY\":130,\"type\":49},{\"gridX\":2036,\"gridY\":120,\"type\":48},{\"gridX\":2020,\"gridY\":120,\"type\":48},{\"gridX\":2052,\"gridY\":120,\"type\":48},{\"gridX\":2016,\"gridY\":120,\"type\":47},{\"gridX\":2068,\"gridY\":120,\"type\":49},{\"gridX\":732,\"gridY\":15,\"type\":62},{\"gridX\":795,\"gridY\":33,\"type\":62},{\"gridX\":404,\"gridY\":242,\"type\":34},{\"gridX\":388,\"gridY\":242,\"type\":34},{\"gridX\":372,\"gridY\":242,\"type\":34},{\"gridX\":356,\"gridY\":242,\"type\":34},{\"gridX\":1177,\"gridY\":5,\"type\":62},{\"gridX\":1231,\"gridY\":8,\"type\":62},{\"gridX\":1284,\"gridY\":4,\"type\":62},{\"gridX\":1336,\"gridY\":4,\"type\":62},{\"gridX\":1384,\"gridY\":5,\"type\":62},{\"gridX\":1562,\"gridY\":7,\"type\":21},{\"gridX\":1453,\"gridY\":55,\"type\":41},{\"gridX\":946,\"gridY\":128,\"type\":50},{\"gridX\":962,\"gridY\":128,\"type\":50},{\"gridX\":978,\"gridY\":128,\"type\":50},{\"gridX\":1215,\"gridY\":210,\"type\":62},{\"gridX\":1390,\"gridY\":131,\"type\":60},{\"gridX\":1383,\"gridY\":125,\"type\":59},{\"gridX\":1331,\"gridY\":224,\"type\":60},{\"gridX\":1344,\"gridY\":148,\"type\":61},{\"gridX\":1324,\"gridY\":131,\"type\":60},{\"gridX\":1301,\"gridY\":131,\"type\":60},{\"gridX\":1317,\"gridY\":126,\"type\":59},{\"gridX\":1342,\"gridY\":215,\"type\":63},{\"gridX\":1325,\"gridY\":113,\"type\":59},{\"gridX\":1366,\"gridY\":148,\"type\":61},{\"gridX\":1350,\"gridY\":224,\"type\":60},{\"gridX\":1342,\"gridY\":202,\"type\":59},{\"gridX\":1353,\"gridY\":193,\"type\":59},{\"gridX\":1365,\"gridY\":217,\"type\":63},{\"gridX\":1373,\"gridY\":224,\"type\":60},{\"gridX\":1360,\"gridY\":202,\"type\":59},{\"gridX\":1414,\"gridY\":188,\"type\":60},{\"gridX\":1435,\"gridY\":205,\"type\":61},{\"gridX\":1447,\"gridY\":183,\"type\":59},{\"gridX\":1281,\"gridY\":148,\"type\":61},{\"gridX\":1480,\"gridY\":205,\"type\":61},{\"gridX\":1458,\"gridY\":188,\"type\":60},{\"gridX\":316,\"gridY\":192,\"type\":52},{\"gridX\":316,\"gridY\":208,\"type\":51}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":2346,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":841,\"gridY\":96,\"type\":4},{\"gridX\":1063,\"gridY\":128,\"type\":3},{\"gridX\":619,\"gridY\":55,\"type\":9},{\"gridX\":877,\"gridY\":96,\"type\":11},{\"gridX\":2241,\"gridY\":84,\"type\":3},{\"gridX\":2290,\"gridY\":93,\"type\":3},{\"gridX\":923,\"gridY\":55,\"type\":12},{\"gridX\":1577,\"gridY\":96,\"type\":4},{\"gridX\":1649,\"gridY\":96,\"type\":4},{\"gridX\":1718,\"gridY\":96,\"type\":4},{\"gridX\":2111,\"gridY\":135,\"type\":5},{\"gridX\":1111,\"gridY\":121,\"type\":3},{\"gridX\":1159,\"gridY\":112,\"type\":3},{\"gridX\":1395,\"gridY\":155,\"type\":5}],\"collectibles\":[{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":802,\"gridY\":50,\"type\":1},{\"gridX\":739,\"gridY\":32,\"type\":0},{\"gridX\":388,\"gridY\":188,\"type\":0},{\"gridX\":388,\"gridY\":180,\"type\":0},{\"gridX\":388,\"gridY\":172,\"type\":0},{\"gridX\":388,\"gridY\":164,\"type\":0},{\"gridX\":388,\"gridY\":156,\"type\":0},{\"gridX\":388,\"gridY\":148,\"type\":0},{\"gridX\":388,\"gridY\":140,\"type\":0},{\"gridX\":152,\"gridY\":12,\"type\":0},{\"gridX\":160,\"gridY\":12,\"type\":0},{\"gridX\":168,\"gridY\":12,\"type\":0},{\"gridX\":176,\"gridY\":12,\"type\":0},{\"gridX\":880,\"gridY\":59,\"type\":0},{\"gridX\":879,\"gridY\":35,\"type\":0},{\"gridX\":945,\"gridY\":36,\"type\":0},{\"gridX\":888,\"gridY\":35,\"type\":0},{\"gridX\":900,\"gridY\":36,\"type\":0},{\"gridX\":911,\"gridY\":35,\"type\":0},{\"gridX\":922,\"gridY\":35,\"type\":0},{\"gridX\":934,\"gridY\":36,\"type\":0},{\"gridX\":870,\"gridY\":59,\"type\":0},{\"gridX\":2112,\"gridY\":88,\"type\":1},{\"gridX\":1016,\"gridY\":12,\"type\":0},{\"gridX\":1020,\"gridY\":20,\"type\":0},{\"gridX\":1024,\"gridY\":28,\"type\":0},{\"gridX\":1028,\"gridY\":36,\"type\":0},{\"gridX\":1036,\"gridY\":44,\"type\":0},{\"gridX\":1118,\"gridY\":13,\"type\":0},{\"gridX\":1110,\"gridY\":13,\"type\":0},{\"gridX\":1102,\"gridY\":13,\"type\":0},{\"gridX\":1180,\"gridY\":20,\"type\":0},{\"gridX\":1188,\"gridY\":20,\"type\":0},{\"gridX\":1186,\"gridY\":26,\"type\":0},{\"gridX\":1234,\"gridY\":22,\"type\":0},{\"gridX\":1240,\"gridY\":27,\"type\":0},{\"gridX\":1296,\"gridY\":18,\"type\":0},{\"gridX\":1288,\"gridY\":18,\"type\":0},{\"gridX\":1387,\"gridY\":19,\"type\":0},{\"gridX\":1345,\"gridY\":24,\"type\":0},{\"gridX\":1294,\"gridY\":23,\"type\":0},{\"gridX\":1340,\"gridY\":18,\"type\":0},{\"gridX\":1347,\"gridY\":18,\"type\":0},{\"gridX\":1394,\"gridY\":19,\"type\":0},{\"gridX\":1392,\"gridY\":25,\"type\":0},{\"gridX\":1532,\"gridY\":124,\"type\":0},{\"gridX\":1540,\"gridY\":133,\"type\":0},{\"gridX\":1544,\"gridY\":126,\"type\":0},{\"gridX\":1610,\"gridY\":128,\"type\":0},{\"gridX\":1617,\"gridY\":136,\"type\":0},{\"gridX\":16" << "20,\"gridY\":128,\"type\":0},{\"gridX\":1683,\"gridY\":130,\"type\":0},{\"gridX\":1689,\"gridY\":137,\"type\":0},{\"gridX\":1692,\"gridY\":131,\"type\":0},{\"gridX\":1761,\"gridY\":131,\"type\":0},{\"gridX\":1771,\"gridY\":131,\"type\":0},{\"gridX\":1767,\"gridY\":138,\"type\":0},{\"gridX\":347,\"gridY\":212,\"type\":0},{\"gridX\":356,\"gridY\":212,\"type\":0},{\"gridX\":364,\"gridY\":212,\"type\":0},{\"gridX\":372,\"gridY\":212,\"type\":0},{\"gridX\":380,\"gridY\":212,\"type\":0},{\"gridX\":388,\"gridY\":212,\"type\":0},{\"gridX\":388,\"gridY\":204,\"type\":0},{\"gridX\":388,\"gridY\":196,\"type\":0},{\"gridX\":652,\"gridY\":80,\"type\":0},{\"gridX\":660,\"gridY\":80,\"type\":0},{\"gridX\":668,\"gridY\":80,\"type\":0},{\"gridX\":676,\"gridY\":80,\"type\":0},{\"gridX\":1242,\"gridY\":22,\"type\":0},{\"gridX\":1456,\"gridY\":74,\"type\":0},{\"gridX\":1455,\"gridY\":61,\"type\":0},{\"gridX\":1456,\"gridY\":80,\"type\":0},{\"gridX\":1456,\"gridY\":68,\"type\":0},{\"gridX\":1410,\"gridY\":42,\"type\":0},{\"gridX\":1448,\"gridY\":60,\"type\":0},{\"gridX\":1404,\"gridY\":34,\"type\":0},{\"gridX\":1840,\"gridY\":104,\"type\":0},{\"gridX\":1848,\"gridY\":104,\"type\":0},{\"gridX\":1856,\"gridY\":104,\"type\":0},{\"gridX\":1864,\"gridY\":104,\"type\":0},{\"gridX\":1872,\"gridY\":104,\"type\":0},{\"gridX\":1880,\"gridY\":104,\"type\":0},{\"gridX\":1888,\"gridY\":104,\"type\":0},{\"gridX\":1896,\"gridY\":104,\"type\":0},{\"gridX\":1904,\"gridY\":104,\"type\":0},{\"gridX\":2100,\"gridY\":88,\"type\":0},{\"gridX\":2124,\"gridY\":88,\"type\":0},{\"gridX\":2132,\"gridY\":88,\"type\":0},{\"gridX\":2140,\"gridY\":88,\"type\":0},{\"gridX\":2092,\"gridY\":88,\"type\":0},{\"gridX\":2076,\"gridY\":88,\"type\":0},{\"gridX\":2148,\"gridY\":88,\"type\":0},{\"gridX\":2084,\"gridY\":88,\"type\":0},{\"gridX\":2368,\"gridY\":104,\"type\":0},{\"gridX\":2371,\"gridY\":99,\"type\":0},{\"gridX\":2361,\"gridY\":99,\"type\":0},{\"gridX\":2359,\"gridY\":104,\"type\":0},{\"gridX\":2156,\"gridY\":88,\"type\":0},{\"gridX\":1832,\"gridY\":104,\"type\":0},{\"gridX\":1912,\"gridY\":104,\"type\":0},{\"gridX\":1824,\"gridY\":104,\"type\":0},{\"gridX\":967,\"gridY\":142,\"type\":3},{\"gridX\":950,\"gridY\":135,\"type\":2},{\"gridX\":979,\"gridY\":135,\"type\":2},{\"gridX\":1262,\"gridY\":59,\"type\":2},{\"gridX\":1336,\"gridY\":87,\"type\":2},{\"gridX\":1372,\"gridY\":158,\"type\":2},{\"gridX\":1353,\"gridY\":158,\"type\":2},{\"gridX\":1461,\"gridY\":215,\"type\":2},{\"gridX\":1477,\"gridY\":215,\"type\":2},{\"gridX\":1366,\"gridY\":248,\"type\":3},{\"gridX\":1337,\"gridY\":249,\"type\":0},{\"gridX\":1346,\"gridY\":249,\"type\":0},{\"gridX\":1355,\"gridY\":249,\"type\":0},{\"gridX\":1373,\"gridY\":249,\"type\":0},{\"gridX\":1392,\"gridY\":249,\"type\":0},{\"gridX\":1382,\"gridY\":249,\"type\":0}],\"jons\":[{\"gridX\":12,\"gridY\":175}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":44,\"gridY\":92,\"type\":2},{\"gridX\":113,\"gridY\":92,\"type\":1},{\"gridX\":104,\"gridY\":92,\"type\":3},{\"gridX\":304,\"gridY\":85,\"type\":0},{\"gridX\":844,\"gridY\":92,\"type\":2},{\"gridX\":752,\"gridY\":85,\"type\":0},{\"gridX\":994,\"gridY\":92,\"type\":1},{\"gridX\":969,\"gridY\":92,\"type\":3},{\"gridX\":1400,\"gridY\":92,\"type\":2},{\"gridX\":1409,\"gridY\":85,\"type\":0},{\"gridX\":888,\"gridY\":85,\"type\":0},{\"gridX\":818,\"gridY\":92,\"type\":3}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level13(text.c_str());
}

void Chapter1Level13::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level13 * Chapter1Level13::getInstance()
{
    return s_pInstance;
}

Chapter1Level13::Chapter1Level13(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 14 ///

Chapter1Level14 * Chapter1Level14::s_pInstance = nullptr;

void Chapter1Level14::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":14,\"midgrounds\":[{\"gridX\":24,\"gridY\":96,\"type\":2},{\"gridX\":1396,\"gridY\":96,\"type\":2},{\"gridX\":1508,\"gridY\":96,\"type\":2},{\"gridX\":116,\"gridY\":96,\"type\":2},{\"gridX\":230,\"gridY\":96,\"type\":2},{\"gridX\":1205,\"gridY\":96,\"type\":2},{\"gridX\":416,\"gridY\":128,\"type\":19},{\"gridX\":432,\"gridY\":128,\"type\":19},{\"gridX\":448,\"gridY\":128,\"type\":19},{\"gridX\":448,\"gridY\":192,\"type\":19},{\"gridX\":448,\"gridY\":176,\"type\":19},{\"gridX\":448,\"gridY\":144,\"type\":19},{\"gridX\":448,\"gridY\":208,\"type\":19},{\"gridX\":448,\"gridY\":224,\"type\":20},{\"gridX\":464,\"gridY\":128,\"type\":19},{\"gridX\":480,\"gridY\":128,\"type\":19},{\"gridX\":464,\"gridY\":112,\"type\":19},{\"gridX\":480,\"gridY\":112,\"type\":19},{\"gridX\":464,\"gridY\":144,\"type\":19},{\"gridX\":480,\"gridY\":144,\"type\":19},{\"gridX\":560,\"gridY\":112,\"type\":19},{\"gridX\":560,\"gridY\":176,\"type\":19},{\"gridX\":576,\"gridY\":160,\"type\":19},{\"gridX\":560,\"gridY\":160,\"type\":19},{\"gridX\":576,\"gridY\":128,\"type\":19},{\"gridX\":576,\"gridY\":112,\"type\":19},{\"gridX\":560,\"gridY\":128,\"type\":19},{\"gridX\":576,\"gridY\":176,\"type\":19},{\"gridX\":928,\"gridY\":96,\"type\":13},{\"gridX\":1008,\"gridY\":96,\"type\":14},{\"gridX\":1086,\"gridY\":96,\"type\":13},{\"gridX\":936,\"gridY\":96,\"type\":15},{\"gridX\":1452,\"gridY\":96,\"type\":14},{\"gridX\":1548,\"gridY\":96,\"type\":13},{\"gridX\":1309,\"gridY\":96,\"type\":14},{\"gridX\":1354,\"gridY\":96,\"type\":15},{\"gridX\":1232,\"gridY\":96,\"type\":16},{\"gridX\":359,\"gridY\":96,\"type\":13},{\"gridX\":7,\"gridY\":96,\"type\":13},{\"gridX\":241,\"gridY\":96,\"type\":15},{\"gridX\":371,\"gridY\":96,\"type\":16},{\"gridX\":100,\"gridY\":96,\"type\":15},{\"gridX\":44,\"gridY\":96,\"type\":16},{\"gridX\":57,\"gridY\":96,\"type\":6},{\"gridX\":913,\"gridY\":96,\"type\":6},{\"gridX\":1099,\"gridY\":96,\"type\":6},{\"gridX\":1225,\"gridY\":96,\"type\":11},{\"gridX\":448,\"gridY\":112,\"type\":19},{\"gridX\":100,\"gridY\":96,\"type\":17},{\"gridX\":118,\"gridY\":96,\"type\":17},{\"gridX\":126,\"gridY\":110,\"type\":18},{\"gridX\":134,\"gridY\":124,\"type\":17},{\"gridX\":132,\"gridY\":140,\"type\":18},{\"gridX\":153,\"gridY\":137,\"type\":17},{\"gridX\":152,\"gridY\":123,\"type\":18},{\"gridX\":161,\"gridY\":154,\"type\":17},{\"gridX\":188,\"gridY\":182,\"type\":17},{\"gridX\":179,\"gridY\":163,\"type\":17},{\"gridX\":176,\"gridY\":151,\"type\":18},{\"gridX\":194,\"gridY\":165,\"type\":18},{\"gridX\":207,\"gridY\":157,\"type\":18},{\"gridX\":233,\"gridY\":151,\"type\":17},{\"gridX\":252,\"gridY\":136,\"type\":17},{\"gridX\":267,\"gridY\":124,\"type\":17},{\"gridX\":286,\"gridY\":121,\"type\":17},{\"gridX\":289,\"gridY\":96,\"type\":17},{\"gridX\":208,\"gridY\":176,\"type\":17},{\"gridX\":311,\"gridY\":96,\"type\":17},{\"gridX\":297,\"gridY\":96,\"type\":18},{\"gridX\":284,\"gridY\":110,\"type\":18},{\"gridX\":252,\"gridY\":124,\"type\":18},{\"gridX\":216,\"gridY\":148,\"type\":18},{\"gridX\":244,\"gridY\":155,\"type\":18},{\"gridX\":267,\"gridY\":128,\"type\":18},{\"gridX\":188,\"gridY\":200,\"type\":17},{\"gridX\":202,\"gridY\":192,\"type\":18},{\"gridX\":182,\"gridY\":211,\"type\":18},{\"gridX\":174,\"gridY\":205,\"type\":18},{\"gridX\":200,\"gridY\":212,\"type\":18},{\"gridX\":224,\"gridY\":168,\"type\":18},{\"gridX\":151,\"gridY\":160,\"type\":18},{\"gridX\":301,\"gridY\":108,\"type\":18},{\"gridX\":260,\"gridY\":149,\"type\":17},{\"gridX\":278,\"gridY\":136,\"type\":18},{\"gridX\":416,\"gridY\":144,\"type\":19},{\"gridX\":432,\"gridY\":144,\"type\":19},{\"gridX\":560,\"gridY\":144,\"type\":19},{\"gridX\":576,\"gridY\":144,\"type\":19},{\"gridX\":1131,\"gridY\":96,\"type\":17},{\"gridX\":1115,\"gridY\":96,\"type\":18},{\"gridX\":1151,\"gridY\":96,\"type\":17},{\"gridX\":1171,\"gridY\":96,\"type\":17},{\"gridX\":1168,\"gridY\":116,\"type\":17},{\"gridX\":1184,\"gridY\":125,\"type\":18},{\"gridX\":1171,\"gridY\":135,\"type\":18},{\"gridX\":1157,\"gridY\":136,\"type\":17},{\"gridX\":1154,\"gridY\":151,\"type\":18},{\"gridX\":1123,\"gridY\":109,\"type\":18},{\"gridX\":1145,\"gridY\":168,\"type\":17},{\"gridX\":1133,\"gridY\":113,\"type\":18},{\"gridX\":1129,\"gridY\":127,\"type\":17},{\"gridX\":1147,\"gridY\":118,\"type\":17},{\"gridX\":1146,\"gridY\":136,\"type\":18},{\"gridX\":1133,\"gridY\":144,\"type\":18},{\"gridX\":1141,\"gridY\":156,\"type\":18},{\"gridX\":1124,\"gridY\":177,\"type\":18},{\"gridX\":1125,\"gridY\":188,\"type\":17},{\"gridX\":1131,\"gridY\":166,\"type\":18},{\"gridX\":1159,\"gridY\":96,\"type\":6},{\"gridX\":1232,\"gridY\":0,\"type\":4},{\"gridX\":1246,\"gridY\":0,\"type\":5},{\"gridX\":1168,\"gridY\":0,\"type\":5}],\"grounds\":[{\"gridX\":1038,\"gridY\":20,\"type\":5},{\"gridX\":1046,\"gridY\":20,\"type\":7},{\"gridX\":1110,\"gridY\":20,\"type\":9},{\"gridX\":48,\"gridY\":50,\"type\":10},{\"gridX\":312,\"gridY\":50,\"type\":12},{\"gridX\":56,\"gridY\":50,\"type\":13},{\"gridX\":184,\"gridY\":50,\"type\":13},{\"gridX\":376,\"gridY\":50,\"type\":14},{\"gridX\":256,\"gridY\":48,\"type\":15},{\"gridX\":1151,\"gridY\":48,\"type\":15},{\"gridX\":264,\"gridY\":48,\"type\":16},{\"gridX\":1159,\"gridY\":48,\"type\":18},{\"gridX\":296,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":896,\"gridY\":90,\"type\":20},{\"gridX\":224,\"gridY\":90,\"type\":20},{\"gridX\":328,\"gridY\":90,\"type\":21},{\"gridX\":1256,\"gridY\":90,\"type\":21},{\"gridX\":264,\"gridY\":90,\"type\":22},{\"gridX\":1160,\"gridY\":90,\"type\":22},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":1416,\"gridY\":90,\"type\":23},{\"gridX\":1544,\"gridY\":90,\"type\":23},{\"gridX\":392,\"gridY\":90,\"type\":23},{\"gridX\":520,\"gridY\":90,\"type\":23},{\"gridX\":904,\"gridY\":90,\"type\":23},{\"gridX\":1032,\"gridY\":90,\"type\":23},{\"gridX\":1672,\"gridY\":90,\"type\":24},{\"gridX\":648,\"gridY\":90,\"type\":24},{\"gridX\":136,\"gridY\":90,\"type\":24},{\"gridX\":1280,\"gridY\":0,\"type\":25},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":232,\"gridY\":88,\"type\":0},{\"gridX\":360,\"gridY\":88,\"type\":0},{\"gridX\":1224,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":120,\"gridY\":80,\"type\":3},{\"gridX\":312,\"gridY\":80,\"type\":3},{\"gridX\":61,\"gridY\":80,\"type\":3},{\"gridX\":1051,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":1408,\"gridY\":96,\"type\":12},{\"gridX\":184,\"gridY\":55,\"type\":26},{\"gridX\":260,\"gridY\":75,\"type\":27},{\"gridX\":246,\"gridY\":55,\"type\":41},{\"gridX\":256,\"gridY\":96,\"type\":22},{\"gridX\":260,\"gridY\":96,\"type\":22},{\"gridX\":904,\"gridY\":96,\"type\":24},{\"gridX\":996,\"gridY\":95,\"type\":35},{\"gridX\":1036,\"gridY\":95,\"type\":35},{\"gridX\":1076,\"gridY\":95,\"type\":35},{\"gridX\":1184,\"gridY\":96,\"type\":58},{\"gridX\":1184,\"gridY\":113,\"type\":60},{\"gridX\":1190,\"gridY\":105,\"type\":59},{\"gridX\":416,\"gridY\":96,\"type\":52},{\"gridX\":432,\"gridY\":96,\"type\":52},{\"gridX\":464,\"gridY\":96,\"type\":51},{\"gridX\":480,\"gridY\":96,\"type\":51},{\"gridX\":402,\"gridY\":96,\"type\":53},{\"gridX\":409,\"gridY\":96,\"type\":55},{\"gridX\":432,\"gridY\":96,\"type\":56},{\"gridX\":501,\"gridY\":95,\"type\":35},{\"gridX\":432,\"gridY\":112,\"type\":51},{\"gridX\":416,\"gridY\":112,\"type\":51},{\"gridX\":496,\"gridY\":160,\"type\":50},{\"gridX\":512,\"gridY\":96,\"type\":51},{\"gridX\":512,\"gridY\":160,\"type\":50},{\"gridX\":528,\"gridY\":96,\"type\":51},{\"gridX\":528,\"gridY\":160,\"type\":50},{\"gridX\":548,\"gridY\":95,\"type\":35},{\"gridX\":560,\"gridY\":96,\"type\":51},{\"gridX\":576,\"gridY\":96,\"type\":51},{\"gridX\":544,\"gridY\":160,\"type\":50},{\"gridX\":595,\"gridY\":96,\"type\":57},{\"gridX\":370,\"gridY\":55,\"type\":21},{\"gridX\":464,\"gridY\":160,\"type\":52},{\"gridX\":448,\"gridY\":160,\"type\":52},{\"gridX\":464,\"gridY\":192,\"type\":52},{\"gridX\":464,\"gridY\":20" << "8,\"type\":52},{\"gridX\":416,\"gridY\":160,\"type\":52},{\"gridX\":416,\"gridY\":176,\"type\":52},{\"gridX\":416,\"gridY\":192,\"type\":52},{\"gridX\":479,\"gridY\":160,\"type\":52},{\"gridX\":560,\"gridY\":160,\"type\":50},{\"gridX\":592,\"gridY\":160,\"type\":50},{\"gridX\":576,\"gridY\":160,\"type\":50},{\"gridX\":560,\"gridY\":192,\"type\":52},{\"gridX\":576,\"gridY\":192,\"type\":52},{\"gridX\":560,\"gridY\":208,\"type\":52},{\"gridX\":464,\"gridY\":224,\"type\":52},{\"gridX\":448,\"gridY\":96,\"type\":51},{\"gridX\":70,\"gridY\":99,\"type\":60},{\"gridX\":90,\"gridY\":99,\"type\":60},{\"gridX\":79,\"gridY\":96,\"type\":58},{\"gridX\":129,\"gridY\":152,\"type\":60},{\"gridX\":140,\"gridY\":144,\"type\":59},{\"gridX\":148,\"gridY\":152,\"type\":60},{\"gridX\":234,\"gridY\":163,\"type\":60},{\"gridX\":228,\"gridY\":154,\"type\":59},{\"gridX\":216,\"gridY\":163,\"type\":60},{\"gridX\":163,\"gridY\":229,\"type\":60},{\"gridX\":182,\"gridY\":229,\"type\":60},{\"gridX\":201,\"gridY\":229,\"type\":60},{\"gridX\":175,\"gridY\":220,\"type\":59},{\"gridX\":194,\"gridY\":220,\"type\":59},{\"gridX\":331,\"gridY\":96,\"type\":58},{\"gridX\":329,\"gridY\":100,\"type\":60},{\"gridX\":254,\"gridY\":163,\"type\":60},{\"gridX\":251,\"gridY\":160,\"type\":59},{\"gridX\":270,\"gridY\":163,\"type\":60},{\"gridX\":432,\"gridY\":208,\"type\":52},{\"gridX\":286,\"gridY\":180,\"type\":61},{\"gridX\":1135,\"gridY\":191,\"type\":60},{\"gridX\":1151,\"gridY\":191,\"type\":60},{\"gridX\":1116,\"gridY\":208,\"type\":61},{\"gridX\":1146,\"gridY\":179,\"type\":59},{\"gridX\":1117,\"gridY\":138,\"type\":60},{\"gridX\":1277,\"gridY\":84,\"type\":31},{\"gridX\":1277,\"gridY\":80,\"type\":31},{\"gridX\":1232,\"gridY\":75,\"type\":41}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1376,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":208,\"gridY\":55,\"type\":11},{\"gridX\":688,\"gridY\":124,\"type\":3},{\"gridX\":768,\"gridY\":124,\"type\":3},{\"gridX\":848,\"gridY\":124,\"type\":3},{\"gridX\":1016,\"gridY\":96,\"type\":5},{\"gridX\":1056,\"gridY\":96,\"type\":5},{\"gridX\":638,\"gridY\":124,\"type\":3},{\"gridX\":274,\"gridY\":96,\"type\":5},{\"gridX\":297,\"gridY\":96,\"type\":5}],\"collectibles\":[{\"gridX\":160,\"gridY\":64,\"type\":0},{\"gridX\":144,\"gridY\":64,\"type\":0},{\"gridX\":152,\"gridY\":64,\"type\":0},{\"gridX\":184,\"gridY\":72,\"type\":0},{\"gridX\":192,\"gridY\":72,\"type\":0},{\"gridX\":200,\"gridY\":72,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":0},{\"gridX\":260,\"gridY\":112,\"type\":0},{\"gridX\":264,\"gridY\":120,\"type\":0},{\"gridX\":316,\"gridY\":80,\"type\":0},{\"gridX\":316,\"gridY\":72,\"type\":0},{\"gridX\":124,\"gridY\":72,\"type\":0},{\"gridX\":124,\"gridY\":80,\"type\":0},{\"gridX\":124,\"gridY\":64,\"type\":0},{\"gridX\":316,\"gridY\":64,\"type\":0},{\"gridX\":276,\"gridY\":108,\"type\":0},{\"gridX\":276,\"gridY\":115,\"type\":0},{\"gridX\":276,\"gridY\":124,\"type\":0},{\"gridX\":372,\"gridY\":64,\"type\":0},{\"gridX\":372,\"gridY\":72,\"type\":0},{\"gridX\":372,\"gridY\":80,\"type\":0},{\"gridX\":411,\"gridY\":230,\"type\":0},{\"gridX\":421,\"gridY\":237,\"type\":0},{\"gridX\":433,\"gridY\":240,\"type\":0},{\"gridX\":316,\"gridY\":108,\"type\":0},{\"gridX\":577,\"gridY\":168,\"type\":0},{\"gridX\":376,\"gridY\":104,\"type\":0},{\"gridX\":380,\"gridY\":120,\"type\":0},{\"gridX\":384,\"gridY\":136,\"type\":0},{\"gridX\":388,\"gridY\":152,\"type\":0},{\"gridX\":392,\"gridY\":168,\"type\":0},{\"gridX\":396,\"gridY\":184,\"type\":0},{\"gridX\":400,\"gridY\":200,\"type\":0},{\"gridX\":404,\"gridY\":216,\"type\":0},{\"gridX\":627,\"gridY\":153,\"type\":0},{\"gridX\":565,\"gridY\":168,\"type\":0},{\"gridX\":316,\"gridY\":100,\"type\":0},{\"gridX\":316,\"gridY\":116,\"type\":0},{\"gridX\":620,\"gridY\":162,\"type\":0},{\"gridX\":554,\"gridY\":167,\"type\":0},{\"gridX\":543,\"gridY\":167,\"type\":0},{\"gridX\":453,\"gridY\":178,\"type\":1},{\"gridX\":336,\"gridY\":64,\"type\":0},{\"gridX\":344,\"gridY\":64,\"type\":0},{\"gridX\":352,\"gridY\":64,\"type\":0},{\"gridX\":588,\"gridY\":168,\"type\":0},{\"gridX\":599,\"gridY\":168,\"type\":0},{\"gridX\":664,\"gridY\":144,\"type\":0},{\"gridX\":610,\"gridY\":167,\"type\":0},{\"gridX\":672,\"gridY\":144,\"type\":0},{\"gridX\":680,\"gridY\":144,\"type\":0},{\"gridX\":716,\"gridY\":144,\"type\":0},{\"gridX\":700,\"gridY\":144,\"type\":0},{\"gridX\":708,\"gridY\":144,\"type\":0},{\"gridX\":752,\"gridY\":144,\"type\":0},{\"gridX\":760,\"gridY\":144,\"type\":0},{\"gridX\":744,\"gridY\":144,\"type\":0},{\"gridX\":796,\"gridY\":144,\"type\":0},{\"gridX\":788,\"gridY\":144,\"type\":0},{\"gridX\":780,\"gridY\":144,\"type\":0},{\"gridX\":840,\"gridY\":144,\"type\":0},{\"gridX\":824,\"gridY\":144,\"type\":0},{\"gridX\":832,\"gridY\":144,\"type\":0},{\"gridX\":1000,\"gridY\":132,\"type\":0},{\"gridX\":1008,\"gridY\":124,\"type\":0},{\"gridX\":1016,\"gridY\":116,\"type\":0},{\"gridX\":1032,\"gridY\":132,\"type\":0},{\"gridX\":1024,\"gridY\":124,\"type\":0},{\"gridX\":1048,\"gridY\":124,\"type\":0},{\"gridX\":1040,\"gridY\":132,\"type\":0},{\"gridX\":1056,\"gridY\":116,\"type\":0},{\"gridX\":1064,\"gridY\":124,\"type\":0},{\"gridX\":1072,\"gridY\":132,\"type\":0},{\"gridX\":1080,\"gridY\":132,\"type\":0},{\"gridX\":1088,\"gridY\":124,\"type\":0},{\"gridX\":1096,\"gridY\":116,\"type\":0},{\"gridX\":860,\"gridY\":144,\"type\":0},{\"gridX\":868,\"gridY\":144,\"type\":0},{\"gridX\":876,\"gridY\":144,\"type\":0},{\"gridX\":948,\"gridY\":108,\"type\":0},{\"gridX\":966,\"gridY\":108,\"type\":0},{\"gridX\":956,\"gridY\":108,\"type\":0},{\"gridX\":984,\"gridY\":116,\"type\":0},{\"gridX\":992,\"gridY\":124,\"type\":0},{\"gridX\":1300,\"gridY\":128,\"type\":1},{\"gridX\":1124,\"gridY\":104,\"type\":0},{\"gridX\":1132,\"gridY\":104,\"type\":0},{\"gridX\":1141,\"gridY\":104,\"type\":0},{\"gridX\":1196,\"gridY\":144,\"type\":0},{\"gridX\":1204,\"gridY\":144,\"type\":0},{\"gridX\":1188,\"gridY\":144,\"type\":0},{\"gridX\":1332,\"gridY\":112,\"type\":0},{\"gridX\":1316,\"gridY\":128,\"type\":0},{\"gridX\":1324,\"gridY\":120,\"type\":0},{\"gridX\":1336,\"gridY\":104,\"type\":0},{\"gridX\":640,\"gridY\":142,\"type\":1},{\"gridX\":445,\"gridY\":236,\"type\":0},{\"gridX\":452,\"gridY\":227,\"type\":0},{\"gridX\":452,\"gridY\":214,\"type\":0},{\"gridX\":452,\"gridY\":207,\"type\":0},{\"gridX\":452,\"gridY\":200,\"type\":0},{\"gridX\":453,\"gridY\":220,\"type\":0},{\"gridX\":452,\"gridY\":192,\"type\":0},{\"gridX\":67,\"gridY\":60,\"type\":3},{\"gridX\":132,\"gridY\":180,\"type\":0},{\"gridX\":140,\"gridY\":180,\"type\":0},{\"gridX\":156,\"gridY\":180,\"type\":0},{\"gridX\":164,\"gridY\":180,\"type\":0},{\"gridX\":148,\"gridY\":180,\"type\":0},{\"gridX\":224,\"gridY\":192,\"type\":0},{\"gridX\":232,\"gridY\":192,\"type\":0},{\"gridX\":240,\"gridY\":192,\"type\":0},{\"gridX\":256,\"gridY\":192,\"type\":0},{\"gridX\":248,\"gridY\":192,\"type\":0},{\"gridX\":280,\"gridY\":192,\"type\":0},{\"gridX\":264,\"gridY\":192,\"type\":0},{\"gridX\":272,\"gridY\":192,\"type\":0},{\"gridX\":288,\"gridY\":192,\"type\":0},{\"gridX\":296,\"gridY\":192,\"type\":0},{\"gridX\":1145,\"gridY\":219,\"type\":3},{\"gridX\":1160,\"gridY\":76,\"type\":2},{\"gridX\":1172,\"gridY\":76,\"type\":2},{\"gridX\":1196,\"gridY\":76,\"type\":2},{\"gridX\":1232,\"gridY\":76,\"type\":2},{\"gridX\":1220,\"gridY\":76,\"type\":2},{\"gridX\":1208,\"gridY\":76,\"type\":2},{\"gridX\":1184,\"gridY\":76,\"type\":2},{\"gridX\":1148,\"gridY\":76,\"type\":2}],\"jons\":[{\"gridX\":12,\"gridY\":96}],\"extraForegroundObjects\":[{\"gridX\":1280,\"gridY\":95,\"type\":39}],\"foregroundCoverObjects\":[{\"gridX\":416,\"gridY\":160,\"type\":6},{\"gridX\":432,\"gridY\":160,\"type\":6},{\"gridX\":416,\"gridY\":176,\"type\":4},{\"gridX\":432,\"gridY\":176,\"type\":4},{\"gridX\":416,\"gridY\":192,\"type\":6},{\"gridX\":432,\"gridY\":192,\"type\":4},{\"gridX\":432,\"gridY\":208,\"type\":6},{\"gridX\":416,\"gridY\":208,\"type\":8},{\"gri" << "dX\":432,\"gridY\":224,\"type\":8},{\"gridX\":464,\"gridY\":160,\"type\":6},{\"gridX\":464,\"gridY\":176,\"type\":4},{\"gridX\":464,\"gridY\":192,\"type\":6},{\"gridX\":464,\"gridY\":208,\"type\":4},{\"gridX\":480,\"gridY\":160,\"type\":6},{\"gridX\":480,\"gridY\":176,\"type\":4},{\"gridX\":480,\"gridY\":192,\"type\":6},{\"gridX\":480,\"gridY\":208,\"type\":9},{\"gridX\":560,\"gridY\":208,\"type\":4},{\"gridX\":560,\"gridY\":192,\"type\":6},{\"gridX\":576,\"gridY\":192,\"type\":6},{\"gridX\":576,\"gridY\":208,\"type\":4},{\"gridX\":576,\"gridY\":224,\"type\":9},{\"gridX\":560,\"gridY\":224,\"type\":8},{\"gridX\":448,\"gridY\":160,\"type\":6},{\"gridX\":945,\"gridY\":92,\"type\":1},{\"gridX\":959,\"gridY\":92,\"type\":3},{\"gridX\":464,\"gridY\":240,\"type\":9},{\"gridX\":464,\"gridY\":224,\"type\":6},{\"gridX\":60,\"gridY\":92,\"type\":1},{\"gridX\":84,\"gridY\":92,\"type\":3}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level14(text.c_str());
}

void Chapter1Level14::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level14 * Chapter1Level14::getInstance()
{
    return s_pInstance;
}

Chapter1Level14::Chapter1Level14(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 15 ///

Chapter1Level15 * Chapter1Level15::s_pInstance = nullptr;

void Chapter1Level15::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":15,\"midgrounds\":[{\"gridX\":268,\"gridY\":0,\"type\":3},{\"gridX\":176,\"gridY\":96,\"type\":2},{\"gridX\":1333,\"gridY\":0,\"type\":3},{\"gridX\":1397,\"gridY\":0,\"type\":3},{\"gridX\":1420,\"gridY\":0,\"type\":3},{\"gridX\":1456,\"gridY\":0,\"type\":3},{\"gridX\":1646,\"gridY\":0,\"type\":3},{\"gridX\":1680,\"gridY\":0,\"type\":3},{\"gridX\":1212,\"gridY\":96,\"type\":2},{\"gridX\":296,\"gridY\":55,\"type\":7},{\"gridX\":72,\"gridY\":128,\"type\":19},{\"gridX\":88,\"gridY\":112,\"type\":19},{\"gridX\":88,\"gridY\":128,\"type\":19},{\"gridX\":104,\"gridY\":112,\"type\":19},{\"gridX\":104,\"gridY\":128,\"type\":19},{\"gridX\":120,\"gridY\":112,\"type\":19},{\"gridX\":120,\"gridY\":128,\"type\":19},{\"gridX\":136,\"gridY\":112,\"type\":19},{\"gridX\":152,\"gridY\":112,\"type\":19},{\"gridX\":136,\"gridY\":128,\"type\":19},{\"gridX\":152,\"gridY\":128,\"type\":19},{\"gridX\":331,\"gridY\":125,\"type\":17},{\"gridX\":325,\"gridY\":141,\"type\":18},{\"gridX\":414,\"gridY\":115,\"type\":18},{\"gridX\":405,\"gridY\":127,\"type\":18},{\"gridX\":337,\"gridY\":152,\"type\":17},{\"gridX\":338,\"gridY\":138,\"type\":18},{\"gridX\":334,\"gridY\":165,\"type\":18},{\"gridX\":349,\"gridY\":160,\"type\":18},{\"gridX\":344,\"gridY\":173,\"type\":17},{\"gridX\":363,\"gridY\":166,\"type\":18},{\"gridX\":409,\"gridY\":141,\"type\":18},{\"gridX\":374,\"gridY\":156,\"type\":18},{\"gridX\":385,\"gridY\":164,\"type\":18},{\"gridX\":400,\"gridY\":157,\"type\":18},{\"gridX\":386,\"gridY\":176,\"type\":17},{\"gridX\":365,\"gridY\":179,\"type\":17},{\"gridX\":412,\"gridY\":172,\"type\":18},{\"gridX\":403,\"gridY\":172,\"type\":17},{\"gridX\":410,\"gridY\":190,\"type\":18},{\"gridX\":399,\"gridY\":184,\"type\":18},{\"gridX\":616,\"gridY\":96,\"type\":19},{\"gridX\":632,\"gridY\":96,\"type\":19},{\"gridX\":616,\"gridY\":80,\"type\":19},{\"gridX\":632,\"gridY\":80,\"type\":19},{\"gridX\":616,\"gridY\":32,\"type\":19},{\"gridX\":632,\"gridY\":48,\"type\":19},{\"gridX\":616,\"gridY\":48,\"type\":19},{\"gridX\":632,\"gridY\":64,\"type\":19},{\"gridX\":616,\"gridY\":64,\"type\":19},{\"gridX\":632,\"gridY\":32,\"type\":19},{\"gridX\":648,\"gridY\":64,\"type\":19},{\"gridX\":664,\"gridY\":64,\"type\":19},{\"gridX\":680,\"gridY\":64,\"type\":19},{\"gridX\":664,\"gridY\":80,\"type\":19},{\"gridX\":792,\"gridY\":64,\"type\":19},{\"gridX\":704,\"gridY\":8,\"type\":7},{\"gridX\":704,\"gridY\":20,\"type\":7},{\"gridX\":648,\"gridY\":8,\"type\":7},{\"gridX\":648,\"gridY\":20,\"type\":7},{\"gridX\":688,\"gridY\":20,\"type\":7},{\"gridX\":680,\"gridY\":20,\"type\":7},{\"gridX\":672,\"gridY\":20,\"type\":7},{\"gridX\":664,\"gridY\":20,\"type\":7},{\"gridX\":656,\"gridY\":20,\"type\":7},{\"gridX\":696,\"gridY\":20,\"type\":7},{\"gridX\":688,\"gridY\":8,\"type\":7},{\"gridX\":680,\"gridY\":8,\"type\":7},{\"gridX\":672,\"gridY\":8,\"type\":7},{\"gridX\":664,\"gridY\":8,\"type\":7},{\"gridX\":656,\"gridY\":8,\"type\":7},{\"gridX\":696,\"gridY\":8,\"type\":7},{\"gridX\":712,\"gridY\":32,\"type\":7},{\"gridX\":720,\"gridY\":32,\"type\":7},{\"gridX\":728,\"gridY\":32,\"type\":7},{\"gridX\":736,\"gridY\":32,\"type\":7},{\"gridX\":744,\"gridY\":32,\"type\":7},{\"gridX\":752,\"gridY\":32,\"type\":7},{\"gridX\":776,\"gridY\":128,\"type\":19},{\"gridX\":792,\"gridY\":128,\"type\":19},{\"gridX\":776,\"gridY\":144,\"type\":19},{\"gridX\":792,\"gridY\":144,\"type\":19},{\"gridX\":664,\"gridY\":96,\"type\":19},{\"gridX\":680,\"gridY\":96,\"type\":19},{\"gridX\":680,\"gridY\":80,\"type\":19},{\"gridX\":696,\"gridY\":96,\"type\":19},{\"gridX\":888,\"gridY\":96,\"type\":19},{\"gridX\":904,\"gridY\":96,\"type\":19},{\"gridX\":920,\"gridY\":96,\"type\":19},{\"gridX\":936,\"gridY\":96,\"type\":19},{\"gridX\":968,\"gridY\":96,\"type\":19},{\"gridX\":1000,\"gridY\":96,\"type\":19},{\"gridX\":952,\"gridY\":97,\"type\":19},{\"gridX\":984,\"gridY\":96,\"type\":19},{\"gridX\":952,\"gridY\":128,\"type\":20},{\"gridX\":888,\"gridY\":112,\"type\":19},{\"gridX\":904,\"gridY\":112,\"type\":19},{\"gridX\":920,\"gridY\":112,\"type\":19},{\"gridX\":936,\"gridY\":112,\"type\":19},{\"gridX\":952,\"gridY\":112,\"type\":19},{\"gridX\":1096,\"gridY\":0,\"type\":4},{\"gridX\":990,\"gridY\":0,\"type\":5},{\"gridX\":1179,\"gridY\":0,\"type\":5},{\"gridX\":296,\"gridY\":67,\"type\":7},{\"gridX\":1168,\"gridY\":96,\"type\":13},{\"gridX\":1071,\"gridY\":96,\"type\":14},{\"gridX\":1773,\"gridY\":96,\"type\":11},{\"gridX\":1809,\"gridY\":96,\"type\":13},{\"gridX\":1889,\"gridY\":96,\"type\":14},{\"gridX\":348,\"gridY\":96,\"type\":17},{\"gridX\":345,\"gridY\":111,\"type\":18},{\"gridX\":361,\"gridY\":96,\"type\":18},{\"gridX\":377,\"gridY\":96,\"type\":17},{\"gridX\":399,\"gridY\":96,\"type\":17},{\"gridX\":399,\"gridY\":112,\"type\":18},{\"gridX\":368,\"gridY\":104,\"type\":18},{\"gridX\":1220,\"gridY\":0,\"type\":4},{\"gridX\":1222,\"gridY\":0,\"type\":5},{\"gridX\":88,\"gridY\":144,\"type\":19},{\"gridX\":104,\"gridY\":144,\"type\":19},{\"gridX\":120,\"gridY\":144,\"type\":19}],\"grounds\":[{\"gridX\":276,\"gridY\":0,\"type\":0},{\"gridX\":588,\"gridY\":0,\"type\":0},{\"gridX\":1283,\"gridY\":0,\"type\":0},{\"gridX\":1375,\"gridY\":0,\"type\":0},{\"gridX\":284,\"gridY\":0,\"type\":1},{\"gridX\":1291,\"gridY\":0,\"type\":1},{\"gridX\":724,\"gridY\":0,\"type\":2},{\"gridX\":596,\"gridY\":0,\"type\":3},{\"gridX\":788,\"gridY\":0,\"type\":3},{\"gridX\":316,\"gridY\":0,\"type\":4},{\"gridX\":916,\"gridY\":0,\"type\":4},{\"gridX\":1323,\"gridY\":0,\"type\":4},{\"gridX\":1383,\"gridY\":0,\"type\":4},{\"gridX\":1444,\"gridY\":20,\"type\":5},{\"gridX\":1548,\"gridY\":20,\"type\":6},{\"gridX\":1452,\"gridY\":20,\"type\":7},{\"gridX\":1580,\"gridY\":20,\"type\":9},{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":975,\"gridY\":50,\"type\":10},{\"gridX\":264,\"gridY\":50,\"type\":11},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":136,\"gridY\":50,\"type\":13},{\"gridX\":983,\"gridY\":50,\"type\":13},{\"gridX\":1111,\"gridY\":50,\"type\":14},{\"gridX\":1138,\"gridY\":48,\"type\":15},{\"gridX\":1146,\"gridY\":48,\"type\":16},{\"gridX\":1178,\"gridY\":48,\"type\":17},{\"gridX\":1242,\"gridY\":48,\"type\":19},{\"gridX\":832,\"gridY\":90,\"type\":20},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":264,\"gridY\":90,\"type\":21},{\"gridX\":840,\"gridY\":90,\"type\":22},{\"gridX\":904,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":1160,\"gridY\":90,\"type\":23},{\"gridX\":1032,\"gridY\":90,\"type\":23},{\"gridX\":1288,\"gridY\":90,\"type\":24},{\"gridX\":320,\"gridY\":0,\"type\":25},{\"gridX\":1760,\"gridY\":0,\"type\":25},{\"gridX\":1768,\"gridY\":0,\"type\":26},{\"gridX\":328,\"gridY\":0,\"type\":26},{\"gridX\":424,\"gridY\":0,\"type\":26},{\"gridX\":360,\"gridY\":0,\"type\":27},{\"gridX\":456,\"gridY\":0,\"type\":27},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":28},{\"gridX\":2056,\"gridY\":0,\"type\":29},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":355,\"gridY\":0,\"type\":31},{\"gridX\":410,\"gridY\":23,\"type\":34},{\"gridX\":434,\"gridY\":23,\"type\":35},{\"gridX\":480,\"gridY\":0,\"type\":30},{\"gridX\":466,\"gridY\":23,\"type\":36}],\"exitGrounds\":[{\"gridX\":296,\"gridY\":48,\"type\":2},{\"gridX\":296,\"gridY\":88,\"type\":1},{\"gridX\":1516,\"gridY\":0,\"type\":3}],\"holes\":[{\"gridX\":224,\"gridY\":80,\"type\":3},{\"gridX\":276,\"gridY\":32,\"type\":4}],\"foregroundObjects\":[{\"gridX\":1792,\"gridY\":96,\"type\":12},{\"gridX\":176,\"gridY\":95,\"type\":35},{\"gridX\":244,\"gridY\":55,\"type\":27},{\"gridX\":244,\"gridY\":84,\"type\":30},{\"gridX\":308,\"gridY\":7,\"type\":21},{\"gridX\":296,\"gridY\":76,\"type\":34},{\"gridX\":296,\"gridY\":60,\"type\":34},{\"gridX\":1576,\"gridY\":27,\"type\":20},{\"gridX\":72,\"gridY\":96,\"type\":52},{\"gridX\":72,\"gridY\":112,\"type\":52},{\"gridX\":88,\"gridY\":96,\"type\":52},{\"gridX\":104,\"gridY\":96,\"type\":52},{\"gridX\":120,\"gridY\":96,\"type\":52}" << ",{\"gridX\":136,\"gridY\":96,\"type\":52},{\"gridX\":152,\"gridY\":96,\"type\":52},{\"gridX\":136,\"gridY\":160,\"type\":52},{\"gridX\":168,\"gridY\":144,\"type\":50},{\"gridX\":184,\"gridY\":144,\"type\":50},{\"gridX\":65,\"gridY\":124,\"type\":50},{\"gridX\":64,\"gridY\":96,\"type\":53},{\"gridX\":186,\"gridY\":96,\"type\":57},{\"gridX\":331,\"gridY\":96,\"type\":58},{\"gridX\":329,\"gridY\":106,\"type\":60},{\"gridX\":419,\"gridY\":96,\"type\":58},{\"gridX\":410,\"gridY\":97,\"type\":60},{\"gridX\":343,\"gridY\":184,\"type\":59},{\"gridX\":343,\"gridY\":188,\"type\":60},{\"gridX\":360,\"gridY\":188,\"type\":60},{\"gridX\":381,\"gridY\":209,\"type\":60},{\"gridX\":382,\"gridY\":193,\"type\":59},{\"gridX\":393,\"gridY\":192,\"type\":59},{\"gridX\":404,\"gridY\":209,\"type\":60},{\"gridX\":424,\"gridY\":209,\"type\":60},{\"gridX\":397,\"gridY\":203,\"type\":59},{\"gridX\":418,\"gridY\":200,\"type\":59},{\"gridX\":552,\"gridY\":0,\"type\":52},{\"gridX\":552,\"gridY\":16,\"type\":52},{\"gridX\":552,\"gridY\":32,\"type\":52},{\"gridX\":552,\"gridY\":96,\"type\":52},{\"gridX\":552,\"gridY\":80,\"type\":52},{\"gridX\":552,\"gridY\":64,\"type\":52},{\"gridX\":552,\"gridY\":48,\"type\":52},{\"gridX\":568,\"gridY\":96,\"type\":52},{\"gridX\":584,\"gridY\":96,\"type\":52},{\"gridX\":600,\"gridY\":80,\"type\":52},{\"gridX\":600,\"gridY\":64,\"type\":52},{\"gridX\":600,\"gridY\":48,\"type\":52},{\"gridX\":600,\"gridY\":32,\"type\":52},{\"gridX\":600,\"gridY\":16,\"type\":52},{\"gridX\":600,\"gridY\":0,\"type\":52},{\"gridX\":600,\"gridY\":96,\"type\":52},{\"gridX\":648,\"gridY\":96,\"type\":52},{\"gridX\":648,\"gridY\":80,\"type\":52},{\"gridX\":648,\"gridY\":112,\"type\":52},{\"gridX\":696,\"gridY\":64,\"type\":52},{\"gridX\":648,\"gridY\":48,\"type\":52},{\"gridX\":648,\"gridY\":32,\"type\":52},{\"gridX\":664,\"gridY\":32,\"type\":52},{\"gridX\":664,\"gridY\":48,\"type\":52},{\"gridX\":680,\"gridY\":32,\"type\":52},{\"gridX\":680,\"gridY\":48,\"type\":52},{\"gridX\":696,\"gridY\":32,\"type\":52},{\"gridX\":696,\"gridY\":48,\"type\":52},{\"gridX\":644,\"gridY\":60,\"type\":50},{\"gridX\":664,\"gridY\":128,\"type\":52},{\"gridX\":680,\"gridY\":128,\"type\":52},{\"gridX\":568,\"gridY\":29,\"type\":50},{\"gridX\":584,\"gridY\":29,\"type\":50},{\"gridX\":760,\"gridY\":0,\"type\":52},{\"gridX\":760,\"gridY\":16,\"type\":52},{\"gridX\":760,\"gridY\":32,\"type\":52},{\"gridX\":776,\"gridY\":48,\"type\":52},{\"gridX\":792,\"gridY\":48,\"type\":52},{\"gridX\":808,\"gridY\":48,\"type\":52},{\"gridX\":794,\"gridY\":64,\"type\":41},{\"gridX\":743,\"gridY\":7,\"type\":26},{\"gridX\":776,\"gridY\":80,\"type\":52},{\"gridX\":808,\"gridY\":64,\"type\":52},{\"gridX\":808,\"gridY\":80,\"type\":52},{\"gridX\":776,\"gridY\":96,\"type\":52},{\"gridX\":808,\"gridY\":32,\"type\":52},{\"gridX\":808,\"gridY\":16,\"type\":52},{\"gridX\":808,\"gridY\":0,\"type\":52},{\"gridX\":792,\"gridY\":29,\"type\":50},{\"gridX\":776,\"gridY\":29,\"type\":50},{\"gridX\":776,\"gridY\":112,\"type\":52},{\"gridX\":696,\"gridY\":128,\"type\":52},{\"gridX\":696,\"gridY\":144,\"type\":52},{\"gridX\":696,\"gridY\":160,\"type\":51},{\"gridX\":760,\"gridY\":172,\"type\":50},{\"gridX\":712,\"gridY\":172,\"type\":50},{\"gridX\":776,\"gridY\":176,\"type\":52},{\"gridX\":792,\"gridY\":160,\"type\":52},{\"gridX\":776,\"gridY\":160,\"type\":52},{\"gridX\":792,\"gridY\":176,\"type\":52},{\"gridX\":728,\"gridY\":172,\"type\":50},{\"gridX\":744,\"gridY\":172,\"type\":50},{\"gridX\":792,\"gridY\":112,\"type\":52},{\"gridX\":664,\"gridY\":112,\"type\":52},{\"gridX\":680,\"gridY\":112,\"type\":52},{\"gridX\":712,\"gridY\":92,\"type\":50},{\"gridX\":968,\"gridY\":112,\"type\":52},{\"gridX\":984,\"gridY\":112,\"type\":52},{\"gridX\":1000,\"gridY\":112,\"type\":52},{\"gridX\":968,\"gridY\":128,\"type\":52},{\"gridX\":984,\"gridY\":128,\"type\":52},{\"gridX\":1000,\"gridY\":128,\"type\":52},{\"gridX\":968,\"gridY\":144,\"type\":52},{\"gridX\":984,\"gridY\":144,\"type\":52},{\"gridX\":1000,\"gridY\":144,\"type\":52},{\"gridX\":968,\"gridY\":160,\"type\":52},{\"gridX\":984,\"gridY\":160,\"type\":52},{\"gridX\":1000,\"gridY\":160,\"type\":52},{\"gridX\":968,\"gridY\":176,\"type\":52},{\"gridX\":968,\"gridY\":192,\"type\":52},{\"gridX\":984,\"gridY\":176,\"type\":52},{\"gridX\":984,\"gridY\":192,\"type\":52},{\"gridX\":968,\"gridY\":208,\"type\":52},{\"gridX\":480,\"gridY\":93,\"type\":50},{\"gridX\":496,\"gridY\":93,\"type\":50},{\"gridX\":496,\"gridY\":37,\"type\":50},{\"gridX\":760,\"gridY\":48,\"type\":51},{\"gridX\":730,\"gridY\":7,\"type\":20},{\"gridX\":696,\"gridY\":80,\"type\":51},{\"gridX\":728,\"gridY\":92,\"type\":50},{\"gridX\":744,\"gridY\":92,\"type\":50}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1760,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":383,\"gridY\":110,\"type\":3},{\"gridX\":888,\"gridY\":96,\"type\":11},{\"gridX\":928,\"gridY\":96,\"type\":5},{\"gridX\":964,\"gridY\":96,\"type\":5},{\"gridX\":1009,\"gridY\":97,\"type\":5},{\"gridX\":1080,\"gridY\":96,\"type\":11},{\"gridX\":1136,\"gridY\":96,\"type\":11},{\"gridX\":1184,\"gridY\":96,\"type\":11},{\"gridX\":1284,\"gridY\":96,\"type\":4},{\"gridX\":144,\"gridY\":112,\"type\":11}],\"collectibles\":[{\"gridX\":128,\"gridY\":120,\"type\":0},{\"gridX\":136,\"gridY\":128,\"type\":0},{\"gridX\":143,\"gridY\":136,\"type\":0},{\"gridX\":152,\"gridY\":145,\"type\":0},{\"gridX\":161,\"gridY\":151,\"type\":0},{\"gridX\":180,\"gridY\":152,\"type\":0},{\"gridX\":188,\"gridY\":152,\"type\":0},{\"gridX\":196,\"gridY\":152,\"type\":0},{\"gridX\":212,\"gridY\":144,\"type\":0},{\"gridX\":220,\"gridY\":136,\"type\":0},{\"gridX\":228,\"gridY\":128,\"type\":0},{\"gridX\":228,\"gridY\":116,\"type\":0},{\"gridX\":228,\"gridY\":104,\"type\":0},{\"gridX\":228,\"gridY\":80,\"type\":0},{\"gridX\":228,\"gridY\":72,\"type\":0},{\"gridX\":228,\"gridY\":64,\"type\":0},{\"gridX\":280,\"gridY\":68,\"type\":1},{\"gridX\":228,\"gridY\":56,\"type\":0},{\"gridX\":284,\"gridY\":40,\"type\":0},{\"gridX\":284,\"gridY\":16,\"type\":0},{\"gridX\":284,\"gridY\":32,\"type\":0},{\"gridX\":284,\"gridY\":24,\"type\":0},{\"gridX\":284,\"gridY\":8,\"type\":0},{\"gridX\":312,\"gridY\":40,\"type\":0},{\"gridX\":312,\"gridY\":32,\"type\":0},{\"gridX\":312,\"gridY\":72,\"type\":0},{\"gridX\":312,\"gridY\":80,\"type\":0},{\"gridX\":312,\"gridY\":122,\"type\":0},{\"gridX\":316,\"gridY\":132,\"type\":0},{\"gridX\":324,\"gridY\":140,\"type\":0},{\"gridX\":332,\"gridY\":140,\"type\":0},{\"gridX\":340,\"gridY\":140,\"type\":0},{\"gridX\":403,\"gridY\":129,\"type\":0},{\"gridX\":393,\"gridY\":127,\"type\":0},{\"gridX\":384,\"gridY\":121,\"type\":0},{\"gridX\":412,\"gridY\":127,\"type\":0},{\"gridX\":430,\"gridY\":125,\"type\":0},{\"gridX\":421,\"gridY\":126,\"type\":0},{\"gridX\":568,\"gridY\":120,\"type\":0},{\"gridX\":560,\"gridY\":120,\"type\":0},{\"gridX\":552,\"gridY\":120,\"type\":0},{\"gridX\":584,\"gridY\":120,\"type\":0},{\"gridX\":592,\"gridY\":120,\"type\":0},{\"gridX\":576,\"gridY\":120,\"type\":0},{\"gridX\":600,\"gridY\":120,\"type\":0},{\"gridX\":608,\"gridY\":120,\"type\":0},{\"gridX\":616,\"gridY\":120,\"type\":0},{\"gridX\":624,\"gridY\":92,\"type\":0},{\"gridX\":624,\"gridY\":104,\"type\":0},{\"gridX\":624,\"gridY\":116,\"type\":0},{\"gridX\":796,\"gridY\":68,\"type\":1},{\"gridX\":624,\"gridY\":80,\"type\":0},{\"gridX\":624,\"gridY\":12,\"type\":0},{\"gridX\":632,\"gridY\":12,\"type\":0},{\"gridX\":680,\"gridY\":12,\"type\":0},{\"gridX\":672,\"gridY\":12,\"type\":0},{\"gridX\":664,\"gridY\":12,\"type\":0},{\"gridX\":656,\"gridY\":12,\"type\":0},{\"gridX\":648,\"gridY\":12,\"type\":0},{\"gridX\":640,\"gridY\":12,\"type\":0},{\"gridX\":624,\"gridY\":56,\"type\":0},{\"gridX\":688,\"gridY\":12,\"type\":0},{\"gridX\":624,\"gridY\":68,\"type\":0},{\"gridX\":848,\"gridY\":104,\"type\":0},{\"gridX\":856,\"gridY\":104,\"type\":0},{\"gridX\":864,\"gridY\":104,\"type\":0},{\"gridX\":981,\"gridY\":98,\"type\":1},{\"gridX\":990,\"g" << "ridY\":100,\"type\":0},{\"gridX\":1000,\"gridY\":100,\"type\":0},{\"gridX\":971,\"gridY\":100,\"type\":0},{\"gridX\":1056,\"gridY\":104,\"type\":0},{\"gridX\":1064,\"gridY\":104,\"type\":0},{\"gridX\":1072,\"gridY\":104,\"type\":0},{\"gridX\":1104,\"gridY\":104,\"type\":0},{\"gridX\":1112,\"gridY\":104,\"type\":0},{\"gridX\":1120,\"gridY\":104,\"type\":0},{\"gridX\":312,\"gridY\":16,\"type\":0},{\"gridX\":312,\"gridY\":24,\"type\":0},{\"gridX\":312,\"gridY\":56,\"type\":0},{\"gridX\":312,\"gridY\":64,\"type\":0},{\"gridX\":1152,\"gridY\":104,\"type\":0},{\"gridX\":1160,\"gridY\":104,\"type\":0},{\"gridX\":1168,\"gridY\":104,\"type\":0},{\"gridX\":1216,\"gridY\":104,\"type\":0},{\"gridX\":1224,\"gridY\":104,\"type\":0},{\"gridX\":1208,\"gridY\":104,\"type\":0},{\"gridX\":1556,\"gridY\":36,\"type\":0},{\"gridX\":1301,\"gridY\":97,\"type\":0},{\"gridX\":1292,\"gridY\":108,\"type\":0},{\"gridX\":1304,\"gridY\":84,\"type\":0},{\"gridX\":1304,\"gridY\":44,\"type\":0},{\"gridX\":1572,\"gridY\":36,\"type\":0},{\"gridX\":1304,\"gridY\":64,\"type\":0},{\"gridX\":1564,\"gridY\":36,\"type\":0},{\"gridX\":1336,\"gridY\":52,\"type\":0},{\"gridX\":1344,\"gridY\":52,\"type\":0},{\"gridX\":1352,\"gridY\":52,\"type\":0},{\"gridX\":1384,\"gridY\":48,\"type\":0},{\"gridX\":1392,\"gridY\":48,\"type\":0},{\"gridX\":1400,\"gridY\":48,\"type\":0},{\"gridX\":1432,\"gridY\":44,\"type\":0},{\"gridX\":1440,\"gridY\":44,\"type\":0},{\"gridX\":1448,\"gridY\":44,\"type\":0},{\"gridX\":101,\"gridY\":170,\"type\":0},{\"gridX\":111,\"gridY\":170,\"type\":0},{\"gridX\":92,\"gridY\":170,\"type\":0},{\"gridX\":436,\"gridY\":36,\"type\":0},{\"gridX\":460,\"gridY\":36,\"type\":0},{\"gridX\":1071,\"gridY\":59,\"type\":2},{\"gridX\":1047,\"gridY\":59,\"type\":2},{\"gridX\":1062,\"gridY\":62,\"type\":3},{\"gridX\":1127,\"gridY\":43,\"type\":3},{\"gridX\":448,\"gridY\":34,\"type\":2}],\"jons\":[{\"gridX\":677,\"gridY\":128},{\"gridX\":15,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":72,\"gridY\":96,\"type\":5},{\"gridX\":88,\"gridY\":96,\"type\":5},{\"gridX\":104,\"gridY\":96,\"type\":7},{\"gridX\":136,\"gridY\":96,\"type\":7},{\"gridX\":120,\"gridY\":96,\"type\":5},{\"gridX\":152,\"gridY\":96,\"type\":5},{\"gridX\":72,\"gridY\":112,\"type\":6},{\"gridX\":72,\"gridY\":160,\"type\":10},{\"gridX\":88,\"gridY\":160,\"type\":10},{\"gridX\":104,\"gridY\":160,\"type\":11},{\"gridX\":120,\"gridY\":160,\"type\":10},{\"gridX\":136,\"gridY\":176,\"type\":8},{\"gridX\":136,\"gridY\":160,\"type\":6},{\"gridX\":152,\"gridY\":160,\"type\":6},{\"gridX\":152,\"gridY\":176,\"type\":9},{\"gridX\":552,\"gridY\":32,\"type\":4},{\"gridX\":600,\"gridY\":64,\"type\":4},{\"gridX\":584,\"gridY\":64,\"type\":4},{\"gridX\":568,\"gridY\":64,\"type\":4},{\"gridX\":600,\"gridY\":96,\"type\":4},{\"gridX\":584,\"gridY\":96,\"type\":4},{\"gridX\":568,\"gridY\":96,\"type\":4},{\"gridX\":552,\"gridY\":96,\"type\":4},{\"gridX\":552,\"gridY\":64,\"type\":4},{\"gridX\":696,\"gridY\":64,\"type\":4},{\"gridX\":648,\"gridY\":96,\"type\":4},{\"gridX\":600,\"gridY\":32,\"type\":4},{\"gridX\":584,\"gridY\":32,\"type\":4},{\"gridX\":568,\"gridY\":32,\"type\":4},{\"gridX\":648,\"gridY\":80,\"type\":6},{\"gridX\":600,\"gridY\":48,\"type\":6},{\"gridX\":584,\"gridY\":48,\"type\":6},{\"gridX\":568,\"gridY\":48,\"type\":6},{\"gridX\":552,\"gridY\":48,\"type\":6},{\"gridX\":600,\"gridY\":80,\"type\":6},{\"gridX\":584,\"gridY\":80,\"type\":6},{\"gridX\":568,\"gridY\":80,\"type\":6},{\"gridX\":552,\"gridY\":80,\"type\":6},{\"gridX\":648,\"gridY\":48,\"type\":6},{\"gridX\":696,\"gridY\":80,\"type\":6},{\"gridX\":696,\"gridY\":48,\"type\":6},{\"gridX\":680,\"gridY\":48,\"type\":6},{\"gridX\":664,\"gridY\":48,\"type\":6},{\"gridX\":696,\"gridY\":32,\"type\":4},{\"gridX\":680,\"gridY\":32,\"type\":4},{\"gridX\":664,\"gridY\":32,\"type\":4},{\"gridX\":648,\"gridY\":32,\"type\":4},{\"gridX\":552,\"gridY\":112,\"type\":10},{\"gridX\":648,\"gridY\":112,\"type\":6},{\"gridX\":664,\"gridY\":112,\"type\":6},{\"gridX\":680,\"gridY\":112,\"type\":6},{\"gridX\":696,\"gridY\":112,\"type\":6},{\"gridX\":664,\"gridY\":128,\"type\":4},{\"gridX\":680,\"gridY\":128,\"type\":4},{\"gridX\":648,\"gridY\":128,\"type\":8},{\"gridX\":696,\"gridY\":128,\"type\":9},{\"gridX\":680,\"gridY\":144,\"type\":9},{\"gridX\":664,\"gridY\":144,\"type\":8},{\"gridX\":568,\"gridY\":112,\"type\":11},{\"gridX\":584,\"gridY\":112,\"type\":10},{\"gridX\":600,\"gridY\":112,\"type\":11},{\"gridX\":832,\"gridY\":92,\"type\":3},{\"gridX\":760,\"gridY\":32,\"type\":4},{\"gridX\":808,\"gridY\":32,\"type\":4},{\"gridX\":792,\"gridY\":32,\"type\":4},{\"gridX\":776,\"gridY\":32,\"type\":4},{\"gridX\":808,\"gridY\":64,\"type\":4},{\"gridX\":792,\"gridY\":80,\"type\":4},{\"gridX\":776,\"gridY\":64,\"type\":4},{\"gridX\":760,\"gridY\":64,\"type\":4},{\"gridX\":760,\"gridY\":48,\"type\":6},{\"gridX\":792,\"gridY\":48,\"type\":6},{\"gridX\":776,\"gridY\":48,\"type\":6},{\"gridX\":808,\"gridY\":48,\"type\":6},{\"gridX\":808,\"gridY\":80,\"type\":6},{\"gridX\":776,\"gridY\":80,\"type\":6},{\"gridX\":760,\"gridY\":80,\"type\":6},{\"gridX\":776,\"gridY\":96,\"type\":4},{\"gridX\":776,\"gridY\":112,\"type\":6},{\"gridX\":808,\"gridY\":96,\"type\":9},{\"gridX\":760,\"gridY\":96,\"type\":8},{\"gridX\":792,\"gridY\":96,\"type\":4},{\"gridX\":776,\"gridY\":160,\"type\":4},{\"gridX\":792,\"gridY\":160,\"type\":4},{\"gridX\":776,\"gridY\":176,\"type\":6},{\"gridX\":792,\"gridY\":176,\"type\":6},{\"gridX\":776,\"gridY\":192,\"type\":8},{\"gridX\":792,\"gridY\":192,\"type\":9},{\"gridX\":792,\"gridY\":112,\"type\":6},{\"gridX\":904,\"gridY\":128,\"type\":10},{\"gridX\":920,\"gridY\":128,\"type\":10},{\"gridX\":936,\"gridY\":128,\"type\":11},{\"gridX\":888,\"gridY\":128,\"type\":10},{\"gridX\":968,\"gridY\":112,\"type\":6},{\"gridX\":984,\"gridY\":112,\"type\":6},{\"gridX\":1000,\"gridY\":112,\"type\":6},{\"gridX\":968,\"gridY\":128,\"type\":4},{\"gridX\":979,\"gridY\":128,\"type\":4},{\"gridX\":979,\"gridY\":128,\"type\":4},{\"gridX\":979,\"gridY\":128,\"type\":4},{\"gridX\":984,\"gridY\":128,\"type\":4},{\"gridX\":1000,\"gridY\":128,\"type\":4},{\"gridX\":1000,\"gridY\":144,\"type\":6},{\"gridX\":984,\"gridY\":144,\"type\":6},{\"gridX\":968,\"gridY\":144,\"type\":6},{\"gridX\":1000,\"gridY\":160,\"type\":4},{\"gridX\":984,\"gridY\":160,\"type\":4},{\"gridX\":968,\"gridY\":160,\"type\":4},{\"gridX\":968,\"gridY\":208,\"type\":6},{\"gridX\":984,\"gridY\":208,\"type\":6},{\"gridX\":968,\"gridY\":224,\"type\":8},{\"gridX\":984,\"gridY\":224,\"type\":9},{\"gridX\":1026,\"gridY\":85,\"type\":0},{\"gridX\":1092,\"gridY\":92,\"type\":3},{\"gridX\":422,\"gridY\":92,\"type\":1},{\"gridX\":431,\"gridY\":92,\"type\":3},{\"gridX\":339,\"gridY\":92,\"type\":2},{\"gridX\":299,\"gridY\":92,\"type\":3},{\"gridX\":321,\"gridY\":92,\"type\":1},{\"gridX\":1000,\"gridY\":176,\"type\":9},{\"gridX\":496,\"gridY\":92,\"type\":1},{\"gridX\":469,\"gridY\":92,\"type\":2},{\"gridX\":476,\"gridY\":92,\"type\":3},{\"gridX\":72,\"gridY\":144,\"type\":12},{\"gridX\":136,\"gridY\":144,\"type\":12},{\"gridX\":152,\"gridY\":144,\"type\":12},{\"gridX\":88,\"gridY\":144,\"type\":14},{\"gridX\":104,\"gridY\":144,\"type\":14},{\"gridX\":120,\"gridY\":144,\"type\":14}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level15(text.c_str());
}

void Chapter1Level15::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level15 * Chapter1Level15::getInstance()
{
    return s_pInstance;
}

Chapter1Level15::Chapter1Level15(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 16 ///

Chapter1Level16 * Chapter1Level16::s_pInstance = nullptr;

void Chapter1Level16::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":16,\"midgrounds\":[{\"gridX\":1780,\"gridY\":96,\"type\":2},{\"gridX\":1828,\"gridY\":96,\"type\":2},{\"gridX\":32,\"gridY\":96,\"type\":2},{\"gridX\":555,\"gridY\":0,\"type\":5},{\"gridX\":728,\"gridY\":0,\"type\":4},{\"gridX\":771,\"gridY\":0,\"type\":5},{\"gridX\":604,\"gridY\":0,\"type\":3},{\"gridX\":888,\"gridY\":0,\"type\":3},{\"gridX\":936,\"gridY\":0,\"type\":3},{\"gridX\":1032,\"gridY\":0,\"type\":3},{\"gridX\":1080,\"gridY\":0,\"type\":3},{\"gridX\":1140,\"gridY\":0,\"type\":3},{\"gridX\":1372,\"gridY\":0,\"type\":3},{\"gridX\":1464,\"gridY\":0,\"type\":3},{\"gridX\":1536,\"gridY\":0,\"type\":3},{\"gridX\":1628,\"gridY\":0,\"type\":3},{\"gridX\":184,\"gridY\":0,\"type\":3},{\"gridX\":228,\"gridY\":7,\"type\":7},{\"gridX\":228,\"gridY\":19,\"type\":7},{\"gridX\":228,\"gridY\":31,\"type\":7},{\"gridX\":228,\"gridY\":43,\"type\":7},{\"gridX\":228,\"gridY\":55,\"type\":7},{\"gridX\":228,\"gridY\":67,\"type\":7},{\"gridX\":145,\"gridY\":0,\"type\":5},{\"gridX\":25,\"gridY\":0,\"type\":4},{\"gridX\":79,\"gridY\":96,\"type\":6},{\"gridX\":5,\"gridY\":96,\"type\":15},{\"gridX\":59,\"gridY\":96,\"type\":13},{\"gridX\":189,\"gridY\":96,\"type\":17},{\"gridX\":209,\"gridY\":96,\"type\":17},{\"gridX\":542,\"gridY\":75,\"type\":17},{\"gridX\":375,\"gridY\":96,\"type\":17},{\"gridX\":392,\"gridY\":116,\"type\":17},{\"gridX\":413,\"gridY\":124,\"type\":17},{\"gridX\":435,\"gridY\":124,\"type\":17},{\"gridX\":455,\"gridY\":119,\"type\":17},{\"gridX\":475,\"gridY\":115,\"type\":17},{\"gridX\":526,\"gridY\":83,\"type\":18},{\"gridX\":492,\"gridY\":107,\"type\":18},{\"gridX\":503,\"gridY\":95,\"type\":18},{\"gridX\":517,\"gridY\":96,\"type\":18},{\"gridX\":563,\"gridY\":75,\"type\":17},{\"gridX\":384,\"gridY\":102,\"type\":18},{\"gridX\":460,\"gridY\":109,\"type\":18},{\"gridX\":422,\"gridY\":112,\"type\":18},{\"gridX\":429,\"gridY\":146,\"type\":17},{\"gridX\":489,\"gridY\":92,\"type\":18},{\"gridX\":577,\"gridY\":75,\"type\":17},{\"gridX\":536,\"gridY\":95,\"type\":17},{\"gridX\":510,\"gridY\":108,\"type\":18},{\"gridX\":468,\"gridY\":135,\"type\":17},{\"gridX\":449,\"gridY\":138,\"type\":17},{\"gridX\":430,\"gridY\":166,\"type\":17},{\"gridX\":501,\"gridY\":122,\"type\":17},{\"gridX\":470,\"gridY\":151,\"type\":17},{\"gridX\":470,\"gridY\":191,\"type\":18},{\"gridX\":449,\"gridY\":180,\"type\":17},{\"gridX\":469,\"gridY\":173,\"type\":17},{\"gridX\":449,\"gridY\":160,\"type\":17},{\"gridX\":456,\"gridY\":192,\"type\":17},{\"gridX\":490,\"gridY\":152,\"type\":17},{\"gridX\":473,\"gridY\":205,\"type\":18},{\"gridX\":522,\"gridY\":111,\"type\":18},{\"gridX\":503,\"gridY\":132,\"type\":18},{\"gridX\":523,\"gridY\":122,\"type\":17},{\"gridX\":529,\"gridY\":132,\"type\":18},{\"gridX\":487,\"gridY\":165,\"type\":18},{\"gridX\":502,\"gridY\":162,\"type\":18},{\"gridX\":514,\"gridY\":142,\"type\":17},{\"gridX\":504,\"gridY\":150,\"type\":18},{\"gridX\":532,\"gridY\":148,\"type\":18},{\"gridX\":526,\"gridY\":180,\"type\":18},{\"gridX\":520,\"gridY\":164,\"type\":17},{\"gridX\":541,\"gridY\":149,\"type\":17},{\"gridX\":540,\"gridY\":172,\"type\":17},{\"gridX\":532,\"gridY\":188,\"type\":18},{\"gridX\":542,\"gridY\":192,\"type\":17},{\"gridX\":554,\"gridY\":158,\"type\":18},{\"gridX\":530,\"gridY\":203,\"type\":18},{\"gridX\":548,\"gridY\":206,\"type\":18},{\"gridX\":484,\"gridY\":144,\"type\":7},{\"gridX\":484,\"gridY\":156,\"type\":7},{\"gridX\":500,\"gridY\":156,\"type\":7},{\"gridX\":585,\"gridY\":85,\"type\":18},{\"gridX\":593,\"gridY\":97,\"type\":18},{\"gridX\":589,\"gridY\":0,\"type\":4},{\"gridX\":728,\"gridY\":84,\"type\":7},{\"gridX\":728,\"gridY\":72,\"type\":7},{\"gridX\":692,\"gridY\":24,\"type\":7},{\"gridX\":692,\"gridY\":12,\"type\":7},{\"gridX\":692,\"gridY\":0,\"type\":7},{\"gridX\":660,\"gridY\":24,\"type\":7},{\"gridX\":660,\"gridY\":0,\"type\":7},{\"gridX\":660,\"gridY\":12,\"type\":7},{\"gridX\":764,\"gridY\":84,\"type\":7},{\"gridX\":764,\"gridY\":72,\"type\":7},{\"gridX\":800,\"gridY\":120,\"type\":7},{\"gridX\":764,\"gridY\":120,\"type\":7},{\"gridX\":720,\"gridY\":96,\"type\":9},{\"gridX\":764,\"gridY\":132,\"type\":9},{\"gridX\":543,\"gridY\":58,\"type\":18},{\"gridX\":812,\"gridY\":132,\"type\":10},{\"gridX\":800,\"gridY\":48,\"type\":7},{\"gridX\":800,\"gridY\":60,\"type\":7},{\"gridX\":800,\"gridY\":72,\"type\":7},{\"gridX\":800,\"gridY\":84,\"type\":7},{\"gridX\":800,\"gridY\":36,\"type\":7},{\"gridX\":800,\"gridY\":0,\"type\":7},{\"gridX\":800,\"gridY\":12,\"type\":7},{\"gridX\":800,\"gridY\":24,\"type\":7},{\"gridX\":768,\"gridY\":96,\"type\":8},{\"gridX\":852,\"gridY\":120,\"type\":7},{\"gridX\":852,\"gridY\":12,\"type\":7},{\"gridX\":852,\"gridY\":24,\"type\":7},{\"gridX\":852,\"gridY\":36,\"type\":7},{\"gridX\":852,\"gridY\":48,\"type\":7},{\"gridX\":852,\"gridY\":60,\"type\":7},{\"gridX\":852,\"gridY\":72,\"type\":7},{\"gridX\":852,\"gridY\":84,\"type\":7},{\"gridX\":852,\"gridY\":96,\"type\":7},{\"gridX\":852,\"gridY\":108,\"type\":7},{\"gridX\":852,\"gridY\":0,\"type\":7},{\"gridX\":929,\"gridY\":96,\"type\":11},{\"gridX\":1256,\"gridY\":12,\"type\":7},{\"gridX\":1256,\"gridY\":0,\"type\":7},{\"gridX\":1256,\"gridY\":24,\"type\":7},{\"gridX\":1256,\"gridY\":36,\"type\":7},{\"gridX\":1256,\"gridY\":48,\"type\":7},{\"gridX\":1256,\"gridY\":60,\"type\":7},{\"gridX\":1256,\"gridY\":72,\"type\":7},{\"gridX\":1256,\"gridY\":84,\"type\":7},{\"gridX\":1256,\"gridY\":96,\"type\":7},{\"gridX\":1256,\"gridY\":108,\"type\":7},{\"gridX\":1256,\"gridY\":120,\"type\":7},{\"gridX\":1224,\"gridY\":120,\"type\":7},{\"gridX\":1224,\"gridY\":108,\"type\":7},{\"gridX\":1224,\"gridY\":96,\"type\":7},{\"gridX\":1224,\"gridY\":84,\"type\":7},{\"gridX\":1224,\"gridY\":72,\"type\":7},{\"gridX\":1224,\"gridY\":60,\"type\":7},{\"gridX\":1224,\"gridY\":48,\"type\":7},{\"gridX\":1224,\"gridY\":36,\"type\":7},{\"gridX\":1224,\"gridY\":24,\"type\":7},{\"gridX\":1224,\"gridY\":12,\"type\":7},{\"gridX\":1224,\"gridY\":0,\"type\":7},{\"gridX\":1332,\"gridY\":0,\"type\":7},{\"gridX\":1332,\"gridY\":12,\"type\":7},{\"gridX\":1332,\"gridY\":144,\"type\":7},{\"gridX\":1332,\"gridY\":132,\"type\":7},{\"gridX\":1332,\"gridY\":120,\"type\":7},{\"gridX\":1332,\"gridY\":108,\"type\":7},{\"gridX\":1332,\"gridY\":96,\"type\":7},{\"gridX\":1332,\"gridY\":84,\"type\":7},{\"gridX\":1332,\"gridY\":72,\"type\":7},{\"gridX\":1332,\"gridY\":60,\"type\":7},{\"gridX\":1332,\"gridY\":48,\"type\":7},{\"gridX\":1332,\"gridY\":36,\"type\":7},{\"gridX\":1332,\"gridY\":24,\"type\":7},{\"gridX\":181,\"gridY\":147,\"type\":17},{\"gridX\":201,\"gridY\":147,\"type\":17},{\"gridX\":168,\"gridY\":162,\"type\":17},{\"gridX\":153,\"gridY\":165,\"type\":18},{\"gridX\":171,\"gridY\":147,\"type\":18},{\"gridX\":187,\"gridY\":160,\"type\":18},{\"gridX\":160,\"gridY\":178,\"type\":18},{\"gridX\":940,\"gridY\":96,\"type\":7},{\"gridX\":940,\"gridY\":108,\"type\":7},{\"gridX\":940,\"gridY\":120,\"type\":7},{\"gridX\":940,\"gridY\":132,\"type\":7},{\"gridX\":940,\"gridY\":144,\"type\":7},{\"gridX\":1000,\"gridY\":96,\"type\":7},{\"gridX\":1000,\"gridY\":108,\"type\":7},{\"gridX\":1000,\"gridY\":120,\"type\":7},{\"gridX\":1000,\"gridY\":132,\"type\":7},{\"gridX\":1000,\"gridY\":144,\"type\":7},{\"gridX\":1076,\"gridY\":144,\"type\":7},{\"gridX\":1076,\"gridY\":132,\"type\":7},{\"gridX\":1076,\"gridY\":120,\"type\":7},{\"gridX\":1076,\"gridY\":108,\"type\":7},{\"gridX\":1076,\"gridY\":96,\"type\":7},{\"gridX\":1770,\"gridY\":192,\"type\":7},{\"gridX\":1778,\"gridY\":192,\"type\":7}],\"grounds\":[{\"gridX\":702,\"gridY\":0,\"type\":0},{\"gridX\":1104,\"gridY\":0,\"type\":0},{\"gridX\":1264,\"gridY\":0,\"type\":0},{\"gridX\":1724,\"gridY\":0,\"type\":0},{\"gridX\":1412,\"gridY\":0,\"type\":0},{\"gridX\":1584,\"gridY\":0,\"type\":0},{\"gridX\":160,\"gridY\":0,\"type\":0},{\"gridX\":256,\"gridY\":0,\"type\":0},{\"gridX\":1144,\"gridY\":0,\"type\":1},{\"gridX\":1732,\"gridY\":0,\"type\":1},{\"gridX\":1112,\"gridY\":0,\"type\":1},{\"gridX\":710,\"gridY\":0,\"type\":2},{\"gridX\":168,\"g" << "ridY\":0,\"type\":2},{\"gridX\":264,\"gridY\":0,\"type\":3},{\"gridX\":392,\"gridY\":0,\"type\":3},{\"gridX\":520,\"gridY\":0,\"type\":3},{\"gridX\":648,\"gridY\":0,\"type\":4},{\"gridX\":1176,\"gridY\":0,\"type\":4},{\"gridX\":1272,\"gridY\":0,\"type\":4},{\"gridX\":1420,\"gridY\":0,\"type\":4},{\"gridX\":774,\"gridY\":0,\"type\":4},{\"gridX\":1592,\"gridY\":0,\"type\":4},{\"gridX\":232,\"gridY\":0,\"type\":4},{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":836,\"gridY\":50,\"type\":10},{\"gridX\":136,\"gridY\":50,\"type\":11},{\"gridX\":844,\"gridY\":50,\"type\":12},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":908,\"gridY\":50,\"type\":14},{\"gridX\":168,\"gridY\":50,\"type\":14},{\"gridX\":554,\"gridY\":48,\"type\":15},{\"gridX\":716,\"gridY\":48,\"type\":15},{\"gridX\":561,\"gridY\":48,\"type\":16},{\"gridX\":724,\"gridY\":48,\"type\":17},{\"gridX\":788,\"gridY\":48,\"type\":19},{\"gridX\":593,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":896,\"gridY\":90,\"type\":20},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":1032,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":904,\"gridY\":90,\"type\":23},{\"gridX\":392,\"gridY\":90,\"type\":24},{\"gridX\":1160,\"gridY\":90,\"type\":24},{\"gridX\":1760,\"gridY\":0,\"type\":25},{\"gridX\":1768,\"gridY\":0,\"type\":26},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":28},{\"gridX\":2056,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[{\"gridX\":124,\"gridY\":73,\"type\":0},{\"gridX\":140,\"gridY\":73,\"type\":1},{\"gridX\":156,\"gridY\":73,\"type\":2}],\"foregroundObjects\":[{\"gridX\":1817,\"gridY\":205,\"type\":12},{\"gridX\":300,\"gridY\":16,\"type\":34},{\"gridX\":340,\"gridY\":16,\"type\":34},{\"gridX\":320,\"gridY\":76,\"type\":34},{\"gridX\":360,\"gridY\":76,\"type\":34},{\"gridX\":380,\"gridY\":16,\"type\":34},{\"gridX\":415,\"gridY\":7,\"type\":27},{\"gridX\":1748,\"gridY\":7,\"type\":21},{\"gridX\":228,\"gridY\":60,\"type\":34},{\"gridX\":228,\"gridY\":76,\"type\":34},{\"gridX\":228,\"gridY\":42,\"type\":34},{\"gridX\":172,\"gridY\":96,\"type\":58},{\"gridX\":171,\"gridY\":108,\"type\":59},{\"gridX\":226,\"gridY\":96,\"type\":58},{\"gridX\":217,\"gridY\":106,\"type\":59},{\"gridX\":141,\"gridY\":169,\"type\":59},{\"gridX\":155,\"gridY\":189,\"type\":60},{\"gridX\":181,\"gridY\":117,\"type\":63},{\"gridX\":148,\"gridY\":179,\"type\":59},{\"gridX\":199,\"gridY\":116,\"type\":63},{\"gridX\":194,\"gridY\":129,\"type\":59},{\"gridX\":206,\"gridY\":131,\"type\":59},{\"gridX\":197,\"gridY\":139,\"type\":59},{\"gridX\":211,\"gridY\":145,\"type\":59},{\"gridX\":210,\"gridY\":156,\"type\":60},{\"gridX\":558,\"gridY\":96,\"type\":63},{\"gridX\":359,\"gridY\":96,\"type\":58},{\"gridX\":365,\"gridY\":106,\"type\":59},{\"gridX\":376,\"gridY\":116,\"type\":59},{\"gridX\":384,\"gridY\":131,\"type\":63},{\"gridX\":409,\"gridY\":142,\"type\":63},{\"gridX\":398,\"gridY\":133,\"type\":59},{\"gridX\":377,\"gridY\":146,\"type\":60},{\"gridX\":389,\"gridY\":151,\"type\":60},{\"gridX\":348,\"gridY\":105,\"type\":60},{\"gridX\":544,\"gridY\":98,\"type\":59},{\"gridX\":578,\"gridY\":96,\"type\":63},{\"gridX\":493,\"gridY\":98,\"type\":41},{\"gridX\":488,\"gridY\":8,\"type\":63},{\"gridX\":476,\"gridY\":7,\"type\":59},{\"gridX\":486,\"gridY\":58,\"type\":59},{\"gridX\":482,\"gridY\":116,\"type\":63},{\"gridX\":464,\"gridY\":21,\"type\":59},{\"gridX\":473,\"gridY\":55,\"type\":59},{\"gridX\":482,\"gridY\":96,\"type\":63},{\"gridX\":479,\"gridY\":22,\"type\":59},{\"gridX\":465,\"gridY\":61,\"type\":60},{\"gridX\":482,\"gridY\":74,\"type\":63},{\"gridX\":468,\"gridY\":83,\"type\":21},{\"gridX\":448,\"gridY\":7,\"type\":27},{\"gridX\":461,\"gridY\":50,\"type\":33},{\"gridX\":465,\"gridY\":36,\"type\":63},{\"gridX\":465,\"gridY\":58,\"type\":63},{\"gridX\":413,\"gridY\":161,\"type\":63},{\"gridX\":421,\"gridY\":176,\"type\":59},{\"gridX\":422,\"gridY\":190,\"type\":59},{\"gridX\":435,\"gridY\":188,\"type\":63},{\"gridX\":449,\"gridY\":202,\"type\":59},{\"gridX\":458,\"gridY\":210,\"type\":60},{\"gridX\":482,\"gridY\":129,\"type\":59},{\"gridX\":489,\"gridY\":139,\"type\":59},{\"gridX\":538,\"gridY\":107,\"type\":59},{\"gridX\":544,\"gridY\":115,\"type\":63},{\"gridX\":565,\"gridY\":116,\"type\":63},{\"gridX\":583,\"gridY\":112,\"type\":59},{\"gridX\":537,\"gridY\":149,\"type\":60},{\"gridX\":562,\"gridY\":129,\"type\":59},{\"gridX\":545,\"gridY\":136,\"type\":63},{\"gridX\":554,\"gridY\":149,\"type\":60},{\"gridX\":578,\"gridY\":125,\"type\":60},{\"gridX\":473,\"gridY\":164,\"type\":47},{\"gridX\":477,\"gridY\":164,\"type\":48},{\"gridX\":493,\"gridY\":164,\"type\":48},{\"gridX\":509,\"gridY\":164,\"type\":49},{\"gridX\":592,\"gridY\":102,\"type\":59},{\"gridX\":603,\"gridY\":109,\"type\":59},{\"gridX\":601,\"gridY\":125,\"type\":60},{\"gridX\":594,\"gridY\":119,\"type\":59},{\"gridX\":625,\"gridY\":125,\"type\":60},{\"gridX\":613,\"gridY\":122,\"type\":59},{\"gridX\":536,\"gridY\":231,\"type\":60},{\"gridX\":534,\"gridY\":217,\"type\":59},{\"gridX\":550,\"gridY\":218,\"type\":59},{\"gridX\":475,\"gridY\":206,\"type\":59},{\"gridX\":481,\"gridY\":7,\"type\":27},{\"gridX\":736,\"gridY\":92,\"type\":50},{\"gridX\":844,\"gridY\":128,\"type\":50},{\"gridX\":720,\"gridY\":92,\"type\":50},{\"gridX\":800,\"gridY\":92,\"type\":50},{\"gridX\":752,\"gridY\":92,\"type\":50},{\"gridX\":768,\"gridY\":92,\"type\":50},{\"gridX\":784,\"gridY\":92,\"type\":50},{\"gridX\":828,\"gridY\":128,\"type\":50},{\"gridX\":812,\"gridY\":128,\"type\":50},{\"gridX\":764,\"gridY\":128,\"type\":50},{\"gridX\":780,\"gridY\":128,\"type\":50},{\"gridX\":796,\"gridY\":128,\"type\":50},{\"gridX\":655,\"gridY\":33,\"type\":50},{\"gridX\":671,\"gridY\":33,\"type\":50},{\"gridX\":687,\"gridY\":33,\"type\":50},{\"gridX\":1110,\"gridY\":95,\"type\":35},{\"gridX\":912,\"gridY\":95,\"type\":35},{\"gridX\":1220,\"gridY\":128,\"type\":48},{\"gridX\":1216,\"gridY\":128,\"type\":47},{\"gridX\":1236,\"gridY\":128,\"type\":48},{\"gridX\":1252,\"gridY\":128,\"type\":48},{\"gridX\":1268,\"gridY\":128,\"type\":49},{\"gridX\":1328,\"gridY\":84,\"type\":46},{\"gridX\":1392,\"gridY\":168,\"type\":62},{\"gridX\":1492,\"gridY\":172,\"type\":62},{\"gridX\":1784,\"gridY\":200,\"type\":48},{\"gridX\":1764,\"gridY\":200,\"type\":47},{\"gridX\":1896,\"gridY\":200,\"type\":48},{\"gridX\":1880,\"gridY\":200,\"type\":48},{\"gridX\":1864,\"gridY\":200,\"type\":48},{\"gridX\":1848,\"gridY\":200,\"type\":48},{\"gridX\":1832,\"gridY\":200,\"type\":48},{\"gridX\":1816,\"gridY\":200,\"type\":48},{\"gridX\":1800,\"gridY\":200,\"type\":48},{\"gridX\":1768,\"gridY\":200,\"type\":48},{\"gridX\":1960,\"gridY\":200,\"type\":48},{\"gridX\":1944,\"gridY\":200,\"type\":48},{\"gridX\":1928,\"gridY\":200,\"type\":48},{\"gridX\":1912,\"gridY\":200,\"type\":48},{\"gridX\":1592,\"gridY\":176,\"type\":62},{\"gridX\":1696,\"gridY\":200,\"type\":62},{\"gridX\":174,\"gridY\":130,\"type\":60},{\"gridX\":972,\"gridY\":95,\"type\":35},{\"gridX\":1032,\"gridY\":95,\"type\":35},{\"gridX\":940,\"gridY\":116,\"type\":34},{\"gridX\":940,\"gridY\":132,\"type\":34},{\"gridX\":940,\"gridY\":148,\"type\":34},{\"gridX\":1000,\"gridY\":132,\"type\":34},{\"gridX\":1000,\"gridY\":148,\"type\":34},{\"gridX\":1000,\"gridY\":116,\"type\":34},{\"gridX\":1076,\"gridY\":148,\"type\":34},{\"gridX\":1076,\"gridY\":132,\"type\":34},{\"gridX\":1076,\"gridY\":116,\"type\":34},{\"gridX\":1052,\"gridY\":95,\"type\":35},{\"gridX\":1121,\"gridY\":96,\"type\":58},{\"gridX\":1114,\"gridY\":124,\"type\":60},{\"gridX\":1121,\"gridY\":104,\"type\":63},{\"gridX\":1118,\"gridY\":114,\"type\":59},{\"gridX\":1140,\"gridY\":96,\"type\":58},{\"gridX\":1131,\"gridY\":103,\"type\":59},{\"gridX\":1336,\"gridY\":151,\"type\":49},{\"gridX\":1332,\"gridY\":151,\"type\":47},{\"gridX\":1763,\"gridY\":96,\"type\":58},{\"gridX\":1784,\"gridY\":96,\"type\":58},{\"gridX\":1774,\"gridY\":105,\"type\":59},{\"gridX" << "\":1771,\"gridY\":116,\"type\":63},{\"gridX\":1760,\"gridY\":132,\"type\":59},{\"gridX\":1776,\"gridY\":130,\"type\":59},{\"gridX\":1768,\"gridY\":142,\"type\":59},{\"gridX\":1762,\"gridY\":146,\"type\":63},{\"gridX\":1766,\"gridY\":171,\"type\":63},{\"gridX\":1774,\"gridY\":155,\"type\":59},{\"gridX\":1759,\"gridY\":161,\"type\":59}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1778,\"gridY\":205}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":276,\"gridY\":7,\"type\":6},{\"gridX\":296,\"gridY\":77,\"type\":7},{\"gridX\":316,\"gridY\":7,\"type\":6},{\"gridX\":336,\"gridY\":77,\"type\":7},{\"gridX\":356,\"gridY\":7,\"type\":6},{\"gridX\":376,\"gridY\":77,\"type\":7},{\"gridX\":396,\"gridY\":7,\"type\":6},{\"gridX\":1168,\"gridY\":7,\"type\":6},{\"gridX\":1264,\"gridY\":7,\"type\":6},{\"gridX\":1412,\"gridY\":7,\"type\":6},{\"gridX\":1584,\"gridY\":7,\"type\":6},{\"gridX\":550,\"gridY\":7,\"type\":6}],\"collectibles\":[{\"gridX\":184,\"gridY\":33,\"type\":0},{\"gridX\":184,\"gridY\":64,\"type\":0},{\"gridX\":184,\"gridY\":48,\"type\":0},{\"gridX\":183,\"gridY\":16,\"type\":1},{\"gridX\":252,\"gridY\":24,\"type\":0},{\"gridX\":268,\"gridY\":24,\"type\":0},{\"gridX\":260,\"gridY\":24,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0},{\"gridX\":136,\"gridY\":104,\"type\":0},{\"gridX\":144,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":112,\"type\":0},{\"gridX\":136,\"gridY\":112,\"type\":0},{\"gridX\":128,\"gridY\":120,\"type\":0},{\"gridX\":144,\"gridY\":64,\"type\":0},{\"gridX\":152,\"gridY\":64,\"type\":0},{\"gridX\":160,\"gridY\":64,\"type\":0},{\"gridX\":279,\"gridY\":33,\"type\":0},{\"gridX\":285,\"gridY\":48,\"type\":0},{\"gridX\":295,\"gridY\":63,\"type\":0},{\"gridX\":308,\"gridY\":48,\"type\":0},{\"gridX\":316,\"gridY\":32,\"type\":0},{\"gridX\":418,\"gridY\":90,\"type\":1},{\"gridX\":324,\"gridY\":32,\"type\":0},{\"gridX\":332,\"gridY\":48,\"type\":0},{\"gridX\":340,\"gridY\":64,\"type\":0},{\"gridX\":348,\"gridY\":48,\"type\":0},{\"gridX\":356,\"gridY\":32,\"type\":0},{\"gridX\":364,\"gridY\":32,\"type\":0},{\"gridX\":372,\"gridY\":48,\"type\":0},{\"gridX\":380,\"gridY\":64,\"type\":0},{\"gridX\":388,\"gridY\":48,\"type\":0},{\"gridX\":396,\"gridY\":32,\"type\":0},{\"gridX\":404,\"gridY\":32,\"type\":0},{\"gridX\":412,\"gridY\":48,\"type\":0},{\"gridX\":420,\"gridY\":64,\"type\":0},{\"gridX\":520,\"gridY\":130,\"type\":0},{\"gridX\":520,\"gridY\":140,\"type\":0},{\"gridX\":521,\"gridY\":150,\"type\":0},{\"gridX\":604,\"gridY\":80,\"type\":0},{\"gridX\":604,\"gridY\":72,\"type\":0},{\"gridX\":604,\"gridY\":64,\"type\":0},{\"gridX\":604,\"gridY\":56,\"type\":0},{\"gridX\":604,\"gridY\":48,\"type\":0},{\"gridX\":604,\"gridY\":40,\"type\":0},{\"gridX\":604,\"gridY\":32,\"type\":0},{\"gridX\":576,\"gridY\":79,\"type\":1},{\"gridX\":688,\"gridY\":60,\"type\":0},{\"gridX\":680,\"gridY\":44,\"type\":0},{\"gridX\":684,\"gridY\":52,\"type\":0},{\"gridX\":692,\"gridY\":68,\"type\":0},{\"gridX\":696,\"gridY\":76,\"type\":0},{\"gridX\":700,\"gridY\":84,\"type\":0},{\"gridX\":708,\"gridY\":84,\"type\":0},{\"gridX\":716,\"gridY\":84,\"type\":0},{\"gridX\":864,\"gridY\":84,\"type\":0},{\"gridX\":864,\"gridY\":92,\"type\":0},{\"gridX\":864,\"gridY\":100,\"type\":0},{\"gridX\":864,\"gridY\":108,\"type\":0},{\"gridX\":856,\"gridY\":136,\"type\":0},{\"gridX\":864,\"gridY\":116,\"type\":0},{\"gridX\":864,\"gridY\":132,\"type\":0},{\"gridX\":864,\"gridY\":124,\"type\":0},{\"gridX\":848,\"gridY\":136,\"type\":0},{\"gridX\":888,\"gridY\":60,\"type\":0},{\"gridX\":896,\"gridY\":60,\"type\":0},{\"gridX\":904,\"gridY\":60,\"type\":0},{\"gridX\":936,\"gridY\":52,\"type\":0},{\"gridX\":944,\"gridY\":52,\"type\":0},{\"gridX\":952,\"gridY\":52,\"type\":0},{\"gridX\":984,\"gridY\":48,\"type\":0},{\"gridX\":992,\"gridY\":48,\"type\":0},{\"gridX\":1000,\"gridY\":48,\"type\":0},{\"gridX\":1040,\"gridY\":44,\"type\":0},{\"gridX\":1032,\"gridY\":44,\"type\":0},{\"gridX\":1048,\"gridY\":44,\"type\":0},{\"gridX\":1080,\"gridY\":40,\"type\":0},{\"gridX\":1088,\"gridY\":40,\"type\":0},{\"gridX\":1096,\"gridY\":40,\"type\":0},{\"gridX\":1224,\"gridY\":40,\"type\":0},{\"gridX\":1232,\"gridY\":40,\"type\":0},{\"gridX\":1216,\"gridY\":40,\"type\":0},{\"gridX\":1320,\"gridY\":48,\"type\":0},{\"gridX\":1328,\"gridY\":48,\"type\":0},{\"gridX\":1312,\"gridY\":48,\"type\":0},{\"gridX\":1380,\"gridY\":40,\"type\":0},{\"gridX\":1544,\"gridY\":52,\"type\":0},{\"gridX\":1372,\"gridY\":40,\"type\":0},{\"gridX\":1388,\"gridY\":40,\"type\":0},{\"gridX\":1464,\"gridY\":64,\"type\":0},{\"gridX\":1472,\"gridY\":64,\"type\":0},{\"gridX\":1480,\"gridY\":64,\"type\":0},{\"gridX\":1552,\"gridY\":52,\"type\":0},{\"gridX\":1536,\"gridY\":52,\"type\":0},{\"gridX\":1636,\"gridY\":56,\"type\":0},{\"gridX\":1656,\"gridY\":56,\"type\":0},{\"gridX\":1628,\"gridY\":56,\"type\":0},{\"gridX\":1644,\"gridY\":56,\"type\":0},{\"gridX\":840,\"gridY\":136,\"type\":0},{\"gridX\":832,\"gridY\":136,\"type\":0},{\"gridX\":824,\"gridY\":136,\"type\":0},{\"gridX\":864,\"gridY\":76,\"type\":0},{\"gridX\":864,\"gridY\":68,\"type\":0},{\"gridX\":864,\"gridY\":60,\"type\":0},{\"gridX\":221,\"gridY\":185,\"type\":2},{\"gridX\":971,\"gridY\":124,\"type\":2},{\"gridX\":1039,\"gridY\":123,\"type\":2},{\"gridX\":1095,\"gridY\":104,\"type\":3},{\"gridX\":1396,\"gridY\":216,\"type\":2},{\"gridX\":1497,\"gridY\":222,\"type\":2},{\"gridX\":1597,\"gridY\":223,\"type\":2},{\"gridX\":1703,\"gridY\":216,\"type\":3},{\"gridX\":467,\"gridY\":238,\"type\":2}],\"jons\":[{\"gridX\":24,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level16(text.c_str());
}

void Chapter1Level16::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level16 * Chapter1Level16::getInstance()
{
    return s_pInstance;
}

Chapter1Level16::Chapter1Level16(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 17 ///

Chapter1Level17 * Chapter1Level17::s_pInstance = nullptr;

void Chapter1Level17::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":17,\"midgrounds\":[{\"gridX\":472,\"gridY\":0,\"type\":7},{\"gridX\":24,\"gridY\":148,\"type\":7},{\"gridX\":100,\"gridY\":136,\"type\":7},{\"gridX\":24,\"gridY\":160,\"type\":7},{\"gridX\":100,\"gridY\":148,\"type\":7},{\"gridX\":100,\"gridY\":160,\"type\":7},{\"gridX\":100,\"gridY\":184,\"type\":7},{\"gridX\":100,\"gridY\":172,\"type\":7},{\"gridX\":24,\"gridY\":172,\"type\":7},{\"gridX\":24,\"gridY\":184,\"type\":7},{\"gridX\":472,\"gridY\":132,\"type\":7},{\"gridX\":472,\"gridY\":120,\"type\":7},{\"gridX\":472,\"gridY\":108,\"type\":7},{\"gridX\":472,\"gridY\":96,\"type\":7},{\"gridX\":472,\"gridY\":84,\"type\":7},{\"gridX\":472,\"gridY\":72,\"type\":7},{\"gridX\":472,\"gridY\":60,\"type\":7},{\"gridX\":472,\"gridY\":48,\"type\":7},{\"gridX\":472,\"gridY\":36,\"type\":7},{\"gridX\":472,\"gridY\":24,\"type\":7},{\"gridX\":472,\"gridY\":12,\"type\":7},{\"gridX\":560,\"gridY\":172,\"type\":7},{\"gridX\":560,\"gridY\":196,\"type\":7},{\"gridX\":560,\"gridY\":184,\"type\":7},{\"gridX\":472,\"gridY\":156,\"type\":7},{\"gridX\":472,\"gridY\":144,\"type\":7},{\"gridX\":680,\"gridY\":108,\"type\":7},{\"gridX\":680,\"gridY\":96,\"type\":7},{\"gridX\":680,\"gridY\":84,\"type\":7},{\"gridX\":680,\"gridY\":72,\"type\":7},{\"gridX\":680,\"gridY\":60,\"type\":7},{\"gridX\":680,\"gridY\":48,\"type\":7},{\"gridX\":680,\"gridY\":36,\"type\":7},{\"gridX\":680,\"gridY\":24,\"type\":7},{\"gridX\":680,\"gridY\":12,\"type\":7},{\"gridX\":680,\"gridY\":0,\"type\":7},{\"gridX\":680,\"gridY\":120,\"type\":7},{\"gridX\":777,\"gridY\":113,\"type\":18},{\"gridX\":776,\"gridY\":146,\"type\":18},{\"gridX\":784,\"gridY\":129,\"type\":17},{\"gridX\":791,\"gridY\":119,\"type\":17},{\"gridX\":792,\"gridY\":139,\"type\":17},{\"gridX\":813,\"gridY\":119,\"type\":17},{\"gridX\":813,\"gridY\":137,\"type\":17},{\"gridX\":827,\"gridY\":114,\"type\":17},{\"gridX\":833,\"gridY\":130,\"type\":18},{\"gridX\":827,\"gridY\":141,\"type\":18},{\"gridX\":839,\"gridY\":141,\"type\":18},{\"gridX\":1068,\"gridY\":0,\"type\":7},{\"gridX\":1068,\"gridY\":84,\"type\":7},{\"gridX\":1068,\"gridY\":72,\"type\":7},{\"gridX\":1068,\"gridY\":60,\"type\":7},{\"gridX\":1068,\"gridY\":48,\"type\":7},{\"gridX\":1068,\"gridY\":36,\"type\":7},{\"gridX\":1068,\"gridY\":24,\"type\":7},{\"gridX\":1068,\"gridY\":12,\"type\":7},{\"gridX\":1068,\"gridY\":96,\"type\":7},{\"gridX\":1068,\"gridY\":108,\"type\":7},{\"gridX\":968,\"gridY\":128,\"type\":19},{\"gridX\":1176,\"gridY\":16,\"type\":19},{\"gridX\":1032,\"gridY\":128,\"type\":19},{\"gridX\":1032,\"gridY\":112,\"type\":19},{\"gridX\":1016,\"gridY\":128,\"type\":19},{\"gridX\":1000,\"gridY\":128,\"type\":19},{\"gridX\":984,\"gridY\":128,\"type\":19},{\"gridX\":1080,\"gridY\":128,\"type\":19},{\"gridX\":1160,\"gridY\":112,\"type\":19},{\"gridX\":1080,\"gridY\":144,\"type\":19},{\"gridX\":1080,\"gridY\":160,\"type\":19},{\"gridX\":1080,\"gridY\":113,\"type\":19},{\"gridX\":1064,\"gridY\":160,\"type\":19},{\"gridX\":1064,\"gridY\":176,\"type\":19},{\"gridX\":1160,\"gridY\":128,\"type\":19},{\"gridX\":1064,\"gridY\":192,\"type\":20},{\"gridX\":1160,\"gridY\":144,\"type\":20},{\"gridX\":1124,\"gridY\":0,\"type\":7},{\"gridX\":1124,\"gridY\":72,\"type\":7},{\"gridX\":1124,\"gridY\":60,\"type\":7},{\"gridX\":1124,\"gridY\":48,\"type\":7},{\"gridX\":1124,\"gridY\":36,\"type\":7},{\"gridX\":1124,\"gridY\":24,\"type\":7},{\"gridX\":1124,\"gridY\":12,\"type\":7},{\"gridX\":1124,\"gridY\":84,\"type\":7},{\"gridX\":1124,\"gridY\":96,\"type\":7},{\"gridX\":1124,\"gridY\":108,\"type\":7},{\"gridX\":1192,\"gridY\":16,\"type\":19},{\"gridX\":100,\"gridY\":100,\"type\":7},{\"gridX\":100,\"gridY\":112,\"type\":7},{\"gridX\":100,\"gridY\":124,\"type\":7},{\"gridX\":24,\"gridY\":100,\"type\":7},{\"gridX\":24,\"gridY\":112,\"type\":7},{\"gridX\":24,\"gridY\":124,\"type\":7},{\"gridX\":24,\"gridY\":136,\"type\":7},{\"gridX\":24,\"gridY\":16,\"type\":7},{\"gridX\":24,\"gridY\":28,\"type\":7},{\"gridX\":24,\"gridY\":40,\"type\":7},{\"gridX\":24,\"gridY\":52,\"type\":7},{\"gridX\":24,\"gridY\":64,\"type\":7},{\"gridX\":24,\"gridY\":76,\"type\":7},{\"gridX\":24,\"gridY\":88,\"type\":7},{\"gridX\":100,\"gridY\":28,\"type\":7},{\"gridX\":100,\"gridY\":40,\"type\":7},{\"gridX\":100,\"gridY\":52,\"type\":7},{\"gridX\":100,\"gridY\":64,\"type\":7},{\"gridX\":100,\"gridY\":76,\"type\":7},{\"gridX\":100,\"gridY\":88,\"type\":7},{\"gridX\":1456,\"gridY\":96,\"type\":17},{\"gridX\":1476,\"gridY\":96,\"type\":17},{\"gridX\":1410,\"gridY\":0,\"type\":4},{\"gridX\":1421,\"gridY\":0,\"type\":5},{\"gridX\":1466,\"gridY\":0,\"type\":5},{\"gridX\":1176,\"gridY\":0,\"type\":19},{\"gridX\":1192,\"gridY\":0,\"type\":19},{\"gridX\":1464,\"gridY\":121,\"type\":17},{\"gridX\":1486,\"gridY\":121,\"type\":17},{\"gridX\":1497,\"gridY\":96,\"type\":17},{\"gridX\":1517,\"gridY\":97,\"type\":17},{\"gridX\":1467,\"gridY\":107,\"type\":18},{\"gridX\":1365,\"gridY\":96,\"type\":11},{\"gridX\":1259,\"gridY\":96,\"type\":6},{\"gridX\":1434,\"gridY\":96,\"type\":2},{\"gridX\":1287,\"gridY\":96,\"type\":15},{\"gridX\":1319,\"gridY\":96,\"type\":16},{\"gridX\":1409,\"gridY\":96,\"type\":15},{\"gridX\":1476,\"gridY\":144,\"type\":7},{\"gridX\":1472,\"gridY\":168,\"type\":10},{\"gridX\":1508,\"gridY\":156,\"type\":7},{\"gridX\":1476,\"gridY\":156,\"type\":7},{\"gridX\":1508,\"gridY\":144,\"type\":7},{\"gridX\":2089,\"gridY\":7,\"type\":17},{\"gridX\":2089,\"gridY\":29,\"type\":17},{\"gridX\":2075,\"gridY\":46,\"type\":18},{\"gridX\":2091,\"gridY\":50,\"type\":17},{\"gridX\":2110,\"gridY\":7,\"type\":17},{\"gridX\":2109,\"gridY\":28,\"type\":17},{\"gridX\":2109,\"gridY\":50,\"type\":17},{\"gridX\":2128,\"gridY\":49,\"type\":17},{\"gridX\":2129,\"gridY\":7,\"type\":17},{\"gridX\":2128,\"gridY\":28,\"type\":17},{\"gridX\":1224,\"gridY\":96,\"type\":19},{\"gridX\":2083,\"gridY\":204,\"type\":17},{\"gridX\":2067,\"gridY\":213,\"type\":18},{\"gridX\":2109,\"gridY\":211,\"type\":18},{\"gridX\":2118,\"gridY\":222,\"type\":18},{\"gridX\":2097,\"gridY\":208,\"type\":18},{\"gridX\":2104,\"gridY\":194,\"type\":17},{\"gridX\":2123,\"gridY\":201,\"type\":17},{\"gridX\":2062,\"gridY\":195,\"type\":17},{\"gridX\":2075,\"gridY\":182,\"type\":17},{\"gridX\":2096,\"gridY\":182,\"type\":17},{\"gridX\":2117,\"gridY\":182,\"type\":17},{\"gridX\":2138,\"gridY\":182,\"type\":17},{\"gridX\":2084,\"gridY\":160,\"type\":17},{\"gridX\":2105,\"gridY\":160,\"type\":17},{\"gridX\":2127,\"gridY\":160,\"type\":17},{\"gridX\":2084,\"gridY\":139,\"type\":17},{\"gridX\":2106,\"gridY\":138,\"type\":17},{\"gridX\":2127,\"gridY\":138,\"type\":17},{\"gridX\":2105,\"gridY\":116,\"type\":17},{\"gridX\":2127,\"gridY\":116,\"type\":17},{\"gridX\":2105,\"gridY\":94,\"type\":17},{\"gridX\":2127,\"gridY\":94,\"type\":17},{\"gridX\":2105,\"gridY\":72,\"type\":17},{\"gridX\":2128,\"gridY\":72,\"type\":17},{\"gridX\":2149,\"gridY\":7,\"type\":17},{\"gridX\":2149,\"gridY\":29,\"type\":17},{\"gridX\":2149,\"gridY\":50,\"type\":17},{\"gridX\":2149,\"gridY\":71,\"type\":17},{\"gridX\":2149,\"gridY\":92,\"type\":17},{\"gridX\":2169,\"gridY\":7,\"type\":17},{\"gridX\":2169,\"gridY\":29,\"type\":17},{\"gridX\":2169,\"gridY\":50,\"type\":17},{\"gridX\":1506,\"gridY\":120,\"type\":17},{\"gridX\":1488,\"gridY\":107,\"type\":18},{\"gridX\":1509,\"gridY\":109,\"type\":18},{\"gridX\":1571,\"gridY\":0,\"type\":4},{\"gridX\":1552,\"gridY\":0,\"type\":5},{\"gridX\":1448,\"gridY\":0,\"type\":7},{\"gridX\":1448,\"gridY\":12,\"type\":7},{\"gridX\":1448,\"gridY\":24,\"type\":7},{\"gridX\":1448,\"gridY\":36,\"type\":7}],\"grounds\":[{\"gridX\":1976,\"gridY\":0,\"type\":0},{\"gridX\":1040,\"gridY\":0,\"type\":0},{\"gridX\":1515,\"gridY\":0,\"type\":0},{\"gridX\":1176,\"gridY\":0,\"type\":1},{\"gridX\":1523,\"gridY\":0,\"type\":2},{\"gridX\":1984,\"gridY\":0,\"type\":3},{\"gridX\":2112,\"gridY\":0,\"type\":3},{\"gridX\":1048,\"gridY\":0,\"type\":3},{\"gridX\":1208,\"gridY\":0,\"type\":4},{\"gridX\":1586,\"gridY\":0,\"type\":4},{\"gridX\":1443,\"gridY\":50,\"type\":13},{\"gridX" << "\":1406,\"gridY\":48,\"type\":15},{\"gridX\":1544,\"gridY\":48,\"type\":15},{\"gridX\":1552,\"gridY\":48,\"type\":16},{\"gridX\":1414,\"gridY\":48,\"type\":17},{\"gridX\":1478,\"gridY\":48,\"type\":19},{\"gridX\":1584,\"gridY\":48,\"type\":19},{\"gridX\":1620,\"gridY\":90,\"type\":21},{\"gridX\":1684,\"gridY\":90,\"type\":21},{\"gridX\":1204,\"gridY\":90,\"type\":23},{\"gridX\":1332,\"gridY\":90,\"type\":23},{\"gridX\":1492,\"gridY\":90,\"type\":23},{\"gridX\":891,\"gridY\":0,\"type\":25},{\"gridX\":776,\"gridY\":0,\"type\":25},{\"gridX\":951,\"gridY\":0,\"type\":25},{\"gridX\":1712,\"gridY\":0,\"type\":25},{\"gridX\":959,\"gridY\":0,\"type\":26},{\"gridX\":784,\"gridY\":0,\"type\":27},{\"gridX\":1848,\"gridY\":0,\"type\":28},{\"gridX\":1720,\"gridY\":0,\"type\":28},{\"gridX\":899,\"gridY\":0,\"type\":29},{\"gridX\":847,\"gridY\":0,\"type\":29},{\"gridX\":991,\"gridY\":0,\"type\":29},{\"gridX\":1976,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":1460,\"gridY\":88,\"type\":0},{\"gridX\":1652,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":1507,\"gridY\":80,\"type\":3},{\"gridX\":1224,\"gridY\":80,\"type\":3},{\"gridX\":1525,\"gridY\":32,\"type\":4}],\"foregroundObjects\":[{\"gridX\":0,\"gridY\":192,\"type\":48},{\"gridX\":16,\"gridY\":192,\"type\":48},{\"gridX\":32,\"gridY\":192,\"type\":48},{\"gridX\":48,\"gridY\":192,\"type\":48},{\"gridX\":80,\"gridY\":192,\"type\":48},{\"gridX\":64,\"gridY\":192,\"type\":48},{\"gridX\":96,\"gridY\":192,\"type\":48},{\"gridX\":112,\"gridY\":192,\"type\":49},{\"gridX\":2074,\"gridY\":7,\"type\":12},{\"gridX\":396,\"gridY\":172,\"type\":62},{\"gridX\":468,\"gridY\":168,\"type\":48},{\"gridX\":464,\"gridY\":168,\"type\":47},{\"gridX\":484,\"gridY\":168,\"type\":48},{\"gridX\":500,\"gridY\":168,\"type\":48},{\"gridX\":516,\"gridY\":168,\"type\":48},{\"gridX\":560,\"gridY\":204,\"type\":48},{\"gridX\":556,\"gridY\":204,\"type\":47},{\"gridX\":532,\"gridY\":168,\"type\":48},{\"gridX\":548,\"gridY\":168,\"type\":48},{\"gridX\":564,\"gridY\":168,\"type\":48},{\"gridX\":576,\"gridY\":204,\"type\":48},{\"gridX\":580,\"gridY\":168,\"type\":49},{\"gridX\":592,\"gridY\":204,\"type\":49},{\"gridX\":708,\"gridY\":128,\"type\":48},{\"gridX\":676,\"gridY\":128,\"type\":48},{\"gridX\":692,\"gridY\":128,\"type\":48},{\"gridX\":644,\"gridY\":128,\"type\":48},{\"gridX\":660,\"gridY\":128,\"type\":48},{\"gridX\":640,\"gridY\":128,\"type\":47},{\"gridX\":724,\"gridY\":128,\"type\":49},{\"gridX\":776,\"gridY\":156,\"type\":63},{\"gridX\":796,\"gridY\":156,\"type\":63},{\"gridX\":816,\"gridY\":156,\"type\":63},{\"gridX\":836,\"gridY\":156,\"type\":63},{\"gridX\":776,\"gridY\":176,\"type\":63},{\"gridX\":796,\"gridY\":176,\"type\":63},{\"gridX\":816,\"gridY\":176,\"type\":63},{\"gridX\":836,\"gridY\":176,\"type\":63},{\"gridX\":784,\"gridY\":196,\"type\":63},{\"gridX\":804,\"gridY\":196,\"type\":63},{\"gridX\":824,\"gridY\":196,\"type\":63},{\"gridX\":775,\"gridY\":189,\"type\":59},{\"gridX\":810,\"gridY\":208,\"type\":59},{\"gridX\":795,\"gridY\":213,\"type\":60},{\"gridX\":783,\"gridY\":210,\"type\":59},{\"gridX\":839,\"gridY\":189,\"type\":59},{\"gridX\":847,\"gridY\":155,\"type\":59},{\"gridX\":852,\"gridY\":166,\"type\":60},{\"gridX\":771,\"gridY\":157,\"type\":33},{\"gridX\":772,\"gridY\":107,\"type\":32},{\"gridX\":781,\"gridY\":123,\"type\":27},{\"gridX\":813,\"gridY\":123,\"type\":27},{\"gridX\":777,\"gridY\":101,\"type\":63},{\"gridX\":797,\"gridY\":101,\"type\":63},{\"gridX\":818,\"gridY\":101,\"type\":63},{\"gridX\":828,\"gridY\":101,\"type\":63},{\"gridX\":828,\"gridY\":96,\"type\":58},{\"gridX\":778,\"gridY\":96,\"type\":58},{\"gridX\":798,\"gridY\":96,\"type\":58},{\"gridX\":813,\"gridY\":96,\"type\":58},{\"gridX\":804,\"gridY\":236,\"type\":54},{\"gridX\":796,\"gridY\":236,\"type\":53},{\"gridX\":816,\"gridY\":236,\"type\":56},{\"gridX\":968,\"gridY\":96,\"type\":52},{\"gridX\":984,\"gridY\":144,\"type\":52},{\"gridX\":968,\"gridY\":144,\"type\":52},{\"gridX\":968,\"gridY\":112,\"type\":52},{\"gridX\":984,\"gridY\":112,\"type\":52},{\"gridX\":1000,\"gridY\":144,\"type\":52},{\"gridX\":1016,\"gridY\":144,\"type\":52},{\"gridX\":1016,\"gridY\":112,\"type\":52},{\"gridX\":1000,\"gridY\":112,\"type\":52},{\"gridX\":1016,\"gridY\":160,\"type\":52},{\"gridX\":1032,\"gridY\":176,\"type\":52},{\"gridX\":1032,\"gridY\":160,\"type\":52},{\"gridX\":1064,\"gridY\":112,\"type\":52},{\"gridX\":1048,\"gridY\":160,\"type\":52},{\"gridX\":1064,\"gridY\":128,\"type\":52},{\"gridX\":1064,\"gridY\":144,\"type\":52},{\"gridX\":1016,\"gridY\":176,\"type\":52},{\"gridX\":1048,\"gridY\":176,\"type\":52},{\"gridX\":1096,\"gridY\":144,\"type\":52},{\"gridX\":1096,\"gridY\":128,\"type\":52},{\"gridX\":1096,\"gridY\":112,\"type\":52},{\"gridX\":1096,\"gridY\":160,\"type\":52},{\"gridX\":1080,\"gridY\":176,\"type\":52},{\"gridX\":1080,\"gridY\":192,\"type\":52},{\"gridX\":1080,\"gridY\":208,\"type\":52},{\"gridX\":1080,\"gridY\":224,\"type\":52},{\"gridX\":1096,\"gridY\":176,\"type\":52},{\"gridX\":1096,\"gridY\":224,\"type\":52},{\"gridX\":1112,\"gridY\":112,\"type\":52},{\"gridX\":984,\"gridY\":110,\"type\":50},{\"gridX\":1000,\"gridY\":110,\"type\":50},{\"gridX\":1016,\"gridY\":110,\"type\":50},{\"gridX\":1064,\"gridY\":110,\"type\":50},{\"gridX\":1096,\"gridY\":110,\"type\":50},{\"gridX\":1128,\"gridY\":112,\"type\":52},{\"gridX\":1144,\"gridY\":112,\"type\":52},{\"gridX\":1176,\"gridY\":64,\"type\":52},{\"gridX\":1176,\"gridY\":48,\"type\":52},{\"gridX\":1048,\"gridY\":112,\"type\":52},{\"gridX\":1112,\"gridY\":110,\"type\":50},{\"gridX\":1128,\"gridY\":110,\"type\":50},{\"gridX\":1144,\"gridY\":110,\"type\":50},{\"gridX\":1144,\"gridY\":128,\"type\":52},{\"gridX\":1128,\"gridY\":144,\"type\":52},{\"gridX\":1112,\"gridY\":160,\"type\":52},{\"gridX\":1176,\"gridY\":128,\"type\":52},{\"gridX\":1068,\"gridY\":48,\"type\":50},{\"gridX\":1112,\"gridY\":48,\"type\":50},{\"gridX\":1128,\"gridY\":48,\"type\":50},{\"gridX\":1144,\"gridY\":48,\"type\":50},{\"gridX\":1160,\"gridY\":48,\"type\":50},{\"gridX\":1052,\"gridY\":48,\"type\":50},{\"gridX\":1036,\"gridY\":48,\"type\":50},{\"gridX\":1048,\"gridY\":128,\"type\":52},{\"gridX\":1192,\"gridY\":48,\"type\":52},{\"gridX\":1192,\"gridY\":64,\"type\":52},{\"gridX\":1192,\"gridY\":112,\"type\":52},{\"gridX\":964,\"gridY\":96,\"type\":55},{\"gridX\":956,\"gridY\":96,\"type\":53},{\"gridX\":988,\"gridY\":96,\"type\":56},{\"gridX\":1164,\"gridY\":52,\"type\":20},{\"gridX\":1060,\"gridY\":92,\"type\":50},{\"gridX\":1076,\"gridY\":92,\"type\":50},{\"gridX\":1092,\"gridY\":92,\"type\":50},{\"gridX\":1108,\"gridY\":92,\"type\":50},{\"gridX\":1124,\"gridY\":92,\"type\":50},{\"gridX\":964,\"gridY\":124,\"type\":50},{\"gridX\":1443,\"gridY\":96,\"type\":58},{\"gridX\":1524,\"gridY\":96,\"type\":58},{\"gridX\":1444,\"gridY\":107,\"type\":59},{\"gridX\":1442,\"gridY\":125,\"type\":60},{\"gridX\":1454,\"gridY\":119,\"type\":59},{\"gridX\":1471,\"gridY\":75,\"type\":41},{\"gridX\":1322,\"gridY\":23,\"type\":62},{\"gridX\":1192,\"gridY\":32,\"type\":52},{\"gridX\":1176,\"gridY\":32,\"type\":52},{\"gridX\":1506,\"gridY\":142,\"type\":61},{\"gridX\":1526,\"gridY\":120,\"type\":63},{\"gridX\":1526,\"gridY\":107,\"type\":59},{\"gridX\":1240,\"gridY\":96,\"type\":52},{\"gridX\":1444,\"gridY\":148,\"type\":22},{\"gridX\":1644,\"gridY\":0,\"type\":51},{\"gridX\":1646,\"gridY\":15,\"type\":20},{\"gridX\":1812,\"gridY\":95,\"type\":35},{\"gridX\":1884,\"gridY\":95,\"type\":35},{\"gridX\":1736,\"gridY\":95,\"type\":35},{\"gridX\":1960,\"gridY\":95,\"type\":35},{\"gridX\":2065,\"gridY\":127,\"type\":63},{\"gridX\":2036,\"gridY\":185,\"type\":63},{\"gridX\":2063,\"gridY\":106,\"type\":63},{\"gridX\":2043,\"gridY\":156,\"type\":63},{\"gridX\":2043,\"gridY\":135,\"type\":63},{\"gridX\":2044,\"gridY\":113,\"type\":63},{\"gridX\":2054,\"gridY\":86,\"type\":63},{\"gridX\":2045,\"gridY\":101,\"type\":59},{\"gridX\":2049,\"gridY\":166,\"type\":59},{\"gridX\":2065,\"gridY\":142,\"type\":59},{\"gridX\":2036,\"gridY\":174,\"t" << "ype\":59},{\"gridX\":2036,\"gridY\":205,\"type\":63},{\"gridX\":2085,\"gridY\":122,\"type\":63},{\"gridX\":2146,\"gridY\":189,\"type\":63},{\"gridX\":2063,\"gridY\":154,\"type\":63},{\"gridX\":2054,\"gridY\":74,\"type\":59},{\"gridX\":2064,\"gridY\":61,\"type\":59},{\"gridX\":2071,\"gridY\":72,\"type\":59},{\"gridX\":2072,\"gridY\":87,\"type\":63},{\"gridX\":2057,\"gridY\":180,\"type\":63},{\"gridX\":2136,\"gridY\":210,\"type\":63},{\"gridX\":2084,\"gridY\":104,\"type\":63},{\"gridX\":2087,\"gridY\":52,\"type\":59},{\"gridX\":2077,\"gridY\":61,\"type\":63},{\"gridX\":2095,\"gridY\":64,\"type\":59},{\"gridX\":2089,\"gridY\":81,\"type\":63},{\"gridX\":1176,\"gridY\":80,\"type\":51},{\"gridX\":1192,\"gridY\":80,\"type\":51},{\"gridX\":1208,\"gridY\":112,\"type\":52},{\"gridX\":1224,\"gridY\":112,\"type\":52},{\"gridX\":1240,\"gridY\":112,\"type\":52},{\"gridX\":1207,\"gridY\":68,\"type\":50},{\"gridX\":1223,\"gridY\":68,\"type\":50},{\"gridX\":2028,\"gridY\":218,\"type\":60},{\"gridX\":2053,\"gridY\":218,\"type\":60},{\"gridX\":2044,\"gridY\":215,\"type\":59},{\"gridX\":2054,\"gridY\":198,\"type\":59},{\"gridX\":2065,\"gridY\":168,\"type\":59},{\"gridX\":2077,\"gridY\":131,\"type\":59},{\"gridX\":2129,\"gridY\":232,\"type\":63},{\"gridX\":2128,\"gridY\":218,\"type\":59},{\"gridX\":2135,\"gridY\":197,\"type\":59},{\"gridX\":2149,\"gridY\":167,\"type\":63},{\"gridX\":2149,\"gridY\":146,\"type\":63},{\"gridX\":2149,\"gridY\":124,\"type\":63},{\"gridX\":2148,\"gridY\":102,\"type\":63},{\"gridX\":2152,\"gridY\":90,\"type\":59},{\"gridX\":2158,\"gridY\":79,\"type\":59},{\"gridX\":2169,\"gridY\":68,\"type\":63},{\"gridX\":2169,\"gridY\":55,\"type\":59},{\"gridX\":1465,\"gridY\":142,\"type\":61},{\"gridX\":1484,\"gridY\":142,\"type\":61},{\"gridX\":1526,\"gridY\":142,\"type\":61},{\"gridX\":1547,\"gridY\":147,\"type\":22},{\"gridX\":1239,\"gridY\":68,\"type\":50},{\"gridX\":1160,\"gridY\":144,\"type\":50},{\"gridX\":1448,\"gridY\":34,\"type\":34},{\"gridX\":1448,\"gridY\":17,\"type\":34},{\"gridX\":1403,\"gridY\":56,\"type\":32},{\"gridX\":1541,\"gridY\":57,\"type\":32}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":2039,\"gridY\":7}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":152,\"gridY\":204,\"type\":3},{\"gridX\":184,\"gridY\":180,\"type\":3},{\"gridX\":200,\"gridY\":224,\"type\":3},{\"gridX\":232,\"gridY\":180,\"type\":3},{\"gridX\":248,\"gridY\":224,\"type\":3},{\"gridX\":300,\"gridY\":188,\"type\":3},{\"gridX\":891,\"gridY\":96,\"type\":6},{\"gridX\":1424,\"gridY\":96,\"type\":6},{\"gridX\":1369,\"gridY\":96,\"type\":10},{\"gridX\":1302,\"gridY\":96,\"type\":8},{\"gridX\":1464,\"gridY\":148,\"type\":5},{\"gridX\":1500,\"gridY\":148,\"type\":5},{\"gridX\":1772,\"gridY\":96,\"type\":5},{\"gridX\":1848,\"gridY\":96,\"type\":5},{\"gridX\":1924,\"gridY\":96,\"type\":5},{\"gridX\":518,\"gridY\":173,\"type\":5}],\"collectibles\":[{\"gridX\":152,\"gridY\":226,\"type\":0},{\"gridX\":68,\"gridY\":204,\"type\":0},{\"gridX\":84,\"gridY\":204,\"type\":0},{\"gridX\":76,\"gridY\":204,\"type\":0},{\"gridX\":399,\"gridY\":185,\"type\":0},{\"gridX\":406,\"gridY\":185,\"type\":0},{\"gridX\":404,\"gridY\":191,\"type\":0},{\"gridX\":480,\"gridY\":180,\"type\":0},{\"gridX\":488,\"gridY\":180,\"type\":0},{\"gridX\":496,\"gridY\":180,\"type\":0},{\"gridX\":504,\"gridY\":180,\"type\":0},{\"gridX\":568,\"gridY\":212,\"type\":0},{\"gridX\":576,\"gridY\":212,\"type\":0},{\"gridX\":584,\"gridY\":212,\"type\":0},{\"gridX\":592,\"gridY\":212,\"type\":0},{\"gridX\":600,\"gridY\":208,\"type\":0},{\"gridX\":608,\"gridY\":204,\"type\":0},{\"gridX\":616,\"gridY\":196,\"type\":0},{\"gridX\":624,\"gridY\":188,\"type\":0},{\"gridX\":786,\"gridY\":137,\"type\":0},{\"gridX\":795,\"gridY\":137,\"type\":0},{\"gridX\":806,\"gridY\":136,\"type\":0},{\"gridX\":818,\"gridY\":136,\"type\":0},{\"gridX\":830,\"gridY\":135,\"type\":0},{\"gridX\":840,\"gridY\":134,\"type\":0},{\"gridX\":851,\"gridY\":133,\"type\":0},{\"gridX\":896,\"gridY\":112,\"type\":0},{\"gridX\":1006,\"gridY\":132,\"type\":0},{\"gridX\":1015,\"gridY\":132,\"type\":0},{\"gridX\":1024,\"gridY\":132,\"type\":0},{\"gridX\":1035,\"gridY\":132,\"type\":0},{\"gridX\":1036,\"gridY\":124,\"type\":0},{\"gridX\":1036,\"gridY\":116,\"type\":0},{\"gridX\":1584,\"gridY\":20,\"type\":1},{\"gridX\":300,\"gridY\":202,\"type\":0},{\"gridX\":1481,\"gridY\":105,\"type\":1},{\"gridX\":1156,\"gridY\":12,\"type\":0},{\"gridX\":1172,\"gridY\":12,\"type\":0},{\"gridX\":1180,\"gridY\":12,\"type\":0},{\"gridX\":1164,\"gridY\":12,\"type\":0},{\"gridX\":1188,\"gridY\":12,\"type\":0},{\"gridX\":1196,\"gridY\":12,\"type\":0},{\"gridX\":1204,\"gridY\":12,\"type\":0},{\"gridX\":1210,\"gridY\":20,\"type\":0},{\"gridX\":1217,\"gridY\":30,\"type\":0},{\"gridX\":1224,\"gridY\":39,\"type\":0},{\"gridX\":1416,\"gridY\":80,\"type\":0},{\"gridX\":1408,\"gridY\":80,\"type\":0},{\"gridX\":1424,\"gridY\":80,\"type\":0},{\"gridX\":1432,\"gridY\":80,\"type\":0},{\"gridX\":1440,\"gridY\":80,\"type\":0},{\"gridX\":1448,\"gridY\":80,\"type\":0},{\"gridX\":1456,\"gridY\":80,\"type\":0},{\"gridX\":1464,\"gridY\":80,\"type\":0},{\"gridX\":1472,\"gridY\":80,\"type\":0},{\"gridX\":1548,\"gridY\":80,\"type\":0},{\"gridX\":1556,\"gridY\":80,\"type\":0},{\"gridX\":1580,\"gridY\":80,\"type\":0},{\"gridX\":1591,\"gridY\":79,\"type\":0},{\"gridX\":1601,\"gridY\":77,\"type\":0},{\"gridX\":1564,\"gridY\":80,\"type\":0},{\"gridX\":1572,\"gridY\":80,\"type\":0},{\"gridX\":988,\"gridY\":132,\"type\":0},{\"gridX\":996,\"gridY\":132,\"type\":0},{\"gridX\":1036,\"gridY\":108,\"type\":0},{\"gridX\":1036,\"gridY\":100,\"type\":0},{\"gridX\":1036,\"gridY\":92,\"type\":0},{\"gridX\":1036,\"gridY\":84,\"type\":0},{\"gridX\":1036,\"gridY\":76,\"type\":0},{\"gridX\":1036,\"gridY\":68,\"type\":0},{\"gridX\":1036,\"gridY\":60,\"type\":0},{\"gridX\":1044,\"gridY\":56,\"type\":0},{\"gridX\":1052,\"gridY\":56,\"type\":0},{\"gridX\":1628,\"gridY\":46,\"type\":0},{\"gridX\":1672,\"gridY\":96,\"type\":1},{\"gridX\":1623,\"gridY\":56,\"type\":0},{\"gridX\":1617,\"gridY\":65,\"type\":0},{\"gridX\":1610,\"gridY\":70,\"type\":0},{\"gridX\":1647,\"gridY\":23,\"type\":0},{\"gridX\":1648,\"gridY\":32,\"type\":0},{\"gridX\":1650,\"gridY\":40,\"type\":0},{\"gridX\":1652,\"gridY\":48,\"type\":0},{\"gridX\":1654,\"gridY\":56,\"type\":0},{\"gridX\":1656,\"gridY\":64,\"type\":0},{\"gridX\":1660,\"gridY\":72,\"type\":0},{\"gridX\":1664,\"gridY\":80,\"type\":0},{\"gridX\":1676,\"gridY\":108,\"type\":0},{\"gridX\":1680,\"gridY\":116,\"type\":0},{\"gridX\":1688,\"gridY\":124,\"type\":0},{\"gridX\":1696,\"gridY\":132,\"type\":0},{\"gridX\":1704,\"gridY\":132,\"type\":0},{\"gridX\":1924,\"gridY\":144,\"type\":0},{\"gridX\":1924,\"gridY\":136,\"type\":0},{\"gridX\":1848,\"gridY\":136,\"type\":0},{\"gridX\":1772,\"gridY\":112,\"type\":0},{\"gridX\":1772,\"gridY\":120,\"type\":0},{\"gridX\":1848,\"gridY\":144,\"type\":0},{\"gridX\":1848,\"gridY\":120,\"type\":0},{\"gridX\":1924,\"gridY\":160,\"type\":0},{\"gridX\":1924,\"gridY\":152,\"type\":0},{\"gridX\":1924,\"gridY\":128,\"type\":0},{\"gridX\":1848,\"gridY\":128,\"type\":0},{\"gridX\":1772,\"gridY\":128,\"type\":0},{\"gridX\":1229,\"gridY\":100,\"type\":3},{\"gridX\":2079,\"gridY\":244,\"type\":3},{\"gridX\":1536,\"gridY\":12,\"type\":0},{\"gridX\":1560,\"gridY\":12,\"type\":0},{\"gridX\":1528,\"gridY\":12,\"type\":0},{\"gridX\":1528,\"gridY\":28,\"type\":0},{\"gridX\":1552,\"gridY\":12,\"type\":0},{\"gridX\":1544,\"gridY\":12,\"type\":0},{\"gridX\":1568,\"gridY\":12,\"type\":0},{\"gridX\":1576,\"gridY\":12,\"type\":0},{\"gridX\":1528,\"gridY\":20,\"type\":0}],\"jons\":[{\"gridX\":16,\"gridY\":197}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":968,\"gridY\":112,\"type\":4},{\"gridX\":1000,\"gridY\":112,\"type\":4},{\"gridX\":1016,\"gridY\":112,\"type\":6},{\"gridX\":984,\"gridY\":112,\"type\":6},{\"gridX\":968,\"gridY\":144,\"type\":4},{\"gridX\":1000,\"gridY\":144,\"type\":4},{\"gridX\":984,\"gridY\":14" << "4,\"type\":6},{\"gridX\":1016,\"gridY\":144,\"type\":6},{\"gridX\":1064,\"gridY\":112,\"type\":4},{\"gridX\":1064,\"gridY\":128,\"type\":4},{\"gridX\":1064,\"gridY\":144,\"type\":4},{\"gridX\":968,\"gridY\":160,\"type\":10},{\"gridX\":984,\"gridY\":160,\"type\":11},{\"gridX\":1000,\"gridY\":160,\"type\":10},{\"gridX\":1016,\"gridY\":192,\"type\":10},{\"gridX\":1016,\"gridY\":160,\"type\":4},{\"gridX\":1032,\"gridY\":160,\"type\":4},{\"gridX\":1048,\"gridY\":160,\"type\":4},{\"gridX\":1016,\"gridY\":176,\"type\":6},{\"gridX\":1032,\"gridY\":176,\"type\":6},{\"gridX\":1048,\"gridY\":176,\"type\":6},{\"gridX\":1032,\"gridY\":192,\"type\":10},{\"gridX\":1048,\"gridY\":192,\"type\":11},{\"gridX\":1080,\"gridY\":240,\"type\":8},{\"gridX\":1096,\"gridY\":240,\"type\":9},{\"gridX\":1096,\"gridY\":224,\"type\":6},{\"gridX\":1080,\"gridY\":224,\"type\":6},{\"gridX\":1080,\"gridY\":208,\"type\":4},{\"gridX\":1096,\"gridY\":208,\"type\":4},{\"gridX\":1080,\"gridY\":192,\"type\":6},{\"gridX\":1096,\"gridY\":192,\"type\":6},{\"gridX\":1080,\"gridY\":176,\"type\":4},{\"gridX\":1096,\"gridY\":176,\"type\":4},{\"gridX\":1032,\"gridY\":144,\"type\":4},{\"gridX\":1048,\"gridY\":144,\"type\":6},{\"gridX\":1096,\"gridY\":128,\"type\":4},{\"gridX\":1096,\"gridY\":144,\"type\":4},{\"gridX\":1096,\"gridY\":112,\"type\":4},{\"gridX\":1096,\"gridY\":160,\"type\":4},{\"gridX\":1112,\"gridY\":144,\"type\":6},{\"gridX\":1128,\"gridY\":144,\"type\":4},{\"gridX\":1112,\"gridY\":128,\"type\":6},{\"gridX\":1112,\"gridY\":112,\"type\":6},{\"gridX\":1144,\"gridY\":128,\"type\":6},{\"gridX\":1144,\"gridY\":112,\"type\":6},{\"gridX\":1176,\"gridY\":112,\"type\":6},{\"gridX\":1128,\"gridY\":128,\"type\":4},{\"gridX\":1128,\"gridY\":112,\"type\":4},{\"gridX\":1112,\"gridY\":160,\"type\":6},{\"gridX\":1176,\"gridY\":128,\"type\":6},{\"gridX\":1144,\"gridY\":144,\"type\":10},{\"gridX\":1176,\"gridY\":144,\"type\":11},{\"gridX\":1128,\"gridY\":160,\"type\":9},{\"gridX\":1112,\"gridY\":176,\"type\":9},{\"gridX\":1192,\"gridY\":96,\"type\":4},{\"gridX\":1224,\"gridY\":112,\"type\":6},{\"gridX\":1176,\"gridY\":96,\"type\":4},{\"gridX\":1048,\"gridY\":112,\"type\":4},{\"gridX\":1048,\"gridY\":128,\"type\":4},{\"gridX\":1192,\"gridY\":64,\"type\":4},{\"gridX\":1176,\"gridY\":64,\"type\":4},{\"gridX\":1192,\"gridY\":80,\"type\":6},{\"gridX\":1176,\"gridY\":80,\"type\":6},{\"gridX\":1192,\"gridY\":48,\"type\":6},{\"gridX\":1176,\"gridY\":48,\"type\":6},{\"gridX\":1208,\"gridY\":96,\"type\":5},{\"gridX\":1192,\"gridY\":112,\"type\":6},{\"gridX\":1208,\"gridY\":112,\"type\":4},{\"gridX\":1224,\"gridY\":128,\"type\":10},{\"gridX\":1208,\"gridY\":128,\"type\":10},{\"gridX\":1192,\"gridY\":128,\"type\":10},{\"gridX\":1240,\"gridY\":128,\"type\":11},{\"gridX\":1240,\"gridY\":112,\"type\":4},{\"gridX\":1240,\"gridY\":96,\"type\":7},{\"gridX\":1192,\"gridY\":32,\"type\":4},{\"gridX\":1176,\"gridY\":32,\"type\":4},{\"gridX\":1378,\"gridY\":85,\"type\":0},{\"gridX\":1271,\"gridY\":92,\"type\":1},{\"gridX\":1334,\"gridY\":92,\"type\":2},{\"gridX\":1418,\"gridY\":92,\"type\":3},{\"gridX\":1362,\"gridY\":92,\"type\":1},{\"gridX\":1242,\"gridY\":92,\"type\":3}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level17(text.c_str());
}

void Chapter1Level17::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level17 * Chapter1Level17::getInstance()
{
    return s_pInstance;
}

Chapter1Level17::Chapter1Level17(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 18 ///

Chapter1Level18 * Chapter1Level18::s_pInstance = nullptr;

void Chapter1Level18::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":18,\"midgrounds\":[{\"gridX\":1780,\"gridY\":96,\"type\":2},{\"gridX\":540,\"gridY\":96,\"type\":2},{\"gridX\":684,\"gridY\":96,\"type\":2},{\"gridX\":1920,\"gridY\":96,\"type\":2},{\"gridX\":344,\"gridY\":96,\"type\":7},{\"gridX\":344,\"gridY\":180,\"type\":7},{\"gridX\":344,\"gridY\":168,\"type\":7},{\"gridX\":344,\"gridY\":156,\"type\":7},{\"gridX\":344,\"gridY\":144,\"type\":7},{\"gridX\":344,\"gridY\":132,\"type\":7},{\"gridX\":344,\"gridY\":120,\"type\":7},{\"gridX\":344,\"gridY\":108,\"type\":7},{\"gridX\":344,\"gridY\":192,\"type\":7},{\"gridX\":400,\"gridY\":96,\"type\":7},{\"gridX\":400,\"gridY\":180,\"type\":7},{\"gridX\":400,\"gridY\":168,\"type\":7},{\"gridX\":400,\"gridY\":156,\"type\":7},{\"gridX\":400,\"gridY\":144,\"type\":7},{\"gridX\":400,\"gridY\":132,\"type\":7},{\"gridX\":400,\"gridY\":120,\"type\":7},{\"gridX\":400,\"gridY\":108,\"type\":7},{\"gridX\":400,\"gridY\":192,\"type\":7},{\"gridX\":664,\"gridY\":96,\"type\":15},{\"gridX\":0,\"gridY\":7,\"type\":17},{\"gridX\":16,\"gridY\":7,\"type\":17},{\"gridX\":24,\"gridY\":18,\"type\":18},{\"gridX\":35,\"gridY\":29,\"type\":18},{\"gridX\":43,\"gridY\":42,\"type\":18},{\"gridX\":13,\"gridY\":17,\"type\":18},{\"gridX\":616,\"gridY\":0,\"type\":5},{\"gridX\":936,\"gridY\":84,\"type\":17},{\"gridX\":956,\"gridY\":84,\"type\":17},{\"gridX\":974,\"gridY\":84,\"type\":17},{\"gridX\":996,\"gridY\":84,\"type\":17},{\"gridX\":1008,\"gridY\":84,\"type\":17},{\"gridX\":932,\"gridY\":104,\"type\":17},{\"gridX\":952,\"gridY\":104,\"type\":17},{\"gridX\":970,\"gridY\":127,\"type\":17},{\"gridX\":992,\"gridY\":104,\"type\":17},{\"gridX\":1012,\"gridY\":104,\"type\":17},{\"gridX\":932,\"gridY\":124,\"type\":17},{\"gridX\":952,\"gridY\":124,\"type\":17},{\"gridX\":932,\"gridY\":164,\"type\":17},{\"gridX\":972,\"gridY\":144,\"type\":17},{\"gridX\":932,\"gridY\":144,\"type\":17},{\"gridX\":972,\"gridY\":164,\"type\":17},{\"gridX\":992,\"gridY\":144,\"type\":17},{\"gridX\":952,\"gridY\":164,\"type\":17},{\"gridX\":952,\"gridY\":144,\"type\":17},{\"gridX\":1012,\"gridY\":124,\"type\":17},{\"gridX\":1012,\"gridY\":144,\"type\":17},{\"gridX\":992,\"gridY\":124,\"type\":17},{\"gridX\":972,\"gridY\":106,\"type\":17},{\"gridX\":1015,\"gridY\":177,\"type\":17},{\"gridX\":992,\"gridY\":164,\"type\":17},{\"gridX\":1012,\"gridY\":164,\"type\":17},{\"gridX\":932,\"gridY\":69,\"type\":18},{\"gridX\":941,\"gridY\":61,\"type\":18},{\"gridX\":948,\"gridY\":73,\"type\":18},{\"gridX\":1008,\"gridY\":74,\"type\":18},{\"gridX\":1021,\"gridY\":64,\"type\":18},{\"gridX\":1023,\"gridY\":82,\"type\":18},{\"gridX\":1019,\"gridY\":213,\"type\":18},{\"gridX\":1000,\"gridY\":229,\"type\":18},{\"gridX\":1035,\"gridY\":200,\"type\":18},{\"gridX\":1005,\"gridY\":216,\"type\":18},{\"gridX\":1023,\"gridY\":231,\"type\":18},{\"gridX\":1011,\"gridY\":229,\"type\":18},{\"gridX\":1027,\"gridY\":198,\"type\":18},{\"gridX\":1033,\"gridY\":231,\"type\":18},{\"gridX\":1032,\"gridY\":166,\"type\":17},{\"gridX\":1034,\"gridY\":99,\"type\":17},{\"gridX\":1032,\"gridY\":121,\"type\":17},{\"gridX\":928,\"gridY\":185,\"type\":17},{\"gridX\":949,\"gridY\":185,\"type\":17},{\"gridX\":971,\"gridY\":186,\"type\":17},{\"gridX\":1013,\"gridY\":198,\"type\":17},{\"gridX\":993,\"gridY\":185,\"type\":17},{\"gridX\":934,\"gridY\":205,\"type\":17},{\"gridX\":943,\"gridY\":226,\"type\":17},{\"gridX\":964,\"gridY\":226,\"type\":17},{\"gridX\":954,\"gridY\":206,\"type\":17},{\"gridX\":975,\"gridY\":207,\"type\":17},{\"gridX\":985,\"gridY\":227,\"type\":17},{\"gridX\":996,\"gridY\":205,\"type\":17},{\"gridX\":544,\"gridY\":96,\"type\":7},{\"gridX\":544,\"gridY\":108,\"type\":7},{\"gridX\":560,\"gridY\":144,\"type\":7},{\"gridX\":560,\"gridY\":132,\"type\":7},{\"gridX\":560,\"gridY\":120,\"type\":7},{\"gridX\":560,\"gridY\":108,\"type\":7},{\"gridX\":560,\"gridY\":96,\"type\":7},{\"gridX\":636,\"gridY\":140,\"type\":7},{\"gridX\":636,\"gridY\":128,\"type\":7},{\"gridX\":636,\"gridY\":116,\"type\":7},{\"gridX\":636,\"gridY\":104,\"type\":7},{\"gridX\":636,\"gridY\":92,\"type\":7},{\"gridX\":620,\"gridY\":92,\"type\":7},{\"gridX\":620,\"gridY\":104,\"type\":7},{\"gridX\":716,\"gridY\":96,\"type\":7},{\"gridX\":716,\"gridY\":108,\"type\":7},{\"gridX\":716,\"gridY\":120,\"type\":7},{\"gridX\":716,\"gridY\":132,\"type\":7},{\"gridX\":732,\"gridY\":92,\"type\":7},{\"gridX\":732,\"gridY\":128,\"type\":7},{\"gridX\":732,\"gridY\":116,\"type\":7},{\"gridX\":732,\"gridY\":104,\"type\":7},{\"gridX\":732,\"gridY\":140,\"type\":7},{\"gridX\":768,\"gridY\":116,\"type\":7},{\"gridX\":768,\"gridY\":104,\"type\":7},{\"gridX\":768,\"gridY\":92,\"type\":7},{\"gridX\":784,\"gridY\":140,\"type\":7},{\"gridX\":784,\"gridY\":92,\"type\":7},{\"gridX\":784,\"gridY\":104,\"type\":7},{\"gridX\":784,\"gridY\":116,\"type\":7},{\"gridX\":784,\"gridY\":128,\"type\":7},{\"gridX\":832,\"gridY\":96,\"type\":7},{\"gridX\":832,\"gridY\":108,\"type\":7},{\"gridX\":832,\"gridY\":120,\"type\":7},{\"gridX\":1598,\"gridY\":96,\"type\":6},{\"gridX\":1655,\"gridY\":96,\"type\":11},{\"gridX\":1703,\"gridY\":96,\"type\":13},{\"gridX\":1738,\"gridY\":96,\"type\":14},{\"gridX\":1074,\"gridY\":96,\"type\":11},{\"gridX\":1115,\"gridY\":96,\"type\":13},{\"gridX\":1148,\"gridY\":96,\"type\":14},{\"gridX\":1156,\"gridY\":96,\"type\":15},{\"gridX\":1096,\"gridY\":96,\"type\":16},{\"gridX\":703,\"gridY\":96,\"type\":14},{\"gridX\":591,\"gridY\":96,\"type\":13},{\"gridX\":428,\"gridY\":96,\"type\":14},{\"gridX\":455,\"gridY\":96,\"type\":13},{\"gridX\":505,\"gridY\":96,\"type\":16},{\"gridX\":1356,\"gridY\":0,\"type\":4},{\"gridX\":1139,\"gridY\":0,\"type\":5},{\"gridX\":1426,\"gridY\":0,\"type\":5},{\"gridX\":1485,\"gridY\":0,\"type\":4},{\"gridX\":1550,\"gridY\":0,\"type\":5},{\"gridX\":1384,\"gridY\":0,\"type\":3},{\"gridX\":1463,\"gridY\":0,\"type\":3},{\"gridX\":1207,\"gridY\":149,\"type\":17},{\"gridX\":1210,\"gridY\":129,\"type\":17},{\"gridX\":1200,\"gridY\":125,\"type\":18},{\"gridX\":1250,\"gridY\":184,\"type\":17},{\"gridX\":1234,\"gridY\":192,\"type\":18},{\"gridX\":1262,\"gridY\":193,\"type\":18},{\"gridX\":1273,\"gridY\":201,\"type\":18},{\"gridX\":1285,\"gridY\":206,\"type\":17},{\"gridX\":1232,\"gridY\":176,\"type\":18},{\"gridX\":1217,\"gridY\":178,\"type\":17},{\"gridX\":1218,\"gridY\":160,\"type\":18},{\"gridX\":1226,\"gridY\":120,\"type\":18},{\"gridX\":1239,\"gridY\":115,\"type\":17},{\"gridX\":1260,\"gridY\":115,\"type\":17},{\"gridX\":1280,\"gridY\":115,\"type\":17},{\"gridX\":1301,\"gridY\":115,\"type\":17},{\"gridX\":1320,\"gridY\":115,\"type\":17},{\"gridX\":1261,\"gridY\":151,\"type\":18},{\"gridX\":1255,\"gridY\":138,\"type\":18},{\"gridX\":1258,\"gridY\":126,\"type\":17},{\"gridX\":1272,\"gridY\":135,\"type\":17},{\"gridX\":1272,\"gridY\":145,\"type\":18},{\"gridX\":1286,\"gridY\":125,\"type\":18},{\"gridX\":1311,\"gridY\":126,\"type\":18},{\"gridX\":1323,\"gridY\":133,\"type\":18},{\"gridX\":1262,\"gridY\":166,\"type\":17},{\"gridX\":1266,\"gridY\":174,\"type\":18},{\"gridX\":1305,\"gridY\":197,\"type\":17},{\"gridX\":1318,\"gridY\":184,\"type\":17},{\"gridX\":1334,\"gridY\":164,\"type\":17},{\"gridX\":1319,\"gridY\":170,\"type\":18},{\"gridX\":1352,\"gridY\":162,\"type\":18},{\"gridX\":1298,\"gridY\":186,\"type\":18},{\"gridX\":1324,\"gridY\":198,\"type\":18},{\"gridX\":1333,\"gridY\":211,\"type\":17},{\"gridX\":1333,\"gridY\":192,\"type\":18},{\"gridX\":1352,\"gridY\":175,\"type\":18},{\"gridX\":1346,\"gridY\":215,\"type\":18},{\"gridX\":1347,\"gridY\":62,\"type\":17},{\"gridX\":1334,\"gridY\":71,\"type\":18},{\"gridX\":1358,\"gridY\":55,\"type\":17},{\"gridX\":1346,\"gridY\":46,\"type\":18},{\"gridX\":1332,\"gridY\":88,\"type\":18},{\"gridX\":1379,\"gridY\":55,\"type\":17},{\"gridX\":1400,\"gridY\":55,\"type\":17},{\"gridX\":1420,\"gridY\":55,\"type\":17},{\"gridX\":1417,\"gridY\":75,\"type\":17},{\"gridX\":1438,\"gridY\":75,\"type\":17},{\"gridX\":1460,\"gridY\":75,\"type\":17},{\"gridX\":1481,\"gridY\":75,\"type\":17},{\"gridX\":1503,\"gridY\":75,\"type\":1" << "7},{\"gridX\":1524,\"gridY\":75,\"type\":17},{\"gridX\":1545,\"gridY\":75,\"type\":17},{\"gridX\":1413,\"gridY\":161,\"type\":18},{\"gridX\":1396,\"gridY\":75,\"type\":17},{\"gridX\":1348,\"gridY\":83,\"type\":17},{\"gridX\":1376,\"gridY\":74,\"type\":17},{\"gridX\":1387,\"gridY\":93,\"type\":17},{\"gridX\":1361,\"gridY\":75,\"type\":18},{\"gridX\":1404,\"gridY\":176,\"type\":17},{\"gridX\":1427,\"gridY\":157,\"type\":18},{\"gridX\":1424,\"gridY\":142,\"type\":17},{\"gridX\":1444,\"gridY\":144,\"type\":17},{\"gridX\":1464,\"gridY\":144,\"type\":17},{\"gridX\":1483,\"gridY\":146,\"type\":17},{\"gridX\":1456,\"gridY\":152,\"type\":17},{\"gridX\":1466,\"gridY\":161,\"type\":18},{\"gridX\":1424,\"gridY\":94,\"type\":17},{\"gridX\":1445,\"gridY\":93,\"type\":17},{\"gridX\":1466,\"gridY\":94,\"type\":17},{\"gridX\":1486,\"gridY\":94,\"type\":17},{\"gridX\":1507,\"gridY\":94,\"type\":17},{\"gridX\":1471,\"gridY\":112,\"type\":17},{\"gridX\":1491,\"gridY\":114,\"type\":17},{\"gridX\":1502,\"gridY\":112,\"type\":18},{\"gridX\":1525,\"gridY\":87,\"type\":18},{\"gridX\":1539,\"gridY\":86,\"type\":18},{\"gridX\":1551,\"gridY\":96,\"type\":18},{\"gridX\":1347,\"gridY\":104,\"type\":17},{\"gridX\":1354,\"gridY\":125,\"type\":17},{\"gridX\":1368,\"gridY\":94,\"type\":17},{\"gridX\":1364,\"gridY\":111,\"type\":17},{\"gridX\":1386,\"gridY\":108,\"type\":18},{\"gridX\":1363,\"gridY\":143,\"type\":18},{\"gridX\":1375,\"gridY\":153,\"type\":18},{\"gridX\":1378,\"gridY\":141,\"type\":18},{\"gridX\":1393,\"gridY\":135,\"type\":18},{\"gridX\":1413,\"gridY\":135,\"type\":17},{\"gridX\":1388,\"gridY\":163,\"type\":18},{\"gridX\":1391,\"gridY\":123,\"type\":18},{\"gridX\":1322,\"gridY\":148,\"type\":18},{\"gridX\":1239,\"gridY\":129,\"type\":18}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":0},{\"gridX\":512,\"gridY\":0,\"type\":0},{\"gridX\":424,\"gridY\":0,\"type\":1},{\"gridX\":904,\"gridY\":0,\"type\":2},{\"gridX\":8,\"gridY\":0,\"type\":3},{\"gridX\":136,\"gridY\":0,\"type\":3},{\"gridX\":520,\"gridY\":0,\"type\":3},{\"gridX\":648,\"gridY\":0,\"type\":3},{\"gridX\":776,\"gridY\":0,\"type\":3},{\"gridX\":968,\"gridY\":0,\"type\":4},{\"gridX\":456,\"gridY\":0,\"type\":4},{\"gridX\":1187,\"gridY\":20,\"type\":5},{\"gridX\":1426,\"gridY\":20,\"type\":5},{\"gridX\":1195,\"gridY\":20,\"type\":6},{\"gridX\":1227,\"gridY\":20,\"type\":9},{\"gridX\":1434,\"gridY\":20,\"type\":9},{\"gridX\":1339,\"gridY\":50,\"type\":10},{\"gridX\":1113,\"gridY\":50,\"type\":10},{\"gridX\":1121,\"gridY\":50,\"type\":11},{\"gridX\":1347,\"gridY\":50,\"type\":11},{\"gridX\":1379,\"gridY\":50,\"type\":12},{\"gridX\":1153,\"gridY\":50,\"type\":14},{\"gridX\":1435,\"gridY\":48,\"type\":15},{\"gridX\":1443,\"gridY\":48,\"type\":17},{\"gridX\":1507,\"gridY\":48,\"type\":17},{\"gridX\":1088,\"gridY\":90,\"type\":20},{\"gridX\":1160,\"gridY\":90,\"type\":21},{\"gridX\":424,\"gridY\":90,\"type\":21},{\"gridX\":1096,\"gridY\":90,\"type\":22},{\"gridX\":456,\"gridY\":90,\"type\":22},{\"gridX\":648,\"gridY\":90,\"type\":23},{\"gridX\":776,\"gridY\":90,\"type\":23},{\"gridX\":520,\"gridY\":90,\"type\":23},{\"gridX\":904,\"gridY\":90,\"type\":24},{\"gridX\":1564,\"gridY\":0,\"type\":25},{\"gridX\":260,\"gridY\":0,\"type\":25},{\"gridX\":332,\"gridY\":0,\"type\":26},{\"gridX\":268,\"gridY\":0,\"type\":27},{\"gridX\":364,\"gridY\":0,\"type\":27},{\"gridX\":1572,\"gridY\":0,\"type\":27},{\"gridX\":1764,\"gridY\":0,\"type\":28},{\"gridX\":1636,\"gridY\":0,\"type\":28},{\"gridX\":1892,\"gridY\":0,\"type\":28},{\"gridX\":428,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":1555,\"gridY\":16,\"type\":34},{\"gridX\":1625,\"gridY\":0,\"type\":30},{\"gridX\":1579,\"gridY\":16,\"type\":35},{\"gridX\":1611,\"gridY\":16,\"type\":36}],\"exitGrounds\":[],\"holes\":[{\"gridX\":438,\"gridY\":80,\"type\":3},{\"gridX\":1130,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":1680,\"gridY\":96,\"type\":12},{\"gridX\":560,\"gridY\":152,\"type\":34},{\"gridX\":636,\"gridY\":148,\"type\":34},{\"gridX\":732,\"gridY\":148,\"type\":34},{\"gridX\":784,\"gridY\":148,\"type\":34},{\"gridX\":809,\"gridY\":96,\"type\":10},{\"gridX\":128,\"gridY\":7,\"type\":27},{\"gridX\":96,\"gridY\":7,\"type\":27},{\"gridX\":161,\"gridY\":7,\"type\":27},{\"gridX\":193,\"gridY\":7,\"type\":27},{\"gridX\":226,\"gridY\":7,\"type\":27},{\"gridX\":256,\"gridY\":7,\"type\":25},{\"gridX\":257,\"gridY\":13,\"type\":33},{\"gridX\":257,\"gridY\":47,\"type\":33},{\"gridX\":0,\"gridY\":50,\"type\":63},{\"gridX\":0,\"gridY\":114,\"type\":63},{\"gridX\":20,\"gridY\":64,\"type\":63},{\"gridX\":20,\"gridY\":44,\"type\":63},{\"gridX\":0,\"gridY\":29,\"type\":63},{\"gridX\":32,\"gridY\":44,\"type\":59},{\"gridX\":24,\"gridY\":36,\"type\":59},{\"gridX\":17,\"gridY\":27,\"type\":59},{\"gridX\":0,\"gridY\":71,\"type\":63},{\"gridX\":0,\"gridY\":93,\"type\":63},{\"gridX\":40,\"gridY\":104,\"type\":63},{\"gridX\":40,\"gridY\":84,\"type\":63},{\"gridX\":40,\"gridY\":60,\"type\":63},{\"gridX\":20,\"gridY\":108,\"type\":63},{\"gridX\":20,\"gridY\":88,\"type\":63},{\"gridX\":52,\"gridY\":56,\"type\":59},{\"gridX\":60,\"gridY\":68,\"type\":63},{\"gridX\":60,\"gridY\":92,\"type\":63},{\"gridX\":20,\"gridY\":128,\"type\":63},{\"gridX\":0,\"gridY\":136,\"type\":63},{\"gridX\":56,\"gridY\":108,\"type\":59},{\"gridX\":32,\"gridY\":120,\"type\":59},{\"gridX\":344,\"gridY\":192,\"type\":34},{\"gridX\":344,\"gridY\":180,\"type\":34},{\"gridX\":344,\"gridY\":168,\"type\":34},{\"gridX\":344,\"gridY\":156,\"type\":34},{\"gridX\":344,\"gridY\":144,\"type\":34},{\"gridX\":344,\"gridY\":112,\"type\":34},{\"gridX\":344,\"gridY\":100,\"type\":34},{\"gridX\":400,\"gridY\":192,\"type\":34},{\"gridX\":400,\"gridY\":180,\"type\":34},{\"gridX\":400,\"gridY\":168,\"type\":34},{\"gridX\":400,\"gridY\":156,\"type\":34},{\"gridX\":400,\"gridY\":124,\"type\":34},{\"gridX\":400,\"gridY\":112,\"type\":34},{\"gridX\":400,\"gridY\":100,\"type\":34},{\"gridX\":353,\"gridY\":96,\"type\":24},{\"gridX\":384,\"gridY\":96,\"type\":23},{\"gridX\":608,\"gridY\":48,\"type\":3},{\"gridX\":626,\"gridY\":48,\"type\":4},{\"gridX\":612,\"gridY\":48,\"type\":4},{\"gridX\":640,\"gridY\":48,\"type\":5},{\"gridX\":905,\"gridY\":80,\"type\":59},{\"gridX\":916,\"gridY\":72,\"type\":59},{\"gridX\":920,\"gridY\":87,\"type\":59},{\"gridX\":1032,\"gridY\":142,\"type\":63},{\"gridX\":1032,\"gridY\":75,\"type\":63},{\"gridX\":1048,\"gridY\":97,\"type\":59},{\"gridX\":1032,\"gridY\":65,\"type\":59},{\"gridX\":1046,\"gridY\":79,\"type\":59},{\"gridX\":1059,\"gridY\":83,\"type\":63},{\"gridX\":1058,\"gridY\":94,\"type\":59},{\"gridX\":1051,\"gridY\":64,\"type\":59},{\"gridX\":906,\"gridY\":93,\"type\":59},{\"gridX\":912,\"gridY\":108,\"type\":63},{\"gridX\":912,\"gridY\":128,\"type\":63},{\"gridX\":912,\"gridY\":148,\"type\":63},{\"gridX\":912,\"gridY\":168,\"type\":63},{\"gridX\":892,\"gridY\":90,\"type\":59},{\"gridX\":902,\"gridY\":102,\"type\":59},{\"gridX\":907,\"gridY\":184,\"type\":59},{\"gridX\":913,\"gridY\":178,\"type\":59},{\"gridX\":913,\"gridY\":193,\"type\":59},{\"gridX\":899,\"gridY\":197,\"type\":60},{\"gridX\":1032,\"gridY\":185,\"type\":63},{\"gridX\":1042,\"gridY\":179,\"type\":59},{\"gridX\":1032,\"gridY\":96,\"type\":61},{\"gridX\":1043,\"gridY\":54,\"type\":59},{\"gridX\":1071,\"gridY\":82,\"type\":59},{\"gridX\":1043,\"gridY\":42,\"type\":63},{\"gridX\":1043,\"gridY\":23,\"type\":63},{\"gridX\":1043,\"gridY\":5,\"type\":63},{\"gridX\":1043,\"gridY\":0,\"type\":63},{\"gridX\":1061,\"gridY\":0,\"type\":63},{\"gridX\":1063,\"gridY\":13,\"type\":59},{\"gridX\":1052,\"gridY\":25,\"type\":59},{\"gridX\":1039,\"gridY\":34,\"type\":33},{\"gridX\":1039,\"gridY\":0,\"type\":33},{\"gridX\":946,\"gridY\":82,\"type\":62},{\"gridX\":974,\"gridY\":112,\"type\":62},{\"gridX\":1020,\"gridY\":128,\"type\":60},{\"gridX\":1048,\"gridY\":168,\"type\":59},{\"gridX\":1032,\"gridY\":165,\"type\":61},{\"gridX\":1044,\"gridY\":152,\"type\":59},{\"gridX\":940,\"gridY\":143,\"type\":62},{\"gridX\":1032,\"gridY\":127,\"type\":63},{\"gridX" << "\":1009,\"gridY\":78,\"type\":62},{\"gridX\":1044,\"gridY\":119,\"type\":59},{\"gridX\":1050,\"gridY\":109,\"type\":63},{\"gridX\":1009,\"gridY\":205,\"type\":61},{\"gridX\":919,\"gridY\":206,\"type\":59},{\"gridX\":977,\"gridY\":176,\"type\":62},{\"gridX\":926,\"gridY\":219,\"type\":59},{\"gridX\":930,\"gridY\":232,\"type\":60},{\"gridX\":953,\"gridY\":249,\"type\":61},{\"gridX\":974,\"gridY\":249,\"type\":61},{\"gridX\":992,\"gridY\":249,\"type\":61},{\"gridX\":1033,\"gridY\":205,\"type\":61},{\"gridX\":1013,\"gridY\":249,\"type\":61},{\"gridX\":1031,\"gridY\":249,\"type\":61},{\"gridX\":816,\"gridY\":96,\"type\":53},{\"gridX\":872,\"gridY\":96,\"type\":55},{\"gridX\":896,\"gridY\":96,\"type\":56},{\"gridX\":848,\"gridY\":96,\"type\":55},{\"gridX\":824,\"gridY\":96,\"type\":55},{\"gridX\":540,\"gridY\":116,\"type\":48},{\"gridX\":536,\"gridY\":116,\"type\":47},{\"gridX\":556,\"gridY\":116,\"type\":49},{\"gridX\":616,\"gridY\":112,\"type\":48},{\"gridX\":612,\"gridY\":112,\"type\":47},{\"gridX\":632,\"gridY\":112,\"type\":49},{\"gridX\":712,\"gridY\":140,\"type\":48},{\"gridX\":708,\"gridY\":140,\"type\":47},{\"gridX\":728,\"gridY\":140,\"type\":49},{\"gridX\":764,\"gridY\":124,\"type\":48},{\"gridX\":760,\"gridY\":124,\"type\":47},{\"gridX\":780,\"gridY\":124,\"type\":49},{\"gridX\":828,\"gridY\":128,\"type\":48},{\"gridX\":824,\"gridY\":128,\"type\":47},{\"gridX\":844,\"gridY\":128,\"type\":49},{\"gridX\":830,\"gridY\":133,\"type\":21},{\"gridX\":1187,\"gridY\":75,\"type\":63},{\"gridX\":1187,\"gridY\":63,\"type\":59},{\"gridX\":1187,\"gridY\":96,\"type\":63},{\"gridX\":1188,\"gridY\":107,\"type\":59},{\"gridX\":1195,\"gridY\":112,\"type\":60},{\"gridX\":1202,\"gridY\":71,\"type\":59},{\"gridX\":1212,\"gridY\":81,\"type\":59},{\"gridX\":1205,\"gridY\":92,\"type\":63},{\"gridX\":1219,\"gridY\":112,\"type\":60},{\"gridX\":1214,\"gridY\":105,\"type\":59},{\"gridX\":1211,\"gridY\":109,\"type\":59},{\"gridX\":1251,\"gridY\":78,\"type\":59},{\"gridX\":1256,\"gridY\":89,\"type\":59},{\"gridX\":1247,\"gridY\":97,\"type\":60},{\"gridX\":1269,\"gridY\":82,\"type\":59},{\"gridX\":1272,\"gridY\":97,\"type\":60},{\"gridX\":1263,\"gridY\":94,\"type\":59},{\"gridX\":1297,\"gridY\":97,\"type\":60},{\"gridX\":1289,\"gridY\":94,\"type\":59},{\"gridX\":1307,\"gridY\":84,\"type\":59},{\"gridX\":1313,\"gridY\":95,\"type\":59},{\"gridX\":1321,\"gridY\":97,\"type\":60},{\"gridX\":1625,\"gridY\":92,\"type\":50},{\"gridX\":1641,\"gridY\":92,\"type\":50},{\"gridX\":1625,\"gridY\":50,\"type\":50},{\"gridX\":1641,\"gridY\":50,\"type\":50},{\"gridX\":1567,\"gridY\":96,\"type\":58},{\"gridX\":1556,\"gridY\":102,\"type\":60},{\"gridX\":1574,\"gridY\":102,\"type\":60},{\"gridX\":1421,\"gridY\":99,\"type\":59},{\"gridX\":1437,\"gridY\":106,\"type\":63},{\"gridX\":1393,\"gridY\":75,\"type\":60},{\"gridX\":1406,\"gridY\":96,\"type\":63},{\"gridX\":1336,\"gridY\":87,\"type\":61},{\"gridX\":1325,\"gridY\":85,\"type\":59},{\"gridX\":1334,\"gridY\":112,\"type\":63},{\"gridX\":1333,\"gridY\":104,\"type\":59},{\"gridX\":1340,\"gridY\":131,\"type\":63},{\"gridX\":1333,\"gridY\":124,\"type\":59},{\"gridX\":1332,\"gridY\":145,\"type\":60},{\"gridX\":1396,\"gridY\":108,\"type\":59},{\"gridX\":1402,\"gridY\":125,\"type\":63},{\"gridX\":1353,\"gridY\":145,\"type\":60},{\"gridX\":1348,\"gridY\":139,\"type\":59},{\"gridX\":1402,\"gridY\":143,\"type\":61},{\"gridX\":1416,\"gridY\":111,\"type\":63},{\"gridX\":1364,\"gridY\":156,\"type\":59},{\"gridX\":1369,\"gridY\":172,\"type\":63},{\"gridX\":1367,\"gridY\":180,\"type\":60},{\"gridX\":1389,\"gridY\":180,\"type\":60},{\"gridX\":1383,\"gridY\":174,\"type\":59},{\"gridX\":1426,\"gridY\":126,\"type\":60},{\"gridX\":1448,\"gridY\":126,\"type\":60},{\"gridX\":1471,\"gridY\":126,\"type\":60},{\"gridX\":1418,\"gridY\":123,\"type\":59},{\"gridX\":1441,\"gridY\":122,\"type\":59},{\"gridX\":1462,\"gridY\":121,\"type\":59},{\"gridX\":1453,\"gridY\":107,\"type\":59},{\"gridX\":1493,\"gridY\":126,\"type\":60},{\"gridX\":1485,\"gridY\":120,\"type\":59},{\"gridX\":1497,\"gridY\":149,\"type\":58},{\"gridX\":1173,\"gridY\":196,\"type\":60},{\"gridX\":1195,\"gridY\":196,\"type\":60},{\"gridX\":1187,\"gridY\":189,\"type\":59},{\"gridX\":1218,\"gridY\":196,\"type\":60},{\"gridX\":1210,\"gridY\":188,\"type\":59},{\"gridX\":1200,\"gridY\":175,\"type\":63},{\"gridX\":1204,\"gridY\":160,\"type\":59},{\"gridX\":1282,\"gridY\":217,\"type\":60},{\"gridX\":1302,\"gridY\":217,\"type\":60},{\"gridX\":1295,\"gridY\":208,\"type\":59},{\"gridX\":1264,\"gridY\":157,\"type\":60},{\"gridX\":1323,\"gridY\":217,\"type\":60},{\"gridX\":1345,\"gridY\":234,\"type\":61},{\"gridX\":1315,\"gridY\":210,\"type\":59},{\"gridX\":1489,\"gridY\":158,\"type\":59},{\"gridX\":1475,\"gridY\":168,\"type\":60},{\"gridX\":1494,\"gridY\":168,\"type\":60},{\"gridX\":1410,\"gridY\":197,\"type\":61}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1648,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":116,\"gridY\":28,\"type\":3},{\"gridX\":160,\"gridY\":44,\"type\":3},{\"gridX\":536,\"gridY\":121,\"type\":9},{\"gridX\":580,\"gridY\":96,\"type\":9},{\"gridX\":608,\"gridY\":117,\"type\":10},{\"gridX\":708,\"gridY\":145,\"type\":11},{\"gridX\":764,\"gridY\":96,\"type\":12},{\"gridX\":608,\"gridY\":7,\"type\":6},{\"gridX\":593,\"gridY\":7,\"type\":6},{\"gridX\":580,\"gridY\":7,\"type\":6},{\"gridX\":566,\"gridY\":7,\"type\":6},{\"gridX\":552,\"gridY\":7,\"type\":6},{\"gridX\":538,\"gridY\":7,\"type\":6},{\"gridX\":524,\"gridY\":7,\"type\":6},{\"gridX\":624,\"gridY\":7,\"type\":6},{\"gridX\":658,\"gridY\":77,\"type\":7},{\"gridX\":644,\"gridY\":77,\"type\":7},{\"gridX\":630,\"gridY\":77,\"type\":7},{\"gridX\":587,\"gridY\":77,\"type\":7},{\"gridX\":573,\"gridY\":77,\"type\":7},{\"gridX\":558,\"gridY\":77,\"type\":7},{\"gridX\":544,\"gridY\":77,\"type\":7},{\"gridX\":530,\"gridY\":77,\"type\":7},{\"gridX\":641,\"gridY\":7,\"type\":6},{\"gridX\":659,\"gridY\":7,\"type\":6},{\"gridX\":691,\"gridY\":7,\"type\":6},{\"gridX\":725,\"gridY\":7,\"type\":6},{\"gridX\":765,\"gridY\":7,\"type\":6},{\"gridX\":680,\"gridY\":77,\"type\":7},{\"gridX\":711,\"gridY\":77,\"type\":7},{\"gridX\":750,\"gridY\":77,\"type\":7},{\"gridX\":791,\"gridY\":77,\"type\":7},{\"gridX\":895,\"gridY\":7,\"type\":6},{\"gridX\":826,\"gridY\":7,\"type\":6},{\"gridX\":510,\"gridY\":7,\"type\":6},{\"gridX\":674,\"gridY\":7,\"type\":6},{\"gridX\":956,\"gridY\":7,\"type\":6},{\"gridX\":1030,\"gridY\":102,\"type\":6},{\"gridX\":616,\"gridY\":77,\"type\":7},{\"gridX\":601,\"gridY\":77,\"type\":7},{\"gridX\":1030,\"gridY\":171,\"type\":6},{\"gridX\":1029,\"gridY\":116,\"type\":7},{\"gridX\":1016,\"gridY\":193,\"type\":7},{\"gridX\":210,\"gridY\":61,\"type\":3},{\"gridX\":1391,\"gridY\":98,\"type\":6},{\"gridX\":1350,\"gridY\":93,\"type\":6},{\"gridX\":1307,\"gridY\":120,\"type\":5},{\"gridX\":1308,\"gridY\":240,\"type\":5},{\"gridX\":1339,\"gridY\":240,\"type\":5},{\"gridX\":1463,\"gridY\":149,\"type\":5},{\"gridX\":1525,\"gridY\":75,\"type\":5}],\"collectibles\":[{\"gridX\":124,\"gridY\":56,\"type\":0},{\"gridX\":164,\"gridY\":64,\"type\":0},{\"gridX\":170,\"gridY\":80,\"type\":0},{\"gridX\":166,\"gridY\":72,\"type\":0},{\"gridX\":162,\"gridY\":56,\"type\":0},{\"gridX\":220,\"gridY\":96,\"type\":0},{\"gridX\":234,\"gridY\":108,\"type\":0},{\"gridX\":212,\"gridY\":72,\"type\":0},{\"gridX\":214,\"gridY\":80,\"type\":0},{\"gridX\":216,\"gridY\":88,\"type\":0},{\"gridX\":226,\"gridY\":104,\"type\":0},{\"gridX\":280,\"gridY\":104,\"type\":0},{\"gridX\":244,\"gridY\":111,\"type\":0},{\"gridX\":296,\"gridY\":104,\"type\":0},{\"gridX\":288,\"gridY\":104,\"type\":0},{\"gridX\":120,\"gridY\":48,\"type\":0},{\"gridX\":118,\"gridY\":40,\"type\":0},{\"gridX\":324,\"gridY\":112,\"type\":0},{\"gridX\":336,\"gridY\":128,\"type\":0},{\"gridX\":328,\"gridY\":120,\"type\":0},{\"gridX\":344,\"gridY\":132,\"type\":0},{\"gridX\":352,\"gridY\":132,\"type\":0},{\"gridX\":360,\"gridY\":128,\"type\":0},{\"gridX\":368," << "\"gridY\":124,\"type\":0},{\"gridX\":621,\"gridY\":6,\"type\":1},{\"gridX\":304,\"gridY\":104,\"type\":0},{\"gridX\":320,\"gridY\":104,\"type\":0},{\"gridX\":312,\"gridY\":104,\"type\":0},{\"gridX\":392,\"gridY\":140,\"type\":0},{\"gridX\":384,\"gridY\":132,\"type\":0},{\"gridX\":376,\"gridY\":124,\"type\":0},{\"gridX\":400,\"gridY\":140,\"type\":0},{\"gridX\":408,\"gridY\":140,\"type\":0},{\"gridX\":416,\"gridY\":136,\"type\":0},{\"gridX\":424,\"gridY\":132,\"type\":0},{\"gridX\":432,\"gridY\":124,\"type\":0},{\"gridX\":440,\"gridY\":116,\"type\":0},{\"gridX\":553,\"gridY\":41,\"type\":0},{\"gridX\":440,\"gridY\":102,\"type\":0},{\"gridX\":577,\"gridY\":65,\"type\":0},{\"gridX\":612,\"gridY\":57,\"type\":0},{\"gridX\":633,\"gridY\":57,\"type\":0},{\"gridX\":622,\"gridY\":58,\"type\":0},{\"gridX\":676,\"gridY\":40,\"type\":0},{\"gridX\":586,\"gridY\":31,\"type\":0},{\"gridX\":656,\"gridY\":65,\"type\":0},{\"gridX\":740,\"gridY\":73,\"type\":0},{\"gridX\":749,\"gridY\":8,\"type\":0},{\"gridX\":727,\"gridY\":26,\"type\":0},{\"gridX\":754,\"gridY\":46,\"type\":0},{\"gridX\":787,\"gridY\":64,\"type\":0},{\"gridX\":779,\"gridY\":30,\"type\":0},{\"gridX\":799,\"gridY\":8,\"type\":0},{\"gridX\":840,\"gridY\":46,\"type\":0},{\"gridX\":811,\"gridY\":49,\"type\":0},{\"gridX\":1031,\"gridY\":217,\"type\":1},{\"gridX\":841,\"gridY\":8,\"type\":0},{\"gridX\":891,\"gridY\":8,\"type\":0},{\"gridX\":865,\"gridY\":73,\"type\":0},{\"gridX\":980,\"gridY\":64,\"type\":0},{\"gridX\":973,\"gridY\":51,\"type\":0},{\"gridX\":988,\"gridY\":190,\"type\":0},{\"gridX\":964,\"gridY\":27,\"type\":0},{\"gridX\":967,\"gridY\":39,\"type\":0},{\"gridX\":1020,\"gridY\":92,\"type\":0},{\"gridX\":957,\"gridY\":97,\"type\":0},{\"gridX\":981,\"gridY\":190,\"type\":0},{\"gridX\":944,\"gridY\":159,\"type\":0},{\"gridX\":985,\"gridY\":126,\"type\":0},{\"gridX\":951,\"gridY\":159,\"type\":0},{\"gridX\":956,\"gridY\":103,\"type\":0},{\"gridX\":949,\"gridY\":165,\"type\":0},{\"gridX\":1212,\"gridY\":140,\"type\":0},{\"gridX\":978,\"gridY\":127,\"type\":0},{\"gridX\":988,\"gridY\":75,\"type\":0},{\"gridX\":1228,\"gridY\":140,\"type\":0},{\"gridX\":985,\"gridY\":133,\"type\":0},{\"gridX\":987,\"gridY\":196,\"type\":0},{\"gridX\":950,\"gridY\":97,\"type\":0},{\"gridX\":1220,\"gridY\":140,\"type\":0},{\"gridX\":1012,\"gridY\":92,\"type\":0},{\"gridX\":1019,\"gridY\":98,\"type\":0},{\"gridX\":1204,\"gridY\":140,\"type\":0},{\"gridX\":1196,\"gridY\":140,\"type\":0},{\"gridX\":1272,\"gridY\":132,\"type\":0},{\"gridX\":1340,\"gridY\":180,\"type\":0},{\"gridX\":1376,\"gridY\":208,\"type\":0},{\"gridX\":1392,\"gridY\":208,\"type\":0},{\"gridX\":1508,\"gridY\":196,\"type\":0},{\"gridX\":1408,\"gridY\":208,\"type\":0},{\"gridX\":1332,\"gridY\":172,\"type\":0},{\"gridX\":1384,\"gridY\":208,\"type\":0},{\"gridX\":1268,\"gridY\":124,\"type\":0},{\"gridX\":1332,\"gridY\":180,\"type\":0},{\"gridX\":1500,\"gridY\":196,\"type\":0},{\"gridX\":1516,\"gridY\":196,\"type\":0},{\"gridX\":1416,\"gridY\":208,\"type\":0},{\"gridX\":1340,\"gridY\":172,\"type\":0},{\"gridX\":1264,\"gridY\":132,\"type\":0},{\"gridX\":1260,\"gridY\":124,\"type\":0},{\"gridX\":1276,\"gridY\":124,\"type\":0},{\"gridX\":1400,\"gridY\":208,\"type\":0},{\"gridX\":1228,\"gridY\":224,\"type\":0},{\"gridX\":1212,\"gridY\":224,\"type\":0},{\"gridX\":1220,\"gridY\":224,\"type\":0},{\"gridX\":1204,\"gridY\":224,\"type\":0},{\"gridX\":1196,\"gridY\":224,\"type\":0},{\"gridX\":1268,\"gridY\":184,\"type\":0},{\"gridX\":1276,\"gridY\":192,\"type\":0},{\"gridX\":1284,\"gridY\":184,\"type\":0},{\"gridX\":1276,\"gridY\":184,\"type\":0},{\"gridX\":1268,\"gridY\":192,\"type\":0},{\"gridX\":1520,\"gridY\":28,\"type\":2},{\"gridX\":1532,\"gridY\":28,\"type\":2},{\"gridX\":1556,\"gridY\":28,\"type\":2},{\"gridX\":1580,\"gridY\":28,\"type\":2},{\"gridX\":1544,\"gridY\":28,\"type\":2},{\"gridX\":1568,\"gridY\":28,\"type\":2},{\"gridX\":1596,\"gridY\":32,\"type\":3},{\"gridX\":1508,\"gridY\":28,\"type\":2},{\"gridX\":1496,\"gridY\":28,\"type\":2},{\"gridX\":1376,\"gridY\":120,\"type\":3},{\"gridX\":1484,\"gridY\":28,\"type\":2}],\"jons\":[{\"gridX\":1012,\"gridY\":208}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":403,\"gridY\":92,\"type\":3},{\"gridX\":533,\"gridY\":92,\"type\":3},{\"gridX\":476,\"gridY\":92,\"type\":1},{\"gridX\":622,\"gridY\":92,\"type\":2},{\"gridX\":330,\"gridY\":92,\"type\":3},{\"gridX\":257,\"gridY\":85,\"type\":0},{\"gridX\":728,\"gridY\":85,\"type\":0},{\"gridX\":1111,\"gridY\":85,\"type\":0},{\"gridX\":1142,\"gridY\":92,\"type\":3},{\"gridX\":1714,\"gridY\":92,\"type\":2},{\"gridX\":1631,\"gridY\":92,\"type\":2},{\"gridX\":1611,\"gridY\":92,\"type\":1},{\"gridX\":1638,\"gridY\":92,\"type\":3},{\"gridX\":1615,\"gridY\":85,\"type\":0}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level18(text.c_str());
}

void Chapter1Level18::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level18 * Chapter1Level18::getInstance()
{
    return s_pInstance;
}

Chapter1Level18::Chapter1Level18(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 19 ///

Chapter1Level19 * Chapter1Level19::s_pInstance = nullptr;

void Chapter1Level19::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":19,\"midgrounds\":[{\"gridX\":323,\"gridY\":96,\"type\":2},{\"gridX\":398,\"gridY\":96,\"type\":2},{\"gridX\":384,\"gridY\":0,\"type\":3},{\"gridX\":316,\"gridY\":0,\"type\":3},{\"gridX\":549,\"gridY\":96,\"type\":2},{\"gridX\":832,\"gridY\":0,\"type\":3},{\"gridX\":872,\"gridY\":0,\"type\":5},{\"gridX\":896,\"gridY\":0,\"type\":4},{\"gridX\":1156,\"gridY\":0,\"type\":3},{\"gridX\":1212,\"gridY\":0,\"type\":3},{\"gridX\":1252,\"gridY\":0,\"type\":3},{\"gridX\":1312,\"gridY\":0,\"type\":3},{\"gridX\":1044,\"gridY\":96,\"type\":2},{\"gridX\":1164,\"gridY\":96,\"type\":2},{\"gridX\":1284,\"gridY\":96,\"type\":2},{\"gridX\":1390,\"gridY\":0,\"type\":5},{\"gridX\":1044,\"gridY\":0,\"type\":7},{\"gridX\":1108,\"gridY\":12,\"type\":7},{\"gridX\":1108,\"gridY\":0,\"type\":7},{\"gridX\":1044,\"gridY\":24,\"type\":7},{\"gridX\":1044,\"gridY\":12,\"type\":7},{\"gridX\":1044,\"gridY\":36,\"type\":7},{\"gridX\":1108,\"gridY\":24,\"type\":7},{\"gridX\":1108,\"gridY\":36,\"type\":7},{\"gridX\":248,\"gridY\":4,\"type\":7},{\"gridX\":248,\"gridY\":16,\"type\":7},{\"gridX\":248,\"gridY\":52,\"type\":7},{\"gridX\":248,\"gridY\":40,\"type\":7},{\"gridX\":248,\"gridY\":28,\"type\":7},{\"gridX\":360,\"gridY\":0,\"type\":7},{\"gridX\":360,\"gridY\":12,\"type\":7},{\"gridX\":360,\"gridY\":24,\"type\":7},{\"gridX\":360,\"gridY\":36,\"type\":7},{\"gridX\":296,\"gridY\":24,\"type\":7},{\"gridX\":296,\"gridY\":12,\"type\":7},{\"gridX\":296,\"gridY\":0,\"type\":7},{\"gridX\":432,\"gridY\":36,\"type\":7},{\"gridX\":432,\"gridY\":24,\"type\":7},{\"gridX\":432,\"gridY\":12,\"type\":7},{\"gridX\":432,\"gridY\":0,\"type\":7},{\"gridX\":432,\"gridY\":48,\"type\":7},{\"gridX\":1108,\"gridY\":48,\"type\":7},{\"gridX\":854,\"gridY\":96,\"type\":17},{\"gridX\":867,\"gridY\":96,\"type\":17},{\"gridX\":888,\"gridY\":96,\"type\":17},{\"gridX\":908,\"gridY\":96,\"type\":17},{\"gridX\":929,\"gridY\":96,\"type\":17},{\"gridX\":846,\"gridY\":114,\"type\":18},{\"gridX\":850,\"gridY\":114,\"type\":17},{\"gridX\":866,\"gridY\":110,\"type\":18},{\"gridX\":931,\"gridY\":110,\"type\":17},{\"gridX\":887,\"gridY\":114,\"type\":18},{\"gridX\":900,\"gridY\":105,\"type\":18},{\"gridX\":913,\"gridY\":109,\"type\":18},{\"gridX\":936,\"gridY\":122,\"type\":18},{\"gridX\":1218,\"gridY\":96,\"type\":15},{\"gridX\":1076,\"gridY\":96,\"type\":16},{\"gridX\":1338,\"gridY\":96,\"type\":14},{\"gridX\":1104,\"gridY\":96,\"type\":13},{\"gridX\":831,\"gridY\":96,\"type\":11},{\"gridX\":834,\"gridY\":96,\"type\":6},{\"gridX\":948,\"gridY\":188,\"type\":7},{\"gridX\":948,\"gridY\":200,\"type\":7},{\"gridX\":980,\"gridY\":200,\"type\":7},{\"gridX\":980,\"gridY\":188,\"type\":7},{\"gridX\":944,\"gridY\":208,\"type\":9},{\"gridX\":920,\"gridY\":96,\"type\":6},{\"gridX\":662,\"gridY\":96,\"type\":13},{\"gridX\":603,\"gridY\":96,\"type\":14},{\"gridX\":503,\"gridY\":96,\"type\":13},{\"gridX\":423,\"gridY\":96,\"type\":13},{\"gridX\":356,\"gridY\":96,\"type\":14},{\"gridX\":479,\"gridY\":96,\"type\":15},{\"gridX\":671,\"gridY\":96,\"type\":11},{\"gridX\":724,\"gridY\":96,\"type\":16},{\"gridX\":625,\"gridY\":96,\"type\":6},{\"gridX\":1060,\"gridY\":96,\"type\":7},{\"gridX\":1060,\"gridY\":108,\"type\":7},{\"gridX\":1060,\"gridY\":120,\"type\":7},{\"gridX\":1056,\"gridY\":132,\"type\":8},{\"gridX\":1092,\"gridY\":96,\"type\":7},{\"gridX\":1092,\"gridY\":108,\"type\":7},{\"gridX\":1092,\"gridY\":120,\"type\":7},{\"gridX\":496,\"gridY\":96,\"type\":17},{\"gridX\":481,\"gridY\":109,\"type\":18},{\"gridX\":516,\"gridY\":96,\"type\":17},{\"gridX\":537,\"gridY\":96,\"type\":17},{\"gridX\":557,\"gridY\":96,\"type\":17},{\"gridX\":577,\"gridY\":96,\"type\":17},{\"gridX\":598,\"gridY\":96,\"type\":17},{\"gridX\":617,\"gridY\":96,\"type\":17},{\"gridX\":639,\"gridY\":96,\"type\":17},{\"gridX\":682,\"gridY\":96,\"type\":17},{\"gridX\":660,\"gridY\":96,\"type\":17},{\"gridX\":702,\"gridY\":96,\"type\":17},{\"gridX\":722,\"gridY\":96,\"type\":17},{\"gridX\":579,\"gridY\":117,\"type\":17},{\"gridX\":578,\"gridY\":138,\"type\":17},{\"gridX\":557,\"gridY\":117,\"type\":17},{\"gridX\":557,\"gridY\":138,\"type\":17},{\"gridX\":494,\"gridY\":117,\"type\":17},{\"gridX\":515,\"gridY\":117,\"type\":17},{\"gridX\":537,\"gridY\":117,\"type\":17},{\"gridX\":537,\"gridY\":137,\"type\":17},{\"gridX\":515,\"gridY\":138,\"type\":17},{\"gridX\":504,\"gridY\":133,\"type\":18},{\"gridX\":628,\"gridY\":154,\"type\":18},{\"gridX\":639,\"gridY\":154,\"type\":17},{\"gridX\":660,\"gridY\":154,\"type\":17},{\"gridX\":681,\"gridY\":154,\"type\":17},{\"gridX\":679,\"gridY\":133,\"type\":17},{\"gridX\":699,\"gridY\":131,\"type\":17},{\"gridX\":658,\"gridY\":132,\"type\":17},{\"gridX\":637,\"gridY\":132,\"type\":17},{\"gridX\":616,\"gridY\":132,\"type\":17},{\"gridX\":609,\"gridY\":116,\"type\":17},{\"gridX\":631,\"gridY\":114,\"type\":17},{\"gridX\":647,\"gridY\":112,\"type\":18},{\"gridX\":658,\"gridY\":112,\"type\":17},{\"gridX\":678,\"gridY\":114,\"type\":17},{\"gridX\":699,\"gridY\":113,\"type\":17},{\"gridX\":722,\"gridY\":117,\"type\":17},{\"gridX\":716,\"gridY\":128,\"type\":18},{\"gridX\":698,\"gridY\":146,\"type\":18},{\"gridX\":670,\"gridY\":173,\"type\":17},{\"gridX\":651,\"gridY\":175,\"type\":17},{\"gridX\":637,\"gridY\":175,\"type\":17},{\"gridX\":622,\"gridY\":184,\"type\":18},{\"gridX\":681,\"gridY\":174,\"type\":18},{\"gridX\":681,\"gridY\":186,\"type\":18},{\"gridX\":648,\"gridY\":185,\"type\":18}],\"grounds\":[{\"gridX\":128,\"gridY\":0,\"type\":0},{\"gridX\":896,\"gridY\":0,\"type\":0},{\"gridX\":461,\"gridY\":0,\"type\":0},{\"gridX\":936,\"gridY\":0,\"type\":1},{\"gridX\":1032,\"gridY\":0,\"type\":1},{\"gridX\":904,\"gridY\":0,\"type\":1},{\"gridX\":968,\"gridY\":0,\"type\":2},{\"gridX\":1064,\"gridY\":0,\"type\":2},{\"gridX\":136,\"gridY\":0,\"type\":3},{\"gridX\":725,\"gridY\":0,\"type\":3},{\"gridX\":597,\"gridY\":0,\"type\":3},{\"gridX\":469,\"gridY\":0,\"type\":3},{\"gridX\":264,\"gridY\":0,\"type\":4},{\"gridX\":1128,\"gridY\":0,\"type\":4},{\"gridX\":853,\"gridY\":0,\"type\":4},{\"gridX\":864,\"gridY\":50,\"type\":10},{\"gridX\":530,\"gridY\":50,\"type\":10},{\"gridX\":872,\"gridY\":50,\"type\":11},{\"gridX\":768,\"gridY\":50,\"type\":11},{\"gridX\":538,\"gridY\":50,\"type\":12},{\"gridX\":640,\"gridY\":50,\"type\":13},{\"gridX\":904,\"gridY\":50,\"type\":14},{\"gridX\":800,\"gridY\":50,\"type\":14},{\"gridX\":596,\"gridY\":48,\"type\":15},{\"gridX\":768,\"gridY\":48,\"type\":15},{\"gridX\":700,\"gridY\":48,\"type\":15},{\"gridX\":776,\"gridY\":48,\"type\":16},{\"gridX\":604,\"gridY\":48,\"type\":16},{\"gridX\":708,\"gridY\":48,\"type\":19},{\"gridX\":808,\"gridY\":48,\"type\":19},{\"gridX\":636,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":392,\"gridY\":90,\"type\":21},{\"gridX\":1352,\"gridY\":90,\"type\":21},{\"gridX\":808,\"gridY\":90,\"type\":21},{\"gridX\":1416,\"gridY\":90,\"type\":21},{\"gridX\":1288,\"gridY\":90,\"type\":22},{\"gridX\":712,\"gridY\":90,\"type\":22},{\"gridX\":968,\"gridY\":90,\"type\":22},{\"gridX\":1160,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":1032,\"gridY\":90,\"type\":23},{\"gridX\":584,\"gridY\":90,\"type\":23},{\"gridX\":840,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":456,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":1440,\"gridY\":0,\"type\":25},{\"gridX\":1448,\"gridY\":0,\"type\":28},{\"gridX\":1576,\"gridY\":0,\"type\":28},{\"gridX\":1704,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[{\"gridX\":424,\"gridY\":88,\"type\":0},{\"gridX\":776,\"gridY\":88,\"type\":0},{\"gridX\":1384,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":196,\"gridY\":80,\"type\":3},{\"gridX\":248,\"gridY\":80,\"type\":3},{\"gridX\":817,\"gridY\":80,\"type\":3},{\"gridX\":868,\"gridY\":73,\"type\":0},{\"gridX\":884,\"gridY\":73,\"type\":1},{\"gridX\":900,\"gridY\":73,\"type\":1},{\"gridX\":916,\"gridY\":73,\"type\":1},{\"gridX\":932,\"gridY\":73,\"type\":2},{\"gridX\":1072," << "\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":1536,\"gridY\":96,\"type\":12},{\"gridX\":152,\"gridY\":96,\"type\":6},{\"gridX\":433,\"gridY\":61,\"type\":19},{\"gridX\":836,\"gridY\":76,\"type\":34},{\"gridX\":844,\"gridY\":68,\"type\":34},{\"gridX\":836,\"gridY\":60,\"type\":34},{\"gridX\":836,\"gridY\":44,\"type\":34},{\"gridX\":844,\"gridY\":52,\"type\":34},{\"gridX\":844,\"gridY\":36,\"type\":34},{\"gridX\":836,\"gridY\":28,\"type\":34},{\"gridX\":1388,\"gridY\":50,\"type\":3},{\"gridX\":1392,\"gridY\":50,\"type\":4},{\"gridX\":1406,\"gridY\":50,\"type\":5},{\"gridX\":1391,\"gridY\":55,\"type\":41},{\"gridX\":1460,\"gridY\":95,\"type\":35},{\"gridX\":1436,\"gridY\":95,\"type\":35},{\"gridX\":1484,\"gridY\":95,\"type\":35},{\"gridX\":1157,\"gridY\":37,\"type\":62},{\"gridX\":1213,\"gridY\":38,\"type\":62},{\"gridX\":1253,\"gridY\":27,\"type\":62},{\"gridX\":1329,\"gridY\":27,\"type\":62},{\"gridX\":1096,\"gridY\":56,\"type\":48},{\"gridX\":1112,\"gridY\":56,\"type\":48},{\"gridX\":1092,\"gridY\":56,\"type\":47},{\"gridX\":1128,\"gridY\":56,\"type\":49},{\"gridX\":1048,\"gridY\":44,\"type\":49},{\"gridX\":950,\"gridY\":96,\"type\":58},{\"gridX\":946,\"gridY\":127,\"type\":59},{\"gridX\":950,\"gridY\":108,\"type\":63},{\"gridX\":959,\"gridY\":117,\"type\":59},{\"gridX\":869,\"gridY\":117,\"type\":60},{\"gridX\":963,\"gridY\":96,\"type\":58},{\"gridX\":956,\"gridY\":163,\"type\":59},{\"gridX\":954,\"gridY\":139,\"type\":59},{\"gridX\":955,\"gridY\":152,\"type\":63},{\"gridX\":944,\"gridY\":167,\"type\":60},{\"gridX\":965,\"gridY\":167,\"type\":60},{\"gridX\":140,\"gridY\":96,\"type\":53},{\"gridX\":148,\"gridY\":96,\"type\":55},{\"gridX\":172,\"gridY\":96,\"type\":54},{\"gridX\":184,\"gridY\":96,\"type\":56},{\"gridX\":7,\"gridY\":96,\"type\":57},{\"gridX\":265,\"gridY\":63,\"type\":48},{\"gridX\":245,\"gridY\":63,\"type\":47},{\"gridX\":249,\"gridY\":63,\"type\":48},{\"gridX\":281,\"gridY\":63,\"type\":49},{\"gridX\":360,\"gridY\":43,\"type\":47},{\"gridX\":364,\"gridY\":43,\"type\":49},{\"gridX\":468,\"gridY\":8,\"type\":52},{\"gridX\":468,\"gridY\":24,\"type\":52},{\"gridX\":468,\"gridY\":40,\"type\":52},{\"gridX\":468,\"gridY\":56,\"type\":52},{\"gridX\":468,\"gridY\":72,\"type\":52},{\"gridX\":787,\"gridY\":75,\"type\":41},{\"gridX\":768,\"gridY\":96,\"type\":51},{\"gridX\":800,\"gridY\":96,\"type\":51},{\"gridX\":784,\"gridY\":108,\"type\":50},{\"gridX\":696,\"gridY\":59,\"type\":32},{\"gridX\":764,\"gridY\":59,\"type\":32},{\"gridX\":592,\"gridY\":59,\"type\":32},{\"gridX\":665,\"gridY\":84,\"type\":29},{\"gridX\":733,\"gridY\":84,\"type\":29},{\"gridX\":432,\"gridY\":56,\"type\":47},{\"gridX\":436,\"gridY\":56,\"type\":49},{\"gridX\":296,\"gridY\":32,\"type\":47},{\"gridX\":300,\"gridY\":32,\"type\":49},{\"gridX\":1044,\"gridY\":44,\"type\":47},{\"gridX\":846,\"gridY\":134,\"type\":61},{\"gridX\":880,\"gridY\":114,\"type\":59},{\"gridX\":1291,\"gridY\":96,\"type\":57},{\"gridX\":1159,\"gridY\":96,\"type\":57},{\"gridX\":247,\"gridY\":52,\"type\":34},{\"gridX\":464,\"gridY\":113,\"type\":60},{\"gridX\":481,\"gridY\":124,\"type\":59},{\"gridX\":494,\"gridY\":143,\"type\":60},{\"gridX\":510,\"gridY\":138,\"type\":59},{\"gridX\":492,\"gridY\":135,\"type\":59},{\"gridX\":531,\"gridY\":140,\"type\":59},{\"gridX\":516,\"gridY\":143,\"type\":60},{\"gridX\":540,\"gridY\":143,\"type\":60},{\"gridX\":560,\"gridY\":160,\"type\":61},{\"gridX\":581,\"gridY\":143,\"type\":60},{\"gridX\":592,\"gridY\":134,\"type\":59},{\"gridX\":600,\"gridY\":143,\"type\":60},{\"gridX\":599,\"gridY\":129,\"type\":59},{\"gridX\":594,\"gridY\":114,\"type\":59},{\"gridX\":622,\"gridY\":143,\"type\":60},{\"gridX\":711,\"gridY\":143,\"type\":60},{\"gridX\":733,\"gridY\":143,\"type\":60},{\"gridX\":615,\"gridY\":138,\"type\":59},{\"gridX\":726,\"gridY\":137,\"type\":59},{\"gridX\":732,\"gridY\":125,\"type\":59},{\"gridX\":670,\"gridY\":183,\"type\":59},{\"gridX\":659,\"gridY\":194,\"type\":59},{\"gridX\":635,\"gridY\":195,\"type\":63},{\"gridX\":620,\"gridY\":199,\"type\":59},{\"gridX\":648,\"gridY\":189,\"type\":59},{\"gridX\":617,\"gridY\":219,\"type\":60},{\"gridX\":639,\"gridY\":215,\"type\":60},{\"gridX\":632,\"gridY\":210,\"type\":59},{\"gridX\":664,\"gridY\":215,\"type\":60},{\"gridX\":685,\"gridY\":212,\"type\":60},{\"gridX\":678,\"gridY\":208,\"type\":59},{\"gridX\":671,\"gridY\":197,\"type\":59},{\"gridX\":655,\"gridY\":211,\"type\":63},{\"gridX\":628,\"gridY\":132,\"type\":59},{\"gridX\":714,\"gridY\":129,\"type\":59},{\"gridX\":644,\"gridY\":135,\"type\":61},{\"gridX\":668,\"gridY\":135,\"type\":61},{\"gridX\":691,\"gridY\":135,\"type\":61},{\"gridX\":653,\"gridY\":141,\"type\":24},{\"gridX\":686,\"gridY\":141,\"type\":24}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1504,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":268,\"gridY\":68,\"type\":6},{\"gridX\":268,\"gridY\":96,\"type\":6},{\"gridX\":580,\"gridY\":96,\"type\":12},{\"gridX\":593,\"gridY\":96,\"type\":12},{\"gridX\":708,\"gridY\":96,\"type\":12},{\"gridX\":223,\"gridY\":96,\"type\":6},{\"gridX\":214,\"gridY\":77,\"type\":7},{\"gridX\":844,\"gridY\":7,\"type\":6},{\"gridX\":791,\"gridY\":7,\"type\":6},{\"gridX\":739,\"gridY\":7,\"type\":6},{\"gridX\":682,\"gridY\":7,\"type\":6},{\"gridX\":624,\"gridY\":7,\"type\":6},{\"gridX\":568,\"gridY\":7,\"type\":6},{\"gridX\":508,\"gridY\":7,\"type\":6},{\"gridX\":1094,\"gridY\":7,\"type\":6},{\"gridX\":1041,\"gridY\":7,\"type\":6},{\"gridX\":987,\"gridY\":7,\"type\":6},{\"gridX\":512,\"gridY\":96,\"type\":5},{\"gridX\":521,\"gridY\":96,\"type\":10},{\"gridX\":539,\"gridY\":96,\"type\":10},{\"gridX\":563,\"gridY\":96,\"type\":9},{\"gridX\":540,\"gridY\":102,\"type\":3},{\"gridX\":640,\"gridY\":101,\"type\":3},{\"gridX\":553,\"gridY\":96,\"type\":5},{\"gridX\":663,\"gridY\":96,\"type\":5},{\"gridX\":727,\"gridY\":96,\"type\":5},{\"gridX\":685,\"gridY\":96,\"type\":5},{\"gridX\":744,\"gridY\":96,\"type\":8},{\"gridX\":715,\"gridY\":96,\"type\":8},{\"gridX\":569,\"gridY\":102,\"type\":3},{\"gridX\":672,\"gridY\":101,\"type\":3},{\"gridX\":524,\"gridY\":102,\"type\":3},{\"gridX\":695,\"gridY\":102,\"type\":3},{\"gridX\":731,\"gridY\":101,\"type\":3},{\"gridX\":529,\"gridY\":96,\"type\":8},{\"gridX\":915,\"gridY\":7,\"type\":6}],\"collectibles\":[{\"gridX\":128,\"gridY\":104,\"type\":0},{\"gridX\":132,\"gridY\":112,\"type\":0},{\"gridX\":136,\"gridY\":120,\"type\":0},{\"gridX\":140,\"gridY\":128,\"type\":0},{\"gridX\":144,\"gridY\":136,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":168,\"gridY\":136,\"type\":0},{\"gridX\":176,\"gridY\":140,\"type\":0},{\"gridX\":184,\"gridY\":144,\"type\":0},{\"gridX\":200,\"gridY\":136,\"type\":0},{\"gridX\":200,\"gridY\":128,\"type\":0},{\"gridX\":200,\"gridY\":120,\"type\":0},{\"gridX\":200,\"gridY\":112,\"type\":0},{\"gridX\":200,\"gridY\":104,\"type\":0},{\"gridX\":232,\"gridY\":79,\"type\":1},{\"gridX\":376,\"gridY\":71,\"type\":0},{\"gridX\":367,\"gridY\":63,\"type\":0},{\"gridX\":355,\"gridY\":60,\"type\":0},{\"gridX\":260,\"gridY\":13,\"type\":0},{\"gridX\":263,\"gridY\":21,\"type\":0},{\"gridX\":266,\"gridY\":30,\"type\":0},{\"gridX\":272,\"gridY\":40,\"type\":0},{\"gridX\":276,\"gridY\":48,\"type\":0},{\"gridX\":292,\"gridY\":40,\"type\":0},{\"gridX\":288,\"gridY\":48,\"type\":0},{\"gridX\":300,\"gridY\":40,\"type\":0},{\"gridX\":304,\"gridY\":49,\"type\":0},{\"gridX\":311,\"gridY\":58,\"type\":0},{\"gridX\":359,\"gridY\":53,\"type\":0},{\"gridX\":337,\"gridY\":69,\"type\":0},{\"gridX\":347,\"gridY\":66,\"type\":0},{\"gridX\":387,\"gridY\":76,\"type\":0},{\"gridX\":399,\"gridY\":75,\"type\":0},{\"gridX\":408,\"gridY\":71,\"type\":0},{\"gridX\":492,\"gridY\":104,\"type\":0},{\"gridX\":480,\"gridY\":104,\"type\":0},{\"gridX\":504,\"gridY\":104,\"type\":0},{\"gridX\":438,\"gridY\":100,\"type\":0},{\"gridX\":439,\"gridY\":108,\"type\":0},{\"gridX\":441,\"gridY\":116,\"type\":0},{\"gridX\":516,\"gridY\":104,\"type\":0},{\"gridX\":528,\"" << "gridY\":104,\"type\":0},{\"gridX\":528,\"gridY\":104,\"type\":0},{\"gridX\":540,\"gridY\":104,\"type\":0},{\"gridX\":552,\"gridY\":104,\"type\":0},{\"gridX\":564,\"gridY\":104,\"type\":0},{\"gridX\":576,\"gridY\":104,\"type\":0},{\"gridX\":588,\"gridY\":104,\"type\":0},{\"gridX\":600,\"gridY\":104,\"type\":0},{\"gridX\":624,\"gridY\":104,\"type\":0},{\"gridX\":612,\"gridY\":104,\"type\":0},{\"gridX\":636,\"gridY\":104,\"type\":0},{\"gridX\":648,\"gridY\":104,\"type\":0},{\"gridX\":672,\"gridY\":104,\"type\":0},{\"gridX\":672,\"gridY\":104,\"type\":0},{\"gridX\":660,\"gridY\":104,\"type\":0},{\"gridX\":972,\"gridY\":192,\"type\":1},{\"gridX\":820,\"gridY\":28,\"type\":0},{\"gridX\":820,\"gridY\":40,\"type\":0},{\"gridX\":820,\"gridY\":52,\"type\":0},{\"gridX\":820,\"gridY\":64,\"type\":0},{\"gridX\":820,\"gridY\":76,\"type\":0},{\"gridX\":820,\"gridY\":100,\"type\":0},{\"gridX\":789,\"gridY\":98,\"type\":1},{\"gridX\":1076,\"gridY\":100,\"type\":0},{\"gridX\":1045,\"gridY\":57,\"type\":0},{\"gridX\":956,\"gridY\":16,\"type\":0},{\"gridX\":980,\"gridY\":16,\"type\":0},{\"gridX\":964,\"gridY\":16,\"type\":0},{\"gridX\":972,\"gridY\":16,\"type\":0},{\"gridX\":820,\"gridY\":108,\"type\":0},{\"gridX\":820,\"gridY\":16,\"type\":0},{\"gridX\":832,\"gridY\":12,\"type\":0},{\"gridX\":984,\"gridY\":24,\"type\":0},{\"gridX\":988,\"gridY\":32,\"type\":0},{\"gridX\":992,\"gridY\":40,\"type\":0},{\"gridX\":1000,\"gridY\":48,\"type\":0},{\"gridX\":1012,\"gridY\":52,\"type\":0},{\"gridX\":1045,\"gridY\":52,\"type\":0},{\"gridX\":1112,\"gridY\":68,\"type\":0},{\"gridX\":1120,\"gridY\":68,\"type\":0},{\"gridX\":1104,\"gridY\":68,\"type\":0},{\"gridX\":1128,\"gridY\":68,\"type\":0},{\"gridX\":1096,\"gridY\":68,\"type\":0},{\"gridX\":1167,\"gridY\":51,\"type\":0},{\"gridX\":1159,\"gridY\":50,\"type\":0},{\"gridX\":1165,\"gridY\":57,\"type\":0},{\"gridX\":1215,\"gridY\":52,\"type\":0},{\"gridX\":1220,\"gridY\":59,\"type\":0},{\"gridX\":1223,\"gridY\":52,\"type\":0},{\"gridX\":1260,\"gridY\":47,\"type\":0},{\"gridX\":1256,\"gridY\":41,\"type\":0},{\"gridX\":1263,\"gridY\":41,\"type\":0},{\"gridX\":1354,\"gridY\":60,\"type\":0},{\"gridX\":1332,\"gridY\":42,\"type\":0},{\"gridX\":1337,\"gridY\":49,\"type\":0},{\"gridX\":1340,\"gridY\":42,\"type\":0},{\"gridX\":1452,\"gridY\":132,\"type\":0},{\"gridX\":1460,\"gridY\":132,\"type\":0},{\"gridX\":1444,\"gridY\":132,\"type\":0},{\"gridX\":1394,\"gridY\":60,\"type\":0},{\"gridX\":1395,\"gridY\":69,\"type\":0},{\"gridX\":1396,\"gridY\":78,\"type\":0},{\"gridX\":710,\"gridY\":102,\"type\":3},{\"gridX\":672,\"gridY\":160,\"type\":0},{\"gridX\":636,\"gridY\":172,\"type\":0},{\"gridX\":664,\"gridY\":164,\"type\":0},{\"gridX\":688,\"gridY\":164,\"type\":0},{\"gridX\":680,\"gridY\":160,\"type\":0},{\"gridX\":712,\"gridY\":172,\"type\":0},{\"gridX\":720,\"gridY\":172,\"type\":0},{\"gridX\":628,\"gridY\":172,\"type\":0},{\"gridX\":620,\"gridY\":172,\"type\":0},{\"gridX\":612,\"gridY\":172,\"type\":0},{\"gridX\":852,\"gridY\":144,\"type\":0},{\"gridX\":876,\"gridY\":144,\"type\":0},{\"gridX\":860,\"gridY\":144,\"type\":0},{\"gridX\":868,\"gridY\":144,\"type\":0},{\"gridX\":888,\"gridY\":152,\"type\":0},{\"gridX\":892,\"gridY\":160,\"type\":0},{\"gridX\":884,\"gridY\":144,\"type\":0},{\"gridX\":1076,\"gridY\":77,\"type\":0},{\"gridX\":1076,\"gridY\":69,\"type\":0},{\"gridX\":719,\"gridY\":102,\"type\":3}],\"jons\":[{\"gridX\":18,\"gridY\":105}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":784,\"gridY\":92,\"type\":3},{\"gridX\":759,\"gridY\":92,\"type\":2},{\"gridX\":845,\"gridY\":92,\"type\":1},{\"gridX\":68,\"gridY\":92,\"type\":3},{\"gridX\":297,\"gridY\":92,\"type\":2},{\"gridX\":358,\"gridY\":85,\"type\":0},{\"gridX\":978,\"gridY\":92,\"type\":3},{\"gridX\":1246,\"gridY\":85,\"type\":0},{\"gridX\":1343,\"gridY\":92,\"type\":3},{\"gridX\":681,\"gridY\":92,\"type\":1}],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level19(text.c_str());
}

void Chapter1Level19::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level19 * Chapter1Level19::getInstance()
{
    return s_pInstance;
}

Chapter1Level19::Chapter1Level19(const char* json) : Level(json)
{
    // Empty
}

/// Chapter 1 Level 20 ///

Chapter1Level20 * Chapter1Level20::s_pInstance = nullptr;

void Chapter1Level20::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":20,\"midgrounds\":[{\"gridX\":1816,\"gridY\":96,\"type\":2},{\"gridX\":1872,\"gridY\":96,\"type\":2},{\"gridX\":2048,\"gridY\":96,\"type\":0},{\"gridX\":54,\"gridY\":96,\"type\":2},{\"gridX\":456,\"gridY\":96,\"type\":2},{\"gridX\":732,\"gridY\":96,\"type\":2},{\"gridX\":732,\"gridY\":0,\"type\":3},{\"gridX\":664,\"gridY\":0,\"type\":3},{\"gridX\":676,\"gridY\":0,\"type\":3},{\"gridX\":484,\"gridY\":0,\"type\":3},{\"gridX\":748,\"gridY\":0,\"type\":4},{\"gridX\":756,\"gridY\":0,\"type\":5},{\"gridX\":652,\"gridY\":96,\"type\":2},{\"gridX\":1168,\"gridY\":96,\"type\":2},{\"gridX\":1221,\"gridY\":0,\"type\":3},{\"gridX\":1044,\"gridY\":0,\"type\":3},{\"gridX\":1124,\"gridY\":0,\"type\":3},{\"gridX\":978,\"gridY\":0,\"type\":3},{\"gridX\":244,\"gridY\":152,\"type\":7},{\"gridX\":268,\"gridY\":132,\"type\":7},{\"gridX\":244,\"gridY\":140,\"type\":7},{\"gridX\":244,\"gridY\":128,\"type\":7},{\"gridX\":244,\"gridY\":116,\"type\":7},{\"gridX\":244,\"gridY\":104,\"type\":7},{\"gridX\":244,\"gridY\":92,\"type\":7},{\"gridX\":284,\"gridY\":132,\"type\":7},{\"gridX\":284,\"gridY\":120,\"type\":7},{\"gridX\":284,\"gridY\":108,\"type\":7},{\"gridX\":284,\"gridY\":96,\"type\":7},{\"gridX\":268,\"gridY\":96,\"type\":7},{\"gridX\":268,\"gridY\":108,\"type\":7},{\"gridX\":268,\"gridY\":120,\"type\":7},{\"gridX\":304,\"gridY\":152,\"type\":7},{\"gridX\":340,\"gridY\":116,\"type\":7},{\"gridX\":340,\"gridY\":152,\"type\":7},{\"gridX\":340,\"gridY\":128,\"type\":7},{\"gridX\":304,\"gridY\":92,\"type\":7},{\"gridX\":340,\"gridY\":140,\"type\":7},{\"gridX\":304,\"gridY\":104,\"type\":7},{\"gridX\":304,\"gridY\":116,\"type\":7},{\"gridX\":304,\"gridY\":128,\"type\":7},{\"gridX\":304,\"gridY\":140,\"type\":7},{\"gridX\":340,\"gridY\":104,\"type\":7},{\"gridX\":340,\"gridY\":92,\"type\":7},{\"gridX\":386,\"gridY\":96,\"type\":13},{\"gridX\":583,\"gridY\":96,\"type\":14},{\"gridX\":554,\"gridY\":96,\"type\":13},{\"gridX\":464,\"gridY\":96,\"type\":11},{\"gridX\":485,\"gridY\":96,\"type\":6},{\"gridX\":616,\"gridY\":96,\"type\":6},{\"gridX\":1324,\"gridY\":0,\"type\":5},{\"gridX\":1260,\"gridY\":0,\"type\":4},{\"gridX\":1404,\"gridY\":0,\"type\":4},{\"gridX\":1452,\"gridY\":0,\"type\":5},{\"gridX\":1116,\"gridY\":192,\"type\":7},{\"gridX\":1116,\"gridY\":180,\"type\":7},{\"gridX\":1116,\"gridY\":168,\"type\":7},{\"gridX\":1116,\"gridY\":156,\"type\":7},{\"gridX\":1116,\"gridY\":144,\"type\":7},{\"gridX\":1116,\"gridY\":132,\"type\":7},{\"gridX\":1116,\"gridY\":120,\"type\":7},{\"gridX\":1116,\"gridY\":108,\"type\":7},{\"gridX\":1116,\"gridY\":96,\"type\":7},{\"gridX\":1084,\"gridY\":236,\"type\":7},{\"gridX\":1092,\"gridY\":228,\"type\":7},{\"gridX\":1100,\"gridY\":220,\"type\":7},{\"gridX\":1108,\"gridY\":212,\"type\":7},{\"gridX\":1116,\"gridY\":204,\"type\":7},{\"gridX\":852,\"gridY\":140,\"type\":7},{\"gridX\":852,\"gridY\":128,\"type\":7},{\"gridX\":852,\"gridY\":116,\"type\":7},{\"gridX\":852,\"gridY\":104,\"type\":7},{\"gridX\":852,\"gridY\":92,\"type\":7},{\"gridX\":924,\"gridY\":240,\"type\":7},{\"gridX\":932,\"gridY\":240,\"type\":7},{\"gridX\":940,\"gridY\":240,\"type\":7},{\"gridX\":948,\"gridY\":240,\"type\":7},{\"gridX\":876,\"gridY\":192,\"type\":7},{\"gridX\":956,\"gridY\":240,\"type\":7},{\"gridX\":876,\"gridY\":180,\"type\":7},{\"gridX\":876,\"gridY\":168,\"type\":7},{\"gridX\":876,\"gridY\":156,\"type\":7},{\"gridX\":876,\"gridY\":144,\"type\":7},{\"gridX\":876,\"gridY\":132,\"type\":7},{\"gridX\":876,\"gridY\":120,\"type\":7},{\"gridX\":876,\"gridY\":108,\"type\":7},{\"gridX\":876,\"gridY\":96,\"type\":7},{\"gridX\":1020,\"gridY\":240,\"type\":7},{\"gridX\":1012,\"gridY\":240,\"type\":7},{\"gridX\":1004,\"gridY\":240,\"type\":7},{\"gridX\":996,\"gridY\":240,\"type\":7},{\"gridX\":988,\"gridY\":240,\"type\":7},{\"gridX\":980,\"gridY\":240,\"type\":7},{\"gridX\":972,\"gridY\":240,\"type\":7},{\"gridX\":964,\"gridY\":240,\"type\":7},{\"gridX\":1060,\"gridY\":240,\"type\":7},{\"gridX\":1052,\"gridY\":240,\"type\":7},{\"gridX\":1044,\"gridY\":240,\"type\":7},{\"gridX\":1036,\"gridY\":240,\"type\":7},{\"gridX\":1028,\"gridY\":240,\"type\":7},{\"gridX\":1076,\"gridY\":240,\"type\":7},{\"gridX\":1068,\"gridY\":240,\"type\":7},{\"gridX\":1632,\"gridY\":96,\"type\":12},{\"gridX\":1728,\"gridY\":96,\"type\":12},{\"gridX\":1824,\"gridY\":96,\"type\":12},{\"gridX\":1920,\"gridY\":96,\"type\":12},{\"gridX\":1536,\"gridY\":96,\"type\":12},{\"gridX\":110,\"gridY\":96,\"type\":13},{\"gridX\":30,\"gridY\":96,\"type\":13},{\"gridX\":85,\"gridY\":96,\"type\":15},{\"gridX\":323,\"gridY\":96,\"type\":16},{\"gridX\":292,\"gridY\":96,\"type\":15},{\"gridX\":523,\"gridY\":96,\"type\":15},{\"gridX\":413,\"gridY\":96,\"type\":16},{\"gridX\":775,\"gridY\":96,\"type\":11},{\"gridX\":1315,\"gridY\":96,\"type\":15},{\"gridX\":1340,\"gridY\":96,\"type\":16},{\"gridX\":892,\"gridY\":0,\"type\":5},{\"gridX\":916,\"gridY\":96,\"type\":7},{\"gridX\":916,\"gridY\":108,\"type\":7},{\"gridX\":980,\"gridY\":192,\"type\":7},{\"gridX\":980,\"gridY\":168,\"type\":7},{\"gridX\":980,\"gridY\":156,\"type\":7},{\"gridX\":980,\"gridY\":144,\"type\":7},{\"gridX\":980,\"gridY\":132,\"type\":7},{\"gridX\":980,\"gridY\":96,\"type\":7},{\"gridX\":980,\"gridY\":108,\"type\":7},{\"gridX\":980,\"gridY\":120,\"type\":7},{\"gridX\":916,\"gridY\":192,\"type\":7},{\"gridX\":916,\"gridY\":180,\"type\":7},{\"gridX\":916,\"gridY\":168,\"type\":7},{\"gridX\":916,\"gridY\":156,\"type\":7},{\"gridX\":916,\"gridY\":144,\"type\":7},{\"gridX\":916,\"gridY\":132,\"type\":7},{\"gridX\":916,\"gridY\":120,\"type\":7},{\"gridX\":980,\"gridY\":180,\"type\":7},{\"gridX\":1048,\"gridY\":120,\"type\":7},{\"gridX\":1048,\"gridY\":108,\"type\":7},{\"gridX\":1048,\"gridY\":96,\"type\":7},{\"gridX\":980,\"gridY\":216,\"type\":7},{\"gridX\":980,\"gridY\":204,\"type\":7},{\"gridX\":980,\"gridY\":228,\"type\":7},{\"gridX\":1048,\"gridY\":192,\"type\":7},{\"gridX\":1048,\"gridY\":180,\"type\":7},{\"gridX\":1048,\"gridY\":168,\"type\":7},{\"gridX\":1048,\"gridY\":156,\"type\":7},{\"gridX\":1048,\"gridY\":144,\"type\":7},{\"gridX\":1048,\"gridY\":132,\"type\":7},{\"gridX\":1100,\"gridY\":164,\"type\":7},{\"gridX\":1100,\"gridY\":176,\"type\":7},{\"gridX\":1100,\"gridY\":188,\"type\":7},{\"gridX\":540,\"gridY\":24,\"type\":7},{\"gridX\":540,\"gridY\":12,\"type\":7},{\"gridX\":540,\"gridY\":0,\"type\":7},{\"gridX\":524,\"gridY\":36,\"type\":7},{\"gridX\":524,\"gridY\":24,\"type\":7},{\"gridX\":524,\"gridY\":12,\"type\":7},{\"gridX\":524,\"gridY\":0,\"type\":7},{\"gridX\":540,\"gridY\":36,\"type\":7},{\"gridX\":576,\"gridY\":60,\"type\":7},{\"gridX\":628,\"gridY\":60,\"type\":7},{\"gridX\":628,\"gridY\":0,\"type\":7},{\"gridX\":564,\"gridY\":0,\"type\":7},{\"gridX\":564,\"gridY\":12,\"type\":7},{\"gridX\":564,\"gridY\":24,\"type\":7},{\"gridX\":564,\"gridY\":36,\"type\":7},{\"gridX\":564,\"gridY\":48,\"type\":7},{\"gridX\":564,\"gridY\":60,\"type\":7},{\"gridX\":564,\"gridY\":72,\"type\":7},{\"gridX\":572,\"gridY\":72,\"type\":7},{\"gridX\":628,\"gridY\":12,\"type\":7},{\"gridX\":628,\"gridY\":24,\"type\":7},{\"gridX\":628,\"gridY\":36,\"type\":7},{\"gridX\":628,\"gridY\":48,\"type\":7},{\"gridX\":628,\"gridY\":72,\"type\":7},{\"gridX\":620,\"gridY\":72,\"type\":7},{\"gridX\":612,\"gridY\":72,\"type\":7},{\"gridX\":604,\"gridY\":72,\"type\":7},{\"gridX\":596,\"gridY\":72,\"type\":7},{\"gridX\":588,\"gridY\":72,\"type\":7},{\"gridX\":580,\"gridY\":72,\"type\":7},{\"gridX\":996,\"gridY\":0,\"type\":4},{\"gridX\":959,\"gridY\":0,\"type\":5},{\"gridX\":1134,\"gridY\":0,\"type\":5},{\"gridX\":1123,\"gridY\":0,\"type\":4},{\"gridX\":1336,\"gridY\":96,\"type\":17},{\"gridX\":1322,\"gridY\":96,\"type\":18},{\"gridX\":1345,\"gridY\":102,\"type\":18},{\"gridX\":1331,\"gridY\":110,\"type\":18},{\"gridX\":1344,\"gridY\":121,\"type\":17},{\"gridX\":1336,\"gridY\":134,\"type\":18},{\"gridX\":1355,\"gridY\":137,\"type\":17},{\"gridX\":1362,\"gridY\":157,\"type\":17},{\"gridX\":1370,\"gridY\":191,\"type\":17},{\"gridX\":1373,\"gridY\":17" << "2,\"type\":17},{\"gridX\":1394,\"gridY\":113,\"type\":17},{\"gridX\":1395,\"gridY\":96,\"type\":18},{\"gridX\":1385,\"gridY\":130,\"type\":17},{\"gridX\":1374,\"gridY\":149,\"type\":17},{\"gridX\":1349,\"gridY\":144,\"type\":18},{\"gridX\":1354,\"gridY\":170,\"type\":18},{\"gridX\":1382,\"gridY\":185,\"type\":18},{\"gridX\":1388,\"gridY\":145,\"type\":18},{\"gridX\":1383,\"gridY\":113,\"type\":18},{\"gridX\":1374,\"gridY\":136,\"type\":18},{\"gridX\":1400,\"gridY\":125,\"type\":18},{\"gridX\":1334,\"gridY\":194,\"type\":18},{\"gridX\":1608,\"gridY\":128,\"type\":19},{\"gridX\":1752,\"gridY\":128,\"type\":19},{\"gridX\":1752,\"gridY\":144,\"type\":19},{\"gridX\":1664,\"gridY\":144,\"type\":19},{\"gridX\":1664,\"gridY\":128,\"type\":19},{\"gridX\":1608,\"gridY\":144,\"type\":19},{\"gridX\":1496,\"gridY\":176,\"type\":19},{\"gridX\":1480,\"gridY\":176,\"type\":19},{\"gridX\":1464,\"gridY\":192,\"type\":19},{\"gridX\":1464,\"gridY\":176,\"type\":19},{\"gridX\":1448,\"gridY\":192,\"type\":19},{\"gridX\":1448,\"gridY\":176,\"type\":19},{\"gridX\":1448,\"gridY\":96,\"type\":7},{\"gridX\":1504,\"gridY\":96,\"type\":7},{\"gridX\":1472,\"gridY\":96,\"type\":7},{\"gridX\":1480,\"gridY\":96,\"type\":7},{\"gridX\":1448,\"gridY\":108,\"type\":7},{\"gridX\":1472,\"gridY\":108,\"type\":7},{\"gridX\":1480,\"gridY\":108,\"type\":7},{\"gridX\":1504,\"gridY\":108,\"type\":7}],\"grounds\":[{\"gridX\":512,\"gridY\":0,\"type\":0},{\"gridX\":768,\"gridY\":0,\"type\":0},{\"gridX\":1292,\"gridY\":0,\"type\":0},{\"gridX\":1412,\"gridY\":0,\"type\":0},{\"gridX\":1300,\"gridY\":0,\"type\":1},{\"gridX\":1420,\"gridY\":0,\"type\":1},{\"gridX\":776,\"gridY\":0,\"type\":2},{\"gridX\":520,\"gridY\":0,\"type\":3},{\"gridX\":648,\"gridY\":0,\"type\":4},{\"gridX\":1332,\"gridY\":0,\"type\":4},{\"gridX\":1452,\"gridY\":0,\"type\":4},{\"gridX\":840,\"gridY\":0,\"type\":4},{\"gridX\":1060,\"gridY\":20,\"type\":5},{\"gridX\":1360,\"gridY\":20,\"type\":5},{\"gridX\":1476,\"gridY\":20,\"type\":5},{\"gridX\":1400,\"gridY\":20,\"type\":6},{\"gridX\":1516,\"gridY\":20,\"type\":7},{\"gridX\":1068,\"gridY\":20,\"type\":8},{\"gridX\":1196,\"gridY\":20,\"type\":9},{\"gridX\":1432,\"gridY\":20,\"type\":9},{\"gridX\":256,\"gridY\":50,\"type\":10},{\"gridX\":736,\"gridY\":50,\"type\":10},{\"gridX\":1312,\"gridY\":50,\"type\":10},{\"gridX\":1400,\"gridY\":50,\"type\":10},{\"gridX\":842,\"gridY\":50,\"type\":10},{\"gridX\":744,\"gridY\":50,\"type\":11},{\"gridX\":1320,\"gridY\":50,\"type\":11},{\"gridX\":1440,\"gridY\":50,\"type\":11},{\"gridX\":850,\"gridY\":50,\"type\":11},{\"gridX\":392,\"gridY\":50,\"type\":12},{\"gridX\":264,\"gridY\":50,\"type\":13},{\"gridX\":1472,\"gridY\":50,\"type\":14},{\"gridX\":456,\"gridY\":50,\"type\":14},{\"gridX\":1352,\"gridY\":50,\"type\":14},{\"gridX\":876,\"gridY\":48,\"type\":15},{\"gridX\":1111,\"gridY\":48,\"type\":15},{\"gridX\":732,\"gridY\":48,\"type\":15},{\"gridX\":740,\"gridY\":48,\"type\":16},{\"gridX\":1119,\"gridY\":48,\"type\":16},{\"gridX\":884,\"gridY\":48,\"type\":17},{\"gridX\":948,\"gridY\":48,\"type\":17},{\"gridX\":772,\"gridY\":48,\"type\":19},{\"gridX\":1151,\"gridY\":48,\"type\":19},{\"gridX\":1012,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":1288,\"gridY\":90,\"type\":20},{\"gridX\":1409,\"gridY\":90,\"type\":20},{\"gridX\":776,\"gridY\":90,\"type\":21},{\"gridX\":1480,\"gridY\":90,\"type\":21},{\"gridX\":840,\"gridY\":90,\"type\":22},{\"gridX\":1416,\"gridY\":90,\"type\":22},{\"gridX\":1160,\"gridY\":90,\"type\":22},{\"gridX\":1296,\"gridY\":90,\"type\":22},{\"gridX\":520,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":392,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":648,\"gridY\":90,\"type\":23},{\"gridX\":904,\"gridY\":90,\"type\":23},{\"gridX\":1032,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":1224,\"gridY\":90,\"type\":24},{\"gridX\":1360,\"gridY\":90,\"type\":24},{\"gridX\":1536,\"gridY\":0,\"type\":25},{\"gridX\":1544,\"gridY\":0,\"type\":26},{\"gridX\":2024,\"gridY\":0,\"type\":27},{\"gridX\":1576,\"gridY\":0,\"type\":27},{\"gridX\":1640,\"gridY\":0,\"type\":28},{\"gridX\":1768,\"gridY\":0,\"type\":28},{\"gridX\":1896,\"gridY\":0,\"type\":28},{\"gridX\":2088,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":808,\"gridY\":88,\"type\":0},{\"gridX\":1368,\"gridY\":0,\"type\":3},{\"gridX\":1484,\"gridY\":0,\"type\":3},{\"gridX\":1408,\"gridY\":48,\"type\":2},{\"gridX\":1512,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":332,\"gridY\":80,\"type\":3},{\"gridX\":1060,\"gridY\":73,\"type\":0},{\"gridX\":1076,\"gridY\":73,\"type\":1},{\"gridX\":1092,\"gridY\":73,\"type\":2},{\"gridX\":1328,\"gridY\":32,\"type\":4},{\"gridX\":1440,\"gridY\":32,\"type\":4},{\"gridX\":856,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":1808,\"gridY\":96,\"type\":12},{\"gridX\":244,\"gridY\":148,\"type\":34},{\"gridX\":244,\"gridY\":132,\"type\":34},{\"gridX\":360,\"gridY\":84,\"type\":30},{\"gridX\":360,\"gridY\":55,\"type\":27},{\"gridX\":524,\"gridY\":44,\"type\":34},{\"gridX\":540,\"gridY\":40,\"type\":34},{\"gridX\":564,\"gridY\":76,\"type\":34},{\"gridX\":576,\"gridY\":64,\"type\":34},{\"gridX\":588,\"gridY\":76,\"type\":34},{\"gridX\":608,\"gridY\":16,\"type\":34},{\"gridX\":628,\"gridY\":76,\"type\":34},{\"gridX\":804,\"gridY\":7,\"type\":21},{\"gridX\":568,\"gridY\":7,\"type\":26},{\"gridX\":1248,\"gridY\":48,\"type\":4},{\"gridX\":1244,\"gridY\":48,\"type\":3},{\"gridX\":1262,\"gridY\":48,\"type\":4},{\"gridX\":1276,\"gridY\":48,\"type\":5},{\"gridX\":1524,\"gridY\":27,\"type\":20},{\"gridX\":1416,\"gridY\":27,\"type\":41},{\"gridX\":1116,\"gridY\":100,\"type\":34},{\"gridX\":1116,\"gridY\":116,\"type\":34},{\"gridX\":1116,\"gridY\":132,\"type\":34},{\"gridX\":1116,\"gridY\":164,\"type\":34},{\"gridX\":1116,\"gridY\":180,\"type\":34},{\"gridX\":1116,\"gridY\":196,\"type\":34},{\"gridX\":284,\"gridY\":112,\"type\":48},{\"gridX\":280,\"gridY\":112,\"type\":47},{\"gridX\":364,\"gridY\":112,\"type\":49},{\"gridX\":300,\"gridY\":112,\"type\":48},{\"gridX\":240,\"gridY\":160,\"type\":46},{\"gridX\":300,\"gridY\":160,\"type\":46},{\"gridX\":336,\"gridY\":160,\"type\":46},{\"gridX\":244,\"gridY\":116,\"type\":34},{\"gridX\":316,\"gridY\":112,\"type\":48},{\"gridX\":204,\"gridY\":96,\"type\":55},{\"gridX\":196,\"gridY\":96,\"type\":53},{\"gridX\":228,\"gridY\":96,\"type\":54},{\"gridX\":240,\"gridY\":96,\"type\":56},{\"gridX\":348,\"gridY\":112,\"type\":48},{\"gridX\":332,\"gridY\":112,\"type\":48},{\"gridX\":264,\"gridY\":140,\"type\":48},{\"gridX\":280,\"gridY\":140,\"type\":48},{\"gridX\":296,\"gridY\":140,\"type\":49},{\"gridX\":260,\"gridY\":140,\"type\":47},{\"gridX\":264,\"gridY\":145,\"type\":24},{\"gridX\":294,\"gridY\":145,\"type\":22},{\"gridX\":352,\"gridY\":96,\"type\":53},{\"gridX\":480,\"gridY\":96,\"type\":55},{\"gridX\":456,\"gridY\":96,\"type\":55},{\"gridX\":432,\"gridY\":96,\"type\":55},{\"gridX\":408,\"gridY\":96,\"type\":55},{\"gridX\":384,\"gridY\":96,\"type\":55},{\"gridX\":360,\"gridY\":96,\"type\":55},{\"gridX\":600,\"gridY\":96,\"type\":55},{\"gridX\":576,\"gridY\":96,\"type\":55},{\"gridX\":552,\"gridY\":96,\"type\":55},{\"gridX\":528,\"gridY\":96,\"type\":55},{\"gridX\":504,\"gridY\":96,\"type\":55},{\"gridX\":672,\"gridY\":96,\"type\":55},{\"gridX\":696,\"gridY\":96,\"type\":55},{\"gridX\":648,\"gridY\":96,\"type\":55},{\"gridX\":624,\"gridY\":96,\"type\":55},{\"gridX\":712,\"gridY\":96,\"type\":58},{\"gridX\":699,\"gridY\":100,\"type\":60},{\"gridX\":721,\"gridY\":100,\"type\":60},{\"gridX\":712,\"gridY\":96,\"type\":55},{\"gridX\":736,\"gridY\":96,\"type\":55},{\"gridX\":760,\"gridY\":96,\"type\":56},{\"gridX\":903,\"gridY\":6,\"type\":62},{\"gridX\":989,\"gridY\":7,\"type\":62},{\"gridX\":792,\"gridY\":128,\"type\":62},{\"gridX\":1576,\"gridY\":96,\"type\":53},{\"gridX\":1728,\"gridY\":96,\"type\":55},{\"gridX\":1704,\"gridY\":96,\"type\":55},{\"gridX\":1680,\"gridY\":96,\"type\"" << ":55},{\"gridX\":1632,\"gridY\":96,\"type\":55},{\"gridX\":1584,\"gridY\":96,\"type\":55},{\"gridX\":1776,\"gridY\":96,\"type\":56},{\"gridX\":924,\"gridY\":244,\"type\":34},{\"gridX\":876,\"gridY\":196,\"type\":34},{\"gridX\":876,\"gridY\":180,\"type\":34},{\"gridX\":876,\"gridY\":164,\"type\":34},{\"gridX\":876,\"gridY\":148,\"type\":34},{\"gridX\":876,\"gridY\":132,\"type\":34},{\"gridX\":876,\"gridY\":116,\"type\":34},{\"gridX\":876,\"gridY\":100,\"type\":34},{\"gridX\":1020,\"gridY\":244,\"type\":34},{\"gridX\":1004,\"gridY\":244,\"type\":34},{\"gridX\":988,\"gridY\":244,\"type\":34},{\"gridX\":972,\"gridY\":244,\"type\":34},{\"gridX\":956,\"gridY\":244,\"type\":34},{\"gridX\":940,\"gridY\":244,\"type\":34},{\"gridX\":1068,\"gridY\":244,\"type\":34},{\"gridX\":1052,\"gridY\":244,\"type\":34},{\"gridX\":1036,\"gridY\":244,\"type\":34},{\"gridX\":1084,\"gridY\":240,\"type\":34},{\"gridX\":1100,\"gridY\":224,\"type\":34},{\"gridX\":1092,\"gridY\":232,\"type\":34},{\"gridX\":1108,\"gridY\":216,\"type\":34},{\"gridX\":848,\"gridY\":148,\"type\":46},{\"gridX\":892,\"gridY\":200,\"type\":48},{\"gridX\":888,\"gridY\":200,\"type\":47},{\"gridX\":908,\"gridY\":200,\"type\":48},{\"gridX\":924,\"gridY\":200,\"type\":48},{\"gridX\":996,\"gridY\":184,\"type\":48},{\"gridX\":980,\"gridY\":184,\"type\":48},{\"gridX\":940,\"gridY\":200,\"type\":48},{\"gridX\":1012,\"gridY\":184,\"type\":48},{\"gridX\":1028,\"gridY\":184,\"type\":49},{\"gridX\":960,\"gridY\":184,\"type\":47},{\"gridX\":956,\"gridY\":200,\"type\":49},{\"gridX\":983,\"gridY\":189,\"type\":24},{\"gridX\":1015,\"gridY\":189,\"type\":23},{\"gridX\":1096,\"gridY\":192,\"type\":46},{\"gridX\":1080,\"gridY\":164,\"type\":48},{\"gridX\":1112,\"gridY\":164,\"type\":49},{\"gridX\":968,\"gridY\":164,\"type\":48},{\"gridX\":1000,\"gridY\":164,\"type\":48},{\"gridX\":1016,\"gridY\":164,\"type\":48},{\"gridX\":1032,\"gridY\":164,\"type\":48},{\"gridX\":1048,\"gridY\":164,\"type\":48},{\"gridX\":1064,\"gridY\":164,\"type\":48},{\"gridX\":984,\"gridY\":164,\"type\":48},{\"gridX\":902,\"gridY\":140,\"type\":48},{\"gridX\":952,\"gridY\":164,\"type\":48},{\"gridX\":948,\"gridY\":164,\"type\":47},{\"gridX\":898,\"gridY\":140,\"type\":47},{\"gridX\":1096,\"gridY\":164,\"type\":48},{\"gridX\":934,\"gridY\":140,\"type\":49},{\"gridX\":980,\"gridY\":140,\"type\":48},{\"gridX\":976,\"gridY\":140,\"type\":47},{\"gridX\":996,\"gridY\":140,\"type\":49},{\"gridX\":948,\"gridY\":116,\"type\":48},{\"gridX\":964,\"gridY\":116,\"type\":48},{\"gridX\":932,\"gridY\":116,\"type\":48},{\"gridX\":980,\"gridY\":116,\"type\":49},{\"gridX\":912,\"gridY\":116,\"type\":47},{\"gridX\":936,\"gridY\":121,\"type\":24},{\"gridX\":967,\"gridY\":121,\"type\":23},{\"gridX\":1128,\"gridY\":96,\"type\":53},{\"gridX\":1304,\"gridY\":96,\"type\":55},{\"gridX\":1136,\"gridY\":96,\"type\":55},{\"gridX\":1328,\"gridY\":96,\"type\":55},{\"gridX\":1608,\"gridY\":96,\"type\":52},{\"gridX\":1608,\"gridY\":112,\"type\":51},{\"gridX\":1144,\"gridY\":112,\"type\":62},{\"gridX\":1258,\"gridY\":53,\"type\":41},{\"gridX\":1352,\"gridY\":96,\"type\":56},{\"gridX\":1296,\"gridY\":96,\"type\":53},{\"gridX\":1664,\"gridY\":96,\"type\":52},{\"gridX\":1664,\"gridY\":112,\"type\":51},{\"gridX\":1752,\"gridY\":96,\"type\":52},{\"gridX\":1752,\"gridY\":112,\"type\":51},{\"gridX\":964,\"gridY\":184,\"type\":48},{\"gridX\":965,\"gridY\":189,\"type\":23},{\"gridX\":1028,\"gridY\":200,\"type\":47},{\"gridX\":1048,\"gridY\":200,\"type\":48},{\"gridX\":1032,\"gridY\":200,\"type\":48},{\"gridX\":1064,\"gridY\":200,\"type\":49},{\"gridX\":916,\"gridY\":116,\"type\":48},{\"gridX\":918,\"gridY\":140,\"type\":48},{\"gridX\":1198,\"gridY\":96,\"type\":20},{\"gridX\":1175,\"gridY\":95,\"type\":35},{\"gridX\":1224,\"gridY\":95,\"type\":35},{\"gridX\":1160,\"gridY\":96,\"type\":55},{\"gridX\":1184,\"gridY\":96,\"type\":56},{\"gridX\":1216,\"gridY\":96,\"type\":53},{\"gridX\":1224,\"gridY\":96,\"type\":56},{\"gridX\":1399,\"gridY\":102,\"type\":60},{\"gridX\":1320,\"gridY\":210,\"type\":60},{\"gridX\":1342,\"gridY\":210,\"type\":60},{\"gridX\":1366,\"gridY\":210,\"type\":60},{\"gridX\":1390,\"gridY\":210,\"type\":60},{\"gridX\":1334,\"gridY\":205,\"type\":59},{\"gridX\":1382,\"gridY\":208,\"type\":59},{\"gridX\":1356,\"gridY\":206,\"type\":59},{\"gridX\":1349,\"gridY\":193,\"type\":63},{\"gridX\":1356,\"gridY\":180,\"type\":59},{\"gridX\":1390,\"gridY\":195,\"type\":59},{\"gridX\":1464,\"gridY\":112,\"type\":52},{\"gridX\":1448,\"gridY\":112,\"type\":52},{\"gridX\":1448,\"gridY\":128,\"type\":52},{\"gridX\":1480,\"gridY\":112,\"type\":52},{\"gridX\":1496,\"gridY\":112,\"type\":52},{\"gridX\":1448,\"gridY\":144,\"type\":52},{\"gridX\":1448,\"gridY\":160,\"type\":51},{\"gridX\":1464,\"gridY\":160,\"type\":51},{\"gridX\":1480,\"gridY\":160,\"type\":51},{\"gridX\":1496,\"gridY\":160,\"type\":51},{\"gridX\":1496,\"gridY\":128,\"type\":52},{\"gridX\":1496,\"gridY\":144,\"type\":52},{\"gridX\":1448,\"gridY\":208,\"type\":52},{\"gridX\":1464,\"gridY\":208,\"type\":52},{\"gridX\":1480,\"gridY\":192,\"type\":52},{\"gridX\":1496,\"gridY\":192,\"type\":52},{\"gridX\":1448,\"gridY\":224,\"type\":52},{\"gridX\":1496,\"gridY\":208,\"type\":52},{\"gridX\":1496,\"gridY\":224,\"type\":52},{\"gridX\":1448,\"gridY\":240,\"type\":51},{\"gridX\":1464,\"gridY\":240,\"type\":51},{\"gridX\":1480,\"gridY\":240,\"type\":51},{\"gridX\":1496,\"gridY\":240,\"type\":51},{\"gridX\":1608,\"gridY\":160,\"type\":52},{\"gridX\":1664,\"gridY\":160,\"type\":52},{\"gridX\":1752,\"gridY\":160,\"type\":52},{\"gridX\":1608,\"gridY\":176,\"type\":52},{\"gridX\":1664,\"gridY\":176,\"type\":52},{\"gridX\":1752,\"gridY\":176,\"type\":52},{\"gridX\":1608,\"gridY\":192,\"type\":52},{\"gridX\":1664,\"gridY\":192,\"type\":52},{\"gridX\":1752,\"gridY\":192,\"type\":52},{\"gridX\":1664,\"gridY\":208,\"type\":52},{\"gridX\":1752,\"gridY\":208,\"type\":52},{\"gridX\":1656,\"gridY\":96,\"type\":55},{\"gridX\":1608,\"gridY\":96,\"type\":55},{\"gridX\":1752,\"gridY\":96,\"type\":55},{\"gridX\":1412,\"gridY\":96,\"type\":58},{\"gridX\":1385,\"gridY\":119,\"type\":61},{\"gridX\":1608,\"gridY\":208,\"type\":51},{\"gridX\":1664,\"gridY\":224,\"type\":51},{\"gridX\":1752,\"gridY\":224,\"type\":51}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1776,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":124,\"gridY\":96,\"type\":8},{\"gridX\":144,\"gridY\":96,\"type\":9},{\"gridX\":168,\"gridY\":96,\"type\":10},{\"gridX\":184,\"gridY\":96,\"type\":6},{\"gridX\":334,\"gridY\":165,\"type\":6},{\"gridX\":465,\"gridY\":77,\"type\":7},{\"gridX\":584,\"gridY\":7,\"type\":6},{\"gridX\":604,\"gridY\":77,\"type\":7},{\"gridX\":624,\"gridY\":7,\"type\":6},{\"gridX\":644,\"gridY\":77,\"type\":7},{\"gridX\":668,\"gridY\":20,\"type\":3},{\"gridX\":708,\"gridY\":8,\"type\":3},{\"gridX\":688,\"gridY\":77,\"type\":7},{\"gridX\":848,\"gridY\":153,\"type\":6},{\"gridX\":1067,\"gridY\":96,\"type\":5},{\"gridX\":1312,\"gridY\":55,\"type\":4},{\"gridX\":337,\"gridY\":117,\"type\":6},{\"gridX\":1096,\"gridY\":197,\"type\":6},{\"gridX\":1096,\"gridY\":169,\"type\":6},{\"gridX\":1035,\"gridY\":169,\"type\":6},{\"gridX\":989,\"gridY\":169,\"type\":6},{\"gridX\":564,\"gridY\":176,\"type\":3},{\"gridX\":468,\"gridY\":160,\"type\":3},{\"gridX\":400,\"gridY\":180,\"type\":3},{\"gridX\":1459,\"gridY\":96,\"type\":5},{\"gridX\":1491,\"gridY\":96,\"type\":5}],\"collectibles\":[{\"gridX\":140,\"gridY\":104,\"type\":0},{\"gridX\":132,\"gridY\":104,\"type\":0},{\"gridX\":148,\"gridY\":104,\"type\":0},{\"gridX\":212,\"gridY\":156,\"type\":0},{\"gridX\":216,\"gridY\":164,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":204,\"gridY\":140,\"type\":0},{\"gridX\":208,\"gridY\":148,\"type\":0},{\"gridX\":240,\"gridY\":172,\"type\":0},{\"gridX\":248,\"gridY\":172,\"type\":0},{\"gridX\":256,\"gridY\":172,\"type\":0},{\"gridX\":320,\"gridY\":168,\"type\":0},{\"gridX\":312,\"gridY\":172,\"type\":0},{\"gridX\":304,\"gridY\"" << ":172,\"type\":0},{\"gridX\":312,\"gridY\":100,\"type\":0},{\"gridX\":304,\"gridY\":100,\"type\":0},{\"gridX\":320,\"gridY\":100,\"type\":0},{\"gridX\":276,\"gridY\":120,\"type\":1},{\"gridX\":336,\"gridY\":80,\"type\":0},{\"gridX\":336,\"gridY\":72,\"type\":0},{\"gridX\":336,\"gridY\":64,\"type\":0},{\"gridX\":368,\"gridY\":68,\"type\":0},{\"gridX\":376,\"gridY\":68,\"type\":0},{\"gridX\":384,\"gridY\":68,\"type\":0},{\"gridX\":424,\"gridY\":64,\"type\":0},{\"gridX\":432,\"gridY\":64,\"type\":0},{\"gridX\":416,\"gridY\":64,\"type\":0},{\"gridX\":476,\"gridY\":64,\"type\":0},{\"gridX\":484,\"gridY\":56,\"type\":0},{\"gridX\":468,\"gridY\":64,\"type\":0},{\"gridX\":540,\"gridY\":56,\"type\":1},{\"gridX\":592,\"gridY\":32,\"type\":0},{\"gridX\":596,\"gridY\":40,\"type\":0},{\"gridX\":600,\"gridY\":48,\"type\":0},{\"gridX\":604,\"gridY\":56,\"type\":0},{\"gridX\":608,\"gridY\":64,\"type\":0},{\"gridX\":612,\"gridY\":56,\"type\":0},{\"gridX\":616,\"gridY\":48,\"type\":0},{\"gridX\":620,\"gridY\":40,\"type\":0},{\"gridX\":624,\"gridY\":32,\"type\":0},{\"gridX\":632,\"gridY\":32,\"type\":0},{\"gridX\":636,\"gridY\":40,\"type\":0},{\"gridX\":640,\"gridY\":48,\"type\":0},{\"gridX\":644,\"gridY\":56,\"type\":0},{\"gridX\":648,\"gridY\":64,\"type\":0},{\"gridX\":652,\"gridY\":56,\"type\":0},{\"gridX\":656,\"gridY\":48,\"type\":0},{\"gridX\":660,\"gridY\":40,\"type\":0},{\"gridX\":664,\"gridY\":32,\"type\":0},{\"gridX\":676,\"gridY\":32,\"type\":0},{\"gridX\":680,\"gridY\":40,\"type\":0},{\"gridX\":684,\"gridY\":48,\"type\":0},{\"gridX\":688,\"gridY\":56,\"type\":0},{\"gridX\":692,\"gridY\":64,\"type\":0},{\"gridX\":732,\"gridY\":28,\"type\":0},{\"gridX\":740,\"gridY\":28,\"type\":0},{\"gridX\":748,\"gridY\":28,\"type\":0},{\"gridX\":780,\"gridY\":24,\"type\":0},{\"gridX\":788,\"gridY\":24,\"type\":0},{\"gridX\":796,\"gridY\":24,\"type\":0},{\"gridX\":812,\"gridY\":48,\"type\":0},{\"gridX\":816,\"gridY\":64,\"type\":0},{\"gridX\":820,\"gridY\":80,\"type\":0},{\"gridX\":952,\"gridY\":132,\"type\":1},{\"gridX\":828,\"gridY\":104,\"type\":0},{\"gridX\":832,\"gridY\":120,\"type\":0},{\"gridX\":836,\"gridY\":136,\"type\":0},{\"gridX\":712,\"gridY\":128,\"type\":0},{\"gridX\":720,\"gridY\":128,\"type\":0},{\"gridX\":728,\"gridY\":128,\"type\":0},{\"gridX\":1084,\"gridY\":100,\"type\":0},{\"gridX\":1076,\"gridY\":100,\"type\":0},{\"gridX\":1068,\"gridY\":100,\"type\":0},{\"gridX\":1156,\"gridY\":36,\"type\":0},{\"gridX\":1200,\"gridY\":64,\"type\":0},{\"gridX\":1192,\"gridY\":56,\"type\":0},{\"gridX\":1140,\"gridY\":36,\"type\":0},{\"gridX\":1148,\"gridY\":36,\"type\":0},{\"gridX\":1180,\"gridY\":36,\"type\":0},{\"gridX\":1100,\"gridY\":36,\"type\":0},{\"gridX\":1108,\"gridY\":36,\"type\":0},{\"gridX\":1116,\"gridY\":36,\"type\":0},{\"gridX\":1185,\"gridY\":46,\"type\":0},{\"gridX\":1132,\"gridY\":36,\"type\":0},{\"gridX\":1124,\"gridY\":36,\"type\":0},{\"gridX\":1320,\"gridY\":64,\"type\":0},{\"gridX\":1328,\"gridY\":64,\"type\":0},{\"gridX\":1336,\"gridY\":64,\"type\":0},{\"gridX\":1380,\"gridY\":36,\"type\":0},{\"gridX\":1388,\"gridY\":36,\"type\":0},{\"gridX\":1372,\"gridY\":36,\"type\":0},{\"gridX\":1440,\"gridY\":64,\"type\":0},{\"gridX\":1432,\"gridY\":64,\"type\":0},{\"gridX\":1424,\"gridY\":64,\"type\":0},{\"gridX\":1634,\"gridY\":120,\"type\":0},{\"gridX\":1641,\"gridY\":116,\"type\":0},{\"gridX\":1647,\"gridY\":123,\"type\":0},{\"gridX\":1710,\"gridY\":118,\"type\":0},{\"gridX\":1717,\"gridY\":114,\"type\":0},{\"gridX\":1722,\"gridY\":121,\"type\":0},{\"gridX\":1504,\"gridY\":36,\"type\":0},{\"gridX\":1496,\"gridY\":36,\"type\":0},{\"gridX\":1488,\"gridY\":36,\"type\":0},{\"gridX\":994,\"gridY\":24,\"type\":2},{\"gridX\":1126,\"gridY\":148,\"type\":3},{\"gridX\":400,\"gridY\":192,\"type\":2},{\"gridX\":467,\"gridY\":175,\"type\":2},{\"gridX\":564,\"gridY\":191,\"type\":2},{\"gridX\":1384,\"gridY\":128,\"type\":0},{\"gridX\":1392,\"gridY\":128,\"type\":0},{\"gridX\":1436,\"gridY\":112,\"type\":0},{\"gridX\":1432,\"gridY\":124,\"type\":0},{\"gridX\":1476,\"gridY\":100,\"type\":0},{\"gridX\":1424,\"gridY\":128,\"type\":0},{\"gridX\":1416,\"gridY\":128,\"type\":0},{\"gridX\":1400,\"gridY\":128,\"type\":0},{\"gridX\":1488,\"gridY\":100,\"type\":0},{\"gridX\":1408,\"gridY\":128,\"type\":0},{\"gridX\":1452,\"gridY\":100,\"type\":0},{\"gridX\":1464,\"gridY\":100,\"type\":0},{\"gridX\":1612,\"gridY\":228,\"type\":2},{\"gridX\":1744,\"gridY\":232,\"type\":3},{\"gridX\":1600,\"gridY\":216,\"type\":0},{\"gridX\":1600,\"gridY\":208,\"type\":0},{\"gridX\":1656,\"gridY\":224,\"type\":0},{\"gridX\":1656,\"gridY\":232,\"type\":0},{\"gridX\":1656,\"gridY\":240,\"type\":0},{\"gridX\":1664,\"gridY\":240,\"type\":0},{\"gridX\":1672,\"gridY\":240,\"type\":0}],\"jons\":[{\"gridX\":17,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":1448,\"gridY\":112,\"type\":4},{\"gridX\":1464,\"gridY\":112,\"type\":4},{\"gridX\":1480,\"gridY\":112,\"type\":4},{\"gridX\":1496,\"gridY\":112,\"type\":4},{\"gridX\":1448,\"gridY\":128,\"type\":6},{\"gridX\":1464,\"gridY\":128,\"type\":6},{\"gridX\":1480,\"gridY\":128,\"type\":6},{\"gridX\":1496,\"gridY\":128,\"type\":6},{\"gridX\":1448,\"gridY\":160,\"type\":4},{\"gridX\":1464,\"gridY\":160,\"type\":4},{\"gridX\":1480,\"gridY\":160,\"type\":4},{\"gridX\":1496,\"gridY\":160,\"type\":4},{\"gridX\":1496,\"gridY\":144,\"type\":6},{\"gridX\":1480,\"gridY\":144,\"type\":6},{\"gridX\":1464,\"gridY\":144,\"type\":6},{\"gridX\":1448,\"gridY\":144,\"type\":6},{\"gridX\":1496,\"gridY\":192,\"type\":4},{\"gridX\":1480,\"gridY\":192,\"type\":4},{\"gridX\":1496,\"gridY\":208,\"type\":4},{\"gridX\":1480,\"gridY\":208,\"type\":4},{\"gridX\":1464,\"gridY\":208,\"type\":4},{\"gridX\":1448,\"gridY\":208,\"type\":4},{\"gridX\":1496,\"gridY\":224,\"type\":6},{\"gridX\":1480,\"gridY\":224,\"type\":6},{\"gridX\":1464,\"gridY\":224,\"type\":6},{\"gridX\":1496,\"gridY\":240,\"type\":6},{\"gridX\":1480,\"gridY\":240,\"type\":6},{\"gridX\":1464,\"gridY\":240,\"type\":6},{\"gridX\":1448,\"gridY\":240,\"type\":6},{\"gridX\":1448,\"gridY\":224,\"type\":6},{\"gridX\":1664,\"gridY\":96,\"type\":5},{\"gridX\":1752,\"gridY\":96,\"type\":5},{\"gridX\":1608,\"gridY\":96,\"type\":4},{\"gridX\":1608,\"gridY\":112,\"type\":4},{\"gridX\":1752,\"gridY\":112,\"type\":4},{\"gridX\":1664,\"gridY\":112,\"type\":4},{\"gridX\":1752,\"gridY\":224,\"type\":6},{\"gridX\":1664,\"gridY\":224,\"type\":6},{\"gridX\":1608,\"gridY\":208,\"type\":6},{\"gridX\":1752,\"gridY\":208,\"type\":4},{\"gridX\":1752,\"gridY\":192,\"type\":4},{\"gridX\":1752,\"gridY\":176,\"type\":4},{\"gridX\":1752,\"gridY\":160,\"type\":4},{\"gridX\":1664,\"gridY\":208,\"type\":4},{\"gridX\":1664,\"gridY\":192,\"type\":4},{\"gridX\":1664,\"gridY\":176,\"type\":4},{\"gridX\":1664,\"gridY\":160,\"type\":4},{\"gridX\":1608,\"gridY\":192,\"type\":4},{\"gridX\":1608,\"gridY\":176,\"type\":4},{\"gridX\":1608,\"gridY\":160,\"type\":4}],\"markers\":[{\"gridX\":2133,\"gridY\":0,\"type\":1}]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level20(text.c_str());
}

void Chapter1Level20::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level20 * Chapter1Level20::getInstance()
{
    return s_pInstance;
}

Chapter1Level20::Chapter1Level20(const char* json) : Level(json)
{
    // Empty
}
