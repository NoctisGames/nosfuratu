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
        
        m_batPanel = std::unique_ptr<BatPanel>(new BatPanel());
        
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
            
            BatPanelType bpt = getBatPanelType();
            if (bpt != BatPanelType_None)
            {
                m_batPanel->open(bpt);
            }
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
        
        m_batPanel->update(gs->m_fDeltaTime);
        
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
            
            if (!m_batPanel->isAcknowledged())
            {
                return;
            }
            
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
                    EntityUtils::copyAndOffset(m_game->getCollectibleItems(), beginGridX, endGridX);
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
            
            m_iOnlineScore = m_iScore;
            
            if (m_iScore < m_iBestScore)
            {
                m_iScore = m_iBestScore;
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
            
            static float startingTime = 120.0f;
            
            float secondsLeft = clamp(startingTime - m_game->getStateTime(), startingTime, 0);
            
            m_iScoreFromTime = secondsLeft * 1000;
  
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
        gs->m_renderer->renderHud(*m_game, m_hasCompletedLevel ? nullptr : m_backButton.get(), m_batPanel.get(), m_iScore, gs->m_iFPS);
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
    bool isInstructionAcknowledged = m_batPanel->isAcknowledged();
    
    for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld(*(*i));
        
        switch ((*i)->getTouchType())
        {
            case DOWN:
                if (isJonAlive && isInstructionAcknowledged)
                {
                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                    gs->m_isScreenHeldDown = true;
                    gs->m_fScreenHeldTime = 0.0f;
                }
                continue;
            case DRAGGED:
                if (isJonAlive && !m_hasSwiped && isInstructionAcknowledged)
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
                else if (!isInstructionAcknowledged)
                {
                    m_batPanel->handleTouch(*gs->m_touchPoint);
                    
                    return false;
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

BatPanelType Level::getBatPanelType()
{
    if (FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_LEVEL_COMPLETE))
    {
        return BatPanelType_None;
    }
    
    if (m_game->getWorld() == 1 && m_game->getLevel() == 1)
    {
        return BatPanelType_Jump;
    }
    else if (m_game->getWorld() == 1 && m_game->getLevel() == 2)
    {
        return BatPanelType_DoubleJump;
    }
    else if (m_game->getWorld() == 1 && m_game->getLevel() == 3)
    {
        return BatPanelType_Transform;
    }
    else if (m_game->getWorld() == 1 && m_game->getLevel() == 11)
    {
        return BatPanelType_Stomp;
    }
    
    return BatPanelType_None;
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
    int numCarrots = 0;
    for (std::vector<CollectibleItem *>::iterator i = items.begin(); i != items.end(); i++)
    {
        numCarrots++;
        
        if (OverlapTester::doRectanglesOverlap(entity.getMainBounds(), (*i)->getMainBounds()))
        {
            (*i)->collect();
            
            if (dynamic_cast<GoldenCarrot *>((*i)))
            {
                numCarrots--;
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
                numCarrots--;
                m_game->setNumCarrotsCollected(m_game->getNumCarrotsCollected() + 1);
            }
        }
    }
    
    if (!FlagUtil::isFlagSet(m_iLevelStatsFlag, FLAG_BONUS_GOLDEN_CARROT_COLLECTED))
    {
        if (m_game->getNumCarrotsCollected() >= 100
            || numCarrots <= 0)
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
m_iLastKnownJonAbilityFlag(0)
{
    m_json = json;
    m_game = std::unique_ptr<Game>(new Game());
    m_backButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_BackToLevelSelect));
    m_batPanel = std::unique_ptr<BatPanel>(new BatPanel());
}

/// Chapter 1 Level 1 ///

Chapter1Level1 * Chapter1Level1::getInstance()
{
    static Chapter1Level1 *instance = new Chapter1Level1("{\"world\":1,\"level\":1,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":0},{\"gridX\":48,\"gridY\":96,\"type\":1},{\"gridX\":96,\"gridY\":96,\"type\":2},{\"gridX\":160,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":0},{\"gridX\":276,\"gridY\":96,\"type\":2},{\"gridX\":340,\"gridY\":96,\"type\":1},{\"gridX\":388,\"gridY\":96,\"type\":2},{\"gridX\":460,\"gridY\":96,\"type\":0},{\"gridX\":504,\"gridY\":96,\"type\":0},{\"gridX\":612,\"gridY\":96,\"type\":2},{\"gridX\":680,\"gridY\":96,\"type\":2},{\"gridX\":758,\"gridY\":96,\"type\":0},{\"gridX\":799,\"gridY\":96,\"type\":1},{\"gridX\":764,\"gridY\":96,\"type\":2},{\"gridX\":729,\"gridY\":96,\"type\":1},{\"gridX\":822,\"gridY\":96,\"type\":1},{\"gridX\":818,\"gridY\":96,\"type\":0},{\"gridX\":843,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":200,\"gridY\":0,\"type\":26},{\"gridX\":872,\"gridY\":0,\"type\":26},{\"gridX\":136,\"gridY\":0,\"type\":27},{\"gridX\":232,\"gridY\":0,\"type\":27},{\"gridX\":552,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":296,\"gridY\":0,\"type\":28},{\"gridX\":424,\"gridY\":0,\"type\":28},{\"gridX\":616,\"gridY\":0,\"type\":28},{\"gridX\":744,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":240,\"gridY\":0,\"type\":30},{\"gridX\":576,\"gridY\":0,\"type\":30}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":739,\"gridY\":96,\"type\":12},{\"gridX\":409,\"gridY\":96,\"type\":11},{\"gridX\":116,\"gridY\":96,\"type\":18},{\"gridX\":698,\"gridY\":96,\"type\":17}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":706,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":52,\"gridY\":104,\"type\":0},{\"gridX\":60,\"gridY\":104,\"type\":0},{\"gridX\":68,\"gridY\":104,\"type\":0},{\"gridX\":76,\"gridY\":104,\"type\":0},{\"gridX\":84,\"gridY\":104,\"type\":0},{\"gridX\":92,\"gridY\":104,\"type\":0},{\"gridX\":100,\"gridY\":104,\"type\":0},{\"gridX\":108,\"gridY\":108,\"type\":0},{\"gridX\":116,\"gridY\":112,\"type\":0},{\"gridX\":124,\"gridY\":116,\"type\":0},{\"gridX\":132,\"gridY\":120,\"type\":0},{\"gridX\":148,\"gridY\":120,\"type\":0},{\"gridX\":156,\"gridY\":116,\"type\":0},{\"gridX\":164,\"gridY\":112,\"type\":0},{\"gridX\":172,\"gridY\":108,\"type\":0},{\"gridX\":196,\"gridY\":104,\"type\":0},{\"gridX\":180,\"gridY\":104,\"type\":0},{\"gridX\":188,\"gridY\":104,\"type\":0},{\"gridX\":140,\"gridY\":124,\"type\":0},{\"gridX\":256,\"gridY\":88,\"type\":1},{\"gridX\":304,\"gridY\":104,\"type\":0},{\"gridX\":316,\"gridY\":104,\"type\":0},{\"gridX\":328,\"gridY\":104,\"type\":0},{\"gridX\":396,\"gridY\":104,\"type\":0},{\"gridX\":448,\"gridY\":128,\"type\":1},{\"gridX\":340,\"gridY\":104,\"type\":0},{\"gridX\":592,\"gridY\":120,\"type\":1},{\"gridX\":512,\"gridY\":104,\"type\":0},{\"gridX\":520,\"gridY\":104,\"type\":0},{\"gridX\":528,\"gridY\":104,\"type\":0},{\"gridX\":620,\"gridY\":104,\"type\":0},{\"gridX\":628,\"gridY\":104,\"type\":0},{\"gridX\":636,\"gridY\":104,\"type\":0},{\"gridX\":620,\"gridY\":112,\"type\":0},{\"gridX\":668,\"gridY\":104,\"type\":0},{\"gridX\":676,\"gridY\":104,\"type\":0},{\"gridX\":684,\"gridY\":104,\"type\":0},{\"gridX\":692,\"gridY\":112,\"type\":0},{\"gridX\":612,\"gridY\":112,\"type\":0},{\"gridX\":684,\"gridY\":112,\"type\":0},{\"gridX\":652,\"gridY\":104,\"type\":0},{\"gridX\":660,\"gridY\":104,\"type\":0},{\"gridX\":644,\"gridY\":104,\"type\":0},{\"gridX\":408,\"gridY\":112,\"type\":0}],\"jons\":[{\"gridX\":16,\"gridY\":108}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":744,\"gridY\":0,\"type\":0},{\"gridX\":900,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

/// Chapter 1 Level 2 ///

Chapter1Level2 * Chapter1Level2::getInstance()
{
    static Chapter1Level2 *instance = new Chapter1Level2("{\"world\":1,\"level\":2,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":1},{\"gridX\":50,\"gridY\":96,\"type\":2},{\"gridX\":108,\"gridY\":96,\"type\":0},{\"gridX\":172,\"gridY\":96,\"type\":2},{\"gridX\":244,\"gridY\":96,\"type\":2},{\"gridX\":520,\"gridY\":96,\"type\":1},{\"gridX\":564,\"gridY\":96,\"type\":1},{\"gridX\":600,\"gridY\":96,\"type\":2},{\"gridX\":356,\"gridY\":96,\"type\":2},{\"gridX\":666,\"gridY\":96,\"type\":2},{\"gridX\":718,\"gridY\":96,\"type\":1},{\"gridX\":688,\"gridY\":96,\"type\":0},{\"gridX\":639,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":328,\"gridY\":0,\"type\":26},{\"gridX\":488,\"gridY\":0,\"type\":26},{\"gridX\":360,\"gridY\":0,\"type\":27},{\"gridX\":136,\"gridY\":0,\"type\":27},{\"gridX\":200,\"gridY\":0,\"type\":27},{\"gridX\":264,\"gridY\":0,\"type\":27},{\"gridX\":424,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28}],\"pits\":[{\"gridX\":308,\"gridY\":0,\"type\":30},{\"gridX\":124,\"gridY\":0,\"type\":32},{\"gridX\":440,\"gridY\":0,\"type\":31}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":92,\"gridY\":112,\"type\":0},{\"gridX\":96,\"gridY\":112,\"type\":1},{\"gridX\":124,\"gridY\":112,\"type\":2},{\"gridX\":643,\"gridY\":96,\"type\":12},{\"gridX\":404,\"gridY\":96,\"type\":19},{\"gridX\":110,\"gridY\":112,\"type\":1}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":610,\"gridY\":96}],\"enemies\":[{\"gridX\":372,\"gridY\":96,\"type\":2}],\"collectibles\":[{\"gridX\":68,\"gridY\":104,\"type\":0},{\"gridX\":76,\"gridY\":104,\"type\":0},{\"gridX\":72,\"gridY\":112,\"type\":0},{\"gridX\":80,\"gridY\":112,\"type\":0},{\"gridX\":96,\"gridY\":120,\"type\":0},{\"gridX\":108,\"gridY\":120,\"type\":0},{\"gridX\":120,\"gridY\":120,\"type\":0},{\"gridX\":192,\"gridY\":160,\"type\":1},{\"gridX\":324,\"gridY\":88,\"type\":1},{\"gridX\":440,\"gridY\":152,\"type\":1},{\"gridX\":416,\"gridY\":132,\"type\":0},{\"gridX\":412,\"gridY\":120,\"type\":0},{\"gridX\":408,\"gridY\":108,\"type\":0},{\"gridX\":500,\"gridY\":120,\"type\":0},{\"gridX\":508,\"gridY\":112,\"type\":0},{\"gridX\":516,\"gridY\":104,\"type\":0},{\"gridX\":524,\"gridY\":104,\"type\":0},{\"gridX\":532,\"gridY\":104,\"type\":0},{\"gridX\":540,\"gridY\":104,\"type\":0},{\"gridX\":568,\"gridY\":104,\"type\":0},{\"gridX\":560,\"gridY\":104,\"type\":0},{\"gridX\":576,\"gridY\":120,\"type\":0},{\"gridX\":568,\"gridY\":112,\"type\":0},{\"gridX\":576,\"gridY\":112,\"type\":0},{\"gridX\":584,\"gridY\":120,\"type\":0},{\"gridX\":616,\"gridY\":112,\"type\":0},{\"gridX\":624,\"gridY\":104,\"type\":0},{\"gridX\":264,\"gridY\":104,\"type\":0},{\"gridX\":252,\"gridY\":104,\"type\":0},{\"gridX\":276,\"gridY\":104,\"type\":0},{\"gridX\":288,\"gridY\":104,\"type\":0},{\"gridX\":300,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":16,\"gridY\":104}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":648,\"gridY\":0,\"type\":0},{\"gridX\":775,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

/// Chapter 1 Level 3 ///

Chapter1Level3 * Chapter1Level3::getInstance()
{
    static Chapter1Level3 *instance = new Chapter1Level3("{\"world\":1,\"level\":3,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":68,\"gridY\":96,\"type\":2},{\"gridX\":132,\"gridY\":96,\"type\":0},{\"gridX\":168,\"gridY\":96,\"type\":0},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":488,\"gridY\":96,\"type\":2},{\"gridX\":544,\"gridY\":96,\"type\":0},{\"gridX\":724,\"gridY\":96,\"type\":0},{\"gridX\":892,\"gridY\":96,\"type\":0},{\"gridX\":816,\"gridY\":96,\"type\":1},{\"gridX\":960,\"gridY\":96,\"type\":1},{\"gridX\":1020,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":744,\"gridY\":0,\"type\":26},{\"gridX\":584,\"gridY\":0,\"type\":26},{\"gridX\":776,\"gridY\":0,\"type\":26},{\"gridX\":520,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":808,\"gridY\":0,\"type\":28},{\"gridX\":936,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":616,\"gridY\":0,\"type\":28},{\"gridX\":1064,\"gridY\":0,\"type\":28}],\"pits\":[{\"gridX\":636,\"gridY\":0,\"type\":31},{\"gridX\":776,\"gridY\":0,\"type\":30},{\"gridX\":280,\"gridY\":0,\"type\":32},{\"gridX\":464,\"gridY\":0,\"type\":30}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":292,\"gridY\":122,\"type\":0},{\"gridX\":296,\"gridY\":122,\"type\":1},{\"gridX\":310,\"gridY\":122,\"type\":1},{\"gridX\":324,\"gridY\":122,\"type\":1},{\"gridX\":338,\"gridY\":122,\"type\":2},{\"gridX\":188,\"gridY\":96,\"type\":8},{\"gridX\":98,\"gridY\":96,\"type\":22},{\"gridX\":129,\"gridY\":96,\"type\":22},{\"gridX\":364,\"gridY\":76,\"type\":3},{\"gridX\":368,\"gridY\":76,\"type\":4},{\"gridX\":382,\"gridY\":76,\"type\":4},{\"gridX\":396,\"gridY\":76,\"type\":5},{\"gridX\":393,\"gridY\":81,\"type\":19},{\"gridX\":400,\"gridY\":124,\"type\":0},{\"gridX\":404,\"gridY\":124,\"type\":1},{\"gridX\":418,\"gridY\":124,\"type\":1},{\"gridX\":432,\"gridY\":124,\"type\":2},{\"gridX\":600,\"gridY\":96,\"type\":7},{\"gridX\":1058,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1025,\"gridY\":96}],\"enemies\":[{\"gridX\":939,\"gridY\":96,\"type\":0}],\"collectibles\":[{\"gridX\":56,\"gridY\":104,\"type\":0},{\"gridX\":64,\"gridY\":108,\"type\":0},{\"gridX\":72,\"gridY\":112,\"type\":0},{\"gridX\":128,\"gridY\":112,\"type\":0},{\"gridX\":136,\"gridY\":116,\"type\":0},{\"gridX\":144,\"gridY\":120,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":304,\"gridY\":132,\"type\":0},{\"gridX\":312,\"gridY\":132,\"type\":0},{\"gridX\":320,\"gridY\":132,\"type\":0},{\"gridX\":328,\"gridY\":132,\"type\":0},{\"gridX\":336,\"gridY\":132,\"type\":0},{\"gridX\":392,\"gridY\":88,\"type\":1},{\"gridX\":612,\"gridY\":124,\"type\":0},{\"gridX\":420,\"gridY\":140,\"type\":0},{\"gridX\":472,\"gridY\":100,\"type\":0},{\"gridX\":620,\"gridY\":128,\"type\":0},{\"gridX\":488,\"gridY\":108,\"type\":0},{\"gridX\":480,\"gridY\":112,\"type\":0},{\"gridX\":488,\"gridY\":100,\"type\":0},{\"gridX\":480,\"gridY\":96,\"type\":0},{\"gridX\":604,\"gridY\":120,\"type\":0},{\"gridX\":412,\"gridY\":132,\"type\":0},{\"gridX\":480,\"gridY\":104,\"type\":0},{\"gridX\":472,\"gridY\":108,\"type\":0},{\"gridX\":648,\"gridY\":152,\"type\":1},{\"gridX\":732,\"gridY\":104,\"type\":0},{\"gridX\":740,\"gridY\":104,\"type\":0},{\"gridX\":748,\"gridY\":104,\"type\":0},{\"gridX\":988,\"gridY\":168,\"type\":1},{\"gridX\":828,\"gridY\":104,\"type\":0},{\"gridX\":836,\"gridY\":104,\"type\":0},{\"gridX\":844,\"gridY\":104,\"type\":0},{\"gridX\":872,\"gridY\":104,\"type\":0},{\"gridX\":880,\"gridY\":104,\"type\":0},{\"gridX\":888,\"gridY\":104,\"type\":0},{\"gridX\":412,\"gridY\":140,\"type\":0},{\"gridX\":404,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":12,\"gridY\":100}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":1064,\"gridY\":0,\"type\":0},{\"gridX\":1192,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

/// Chapter 1 Level 4 ///

Chapter1Level4 * Chapter1Level4::getInstance()
{
    static Chapter1Level4 *instance = new Chapter1Level4("{\"world\":1,\"level\":4,\"midgrounds\":[{\"gridX\":910,\"gridY\":96,\"type\":2},{\"gridX\":966,\"gridY\":96,\"type\":0},{\"gridX\":840,\"gridY\":96,\"type\":1},{\"gridX\":683,\"gridY\":96,\"type\":0},{\"gridX\":776,\"gridY\":96,\"type\":2},{\"gridX\":723,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":668,\"gridY\":0,\"type\":25},{\"gridX\":676,\"gridY\":0,\"type\":27},{\"gridX\":740,\"gridY\":0,\"type\":28},{\"gridX\":868,\"gridY\":0,\"type\":28},{\"gridX\":996,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":0,\"gridY\":94,\"type\":0},{\"gridX\":4,\"gridY\":94,\"type\":1},{\"gridX\":60,\"gridY\":94,\"type\":2},{\"gridX\":92,\"gridY\":130,\"type\":0},{\"gridX\":18,\"gridY\":94,\"type\":1},{\"gridX\":32,\"gridY\":94,\"type\":1},{\"gridX\":46,\"gridY\":94,\"type\":1},{\"gridX\":96,\"gridY\":130,\"type\":1},{\"gridX\":110,\"gridY\":130,\"type\":1},{\"gridX\":124,\"gridY\":130,\"type\":1},{\"gridX\":138,\"gridY\":130,\"type\":2},{\"gridX\":158,\"gridY\":112,\"type\":0},{\"gridX\":162,\"gridY\":112,\"type\":1},{\"gridX\":176,\"gridY\":112,\"type\":1},{\"gridX\":190,\"gridY\":112,\"type\":2},{\"gridX\":192,\"gridY\":104,\"type\":0},{\"gridX\":196,\"gridY\":104,\"type\":1},{\"gridX\":210,\"gridY\":104,\"type\":1},{\"gridX\":224,\"gridY\":104,\"type\":1},{\"gridX\":238,\"gridY\":104,\"type\":2},{\"gridX\":297,\"gridY\":128,\"type\":0},{\"gridX\":315,\"gridY\":128,\"type\":1},{\"gridX\":329,\"gridY\":128,\"type\":2},{\"gridX\":301,\"gridY\":128,\"type\":1},{\"gridX\":337,\"gridY\":157,\"type\":0},{\"gridX\":341,\"gridY\":157,\"type\":1},{\"gridX\":383,\"gridY\":157,\"type\":2},{\"gridX\":355,\"gridY\":157,\"type\":1},{\"gridX\":429,\"gridY\":187,\"type\":0},{\"gridX\":433,\"gridY\":187,\"type\":1},{\"gridX\":461,\"gridY\":187,\"type\":2},{\"gridX\":369,\"gridY\":157,\"type\":1},{\"gridX\":447,\"gridY\":187,\"type\":1},{\"gridX\":672,\"gridY\":96,\"type\":24},{\"gridX\":704,\"gridY\":96,\"type\":24},{\"gridX\":735,\"gridY\":96,\"type\":24},{\"gridX\":766,\"gridY\":96,\"type\":24},{\"gridX\":798,\"gridY\":96,\"type\":24},{\"gridX\":990,\"gridY\":96,\"type\":12},{\"gridX\":67,\"gridY\":90,\"type\":34},{\"gridX\":82,\"gridY\":90,\"type\":34},{\"gridX\":96,\"gridY\":90,\"type\":34},{\"gridX\":110,\"gridY\":90,\"type\":34},{\"gridX\":124,\"gridY\":90,\"type\":34},{\"gridX\":138,\"gridY\":90,\"type\":34},{\"gridX\":151,\"gridY\":90,\"type\":34},{\"gridX\":165,\"gridY\":90,\"type\":34},{\"gridX\":179,\"gridY\":90,\"type\":34},{\"gridX\":238,\"gridY\":87,\"type\":34},{\"gridX\":253,\"gridY\":87,\"type\":34},{\"gridX\":268,\"gridY\":87,\"type\":34},{\"gridX\":283,\"gridY\":87,\"type\":34},{\"gridX\":298,\"gridY\":87,\"type\":34},{\"gridX\":312,\"gridY\":87,\"type\":34},{\"gridX\":326,\"gridY\":87,\"type\":34},{\"gridX\":339,\"gridY\":87,\"type\":34},{\"gridX\":354,\"gridY\":87,\"type\":34},{\"gridX\":369,\"gridY\":87,\"type\":34},{\"gridX\":385,\"gridY\":87,\"type\":34},{\"gridX\":400,\"gridY\":87,\"type\":34},{\"gridX\":414,\"gridY\":87,\"type\":34},{\"gridX\":485,\"gridY\":87,\"type\":34},{\"gridX\":471,\"gridY\":87,\"type\":34},{\"gridX\":457,\"gridY\":87,\"type\":34},{\"gridX\":442,\"gridY\":87,\"type\":34},{\"gridX\":428,\"gridY\":87,\"type\":34},{\"gridX\":500,\"gridY\":87,\"type\":34},{\"gridX\":515,\"gridY\":87,\"type\":34},{\"gridX\":530,\"gridY\":87,\"type\":34},{\"gridX\":558,\"gridY\":87,\"type\":34},{\"gridX\":544,\"gridY\":87,\"type\":34},{\"gridX\":633,\"gridY\":87,\"type\":34},{\"gridX\":617,\"gridY\":87,\"type\":34},{\"gridX\":602,\"gridY\":87,\"type\":34},{\"gridX\":587,\"gridY\":87,\"type\":34},{\"gridX\":572,\"gridY\":87,\"type\":34},{\"gridX\":650,\"gridY\":87,\"type\":34}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":957,\"gridY\":96}],\"enemies\":[{\"gridX\":168,\"gridY\":117,\"type\":2},{\"gridX\":219,\"gridY\":109,\"type\":2}],\"collectibles\":[{\"gridX\":565,\"gridY\":193,\"type\":0},{\"gridX\":580,\"gridY\":190,\"type\":0},{\"gridX\":764,\"gridY\":152,\"type\":0},{\"gridX\":740,\"gridY\":155,\"type\":0},{\"gridX\":718,\"gridY\":161,\"type\":0},{\"gridX\":698,\"gridY\":169,\"type\":0},{\"gridX\":678,\"gridY\":172,\"type\":0},{\"gridX\":657,\"gridY\":177,\"type\":0},{\"gridX\":641,\"gridY\":181,\"type\":0},{\"gridX\":623,\"gridY\":183,\"type\":0},{\"gridX\":609,\"gridY\":185,\"type\":0},{\"gridX\":596,\"gridY\":188,\"type\":0},{\"gridX\":835,\"gridY\":127,\"type\":0},{\"gridX\":810,\"gridY\":137,\"type\":0},{\"gridX\":786,\"gridY\":145,\"type\":0},{\"gridX\":857,\"gridY\":115,\"type\":0},{\"gridX\":23,\"gridY\":107,\"type\":0},{\"gridX\":35,\"gridY\":106,\"type\":0},{\"gridX\":48,\"gridY\":106,\"type\":0},{\"gridX\":100,\"gridY\":142,\"type\":0},{\"gridX\":115,\"gridY\":141,\"type\":0},{\"gridX\":129,\"gridY\":141,\"type\":0},{\"gridX\":166,\"gridY\":134,\"type\":0},{\"gridX\":218,\"gridY\":125,\"type\":0},{\"gridX\":305,\"gridY\":139,\"type\":0},{\"gridX\":316,\"gridY\":138,\"type\":0},{\"gridX\":357,\"gridY\":168,\"type\":0},{\"gridX\":367,\"gridY\":168,\"type\":0},{\"gridX\":379,\"gridY\":168,\"type\":0},{\"gridX\":432,\"gridY\":197,\"type\":0},{\"gridX\":452,\"gridY\":197,\"type\":0},{\"gridX\":409,\"gridY\":191,\"type\":1},{\"gridX\":272,\"gridY\":121,\"type\":1},{\"gridX\":909,\"gridY\":102,\"type\":0},{\"gridX\":921,\"gridY\":102,\"type\":0},{\"gridX\":933,\"gridY\":102,\"type\":0},{\"gridX\":943,\"gridY\":102,\"type\":0},{\"gridX\":952,\"gridY\":101,\"type\":0},{\"gridX\":961,\"gridY\":100,\"type\":0},{\"gridX\":973,\"gridY\":100,\"type\":1}],\"jons\":[{\"gridX\":0,\"gridY\":109}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":996,\"gridY\":0,\"type\":0},{\"gridX\":1124,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

/// Chapter 1 Level 5 ///

Chapter1Level5 * Chapter1Level5::getInstance()
{
    static Chapter1Level5 *instance = new Chapter1Level5("{\"world\":1,\"level\":5,\"midgrounds\":[{\"gridX\":1556,\"gridY\":96,\"type\":2},{\"gridX\":1161,\"gridY\":96,\"type\":2},{\"gridX\":1508,\"gridY\":96,\"type\":1},{\"gridX\":1476,\"gridY\":96,\"type\":0},{\"gridX\":1082,\"gridY\":96,\"type\":1},{\"gridX\":1086,\"gridY\":96,\"type\":0},{\"gridX\":864,\"gridY\":96,\"type\":0},{\"gridX\":812,\"gridY\":96,\"type\":2},{\"gridX\":897,\"gridY\":96,\"type\":0},{\"gridX\":549,\"gridY\":96,\"type\":2},{\"gridX\":616,\"gridY\":96,\"type\":1},{\"gridX\":336,\"gridY\":96,\"type\":1},{\"gridX\":465,\"gridY\":96,\"type\":0},{\"gridX\":389,\"gridY\":96,\"type\":2},{\"gridX\":199,\"gridY\":96,\"type\":2},{\"gridX\":262,\"gridY\":96,\"type\":0},{\"gridX\":4,\"gridY\":96,\"type\":2},{\"gridX\":66,\"gridY\":96,\"type\":1},{\"gridX\":111,\"gridY\":96,\"type\":0},{\"gridX\":1685,\"gridY\":96,\"type\":2},{\"gridX\":1646,\"gridY\":96,\"type\":1},{\"gridX\":1614,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":1544,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1608,\"gridY\":0,\"type\":28}],\"pits\":[{\"gridX\":161,\"gridY\":0,\"type\":30},{\"gridX\":309,\"gridY\":0,\"type\":30},{\"gridX\":367,\"gridY\":0,\"type\":30},{\"gridX\":490,\"gridY\":0,\"type\":31},{\"gridX\":663,\"gridY\":0,\"type\":31},{\"gridX\":743,\"gridY\":0,\"type\":31},{\"gridX\":927,\"gridY\":0,\"type\":32},{\"gridX\":1213,\"gridY\":0,\"type\":33}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1602,\"gridY\":96,\"type\":12},{\"gridX\":154,\"gridY\":96,\"type\":19},{\"gridX\":1122,\"gridY\":96,\"type\":24}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1569,\"gridY\":96}],\"enemies\":[{\"gridX\":458,\"gridY\":96,\"type\":4},{\"gridX\":855,\"gridY\":96,\"type\":4},{\"gridX\":1532,\"gridY\":96,\"type\":4}],\"collectibles\":[{\"gridX\":216,\"gridY\":154,\"type\":1},{\"gridX\":353,\"gridY\":121,\"type\":0},{\"gridX\":260,\"gridY\":105,\"type\":0},{\"gridX\":276,\"gridY\":105,\"type\":0},{\"gridX\":288,\"gridY\":105,\"type\":0},{\"gridX\":49,\"gridY\":101,\"type\":0},{\"gridX\":60,\"gridY\":101,\"type\":0},{\"gridX\":72,\"gridY\":100,\"type\":0},{\"gridX\":37,\"gridY\":101,\"type\":0},{\"gridX\":353,\"gridY\":113,\"type\":0},{\"gridX\":353,\"gridY\":104,\"type\":0},{\"gridX\":415,\"gridY\":113,\"type\":0},{\"gridX\":428,\"gridY\":104,\"type\":0},{\"gridX\":461,\"gridY\":122,\"type\":0},{\"gridX\":472,\"gridY\":120,\"type\":0},{\"gridX\":519,\"gridY\":126,\"type\":0},{\"gridX\":530,\"gridY\":126,\"type\":0},{\"gridX\":596,\"gridY\":107,\"type\":0},{\"gridX\":605,\"gridY\":107,\"type\":0},{\"gridX\":613,\"gridY\":106,\"type\":0},{\"gridX\":622,\"gridY\":106,\"type\":0},{\"gridX\":784,\"gridY\":81,\"type\":1},{\"gridX\":868,\"gridY\":119,\"type\":0},{\"gridX\":829,\"gridY\":105,\"type\":0},{\"gridX\":903,\"gridY\":105,\"type\":0},{\"gridX\":1281,\"gridY\":144,\"type\":1},{\"gridX\":1220,\"gridY\":113,\"type\":0},{\"gridX\":1231,\"gridY\":120,\"type\":0},{\"gridX\":1239,\"gridY\":125,\"type\":0},{\"gridX\":1246,\"gridY\":131,\"type\":0},{\"gridX\":1252,\"gridY\":135,\"type\":0},{\"gridX\":1260,\"gridY\":138,\"type\":0},{\"gridX\":1308,\"gridY\":124,\"type\":0},{\"gridX\":1322,\"gridY\":122,\"type\":0},{\"gridX\":1333,\"gridY\":119,\"type\":0},{\"gridX\":1346,\"gridY\":118,\"type\":0},{\"gridX\":1360,\"gridY\":116,\"type\":0},{\"gridX\":1374,\"gridY\":114,\"type\":0},{\"gridX\":1386,\"gridY\":112,\"type\":0},{\"gridX\":1400,\"gridY\":111,\"type\":0},{\"gridX\":942,\"gridY\":123,\"type\":0},{\"gridX\":961,\"gridY\":126,\"type\":0},{\"gridX\":984,\"gridY\":129,\"type\":0},{\"gridX\":1003,\"gridY\":129,\"type\":0},{\"gridX\":1026,\"gridY\":125,\"type\":0},{\"gridX\":1041,\"gridY\":121,\"type\":0},{\"gridX\":1122,\"gridY\":122,\"type\":0},{\"gridX\":1133,\"gridY\":122,\"type\":0},{\"gridX\":1144,\"gridY\":122,\"type\":0},{\"gridX\":1413,\"gridY\":109,\"type\":0},{\"gridX\":1426,\"gridY\":107,\"type\":0},{\"gridX\":1441,\"gridY\":106,\"type\":0},{\"gridX\":1521,\"gridY\":99,\"type\":0}],\"jons\":[{\"gridX\":11,\"gridY\":102}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":1608,\"gridY\":0,\"type\":0},{\"gridX\":1736,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

/// Chapter 1 Level 6 ///

Chapter1Level6 * Chapter1Level6::getInstance()
{
    static Chapter1Level6 *instance = new Chapter1Level6("{\"world\":1,\"level\":6,\"midgrounds\":[{\"gridX\":144,\"gridY\":96,\"type\":2},{\"gridX\":92,\"gridY\":96,\"type\":0},{\"gridX\":28,\"gridY\":96,\"type\":1},{\"gridX\":428,\"gridY\":96,\"type\":2},{\"gridX\":483,\"gridY\":96,\"type\":1},{\"gridX\":750,\"gridY\":96,\"type\":0},{\"gridX\":933,\"gridY\":96,\"type\":0},{\"gridX\":786,\"gridY\":96,\"type\":2},{\"gridX\":860,\"gridY\":96,\"type\":2},{\"gridX\":978,\"gridY\":96,\"type\":2},{\"gridX\":1015,\"gridY\":96,\"type\":1},{\"gridX\":1054,\"gridY\":96,\"type\":0},{\"gridX\":0,\"gridY\":96,\"type\":0},{\"gridX\":111,\"gridY\":96,\"type\":1},{\"gridX\":51,\"gridY\":96,\"type\":2},{\"gridX\":268,\"gridY\":96,\"type\":1},{\"gridX\":549,\"gridY\":96,\"type\":2},{\"gridX\":534,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":368,\"gridY\":0,\"type\":27},{\"gridX\":560,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":432,\"gridY\":0,\"type\":28},{\"gridX\":724,\"gridY\":0,\"type\":28},{\"gridX\":852,\"gridY\":0,\"type\":28},{\"gridX\":980,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":596,\"gridY\":0,\"type\":28}],\"pits\":[{\"gridX\":311,\"gridY\":0,\"type\":32},{\"gridX\":599,\"gridY\":0,\"type\":32}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":228,\"gridY\":96,\"type\":6},{\"gridX\":172,\"gridY\":96,\"type\":24},{\"gridX\":604,\"gridY\":120,\"type\":0},{\"gridX\":608,\"gridY\":120,\"type\":1},{\"gridX\":622,\"gridY\":120,\"type\":2},{\"gridX\":974,\"gridY\":96,\"type\":12},{\"gridX\":796,\"gridY\":106,\"type\":0},{\"gridX\":800,\"gridY\":106,\"type\":1},{\"gridX\":814,\"gridY\":106,\"type\":2},{\"gridX\":886,\"gridY\":96,\"type\":24},{\"gridX\":918,\"gridY\":96,\"type\":24},{\"gridX\":542,\"gridY\":96,\"type\":14},{\"gridX\":537,\"gridY\":96,\"type\":13},{\"gridX\":287,\"gridY\":96,\"type\":24},{\"gridX\":270,\"gridY\":96,\"type\":23},{\"gridX\":264,\"gridY\":96,\"type\":22},{\"gridX\":553,\"gridY\":96,\"type\":15},{\"gridX\":575,\"gridY\":96,\"type\":15},{\"gridX\":597,\"gridY\":96,\"type\":16}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":941,\"gridY\":96}],\"enemies\":[{\"gridX\":790,\"gridY\":96,\"type\":2},{\"gridX\":801,\"gridY\":111,\"type\":2},{\"gridX\":104,\"gridY\":96,\"type\":2},{\"gridX\":432,\"gridY\":96,\"type\":2}],\"collectibles\":[{\"gridX\":40,\"gridY\":104,\"type\":0},{\"gridX\":48,\"gridY\":104,\"type\":0},{\"gridX\":56,\"gridY\":104,\"type\":0},{\"gridX\":164,\"gridY\":112,\"type\":0},{\"gridX\":172,\"gridY\":116,\"type\":0},{\"gridX\":180,\"gridY\":120,\"type\":0},{\"gridX\":188,\"gridY\":124,\"type\":0},{\"gridX\":328,\"gridY\":136,\"type\":1},{\"gridX\":690,\"gridY\":170,\"type\":1},{\"gridX\":638,\"gridY\":142,\"type\":0},{\"gridX\":653,\"gridY\":151,\"type\":0},{\"gridX\":668,\"gridY\":157,\"type\":0},{\"gridX\":678,\"gridY\":163,\"type\":0},{\"gridX\":884,\"gridY\":112,\"type\":0},{\"gridX\":892,\"gridY\":116,\"type\":0},{\"gridX\":900,\"gridY\":120,\"type\":0},{\"gridX\":908,\"gridY\":120,\"type\":0},{\"gridX\":916,\"gridY\":120,\"type\":0},{\"gridX\":924,\"gridY\":120,\"type\":0},{\"gridX\":932,\"gridY\":120,\"type\":0},{\"gridX\":939,\"gridY\":119,\"type\":0},{\"gridX\":972,\"gridY\":120,\"type\":1},{\"gridX\":455,\"gridY\":102,\"type\":0},{\"gridX\":463,\"gridY\":107,\"type\":0},{\"gridX\":471,\"gridY\":102,\"type\":0},{\"gridX\":479,\"gridY\":108,\"type\":0},{\"gridX\":486,\"gridY\":101,\"type\":0},{\"gridX\":494,\"gridY\":107,\"type\":0},{\"gridX\":500,\"gridY\":99,\"type\":0},{\"gridX\":565,\"gridY\":117,\"type\":0},{\"gridX\":574,\"gridY\":122,\"type\":0},{\"gridX\":584,\"gridY\":127,\"type\":0}],\"jons\":[{\"gridX\":2,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":980,\"gridY\":0,\"type\":0},{\"gridX\":1108,\"gridY\":0,\"type\":1},{\"gridX\":1126,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

/// Chapter 1 Level 7 ///

Chapter1Level7 * Chapter1Level7::getInstance()
{
    static Chapter1Level7 *instance = new Chapter1Level7("{\"world\":1,\"level\":7,\"midgrounds\":[{\"gridX\":40,\"gridY\":96,\"type\":2},{\"gridX\":0,\"gridY\":96,\"type\":0},{\"gridX\":178,\"gridY\":96,\"type\":1},{\"gridX\":56,\"gridY\":96,\"type\":0},{\"gridX\":64,\"gridY\":96,\"type\":2},{\"gridX\":923,\"gridY\":96,\"type\":2},{\"gridX\":878,\"gridY\":96,\"type\":1},{\"gridX\":912,\"gridY\":96,\"type\":0},{\"gridX\":812,\"gridY\":96,\"type\":0},{\"gridX\":830,\"gridY\":96,\"type\":2},{\"gridX\":972,\"gridY\":96,\"type\":1},{\"gridX\":818,\"gridY\":96,\"type\":1},{\"gridX\":868,\"gridY\":96,\"type\":0},{\"gridX\":574,\"gridY\":96,\"type\":2},{\"gridX\":511,\"gridY\":96,\"type\":1},{\"gridX\":349,\"gridY\":96,\"type\":0},{\"gridX\":309,\"gridY\":96,\"type\":2},{\"gridX\":16,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":308,\"gridY\":0,\"type\":25},{\"gridX\":516,\"gridY\":0,\"type\":25},{\"gridX\":808,\"gridY\":0,\"type\":25},{\"gridX\":200,\"gridY\":0,\"type\":26},{\"gridX\":316,\"gridY\":0,\"type\":27},{\"gridX\":944,\"gridY\":0,\"type\":27},{\"gridX\":136,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":524,\"gridY\":0,\"type\":28},{\"gridX\":816,\"gridY\":0,\"type\":28},{\"gridX\":1008,\"gridY\":0,\"type\":28},{\"gridX\":232,\"gridY\":0,\"type\":29},{\"gridX\":380,\"gridY\":0,\"type\":29},{\"gridX\":652,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":124,\"gridY\":0,\"type\":31}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":225,\"gridY\":96,\"type\":17},{\"gridX\":252,\"gridY\":132,\"type\":0},{\"gridX\":256,\"gridY\":132,\"type\":1},{\"gridX\":270,\"gridY\":132,\"type\":2},{\"gridX\":400,\"gridY\":148,\"type\":0},{\"gridX\":446,\"gridY\":148,\"type\":1},{\"gridX\":432,\"gridY\":148,\"type\":1},{\"gridX\":418,\"gridY\":148,\"type\":1},{\"gridX\":404,\"gridY\":148,\"type\":1},{\"gridX\":460,\"gridY\":148,\"type\":1},{\"gridX\":474,\"gridY\":148,\"type\":1},{\"gridX\":488,\"gridY\":148,\"type\":2},{\"gridX\":435,\"gridY\":153,\"type\":13},{\"gridX\":440,\"gridY\":153,\"type\":14},{\"gridX\":451,\"gridY\":153,\"type\":16},{\"gridX\":556,\"gridY\":186,\"type\":34},{\"gridX\":556,\"gridY\":172,\"type\":34},{\"gridX\":556,\"gridY\":158,\"type\":34},{\"gridX\":556,\"gridY\":144,\"type\":34},{\"gridX\":577,\"gridY\":96,\"type\":8},{\"gridX\":645,\"gridY\":96,\"type\":10},{\"gridX\":1003,\"gridY\":96,\"type\":12},{\"gridX\":244,\"gridY\":82,\"type\":34},{\"gridX\":262,\"gridY\":82,\"type\":34},{\"gridX\":278,\"gridY\":82,\"type\":34},{\"gridX\":293,\"gridY\":82,\"type\":34},{\"gridX\":388,\"gridY\":77,\"type\":34},{\"gridX\":403,\"gridY\":77,\"type\":34},{\"gridX\":419,\"gridY\":77,\"type\":34},{\"gridX\":435,\"gridY\":77,\"type\":34},{\"gridX\":450,\"gridY\":77,\"type\":34},{\"gridX\":466,\"gridY\":77,\"type\":34},{\"gridX\":482,\"gridY\":77,\"type\":34},{\"gridX\":500,\"gridY\":77,\"type\":34},{\"gridX\":661,\"gridY\":80,\"type\":34},{\"gridX\":679,\"gridY\":80,\"type\":34},{\"gridX\":696,\"gridY\":80,\"type\":34},{\"gridX\":714,\"gridY\":80,\"type\":34},{\"gridX\":730,\"gridY\":80,\"type\":34},{\"gridX\":746,\"gridY\":80,\"type\":34},{\"gridX\":762,\"gridY\":80,\"type\":34},{\"gridX\":777,\"gridY\":80,\"type\":34},{\"gridX\":793,\"gridY\":80,\"type\":34}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":970,\"gridY\":96}],\"enemies\":[{\"gridX\":160,\"gridY\":120,\"type\":3},{\"gridX\":253,\"gridY\":100,\"type\":3},{\"gridX\":282,\"gridY\":100,\"type\":3},{\"gridX\":324,\"gridY\":96,\"type\":2},{\"gridX\":354,\"gridY\":96,\"type\":0},{\"gridX\":482,\"gridY\":153,\"type\":2},{\"gridX\":409,\"gridY\":100,\"type\":3},{\"gridX\":432,\"gridY\":108,\"type\":3},{\"gridX\":451,\"gridY\":101,\"type\":3},{\"gridX\":474,\"gridY\":109,\"type\":3},{\"gridX\":698,\"gridY\":139,\"type\":3},{\"gridX\":726,\"gridY\":134,\"type\":3},{\"gridX\":751,\"gridY\":132,\"type\":3},{\"gridX\":777,\"gridY\":128,\"type\":3},{\"gridX\":914,\"gridY\":119,\"type\":3},{\"gridX\":917,\"gridY\":96,\"type\":2}],\"collectibles\":[{\"gridX\":260,\"gridY\":150,\"type\":1},{\"gridX\":516,\"gridY\":170,\"type\":1},{\"gridX\":941,\"gridY\":160,\"type\":1},{\"gridX\":59,\"gridY\":103,\"type\":0},{\"gridX\":90,\"gridY\":102,\"type\":0},{\"gridX\":111,\"gridY\":102,\"type\":0},{\"gridX\":161,\"gridY\":134,\"type\":0},{\"gridX\":233,\"gridY\":126,\"type\":0},{\"gridX\":243,\"gridY\":137,\"type\":0},{\"gridX\":308,\"gridY\":120,\"type\":0},{\"gridX\":316,\"gridY\":109,\"type\":0},{\"gridX\":340,\"gridY\":111,\"type\":0},{\"gridX\":363,\"gridY\":118,\"type\":0},{\"gridX\":369,\"gridY\":131,\"type\":0},{\"gridX\":375,\"gridY\":140,\"type\":0},{\"gridX\":383,\"gridY\":149,\"type\":0},{\"gridX\":394,\"gridY\":159,\"type\":0},{\"gridX\":406,\"gridY\":166,\"type\":0},{\"gridX\":452,\"gridY\":174,\"type\":0},{\"gridX\":473,\"gridY\":168,\"type\":0},{\"gridX\":537,\"gridY\":130,\"type\":0},{\"gridX\":541,\"gridY\":120,\"type\":0},{\"gridX\":543,\"gridY\":111,\"type\":0},{\"gridX\":577,\"gridY\":114,\"type\":0},{\"gridX\":609,\"gridY\":118,\"type\":0},{\"gridX\":617,\"gridY\":122,\"type\":0},{\"gridX\":623,\"gridY\":126,\"type\":0},{\"gridX\":692,\"gridY\":118,\"type\":0},{\"gridX\":711,\"gridY\":115,\"type\":0},{\"gridX\":732,\"gridY\":114,\"type\":0},{\"gridX\":747,\"gridY\":114,\"type\":0},{\"gridX\":770,\"gridY\":112,\"type\":0},{\"gridX\":786,\"gridY\":111,\"type\":0},{\"gridX\":839,\"gridY\":104,\"type\":0},{\"gridX\":859,\"gridY\":104,\"type\":0},{\"gridX\":871,\"gridY\":104,\"type\":0},{\"gridX\":849,\"gridY\":103,\"type\":0},{\"gridX\":423,\"gridY\":172,\"type\":0}],\"jons\":[{\"gridX\":5,\"gridY\":98}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":1008,\"gridY\":0,\"type\":0},{\"gridX\":1136,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

/// Chapter 1 Level 8 ///

Chapter1Level8 * Chapter1Level8::getInstance()
{
    static Chapter1Level8 *instance = new Chapter1Level8("{\"world\":1,\"level\":8,\"midgrounds\":[{\"gridX\":752,\"gridY\":96,\"type\":2},{\"gridX\":822,\"gridY\":96,\"type\":1},{\"gridX\":904,\"gridY\":96,\"type\":0},{\"gridX\":956,\"gridY\":96,\"type\":0},{\"gridX\":799,\"gridY\":96,\"type\":0},{\"gridX\":918,\"gridY\":96,\"type\":2},{\"gridX\":879,\"gridY\":96,\"type\":1},{\"gridX\":840,\"gridY\":96,\"type\":2},{\"gridX\":871,\"gridY\":96,\"type\":0},{\"gridX\":587,\"gridY\":96,\"type\":0},{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":51,\"gridY\":96,\"type\":1},{\"gridX\":37,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":2,\"gridY\":0,\"type\":25},{\"gridX\":223,\"gridY\":0,\"type\":25},{\"gridX\":378,\"gridY\":0,\"type\":25},{\"gridX\":554,\"gridY\":0,\"type\":25},{\"gridX\":10,\"gridY\":0,\"type\":26},{\"gridX\":42,\"gridY\":0,\"type\":27},{\"gridX\":562,\"gridY\":0,\"type\":27},{\"gridX\":882,\"gridY\":0,\"type\":27},{\"gridX\":754,\"gridY\":0,\"type\":28},{\"gridX\":626,\"gridY\":0,\"type\":28},{\"gridX\":946,\"gridY\":0,\"type\":28},{\"gridX\":106,\"gridY\":0,\"type\":29},{\"gridX\":231,\"gridY\":0,\"type\":29},{\"gridX\":384,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":618,\"gridY\":0,\"type\":32}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":876,\"gridY\":96,\"type\":12},{\"gridX\":112,\"gridY\":99,\"type\":34},{\"gridX\":118,\"gridY\":112,\"type\":34},{\"gridX\":126,\"gridY\":122,\"type\":34},{\"gridX\":132,\"gridY\":132,\"type\":34},{\"gridX\":140,\"gridY\":142,\"type\":34},{\"gridX\":151,\"gridY\":150,\"type\":34},{\"gridX\":165,\"gridY\":146,\"type\":34},{\"gridX\":191,\"gridY\":108,\"type\":34},{\"gridX\":184,\"gridY\":122,\"type\":34},{\"gridX\":176,\"gridY\":135,\"type\":34},{\"gridX\":199,\"gridY\":90,\"type\":34},{\"gridX\":223,\"gridY\":96,\"type\":20},{\"gridX\":246,\"gridY\":94,\"type\":34},{\"gridX\":264,\"gridY\":146,\"type\":34},{\"gridX\":275,\"gridY\":169,\"type\":34},{\"gridX\":282,\"gridY\":179,\"type\":34},{\"gridX\":270,\"gridY\":158,\"type\":34},{\"gridX\":259,\"gridY\":134,\"type\":34},{\"gridX\":256,\"gridY\":120,\"type\":34},{\"gridX\":251,\"gridY\":106,\"type\":34},{\"gridX\":289,\"gridY\":188,\"type\":34},{\"gridX\":297,\"gridY\":199,\"type\":34},{\"gridX\":310,\"gridY\":197,\"type\":34},{\"gridX\":320,\"gridY\":189,\"type\":34},{\"gridX\":327,\"gridY\":180,\"type\":34},{\"gridX\":332,\"gridY\":169,\"type\":34},{\"gridX\":338,\"gridY\":156,\"type\":34},{\"gridX\":344,\"gridY\":143,\"type\":34},{\"gridX\":355,\"gridY\":114,\"type\":34},{\"gridX\":349,\"gridY\":129,\"type\":34},{\"gridX\":381,\"gridY\":96,\"type\":21},{\"gridX\":361,\"gridY\":99,\"type\":34},{\"gridX\":399,\"gridY\":91,\"type\":34},{\"gridX\":428,\"gridY\":164,\"type\":34},{\"gridX\":421,\"gridY\":151,\"type\":34},{\"gridX\":416,\"gridY\":138,\"type\":34},{\"gridX\":410,\"gridY\":122,\"type\":34},{\"gridX\":405,\"gridY\":106,\"type\":34},{\"gridX\":435,\"gridY\":176,\"type\":34},{\"gridX\":442,\"gridY\":188,\"type\":34},{\"gridX\":448,\"gridY\":200,\"type\":34},{\"gridX\":457,\"gridY\":210,\"type\":34},{\"gridX\":514,\"gridY\":210,\"type\":34},{\"gridX\":500,\"gridY\":210,\"type\":34},{\"gridX\":486,\"gridY\":210,\"type\":34},{\"gridX\":472,\"gridY\":210,\"type\":34},{\"gridX\":519,\"gridY\":197,\"type\":34},{\"gridX\":525,\"gridY\":183,\"type\":34},{\"gridX\":539,\"gridY\":123,\"type\":34},{\"gridX\":536,\"gridY\":138,\"type\":34},{\"gridX\":533,\"gridY\":153,\"type\":34},{\"gridX\":530,\"gridY\":168,\"type\":34},{\"gridX\":541,\"gridY\":107,\"type\":34},{\"gridX\":100,\"gridY\":96,\"type\":19},{\"gridX\":567,\"gridY\":96,\"type\":10},{\"gridX\":43,\"gridY\":96,\"type\":8}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":844,\"gridY\":96}],\"enemies\":[{\"gridX\":746,\"gridY\":96,\"type\":0}],\"collectibles\":[{\"gridX\":592,\"gridY\":136,\"type\":1},{\"gridX\":224,\"gridY\":104,\"type\":1},{\"gridX\":774,\"gridY\":142,\"type\":1},{\"gridX\":25,\"gridY\":98,\"type\":0},{\"gridX\":16,\"gridY\":98,\"type\":0},{\"gridX\":104,\"gridY\":114,\"type\":0},{\"gridX\":33,\"gridY\":98,\"type\":0},{\"gridX\":126,\"gridY\":156,\"type\":0},{\"gridX\":117,\"gridY\":143,\"type\":0},{\"gridX\":109,\"gridY\":129,\"type\":0},{\"gridX\":196,\"gridY\":153,\"type\":0},{\"gridX\":205,\"gridY\":140,\"type\":0},{\"gridX\":213,\"gridY\":126,\"type\":0},{\"gridX\":236,\"gridY\":141,\"type\":0},{\"gridX\":241,\"gridY\":156,\"type\":0},{\"gridX\":250,\"gridY\":172,\"type\":0},{\"gridX\":257,\"gridY\":188,\"type\":0},{\"gridX\":267,\"gridY\":201,\"type\":0},{\"gridX\":278,\"gridY\":213,\"type\":0},{\"gridX\":344,\"gridY\":202,\"type\":0},{\"gridX\":355,\"gridY\":186,\"type\":0},{\"gridX\":363,\"gridY\":168,\"type\":0},{\"gridX\":370,\"gridY\":150,\"type\":0},{\"gridX\":376,\"gridY\":132,\"type\":0},{\"gridX\":392,\"gridY\":150,\"type\":0},{\"gridX\":444,\"gridY\":128,\"type\":0},{\"gridX\":413,\"gridY\":218,\"type\":0},{\"gridX\":407,\"gridY\":204,\"type\":0},{\"gridX\":403,\"gridY\":188,\"type\":0},{\"gridX\":399,\"gridY\":168,\"type\":0},{\"gridX\":551,\"gridY\":197,\"type\":0},{\"gridX\":557,\"gridY\":178,\"type\":0},{\"gridX\":561,\"gridY\":157,\"type\":0},{\"gridX\":564,\"gridY\":138,\"type\":0},{\"gridX\":625,\"gridY\":133,\"type\":0},{\"gridX\":646,\"gridY\":128,\"type\":0},{\"gridX\":665,\"gridY\":125,\"type\":0},{\"gridX\":683,\"gridY\":119,\"type\":0},{\"gridX\":701,\"gridY\":114,\"type\":0},{\"gridX\":717,\"gridY\":110,\"type\":0}],\"jons\":[{\"gridX\":0,\"gridY\":114}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 9 ///

Chapter1Level9 * Chapter1Level9::getInstance()
{
    static Chapter1Level9 *instance = new Chapter1Level9("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":1678,\"gridY\":96,\"type\":0},{\"gridX\":1631,\"gridY\":96,\"type\":2},{\"gridX\":1524,\"gridY\":96,\"type\":2},{\"gridX\":1583,\"gridY\":96,\"type\":0},{\"gridX\":1227,\"gridY\":96,\"type\":2},{\"gridX\":1129,\"gridY\":96,\"type\":1},{\"gridX\":1198,\"gridY\":96,\"type\":0},{\"gridX\":1079,\"gridY\":96,\"type\":0},{\"gridX\":952,\"gridY\":96,\"type\":2},{\"gridX\":1007,\"gridY\":96,\"type\":0},{\"gridX\":884,\"gridY\":96,\"type\":1},{\"gridX\":852,\"gridY\":96,\"type\":0},{\"gridX\":790,\"gridY\":96,\"type\":2},{\"gridX\":445,\"gridY\":96,\"type\":1},{\"gridX\":508,\"gridY\":96,\"type\":0},{\"gridX\":471,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":2},{\"gridX\":231,\"gridY\":96,\"type\":0},{\"gridX\":301,\"gridY\":96,\"type\":0},{\"gridX\":189,\"gridY\":96,\"type\":1},{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":42,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":27},{\"gridX\":1542,\"gridY\":0,\"type\":27},{\"gridX\":198,\"gridY\":0,\"type\":27},{\"gridX\":262,\"gridY\":0,\"type\":27},{\"gridX\":838,\"gridY\":0,\"type\":27},{\"gridX\":70,\"gridY\":0,\"type\":28},{\"gridX\":1604,\"gridY\":0,\"type\":28},{\"gridX\":326,\"gridY\":0,\"type\":28},{\"gridX\":454,\"gridY\":0,\"type\":28},{\"gridX\":582,\"gridY\":0,\"type\":28},{\"gridX\":710,\"gridY\":0,\"type\":28},{\"gridX\":902,\"gridY\":0,\"type\":28},{\"gridX\":1030,\"gridY\":0,\"type\":28},{\"gridX\":1158,\"gridY\":0,\"type\":28},{\"gridX\":1286,\"gridY\":0,\"type\":28},{\"gridX\":1414,\"gridY\":0,\"type\":28}],\"pits\":[{\"gridX\":70,\"gridY\":0,\"type\":32},{\"gridX\":320,\"gridY\":0,\"type\":32},{\"gridX\":548,\"gridY\":2,\"type\":33},{\"gridX\":1268,\"gridY\":0,\"type\":33}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1657,\"gridY\":96,\"type\":12},{\"gridX\":100,\"gridY\":105,\"type\":34},{\"gridX\":144,\"gridY\":105,\"type\":34},{\"gridX\":253,\"gridY\":95,\"type\":35},{\"gridX\":294,\"gridY\":132,\"type\":34},{\"gridX\":376,\"gridY\":150,\"type\":34},{\"gridX\":376,\"gridY\":120,\"type\":34},{\"gridX\":376,\"gridY\":135,\"type\":34},{\"gridX\":470,\"gridY\":96,\"type\":24},{\"gridX\":502,\"gridY\":96,\"type\":24},{\"gridX\":535,\"gridY\":96,\"type\":20},{\"gridX\":827,\"gridY\":161,\"type\":34},{\"gridX\":827,\"gridY\":176,\"type\":34},{\"gridX\":827,\"gridY\":191,\"type\":34},{\"gridX\":827,\"gridY\":206,\"type\":34},{\"gridX\":827,\"gridY\":145,\"type\":34},{\"gridX\":827,\"gridY\":129,\"type\":34},{\"gridX\":859,\"gridY\":96,\"type\":13},{\"gridX\":864,\"gridY\":96,\"type\":15},{\"gridX\":886,\"gridY\":96,\"type\":16},{\"gridX\":956,\"gridY\":96,\"type\":17},{\"gridX\":995,\"gridY\":96,\"type\":17},{\"gridX\":975,\"gridY\":96,\"type\":18},{\"gridX\":1015,\"gridY\":96,\"type\":10},{\"gridX\":1046,\"gridY\":96,\"type\":6},{\"gridX\":1029,\"gridY\":96,\"type\":23},{\"gridX\":1081,\"gridY\":114,\"type\":34},{\"gridX\":1101,\"gridY\":114,\"type\":34},{\"gridX\":1120,\"gridY\":114,\"type\":34},{\"gridX\":1164,\"gridY\":114,\"type\":34},{\"gridX\":1180,\"gridY\":114,\"type\":34},{\"gridX\":1582,\"gridY\":96,\"type\":10}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1617,\"gridY\":96}],\"enemies\":[{\"gridX\":277,\"gridY\":96,\"type\":0},{\"gridX\":955,\"gridY\":105,\"type\":2},{\"gridX\":974,\"gridY\":101,\"type\":2},{\"gridX\":1015,\"gridY\":112,\"type\":2},{\"gridX\":993,\"gridY\":106,\"type\":2},{\"gridX\":1300,\"gridY\":110,\"type\":3},{\"gridX\":1367,\"gridY\":108,\"type\":3},{\"gridX\":1438,\"gridY\":106,\"type\":3},{\"gridX\":1483,\"gridY\":112,\"type\":3},{\"gridX\":1564,\"gridY\":96,\"type\":4},{\"gridX\":1580,\"gridY\":112,\"type\":4}],\"collectibles\":[{\"gridX\":1155,\"gridY\":102,\"type\":1},{\"gridX\":1636,\"gridY\":146,\"type\":1},{\"gridX\":126,\"gridY\":106,\"type\":1},{\"gridX\":51,\"gridY\":104,\"type\":0},{\"gridX\":40,\"gridY\":104,\"type\":0},{\"gridX\":27,\"gridY\":104,\"type\":0},{\"gridX\":198,\"gridY\":104,\"type\":0},{\"gridX\":213,\"gridY\":104,\"type\":0},{\"gridX\":305,\"gridY\":102,\"type\":0},{\"gridX\":315,\"gridY\":102,\"type\":0},{\"gridX\":491,\"gridY\":119,\"type\":0},{\"gridX\":480,\"gridY\":119,\"type\":0},{\"gridX\":502,\"gridY\":119,\"type\":0},{\"gridX\":516,\"gridY\":119,\"type\":0},{\"gridX\":637,\"gridY\":191,\"type\":0},{\"gridX\":657,\"gridY\":187,\"type\":0},{\"gridX\":677,\"gridY\":183,\"type\":0},{\"gridX\":694,\"gridY\":179,\"type\":0},{\"gridX\":712,\"gridY\":175,\"type\":0},{\"gridX\":733,\"gridY\":170,\"type\":0},{\"gridX\":751,\"gridY\":165,\"type\":0},{\"gridX\":619,\"gridY\":193,\"type\":0},{\"gridX\":872,\"gridY\":112,\"type\":0},{\"gridX\":1051,\"gridY\":136,\"type\":0},{\"gridX\":1062,\"gridY\":136,\"type\":0},{\"gridX\":1238,\"gridY\":102,\"type\":0},{\"gridX\":1264,\"gridY\":101,\"type\":0},{\"gridX\":1251,\"gridY\":101,\"type\":0}],\"jons\":[{\"gridX\":6,\"gridY\":103}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":71,\"gridY\":0,\"type\":0}]}");
    
    return instance;
}

/// Chapter 1 Level 11 ///

Chapter1Level11 * Chapter1Level11::getInstance()
{
    static Chapter1Level11 *instance = new Chapter1Level11("{\"world\":1,\"level\":11,\"midgrounds\":[{\"gridX\":64,\"gridY\":96,\"type\":1},{\"gridX\":3,\"gridY\":96,\"type\":0},{\"gridX\":157,\"gridY\":96,\"type\":2},{\"gridX\":211,\"gridY\":96,\"type\":1},{\"gridX\":261,\"gridY\":96,\"type\":0},{\"gridX\":390,\"gridY\":96,\"type\":1},{\"gridX\":446,\"gridY\":96,\"type\":2},{\"gridX\":509,\"gridY\":96,\"type\":0},{\"gridX\":546,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":31,\"gridY\":50,\"type\":10},{\"gridX\":39,\"gridY\":50,\"type\":13},{\"gridX\":167,\"gridY\":50,\"type\":13},{\"gridX\":294,\"gridY\":50,\"type\":14},{\"gridX\":168,\"gridY\":90,\"type\":21},{\"gridX\":264,\"gridY\":90,\"type\":21},{\"gridX\":136,\"gridY\":90,\"type\":21},{\"gridX\":200,\"gridY\":90,\"type\":21},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":296,\"gridY\":90,\"type\":24},{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":386,\"gridY\":0,\"type\":25},{\"gridX\":394,\"gridY\":0,\"type\":27},{\"gridX\":458,\"gridY\":0,\"type\":28},{\"gridX\":586,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":232,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":76,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":450,\"gridY\":96,\"type\":12},{\"gridX\":101,\"gridY\":95,\"type\":35},{\"gridX\":113,\"gridY\":55,\"type\":26},{\"gridX\":174,\"gridY\":55,\"type\":26},{\"gridX\":307,\"gridY\":104,\"type\":34},{\"gridX\":330,\"gridY\":104,\"type\":34},{\"gridX\":236,\"gridY\":55,\"type\":41},{\"gridX\":351,\"gridY\":104,\"type\":34},{\"gridX\":371,\"gridY\":104,\"type\":34},{\"gridX\":115,\"gridY\":122,\"type\":34},{\"gridX\":115,\"gridY\":139,\"type\":34}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":414,\"gridY\":96}],\"enemies\":[{\"gridX\":163,\"gridY\":83,\"type\":1}],\"collectibles\":[{\"gridX\":25,\"gridY\":102,\"type\":0},{\"gridX\":53,\"gridY\":101,\"type\":0},{\"gridX\":81,\"gridY\":100,\"type\":0},{\"gridX\":80,\"gridY\":79,\"type\":1},{\"gridX\":114,\"gridY\":73,\"type\":0},{\"gridX\":123,\"gridY\":73,\"type\":0},{\"gridX\":174,\"gridY\":72,\"type\":0},{\"gridX\":184,\"gridY\":72,\"type\":0},{\"gridX\":253,\"gridY\":110,\"type\":1},{\"gridX\":388,\"gridY\":109,\"type\":1},{\"gridX\":150,\"gridY\":66,\"type\":0},{\"gridX\":202,\"gridY\":65,\"type\":0},{\"gridX\":216,\"gridY\":64,\"type\":0}],\"jons\":[{\"gridX\":0,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 12 ///

Chapter1Level12 * Chapter1Level12::getInstance()
{
    static Chapter1Level12 *instance = new Chapter1Level12("{\"world\":1,\"level\":12,\"midgrounds\":[{\"gridX\":255,\"gridY\":0,\"type\":5},{\"gridX\":374,\"gridY\":0,\"type\":3},{\"gridX\":396,\"gridY\":0,\"type\":4},{\"gridX\":72,\"gridY\":96,\"type\":2},{\"gridX\":0,\"gridY\":96,\"type\":1},{\"gridX\":52,\"gridY\":96,\"type\":0},{\"gridX\":129,\"gridY\":96,\"type\":2},{\"gridX\":167,\"gridY\":96,\"type\":1},{\"gridX\":159,\"gridY\":96,\"type\":0},{\"gridX\":275,\"gridY\":96,\"type\":2},{\"gridX\":243,\"gridY\":96,\"type\":0},{\"gridX\":404,\"gridY\":96,\"type\":1},{\"gridX\":391,\"gridY\":96,\"type\":0},{\"gridX\":315,\"gridY\":96,\"type\":1},{\"gridX\":348,\"gridY\":96,\"type\":2},{\"gridX\":148,\"gridY\":0,\"type\":4},{\"gridX\":789,\"gridY\":96,\"type\":2},{\"gridX\":709,\"gridY\":96,\"type\":1},{\"gridX\":749,\"gridY\":96,\"type\":1},{\"gridX\":869,\"gridY\":0,\"type\":3},{\"gridX\":894,\"gridY\":0,\"type\":3}],\"grounds\":[{\"gridX\":912,\"gridY\":0,\"type\":0},{\"gridX\":1048,\"gridY\":0,\"type\":2},{\"gridX\":920,\"gridY\":0,\"type\":3},{\"gridX\":1112,\"gridY\":0,\"type\":4},{\"gridX\":308,\"gridY\":20,\"type\":5},{\"gridX\":745,\"gridY\":20,\"type\":5},{\"gridX\":316,\"gridY\":20,\"type\":6},{\"gridX\":753,\"gridY\":20,\"type\":7},{\"gridX\":348,\"gridY\":20,\"type\":9},{\"gridX\":817,\"gridY\":20,\"type\":9},{\"gridX\":98,\"gridY\":50,\"type\":10},{\"gridX\":353,\"gridY\":50,\"type\":10},{\"gridX\":234,\"gridY\":50,\"type\":11},{\"gridX\":425,\"gridY\":50,\"type\":11},{\"gridX\":361,\"gridY\":50,\"type\":12},{\"gridX\":106,\"gridY\":50,\"type\":13},{\"gridX\":266,\"gridY\":50,\"type\":14},{\"gridX\":457,\"gridY\":50,\"type\":14},{\"gridX\":696,\"gridY\":90,\"type\":20},{\"gridX\":392,\"gridY\":90,\"type\":21},{\"gridX\":72,\"gridY\":90,\"type\":22},{\"gridX\":136,\"gridY\":90,\"type\":22},{\"gridX\":328,\"gridY\":90,\"type\":22},{\"gridX\":200,\"gridY\":90,\"type\":23},{\"gridX\":704,\"gridY\":90,\"type\":23},{\"gridX\":130,\"gridY\":90,\"type\":24},{\"gridX\":456,\"gridY\":90,\"type\":24},{\"gridX\":832,\"gridY\":90,\"type\":24},{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":27}],\"pits\":[],\"exitGrounds\":[{\"gridX\":424,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":112,\"gridY\":80,\"type\":3},{\"gridX\":751,\"gridY\":80,\"type\":3},{\"gridX\":766,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":100,\"gridY\":96,\"type\":8},{\"gridX\":132,\"gridY\":96,\"type\":8},{\"gridX\":1013,\"gridY\":7,\"type\":12},{\"gridX\":148,\"gridY\":55,\"type\":25},{\"gridX\":171,\"gridY\":84,\"type\":28},{\"gridX\":193,\"gridY\":55,\"type\":25},{\"gridX\":223,\"gridY\":84,\"type\":28},{\"gridX\":309,\"gridY\":27,\"type\":26},{\"gridX\":353,\"gridY\":83,\"type\":30},{\"gridX\":737,\"gridY\":185,\"type\":34},{\"gridX\":783,\"gridY\":185,\"type\":34},{\"gridX\":612,\"gridY\":158,\"type\":1},{\"gridX\":608,\"gridY\":158,\"type\":0},{\"gridX\":626,\"gridY\":158,\"type\":2},{\"gridX\":662,\"gridY\":190,\"type\":0},{\"gridX\":666,\"gridY\":190,\"type\":1},{\"gridX\":680,\"gridY\":190,\"type\":2},{\"gridX\":695,\"gridY\":185,\"type\":34},{\"gridX\":709,\"gridY\":185,\"type\":34},{\"gridX\":826,\"gridY\":223,\"type\":34},{\"gridX\":812,\"gridY\":210,\"type\":34},{\"gridX\":798,\"gridY\":197,\"type\":34},{\"gridX\":723,\"gridY\":185,\"type\":34},{\"gridX\":737,\"gridY\":170,\"type\":34},{\"gridX\":783,\"gridY\":170,\"type\":34},{\"gridX\":737,\"gridY\":155,\"type\":34},{\"gridX\":783,\"gridY\":155,\"type\":34},{\"gridX\":737,\"gridY\":140,\"type\":34},{\"gridX\":783,\"gridY\":140,\"type\":34},{\"gridX\":737,\"gridY\":125,\"type\":34},{\"gridX\":783,\"gridY\":125,\"type\":34},{\"gridX\":737,\"gridY\":110,\"type\":34},{\"gridX\":783,\"gridY\":110,\"type\":34},{\"gridX\":426,\"gridY\":55,\"type\":41},{\"gridX\":495,\"gridY\":103,\"type\":0},{\"gridX\":499,\"gridY\":103,\"type\":1},{\"gridX\":513,\"gridY\":103,\"type\":2},{\"gridX\":499,\"gridY\":108,\"type\":19},{\"gridX\":508,\"gridY\":108,\"type\":19},{\"gridX\":783,\"gridY\":96,\"type\":34},{\"gridX\":737,\"gridY\":96,\"type\":34},{\"gridX\":70,\"gridY\":96,\"type\":9}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":977,\"gridY\":7}],\"enemies\":[{\"gridX\":813,\"gridY\":27,\"type\":4},{\"gridX\":923,\"gridY\":7,\"type\":5},{\"gridX\":172,\"gridY\":96,\"type\":5},{\"gridX\":274,\"gridY\":96,\"type\":5},{\"gridX\":220,\"gridY\":96,\"type\":5},{\"gridX\":329,\"gridY\":96,\"type\":5},{\"gridX\":402,\"gridY\":96,\"type\":5}],\"collectibles\":[{\"gridX\":28,\"gridY\":98,\"type\":0},{\"gridX\":37,\"gridY\":98,\"type\":0},{\"gridX\":33,\"gridY\":105,\"type\":0},{\"gridX\":119,\"gridY\":150,\"type\":1},{\"gridX\":320,\"gridY\":37,\"type\":1},{\"gridX\":117,\"gridY\":75,\"type\":0},{\"gridX\":117,\"gridY\":65,\"type\":0},{\"gridX\":147,\"gridY\":69,\"type\":0},{\"gridX\":192,\"gridY\":68,\"type\":0},{\"gridX\":171,\"gridY\":65,\"type\":0},{\"gridX\":223,\"gridY\":65,\"type\":0},{\"gridX\":374,\"gridY\":64,\"type\":0},{\"gridX\":390,\"gridY\":64,\"type\":0},{\"gridX\":405,\"gridY\":63,\"type\":0},{\"gridX\":453,\"gridY\":114,\"type\":0},{\"gridX\":469,\"gridY\":123,\"type\":0},{\"gridX\":535,\"gridY\":166,\"type\":0},{\"gridX\":547,\"gridY\":166,\"type\":0},{\"gridX\":541,\"gridY\":161,\"type\":0},{\"gridX\":541,\"gridY\":173,\"type\":0},{\"gridX\":235,\"gridY\":65,\"type\":0},{\"gridX\":247,\"gridY\":65,\"type\":0},{\"gridX\":763,\"gridY\":187,\"type\":0},{\"gridX\":763,\"gridY\":167,\"type\":0},{\"gridX\":763,\"gridY\":151,\"type\":0},{\"gridX\":763,\"gridY\":133,\"type\":0},{\"gridX\":763,\"gridY\":119,\"type\":0},{\"gridX\":669,\"gridY\":203,\"type\":0},{\"gridX\":676,\"gridY\":203,\"type\":0},{\"gridX\":612,\"gridY\":173,\"type\":0},{\"gridX\":621,\"gridY\":173,\"type\":0},{\"gridX\":762,\"gridY\":213,\"type\":1},{\"gridX\":770,\"gridY\":34,\"type\":0},{\"gridX\":817,\"gridY\":42,\"type\":0},{\"gridX\":845,\"gridY\":58,\"type\":0},{\"gridX\":877,\"gridY\":54,\"type\":0},{\"gridX\":898,\"gridY\":49,\"type\":0},{\"gridX\":926,\"gridY\":41,\"type\":0},{\"gridX\":960,\"gridY\":17,\"type\":0},{\"gridX\":969,\"gridY\":17,\"type\":0}],\"jons\":[{\"gridX\":0,\"gridY\":124}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 13 ///

Chapter1Level13 * Chapter1Level13::getInstance()
{
    static Chapter1Level13 *instance = new Chapter1Level13("{\"world\":1,\"level\":13,\"midgrounds\":[{\"gridX\":57,\"gridY\":0,\"type\":4},{\"gridX\":116,\"gridY\":0,\"type\":3},{\"gridX\":17,\"gridY\":0,\"type\":5},{\"gridX\":111,\"gridY\":96,\"type\":2},{\"gridX\":184,\"gridY\":96,\"type\":1},{\"gridX\":249,\"gridY\":96,\"type\":0},{\"gridX\":314,\"gridY\":96,\"type\":1},{\"gridX\":314,\"gridY\":96,\"type\":1},{\"gridX\":385,\"gridY\":96,\"type\":1},{\"gridX\":499,\"gridY\":96,\"type\":2},{\"gridX\":644,\"gridY\":96,\"type\":1},{\"gridX\":698,\"gridY\":96,\"type\":0},{\"gridX\":548,\"gridY\":96,\"type\":1},{\"gridX\":464,\"gridY\":96,\"type\":0},{\"gridX\":356,\"gridY\":96,\"type\":0},{\"gridX\":610,\"gridY\":96,\"type\":2},{\"gridX\":884,\"gridY\":0,\"type\":4},{\"gridX\":939,\"gridY\":0,\"type\":4},{\"gridX\":829,\"gridY\":0,\"type\":5},{\"gridX\":864,\"gridY\":0,\"type\":3},{\"gridX\":919,\"gridY\":0,\"type\":5},{\"gridX\":858,\"gridY\":0,\"type\":4}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":0},{\"gridX\":231,\"gridY\":0,\"type\":0},{\"gridX\":493,\"gridY\":0,\"type\":0},{\"gridX\":239,\"gridY\":0,\"type\":1},{\"gridX\":501,\"gridY\":0,\"type\":1},{\"gridX\":533,\"gridY\":0,\"type\":1},{\"gridX\":565,\"gridY\":0,\"type\":1},{\"gridX\":8,\"gridY\":0,\"type\":2},{\"gridX\":72,\"gridY\":0,\"type\":4},{\"gridX\":271,\"gridY\":0,\"type\":4},{\"gridX\":597,\"gridY\":0,\"type\":4},{\"gridX\":145,\"gridY\":20,\"type\":5},{\"gridX\":650,\"gridY\":20,\"type\":5},{\"gridX\":658,\"gridY\":20,\"type\":6},{\"gridX\":722,\"gridY\":20,\"type\":6},{\"gridX\":754,\"gridY\":20,\"type\":6},{\"gridX\":153,\"gridY\":20,\"type\":6},{\"gridX\":786,\"gridY\":20,\"type\":7},{\"gridX\":850,\"gridY\":20,\"type\":8},{\"gridX\":978,\"gridY\":20,\"type\":8},{\"gridX\":185,\"gridY\":20,\"type\":9},{\"gridX\":1106,\"gridY\":20,\"type\":9},{\"gridX\":0,\"gridY\":48,\"type\":15},{\"gridX\":801,\"gridY\":48,\"type\":15},{\"gridX\":809,\"gridY\":48,\"type\":16},{\"gridX\":8,\"gridY\":48,\"type\":17},{\"gridX\":841,\"gridY\":48,\"type\":18},{\"gridX\":72,\"gridY\":48,\"type\":19},{\"gridX\":969,\"gridY\":48,\"type\":19},{\"gridX\":106,\"gridY\":90,\"type\":20},{\"gridX\":114,\"gridY\":90,\"type\":23},{\"gridX\":242,\"gridY\":90,\"type\":23},{\"gridX\":370,\"gridY\":90,\"type\":23},{\"gridX\":498,\"gridY\":90,\"type\":23},{\"gridX\":626,\"gridY\":90,\"type\":23},{\"gridX\":754,\"gridY\":90,\"type\":24}],\"pits\":[],\"exitGrounds\":[{\"gridX\":690,\"gridY\":0,\"type\":3}],\"holes\":[],\"foregroundObjects\":[{\"gridX\":842,\"gridY\":27,\"type\":12},{\"gridX\":187,\"gridY\":27,\"type\":25},{\"gridX\":242,\"gridY\":84,\"type\":30},{\"gridX\":238,\"gridY\":7,\"type\":21},{\"gridX\":232,\"gridY\":7,\"type\":25},{\"gridX\":591,\"gridY\":7,\"type\":20},{\"gridX\":612,\"gridY\":84,\"type\":30},{\"gridX\":581,\"gridY\":84,\"type\":30},{\"gridX\":727,\"gridY\":27,\"type\":27},{\"gridX\":759,\"gridY\":27,\"type\":27},{\"gridX\":620,\"gridY\":16,\"type\":34},{\"gridX\":620,\"gridY\":29,\"type\":34},{\"gridX\":229,\"gridY\":96,\"type\":9},{\"gridX\":135,\"gridY\":96,\"type\":8},{\"gridX\":183,\"gridY\":96,\"type\":17},{\"gridX\":244,\"gridY\":96,\"type\":11},{\"gridX\":437,\"gridY\":96,\"type\":11},{\"gridX\":645,\"gridY\":96,\"type\":10},{\"gridX\":726,\"gridY\":96,\"type\":8},{\"gridX\":726,\"gridY\":62,\"type\":34},{\"gridX\":770,\"gridY\":63,\"type\":34},{\"gridX\":540,\"gridY\":37,\"type\":4},{\"gridX\":536,\"gridY\":37,\"type\":3},{\"gridX\":554,\"gridY\":37,\"type\":5},{\"gridX\":540,\"gridY\":42,\"type\":26}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":797,\"gridY\":27}],\"enemies\":[{\"gridX\":211,\"gridY\":83,\"type\":1},{\"gridX\":204,\"gridY\":83,\"type\":1},{\"gridX\":218,\"gridY\":83,\"type\":1},{\"gridX\":309,\"gridY\":24,\"type\":3},{\"gridX\":412,\"gridY\":14,\"type\":3},{\"gridX\":144,\"gridY\":27,\"type\":2}],\"collectibles\":[{\"gridX\":27,\"gridY\":14,\"type\":0},{\"gridX\":44,\"gridY\":14,\"type\":0},{\"gridX\":64,\"gridY\":14,\"type\":0},{\"gridX\":186,\"gridY\":44,\"type\":0},{\"gridX\":467,\"gridY\":50,\"type\":1},{\"gridX\":649,\"gridY\":31,\"type\":1},{\"gridX\":129,\"gridY\":19,\"type\":1},{\"gridX\":483,\"gridY\":35,\"type\":0},{\"gridX\":492,\"gridY\":22,\"type\":0},{\"gridX\":499,\"gridY\":10,\"type\":0},{\"gridX\":313,\"gridY\":45,\"type\":0},{\"gridX\":413,\"gridY\":43,\"type\":0},{\"gridX\":623,\"gridY\":50,\"type\":0},{\"gridX\":543,\"gridY\":16,\"type\":0},{\"gridX\":561,\"gridY\":16,\"type\":0},{\"gridX\":527,\"gridY\":16,\"type\":0},{\"gridX\":737,\"gridY\":41,\"type\":0},{\"gridX\":755,\"gridY\":41,\"type\":0},{\"gridX\":773,\"gridY\":41,\"type\":0}],\"jons\":[{\"gridX\":0,\"gridY\":7}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 14 ///

Chapter1Level14 * Chapter1Level14::getInstance()
{
    static Chapter1Level14 *instance = new Chapter1Level14("{\"world\":1,\"level\":14,\"midgrounds\":[{\"gridX\":227,\"gridY\":0,\"type\":4},{\"gridX\":105,\"gridY\":0,\"type\":3},{\"gridX\":165,\"gridY\":0,\"type\":5},{\"gridX\":482,\"gridY\":0,\"type\":4},{\"gridX\":500,\"gridY\":0,\"type\":5},{\"gridX\":524,\"gridY\":0,\"type\":5},{\"gridX\":582,\"gridY\":0,\"type\":3},{\"gridX\":315,\"gridY\":0,\"type\":5},{\"gridX\":841,\"gridY\":0,\"type\":4},{\"gridX\":841,\"gridY\":0,\"type\":4},{\"gridX\":902,\"gridY\":0,\"type\":4},{\"gridX\":949,\"gridY\":0,\"type\":4},{\"gridX\":861,\"gridY\":0,\"type\":5}],\"grounds\":[{\"gridX\":163,\"gridY\":0,\"type\":0},{\"gridX\":435,\"gridY\":0,\"type\":0},{\"gridX\":824,\"gridY\":0,\"type\":0},{\"gridX\":427,\"gridY\":0,\"type\":0},{\"gridX\":235,\"gridY\":0,\"type\":1},{\"gridX\":832,\"gridY\":0,\"type\":1},{\"gridX\":171,\"gridY\":0,\"type\":2},{\"gridX\":267,\"gridY\":0,\"type\":2},{\"gridX\":443,\"gridY\":0,\"type\":3},{\"gridX\":864,\"gridY\":0,\"type\":3},{\"gridX\":331,\"gridY\":0,\"type\":4},{\"gridX\":435,\"gridY\":0,\"type\":4},{\"gridX\":571,\"gridY\":0,\"type\":4},{\"gridX\":559,\"gridY\":0,\"type\":4},{\"gridX\":992,\"gridY\":0,\"type\":4},{\"gridX\":0,\"gridY\":20,\"type\":5},{\"gridX\":8,\"gridY\":20,\"type\":6},{\"gridX\":103,\"gridY\":20,\"type\":9},{\"gridX\":298,\"gridY\":50,\"type\":10},{\"gridX\":338,\"gridY\":50,\"type\":14},{\"gridX\":160,\"gridY\":48,\"type\":15},{\"gridX\":468,\"gridY\":48,\"type\":15},{\"gridX\":835,\"gridY\":48,\"type\":15},{\"gridX\":475,\"gridY\":48,\"type\":16},{\"gridX\":507,\"gridY\":48,\"type\":16},{\"gridX\":168,\"gridY\":48,\"type\":17},{\"gridX\":843,\"gridY\":48,\"type\":18},{\"gridX\":232,\"gridY\":48,\"type\":19},{\"gridX\":539,\"gridY\":48,\"type\":19},{\"gridX\":971,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":392,\"gridY\":90,\"type\":23},{\"gridX\":520,\"gridY\":90,\"type\":23},{\"gridX\":648,\"gridY\":90,\"type\":24}],\"pits\":[],\"exitGrounds\":[{\"gridX\":39,\"gridY\":0,\"type\":3},{\"gridX\":71,\"gridY\":0,\"type\":3},{\"gridX\":306,\"gridY\":48,\"type\":2}],\"holes\":[],\"foregroundObjects\":[{\"gridX\":356,\"gridY\":50,\"type\":34},{\"gridX\":378,\"gridY\":50,\"type\":34},{\"gridX\":399,\"gridY\":50,\"type\":34},{\"gridX\":584,\"gridY\":69,\"type\":34},{\"gridX\":592,\"gridY\":5,\"type\":34},{\"gridX\":616,\"gridY\":5,\"type\":34},{\"gridX\":608,\"gridY\":68,\"type\":34},{\"gridX\":636,\"gridY\":62,\"type\":34},{\"gridX\":640,\"gridY\":5,\"type\":34},{\"gridX\":664,\"gridY\":60,\"type\":34},{\"gridX\":695,\"gridY\":59,\"type\":34},{\"gridX\":672,\"gridY\":5,\"type\":34},{\"gridX\":722,\"gridY\":57,\"type\":34},{\"gridX\":749,\"gridY\":55,\"type\":34},{\"gridX\":704,\"gridY\":5,\"type\":34},{\"gridX\":735,\"gridY\":4,\"type\":34},{\"gridX\":762,\"gridY\":4,\"type\":34},{\"gridX\":776,\"gridY\":52,\"type\":34},{\"gridX\":788,\"gridY\":3,\"type\":34},{\"gridX\":801,\"gridY\":48,\"type\":34},{\"gridX\":871,\"gridY\":7,\"type\":12},{\"gridX\":493,\"gridY\":44,\"type\":30},{\"gridX\":825,\"gridY\":7,\"type\":26}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":837,\"gridY\":7}],\"enemies\":[{\"gridX\":302,\"gridY\":7,\"type\":0},{\"gridX\":572,\"gridY\":7,\"type\":0},{\"gridX\":328,\"gridY\":8,\"type\":2},{\"gridX\":98,\"gridY\":27,\"type\":2},{\"gridX\":193,\"gridY\":7,\"type\":2},{\"gridX\":458,\"gridY\":7,\"type\":2}],\"collectibles\":[{\"gridX\":29,\"gridY\":32,\"type\":0},{\"gridX\":53,\"gridY\":32,\"type\":0},{\"gridX\":70,\"gridY\":32,\"type\":0},{\"gridX\":88,\"gridY\":32,\"type\":0},{\"gridX\":152,\"gridY\":51,\"type\":1},{\"gridX\":229,\"gridY\":30,\"type\":0},{\"gridX\":253,\"gridY\":27,\"type\":0},{\"gridX\":277,\"gridY\":23,\"type\":0},{\"gridX\":359,\"gridY\":75,\"type\":0},{\"gridX\":379,\"gridY\":75,\"type\":0},{\"gridX\":400,\"gridY\":75,\"type\":0},{\"gridX\":417,\"gridY\":52,\"type\":1},{\"gridX\":484,\"gridY\":40,\"type\":1},{\"gridX\":611,\"gridY\":44,\"type\":0},{\"gridX\":642,\"gridY\":40,\"type\":0},{\"gridX\":678,\"gridY\":38,\"type\":0},{\"gridX\":715,\"gridY\":36,\"type\":0},{\"gridX\":746,\"gridY\":35,\"type\":0},{\"gridX\":781,\"gridY\":32,\"type\":0},{\"gridX\":807,\"gridY\":26,\"type\":0}],\"jons\":[{\"gridX\":0,\"gridY\":37}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 15 ///

Chapter1Level15 * Chapter1Level15::getInstance()
{
    static Chapter1Level15 *instance = new Chapter1Level15("{\"world\":1,\"level\":15,\"midgrounds\":[{\"gridX\":815,\"gridY\":0,\"type\":3},{\"gridX\":846,\"gridY\":96,\"type\":2},{\"gridX\":935,\"gridY\":96,\"type\":1},{\"gridX\":1018,\"gridY\":96,\"type\":0},{\"gridX\":1142,\"gridY\":96,\"type\":2},{\"gridX\":1066,\"gridY\":96,\"type\":1},{\"gridX\":1224,\"gridY\":96,\"type\":1},{\"gridX\":1309,\"gridY\":96,\"type\":0},{\"gridX\":1395,\"gridY\":96,\"type\":2},{\"gridX\":1334,\"gridY\":96,\"type\":1},{\"gridX\":1206,\"gridY\":96,\"type\":0},{\"gridX\":1481,\"gridY\":96,\"type\":1},{\"gridX\":1538,\"gridY\":96,\"type\":0},{\"gridX\":1614,\"gridY\":96,\"type\":2},{\"gridX\":1733,\"gridY\":96,\"type\":1},{\"gridX\":1759,\"gridY\":96,\"type\":0},{\"gridX\":1717,\"gridY\":96,\"type\":2},{\"gridX\":1661,\"gridY\":96,\"type\":0},{\"gridX\":1691,\"gridY\":96,\"type\":1},{\"gridX\":1836,\"gridY\":96,\"type\":0},{\"gridX\":1800,\"gridY\":96,\"type\":2},{\"gridX\":1789,\"gridY\":0,\"type\":4},{\"gridX\":1542,\"gridY\":0,\"type\":4},{\"gridX\":1314,\"gridY\":0,\"type\":3},{\"gridX\":1382,\"gridY\":0,\"type\":3},{\"gridX\":1185,\"gridY\":0,\"type\":4},{\"gridX\":1143,\"gridY\":0,\"type\":5},{\"gridX\":954,\"gridY\":0,\"type\":4},{\"gridX\":791,\"gridY\":0,\"type\":5},{\"gridX\":742,\"gridY\":0,\"type\":4},{\"gridX\":849,\"gridY\":0,\"type\":5},{\"gridX\":559,\"gridY\":0,\"type\":4},{\"gridX\":502,\"gridY\":0,\"type\":4},{\"gridX\":632,\"gridY\":0,\"type\":4},{\"gridX\":326,\"gridY\":0,\"type\":5},{\"gridX\":183,\"gridY\":0,\"type\":4},{\"gridX\":13,\"gridY\":0,\"type\":5},{\"gridX\":114,\"gridY\":0,\"type\":4}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":0},{\"gridX\":828,\"gridY\":0,\"type\":0},{\"gridX\":951,\"gridY\":0,\"type\":0},{\"gridX\":1128,\"gridY\":0,\"type\":0},{\"gridX\":1419,\"gridY\":0,\"type\":0},{\"gridX\":648,\"gridY\":0,\"type\":1},{\"gridX\":835,\"gridY\":0,\"type\":2},{\"gridX\":8,\"gridY\":0,\"type\":3},{\"gridX\":136,\"gridY\":0,\"type\":3},{\"gridX\":264,\"gridY\":0,\"type\":3},{\"gridX\":392,\"gridY\":0,\"type\":3},{\"gridX\":520,\"gridY\":0,\"type\":3},{\"gridX\":1136,\"gridY\":0,\"type\":3},{\"gridX\":680,\"gridY\":0,\"type\":4},{\"gridX\":899,\"gridY\":0,\"type\":4},{\"gridX\":959,\"gridY\":0,\"type\":4},{\"gridX\":1264,\"gridY\":0,\"type\":4},{\"gridX\":1427,\"gridY\":0,\"type\":4},{\"gridX\":951,\"gridY\":20,\"type\":5},{\"gridX\":959,\"gridY\":20,\"type\":9},{\"gridX\":1536,\"gridY\":50,\"type\":10},{\"gridX\":950,\"gridY\":50,\"type\":10},{\"gridX\":1544,\"gridY\":50,\"type\":13},{\"gridX\":1672,\"gridY\":50,\"type\":13},{\"gridX\":1800,\"gridY\":50,\"type\":13},{\"gridX\":1928,\"gridY\":50,\"type\":14},{\"gridX\":958,\"gridY\":50,\"type\":14},{\"gridX\":728,\"gridY\":48,\"type\":15},{\"gridX\":1142,\"gridY\":48,\"type\":15},{\"gridX\":0,\"gridY\":48,\"type\":15},{\"gridX\":104,\"gridY\":48,\"type\":16},{\"gridX\":136,\"gridY\":48,\"type\":16},{\"gridX\":72,\"gridY\":48,\"type\":16},{\"gridX\":616,\"gridY\":48,\"type\":16},{\"gridX\":296,\"gridY\":48,\"type\":17},{\"gridX\":1150,\"gridY\":48,\"type\":17},{\"gridX\":8,\"gridY\":48,\"type\":17},{\"gridX\":360,\"gridY\":48,\"type\":18},{\"gridX\":736,\"gridY\":48,\"type\":18},{\"gridX\":488,\"gridY\":48,\"type\":18},{\"gridX\":168,\"gridY\":48,\"type\":18},{\"gridX\":648,\"gridY\":48,\"type\":19},{\"gridX\":864,\"gridY\":48,\"type\":19},{\"gridX\":1214,\"gridY\":48,\"type\":19},{\"gridX\":834,\"gridY\":90,\"type\":20},{\"gridX\":970,\"gridY\":90,\"type\":23},{\"gridX\":842,\"gridY\":90,\"type\":23},{\"gridX\":1098,\"gridY\":90,\"type\":23},{\"gridX\":1482,\"gridY\":90,\"type\":23},{\"gridX\":1610,\"gridY\":90,\"type\":23},{\"gridX\":1738,\"gridY\":90,\"type\":23},{\"gridX\":1226,\"gridY\":90,\"type\":23},{\"gridX\":1354,\"gridY\":90,\"type\":23},{\"gridX\":1866,\"gridY\":90,\"type\":24}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1701,\"gridY\":55,\"type\":12},{\"gridX\":49,\"gridY\":7,\"type\":26},{\"gridX\":103,\"gridY\":7,\"type\":27},{\"gridX\":135,\"gridY\":7,\"type\":27},{\"gridX\":238,\"gridY\":7,\"type\":27},{\"gridX\":270,\"gridY\":7,\"type\":27},{\"gridX\":302,\"gridY\":7,\"type\":27},{\"gridX\":393,\"gridY\":7,\"type\":27},{\"gridX\":425,\"gridY\":7,\"type\":27},{\"gridX\":457,\"gridY\":7,\"type\":27},{\"gridX\":516,\"gridY\":7,\"type\":27},{\"gridX\":548,\"gridY\":7,\"type\":27},{\"gridX\":580,\"gridY\":7,\"type\":27},{\"gridX\":894,\"gridY\":7,\"type\":41},{\"gridX\":942,\"gridY\":84,\"type\":30},{\"gridX\":1137,\"gridY\":58,\"type\":32},{\"gridX\":958,\"gridY\":55,\"type\":25},{\"gridX\":959,\"gridY\":27,\"type\":25},{\"gridX\":961,\"gridY\":7,\"type\":25},{\"gridX\":823,\"gridY\":3,\"type\":31},{\"gridX\":1183,\"gridY\":44,\"type\":30},{\"gridX\":1197,\"gridY\":7,\"type\":41},{\"gridX\":1422,\"gridY\":7,\"type\":41},{\"gridX\":1539,\"gridY\":55,\"type\":27},{\"gridX\":1571,\"gridY\":55,\"type\":27},{\"gridX\":1603,\"gridY\":55,\"type\":27},{\"gridX\":1635,\"gridY\":55,\"type\":27},{\"gridX\":1540,\"gridY\":84,\"type\":30},{\"gridX\":1572,\"gridY\":84,\"type\":30},{\"gridX\":1603,\"gridY\":84,\"type\":30},{\"gridX\":1634,\"gridY\":84,\"type\":30},{\"gridX\":1152,\"gridY\":44,\"type\":30},{\"gridX\":1163,\"gridY\":7,\"type\":41},{\"gridX\":1129,\"gridY\":7,\"type\":25},{\"gridX\":1135,\"gridY\":7,\"type\":25}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1666,\"gridY\":55}],\"enemies\":[{\"gridX\":681,\"gridY\":7,\"type\":2},{\"gridX\":952,\"gridY\":7,\"type\":2},{\"gridX\":952,\"gridY\":27,\"type\":2},{\"gridX\":951,\"gridY\":55,\"type\":2}],\"collectibles\":[{\"gridX\":52,\"gridY\":23,\"type\":0},{\"gridX\":127,\"gridY\":24,\"type\":0},{\"gridX\":201,\"gridY\":20,\"type\":0},{\"gridX\":284,\"gridY\":25,\"type\":0},{\"gridX\":361,\"gridY\":19,\"type\":0},{\"gridX\":439,\"gridY\":24,\"type\":0},{\"gridX\":498,\"gridY\":19,\"type\":0},{\"gridX\":564,\"gridY\":27,\"type\":0},{\"gridX\":977,\"gridY\":37,\"type\":1},{\"gridX\":1174,\"gridY\":16,\"type\":0},{\"gridX\":1198,\"gridY\":16,\"type\":0},{\"gridX\":1222,\"gridY\":16,\"type\":0},{\"gridX\":1138,\"gridY\":22,\"type\":1},{\"gridX\":1296,\"gridY\":23,\"type\":0},{\"gridX\":1332,\"gridY\":21,\"type\":0},{\"gridX\":1366,\"gridY\":19,\"type\":0},{\"gridX\":1399,\"gridY\":17,\"type\":0},{\"gridX\":1657,\"gridY\":69,\"type\":1},{\"gridX\":1540,\"gridY\":70,\"type\":0},{\"gridX\":1566,\"gridY\":69,\"type\":0},{\"gridX\":1592,\"gridY\":69,\"type\":0},{\"gridX\":1626,\"gridY\":68,\"type\":0},{\"gridX\":1443,\"gridY\":38,\"type\":0},{\"gridX\":1461,\"gridY\":57,\"type\":0},{\"gridX\":1494,\"gridY\":70,\"type\":0},{\"gridX\":728,\"gridY\":24,\"type\":0},{\"gridX\":762,\"gridY\":23,\"type\":0},{\"gridX\":798,\"gridY\":22,\"type\":0}],\"jons\":[{\"gridX\":2,\"gridY\":9}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 16 ///

Chapter1Level16 * Chapter1Level16::getInstance()
{
    static Chapter1Level16 *instance = new Chapter1Level16("{\"world\":1,\"level\":16,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":38,\"gridY\":96,\"type\":1},{\"gridX\":100,\"gridY\":96,\"type\":0},{\"gridX\":110,\"gridY\":96,\"type\":2},{\"gridX\":137,\"gridY\":96,\"type\":1},{\"gridX\":242,\"gridY\":96,\"type\":2},{\"gridX\":298,\"gridY\":96,\"type\":0},{\"gridX\":338,\"gridY\":96,\"type\":1},{\"gridX\":446,\"gridY\":96,\"type\":0},{\"gridX\":391,\"gridY\":96,\"type\":2},{\"gridX\":526,\"gridY\":96,\"type\":1},{\"gridX\":371,\"gridY\":96,\"type\":0},{\"gridX\":469,\"gridY\":96,\"type\":1},{\"gridX\":487,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":471,\"gridY\":0,\"type\":0},{\"gridX\":631,\"gridY\":0,\"type\":0},{\"gridX\":479,\"gridY\":0,\"type\":1},{\"gridX\":511,\"gridY\":0,\"type\":1},{\"gridX\":639,\"gridY\":0,\"type\":1},{\"gridX\":543,\"gridY\":0,\"type\":4},{\"gridX\":671,\"gridY\":0,\"type\":4},{\"gridX\":244,\"gridY\":20,\"type\":5},{\"gridX\":330,\"gridY\":20,\"type\":5},{\"gridX\":686,\"gridY\":20,\"type\":5},{\"gridX\":252,\"gridY\":20,\"type\":6},{\"gridX\":284,\"gridY\":20,\"type\":6},{\"gridX\":694,\"gridY\":20,\"type\":8},{\"gridX\":316,\"gridY\":20,\"type\":9},{\"gridX\":338,\"gridY\":20,\"type\":9},{\"gridX\":822,\"gridY\":20,\"type\":9},{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":162,\"gridY\":50,\"type\":10},{\"gridX\":228,\"gridY\":50,\"type\":10},{\"gridX\":433,\"gridY\":50,\"type\":10},{\"gridX\":170,\"gridY\":50,\"type\":11},{\"gridX\":441,\"gridY\":50,\"type\":11},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":236,\"gridY\":50,\"type\":14},{\"gridX\":473,\"gridY\":50,\"type\":14},{\"gridX\":202,\"gridY\":50,\"type\":14},{\"gridX\":136,\"gridY\":50,\"type\":14},{\"gridX\":210,\"gridY\":48,\"type\":15},{\"gridX\":515,\"gridY\":48,\"type\":15},{\"gridX\":144,\"gridY\":48,\"type\":15},{\"gridX\":523,\"gridY\":48,\"type\":16},{\"gridX\":152,\"gridY\":48,\"type\":19},{\"gridX\":218,\"gridY\":48,\"type\":19},{\"gridX\":555,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":488,\"gridY\":90,\"type\":21},{\"gridX\":8,\"gridY\":90,\"type\":22},{\"gridX\":264,\"gridY\":90,\"type\":22},{\"gridX\":520,\"gridY\":90,\"type\":22},{\"gridX\":200,\"gridY\":90,\"type\":22},{\"gridX\":72,\"gridY\":90,\"type\":23},{\"gridX\":360,\"gridY\":90,\"type\":23}],\"pits\":[],\"exitGrounds\":[{\"gridX\":328,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":488,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":164,\"gridY\":55,\"type\":27},{\"gridX\":194,\"gridY\":55,\"type\":26},{\"gridX\":247,\"gridY\":27,\"type\":27},{\"gridX\":229,\"gridY\":55,\"type\":26},{\"gridX\":331,\"gridY\":27,\"type\":20},{\"gridX\":280,\"gridY\":27,\"type\":27},{\"gridX\":310,\"gridY\":27,\"type\":25},{\"gridX\":316,\"gridY\":27,\"type\":25},{\"gridX\":375,\"gridY\":95,\"type\":35},{\"gridX\":399,\"gridY\":95,\"type\":35},{\"gridX\":422,\"gridY\":95,\"type\":35},{\"gridX\":471,\"gridY\":95,\"type\":35},{\"gridX\":445,\"gridY\":95,\"type\":35},{\"gridX\":513,\"gridY\":95,\"type\":35},{\"gridX\":454,\"gridY\":96,\"type\":23},{\"gridX\":428,\"gridY\":96,\"type\":23},{\"gridX\":406,\"gridY\":96,\"type\":23},{\"gridX\":383,\"gridY\":96,\"type\":23},{\"gridX\":482,\"gridY\":96,\"type\":22},{\"gridX\":504,\"gridY\":96,\"type\":22},{\"gridX\":541,\"gridY\":95,\"type\":35},{\"gridX\":523,\"gridY\":96,\"type\":23},{\"gridX\":551,\"gridY\":96,\"type\":23},{\"gridX\":565,\"gridY\":0,\"type\":34},{\"gridX\":564,\"gridY\":42,\"type\":34},{\"gridX\":600,\"gridY\":42,\"type\":34},{\"gridX\":602,\"gridY\":0,\"type\":34},{\"gridX\":566,\"gridY\":98,\"type\":34},{\"gridX\":566,\"gridY\":113,\"type\":34},{\"gridX\":566,\"gridY\":128,\"type\":34},{\"gridX\":566,\"gridY\":144,\"type\":34},{\"gridX\":731,\"gridY\":27,\"type\":12},{\"gridX\":681,\"gridY\":22,\"type\":31},{\"gridX\":652,\"gridY\":7,\"type\":41}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":697,\"gridY\":27}],\"enemies\":[{\"gridX\":52,\"gridY\":55,\"type\":2},{\"gridX\":102,\"gridY\":55,\"type\":2}],\"collectibles\":[{\"gridX\":529,\"gridY\":27,\"type\":1},{\"gridX\":52,\"gridY\":69,\"type\":0},{\"gridX\":102,\"gridY\":70,\"type\":0},{\"gridX\":184,\"gridY\":68,\"type\":0},{\"gridX\":245,\"gridY\":70,\"type\":0},{\"gridX\":258,\"gridY\":55,\"type\":0},{\"gridX\":272,\"gridY\":40,\"type\":0},{\"gridX\":300,\"gridY\":51,\"type\":0},{\"gridX\":321,\"gridY\":54,\"type\":0},{\"gridX\":343,\"gridY\":72,\"type\":0},{\"gridX\":338,\"gridY\":46,\"type\":0},{\"gridX\":373,\"gridY\":132,\"type\":0},{\"gridX\":420,\"gridY\":137,\"type\":0},{\"gridX\":453,\"gridY\":137,\"type\":0},{\"gridX\":399,\"gridY\":136,\"type\":0},{\"gridX\":492,\"gridY\":133,\"type\":0},{\"gridX\":577,\"gridY\":27,\"type\":0},{\"gridX\":602,\"gridY\":27,\"type\":0},{\"gridX\":823,\"gridY\":62,\"type\":1},{\"gridX\":643,\"gridY\":13,\"type\":0},{\"gridX\":680,\"gridY\":47,\"type\":0},{\"gridX\":666,\"gridY\":32,\"type\":0},{\"gridX\":492,\"gridY\":116,\"type\":0},{\"gridX\":492,\"gridY\":100,\"type\":0},{\"gridX\":493,\"gridY\":65,\"type\":0},{\"gridX\":494,\"gridY\":41,\"type\":0},{\"gridX\":494,\"gridY\":20,\"type\":0},{\"gridX\":695,\"gridY\":65,\"type\":0}],\"jons\":[{\"gridX\":0,\"gridY\":62}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 17 ///

Chapter1Level17 * Chapter1Level17::getInstance()
{
    static Chapter1Level17 *instance = new Chapter1Level17("{\"world\":1,\"level\":17,\"midgrounds\":[{\"gridX\":713,\"gridY\":0,\"type\":4},{\"gridX\":45,\"gridY\":0,\"type\":3},{\"gridX\":1371,\"gridY\":96,\"type\":2},{\"gridX\":1313,\"gridY\":96,\"type\":1},{\"gridX\":1280,\"gridY\":96,\"type\":0},{\"gridX\":1160,\"gridY\":96,\"type\":2},{\"gridX\":1204,\"gridY\":96,\"type\":0},{\"gridX\":1098,\"gridY\":96,\"type\":1},{\"gridX\":1051,\"gridY\":96,\"type\":2},{\"gridX\":1033,\"gridY\":96,\"type\":0},{\"gridX\":946,\"gridY\":96,\"type\":0},{\"gridX\":988,\"gridY\":96,\"type\":2},{\"gridX\":863,\"gridY\":96,\"type\":0},{\"gridX\":884,\"gridY\":96,\"type\":1},{\"gridX\":814,\"gridY\":96,\"type\":1},{\"gridX\":834,\"gridY\":96,\"type\":2},{\"gridX\":905,\"gridY\":96,\"type\":2},{\"gridX\":964,\"gridY\":96,\"type\":1},{\"gridX\":691,\"gridY\":96,\"type\":2},{\"gridX\":612,\"gridY\":96,\"type\":1},{\"gridX\":666,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":20,\"type\":5},{\"gridX\":8,\"gridY\":20,\"type\":7},{\"gridX\":72,\"gridY\":20,\"type\":9},{\"gridX\":1332,\"gridY\":50,\"type\":10},{\"gridX\":677,\"gridY\":50,\"type\":10},{\"gridX\":1532,\"gridY\":50,\"type\":11},{\"gridX\":1340,\"gridY\":50,\"type\":12},{\"gridX\":1404,\"gridY\":50,\"type\":13},{\"gridX\":1564,\"gridY\":50,\"type\":14},{\"gridX\":685,\"gridY\":50,\"type\":14},{\"gridX\":705,\"gridY\":48,\"type\":15},{\"gridX\":713,\"gridY\":48,\"type\":17},{\"gridX\":777,\"gridY\":48,\"type\":19},{\"gridX\":604,\"gridY\":90,\"type\":20},{\"gridX\":1572,\"gridY\":90,\"type\":22},{\"gridX\":612,\"gridY\":90,\"type\":22},{\"gridX\":1316,\"gridY\":90,\"type\":23},{\"gridX\":1444,\"gridY\":90,\"type\":23},{\"gridX\":708,\"gridY\":90,\"type\":23},{\"gridX\":1636,\"gridY\":90,\"type\":24},{\"gridX\":1284,\"gridY\":0,\"type\":26},{\"gridX\":1220,\"gridY\":0,\"type\":27},{\"gridX\":836,\"gridY\":0,\"type\":28},{\"gridX\":964,\"gridY\":0,\"type\":28},{\"gridX\":1092,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[{\"gridX\":676,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":1358,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":1398,\"gridY\":55,\"type\":12},{\"gridX\":382,\"gridY\":13,\"type\":34},{\"gridX\":497,\"gridY\":52,\"type\":34},{\"gridX\":700,\"gridY\":56,\"type\":32},{\"gridX\":735,\"gridY\":96,\"type\":7},{\"gridX\":664,\"gridY\":96,\"type\":10},{\"gridX\":831,\"gridY\":96,\"type\":15},{\"gridX\":826,\"gridY\":96,\"type\":13},{\"gridX\":853,\"gridY\":96,\"type\":15},{\"gridX\":875,\"gridY\":96,\"type\":15},{\"gridX\":897,\"gridY\":96,\"type\":15},{\"gridX\":919,\"gridY\":96,\"type\":16},{\"gridX\":1001,\"gridY\":95,\"type\":35},{\"gridX\":1029,\"gridY\":96,\"type\":17},{\"gridX\":1042,\"gridY\":96,\"type\":23},{\"gridX\":1009,\"gridY\":96,\"type\":23},{\"gridX\":1058,\"gridY\":96,\"type\":23},{\"gridX\":1434,\"gridY\":96,\"type\":24},{\"gridX\":1466,\"gridY\":96,\"type\":24},{\"gridX\":1498,\"gridY\":96,\"type\":24},{\"gridX\":1530,\"gridY\":96,\"type\":24},{\"gridX\":1562,\"gridY\":96,\"type\":24},{\"gridX\":1074,\"gridY\":96,\"type\":23},{\"gridX\":1090,\"gridY\":96,\"type\":22},{\"gridX\":1095,\"gridY\":96,\"type\":22},{\"gridX\":1139,\"gridY\":96,\"type\":6},{\"gridX\":1119,\"gridY\":96,\"type\":13},{\"gridX\":1124,\"gridY\":96,\"type\":14},{\"gridX\":1135,\"gridY\":96,\"type\":16},{\"gridX\":1240,\"gridY\":119,\"type\":1},{\"gridX\":1236,\"gridY\":119,\"type\":0},{\"gridX\":1254,\"gridY\":119,\"type\":2},{\"gridX\":1245,\"gridY\":96,\"type\":22},{\"gridX\":1024,\"gridY\":96,\"type\":17},{\"gridX\":677,\"gridY\":55,\"type\":19},{\"gridX\":1036,\"gridY\":96,\"type\":22},{\"gridX\":1418,\"gridY\":95,\"type\":35},{\"gridX\":1414,\"gridY\":125,\"type\":34},{\"gridX\":1414,\"gridY\":141,\"type\":34},{\"gridX\":1414,\"gridY\":159,\"type\":34},{\"gridX\":1263,\"gridY\":129,\"type\":34}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1360,\"gridY\":55}],\"enemies\":[{\"gridX\":234,\"gridY\":10,\"type\":3},{\"gridX\":339,\"gridY\":15,\"type\":3},{\"gridX\":441,\"gridY\":25,\"type\":3},{\"gridX\":538,\"gridY\":27,\"type\":3},{\"gridX\":596,\"gridY\":40,\"type\":3},{\"gridX\":705,\"gridY\":75,\"type\":2},{\"gridX\":664,\"gridY\":83,\"type\":1},{\"gridX\":986,\"gridY\":96,\"type\":4},{\"gridX\":1387,\"gridY\":96,\"type\":5},{\"gridX\":1109,\"gridY\":96,\"type\":0},{\"gridX\":1154,\"gridY\":128,\"type\":5},{\"gridX\":1299,\"gridY\":96,\"type\":2},{\"gridX\":1324,\"gridY\":96,\"type\":2}],\"collectibles\":[{\"gridX\":44,\"gridY\":32,\"type\":0},{\"gridX\":65,\"gridY\":32,\"type\":0},{\"gridX\":23,\"gridY\":32,\"type\":0},{\"gridX\":124,\"gridY\":41,\"type\":0},{\"gridX\":169,\"gridY\":40,\"type\":0},{\"gridX\":202,\"gridY\":37,\"type\":0},{\"gridX\":235,\"gridY\":33,\"type\":0},{\"gridX\":272,\"gridY\":39,\"type\":0},{\"gridX\":301,\"gridY\":39,\"type\":0},{\"gridX\":341,\"gridY\":38,\"type\":0},{\"gridX\":385,\"gridY\":46,\"type\":0},{\"gridX\":443,\"gridY\":50,\"type\":0},{\"gridX\":538,\"gridY\":47,\"type\":0},{\"gridX\":597,\"gridY\":62,\"type\":0},{\"gridX\":639,\"gridY\":64,\"type\":0},{\"gridX\":710,\"gridY\":116,\"type\":0},{\"gridX\":791,\"gridY\":128,\"type\":1},{\"gridX\":729,\"gridY\":127,\"type\":0},{\"gridX\":833,\"gridY\":125,\"type\":0},{\"gridX\":855,\"gridY\":120,\"type\":0},{\"gridX\":879,\"gridY\":117,\"type\":0},{\"gridX\":904,\"gridY\":113,\"type\":0},{\"gridX\":938,\"gridY\":97,\"type\":0},{\"gridX\":1025,\"gridY\":121,\"type\":0},{\"gridX\":1049,\"gridY\":118,\"type\":0},{\"gridX\":1118,\"gridY\":120,\"type\":0},{\"gridX\":1130,\"gridY\":141,\"type\":0},{\"gridX\":1144,\"gridY\":157,\"type\":0},{\"gridX\":1173,\"gridY\":166,\"type\":0},{\"gridX\":1190,\"gridY\":104,\"type\":1},{\"gridX\":1240,\"gridY\":130,\"type\":0},{\"gridX\":1249,\"gridY\":130,\"type\":0},{\"gridX\":1245,\"gridY\":137,\"type\":0},{\"gridX\":1363,\"gridY\":98,\"type\":0},{\"gridX\":1299,\"gridY\":113,\"type\":0},{\"gridX\":1324,\"gridY\":112,\"type\":0},{\"gridX\":1362,\"gridY\":81,\"type\":0},{\"gridX\":1362,\"gridY\":66,\"type\":0}],\"jons\":[{\"gridX\":4,\"gridY\":31}],\"extraForegroundObjects\":[{\"gridX\":781,\"gridY\":95,\"type\":39}],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 18 ///

Chapter1Level18 * Chapter1Level18::getInstance()
{
    static Chapter1Level18 *instance = new Chapter1Level18("{\"world\":1,\"level\":18,\"midgrounds\":[{\"gridX\":187,\"gridY\":0,\"type\":5},{\"gridX\":354,\"gridY\":0,\"type\":3},{\"gridX\":395,\"gridY\":0,\"type\":4},{\"gridX\":121,\"gridY\":96,\"type\":2},{\"gridX\":0,\"gridY\":96,\"type\":1},{\"gridX\":22,\"gridY\":96,\"type\":0},{\"gridX\":41,\"gridY\":96,\"type\":2},{\"gridX\":83,\"gridY\":96,\"type\":1},{\"gridX\":26,\"gridY\":0,\"type\":5},{\"gridX\":65,\"gridY\":0,\"type\":4},{\"gridX\":194,\"gridY\":96,\"type\":1},{\"gridX\":236,\"gridY\":96,\"type\":0},{\"gridX\":264,\"gridY\":96,\"type\":2},{\"gridX\":294,\"gridY\":96,\"type\":0},{\"gridX\":344,\"gridY\":96,\"type\":0},{\"gridX\":314,\"gridY\":96,\"type\":1},{\"gridX\":404,\"gridY\":96,\"type\":2},{\"gridX\":444,\"gridY\":96,\"type\":0},{\"gridX\":468,\"gridY\":96,\"type\":1},{\"gridX\":581,\"gridY\":96,\"type\":1},{\"gridX\":1221,\"gridY\":96,\"type\":2},{\"gridX\":1146,\"gridY\":96,\"type\":0},{\"gridX\":1150,\"gridY\":96,\"type\":1},{\"gridX\":1127,\"gridY\":96,\"type\":1},{\"gridX\":1237,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":103,\"gridY\":50,\"type\":10},{\"gridX\":8,\"gridY\":50,\"type\":12},{\"gridX\":111,\"gridY\":50,\"type\":13},{\"gridX\":1143,\"gridY\":50,\"type\":13},{\"gridX\":72,\"gridY\":50,\"type\":14},{\"gridX\":239,\"gridY\":50,\"type\":14},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":136,\"gridY\":90,\"type\":21},{\"gridX\":328,\"gridY\":90,\"type\":21},{\"gridX\":360,\"gridY\":90,\"type\":21},{\"gridX\":1143,\"gridY\":90,\"type\":21},{\"gridX\":424,\"gridY\":90,\"type\":22},{\"gridX\":488,\"gridY\":90,\"type\":22},{\"gridX\":552,\"gridY\":90,\"type\":22},{\"gridX\":1175,\"gridY\":90,\"type\":22},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":200,\"gridY\":90,\"type\":23},{\"gridX\":616,\"gridY\":90,\"type\":24},{\"gridX\":1135,\"gridY\":0,\"type\":25},{\"gridX\":1271,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":168,\"gridY\":88,\"type\":0},{\"gridX\":392,\"gridY\":88,\"type\":1},{\"gridX\":1239,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":339,\"gridY\":80,\"type\":3},{\"gridX\":1158,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":1580,\"gridY\":128,\"type\":0},{\"gridX\":1584,\"gridY\":128,\"type\":1},{\"gridX\":1598,\"gridY\":128,\"type\":1},{\"gridX\":1612,\"gridY\":128,\"type\":1},{\"gridX\":1626,\"gridY\":128,\"type\":1},{\"gridX\":1640,\"gridY\":128,\"type\":1},{\"gridX\":1654,\"gridY\":128,\"type\":1},{\"gridX\":1668,\"gridY\":128,\"type\":1},{\"gridX\":1682,\"gridY\":128,\"type\":1},{\"gridX\":1696,\"gridY\":128,\"type\":1},{\"gridX\":1710,\"gridY\":128,\"type\":1},{\"gridX\":1724,\"gridY\":128,\"type\":2},{\"gridX\":1626,\"gridY\":133,\"type\":12},{\"gridX\":171,\"gridY\":55,\"type\":41},{\"gridX\":140,\"gridY\":84,\"type\":30},{\"gridX\":222,\"gridY\":96,\"type\":24},{\"gridX\":272,\"gridY\":96,\"type\":13},{\"gridX\":277,\"gridY\":96,\"type\":15},{\"gridX\":299,\"gridY\":96,\"type\":15},{\"gridX\":321,\"gridY\":96,\"type\":14},{\"gridX\":332,\"gridY\":96,\"type\":16},{\"gridX\":439,\"gridY\":95,\"type\":35},{\"gridX\":373,\"gridY\":95,\"type\":35},{\"gridX\":339,\"gridY\":22,\"type\":4},{\"gridX\":353,\"gridY\":22,\"type\":5},{\"gridX\":335,\"gridY\":22,\"type\":3},{\"gridX\":391,\"gridY\":49,\"type\":4},{\"gridX\":387,\"gridY\":49,\"type\":3},{\"gridX\":405,\"gridY\":49,\"type\":5},{\"gridX\":393,\"gridY\":54,\"type\":19},{\"gridX\":207,\"gridY\":65,\"type\":34},{\"gridX\":531,\"gridY\":96,\"type\":7},{\"gridX\":600,\"gridY\":96,\"type\":20},{\"gridX\":1254,\"gridY\":55,\"type\":26},{\"gridX\":1268,\"gridY\":96,\"type\":20},{\"gridX\":1242,\"gridY\":55,\"type\":41}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1584,\"gridY\":133}],\"enemies\":[{\"gridX\":77,\"gridY\":83,\"type\":1},{\"gridX\":86,\"gridY\":83,\"type\":1},{\"gridX\":95,\"gridY\":83,\"type\":1},{\"gridX\":105,\"gridY\":55,\"type\":2},{\"gridX\":210,\"gridY\":110,\"type\":3},{\"gridX\":256,\"gridY\":96,\"type\":2},{\"gridX\":356,\"gridY\":96,\"type\":0},{\"gridX\":543,\"gridY\":117,\"type\":2},{\"gridX\":1196,\"gridY\":96,\"type\":4},{\"gridX\":1215,\"gridY\":111,\"type\":3}],\"collectibles\":[{\"gridX\":367,\"gridY\":46,\"type\":1},{\"gridX\":1198,\"gridY\":66,\"type\":1},{\"gridX\":39,\"gridY\":61,\"type\":0},{\"gridX\":60,\"gridY\":60,\"type\":0},{\"gridX\":133,\"gridY\":64,\"type\":0},{\"gridX\":146,\"gridY\":64,\"type\":0},{\"gridX\":159,\"gridY\":64,\"type\":0},{\"gridX\":190,\"gridY\":105,\"type\":0},{\"gridX\":276,\"gridY\":116,\"type\":0},{\"gridX\":293,\"gridY\":116,\"type\":0},{\"gridX\":308,\"gridY\":115,\"type\":0},{\"gridX\":468,\"gridY\":134,\"type\":0},{\"gridX\":497,\"gridY\":133,\"type\":0},{\"gridX\":530,\"gridY\":132,\"type\":0},{\"gridX\":717,\"gridY\":223,\"type\":1},{\"gridX\":608,\"gridY\":123,\"type\":0},{\"gridX\":618,\"gridY\":144,\"type\":0},{\"gridX\":636,\"gridY\":164,\"type\":0},{\"gridX\":650,\"gridY\":185,\"type\":0},{\"gridX\":669,\"gridY\":206,\"type\":0},{\"gridX\":848,\"gridY\":209,\"type\":0},{\"gridX\":854,\"gridY\":205,\"type\":0},{\"gridX\":848,\"gridY\":202,\"type\":0},{\"gridX\":891,\"gridY\":203,\"type\":0},{\"gridX\":842,\"gridY\":205,\"type\":0},{\"gridX\":753,\"gridY\":217,\"type\":0},{\"gridX\":813,\"gridY\":211,\"type\":0},{\"gridX\":781,\"gridY\":215,\"type\":0},{\"gridX\":891,\"gridY\":196,\"type\":0},{\"gridX\":885,\"gridY\":200,\"type\":0},{\"gridX\":897,\"gridY\":200,\"type\":0},{\"gridX\":934,\"gridY\":195,\"type\":0},{\"gridX\":928,\"gridY\":192,\"type\":0},{\"gridX\":934,\"gridY\":188,\"type\":0},{\"gridX\":940,\"gridY\":191,\"type\":0},{\"gridX\":967,\"gridY\":187,\"type\":0},{\"gridX\":973,\"gridY\":190,\"type\":0},{\"gridX\":973,\"gridY\":183,\"type\":0},{\"gridX\":979,\"gridY\":187,\"type\":0},{\"gridX\":1021,\"gridY\":179,\"type\":0},{\"gridX\":1015,\"gridY\":175,\"type\":0},{\"gridX\":1021,\"gridY\":172,\"type\":0},{\"gridX\":1027,\"gridY\":175,\"type\":0},{\"gridX\":1063,\"gridY\":171,\"type\":0},{\"gridX\":1057,\"gridY\":168,\"type\":0},{\"gridX\":1063,\"gridY\":164,\"type\":0},{\"gridX\":1069,\"gridY\":167,\"type\":0},{\"gridX\":1102,\"gridY\":155,\"type\":0},{\"gridX\":1096,\"gridY\":151,\"type\":0},{\"gridX\":1108,\"gridY\":151,\"type\":0},{\"gridX\":1102,\"gridY\":148,\"type\":0},{\"gridX\":1280,\"gridY\":118,\"type\":0},{\"gridX\":1293,\"gridY\":142,\"type\":0},{\"gridX\":1303,\"gridY\":164,\"type\":0},{\"gridX\":1315,\"gridY\":187,\"type\":0},{\"gridX\":1371,\"gridY\":203,\"type\":0},{\"gridX\":1446,\"gridY\":195,\"type\":0},{\"gridX\":1408,\"gridY\":200,\"type\":0},{\"gridX\":1476,\"gridY\":191,\"type\":0},{\"gridX\":1506,\"gridY\":185,\"type\":0},{\"gridX\":1534,\"gridY\":175,\"type\":0},{\"gridX\":1558,\"gridY\":167,\"type\":0}],\"jons\":[{\"gridX\":13,\"gridY\":54}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 19 ///

Chapter1Level19 * Chapter1Level19::getInstance()
{
    static Chapter1Level19 *instance = new Chapter1Level19("{\"world\":1,\"level\":19,\"midgrounds\":[{\"gridX\":806,\"gridY\":0,\"type\":5},{\"gridX\":768,\"gridY\":0,\"type\":5},{\"gridX\":680,\"gridY\":0,\"type\":3},{\"gridX\":637,\"gridY\":0,\"type\":3},{\"gridX\":138,\"gridY\":96,\"type\":2},{\"gridX\":187,\"gridY\":96,\"type\":1},{\"gridX\":166,\"gridY\":96,\"type\":0},{\"gridX\":238,\"gridY\":96,\"type\":1},{\"gridX\":215,\"gridY\":96,\"type\":0},{\"gridX\":307,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":711,\"gridY\":0,\"type\":0},{\"gridX\":758,\"gridY\":0,\"type\":0},{\"gridX\":766,\"gridY\":0,\"type\":2},{\"gridX\":719,\"gridY\":0,\"type\":4},{\"gridX\":830,\"gridY\":0,\"type\":4},{\"gridX\":775,\"gridY\":50,\"type\":10},{\"gridX\":767,\"gridY\":50,\"type\":10},{\"gridX\":815,\"gridY\":50,\"type\":14},{\"gridX\":128,\"gridY\":90,\"type\":20},{\"gridX\":307,\"gridY\":90,\"type\":20},{\"gridX\":315,\"gridY\":90,\"type\":21},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":24},{\"gridX\":347,\"gridY\":90,\"type\":24}],\"pits\":[],\"exitGrounds\":[{\"gridX\":783,\"gridY\":48,\"type\":2}],\"holes\":[],\"foregroundObjects\":[{\"gridX\":0,\"gridY\":127,\"type\":0},{\"gridX\":4,\"gridY\":127,\"type\":1},{\"gridX\":18,\"gridY\":127,\"type\":2},{\"gridX\":975,\"gridY\":126,\"type\":12},{\"gridX\":781,\"gridY\":7,\"type\":20},{\"gridX\":44,\"gridY\":151,\"type\":34},{\"gridX\":74,\"gridY\":122,\"type\":34},{\"gridX\":46,\"gridY\":99,\"type\":34},{\"gridX\":60,\"gridY\":109,\"type\":34},{\"gridX\":46,\"gridY\":82,\"type\":34},{\"gridX\":29,\"gridY\":85,\"type\":19},{\"gridX\":28,\"gridY\":80,\"type\":3},{\"gridX\":32,\"gridY\":80,\"type\":5},{\"gridX\":1,\"gridY\":157,\"type\":34},{\"gridX\":23,\"gridY\":157,\"type\":34},{\"gridX\":88,\"gridY\":136,\"type\":34},{\"gridX\":102,\"gridY\":149,\"type\":34},{\"gridX\":147,\"gridY\":95,\"type\":35},{\"gridX\":136,\"gridY\":95,\"type\":35},{\"gridX\":309,\"gridY\":96,\"type\":10},{\"gridX\":322,\"gridY\":96,\"type\":23},{\"gridX\":391,\"gridY\":105,\"type\":0},{\"gridX\":395,\"gridY\":105,\"type\":1},{\"gridX\":409,\"gridY\":105,\"type\":1},{\"gridX\":423,\"gridY\":105,\"type\":2},{\"gridX\":442,\"gridY\":146,\"type\":34},{\"gridX\":442,\"gridY\":103,\"type\":34},{\"gridX\":477,\"gridY\":141,\"type\":34},{\"gridX\":477,\"gridY\":101,\"type\":34},{\"gridX\":515,\"gridY\":137,\"type\":34},{\"gridX\":515,\"gridY\":97,\"type\":34},{\"gridX\":556,\"gridY\":130,\"type\":34},{\"gridX\":555,\"gridY\":91,\"type\":34},{\"gridX\":599,\"gridY\":123,\"type\":34},{\"gridX\":600,\"gridY\":85,\"type\":34},{\"gridX\":638,\"gridY\":116,\"type\":34},{\"gridX\":636,\"gridY\":77,\"type\":34},{\"gridX\":700,\"gridY\":98,\"type\":34},{\"gridX\":717,\"gridY\":81,\"type\":34},{\"gridX\":800,\"gridY\":89,\"type\":34},{\"gridX\":833,\"gridY\":83,\"type\":34},{\"gridX\":934,\"gridY\":121,\"type\":0},{\"gridX\":938,\"gridY\":121,\"type\":1},{\"gridX\":952,\"gridY\":121,\"type\":1},{\"gridX\":1008,\"gridY\":121,\"type\":1},{\"gridX\":994,\"gridY\":121,\"type\":1},{\"gridX\":980,\"gridY\":121,\"type\":1},{\"gridX\":1036,\"gridY\":121,\"type\":1},{\"gridX\":1022,\"gridY\":121,\"type\":1},{\"gridX\":966,\"gridY\":121,\"type\":1},{\"gridX\":1050,\"gridY\":121,\"type\":1},{\"gridX\":1064,\"gridY\":121,\"type\":2},{\"gridX\":341,\"gridY\":96,\"type\":10}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":939,\"gridY\":126}],\"enemies\":[{\"gridX\":166,\"gridY\":96,\"type\":0},{\"gridX\":179,\"gridY\":157,\"type\":3},{\"gridX\":208,\"gridY\":96,\"type\":2},{\"gridX\":232,\"gridY\":96,\"type\":2},{\"gridX\":258,\"gridY\":96,\"type\":2}],\"collectibles\":[{\"gridX\":29,\"gridY\":120,\"type\":0},{\"gridX\":29,\"gridY\":110,\"type\":0},{\"gridX\":29,\"gridY\":132,\"type\":0},{\"gridX\":193,\"gridY\":132,\"type\":0},{\"gridX\":669,\"gridY\":87,\"type\":0},{\"gridX\":685,\"gridY\":71,\"type\":0},{\"gridX\":693,\"gridY\":60,\"type\":0},{\"gridX\":677,\"gridY\":79,\"type\":0},{\"gridX\":661,\"gridY\":96,\"type\":0},{\"gridX\":702,\"gridY\":48,\"type\":0},{\"gridX\":55,\"gridY\":132,\"type\":0},{\"gridX\":69,\"gridY\":146,\"type\":0},{\"gridX\":82,\"gridY\":159,\"type\":0},{\"gridX\":446,\"gridY\":127,\"type\":0},{\"gridX\":483,\"gridY\":123,\"type\":0},{\"gridX\":519,\"gridY\":119,\"type\":0},{\"gridX\":557,\"gridY\":113,\"type\":0},{\"gridX\":602,\"gridY\":105,\"type\":0},{\"gridX\":637,\"gridY\":100,\"type\":0},{\"gridX\":131,\"gridY\":161,\"type\":0},{\"gridX\":232,\"gridY\":111,\"type\":0},{\"gridX\":259,\"gridY\":110,\"type\":0},{\"gridX\":328,\"gridY\":128,\"type\":0},{\"gridX\":396,\"gridY\":120,\"type\":0},{\"gridX\":206,\"gridY\":111,\"type\":0},{\"gridX\":414,\"gridY\":120,\"type\":0},{\"gridX\":789,\"gridY\":21,\"type\":0},{\"gridX\":797,\"gridY\":36,\"type\":0},{\"gridX\":809,\"gridY\":70,\"type\":0},{\"gridX\":820,\"gridY\":89,\"type\":0},{\"gridX\":833,\"gridY\":107,\"type\":0},{\"gridX\":848,\"gridY\":121,\"type\":0},{\"gridX\":861,\"gridY\":134,\"type\":0}],\"jons\":[{\"gridX\":0,\"gridY\":132}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 20 ///

Chapter1Level20 * Chapter1Level20::getInstance()
{
    static Chapter1Level20 *instance = new Chapter1Level20("{\"world\":1,\"level\":20,\"midgrounds\":[{\"gridX\":369,\"gridY\":0,\"type\":4},{\"gridX\":342,\"gridY\":0,\"type\":3},{\"gridX\":286,\"gridY\":96,\"type\":0},{\"gridX\":365,\"gridY\":96,\"type\":1},{\"gridX\":415,\"gridY\":96,\"type\":2},{\"gridX\":307,\"gridY\":96,\"type\":2},{\"gridX\":726,\"gridY\":96,\"type\":0},{\"gridX\":764,\"gridY\":96,\"type\":2},{\"gridX\":843,\"gridY\":96,\"type\":1},{\"gridX\":1122,\"gridY\":96,\"type\":0},{\"gridX\":1150,\"gridY\":96,\"type\":2},{\"gridX\":1348,\"gridY\":96,\"type\":2},{\"gridX\":1317,\"gridY\":96,\"type\":0},{\"gridX\":1249,\"gridY\":96,\"type\":1},{\"gridX\":1632,\"gridY\":96,\"type\":2},{\"gridX\":1743,\"gridY\":96,\"type\":1},{\"gridX\":1696,\"gridY\":96,\"type\":0},{\"gridX\":1715,\"gridY\":96,\"type\":2},{\"gridX\":1851,\"gridY\":96,\"type\":0},{\"gridX\":1770,\"gridY\":96,\"type\":2},{\"gridX\":1893,\"gridY\":96,\"type\":1},{\"gridX\":1813,\"gridY\":96,\"type\":1},{\"gridX\":1667,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":304,\"gridY\":0,\"type\":0},{\"gridX\":1366,\"gridY\":0,\"type\":1},{\"gridX\":312,\"gridY\":0,\"type\":4},{\"gridX\":338,\"gridY\":20,\"type\":5},{\"gridX\":346,\"gridY\":20,\"type\":9},{\"gridX\":365,\"gridY\":50,\"type\":10},{\"gridX\":373,\"gridY\":50,\"type\":14},{\"gridX\":300,\"gridY\":90,\"type\":22},{\"gridX\":396,\"gridY\":90,\"type\":22},{\"gridX\":65,\"gridY\":0,\"type\":25},{\"gridX\":292,\"gridY\":0,\"type\":25},{\"gridX\":711,\"gridY\":0,\"type\":25},{\"gridX\":841,\"gridY\":0,\"type\":25},{\"gridX\":910,\"gridY\":0,\"type\":25},{\"gridX\":1118,\"gridY\":0,\"type\":25},{\"gridX\":1254,\"gridY\":0,\"type\":25},{\"gridX\":1641,\"gridY\":0,\"type\":25},{\"gridX\":783,\"gridY\":0,\"type\":26},{\"gridX\":1126,\"gridY\":0,\"type\":26},{\"gridX\":1158,\"gridY\":0,\"type\":26},{\"gridX\":849,\"gridY\":0,\"type\":26},{\"gridX\":719,\"gridY\":0,\"type\":27},{\"gridX\":1649,\"gridY\":0,\"type\":28},{\"gridX\":1262,\"gridY\":0,\"type\":28},{\"gridX\":1777,\"gridY\":0,\"type\":28},{\"gridX\":1905,\"gridY\":0,\"type\":28},{\"gridX\":73,\"gridY\":0,\"type\":29},{\"gridX\":460,\"gridY\":0,\"type\":29},{\"gridX\":881,\"gridY\":0,\"type\":29},{\"gridX\":815,\"gridY\":0,\"type\":29},{\"gridX\":918,\"gridY\":0,\"type\":29},{\"gridX\":1190,\"gridY\":0,\"type\":29},{\"gridX\":1390,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":364,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":304,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":1742,\"gridY\":96,\"type\":12},{\"gridX\":0,\"gridY\":116,\"type\":0},{\"gridX\":4,\"gridY\":116,\"type\":1},{\"gridX\":18,\"gridY\":116,\"type\":1},{\"gridX\":32,\"gridY\":116,\"type\":1},{\"gridX\":46,\"gridY\":116,\"type\":2},{\"gridX\":0,\"gridY\":140,\"type\":34},{\"gridX\":35,\"gridY\":140,\"type\":34},{\"gridX\":368,\"gridY\":55,\"type\":41},{\"gridX\":348,\"gridY\":96,\"type\":11},{\"gridX\":411,\"gridY\":96,\"type\":23},{\"gridX\":814,\"gridY\":95,\"type\":35},{\"gridX\":811,\"gridY\":96,\"type\":41},{\"gridX\":714,\"gridY\":120,\"type\":34},{\"gridX\":741,\"gridY\":120,\"type\":34},{\"gridX\":769,\"gridY\":120,\"type\":34},{\"gridX\":870,\"gridY\":96,\"type\":23},{\"gridX\":844,\"gridY\":96,\"type\":23},{\"gridX\":859,\"gridY\":96,\"type\":22},{\"gridX\":864,\"gridY\":96,\"type\":22},{\"gridX\":990,\"gridY\":109,\"type\":1},{\"gridX\":972,\"gridY\":109,\"type\":0},{\"gridX\":976,\"gridY\":109,\"type\":1},{\"gridX\":1004,\"gridY\":109,\"type\":1},{\"gridX\":1046,\"gridY\":109,\"type\":2},{\"gridX\":1014,\"gridY\":134,\"type\":0},{\"gridX\":1032,\"gridY\":134,\"type\":1},{\"gridX\":1060,\"gridY\":134,\"type\":2},{\"gridX\":1018,\"gridY\":134,\"type\":1},{\"gridX\":1018,\"gridY\":109,\"type\":1},{\"gridX\":1032,\"gridY\":109,\"type\":1},{\"gridX\":1062,\"gridY\":140,\"type\":34},{\"gridX\":1080,\"gridY\":140,\"type\":34},{\"gridX\":1099,\"gridY\":140,\"type\":34},{\"gridX\":1117,\"gridY\":140,\"type\":34},{\"gridX\":1117,\"gridY\":126,\"type\":34},{\"gridX\":1117,\"gridY\":112,\"type\":34},{\"gridX\":1144,\"gridY\":140,\"type\":34},{\"gridX\":1144,\"gridY\":126,\"type\":34},{\"gridX\":1207,\"gridY\":115,\"type\":0},{\"gridX\":1211,\"gridY\":115,\"type\":1},{\"gridX\":1225,\"gridY\":115,\"type\":2},{\"gridX\":1274,\"gridY\":134,\"type\":1},{\"gridX\":1270,\"gridY\":134,\"type\":0},{\"gridX\":1288,\"gridY\":134,\"type\":2},{\"gridX\":1333,\"gridY\":128,\"type\":0},{\"gridX\":1337,\"gridY\":128,\"type\":1},{\"gridX\":1351,\"gridY\":128,\"type\":2},{\"gridX\":1315,\"gridY\":155,\"type\":0},{\"gridX\":1319,\"gridY\":155,\"type\":1},{\"gridX\":1333,\"gridY\":155,\"type\":2},{\"gridX\":1309,\"gridY\":96,\"type\":41},{\"gridX\":1285,\"gridY\":96,\"type\":10},{\"gridX\":1423,\"gridY\":128,\"type\":0},{\"gridX\":1427,\"gridY\":128,\"type\":1},{\"gridX\":1441,\"gridY\":128,\"type\":1},{\"gridX\":1455,\"gridY\":128,\"type\":2},{\"gridX\":1421,\"gridY\":88,\"type\":4},{\"gridX\":1417,\"gridY\":88,\"type\":3},{\"gridX\":1435,\"gridY\":88,\"type\":5},{\"gridX\":1488,\"gridY\":95,\"type\":0},{\"gridX\":1492,\"gridY\":95,\"type\":1},{\"gridX\":1506,\"gridY\":95,\"type\":2},{\"gridX\":1548,\"gridY\":112,\"type\":0},{\"gridX\":1552,\"gridY\":112,\"type\":1},{\"gridX\":1566,\"gridY\":112,\"type\":2},{\"gridX\":1591,\"gridY\":91,\"type\":0},{\"gridX\":1595,\"gridY\":91,\"type\":1},{\"gridX\":1609,\"gridY\":91,\"type\":2},{\"gridX\":1758,\"gridY\":127,\"type\":0},{\"gridX\":1762,\"gridY\":127,\"type\":1},{\"gridX\":1776,\"gridY\":127,\"type\":1},{\"gridX\":1806,\"gridY\":119,\"type\":1},{\"gridX\":1802,\"gridY\":119,\"type\":0},{\"gridX\":1820,\"gridY\":119,\"type\":2},{\"gridX\":1852,\"gridY\":128,\"type\":0},{\"gridX\":1856,\"gridY\":128,\"type\":1},{\"gridX\":1870,\"gridY\":128,\"type\":2},{\"gridX\":1873,\"gridY\":118,\"type\":0},{\"gridX\":1877,\"gridY\":118,\"type\":1},{\"gridX\":1891,\"gridY\":118,\"type\":1},{\"gridX\":1905,\"gridY\":118,\"type\":1},{\"gridX\":1790,\"gridY\":127,\"type\":2},{\"gridX\":1046,\"gridY\":134,\"type\":1},{\"gridX\":1144,\"gridY\":153,\"type\":34},{\"gridX\":1117,\"gridY\":153,\"type\":34},{\"gridX\":1919,\"gridY\":118,\"type\":2}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":1696,\"gridY\":96}],\"enemies\":[{\"gridX\":237,\"gridY\":87,\"type\":3},{\"gridX\":328,\"gridY\":96,\"type\":2},{\"gridX\":401,\"gridY\":96,\"type\":2},{\"gridX\":429,\"gridY\":96,\"type\":2},{\"gridX\":653,\"gridY\":81,\"type\":3},{\"gridX\":688,\"gridY\":88,\"type\":3},{\"gridX\":1039,\"gridY\":114,\"type\":5},{\"gridX\":1164,\"gridY\":96,\"type\":2},{\"gridX\":1187,\"gridY\":96,\"type\":2},{\"gridX\":1176,\"gridY\":96,\"type\":0},{\"gridX\":1208,\"gridY\":120,\"type\":2},{\"gridX\":1222,\"gridY\":120,\"type\":2},{\"gridX\":1254,\"gridY\":96,\"type\":2},{\"gridX\":1272,\"gridY\":96,\"type\":2},{\"gridX\":1298,\"gridY\":96,\"type\":2},{\"gridX\":1325,\"gridY\":96,\"type\":2},{\"gridX\":1273,\"gridY\":139,\"type\":2},{\"gridX\":1389,\"gridY\":96,\"type\":2},{\"gridX\":1371,\"gridY\":96,\"type\":2},{\"gridX\":1348,\"gridY\":133,\"type\":2},{\"gridX\":1324,\"gridY\":160,\"type\":2},{\"gridX\":1348,\"gridY\":96,\"type\":2},{\"gridX\":1427,\"gridY\":133,\"type\":2},{\"gridX\":1445,\"gridY\":133,\"type\":2},{\"gridX\":1416,\"gridY\":93,\"type\":2},{\"gridX\":1430,\"gridY\":93,\"type\":2},{\"gridX\":1591,\"gridY\":96,\"type\":2},{\"gridX\":1606,\"gridY\":96,\"type\":2},{\"gridX\":1556,\"gridY\":117,\"type\":2},{\"gridX\":1488,\"gridY\":100,\"type\":2},{\"gridX\":1501,\"gridY\":100,\"type\":2},{\"gridX\":1805,\"gridY\":124,\"type\":2},{\"gridX\":1813,\"gridY\":124,\"type\":2},{\"gridX\":1877,\"gridY\":123,\"type\":2},{\"gridX\":1887,\"gridY\":123,\"type\":2},{\"gridX\":1900,\"gridY\":123,\"type\":2},{\"gridX\":1911,\"gridY\":123,\"type\":2},{\"gridX\":1852,\"gridY\":133,\"type\":2},{\"gridX\":1863,\"gridY\":133,\"type\":2},{\"gridX\":1760,\"gridY\":132,\"type\":2},{\"gridX\":1771,\"gridY\":132,\"type\":2},{\"gridX\":1781,\"gridY\":132,\"type\":2},{\"gridX\":38,\"gridY\":121,\"type\":4}],\"collectibles\":[{\"gridX\":343,\"gridY\":37,\"type\":1},{\"gridX\":862,\"gridY\":102,\"type\":1},{\"gridX\":1636,\"gridY\":93,\"type\":1},{\"gridX\":21,\"gridY\":142,\"type\":0},{\"gridX\":39,\"gridY\":161,\"type\":0},{\"gridX\":71,\"gridY\":124,\"type\":0},{\"gridX\":71,\"gridY\":116,\"type\":0},{\"gridX\":71,\"gridY\":108,\"type\":0},{\"gridX\":115,\"gridY\":107,\"type\":0},{\"gridX\":144,\"gridY\":103,\"type\":0},{\"gridX\":170,\"gridY\":102,\"type\":0},{\"gridX\":197,\"gridY\":100,\"type\":0},{\"gridX\":225,\"gridY\":97,\"type\":0},{\"gridX\":265,\"gridY\":102,\"type\":0},{\"gridX\":513,\"gridY\":115,\"type\":0},{\"gridX\":531,\"gridY\":113,\"type\":0},{\"gridX\":550,\"gridY\":112,\"type\":0},{\"gridX\":573,\"gridY\":111,\"type\":0},{\"gridX\":599,\"gridY\":109,\"type\":0},{\"gridX\":622,\"gridY\":106,\"type\":0},{\"gridX\":806,\"gridY\":126,\"type\":0},{\"gridX\":915,\"gridY\":104,\"type\":0},{\"gridX\":974,\"gridY\":122,\"type\":0},{\"gridX\":1047,\"gridY\":147,\"type\":0},{\"gridX\":1134,\"gridY\":174,\"type\":0},{\"gridX\":1207,\"gridY\":134,\"type\":0},{\"gridX\":1667,\"gridY\":103,\"type\":0},{\"gridX\":1676,\"gridY\":103,\"type\":0},{\"gridX\":1686,\"gridY\":102,\"type\":0},{\"gridX\":1134,\"gridY\":158,\"type\":0},{\"gridX\":1134,\"gridY\":143,\"type\":0},{\"gridX\":1134,\"gridY\":128,\"type\":0}],\"jons\":[{\"gridX\":0,\"gridY\":121}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}
