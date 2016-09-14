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
    m_isReleasingShockwave = false;
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

	gs->m_renderer->init(calcRendererTypeFromLevel(m_game->getWorld(), m_game->getLevel()));
    
    gs->m_renderer->beginOpeningPanningSequence(*m_game);
    
    EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), 0);
    EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), 0);
    EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), 0);
    EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), 0);
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
    
    stopLoopingSounds();
    
    Assets::getInstance()->setMusicId(MUSIC_PLAY_LEVEL_SELECT_LOOP);
    
    m_fStateTime = 0;
    m_isReleasingShockwave = false;
    m_fShockwaveElapsedTime = 0;
    m_fShockwaveCenterX = 0;
    m_fShockwaveCenterY = 0;
    m_hasShownOpeningSequence = false;
    m_hasOpeningSequenceCompleted = false;
    m_hasSwiped = false;
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

int Level::getScore()
{
    return m_iScore;
}

int Level::getOnlineScore()
{
    return m_iOnlineScore;
}

int Level::getLevelStatsFlag()
{
    return m_iLevelStatsFlag;
}

int Level::getNumGoldenCarrots()
{
    return m_iNumGoldenCarrots;
}

int Level::getJonAbilityFlag()
{
    return m_game->getJon().getAbilityFlag();
}

bool Level::hasCompletedLevel()
{
    return m_hasCompletedLevel;
}

Game& Level::getGame()
{
    return *m_game;
}

void Level::update(GameScreen* gs)
{
    Jon& jon = m_game->getJon();
    jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
    
    if (m_game->getCountHissWithMinas().size() > 0)
    {
        CountHissWithMina& countHissWithMina = m_game->getCountHissWithMina();
        countHissWithMina.setMoving();
        countHissWithMina.update(gs->m_fDeltaTime);
    }
    
    if (!m_hasShownOpeningSequence)
    {
		if (gs->m_stateMachine->getPreviousState() == GameScreenLevelEditor::getInstance())
		{
			m_hasShownOpeningSequence = true;
			m_hasOpeningSequenceCompleted = true;

			updateCamera(gs, 0, false, true);

			Assets::getInstance()->setMusicId(MUSIC_PLAY_WORLD_1_LOOP);

			return;
		}

        gs->m_renderer->beginOpeningPanningSequence(*m_game);
        
        m_hasShownOpeningSequence = true;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_WORLD_1_LOOP_INTRO);
    }
    else if (!m_hasOpeningSequenceCompleted)
    {
        jon.update(gs->m_fDeltaTime);
        
        int result = gs->m_renderer->updateCameraToFollowPathToJon(*m_game);
        m_hasOpeningSequenceCompleted = result == 3;
        m_activateRadialBlur = result == 1;
        jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
        
        if (m_hasOpeningSequenceCompleted)
        {
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
    else
    {
        if (m_isDisplayingResults)
        {
            for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
            {
                switch ((*i)->getTouchType())
                {
                    case DOWN:
                    case DRAGGED:
                        continue;
                    case UP:
                        gs->m_stateMachine->revertToPreviousState();
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
            
            m_game->update(gs->m_fDeltaTime);
            
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
                    if (!m_isReleasingShockwave)
                    {
                        m_fShockwaveCenterX = jon.getPosition().getX();
                        m_fShockwaveCenterY = jon.getPosition().getY();
                        m_fShockwaveElapsedTime = 0.0f;
                        m_isReleasingShockwave = true;
                    }
                }
            }
            
            if (m_isReleasingShockwave)
            {
                m_fShockwaveElapsedTime += gs->m_fDeltaTime * 1.2f;
                
                if (m_fShockwaveElapsedTime > 2)
                {
                    m_fShockwaveElapsedTime = 0;
                    m_isReleasingShockwave = false;
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
                    m_fShockwaveElapsedTime = 0;
                    m_isReleasingShockwave = false;
                }
            }
        }
        
        updateCamera(gs);
        
        if (m_game->getMarkers().size() > 1)
        {
            Marker* nextEndLoopMarker = m_game->getMarkers().at(1);
            if (gs->m_renderer->getCameraBounds().getRight() > nextEndLoopMarker->getGridX() * GRID_CELL_SIZE)
            {
                if (m_exitLoop || gs->m_stateMachine->getPreviousState() == GameScreenLevelEditor::getInstance())
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
                    EntityUtils::copyAndOffset(m_game->getCountHissWithMinas(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getEnemies(), beginGridX, endGridX);
                    EntityUtils::offsetOnly(m_game->getCollectibleItems(), beginGridX, endGridX);
                    EntityUtils::copyAndOffset(m_game->getExtraForegroundObjects(), beginGridX, endGridX);
                    
                    EntityUtils::offsetAll(m_game->getMarkers(), beginGridX, endGridX);
                    
                    EntityUtils::setGameToEntities(m_game->getForegroundObjects(), m_game.get());
                    EntityUtils::setGameToEntities(m_game->getMidBossForegroundObjects(), m_game.get());
                    EntityUtils::setGameToEntities(m_game->getEnemies(), m_game.get());
                    EntityUtils::setGameToEntities(m_game->getExtraForegroundObjects(), m_game.get());
                    
                    m_game->calcFarRight();
                }
            }
        }
        
        if (m_hasCompletedLevel)
        {
            m_fStateTime += gs->m_fDeltaTime / 2;
            if (m_fStateTime > 1)
            {
                m_fStateTime = 1;
                m_isDisplayingResults = true;
            }
            
            short musicId = MUSIC_SET_VOLUME * 1000 + (short) ((0.5f - m_fStateTime / 2) * 100);
            Assets::getInstance()->setMusicId(musicId);
        }
        else if (jon.getMainBounds().getLeft() > m_game->getFarRight())
        {
            // Has Cleared the Level
            
            static float startingTime = 300.0f;
            
            float secondsLeft = clamp(startingTime - m_game->getStateTime(), startingTime, 0);
            
            m_iScoreFromTime = secondsLeft * 100;
            
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
            
            m_iLevelStatsFlag = FlagUtil::setFlag(m_iLevelStatsFlag, FLAG_LEVEL_COMPLETE);
            
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
    
    if (m_isReleasingShockwave)
    {
        gs->m_renderer->renderToSecondFramebufferWithShockwave(m_fShockwaveCenterX, m_fShockwaveCenterY, m_fShockwaveElapsedTime, jon.isTransformingIntoVampire() || jon.isVampire());
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
    
    if (m_hasOpeningSequenceCompleted)
    {
        gs->m_renderer->renderHud(*m_game, m_hasCompletedLevel ? nullptr : m_backButton.get(), m_iScore);
    }

	if (m_isDebugMode)
	{
		gs->m_renderer->renderDebugInfo(*m_game, gs->m_iFPS);
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
    gs->m_renderer->updateCameraToFollowJon(*m_game, gs->m_fDeltaTime, paddingX, false, ignoreY, instant);
}

void Level::additionalRenderingBeforeHud(GameScreen* gs)
{
    // Empty, override in subclass
}

bool Level::isInSlowMotionMode()
{
    return false;
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
                if (isJonAlive && !m_hasSwiped)
                {
                    if (gs->m_touchPoint->getX() >= (gs->m_touchPointDown->getX() + SWIPE_WIDTH))
                    {
                        // Swipe Right
                        jon.triggerRightAction();
                        m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getX() <= (gs->m_touchPointDown->getX() - SWIPE_WIDTH))
                    {
                        // Swipe Left
                        jon.triggerLeftAction();
                        m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() >= (gs->m_touchPointDown->getY() + SWIPE_HEIGHT))
                    {
                        // Swipe Up
                        jon.triggerUpAction();
                        m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() <= (gs->m_touchPointDown->getY() - SWIPE_HEIGHT))
                    {
                        // Swipe Down
                        jon.triggerDownAction();
                        m_hasSwiped = true;
                    }
                    
                    if (m_hasSwiped)
                    {
                        gs->m_isScreenHeldDown = false;
                        gs->m_fScreenHeldTime = 0.0f;
                    }
                }
                continue;
            case UP:
                if (!m_hasCompletedLevel
                    && OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getMainBounds()))
                {
                    m_exitLoop = true;
                    
                    gs->m_renderer->stopCamera();
                    
                    gs->m_stateMachine->revertToPreviousState();
                    
                    return true;
                }
                
                if (isJonAlive)
                {
                    if (!m_hasSwiped && gs->m_fScreenHeldTime < 0.4f)
                    {
                        jon.triggerJump();
                    }
                    
                    if (gs->m_fScreenHeldTime > 0.4f)
                    {
                        jon.triggerCancelTransform();
                    }
                    
                    gs->m_isScreenHeldDown = false;
                    gs->m_fScreenHeldTime = 0;
                    
                    m_hasSwiped = false;
                    
                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                }
                
                break;
        }
    }
    
    return false;
}

void Level::updateScore()
{
    m_iScoreFromObjects = m_game->getNumCarrotsCollected() * 200;
    m_iScoreFromObjects += m_game->getNumGoldenCarrotsCollected() * 5000;
    
    if (m_game->getJons().size() > 0)
    {
        m_iScoreFromObjects += m_game->getJon().getNumEnemiesDestroyed() * 2000;
    }
    
    m_iScore = m_iScoreFromTime + m_iScoreFromObjects;
}

void Level::stopLoopingSounds()
{
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPARROW_FLY);
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SAW_GRIND);
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
            }
        }
    }
    
    if (!FlagUtil::isFlagSet(m_iLevelStatsFlag, FLAG_BONUS_GOLDEN_CARROT_COLLECTED))
    {
        if (m_game->getNumCarrotsCollected() >= 100)
        {
            m_game->setNumGoldenCarrotsCollected(m_game->getNumGoldenCarrotsCollected() + 1);
            
            m_iLevelStatsFlag = FlagUtil::setFlag(m_iLevelStatsFlag, FLAG_BONUS_GOLDEN_CARROT_COLLECTED);
        }
    }
}

Level::Level(const char* json) :
m_sourceGame(nullptr),
m_fStateTime(0.0f),
m_isReleasingShockwave(false),
m_fShockwaveElapsedTime(0.0f),
m_fShockwaveCenterX(0.0f),
m_fShockwaveCenterY(0.0f),
m_iScoreFromTime(0),
m_iScoreFromObjects(0),
m_iScore(0),
m_iOnlineScore(0),
m_iLevelStatsFlag(0),
m_iNumGoldenCarrots(0),
m_hasShownOpeningSequence(false),
m_hasOpeningSequenceCompleted(false),
m_activateRadialBlur(false),
m_hasSwiped(false),
m_showDeathTransOut(false),
m_exitLoop(false),
m_hasCompletedLevel(false),
m_isDisplayingResults(false),
m_iBestScore(0),
m_iBestOnlineScore(0),
m_iBestLevelStatsFlag(0),
m_iLastKnownNumGoldenCarrots(0),
m_iLastKnownJonAbilityFlag(0),
m_isDebugMode(false)
{
    m_json = json;
    m_game = std::unique_ptr<Game>(new Game());
    m_backButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_BackToLevelSelect));
}

/// Chapter 1 Level 1 ///

Chapter1Level1 * Chapter1Level1::getInstance()
{
    static Chapter1Level1 *instance = new Chapter1Level1("{\"world\":1,\"level\":1,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":60,\"gridY\":96,\"type\":1},{\"gridX\":102,\"gridY\":96,\"type\":0},{\"gridX\":136,\"gridY\":96,\"type\":2},{\"gridX\":272,\"gridY\":96,\"type\":2},{\"gridX\":200,\"gridY\":96,\"type\":0},{\"gridX\":362,\"gridY\":96,\"type\":2},{\"gridX\":418,\"gridY\":96,\"type\":1},{\"gridX\":508,\"gridY\":96,\"type\":0},{\"gridX\":540,\"gridY\":96,\"type\":1},{\"gridX\":648,\"gridY\":96,\"type\":0},{\"gridX\":716,\"gridY\":96,\"type\":2},{\"gridX\":810,\"gridY\":96,\"type\":1},{\"gridX\":896,\"gridY\":96,\"type\":0},{\"gridX\":1004,\"gridY\":96,\"type\":0},{\"gridX\":1076,\"gridY\":96,\"type\":2},{\"gridX\":1282,\"gridY\":96,\"type\":0},{\"gridX\":1338,\"gridY\":96,\"type\":2},{\"gridX\":1636,\"gridY\":96,\"type\":1},{\"gridX\":1668,\"gridY\":96,\"type\":0},{\"gridX\":1750,\"gridY\":96,\"type\":2},{\"gridX\":1688,\"gridY\":96,\"type\":2},{\"gridX\":1730,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":28},{\"gridX\":1800,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1928,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":476,\"gridY\":0,\"type\":30},{\"gridX\":934,\"gridY\":0,\"type\":31},{\"gridX\":1382,\"gridY\":0,\"type\":33},{\"gridX\":1142,\"gridY\":0,\"type\":32}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1688,\"gridY\":96,\"type\":12},{\"gridX\":620,\"gridY\":96,\"type\":21},{\"gridX\":740,\"gridY\":160,\"type\":1},{\"gridX\":658,\"gridY\":140,\"type\":0},{\"gridX\":676,\"gridY\":140,\"type\":2},{\"gridX\":712,\"gridY\":160,\"type\":1},{\"gridX\":708,\"gridY\":160,\"type\":0},{\"gridX\":754,\"gridY\":160,\"type\":2},{\"gridX\":726,\"gridY\":160,\"type\":1},{\"gridX\":1664,\"gridY\":144,\"type\":34},{\"gridX\":1664,\"gridY\":160,\"type\":34},{\"gridX\":1664,\"gridY\":176,\"type\":34},{\"gridX\":1664,\"gridY\":192,\"type\":34},{\"gridX\":1664,\"gridY\":208,\"type\":34},{\"gridX\":1664,\"gridY\":224,\"type\":34},{\"gridX\":662,\"gridY\":140,\"type\":1}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1656,\"gridY\":96}],\"enemies\":[{\"gridX\":296,\"gridY\":96,\"type\":8},{\"gridX\":392,\"gridY\":96,\"type\":9},{\"gridX\":559,\"gridY\":96,\"type\":10},{\"gridX\":717,\"gridY\":96,\"type\":11},{\"gridX\":915,\"gridY\":96,\"type\":12},{\"gridX\":1316,\"gridY\":96,\"type\":6},{\"gridX\":1024,\"gridY\":96,\"type\":4}],\"collectibles\":[{\"gridX\":136,\"gridY\":104,\"type\":0},{\"gridX\":144,\"gridY\":104,\"type\":0},{\"gridX\":152,\"gridY\":104,\"type\":0},{\"gridX\":160,\"gridY\":104,\"type\":0},{\"gridX\":168,\"gridY\":104,\"type\":0},{\"gridX\":224,\"gridY\":112,\"type\":0},{\"gridX\":232,\"gridY\":120,\"type\":0},{\"gridX\":240,\"gridY\":120,\"type\":0},{\"gridX\":216,\"gridY\":104,\"type\":0},{\"gridX\":280,\"gridY\":104,\"type\":0},{\"gridX\":456,\"gridY\":104,\"type\":0},{\"gridX\":336,\"gridY\":120,\"type\":0},{\"gridX\":312,\"gridY\":104,\"type\":0},{\"gridX\":448,\"gridY\":104,\"type\":0},{\"gridX\":440,\"gridY\":104,\"type\":0},{\"gridX\":432,\"gridY\":104,\"type\":0},{\"gridX\":424,\"gridY\":104,\"type\":0},{\"gridX\":416,\"gridY\":104,\"type\":0},{\"gridX\":248,\"gridY\":120,\"type\":0},{\"gridX\":352,\"gridY\":112,\"type\":0},{\"gridX\":272,\"gridY\":112,\"type\":0},{\"gridX\":240,\"gridY\":128,\"type\":0},{\"gridX\":256,\"gridY\":120,\"type\":0},{\"gridX\":264,\"gridY\":120,\"type\":0},{\"gridX\":248,\"gridY\":128,\"type\":0},{\"gridX\":328,\"gridY\":120,\"type\":0},{\"gridX\":344,\"gridY\":120,\"type\":0},{\"gridX\":408,\"gridY\":104,\"type\":0},{\"gridX\":256,\"gridY\":128,\"type\":0},{\"gridX\":488,\"gridY\":88,\"type\":1},{\"gridX\":320,\"gridY\":112,\"type\":0},{\"gridX\":788,\"gridY\":128,\"type\":1},{\"gridX\":512,\"gridY\":104,\"type\":0},{\"gridX\":688,\"gridY\":120,\"type\":0},{\"gridX\":520,\"gridY\":112,\"type\":0},{\"gridX\":528,\"gridY\":112,\"type\":0},{\"gridX\":336,\"gridY\":128,\"type\":0},{\"gridX\":360,\"gridY\":104,\"type\":0},{\"gridX\":576,\"gridY\":104,\"type\":0},{\"gridX\":584,\"gridY\":104,\"type\":0},{\"gridX\":592,\"gridY\":104,\"type\":0},{\"gridX\":696,\"gridY\":120,\"type\":0},{\"gridX\":704,\"gridY\":120,\"type\":0},{\"gridX\":696,\"gridY\":128,\"type\":0},{\"gridX\":876,\"gridY\":112,\"type\":0},{\"gridX\":884,\"gridY\":108,\"type\":0},{\"gridX\":860,\"gridY\":120,\"type\":0},{\"gridX\":868,\"gridY\":116,\"type\":0},{\"gridX\":1124,\"gridY\":104,\"type\":0},{\"gridX\":1132,\"gridY\":104,\"type\":0},{\"gridX\":1140,\"gridY\":104,\"type\":0},{\"gridX\":1148,\"gridY\":112,\"type\":0},{\"gridX\":1140,\"gridY\":112,\"type\":0},{\"gridX\":1132,\"gridY\":112,\"type\":0},{\"gridX\":1156,\"gridY\":120,\"type\":0},{\"gridX\":1140,\"gridY\":120,\"type\":0},{\"gridX\":1148,\"gridY\":120,\"type\":0},{\"gridX\":1156,\"gridY\":128,\"type\":0},{\"gridX\":1164,\"gridY\":128,\"type\":0},{\"gridX\":1164,\"gridY\":120,\"type\":0},{\"gridX\":600,\"gridY\":104,\"type\":0},{\"gridX\":536,\"gridY\":104,\"type\":0},{\"gridX\":936,\"gridY\":104,\"type\":0},{\"gridX\":944,\"gridY\":108,\"type\":0},{\"gridX\":952,\"gridY\":108,\"type\":0},{\"gridX\":960,\"gridY\":108,\"type\":0},{\"gridX\":968,\"gridY\":108,\"type\":0},{\"gridX\":976,\"gridY\":108,\"type\":0},{\"gridX\":984,\"gridY\":108,\"type\":0},{\"gridX\":992,\"gridY\":104,\"type\":0},{\"gridX\":1356,\"gridY\":180,\"type\":0},{\"gridX\":1332,\"gridY\":132,\"type\":0},{\"gridX\":1336,\"gridY\":140,\"type\":0},{\"gridX\":1252,\"gridY\":112,\"type\":0},{\"gridX\":1268,\"gridY\":104,\"type\":0},{\"gridX\":1260,\"gridY\":104,\"type\":0},{\"gridX\":1276,\"gridY\":104,\"type\":0},{\"gridX\":1268,\"gridY\":112,\"type\":0},{\"gridX\":1260,\"gridY\":112,\"type\":0},{\"gridX\":1260,\"gridY\":120,\"type\":0},{\"gridX\":1252,\"gridY\":120,\"type\":0},{\"gridX\":1244,\"gridY\":120,\"type\":0},{\"gridX\":1220,\"gridY\":120,\"type\":0},{\"gridX\":1244,\"gridY\":128,\"type\":0},{\"gridX\":1236,\"gridY\":128,\"type\":0},{\"gridX\":1196,\"gridY\":112,\"type\":0},{\"gridX\":1040,\"gridY\":104,\"type\":0},{\"gridX\":1056,\"gridY\":104,\"type\":0},{\"gridX\":1228,\"gridY\":124,\"type\":0},{\"gridX\":1048,\"gridY\":104,\"type\":0},{\"gridX\":1064,\"gridY\":104,\"type\":0},{\"gridX\":1172,\"gridY\":124,\"type\":0},{\"gridX\":1236,\"gridY\":120,\"type\":0},{\"gridX\":1348,\"gridY\":164,\"type\":0},{\"gridX\":1352,\"gridY\":172,\"type\":0},{\"gridX\":1072,\"gridY\":104,\"type\":0},{\"gridX\":1212,\"gridY\":116,\"type\":0},{\"gridX\":1180,\"gridY\":120,\"type\":0},{\"gridX\":1204,\"gridY\":112,\"type\":0},{\"gridX\":1188,\"gridY\":116,\"type\":0},{\"gridX\":1340,\"gridY\":148,\"type\":0},{\"gridX\":1344,\"gridY\":156,\"type\":0},{\"gridX\":1580,\"gridY\":188,\"type\":1}],\"jons\":[{\"gridX\":28,\"gridY\":96}],\"extraForegroundObjects\":[{\"gridX\":776,\"gridY\":95,\"type\":39}],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 2 ///

Chapter1Level2 * Chapter1Level2::getInstance()
{
    static Chapter1Level2 *instance = new Chapter1Level2("{\"world\":1,\"level\":2,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 3 ///

Chapter1Level3 * Chapter1Level3::getInstance()
{
    static Chapter1Level3 *instance = new Chapter1Level3("{\"world\":1,\"level\":3,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 4 ///

Chapter1Level4 * Chapter1Level4::getInstance()
{
    static Chapter1Level4 *instance = new Chapter1Level4("{\"world\":1,\"level\":4,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 5 ///

Chapter1Level5 * Chapter1Level5::getInstance()
{
    static Chapter1Level5 *instance = new Chapter1Level5("{\"world\":1,\"level\":5,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 6 ///

Chapter1Level6 * Chapter1Level6::getInstance()
{
    static Chapter1Level6 *instance = new Chapter1Level6("{\"world\":1,\"level\":6,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 7 ///

Chapter1Level7 * Chapter1Level7::getInstance()
{
    static Chapter1Level7 *instance = new Chapter1Level7("{\"world\":1,\"level\":7,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 8 ///

Chapter1Level8 * Chapter1Level8::getInstance()
{
    static Chapter1Level8 *instance = new Chapter1Level8("{\"world\":1,\"level\":8,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 9 ///

Chapter1Level9 * Chapter1Level9::getInstance()
{
    static Chapter1Level9 *instance = new Chapter1Level9("{\"world\":1,\"level\":9,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 11 ///

Chapter1Level11 * Chapter1Level11::getInstance()
{
    static Chapter1Level11 *instance = new Chapter1Level11("{\"world\":1,\"level\":11,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 12 ///

Chapter1Level12 * Chapter1Level12::getInstance()
{
    static Chapter1Level12 *instance = new Chapter1Level12("{\"world\":1,\"level\":12,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 13 ///

Chapter1Level13 * Chapter1Level13::getInstance()
{
    static Chapter1Level13 *instance = new Chapter1Level13("{\"world\":1,\"level\":13,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 14 ///

Chapter1Level14 * Chapter1Level14::getInstance()
{
    static Chapter1Level14 *instance = new Chapter1Level14("{\"world\":1,\"level\":14,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 15 ///

Chapter1Level15 * Chapter1Level15::getInstance()
{
    static Chapter1Level15 *instance = new Chapter1Level15("{\"world\":1,\"level\":15,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 16 ///

Chapter1Level16 * Chapter1Level16::getInstance()
{
    static Chapter1Level16 *instance = new Chapter1Level16("{\"world\":1,\"level\":16,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 17 ///

Chapter1Level17 * Chapter1Level17::getInstance()
{
    static Chapter1Level17 *instance = new Chapter1Level17("{\"world\":1,\"level\":17,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 18 ///

Chapter1Level18 * Chapter1Level18::getInstance()
{
    static Chapter1Level18 *instance = new Chapter1Level18("{\"world\":1,\"level\":18,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 19 ///

Chapter1Level19 * Chapter1Level19::getInstance()
{
    static Chapter1Level19 *instance = new Chapter1Level19("{\"world\":1,\"level\":19,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 20 ///

Chapter1Level20 * Chapter1Level20::getInstance()
{
    static Chapter1Level20 *instance = new Chapter1Level20("{\"world\":1,\"level\":20,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":340,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":308,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":128,\"gridY\":100,\"type\":0},{\"gridX\":128,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":108,\"type\":0},{\"gridX\":136,\"gridY\":100,\"type\":0},{\"gridX\":136,\"gridY\":108,\"type\":0},{\"gridX\":144,\"gridY\":100,\"type\":0},{\"gridX\":152,\"gridY\":108,\"type\":0},{\"gridX\":152,\"gridY\":100,\"type\":0},{\"gridX\":160,\"gridY\":108,\"type\":0},{\"gridX\":160,\"gridY\":100,\"type\":0},{\"gridX\":168,\"gridY\":108,\"type\":0},{\"gridX\":168,\"gridY\":100,\"type\":0},{\"gridX\":176,\"gridY\":108,\"type\":0},{\"gridX\":176,\"gridY\":100,\"type\":0},{\"gridX\":184,\"gridY\":104,\"type\":1},{\"gridX\":192,\"gridY\":108,\"type\":0},{\"gridX\":192,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":108,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":208,\"gridY\":108,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":108,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":224,\"gridY\":108,\"type\":0},{\"gridX\":224,\"gridY\":100,\"type\":0},{\"gridX\":232,\"gridY\":108,\"type\":0},{\"gridX\":232,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":100,\"type\":0},{\"gridX\":240,\"gridY\":108,\"type\":0},{\"gridX\":248,\"gridY\":100,\"type\":0},{\"gridX\":248,\"gridY\":108,\"type\":0},{\"gridX\":256,\"gridY\":104,\"type\":1},{\"gridX\":264,\"gridY\":100,\"type\":0},{\"gridX\":264,\"gridY\":108,\"type\":0},{\"gridX\":272,\"gridY\":100,\"type\":0},{\"gridX\":272,\"gridY\":108,\"type\":0},{\"gridX\":280,\"gridY\":100,\"type\":0},{\"gridX\":280,\"gridY\":108,\"type\":0},{\"gridX\":288,\"gridY\":100,\"type\":0},{\"gridX\":288,\"gridY\":108,\"type\":0},{\"gridX\":296,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":108,\"type\":0},{\"gridX\":304,\"gridY\":104,\"type\":1},{\"gridX\":144,\"gridY\":116,\"type\":0},{\"gridX\":152,\"gridY\":116,\"type\":0},{\"gridX\":160,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":116,\"type\":0},{\"gridX\":216,\"gridY\":116,\"type\":0},{\"gridX\":224,\"gridY\":116,\"type\":0},{\"gridX\":232,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":124,\"type\":0},{\"gridX\":160,\"gridY\":124,\"type\":0},{\"gridX\":200,\"gridY\":116,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":240,\"gridY\":116,\"type\":0},{\"gridX\":208,\"gridY\":124,\"type\":0},{\"gridX\":216,\"gridY\":124,\"type\":0},{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":232,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":116,\"type\":0},{\"gridX\":280,\"gridY\":124,\"type\":0},{\"gridX\":152,\"gridY\":132,\"type\":0},{\"gridX\":216,\"gridY\":132,\"type\":0},{\"gridX\":224,\"gridY\":132,\"type\":0},{\"gridX\":160,\"gridY\":132,\"type\":0},{\"gridX\":144,\"gridY\":132,\"type\":0},{\"gridX\":176,\"gridY\":116,\"type\":0},{\"gridX\":168,\"gridY\":124,\"type\":0},{\"gridX\":168,\"gridY\":132,\"type\":0},{\"gridX\":136,\"gridY\":124,\"type\":0},{\"gridX\":136,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":124,\"type\":0},{\"gridX\":176,\"gridY\":132,\"type\":0},{\"gridX\":128,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":116,\"type\":0},{\"gridX\":248,\"gridY\":116,\"type\":0},{\"gridX\":200,\"gridY\":124,\"type\":0},{\"gridX\":240,\"gridY\":124,\"type\":0},{\"gridX\":208,\"gridY\":132,\"type\":0},{\"gridX\":232,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":116,\"type\":0},{\"gridX\":296,\"gridY\":116,\"type\":0},{\"gridX\":288,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":124,\"type\":0},{\"gridX\":264,\"gridY\":124,\"type\":0},{\"gridX\":296,\"gridY\":124,\"type\":0},{\"gridX\":272,\"gridY\":132,\"type\":0},{\"gridX\":280,\"gridY\":132,\"type\":0},{\"gridX\":288,\"gridY\":132,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":264,\"gridY\":132,\"type\":0},{\"gridX\":192,\"gridY\":124,\"type\":0},{\"gridX\":248,\"gridY\":124,\"type\":0},{\"gridX\":192,\"gridY\":132,\"type\":0},{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":240,\"gridY\":132,\"type\":0},{\"gridX\":248,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}
