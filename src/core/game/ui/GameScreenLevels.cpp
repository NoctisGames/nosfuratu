//
//  GameScreenLevels.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenLevels.h"

#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenLevelEditor.h"
#include "MathUtil.h"
#include "GameTracker.h"
#include "BatPanel.h"
#include "GameScreenWorldMap.h"
#include "GameScreenTransitions.h"

/// Level ///

Level * Level::getInstance()
{
    static Level *instance = new Level();
    
    return instance;
}

void Level::enter(GameScreen* gs)
{
    m_fStateTime = 0;
    m_iScoreFromTime = 0;
    m_iScoreFromObjects = 0;
    m_iScore = 0;
    m_iOnlineScore = 0;
    m_iLevelStatsFlag = m_iBestLevelStatsFlag;
    m_iNumGoldenCarrots = m_iLastKnownNumGoldenCarrots;
    m_hasCompletedLevel = false;
    m_isDisplayingResults = false;
    gs->m_isReleasingShockwave = false;
    gs->m_isScreenHeldDown = false;
    gs->m_fScreenHeldTime = 0;
    
    if (!m_game->isLoaded())
    {
        if (m_sourceGame)
        {
            m_game->copy(m_sourceGame);
        }
        else
        {
            m_game->load(m_json);
        }
        
        m_game->setBestLevelStatsFlag(m_iBestLevelStatsFlag);
        m_game->setCameraBounds(&gs->m_renderer->getCameraBounds());
        
        Jon& jon = m_game->getJon();
        jon.setAbilityFlag(m_iLastKnownJonAbilityFlag);
    }
    
    m_continueButton->getColor().alpha = 0;
    
    m_batPanel->reset();
    
    configBatPanel();

	gs->m_renderer->init(calcRendererTypeFromLevel(m_game->getWorld(), m_game->getLevel()));
    
    gs->m_renderer->beginOpeningPanningSequence(*m_game);
    
    onEnter(gs);
    
    EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), 0);
    EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), 0);
    EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), 0);
    EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), 0);
    
    static float fgWidth = CAM_WIDTH / 32;
    static float fgHeight = fgWidth * 1.171875f;
    
    static float textY = CAM_HEIGHT - fgHeight;
    
    GameTracker::getInstance()->config(CAM_WIDTH * 0.5f,
                                       textY,
                                       3.34f,
                                       textY - 0.14f,
                                       fgWidth,
                                       fgHeight);
    
    m_playLevelSelectMusicOnExit = gs->m_stateMachine->getPreviousState() == WorldMap::getInstance();
}

void Level::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        render(gs);
    }
    else
    {
        update(gs);
    }
}

void Level::exit(GameScreen* gs)
{
    m_game->reset();
    
    m_sourceGame = nullptr;
    
    stopLoopingSounds();
    
    if (m_playLevelSelectMusicOnExit)
    {
        Assets::getInstance()->setMusicId(MUSIC_PLAY_LEVEL_SELECT_LOOP);
    }
    
    m_fStateTime = 0;
    gs->m_isReleasingShockwave = false;
    gs->m_fShockwaveElapsedTime = 0;
    gs->m_fShockwaveCenterX = 0;
    gs->m_fShockwaveCenterY = 0;
    m_hasShownOpeningSequence = false;
    m_hasOpeningSequenceCompleted = false;
    gs->m_hasSwiped = false;
    m_showDeathTransOut = false;
    m_hasCompletedLevel = false;
    m_isDisplayingResults = false;
    m_exitLoop = false;
    
    m_iBestScore = 0;
    m_iBestOnlineScore = 0;
    m_iBestLevelStatsFlag = 0;
    m_iLastKnownNumGoldenCarrots = 0;
    m_iLastKnownJonAbilityFlag = 0;
}

void Level::setSourceGame(Game* game)
{
    m_sourceGame = game;
}

void Level::setBestStats(int bestScore, int bestOnlineScore, int bestLevelStatsFlag, int numGoldenCarrots, int jonAbilityFlag)
{
    m_iBestScore = bestScore;
    m_iBestOnlineScore = bestOnlineScore;
    m_iBestLevelStatsFlag = bestLevelStatsFlag;
    m_iLastKnownNumGoldenCarrots = numGoldenCarrots;
    m_iLastKnownJonAbilityFlag = jonAbilityFlag;
}

void Level::stopLoopingSounds()
{
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPARROW_FLY);
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SAW_GRIND);
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPIKED_BALL_ROLLING);
}

int Level::getJonAbilityFlag()
{
    return m_game->getJon().getAbilityFlag();
}

void Level::onEnter(GameScreen* gs)
{
    if (m_hasOpeningSequenceCompleted)
    {
        CountHissWithMina& countHissWithMina = m_game->getCountHissWithMina();
        countHissWithMina.getPosition().setX(m_game->getFarRight() + CAM_WIDTH * 2);
    }
}

void Level::beginOpeningSequence(GameScreen* gs)
{
    CountHissWithMina& countHissWithMina = m_game->getCountHissWithMina();
    countHissWithMina.beginMovement();
    
    if (gs->m_stateMachine->getPreviousState() == GameScreenLevelEditor::getInstance())
	{
		m_hasShownOpeningSequence = true;
		m_hasOpeningSequenceCompleted = true;

		updateCamera(gs, 0, false, true);

		Assets::getInstance()->setMusicId(MUSIC_PLAY_WORLD_1_LOOP);

		return;
	}

	gs->m_renderer->beginOpeningPanningSequence(*m_game);

	EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), 0);

	m_hasShownOpeningSequence = true;

	if (Assets::getInstance()->isMusicEnabled())
	{
		Assets::getInstance()->addSoundIdToPlayQueue(SOUND_WORLD_1_LOOP_INTRO);
	}
}

void Level::handleOpeningSequence(GameScreen* gs)
{
	CountHissWithMina& countHissWithMina = m_game->getCountHissWithMina();
	countHissWithMina.update(gs->m_fDeltaTime);

	Jon& jon = m_game->getJon();
	jon.update(gs->m_fDeltaTime);

	int result = gs->m_renderer->updateCameraToFollowPathToJon(*m_game);
	m_hasOpeningSequenceCompleted = result == 3;
	m_activateRadialBlur = result == 1;
	jon.setAllowedToMove(m_hasOpeningSequenceCompleted);

	if (m_hasOpeningSequenceCompleted)
	{
		countHissWithMina.getPosition().setX(m_game->getFarRight() + CAM_WIDTH * 2);

		Assets::getInstance()->setMusicId(MUSIC_PLAY_WORLD_1_LOOP);
	}

	if (result == 2)
	{
		jon.beginWarmingUp();
	}

	EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), 0);
}

void Level::update(GameScreen* gs)
{
    Jon& jon = m_game->getJon();
    jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
    
    if (!m_hasShownOpeningSequence)
    {
		beginOpeningSequence(gs);
    }
    else if (!m_hasOpeningSequenceCompleted)
    {
		handleOpeningSequence(gs);
    }
    else
    {
        GameTracker::getInstance()->update(gs->m_fDeltaTime);
        
        m_batPanel->update(gs);
        
        if (m_batPanel->isRequestingInput())
        {
            return;
        }
        
        if (m_isDisplayingResults)
        {
            m_continueButton->getColor().alpha += gs->m_fDeltaTime;
            if (m_continueButton->getColor().alpha > 1)
            {
                m_continueButton->getColor().alpha = 1;
            }
            
            for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
            {
                gs->touchToWorld(*(*i));
                
                switch ((*i)->getTouchType())
                {
                    case DOWN:
                    case DRAGGED:
                        continue;
                    case UP:
                        if (m_continueButton->handleClick(*gs->m_touchPoint))
                        {
                            if (m_game->getWorld() == 1
                                && m_game->getLevel() == 21)
                            {
                                m_playLevelSelectMusicOnExit = false;
                                LevelToComingSoon::getInstance()->setLevelComingFrom(this);
                                gs->m_stateMachine->changeState(LevelToComingSoon::getInstance());
                            }
                            else
                            {
                                gs->m_stateMachine->revertToPreviousState();
                            }
                            
                            return;
                        }
                        
                        break;
                }
            }
            
            return;
        }
        
        if (handleTouchInput(gs))
        {
            return;
        }
        
        if (m_showDeathTransOut)
        {
            // Starting new game after death
            
            m_fStateTime += gs->m_fDeltaTime * 2;
            
            if (m_fStateTime > 2.4f)
            {
                m_fStateTime = 0;
                m_showDeathTransOut = false;
            }
        }
        
        if (jon.isDead())
        {
            stopLoopingSounds();
            
            // Starting death transition, when screen goes black, new game begins
            
            m_fStateTime += gs->m_fDeltaTime * 2;
            
            if (m_fStateTime > 1.6f)
            {
                m_game->reset();
                enter(gs);
                
                updateCamera(gs, 0, false, true);
                
                m_showDeathTransOut = true;
            }
            
            return;
        }
        else
        {
            updateScore();
            
            // Is Still Actively playing the Level
            
			if (!m_hasCompletedLevel)
			{
				m_game->update(gs->m_fDeltaTime);
			}
            
            if (isInSlowMotionMode())
            {
                gs->m_fDeltaTime /= 8;
            }
            
            if (jon.isTransformingIntoVampire() || jon.isRevertingToRabbit())
            {
                if (jon.getTransformStateTime() < 0.0625f)
                {
                    if (!isInSlowMotionMode())
                    {
                        gs->m_fDeltaTime /= 8;
                    }
                }
                else
                {
                    if (!gs->m_isReleasingShockwave)
                    {
                        gs->m_fShockwaveCenterX = jon.getPosition().getX();
                        gs->m_fShockwaveCenterY = jon.getPosition().getY();
                        gs->m_fShockwaveElapsedTime = 0.0f;
                        gs->m_isReleasingShockwave = true;
                    }
                }
            }
            
            if (gs->m_isReleasingShockwave)
            {
                gs->m_fShockwaveElapsedTime += gs->m_fDeltaTime * 1.2f;
                
                if (gs->m_fShockwaveElapsedTime > 2)
                {
                    gs->m_fShockwaveElapsedTime = 0;
                    gs->m_isReleasingShockwave = false;
                }
            }
            
            m_game->updateAndClean(gs->m_fDeltaTime);
            
            handleCollections(jon, m_game->getCollectibleItems(), gs->m_fDeltaTime);
            
            updateScore();
            
            if (gs->m_isScreenHeldDown)
            {
                gs->m_fScreenHeldTime += gs->m_fDeltaTime;
                
                if (gs->m_fScreenHeldTime > 0.4f)
                {
                    jon.triggerTransform();
                    gs->m_isScreenHeldDown = false;
                    gs->m_fShockwaveElapsedTime = 0;
                    gs->m_isReleasingShockwave = false;
                }
            }
        }
        
        updateCamera(gs);
        
        if (m_game->getMarkers().size() > 1)
        {
            Marker* nextEndLoopMarker = m_game->getMarkers().at(1);
            if (gs->m_renderer->getCameraBounds().getRight() > nextEndLoopMarker->getGridX() * GRID_CELL_SIZE)
            {
                if (m_exitLoop)
                {
                    m_game->getMarkers().erase(m_game->getMarkers().begin());
                    m_game->getMarkers().erase(m_game->getMarkers().begin());
                    
                    m_exitLoop = false;
                }
                else
                {
                    Marker* beginLoopMarker = m_game->getMarkers().at(0);
                    
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
                    
                    EntityUtils::offsetAll(m_game->getMarkers(), beginGridX, endGridX);
                    
                    EntityUtils::setGameToEntities(m_game->getForegroundObjects(), m_game.get());
                    EntityUtils::setGameToEntities(m_game->getMidBossForegroundObjects(), m_game.get());
                    EntityUtils::setGameToEntities(m_game->getEndBossForegroundObjects(), m_game.get());
                    EntityUtils::setGameToEntities(m_game->getEnemies(), m_game.get());
                    EntityUtils::setGameToEntities(m_game->getExtraForegroundObjects(), m_game.get());
                    EntityUtils::setGameToEntities(m_game->getEndBossSnakes(), m_game.get());
                    
                    m_game->calcFarRight();
                }
            }
        }
        
        if (m_hasCompletedLevel)
        {
            m_fStateTime += gs->m_fDeltaTime / 2;
            if (m_fStateTime > 1)
            {
				stopLoopingSounds();

                m_fStateTime = 1;
                m_isDisplayingResults = true;
            }
            
            if (m_game->getLevel() != 10
                && m_game->getLevel() != 21)
            {
                short musicId = MUSIC_SET_VOLUME * 1000 + (short) ((0.5f - m_fStateTime / 2) * 100);
                Assets::getInstance()->setMusicId(musicId);
            }
        }
        else if (jon.getMainBounds().getLeft() > m_game->getFarRight())
        {
            // Has Cleared the Level
            
            m_iLevelStatsFlag = FlagUtil::setFlag(m_iLevelStatsFlag, FLAG_LEVEL_COMPLETE);
            
            if (m_game->getNumCarrotsCollected() >= 100)
            {
                m_game->setNumGoldenCarrotsCollected(m_game->getNumGoldenCarrotsCollected() + 1);
                
                GameTracker::getInstance()->onScored(SCORE_GOLDEN_CARROT);
                GameTracker::getInstance()->onBonusGoldenCarrotEarned();
                
                m_iLevelStatsFlag = FlagUtil::setFlag(m_iLevelStatsFlag, FLAG_BONUS_GOLDEN_CARROT_COLLECTED);
            }
            
            static float startingTime = 300.0f;
            
            float secondsLeft = clamp(startingTime - m_game->getStateTime(), startingTime, 0);
            
            m_iScoreFromTime = secondsLeft * 100;
            
            if (m_iScoreFromTime > 0)
            {
                GameTracker::getInstance()->onTimeBonusScoreEarned(m_iScoreFromTime);
            }
            
            updateScore();
            
            if (m_iScore < m_iBestScore)
            {
                m_iScore = m_iBestScore;
            }
            
            m_iOnlineScore = m_iScore;
            
            if (m_iOnlineScore > m_iBestOnlineScore)
            {
                // TODO, show Submit Highscore button
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
            
            gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_COMPLETED * 1000;
            gs->m_iRequestedAction += m_game->getWorld() * 100;
            gs->m_iRequestedAction += m_game->getLevel();
  
            m_fStateTime = 0;
			gs->m_renderer->stopCamera();
            m_hasCompletedLevel = true;
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_LEVEL_COMPLETE);
        }
        
        EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
        EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
        EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
        EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
    }
}

void Level::render(GameScreen* gs)
{
    Jon& jon = m_game->getJon();
    
    gs->m_renderer->beginFrame(gs->m_fDeltaTime);
    
    gs->m_renderer->renderWorld(*m_game);
    
    if (gs->m_isReleasingShockwave)
    {
        gs->m_renderer->renderToSecondFramebufferWithShockwave(gs->m_fShockwaveCenterX, gs->m_fShockwaveCenterY, gs->m_fShockwaveElapsedTime, jon.isTransformingIntoVampire() || jon.isVampire());
    }
    else
    {
        gs->m_renderer->renderToSecondFramebuffer(*m_game);
    }
    
    gs->m_renderer->renderJonAndExtraForegroundObjects(*m_game);
    
    additionalRenderingBeforeHud(gs);
    
    if (m_hasCompletedLevel)
    {
        gs->m_renderer->renderBlackOverlay(m_fStateTime);
    }
    
    gs->m_renderer->renderBatPanel(*m_batPanel);
    
    if (gs->m_isPaused)
    {
        gs->m_renderer->renderToThirdFramebufferWithObfuscation();
    }
    
    if (m_hasOpeningSequenceCompleted)
    {
        gs->m_renderer->renderHud(*m_game, m_hasCompletedLevel ? nullptr : m_backButton.get(), m_isDisplayingResults ? m_continueButton.get() : nullptr, m_iScore);
    }

#if DEBUG || _DEBUG
	gs->m_renderer->renderDebugInfo(*m_game, gs->m_iFPS);
#endif
    
    if (gs->m_isPaused)
    {
		m_fStateTime = 0;
		m_showDeathTransOut = false;

        gs->m_renderer->renderResumeButtonOverlay();
    }
    
    if (jon.isDead())
    {
        gs->m_renderer->renderToScreenWithTransDeathIn(m_fStateTime);
    }
    else if (m_showDeathTransOut)
    {
        gs->m_renderer->renderToScreenWithTransDeathOut(m_fStateTime);
    }
    else
    {
        if (m_activateRadialBlur)
        {
            gs->m_renderer->renderToScreenWithRadialBlur();
        }
        else
        {
            gs->m_renderer->renderToScreen();
        }
    }
    
    gs->m_renderer->endFrame();
}

void Level::updateCamera(GameScreen* gs, float paddingX, bool ignoreY, bool instant)
{
    gs->m_renderer->updateCameraToFollowJon(*m_game, m_batPanel.get(), gs->m_fDeltaTime, paddingX, false, ignoreY, instant);
}

void Level::additionalRenderingBeforeHud(GameScreen* gs)
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
        m_batPanel->config(m_game.get(), m_game->getWorld(), m_game->getLevel());
    }
}

bool Level::handleOpeningSequenceTouchInput(GameScreen* gs)
{
    for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        switch ((*i)->getTouchType())
        {
            case DOWN:
                continue;
            case DRAGGED:
                continue;
            case UP:
                return true;
        }
    }
    
    return false;
}

bool Level::handleTouchInput(GameScreen* gs)
{
    Jon& jon = m_game->getJon();
    bool isJonAlive = jon.isAlive();
    
    for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld(*(*i));
        
        switch ((*i)->getTouchType())
        {
            case DOWN:
                if (isJonAlive)
                {
                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                    gs->m_isScreenHeldDown = true;
                    gs->m_fScreenHeldTime = 0.0f;
                }
                continue;
            case DRAGGED:
                if (isJonAlive && !gs->m_hasSwiped)
                {
                    if (gs->m_touchPoint->getX() >= (gs->m_touchPointDown->getX() + SWIPE_WIDTH))
                    {
                        // Swipe Right
                        jon.triggerRightAction();
                        gs->m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getX() <= (gs->m_touchPointDown->getX() - SWIPE_WIDTH))
                    {
                        // Swipe Left
                        jon.triggerLeftAction();
                        gs->m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() >= (gs->m_touchPointDown->getY() + SWIPE_HEIGHT))
                    {
                        // Swipe Up
                        jon.triggerUpAction();
                        gs->m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() <= (gs->m_touchPointDown->getY() - SWIPE_HEIGHT))
                    {
                        // Swipe Down
                        jon.triggerDownAction();
                        gs->m_hasSwiped = true;
                    }
                    
                    if (gs->m_hasSwiped)
                    {
                        gs->m_isScreenHeldDown = false;
                        gs->m_fScreenHeldTime = 0.0f;
                    }
                }
                continue;
            case UP:
                if (!m_hasCompletedLevel
                    && m_backButton->handleClick(*gs->m_touchPoint))
                {
                    m_exitLoop = true;
                    
                    gs->m_renderer->stopCamera();
                    
                    gs->m_stateMachine->revertToPreviousState();
                    
                    return true;
                }
                
                if (isJonAlive)
                {
                    if (!gs->m_hasSwiped && gs->m_fScreenHeldTime < 0.4f)
                    {
                        jon.triggerJump();
                    }
                    
                    if (gs->m_fScreenHeldTime > 0.4f)
                    {
                        jon.triggerCancelTransform();
                    }
                    
                    gs->m_isScreenHeldDown = false;
                    gs->m_fScreenHeldTime = 0;
                    
                    gs->m_hasSwiped = false;
                    
                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                }
                break;
        }
    }
    
    return false;
}

void Level::updateScore()
{
    m_iScoreFromObjects = m_game->getNumCarrotsCollected() * SCORE_CARROT;
    m_iScoreFromObjects += m_game->getNumGoldenCarrotsCollected() * SCORE_GOLDEN_CARROT;
    
    if (m_game->getJons().size() > 0)
    {
        m_iScoreFromObjects += m_game->getJon().getNumEnemiesDestroyed() * SCORE_ENEMY;
    }
    
    m_iScore = m_iScoreFromTime + m_iScoreFromObjects;
}

void Level::handleCollections(PhysicalEntity& entity, std::vector<CollectibleItem *>& items, float deltaTime)
{
    for (std::vector<CollectibleItem *>::iterator i = items.begin(); i != items.end(); i++)
    {
        if ((*i)->isCollected())
        {
            continue;
        }
        
        if (OverlapTester::doRectanglesOverlap(entity.getMainBounds(), (*i)->getMainBounds()))
        {
            (*i)->collect();
            
            if (dynamic_cast<GoldenCarrot *>((*i)))
            {
                GoldenCarrot* gc = dynamic_cast<GoldenCarrot *>((*i));
                
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
            else
            {
                m_game->setNumCarrotsCollected(m_game->getNumCarrotsCollected() + 1);
                
                GameTracker::getInstance()->onScored(SCORE_CARROT);
            }
        }
    }
}

Level::Level(const char* json) :
m_sourceGame(nullptr),
m_fStateTime(0.0f),
m_iScoreFromTime(0),
m_iScoreFromObjects(0),
m_iScore(0),
m_iOnlineScore(0),
m_iLevelStatsFlag(0),
m_iNumGoldenCarrots(0),
m_hasShownOpeningSequence(false),
m_hasOpeningSequenceCompleted(false),
m_activateRadialBlur(false),
m_showDeathTransOut(false),
m_exitLoop(false),
m_hasCompletedLevel(false),
m_isDisplayingResults(false),
m_iBestScore(0),
m_iBestOnlineScore(0),
m_iBestLevelStatsFlag(0),
m_iLastKnownNumGoldenCarrots(0),
m_iLastKnownJonAbilityFlag(0),
m_playLevelSelectMusicOnExit(false)
{
    m_json = json;
    m_game = std::unique_ptr<Game>(new Game());
    m_batPanel = std::unique_ptr<BatPanel>(new BatPanel());
    m_backButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_BackToLevelSelect));
    m_continueButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_ContinueToLevelSelect));
}

#include <sstream>

/// Chapter 1 Level 1 ///

Chapter1Level1 * Chapter1Level1::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":1,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":0},{\"gridX\":32,\"gridY\":96,\"type\":1},{\"gridX\":72,\"gridY\":96,\"type\":2},{\"gridX\":196,\"gridY\":96,\"type\":0},{\"gridX\":228,\"gridY\":96,\"type\":1},{\"gridX\":268,\"gridY\":96,\"type\":2},{\"gridX\":348,\"gridY\":96,\"type\":2},{\"gridX\":408,\"gridY\":96,\"type\":0},{\"gridX\":460,\"gridY\":96,\"type\":2},{\"gridX\":572,\"gridY\":96,\"type\":0},{\"gridX\":612,\"gridY\":96,\"type\":1},{\"gridX\":656,\"gridY\":96,\"type\":2},{\"gridX\":720,\"gridY\":96,\"type\":0},{\"gridX\":760,\"gridY\":96,\"type\":1},{\"gridX\":800,\"gridY\":96,\"type\":2},{\"gridX\":912,\"gridY\":96,\"type\":2},{\"gridX\":1036,\"gridY\":96,\"type\":1},{\"gridX\":1076,\"gridY\":96,\"type\":0},{\"gridX\":1120,\"gridY\":96,\"type\":2},{\"gridX\":1192,\"gridY\":96,\"type\":2},{\"gridX\":1264,\"gridY\":96,\"type\":0},{\"gridX\":1336,\"gridY\":96,\"type\":0},{\"gridX\":1380,\"gridY\":96,\"type\":2},{\"gridX\":1464,\"gridY\":96,\"type\":1},{\"gridX\":1512,\"gridY\":96,\"type\":2},{\"gridX\":1628,\"gridY\":96,\"type\":0},{\"gridX\":1672,\"gridY\":96,\"type\":1},{\"gridX\":1712,\"gridY\":96,\"type\":2},{\"gridX\":1772,\"gridY\":96,\"type\":0},{\"gridX\":1804,\"gridY\":96,\"type\":1},{\"gridX\":1844,\"gridY\":96,\"type\":1},{\"gridX\":1884,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":132,\"gridY\":0,\"type\":31},{\"gridX\":512,\"gridY\":0,\"type\":30},{\"gridX\":1564,\"gridY\":0,\"type\":31}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1664,\"gridY\":96,\"type\":12},{\"gridX\":320,\"gridY\":116,\"type\":0},{\"gridX\":324,\"gridY\":116,\"type\":1},{\"gridX\":338,\"gridY\":116,\"type\":1},{\"gridX\":366,\"gridY\":116,\"type\":2},{\"gridX\":852,\"gridY\":116,\"type\":0},{\"gridX\":856,\"gridY\":116,\"type\":1},{\"gridX\":870,\"gridY\":116,\"type\":1},{\"gridX\":912,\"gridY\":116,\"type\":2},{\"gridX\":916,\"gridY\":96,\"type\":13},{\"gridX\":943,\"gridY\":96,\"type\":15},{\"gridX\":921,\"gridY\":96,\"type\":15},{\"gridX\":965,\"gridY\":96,\"type\":16},{\"gridX\":968,\"gridY\":116,\"type\":0},{\"gridX\":884,\"gridY\":116,\"type\":1},{\"gridX\":1028,\"gridY\":116,\"type\":2},{\"gridX\":972,\"gridY\":116,\"type\":1},{\"gridX\":1000,\"gridY\":116,\"type\":1},{\"gridX\":986,\"gridY\":116,\"type\":1},{\"gridX\":1014,\"gridY\":116,\"type\":1},{\"gridX\":898,\"gridY\":116,\"type\":1},{\"gridX\":1024,\"gridY\":96,\"type\":13},{\"gridX\":1029,\"gridY\":96,\"type\":15},{\"gridX\":1051,\"gridY\":96,\"type\":15},{\"gridX\":1073,\"gridY\":96,\"type\":15},{\"gridX\":708,\"gridY\":96,\"type\":13},{\"gridX\":724,\"gridY\":96,\"type\":14},{\"gridX\":757,\"gridY\":96,\"type\":16},{\"gridX\":713,\"gridY\":96,\"type\":14},{\"gridX\":735,\"gridY\":96,\"type\":14},{\"gridX\":554,\"gridY\":96,\"type\":14},{\"gridX\":549,\"gridY\":96,\"type\":13},{\"gridX\":565,\"gridY\":96,\"type\":16},{\"gridX\":1112,\"gridY\":96,\"type\":13},{\"gridX\":1117,\"gridY\":96,\"type\":15},{\"gridX\":1139,\"gridY\":96,\"type\":15},{\"gridX\":1194,\"gridY\":96,\"type\":16},{\"gridX\":1161,\"gridY\":96,\"type\":15},{\"gridX\":1183,\"gridY\":96,\"type\":14},{\"gridX\":332,\"gridY\":96,\"type\":8},{\"gridX\":860,\"gridY\":96,\"type\":8},{\"gridX\":1304,\"gridY\":96,\"type\":8},{\"gridX\":1292,\"gridY\":116,\"type\":0},{\"gridX\":1296,\"gridY\":116,\"type\":1},{\"gridX\":1310,\"gridY\":116,\"type\":1},{\"gridX\":1324,\"gridY\":116,\"type\":1},{\"gridX\":1338,\"gridY\":116,\"type\":2},{\"gridX\":1372,\"gridY\":96,\"type\":13},{\"gridX\":1421,\"gridY\":96,\"type\":15},{\"gridX\":1377,\"gridY\":96,\"type\":15},{\"gridX\":1399,\"gridY\":96,\"type\":15},{\"gridX\":1443,\"gridY\":96,\"type\":14},{\"gridX\":1454,\"gridY\":96,\"type\":16},{\"gridX\":746,\"gridY\":96,\"type\":14},{\"gridX\":352,\"gridY\":116,\"type\":1},{\"gridX\":928,\"gridY\":140,\"type\":0},{\"gridX\":932,\"gridY\":140,\"type\":1},{\"gridX\":960,\"gridY\":140,\"type\":2},{\"gridX\":1335,\"gridY\":96,\"type\":13},{\"gridX\":1340,\"gridY\":96,\"type\":14},{\"gridX\":1351,\"gridY\":96,\"type\":16},{\"gridX\":946,\"gridY\":140,\"type\":1}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1632,\"gridY\":96}],\"enemies\":[{\"gridX\":1096,\"gridY\":96,\"type\":6},{\"gridX\":1356,\"gridY\":96,\"type\":6}],\"collectibles\":[{\"gridX\":148,\"gridY\":132,\"type\":0},{\"gridX\":140,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":164,\"gridY\":136,\"type\":1},{\"gridX\":188,\"gridY\":116,\"type\":0},{\"gridX\":180,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":104,\"type\":0},{\"gridX\":184,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":104,\"type\":0},{\"gridX\":240,\"gridY\":104,\"type\":0},{\"gridX\":268,\"gridY\":104,\"type\":0},{\"gridX\":276,\"gridY\":104,\"type\":0},{\"gridX\":284,\"gridY\":104,\"type\":0},{\"gridX\":344,\"gridY\":128,\"type\":0},{\"gridX\":328,\"gridY\":128,\"type\":0},{\"gridX\":336,\"gridY\":128,\"type\":0},{\"gridX\":392,\"gridY\":104,\"type\":0},{\"gridX\":408,\"gridY\":104,\"type\":0},{\"gridX\":400,\"gridY\":104,\"type\":0},{\"gridX\":436,\"gridY\":104,\"type\":0},{\"gridX\":444,\"gridY\":104,\"type\":0},{\"gridX\":452,\"gridY\":104,\"type\":0},{\"gridX\":512,\"gridY\":104,\"type\":0},{\"gridX\":516,\"gridY\":112,\"type\":0},{\"gridX\":520,\"gridY\":120,\"type\":0},{\"gridX\":524,\"gridY\":128,\"type\":0},{\"gridX\":560,\"gridY\":128,\"type\":0},{\"gridX\":564,\"gridY\":120,\"type\":0},{\"gridX\":568,\"gridY\":112,\"type\":0},{\"gridX\":572,\"gridY\":104,\"type\":0},{\"gridX\":600,\"gridY\":104,\"type\":0},{\"gridX\":608,\"gridY\":104,\"type\":0},{\"gridX\":616,\"gridY\":104,\"type\":0},{\"gridX\":644,\"gridY\":104,\"type\":0},{\"gridX\":652,\"gridY\":104,\"type\":0},{\"gridX\":660,\"gridY\":104,\"type\":0},{\"gridX\":708,\"gridY\":112,\"type\":0},{\"gridX\":712,\"gridY\":120,\"type\":0},{\"gridX\":716,\"gridY\":128,\"type\":0},{\"gridX\":720,\"gridY\":136,\"type\":0},{\"gridX\":748,\"gridY\":128,\"type\":0},{\"gridX\":744,\"gridY\":136,\"type\":0},{\"gridX\":752,\"gridY\":120,\"type\":0},{\"gridX\":760,\"gridY\":104,\"type\":0},{\"gridX\":704,\"gridY\":104,\"type\":0},{\"gridX\":756,\"gridY\":112,\"type\":0},{\"gridX\":988,\"gridY\":128,\"type\":0},{\"gridX\":900,\"gridY\":128,\"type\":0},{\"gridX\":868,\"gridY\":128,\"type\":0},{\"gridX\":876,\"gridY\":128,\"type\":0},{\"gridX\":808,\"gridY\":104,\"type\":0},{\"gridX\":816,\"gridY\":104,\"type\":0},{\"gridX\":800,\"gridY\":104,\"type\":0},{\"gridX\":908,\"gridY\":128,\"type\":0},{\"gridX\":1440,\"gridY\":156,\"type\":0},{\"gridX\":1392,\"gridY\":172,\"type\":0},{\"gridX\":860,\"gridY\":128,\"type\":0},{\"gridX\":996,\"gridY\":128,\"type\":0},{\"gridX\":1012,\"gridY\":128,\"type\":0},{\"gridX\":1028,\"gridY\":128,\"type\":0},{\"gridX\":1004,\"gridY\":128,\"type\":0},{\"gridX\":1020,\"gridY\":128,\"type\":0},{\"gridX\":1132,\"gridY\":172,\"type\":0},{\"gridX\":892,\"gridY\":128,\"type\":0},{\"gridX\":1128,\"gridY\":164,\"type\":0},{\"gridX\":1124,\"gridY\":156,\"type\":0},{\"gridX\":1136,\"gridY\":180,\"type\":0},{\"gridX\":1116,\"gridY\":140,\"type\":0},{\"gridX\":1120,\"gridY\":148,\"type\":0},{\"gridX\":1184,\"gridY\":148,\"type\":1},{\"gridX\":528,\"gridY\":136,\"type\":0},{\"gridX\":556,\"gridY\":136,\"type\":0},{\"gridX\":1232,\"gridY\":104,\"type\":0},{\"g" << "ridX\":1272,\"gridY\":104,\"type\":0},{\"gridX\":1240,\"gridY\":104,\"type\":0},{\"gridX\":1248,\"gridY\":104,\"type\":0},{\"gridX\":1256,\"gridY\":104,\"type\":0},{\"gridX\":1264,\"gridY\":104,\"type\":0},{\"gridX\":1308,\"gridY\":128,\"type\":0},{\"gridX\":1316,\"gridY\":128,\"type\":0},{\"gridX\":1324,\"gridY\":128,\"type\":0},{\"gridX\":1332,\"gridY\":128,\"type\":0},{\"gridX\":1384,\"gridY\":156,\"type\":0},{\"gridX\":1388,\"gridY\":164,\"type\":0},{\"gridX\":1372,\"gridY\":132,\"type\":0},{\"gridX\":1376,\"gridY\":140,\"type\":0},{\"gridX\":1380,\"gridY\":148,\"type\":0},{\"gridX\":1444,\"gridY\":148,\"type\":0},{\"gridX\":1448,\"gridY\":140,\"type\":0},{\"gridX\":1452,\"gridY\":132,\"type\":0},{\"gridX\":1456,\"gridY\":124,\"type\":0},{\"gridX\":1460,\"gridY\":116,\"type\":0},{\"gridX\":1572,\"gridY\":116,\"type\":0},{\"gridX\":1576,\"gridY\":124,\"type\":0},{\"gridX\":1580,\"gridY\":132,\"type\":0},{\"gridX\":1612,\"gridY\":132,\"type\":0},{\"gridX\":1616,\"gridY\":124,\"type\":0},{\"gridX\":1620,\"gridY\":116,\"type\":0},{\"gridX\":1596,\"gridY\":136,\"type\":1},{\"gridX\":352,\"gridY\":128,\"type\":0},{\"gridX\":360,\"gridY\":128,\"type\":0},{\"gridX\":1348,\"gridY\":120,\"type\":0},{\"gridX\":1360,\"gridY\":112,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level1 *instance = new Chapter1Level1(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 2 ///

Chapter1Level2 * Chapter1Level2::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":2,\"midgrounds\":[{\"gridX\":12,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":1},{\"gridX\":208,\"gridY\":96,\"type\":2},{\"gridX\":160,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":0},{\"gridX\":352,\"gridY\":96,\"type\":0},{\"gridX\":456,\"gridY\":96,\"type\":1},{\"gridX\":508,\"gridY\":96,\"type\":0},{\"gridX\":712,\"gridY\":96,\"type\":0},{\"gridX\":616,\"gridY\":96,\"type\":2},{\"gridX\":744,\"gridY\":96,\"type\":1},{\"gridX\":796,\"gridY\":96,\"type\":0},{\"gridX\":894,\"gridY\":96,\"type\":2},{\"gridX\":984,\"gridY\":96,\"type\":2},{\"gridX\":952,\"gridY\":96,\"type\":0},{\"gridX\":1092,\"gridY\":96,\"type\":2},{\"gridX\":1044,\"gridY\":96,\"type\":0},{\"gridX\":1160,\"gridY\":96,\"type\":1},{\"gridX\":1232,\"gridY\":96,\"type\":0},{\"gridX\":1328,\"gridY\":96,\"type\":0},{\"gridX\":1404,\"gridY\":96,\"type\":0},{\"gridX\":1468,\"gridY\":96,\"type\":1},{\"gridX\":1532,\"gridY\":96,\"type\":2},{\"gridX\":1592,\"gridY\":96,\"type\":1},{\"gridX\":1672,\"gridY\":96,\"type\":2},{\"gridX\":1728,\"gridY\":96,\"type\":0},{\"gridX\":1760,\"gridY\":96,\"type\":2},{\"gridX\":1816,\"gridY\":96,\"type\":1},{\"gridX\":1856,\"gridY\":96,\"type\":2},{\"gridX\":1636,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":1640,\"gridY\":0,\"type\":26},{\"gridX\":1288,\"gridY\":0,\"type\":26},{\"gridX\":1576,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":1448,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1320,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":388,\"gridY\":0,\"type\":30},{\"gridX\":552,\"gridY\":0,\"type\":30},{\"gridX\":264,\"gridY\":0,\"type\":30},{\"gridX\":1364,\"gridY\":0,\"type\":30},{\"gridX\":1088,\"gridY\":0,\"type\":31},{\"gridX\":1232,\"gridY\":0,\"type\":30}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1664,\"gridY\":96,\"type\":12},{\"gridX\":596,\"gridY\":116,\"type\":1},{\"gridX\":592,\"gridY\":116,\"type\":0},{\"gridX\":610,\"gridY\":116,\"type\":2},{\"gridX\":420,\"gridY\":116,\"type\":1},{\"gridX\":416,\"gridY\":116,\"type\":0},{\"gridX\":434,\"gridY\":116,\"type\":2},{\"gridX\":680,\"gridY\":116,\"type\":1},{\"gridX\":676,\"gridY\":116,\"type\":0},{\"gridX\":694,\"gridY\":116,\"type\":1},{\"gridX\":708,\"gridY\":116,\"type\":2},{\"gridX\":144,\"gridY\":96,\"type\":8},{\"gridX\":856,\"gridY\":116,\"type\":0},{\"gridX\":860,\"gridY\":116,\"type\":1},{\"gridX\":874,\"gridY\":116,\"type\":1},{\"gridX\":888,\"gridY\":116,\"type\":2},{\"gridX\":934,\"gridY\":140,\"type\":0},{\"gridX\":938,\"gridY\":140,\"type\":1},{\"gridX\":952,\"gridY\":140,\"type\":1},{\"gridX\":980,\"gridY\":140,\"type\":2},{\"gridX\":908,\"gridY\":96,\"type\":13},{\"gridX\":913,\"gridY\":96,\"type\":15},{\"gridX\":935,\"gridY\":96,\"type\":16},{\"gridX\":966,\"gridY\":140,\"type\":1},{\"gridX\":160,\"gridY\":116,\"type\":1},{\"gridX\":156,\"gridY\":116,\"type\":0},{\"gridX\":174,\"gridY\":116,\"type\":2},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":292,\"gridY\":116,\"type\":1},{\"gridX\":306,\"gridY\":116,\"type\":2},{\"gridX\":636,\"gridY\":140,\"type\":0},{\"gridX\":640,\"gridY\":140,\"type\":1},{\"gridX\":654,\"gridY\":140,\"type\":2},{\"gridX\":1238,\"gridY\":116,\"type\":0},{\"gridX\":1242,\"gridY\":116,\"type\":1},{\"gridX\":1256,\"gridY\":116,\"type\":2}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1632,\"gridY\":96}],\"enemies\":[{\"gridX\":212,\"gridY\":96,\"type\":8},{\"gridX\":472,\"gridY\":96,\"type\":8},{\"gridX\":1060,\"gridY\":96,\"type\":6},{\"gridX\":1292,\"gridY\":96,\"type\":8},{\"gridX\":1516,\"gridY\":96,\"type\":6},{\"gridX\":652,\"gridY\":96,\"type\":8},{\"gridX\":752,\"gridY\":96,\"type\":8},{\"gridX\":1036,\"gridY\":96,\"type\":8}],\"collectibles\":[{\"gridX\":384,\"gridY\":104,\"type\":0},{\"gridX\":388,\"gridY\":112,\"type\":0},{\"gridX\":392,\"gridY\":120,\"type\":0},{\"gridX\":396,\"gridY\":128,\"type\":0},{\"gridX\":424,\"gridY\":128,\"type\":0},{\"gridX\":416,\"gridY\":128,\"type\":0},{\"gridX\":432,\"gridY\":128,\"type\":0},{\"gridX\":540,\"gridY\":104,\"type\":0},{\"gridX\":532,\"gridY\":104,\"type\":0},{\"gridX\":548,\"gridY\":104,\"type\":0},{\"gridX\":552,\"gridY\":112,\"type\":0},{\"gridX\":556,\"gridY\":120,\"type\":0},{\"gridX\":560,\"gridY\":128,\"type\":0},{\"gridX\":564,\"gridY\":136,\"type\":0},{\"gridX\":592,\"gridY\":128,\"type\":0},{\"gridX\":600,\"gridY\":128,\"type\":0},{\"gridX\":608,\"gridY\":128,\"type\":0},{\"gridX\":688,\"gridY\":128,\"type\":0},{\"gridX\":696,\"gridY\":128,\"type\":0},{\"gridX\":704,\"gridY\":128,\"type\":0},{\"gridX\":400,\"gridY\":136,\"type\":1},{\"gridX\":260,\"gridY\":104,\"type\":0},{\"gridX\":264,\"gridY\":112,\"type\":0},{\"gridX\":268,\"gridY\":120,\"type\":0},{\"gridX\":272,\"gridY\":128,\"type\":0},{\"gridX\":276,\"gridY\":136,\"type\":0},{\"gridX\":336,\"gridY\":104,\"type\":0},{\"gridX\":344,\"gridY\":104,\"type\":0},{\"gridX\":328,\"gridY\":104,\"type\":0},{\"gridX\":764,\"gridY\":104,\"type\":0},{\"gridX\":780,\"gridY\":104,\"type\":0},{\"gridX\":772,\"gridY\":104,\"type\":0},{\"gridX\":136,\"gridY\":112,\"type\":0},{\"gridX\":132,\"gridY\":104,\"type\":0},{\"gridX\":140,\"gridY\":120,\"type\":0},{\"gridX\":144,\"gridY\":128,\"type\":0},{\"gridX\":148,\"gridY\":136,\"type\":0},{\"gridX\":868,\"gridY\":128,\"type\":0},{\"gridX\":860,\"gridY\":128,\"type\":0},{\"gridX\":876,\"gridY\":128,\"type\":0},{\"gridX\":884,\"gridY\":128,\"type\":0},{\"gridX\":976,\"gridY\":152,\"type\":0},{\"gridX\":952,\"gridY\":152,\"type\":0},{\"gridX\":960,\"gridY\":152,\"type\":0},{\"gridX\":968,\"gridY\":152,\"type\":0},{\"gridX\":1116,\"gridY\":180,\"type\":1},{\"gridX\":216,\"gridY\":112,\"type\":0},{\"gridX\":204,\"gridY\":104,\"type\":0},{\"gridX\":228,\"gridY\":104,\"type\":0},{\"gridX\":464,\"gridY\":104,\"type\":0},{\"gridX\":476,\"gridY\":112,\"type\":0},{\"gridX\":488,\"gridY\":104,\"type\":0},{\"gridX\":1100,\"gridY\":172,\"type\":0},{\"gridX\":1080,\"gridY\":132,\"type\":0},{\"gridX\":1084,\"gridY\":140,\"type\":0},{\"gridX\":1088,\"gridY\":148,\"type\":0},{\"gridX\":1092,\"gridY\":156,\"type\":0},{\"gridX\":1096,\"gridY\":164,\"type\":0},{\"gridX\":1220,\"gridY\":104,\"type\":0},{\"gridX\":1224,\"gridY\":112,\"type\":0},{\"gridX\":1228,\"gridY\":120,\"type\":0},{\"gridX\":1232,\"gridY\":128,\"type\":0},{\"gridX\":1260,\"gridY\":128,\"type\":0},{\"gridX\":1264,\"gridY\":120,\"type\":0},{\"gridX\":1268,\"gridY\":112,\"type\":0},{\"gridX\":1272,\"gridY\":104,\"type\":0},{\"gridX\":1132,\"gridY\":172,\"type\":0},{\"gridX\":1136,\"gridY\":164,\"type\":0},{\"gridX\":1140,\"gridY\":156,\"type\":0},{\"gridX\":1144,\"gridY\":148,\"type\":0},{\"gridX\":1148,\"gridY\":140,\"type\":0},{\"gridX\":1152,\"gridY\":132,\"type\":0},{\"gridX\":1580,\"gridY\":168,\"type\":1},{\"gridX\":1296,\"gridY\":112,\"type\":0},{\"gridX\":1284,\"gridY\":104,\"type\":0},{\"gridX\":1308,\"gridY\":104,\"type\":0},{\"gridX\":1356,\"gridY\":104,\"type\":0},{\"gridX\":1364,\"gridY\":120,\"type\":0},{\"gridX\":1452,\"gridY\":104,\"type\":0},{\"gridX\":824,\"gridY\":104,\"type\":0},{\"gridX\":828,\"gridY\":112,\"type\":0},{\"gridX\":832,\"gridY\":120,\"type\":0},{\"gridX\":888,\"gridY\":136,\"type\":0},{\"gridX\":892,\"gridY\":144,\"type\":0},{\"gridX\":1444,\"gridY\":104,\"type\":0},{\"gridX\":1460,\"gridY\":104,\"type\":0},{\"gridX\":1436,\"gridY\":104,\"type\":0},{\"gridX\":1464,\"gridY\":112,\"type\":0},{\"gridX\":1468,\"gridY\":120,\"type\":0},{\"gridX\":1472,\"gridY\":128,\"type\":0},{\"gridX\"" << ":1476,\"gridY\":136,\"type\":0},{\"gridX\":1544,\"gridY\":160,\"type\":0},{\"gridX\":1528,\"gridY\":128,\"type\":0},{\"gridX\":1532,\"gridY\":136,\"type\":0},{\"gridX\":1536,\"gridY\":144,\"type\":0},{\"gridX\":1540,\"gridY\":152,\"type\":0},{\"gridX\":680,\"gridY\":128,\"type\":0},{\"gridX\":944,\"gridY\":152,\"type\":0},{\"gridX\":836,\"gridY\":128,\"type\":0},{\"gridX\":936,\"gridY\":152,\"type\":0},{\"gridX\":840,\"gridY\":136,\"type\":0},{\"gridX\":1360,\"gridY\":112,\"type\":0},{\"gridX\":1428,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level2 *instance = new Chapter1Level2(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 3 ///

Chapter1Level3 * Chapter1Level3::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":3,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":56,\"gridY\":96,\"type\":0},{\"gridX\":92,\"gridY\":96,\"type\":1},{\"gridX\":256,\"gridY\":96,\"type\":2},{\"gridX\":372,\"gridY\":96,\"type\":1},{\"gridX\":312,\"gridY\":96,\"type\":0},{\"gridX\":412,\"gridY\":96,\"type\":2},{\"gridX\":468,\"gridY\":96,\"type\":1},{\"gridX\":520,\"gridY\":96,\"type\":0},{\"gridX\":568,\"gridY\":96,\"type\":2},{\"gridX\":648,\"gridY\":96,\"type\":1},{\"gridX\":700,\"gridY\":96,\"type\":0},{\"gridX\":760,\"gridY\":96,\"type\":1},{\"gridX\":796,\"gridY\":96,\"type\":0},{\"gridX\":864,\"gridY\":96,\"type\":0},{\"gridX\":940,\"gridY\":96,\"type\":2},{\"gridX\":896,\"gridY\":96,\"type\":0},{\"gridX\":1024,\"gridY\":96,\"type\":1},{\"gridX\":1064,\"gridY\":96,\"type\":0},{\"gridX\":1104,\"gridY\":96,\"type\":0},{\"gridX\":1140,\"gridY\":96,\"type\":0},{\"gridX\":1208,\"gridY\":96,\"type\":1},{\"gridX\":1260,\"gridY\":96,\"type\":2},{\"gridX\":1324,\"gridY\":96,\"type\":2},{\"gridX\":1384,\"gridY\":96,\"type\":0},{\"gridX\":1424,\"gridY\":96,\"type\":1},{\"gridX\":1476,\"gridY\":96,\"type\":1},{\"gridX\":1668,\"gridY\":96,\"type\":2},{\"gridX\":1752,\"gridY\":96,\"type\":1},{\"gridX\":1792,\"gridY\":96,\"type\":0},{\"gridX\":1824,\"gridY\":96,\"type\":2},{\"gridX\":1884,\"gridY\":96,\"type\":2},{\"gridX\":1948,\"gridY\":96,\"type\":2},{\"gridX\":2012,\"gridY\":96,\"type\":0},{\"gridX\":2056,\"gridY\":96,\"type\":2},{\"gridX\":2116,\"gridY\":96,\"type\":2},{\"gridX\":2176,\"gridY\":96,\"type\":1},{\"gridX\":2220,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":28},{\"gridX\":2056,\"gridY\":0,\"type\":28},{\"gridX\":2184,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":2312,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":136,\"gridY\":0,\"type\":32},{\"gridX\":1544,\"gridY\":0,\"type\":32}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":2056,\"gridY\":96,\"type\":12},{\"gridX\":340,\"gridY\":132,\"type\":0},{\"gridX\":344,\"gridY\":132,\"type\":1},{\"gridX\":372,\"gridY\":132,\"type\":2},{\"gridX\":358,\"gridY\":132,\"type\":1},{\"gridX\":504,\"gridY\":132,\"type\":0},{\"gridX\":508,\"gridY\":132,\"type\":1},{\"gridX\":522,\"gridY\":132,\"type\":1},{\"gridX\":536,\"gridY\":132,\"type\":2},{\"gridX\":432,\"gridY\":172,\"type\":0},{\"gridX\":436,\"gridY\":172,\"type\":1},{\"gridX\":450,\"gridY\":172,\"type\":1},{\"gridX\":464,\"gridY\":172,\"type\":2},{\"gridX\":728,\"gridY\":116,\"type\":0},{\"gridX\":732,\"gridY\":116,\"type\":1},{\"gridX\":746,\"gridY\":116,\"type\":1},{\"gridX\":760,\"gridY\":116,\"type\":2},{\"gridX\":828,\"gridY\":116,\"type\":0},{\"gridX\":832,\"gridY\":116,\"type\":1},{\"gridX\":860,\"gridY\":116,\"type\":2},{\"gridX\":846,\"gridY\":116,\"type\":1},{\"gridX\":916,\"gridY\":96,\"type\":10},{\"gridX\":891,\"gridY\":96,\"type\":13},{\"gridX\":896,\"gridY\":96,\"type\":14},{\"gridX\":907,\"gridY\":96,\"type\":16},{\"gridX\":1208,\"gridY\":96,\"type\":11},{\"gridX\":1328,\"gridY\":96,\"type\":8},{\"gridX\":1376,\"gridY\":96,\"type\":8},{\"gridX\":1620,\"gridY\":48,\"type\":3},{\"gridX\":1638,\"gridY\":48,\"type\":4},{\"gridX\":1652,\"gridY\":48,\"type\":4},{\"gridX\":1666,\"gridY\":48,\"type\":5},{\"gridX\":1624,\"gridY\":48,\"type\":4},{\"gridX\":1716,\"gridY\":116,\"type\":0},{\"gridX\":1720,\"gridY\":116,\"type\":1},{\"gridX\":1734,\"gridY\":116,\"type\":1},{\"gridX\":1748,\"gridY\":116,\"type\":2},{\"gridX\":1772,\"gridY\":140,\"type\":0},{\"gridX\":1776,\"gridY\":140,\"type\":1},{\"gridX\":1790,\"gridY\":140,\"type\":2},{\"gridX\":1816,\"gridY\":116,\"type\":0},{\"gridX\":1820,\"gridY\":116,\"type\":1},{\"gridX\":1834,\"gridY\":116,\"type\":2},{\"gridX\":1656,\"gridY\":53,\"type\":41}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":2024,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":400,\"gridY\":96,\"type\":8},{\"gridX\":488,\"gridY\":96,\"type\":8},{\"gridX\":584,\"gridY\":96,\"type\":8},{\"gridX\":800,\"gridY\":96,\"type\":8},{\"gridX\":1129,\"gridY\":96,\"type\":9},{\"gridX\":1168,\"gridY\":96,\"type\":9},{\"gridX\":1740,\"gridY\":121,\"type\":8},{\"gridX\":1820,\"gridY\":120,\"type\":9}],\"collectibles\":[{\"gridX\":144,\"gridY\":112,\"type\":0},{\"gridX\":148,\"gridY\":120,\"type\":0},{\"gridX\":152,\"gridY\":128,\"type\":0},{\"gridX\":252,\"gridY\":120,\"type\":0},{\"gridX\":212,\"gridY\":120,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":476,\"gridY\":228,\"type\":1},{\"gridX\":344,\"gridY\":144,\"type\":0},{\"gridX\":352,\"gridY\":144,\"type\":0},{\"gridX\":360,\"gridY\":144,\"type\":0},{\"gridX\":624,\"gridY\":124,\"type\":0},{\"gridX\":520,\"gridY\":144,\"type\":0},{\"gridX\":528,\"gridY\":144,\"type\":0},{\"gridX\":548,\"gridY\":136,\"type\":0},{\"gridX\":556,\"gridY\":128,\"type\":0},{\"gridX\":564,\"gridY\":120,\"type\":0},{\"gridX\":616,\"gridY\":116,\"type\":0},{\"gridX\":632,\"gridY\":128,\"type\":0},{\"gridX\":648,\"gridY\":116,\"type\":0},{\"gridX\":572,\"gridY\":112,\"type\":0},{\"gridX\":640,\"gridY\":124,\"type\":0},{\"gridX\":452,\"gridY\":184,\"type\":0},{\"gridX\":436,\"gridY\":184,\"type\":0},{\"gridX\":444,\"gridY\":184,\"type\":0},{\"gridX\":368,\"gridY\":144,\"type\":0},{\"gridX\":456,\"gridY\":192,\"type\":0},{\"gridX\":536,\"gridY\":144,\"type\":0},{\"gridX\":220,\"gridY\":128,\"type\":0},{\"gridX\":244,\"gridY\":128,\"type\":0},{\"gridX\":688,\"gridY\":104,\"type\":0},{\"gridX\":696,\"gridY\":104,\"type\":0},{\"gridX\":704,\"gridY\":104,\"type\":0},{\"gridX\":732,\"gridY\":128,\"type\":0},{\"gridX\":748,\"gridY\":128,\"type\":0},{\"gridX\":740,\"gridY\":128,\"type\":0},{\"gridX\":756,\"gridY\":128,\"type\":0},{\"gridX\":832,\"gridY\":128,\"type\":0},{\"gridX\":840,\"gridY\":128,\"type\":0},{\"gridX\":848,\"gridY\":128,\"type\":0},{\"gridX\":856,\"gridY\":128,\"type\":0},{\"gridX\":932,\"gridY\":132,\"type\":0},{\"gridX\":936,\"gridY\":140,\"type\":0},{\"gridX\":940,\"gridY\":148,\"type\":0},{\"gridX\":944,\"gridY\":156,\"type\":0},{\"gridX\":980,\"gridY\":172,\"type\":1},{\"gridX\":460,\"gridY\":200,\"type\":0},{\"gridX\":1020,\"gridY\":104,\"type\":0},{\"gridX\":1028,\"gridY\":104,\"type\":0},{\"gridX\":1036,\"gridY\":104,\"type\":0},{\"gridX\":1100,\"gridY\":120,\"type\":0},{\"gridX\":1104,\"gridY\":112,\"type\":0},{\"gridX\":1108,\"gridY\":104,\"type\":0},{\"gridX\":1120,\"gridY\":104,\"type\":0},{\"gridX\":1128,\"gridY\":112,\"type\":0},{\"gridX\":1136,\"gridY\":116,\"type\":0},{\"gridX\":1144,\"gridY\":112,\"type\":0},{\"gridX\":1152,\"gridY\":104,\"type\":0},{\"gridX\":1096,\"gridY\":128,\"type\":0},{\"gridX\":1172,\"gridY\":120,\"type\":0},{\"gridX\":1168,\"gridY\":112,\"type\":0},{\"gridX\":1164,\"gridY\":104,\"type\":0},{\"gridX\":1180,\"gridY\":124,\"type\":0},{\"gridX\":1188,\"gridY\":128,\"type\":0},{\"gridX\":1256,\"gridY\":104,\"type\":0},{\"gridX\":1264,\"gridY\":104,\"type\":0},{\"gridX\":1272,\"gridY\":104,\"type\":0},{\"gridX\":1336,\"gridY\":112,\"type\":0},{\"gridX\":1328,\"gridY\":112,\"type\":0},{\"gridX\":1344,\"gridY\":104,\"type\":0},{\"gridX\":1352,\"gridY\":104,\"type\":0},{\"gridX\":1360,\"gridY\":104,\"type\":0},{\"gridX\":1368,\"gridY\":112,\"type\":0},{\"gridX\":1376,\"gridY\":112,\"type\":0},{\"gridX\":1432,\"gridY\":104,\"type\":0},{\"gridX\":1416,\"gridY\":104,\"type\":0},{\"gridX\":1424,\"gridY\":104,\"t" << "ype\":0},{\"gridX\":1460,\"gridY\":104,\"type\":0},{\"gridX\":1468,\"gridY\":104,\"type\":0},{\"gridX\":1476,\"gridY\":104,\"type\":0},{\"gridX\":1512,\"gridY\":104,\"type\":0},{\"gridX\":1520,\"gridY\":104,\"type\":0},{\"gridX\":1528,\"gridY\":104,\"type\":0},{\"gridX\":1708,\"gridY\":104,\"type\":0},{\"gridX\":1624,\"gridY\":92,\"type\":0},{\"gridX\":1618,\"gridY\":92,\"type\":0},{\"gridX\":1616,\"gridY\":100,\"type\":0},{\"gridX\":1612,\"gridY\":104,\"type\":0},{\"gridX\":1624,\"gridY\":104,\"type\":0},{\"gridX\":1660,\"gridY\":84,\"type\":1},{\"gridX\":1624,\"gridY\":98,\"type\":0},{\"gridX\":1612,\"gridY\":92,\"type\":0},{\"gridX\":1716,\"gridY\":104,\"type\":0},{\"gridX\":1724,\"gridY\":104,\"type\":0},{\"gridX\":1608,\"gridY\":108,\"type\":0},{\"gridX\":1816,\"gridY\":128,\"type\":0},{\"gridX\":1824,\"gridY\":128,\"type\":0},{\"gridX\":1832,\"gridY\":128,\"type\":0},{\"gridX\":1944,\"gridY\":104,\"type\":0},{\"gridX\":1936,\"gridY\":104,\"type\":0},{\"gridX\":1952,\"gridY\":104,\"type\":0},{\"gridX\":1756,\"gridY\":112,\"type\":0},{\"gridX\":1764,\"gridY\":108,\"type\":0},{\"gridX\":1772,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":28,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level3 *instance = new Chapter1Level3(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 4 ///

Chapter1Level4 * Chapter1Level4::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":4,\"midgrounds\":[{\"gridX\":276,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1},{\"gridX\":428,\"gridY\":96,\"type\":0},{\"gridX\":736,\"gridY\":96,\"type\":1},{\"gridX\":704,\"gridY\":96,\"type\":0},{\"gridX\":537,\"gridY\":96,\"type\":2},{\"gridX\":1220,\"gridY\":96,\"type\":1},{\"gridX\":1264,\"gridY\":96,\"type\":0},{\"gridX\":1644,\"gridY\":96,\"type\":2},{\"gridX\":1444,\"gridY\":96,\"type\":1},{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":64,\"gridY\":96,\"type\":0},{\"gridX\":100,\"gridY\":96,\"type\":0},{\"gridX\":2024,\"gridY\":96,\"type\":2},{\"gridX\":2084,\"gridY\":96,\"type\":1},{\"gridX\":2128,\"gridY\":96,\"type\":0},{\"gridX\":2160,\"gridY\":96,\"type\":2},{\"gridX\":2220,\"gridY\":96,\"type\":0},{\"gridX\":1960,\"gridY\":96,\"type\":0},{\"gridX\":464,\"gridY\":96,\"type\":1},{\"gridX\":656,\"gridY\":96,\"type\":0},{\"gridX\":776,\"gridY\":96,\"type\":0},{\"gridX\":836,\"gridY\":96,\"type\":1},{\"gridX\":920,\"gridY\":96,\"type\":2},{\"gridX\":1084,\"gridY\":96,\"type\":2},{\"gridX\":1512,\"gridY\":96,\"type\":0},{\"gridX\":1572,\"gridY\":96,\"type\":0},{\"gridX\":1992,\"gridY\":96,\"type\":0},{\"gridX\":1000,\"gridY\":96,\"type\":1},{\"gridX\":1140,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":27},{\"gridX\":72,\"gridY\":0,\"type\":28},{\"gridX\":200,\"gridY\":0,\"type\":28},{\"gridX\":328,\"gridY\":0,\"type\":28},{\"gridX\":456,\"gridY\":0,\"type\":28},{\"gridX\":584,\"gridY\":0,\"type\":28},{\"gridX\":712,\"gridY\":0,\"type\":28},{\"gridX\":840,\"gridY\":0,\"type\":28},{\"gridX\":968,\"gridY\":0,\"type\":28},{\"gridX\":1096,\"gridY\":0,\"type\":28},{\"gridX\":1224,\"gridY\":0,\"type\":28},{\"gridX\":1352,\"gridY\":0,\"type\":28},{\"gridX\":1480,\"gridY\":0,\"type\":28},{\"gridX\":1608,\"gridY\":0,\"type\":28},{\"gridX\":1736,\"gridY\":0,\"type\":28},{\"gridX\":1864,\"gridY\":0,\"type\":28},{\"gridX\":1992,\"gridY\":0,\"type\":28},{\"gridX\":2120,\"gridY\":0,\"type\":28},{\"gridX\":2248,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":1064,\"gridY\":0,\"type\":30},{\"gridX\":1168,\"gridY\":0,\"type\":31},{\"gridX\":1316,\"gridY\":0,\"type\":32},{\"gridX\":1704,\"gridY\":0,\"type\":33},{\"gridX\":132,\"gridY\":0,\"type\":32},{\"gridX\":720,\"gridY\":0,\"type\":32}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":2016,\"gridY\":96,\"type\":12},{\"gridX\":336,\"gridY\":96,\"type\":10},{\"gridX\":416,\"gridY\":96,\"type\":8},{\"gridX\":504,\"gridY\":96,\"type\":7},{\"gridX\":603,\"gridY\":112,\"type\":1},{\"gridX\":617,\"gridY\":112,\"type\":1},{\"gridX\":631,\"gridY\":112,\"type\":1},{\"gridX\":645,\"gridY\":112,\"type\":2},{\"gridX\":599,\"gridY\":112,\"type\":0},{\"gridX\":592,\"gridY\":96,\"type\":24},{\"gridX\":624,\"gridY\":96,\"type\":24},{\"gridX\":656,\"gridY\":96,\"type\":24},{\"gridX\":689,\"gridY\":96,\"type\":24},{\"gridX\":851,\"gridY\":96,\"type\":24},{\"gridX\":883,\"gridY\":96,\"type\":24},{\"gridX\":689,\"gridY\":135,\"type\":1},{\"gridX\":703,\"gridY\":135,\"type\":2},{\"gridX\":685,\"gridY\":135,\"type\":0},{\"gridX\":812,\"gridY\":128,\"type\":1},{\"gridX\":826,\"gridY\":128,\"type\":2},{\"gridX\":808,\"gridY\":128,\"type\":0},{\"gridX\":882,\"gridY\":112,\"type\":1},{\"gridX\":878,\"gridY\":112,\"type\":0},{\"gridX\":910,\"gridY\":112,\"type\":2},{\"gridX\":896,\"gridY\":112,\"type\":1},{\"gridX\":1492,\"gridY\":96,\"type\":24},{\"gridX\":1560,\"gridY\":96,\"type\":23},{\"gridX\":1624,\"gridY\":96,\"type\":22},{\"gridX\":1740,\"gridY\":132,\"type\":1},{\"gridX\":1736,\"gridY\":132,\"type\":0},{\"gridX\":1754,\"gridY\":132,\"type\":2},{\"gridX\":1804,\"gridY\":160,\"type\":1},{\"gridX\":1800,\"gridY\":160,\"type\":0},{\"gridX\":1818,\"gridY\":160,\"type\":1},{\"gridX\":1832,\"gridY\":160,\"type\":1},{\"gridX\":1846,\"gridY\":160,\"type\":1},{\"gridX\":1860,\"gridY\":160,\"type\":1},{\"gridX\":1874,\"gridY\":160,\"type\":1},{\"gridX\":1888,\"gridY\":160,\"type\":1},{\"gridX\":1902,\"gridY\":160,\"type\":2},{\"gridX\":560,\"gridY\":96,\"type\":24},{\"gridX\":528,\"gridY\":96,\"type\":24},{\"gridX\":915,\"gridY\":96,\"type\":24},{\"gridX\":947,\"gridY\":96,\"type\":24},{\"gridX\":978,\"gridY\":96,\"type\":23},{\"gridX\":483,\"gridY\":96,\"type\":13},{\"gridX\":488,\"gridY\":96,\"type\":14},{\"gridX\":499,\"gridY\":96,\"type\":16},{\"gridX\":160,\"gridY\":116,\"type\":1},{\"gridX\":156,\"gridY\":116,\"type\":0},{\"gridX\":174,\"gridY\":116,\"type\":2},{\"gridX\":188,\"gridY\":96,\"type\":0},{\"gridX\":192,\"gridY\":96,\"type\":1},{\"gridX\":206,\"gridY\":96,\"type\":2},{\"gridX\":1852,\"gridY\":140,\"type\":34},{\"gridX\":1852,\"gridY\":124,\"type\":34},{\"gridX\":1852,\"gridY\":108,\"type\":34}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1984,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":256,\"gridY\":108,\"type\":0},{\"gridX\":312,\"gridY\":104,\"type\":0},{\"gridX\":320,\"gridY\":112,\"type\":0},{\"gridX\":328,\"gridY\":120,\"type\":0},{\"gridX\":336,\"gridY\":128,\"type\":0},{\"gridX\":344,\"gridY\":128,\"type\":0},{\"gridX\":352,\"gridY\":128,\"type\":0},{\"gridX\":416,\"gridY\":136,\"type\":0},{\"gridX\":408,\"gridY\":128,\"type\":0},{\"gridX\":424,\"gridY\":136,\"type\":0},{\"gridX\":400,\"gridY\":120,\"type\":0},{\"gridX\":416,\"gridY\":128,\"type\":0},{\"gridX\":587,\"gridY\":145,\"type\":0},{\"gridX\":571,\"gridY\":162,\"type\":0},{\"gridX\":528,\"gridY\":140,\"type\":0},{\"gridX\":561,\"gridY\":165,\"type\":1},{\"gridX\":408,\"gridY\":120,\"type\":0},{\"gridX\":537,\"gridY\":150,\"type\":0},{\"gridX\":579,\"gridY\":155,\"type\":0},{\"gridX\":624,\"gridY\":124,\"type\":0},{\"gridX\":632,\"gridY\":124,\"type\":0},{\"gridX\":640,\"gridY\":124,\"type\":0},{\"gridX\":648,\"gridY\":124,\"type\":0},{\"gridX\":616,\"gridY\":124,\"type\":0},{\"gridX\":688,\"gridY\":148,\"type\":0},{\"gridX\":696,\"gridY\":148,\"type\":0},{\"gridX\":704,\"gridY\":148,\"type\":0},{\"gridX\":824,\"gridY\":140,\"type\":0},{\"gridX\":828,\"gridY\":148,\"type\":0},{\"gridX\":816,\"gridY\":140,\"type\":0},{\"gridX\":912,\"gridY\":132,\"type\":0},{\"gridX\":900,\"gridY\":124,\"type\":0},{\"gridX\":908,\"gridY\":124,\"type\":0},{\"gridX\":748,\"gridY\":164,\"type\":0},{\"gridX\":740,\"gridY\":168,\"type\":0},{\"gridX\":756,\"gridY\":160,\"type\":0},{\"gridX\":1076,\"gridY\":88,\"type\":1},{\"gridX\":1064,\"gridY\":104,\"type\":0},{\"gridX\":1032,\"gridY\":104,\"type\":0},{\"gridX\":1040,\"gridY\":104,\"type\":0},{\"gridX\":1048,\"gridY\":104,\"type\":0},{\"gridX\":1056,\"gridY\":104,\"type\":0},{\"gridX\":1120,\"gridY\":104,\"type\":0},{\"gridX\":1128,\"gridY\":104,\"type\":0},{\"gridX\":1136,\"gridY\":104,\"type\":0},{\"gridX\":1164,\"gridY\":104,\"type\":0},{\"gridX\":1168,\"gridY\":112,\"type\":0},{\"gridX\":1172,\"gridY\":120,\"type\":0},{\"gridX\":1176,\"gridY\":128,\"type\":0},{\"gridX\":1180,\"gridY\":136,\"type\":0},{\"gridX\":1384,\"gridY\":128,\"type\":0},{\"gridX\":1392,\"gridY\":132,\"type\":0},{\"gridX\":1400,\"gridY\":136,\"type\":0},{\"gridX\":1248,\"gridY\":104,\"type\":0},{\"gridX\":1256,\"gridY\":104,\"type\":0},{\"gridX\":1264,\"gridY\":104,\"type\":0},{\"gridX\":1304,\"gridY\":104,\"type\":0},{\"gridX\":1288,\"gridY\":104,\"type\":0},{\"gridX\":1296,\"gridY\":104,\"type\":0},{\"gridX\":1536,\"gridY\":104,\"type\":0},{\"gridX\":1532,\"gridY\":108,\"type\":0},{\"gridX\":1592,\"gridY\":108,\"type\":0},{\"gridX\":1596,\"gridY\":104,\"type\":0},{\"gridX\":1500,\"gridY\":124,\"type\":0},{\"gridX\":1508,\"gridY\":124,\"type\":0},{\"gridX\":1504,\"gridY\":128,\"type\":0},{\"gridX\":1560,\"gridY\":124,\"type\":0},{\"gridX\":1568,\"gridY\":124,\"type\":0},{\"gridX\":1564,\"gridY\":128,\"type\":0},{\"gridX\":1620,\"grid" << "Y\":124,\"type\":0},{\"gridX\":1636,\"gridY\":132,\"type\":0},{\"gridX\":1628,\"gridY\":128,\"type\":0},{\"gridX\":1748,\"gridY\":144,\"type\":0},{\"gridX\":1756,\"gridY\":144,\"type\":0},{\"gridX\":1812,\"gridY\":172,\"type\":0},{\"gridX\":1820,\"gridY\":172,\"type\":0},{\"gridX\":1828,\"gridY\":172,\"type\":0},{\"gridX\":1992,\"gridY\":128,\"type\":0},{\"gridX\":1844,\"gridY\":172,\"type\":0},{\"gridX\":1852,\"gridY\":172,\"type\":0},{\"gridX\":1860,\"gridY\":172,\"type\":0},{\"gridX\":2000,\"gridY\":124,\"type\":0},{\"gridX\":1760,\"gridY\":152,\"type\":0},{\"gridX\":1876,\"gridY\":172,\"type\":0},{\"gridX\":1884,\"gridY\":172,\"type\":0},{\"gridX\":1892,\"gridY\":172,\"type\":0},{\"gridX\":1952,\"gridY\":124,\"type\":0},{\"gridX\":1984,\"gridY\":132,\"type\":0},{\"gridX\":1912,\"gridY\":164,\"type\":0},{\"gridX\":1920,\"gridY\":156,\"type\":0},{\"gridX\":1928,\"gridY\":148,\"type\":0},{\"gridX\":1936,\"gridY\":140,\"type\":0},{\"gridX\":1944,\"gridY\":132,\"type\":0},{\"gridX\":1776,\"gridY\":172,\"type\":1},{\"gridX\":548,\"gridY\":159,\"type\":0},{\"gridX\":521,\"gridY\":128,\"type\":0},{\"gridX\":1540,\"gridY\":108,\"type\":0},{\"gridX\":1600,\"gridY\":108,\"type\":0}],\"jons\":[{\"gridX\":28,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level4 *instance = new Chapter1Level4(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 5 ///

Chapter1Level5 * Chapter1Level5::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":5,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":60,\"gridY\":96,\"type\":0},{\"gridX\":92,\"gridY\":96,\"type\":2},{\"gridX\":212,\"gridY\":96,\"type\":0},{\"gridX\":152,\"gridY\":96,\"type\":2},{\"gridX\":248,\"gridY\":96,\"type\":1},{\"gridX\":536,\"gridY\":96,\"type\":2},{\"gridX\":600,\"gridY\":96,\"type\":1},{\"gridX\":640,\"gridY\":96,\"type\":0},{\"gridX\":704,\"gridY\":96,\"type\":2},{\"gridX\":800,\"gridY\":96,\"type\":0},{\"gridX\":840,\"gridY\":96,\"type\":1},{\"gridX\":884,\"gridY\":96,\"type\":2},{\"gridX\":1164,\"gridY\":96,\"type\":2},{\"gridX\":1220,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":292,\"gridY\":0,\"type\":33},{\"gridX\":922,\"gridY\":0,\"type\":33}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1664,\"gridY\":96,\"type\":12},{\"gridX\":128,\"gridY\":96,\"type\":41},{\"gridX\":198,\"gridY\":150,\"type\":0},{\"gridX\":258,\"gridY\":150,\"type\":2},{\"gridX\":202,\"gridY\":150,\"type\":1},{\"gridX\":216,\"gridY\":150,\"type\":1},{\"gridX\":230,\"gridY\":150,\"type\":1},{\"gridX\":668,\"gridY\":116,\"type\":0},{\"gridX\":672,\"gridY\":116,\"type\":1},{\"gridX\":686,\"gridY\":116,\"type\":1},{\"gridX\":700,\"gridY\":116,\"type\":2},{\"gridX\":244,\"gridY\":150,\"type\":1},{\"gridX\":665,\"gridY\":96,\"type\":13},{\"gridX\":703,\"gridY\":96,\"type\":16},{\"gridX\":670,\"gridY\":96,\"type\":15},{\"gridX\":143,\"gridY\":96,\"type\":13},{\"gridX\":148,\"gridY\":96,\"type\":15},{\"gridX\":209,\"gridY\":96,\"type\":15},{\"gridX\":170,\"gridY\":96,\"type\":16},{\"gridX\":180,\"gridY\":96,\"type\":20},{\"gridX\":204,\"gridY\":96,\"type\":13},{\"gridX\":231,\"gridY\":96,\"type\":15},{\"gridX\":253,\"gridY\":96,\"type\":15},{\"gridX\":275,\"gridY\":96,\"type\":16},{\"gridX\":540,\"gridY\":224,\"type\":34},{\"gridX\":540,\"gridY\":208,\"type\":34},{\"gridX\":540,\"gridY\":240,\"type\":34},{\"gridX\":540,\"gridY\":192,\"type\":34},{\"gridX\":540,\"gridY\":176,\"type\":34},{\"gridX\":776,\"gridY\":116,\"type\":0},{\"gridX\":780,\"gridY\":116,\"type\":1},{\"gridX\":794,\"gridY\":116,\"type\":2},{\"gridX\":808,\"gridY\":140,\"type\":0},{\"gridX\":812,\"gridY\":140,\"type\":1},{\"gridX\":840,\"gridY\":140,\"type\":2},{\"gridX\":864,\"gridY\":164,\"type\":0},{\"gridX\":868,\"gridY\":164,\"type\":1},{\"gridX\":882,\"gridY\":164,\"type\":1},{\"gridX\":910,\"gridY\":164,\"type\":2},{\"gridX\":896,\"gridY\":164,\"type\":1},{\"gridX\":740,\"gridY\":96,\"type\":19},{\"gridX\":826,\"gridY\":140,\"type\":1},{\"gridX\":692,\"gridY\":96,\"type\":14},{\"gridX\":834,\"gridY\":96,\"type\":13},{\"gridX\":839,\"gridY\":96,\"type\":15},{\"gridX\":861,\"gridY\":96,\"type\":15},{\"gridX\":883,\"gridY\":96,\"type\":15},{\"gridX\":905,\"gridY\":96,\"type\":14},{\"gridX\":916,\"gridY\":96,\"type\":16},{\"gridX\":1328,\"gridY\":116,\"type\":0},{\"gridX\":1332,\"gridY\":116,\"type\":1},{\"gridX\":1346,\"gridY\":116,\"type\":2},{\"gridX\":1424,\"gridY\":116,\"type\":0},{\"gridX\":1428,\"gridY\":116,\"type\":1},{\"gridX\":1442,\"gridY\":116,\"type\":2},{\"gridX\":1328,\"gridY\":96,\"type\":13},{\"gridX\":1333,\"gridY\":96,\"type\":14},{\"gridX\":1344,\"gridY\":96,\"type\":16},{\"gridX\":1424,\"gridY\":96,\"type\":13},{\"gridX\":276,\"gridY\":112,\"type\":0},{\"gridX\":280,\"gridY\":112,\"type\":1},{\"gridX\":294,\"gridY\":112,\"type\":2}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1632,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":252,\"gridY\":172,\"type\":3},{\"gridX\":816,\"gridY\":96,\"type\":8},{\"gridX\":908,\"gridY\":188,\"type\":3},{\"gridX\":1244,\"gridY\":96,\"type\":9},{\"gridX\":332,\"gridY\":96,\"type\":3},{\"gridX\":380,\"gridY\":96,\"type\":3},{\"gridX\":428,\"gridY\":96,\"type\":3}],\"collectibles\":[{\"gridX\":672,\"gridY\":128,\"type\":0},{\"gridX\":1276,\"gridY\":104,\"type\":0},{\"gridX\":1268,\"gridY\":104,\"type\":0},{\"gridX\":1260,\"gridY\":104,\"type\":0},{\"gridX\":680,\"gridY\":128,\"type\":0},{\"gridX\":688,\"gridY\":128,\"type\":0},{\"gridX\":284,\"gridY\":236,\"type\":1},{\"gridX\":696,\"gridY\":128,\"type\":0},{\"gridX\":724,\"gridY\":116,\"type\":0},{\"gridX\":732,\"gridY\":112,\"type\":0},{\"gridX\":740,\"gridY\":108,\"type\":0},{\"gridX\":776,\"gridY\":128,\"type\":0},{\"gridX\":784,\"gridY\":128,\"type\":0},{\"gridX\":792,\"gridY\":128,\"type\":0},{\"gridX\":832,\"gridY\":152,\"type\":0},{\"gridX\":816,\"gridY\":152,\"type\":0},{\"gridX\":824,\"gridY\":152,\"type\":0},{\"gridX\":864,\"gridY\":176,\"type\":0},{\"gridX\":872,\"gridY\":176,\"type\":0},{\"gridX\":880,\"gridY\":176,\"type\":0},{\"gridX\":896,\"gridY\":176,\"type\":0},{\"gridX\":904,\"gridY\":176,\"type\":0},{\"gridX\":912,\"gridY\":176,\"type\":0},{\"gridX\":1180,\"gridY\":104,\"type\":1},{\"gridX\":928,\"gridY\":184,\"type\":0},{\"gridX\":936,\"gridY\":184,\"type\":0},{\"gridX\":968,\"gridY\":180,\"type\":0},{\"gridX\":976,\"gridY\":180,\"type\":0},{\"gridX\":1008,\"gridY\":172,\"type\":0},{\"gridX\":1016,\"gridY\":172,\"type\":0},{\"gridX\":1056,\"gridY\":164,\"type\":0},{\"gridX\":1048,\"gridY\":164,\"type\":0},{\"gridX\":1088,\"gridY\":156,\"type\":0},{\"gridX\":1096,\"gridY\":156,\"type\":0},{\"gridX\":1136,\"gridY\":148,\"type\":0},{\"gridX\":1128,\"gridY\":148,\"type\":0},{\"gridX\":596,\"gridY\":104,\"type\":0},{\"gridX\":604,\"gridY\":104,\"type\":0},{\"gridX\":612,\"gridY\":104,\"type\":0},{\"gridX\":636,\"gridY\":112,\"type\":0},{\"gridX\":640,\"gridY\":120,\"type\":0},{\"gridX\":644,\"gridY\":128,\"type\":0},{\"gridX\":136,\"gridY\":112,\"type\":0},{\"gridX\":140,\"gridY\":120,\"type\":0},{\"gridX\":144,\"gridY\":128,\"type\":0},{\"gridX\":208,\"gridY\":164,\"type\":0},{\"gridX\":200,\"gridY\":164,\"type\":0},{\"gridX\":1328,\"gridY\":128,\"type\":0},{\"gridX\":1336,\"gridY\":128,\"type\":0},{\"gridX\":1344,\"gridY\":128,\"type\":0},{\"gridX\":1392,\"gridY\":108,\"type\":0},{\"gridX\":1376,\"gridY\":108,\"type\":0},{\"gridX\":1384,\"gridY\":108,\"type\":0},{\"gridX\":1424,\"gridY\":128,\"type\":0},{\"gridX\":1432,\"gridY\":128,\"type\":0},{\"gridX\":1440,\"gridY\":128,\"type\":0},{\"gridX\":188,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":196,\"gridY\":140,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":96},{\"gridX\":1188,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level5 *instance = new Chapter1Level5(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 6 ///

Chapter1Level6 * Chapter1Level6::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":6,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":60,\"gridY\":96,\"type\":0},{\"gridX\":92,\"gridY\":96,\"type\":2},{\"gridX\":212,\"gridY\":96,\"type\":0},{\"gridX\":152,\"gridY\":96,\"type\":2},{\"gridX\":248,\"gridY\":96,\"type\":1},{\"gridX\":536,\"gridY\":96,\"type\":2},{\"gridX\":596,\"gridY\":96,\"type\":1},{\"gridX\":636,\"gridY\":96,\"type\":0},{\"gridX\":696,\"gridY\":96,\"type\":2},{\"gridX\":756,\"gridY\":96,\"type\":0},{\"gridX\":800,\"gridY\":96,\"type\":1},{\"gridX\":840,\"gridY\":96,\"type\":2},{\"gridX\":1164,\"gridY\":96,\"type\":2},{\"gridX\":1228,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":292,\"gridY\":0,\"type\":33},{\"gridX\":900,\"gridY\":0,\"type\":33}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1664,\"gridY\":96,\"type\":12},{\"gridX\":128,\"gridY\":96,\"type\":41},{\"gridX\":188,\"gridY\":96,\"type\":24},{\"gridX\":224,\"gridY\":96,\"type\":24},{\"gridX\":260,\"gridY\":96,\"type\":24},{\"gridX\":198,\"gridY\":150,\"type\":0},{\"gridX\":258,\"gridY\":150,\"type\":2},{\"gridX\":202,\"gridY\":150,\"type\":1},{\"gridX\":216,\"gridY\":150,\"type\":1},{\"gridX\":230,\"gridY\":150,\"type\":1},{\"gridX\":668,\"gridY\":116,\"type\":0},{\"gridX\":672,\"gridY\":116,\"type\":1},{\"gridX\":686,\"gridY\":116,\"type\":1},{\"gridX\":700,\"gridY\":116,\"type\":2},{\"gridX\":756,\"gridY\":140,\"type\":0},{\"gridX\":774,\"gridY\":140,\"type\":1},{\"gridX\":872,\"gridY\":140,\"type\":2},{\"gridX\":760,\"gridY\":140,\"type\":1},{\"gridX\":788,\"gridY\":140,\"type\":1},{\"gridX\":802,\"gridY\":140,\"type\":1},{\"gridX\":816,\"gridY\":140,\"type\":1},{\"gridX\":830,\"gridY\":140,\"type\":1},{\"gridX\":844,\"gridY\":140,\"type\":1},{\"gridX\":858,\"gridY\":140,\"type\":1},{\"gridX\":244,\"gridY\":150,\"type\":1},{\"gridX\":152,\"gridY\":96,\"type\":24},{\"gridX\":736,\"gridY\":96,\"type\":19},{\"gridX\":665,\"gridY\":96,\"type\":13},{\"gridX\":692,\"gridY\":96,\"type\":15},{\"gridX\":714,\"gridY\":96,\"type\":16},{\"gridX\":670,\"gridY\":96,\"type\":15}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1632,\"gridY\":96}],\"enemies\":[{\"gridX\":280,\"gridY\":140,\"type\":3},{\"gridX\":896,\"gridY\":128,\"type\":3},{\"gridX\":960,\"gridY\":112,\"type\":3},{\"gridX\":1024,\"gridY\":112,\"type\":3},{\"gridX\":1112,\"gridY\":96,\"type\":3},{\"gridX\":308,\"gridY\":136,\"type\":3},{\"gridX\":336,\"gridY\":132,\"type\":3},{\"gridX\":364,\"gridY\":128,\"type\":3},{\"gridX\":392,\"gridY\":124,\"type\":3},{\"gridX\":420,\"gridY\":120,\"type\":3},{\"gridX\":460,\"gridY\":100,\"type\":3},{\"gridX\":500,\"gridY\":100,\"type\":3}],\"collectibles\":[{\"gridX\":672,\"gridY\":128,\"type\":0},{\"gridX\":744,\"gridY\":116,\"type\":0},{\"gridX\":748,\"gridY\":124,\"type\":0},{\"gridX\":996,\"gridY\":148,\"type\":0},{\"gridX\":932,\"gridY\":160,\"type\":0},{\"gridX\":1072,\"gridY\":148,\"type\":0},{\"gridX\":1156,\"gridY\":136,\"type\":0},{\"gridX\":1144,\"gridY\":128,\"type\":0},{\"gridX\":912,\"gridY\":148,\"type\":0},{\"gridX\":952,\"gridY\":148,\"type\":0},{\"gridX\":976,\"gridY\":136,\"type\":0},{\"gridX\":1016,\"gridY\":136,\"type\":0},{\"gridX\":1052,\"gridY\":136,\"type\":0},{\"gridX\":1092,\"gridY\":136,\"type\":0},{\"gridX\":1132,\"gridY\":120,\"type\":0},{\"gridX\":1228,\"gridY\":104,\"type\":0},{\"gridX\":1220,\"gridY\":104,\"type\":0},{\"gridX\":1236,\"gridY\":104,\"type\":0},{\"gridX\":752,\"gridY\":132,\"type\":1},{\"gridX\":684,\"gridY\":128,\"type\":0},{\"gridX\":696,\"gridY\":128,\"type\":0},{\"gridX\":832,\"gridY\":104,\"type\":0},{\"gridX\":840,\"gridY\":104,\"type\":0},{\"gridX\":848,\"gridY\":104,\"type\":0},{\"gridX\":200,\"gridY\":164,\"type\":0},{\"gridX\":208,\"gridY\":164,\"type\":0},{\"gridX\":216,\"gridY\":164,\"type\":0},{\"gridX\":232,\"gridY\":164,\"type\":0},{\"gridX\":240,\"gridY\":164,\"type\":0},{\"gridX\":248,\"gridY\":164,\"type\":0},{\"gridX\":272,\"gridY\":156,\"type\":0},{\"gridX\":280,\"gridY\":152,\"type\":0},{\"gridX\":288,\"gridY\":156,\"type\":0},{\"gridX\":296,\"gridY\":156,\"type\":0},{\"gridX\":304,\"gridY\":152,\"type\":0},{\"gridX\":312,\"gridY\":148,\"type\":0},{\"gridX\":320,\"gridY\":152,\"type\":0},{\"gridX\":328,\"gridY\":152,\"type\":0},{\"gridX\":336,\"gridY\":148,\"type\":0},{\"gridX\":344,\"gridY\":144,\"type\":0},{\"gridX\":352,\"gridY\":148,\"type\":0},{\"gridX\":360,\"gridY\":144,\"type\":0},{\"gridX\":368,\"gridY\":140,\"type\":0},{\"gridX\":376,\"gridY\":144,\"type\":0},{\"gridX\":384,\"gridY\":144,\"type\":0},{\"gridX\":392,\"gridY\":140,\"type\":0},{\"gridX\":400,\"gridY\":136,\"type\":0},{\"gridX\":408,\"gridY\":140,\"type\":0},{\"gridX\":416,\"gridY\":140,\"type\":0},{\"gridX\":424,\"gridY\":136,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level6 *instance = new Chapter1Level6(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 7 ///

Chapter1Level7 * Chapter1Level7::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":7,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1664,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1632,\"gridY\":96}],\"enemies\":[],\"collectibles\":[],\"jons\":[{\"gridX\":24,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level7 *instance = new Chapter1Level7(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 8 ///

Chapter1Level8 * Chapter1Level8::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":8,\"midgrounds\":[{\"gridX\":146,\"gridY\":96,\"type\":2},{\"gridX\":38,\"gridY\":96,\"type\":0},{\"gridX\":68,\"gridY\":96,\"type\":1},{\"gridX\":532,\"gridY\":96,\"type\":2},{\"gridX\":471,\"gridY\":96,\"type\":0},{\"gridX\":723,\"gridY\":96,\"type\":2},{\"gridX\":705,\"gridY\":96,\"type\":0},{\"gridX\":1008,\"gridY\":96,\"type\":1},{\"gridX\":1045,\"gridY\":96,\"type\":2},{\"gridX\":1107,\"gridY\":96,\"type\":0},{\"gridX\":1180,\"gridY\":96,\"type\":2},{\"gridX\":1140,\"gridY\":96,\"type\":1},{\"gridX\":898,\"gridY\":96,\"type\":2},{\"gridX\":790,\"gridY\":96,\"type\":1},{\"gridX\":1573,\"gridY\":96,\"type\":1},{\"gridX\":1501,\"gridY\":96,\"type\":2},{\"gridX\":1549,\"gridY\":96,\"type\":0},{\"gridX\":1680,\"gridY\":96,\"type\":1},{\"gridX\":1727,\"gridY\":96,\"type\":0},{\"gridX\":1623,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":184,\"gridY\":0,\"type\":33},{\"gridX\":578,\"gridY\":0,\"type\":30},{\"gridX\":507,\"gridY\":0,\"type\":30},{\"gridX\":628,\"gridY\":0,\"type\":30},{\"gridX\":1226,\"gridY\":0,\"type\":33}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1664,\"gridY\":96,\"type\":12},{\"gridX\":96,\"gridY\":104,\"type\":34},{\"gridX\":104,\"gridY\":120,\"type\":34},{\"gridX\":120,\"gridY\":128,\"type\":34},{\"gridX\":246,\"gridY\":120,\"type\":1},{\"gridX\":242,\"gridY\":120,\"type\":0},{\"gridX\":260,\"gridY\":120,\"type\":2},{\"gridX\":328,\"gridY\":154,\"type\":1},{\"gridX\":342,\"gridY\":154,\"type\":2},{\"gridX\":324,\"gridY\":154,\"type\":0},{\"gridX\":612,\"gridY\":96,\"type\":23},{\"gridX\":758,\"gridY\":138,\"type\":1},{\"gridX\":832,\"gridY\":190,\"type\":1},{\"gridX\":784,\"gridY\":104,\"type\":34},{\"gridX\":808,\"gridY\":104,\"type\":34},{\"gridX\":832,\"gridY\":104,\"type\":34},{\"gridX\":856,\"gridY\":104,\"type\":34},{\"gridX\":880,\"gridY\":104,\"type\":34},{\"gridX\":904,\"gridY\":104,\"type\":34},{\"gridX\":976,\"gridY\":104,\"type\":34},{\"gridX\":1000,\"gridY\":104,\"type\":34},{\"gridX\":1034,\"gridY\":139,\"type\":1},{\"gridX\":1081,\"gridY\":215,\"type\":1},{\"gridX\":993,\"gridY\":128,\"type\":1},{\"gridX\":1266,\"gridY\":200,\"type\":1},{\"gridX\":1280,\"gridY\":200,\"type\":1},{\"gridX\":1314,\"gridY\":189,\"type\":1},{\"gridX\":1327,\"gridY\":189,\"type\":1},{\"gridX\":1340,\"gridY\":189,\"type\":1},{\"gridX\":1402,\"gridY\":194,\"type\":1},{\"gridX\":1416,\"gridY\":194,\"type\":1},{\"gridX\":1429,\"gridY\":194,\"type\":1},{\"gridX\":1480,\"gridY\":200,\"type\":34},{\"gridX\":1480,\"gridY\":184,\"type\":34},{\"gridX\":1480,\"gridY\":168,\"type\":34},{\"gridX\":1480,\"gridY\":152,\"type\":34},{\"gridX\":1480,\"gridY\":136,\"type\":34},{\"gridX\":1480,\"gridY\":120,\"type\":34},{\"gridX\":1480,\"gridY\":104,\"type\":34},{\"gridX\":887,\"gridY\":178,\"type\":1},{\"gridX\":846,\"gridY\":190,\"type\":1},{\"gridX\":831,\"gridY\":149,\"type\":1},{\"gridX\":784,\"gridY\":120,\"type\":34},{\"gridX\":784,\"gridY\":136,\"type\":34},{\"gridX\":792,\"gridY\":152,\"type\":34},{\"gridX\":800,\"gridY\":168,\"type\":34},{\"gridX\":816,\"gridY\":176,\"type\":34},{\"gridX\":845,\"gridY\":149,\"type\":1},{\"gridX\":827,\"gridY\":149,\"type\":0},{\"gridX\":859,\"gridY\":149,\"type\":1},{\"gridX\":873,\"gridY\":149,\"type\":1},{\"gridX\":887,\"gridY\":149,\"type\":1},{\"gridX\":901,\"gridY\":149,\"type\":1},{\"gridX\":915,\"gridY\":149,\"type\":2},{\"gridX\":829,\"gridY\":190,\"type\":0},{\"gridX\":860,\"gridY\":190,\"type\":2},{\"gridX\":883,\"gridY\":178,\"type\":0},{\"gridX\":900,\"gridY\":178,\"type\":2},{\"gridX\":771,\"gridY\":138,\"type\":2},{\"gridX\":754,\"gridY\":138,\"type\":0},{\"gridX\":888,\"gridY\":161,\"type\":34},{\"gridX\":1094,\"gridY\":123,\"type\":1},{\"gridX\":1117,\"gridY\":171,\"type\":1},{\"gridX\":1130,\"gridY\":171,\"type\":1},{\"gridX\":1143,\"gridY\":171,\"type\":1},{\"gridX\":1114,\"gridY\":171,\"type\":0},{\"gridX\":1157,\"gridY\":171,\"type\":2},{\"gridX\":989,\"gridY\":128,\"type\":0},{\"gridX\":1007,\"gridY\":128,\"type\":2},{\"gridX\":1263,\"gridY\":200,\"type\":0},{\"gridX\":1294,\"gridY\":200,\"type\":2},{\"gridX\":1310,\"gridY\":189,\"type\":0},{\"gridX\":1353,\"gridY\":189,\"type\":2},{\"gridX\":1399,\"gridY\":194,\"type\":0},{\"gridX\":1442,\"gridY\":194,\"type\":2},{\"gridX\":926,\"gridY\":103,\"type\":34},{\"gridX\":1030,\"gridY\":139,\"type\":0},{\"gridX\":1048,\"gridY\":139,\"type\":2},{\"gridX\":1090,\"gridY\":123,\"type\":0},{\"gridX\":1108,\"gridY\":123,\"type\":2},{\"gridX\":1077,\"gridY\":215,\"type\":0},{\"gridX\":1095,\"gridY\":215,\"type\":2},{\"gridX\":1373,\"gridY\":197,\"type\":34},{\"gridX\":1373,\"gridY\":217,\"type\":34},{\"gridX\":1480,\"gridY\":216,\"type\":34}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1632,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":152,\"gridY\":96,\"type\":6},{\"gridX\":243,\"gridY\":125,\"type\":6},{\"gridX\":453,\"gridY\":96,\"type\":6},{\"gridX\":683,\"gridY\":96,\"type\":6},{\"gridX\":757,\"gridY\":143,\"type\":6},{\"gridX\":881,\"gridY\":183,\"type\":6},{\"gridX\":825,\"gridY\":154,\"type\":6},{\"gridX\":949,\"gridY\":97,\"type\":6},{\"gridX\":1034,\"gridY\":144,\"type\":6},{\"gridX\":1076,\"gridY\":220,\"type\":6},{\"gridX\":992,\"gridY\":133,\"type\":6},{\"gridX\":1093,\"gridY\":128,\"type\":6},{\"gridX\":1114,\"gridY\":176,\"type\":6},{\"gridX\":1140,\"gridY\":176,\"type\":6}],\"collectibles\":[{\"gridX\":403,\"gridY\":108,\"type\":0},{\"gridX\":413,\"gridY\":114,\"type\":0},{\"gridX\":424,\"gridY\":119,\"type\":0},{\"gridX\":435,\"gridY\":121,\"type\":0},{\"gridX\":641,\"gridY\":93,\"type\":1},{\"gridX\":839,\"gridY\":167,\"type\":1},{\"gridX\":852,\"gridY\":196,\"type\":0},{\"gridX\":892,\"gridY\":226,\"type\":0},{\"gridX\":904,\"gridY\":222,\"type\":0},{\"gridX\":912,\"gridY\":215,\"type\":0},{\"gridX\":918,\"gridY\":207,\"type\":0},{\"gridX\":925,\"gridY\":198,\"type\":0},{\"gridX\":930,\"gridY\":188,\"type\":0},{\"gridX\":844,\"gridY\":198,\"type\":0},{\"gridX\":972,\"gridY\":155,\"type\":0},{\"gridX\":935,\"gridY\":177,\"type\":0},{\"gridX\":940,\"gridY\":162,\"type\":0},{\"gridX\":943,\"gridY\":146,\"type\":0},{\"gridX\":834,\"gridY\":197,\"type\":0},{\"gridX\":963,\"gridY\":143,\"type\":0},{\"gridX\":983,\"gridY\":163,\"type\":0},{\"gridX\":995,\"gridY\":169,\"type\":0},{\"gridX\":1007,\"gridY\":171,\"type\":0},{\"gridX\":1018,\"gridY\":168,\"type\":0},{\"gridX\":1066,\"gridY\":214,\"type\":0},{\"gridX\":1052,\"gridY\":207,\"type\":0},{\"gridX\":99,\"gridY\":136,\"type\":0},{\"gridX\":112,\"gridY\":144,\"type\":0},{\"gridX\":125,\"gridY\":146,\"type\":0},{\"gridX\":89,\"gridY\":127,\"type\":0},{\"gridX\":155,\"gridY\":113,\"type\":0},{\"gridX\":151,\"gridY\":123,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":137,\"gridY\":141,\"type\":0},{\"gridX\":236,\"gridY\":157,\"type\":0},{\"gridX\":169,\"gridY\":134,\"type\":0},{\"gridX\":164,\"gridY\":124,\"type\":0},{\"gridX\":242,\"gridY\":148,\"type\":0},{\"gridX\":332,\"gridY\":169,\"type\":0},{\"gridX\":332,\"gridY\":162,\"type\":0},{\"gridX\":455,\"gridY\":113,\"type\":0},{\"gridX\":449,\"gridY\":121,\"type\":0},{\"gridX\":550,\"gridY\":101,\"type\":0},{\"gridX\":541,\"gridY\":122,\"type\":0},{\"gridX\":547,\"gridY\":108,\"type\":0},{\"gridX\":544,\"gridY\":115,\"type\"" << ":0},{\"gridX\":685,\"gridY\":114,\"type\":0},{\"gridX\":690,\"gridY\":129,\"type\":0},{\"gridX\":695,\"gridY\":141,\"type\":0},{\"gridX\":701,\"gridY\":153,\"type\":0},{\"gridX\":710,\"gridY\":164,\"type\":0},{\"gridX\":1036,\"gridY\":163,\"type\":0},{\"gridX\":1039,\"gridY\":179,\"type\":0},{\"gridX\":1042,\"gridY\":193,\"type\":0},{\"gridX\":1064,\"gridY\":198,\"type\":0},{\"gridX\":1075,\"gridY\":190,\"type\":0},{\"gridX\":1083,\"gridY\":181,\"type\":0},{\"gridX\":1090,\"gridY\":169,\"type\":0},{\"gridX\":1095,\"gridY\":156,\"type\":0},{\"gridX\":1002,\"gridY\":148,\"type\":0},{\"gridX\":1010,\"gridY\":159,\"type\":0},{\"gridX\":1227,\"gridY\":247,\"type\":1},{\"gridX\":1290,\"gridY\":215,\"type\":0},{\"gridX\":1289,\"gridY\":206,\"type\":0},{\"gridX\":1322,\"gridY\":204,\"type\":0},{\"gridX\":1322,\"gridY\":196,\"type\":0},{\"gridX\":1374,\"gridY\":235,\"type\":0},{\"gridX\":1363,\"gridY\":233,\"type\":0},{\"gridX\":1422,\"gridY\":211,\"type\":0},{\"gridX\":1415,\"gridY\":204,\"type\":0},{\"gridX\":1426,\"gridY\":204,\"type\":0},{\"gridX\":1438,\"gridY\":205,\"type\":0},{\"gridX\":1494,\"gridY\":244,\"type\":0},{\"gridX\":1503,\"gridY\":240,\"type\":0},{\"gridX\":1511,\"gridY\":235,\"type\":0},{\"gridX\":1518,\"gridY\":229,\"type\":0},{\"gridX\":1133,\"gridY\":196,\"type\":0},{\"gridX\":1280,\"gridY\":206,\"type\":0},{\"gridX\":1282,\"gridY\":215,\"type\":0},{\"gridX\":747,\"gridY\":166,\"type\":0},{\"gridX\":737,\"gridY\":172,\"type\":0},{\"gridX\":724,\"gridY\":170,\"type\":0},{\"gridX\":459,\"gridY\":128,\"type\":0},{\"gridX\":764,\"gridY\":160,\"type\":0},{\"gridX\":768,\"gridY\":171,\"type\":0},{\"gridX\":772,\"gridY\":182,\"type\":0},{\"gridX\":778,\"gridY\":193,\"type\":0},{\"gridX\":860,\"gridY\":207,\"type\":0},{\"gridX\":869,\"gridY\":216,\"type\":0},{\"gridX\":879,\"gridY\":223,\"type\":0},{\"gridX\":846,\"gridY\":186,\"type\":0},{\"gridX\":841,\"gridY\":176,\"type\":0},{\"gridX\":1432,\"gridY\":211,\"type\":0},{\"gridX\":1602,\"gridY\":101,\"type\":0},{\"gridX\":1613,\"gridY\":100,\"type\":0},{\"gridX\":1624,\"gridY\":100,\"type\":0},{\"gridX\":1609,\"gridY\":109,\"type\":0},{\"gridX\":1620,\"gridY\":108,\"type\":0},{\"gridX\":536,\"gridY\":129,\"type\":0},{\"gridX\":462,\"gridY\":139,\"type\":0},{\"gridX\":468,\"gridY\":150,\"type\":0},{\"gridX\":477,\"gridY\":160,\"type\":0},{\"gridX\":488,\"gridY\":167,\"type\":0}],\"jons\":[{\"gridX\":18,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level8 *instance = new Chapter1Level8(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 9 ///

Chapter1Level9 * Chapter1Level9::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":9,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1664,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1632,\"gridY\":96}],\"enemies\":[],\"collectibles\":[],\"jons\":[{\"gridX\":24,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level9 *instance = new Chapter1Level9(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 11 ///

Chapter1Level11 * Chapter1Level11::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":11,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level11 *instance = new Chapter1Level11(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 12 ///

Chapter1Level12 * Chapter1Level12::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":12,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":284,\"gridY\":90,\"type\":20},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":292,\"gridY\":90,\"type\":23},{\"gridX\":420,\"gridY\":90,\"type\":23},{\"gridX\":548,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":24},{\"gridX\":676,\"gridY\":90,\"type\":24}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":116,\"gridY\":96,\"type\":41},{\"gridX\":440,\"gridY\":96,\"type\":12},{\"gridX\":232,\"gridY\":180,\"type\":0},{\"gridX\":236,\"gridY\":180,\"type\":1},{\"gridX\":250,\"gridY\":180,\"type\":1},{\"gridX\":264,\"gridY\":180,\"type\":2}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":408,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":164,\"gridY\":104,\"type\":3}],\"collectibles\":[{\"gridX\":252,\"gridY\":192,\"type\":1}],\"jons\":[{\"gridX\":16,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level12 *instance = new Chapter1Level12(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 13 ///

Chapter1Level13 * Chapter1Level13::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":13,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":88,\"type\":20},{\"gridX\":280,\"gridY\":88,\"type\":20},{\"gridX\":8,\"gridY\":88,\"type\":23},{\"gridX\":288,\"gridY\":88,\"type\":23},{\"gridX\":416,\"gridY\":88,\"type\":23},{\"gridX\":544,\"gridY\":88,\"type\":23},{\"gridX\":136,\"gridY\":88,\"type\":24},{\"gridX\":672,\"gridY\":88,\"type\":24}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":416,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":384,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[],\"collectibles\":[],\"jons\":[{\"gridX\":24,\"gridY\":104}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level13 *instance = new Chapter1Level13(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 14 ///

Chapter1Level14 * Chapter1Level14::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":14,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":164,\"gridY\":90,\"type\":20},{\"gridX\":92,\"gridY\":88,\"type\":20},{\"gridX\":100,\"gridY\":90,\"type\":21},{\"gridX\":8,\"gridY\":90,\"type\":22},{\"gridX\":172,\"gridY\":90,\"type\":23},{\"gridX\":132,\"gridY\":90,\"type\":24},{\"gridX\":300,\"gridY\":90,\"type\":24},{\"gridX\":72,\"gridY\":90,\"type\":24}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":227,\"gridY\":96,\"type\":8},{\"gridX\":300,\"gridY\":96,\"type\":12},{\"gridX\":52,\"gridY\":96,\"type\":8},{\"gridX\":200,\"gridY\":96,\"type\":8}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":268,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[{\"gridX\":168,\"gridY\":96,\"type\":5},{\"gridX\":64,\"gridY\":96,\"type\":5}],\"collectibles\":[],\"jons\":[{\"gridX\":0,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level14 *instance = new Chapter1Level14(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 15 ///

Chapter1Level15 * Chapter1Level15::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":15,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level15 *instance = new Chapter1Level15(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 16 ///

Chapter1Level16 * Chapter1Level16::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":16,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level16 *instance = new Chapter1Level16(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 17 ///

Chapter1Level17 * Chapter1Level17::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":17,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level17 *instance = new Chapter1Level17(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 18 ///

Chapter1Level18 * Chapter1Level18::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":18,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level18 *instance = new Chapter1Level18(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 19 ///

Chapter1Level19 * Chapter1Level19::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":19,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level19 *instance = new Chapter1Level19(text.c_str());
    
    return instance;
}

/// Chapter 1 Level 20 ///

Chapter1Level20 * Chapter1Level20::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":20,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level20 *instance = new Chapter1Level20(text.c_str());
    
    return instance;
}
