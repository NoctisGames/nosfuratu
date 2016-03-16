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

/// Level ///

Level * Level::getInstance()
{
    static Level *instance = new Level();
    
    return instance;
}

void Level::enter(GameScreen* gs)
{
    m_fStateTime = 0;
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
        
        m_game->setCameraBounds(&gs->m_renderer->getCameraBounds());
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
    Jon& jon = m_game->getJon();
    
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderWorld(*m_game);
        
        if (m_isReleasingShockwave)
        {
            Jon& jon = m_game->getJon();
            gs->m_renderer->renderToSecondFramebufferWithShockwave(m_fShockwaveCenterX, m_fShockwaveCenterY, m_fShockwaveElapsedTime, jon.isTransformingIntoVampire() || jon.isVampire());
        }
        else
        {
            gs->m_renderer->renderToSecondFramebuffer(*m_game);
        }
        
        gs->m_renderer->renderJonAndExtraForegroundObjects(*m_game);
        
        if (m_hasOpeningSequenceCompleted)
        {
            gs->m_renderer->renderHud(*m_game, m_backButton.get(), gs->m_iFPS);
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
    else
    {
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->processTouchEvents();
            return;
        }
        
        Jon& jon = m_game->getJon();
        jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
        
        if (!m_hasShownOpeningSequence)
        {
            gs->m_renderer->beginOpeningPanningSequence(*m_game);
            
            m_hasShownOpeningSequence = true;
            
            Assets::getInstance()->setMusicId(MUSIC_PLAY_INTRO_WORLD_1);
        }
        else if (!m_hasOpeningSequenceCompleted)
        {
            if (gs->m_stateMachine->getPreviousState() == GameScreenLevelEditor::getInstance() && handleOpeningSequenceTouchInput(gs))
            {
                m_hasOpeningSequenceCompleted = true;
                
                jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
                
                Assets::getInstance()->setMusicId(MUSIC_PLAY_WORLD_1_LOOP);
                
                return;
            }
            else
            {
                gs->processTouchEvents();
            }
            
            jon.update(gs->m_fDeltaTime);
            
            int result = gs->m_renderer->updateCameraToFollowPathToJon(*m_game);
            m_hasOpeningSequenceCompleted = result == 3;
            m_activateRadialBlur = result == 1;
            jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
            
            if (m_hasOpeningSequenceCompleted)
            {
                Assets::getInstance()->setMusicId(MUSIC_PLAY_WORLD_1_LOOP);
            }
            else if (result == 2)
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
                Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
                
                // Starting death transition, when screen goes black, new game begins
                
                m_fStateTime += gs->m_fDeltaTime * 2;
                
                if (m_fStateTime > 1.6f)
                {
                    m_game->reset();
                    enter(gs);
                    m_showDeathTransOut = true;
                }
                
                return;
            }
            else if (jon.getPosition().getX() - jon.getWidth() > m_game->getFarRight())
            {
                // Has Cleared the Level
                
                Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
                Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPARROW_FLY);
                Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SAW_GRIND);
                
                m_fStateTime += gs->m_fDeltaTime;
                
                if (m_fStateTime > 5)
                {
                    m_game->reset();
                    enter(gs);
                    
                    gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_COMPLETED * 1000;
                    gs->m_iRequestedAction += m_game->getWorld() * 100;
                    gs->m_iRequestedAction += m_game->getLevel();
                    
                    return;
                }
            }
            else
            {
                // Is Still Actively playing the Level
                
                m_game->update(gs->m_fDeltaTime);
                
                if (jon.isTransformingIntoVampire() || jon.isRevertingToRabbit())
                {
                    if (jon.getTransformStateTime() < 0.125f)
                    {
                        gs->m_fDeltaTime /= 8;
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
                    m_fShockwaveElapsedTime += gs->m_fDeltaTime;
                    
                    if (m_fShockwaveElapsedTime > 4)
                    {
                        m_fShockwaveElapsedTime = 0;
                        m_isReleasingShockwave = false;
                    }
                }
                
                m_game->updateAndClean(gs->m_fDeltaTime);
                
                EntityUtils::handleCollections(jon, m_game->getCollectibleItems(), gs->m_fDeltaTime);
                
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
            
            gs->m_renderer->updateCameraToFollowJon(*m_game, gs->m_fDeltaTime);
            
            EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
            EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
            EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
            EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
        }
    }
}

void Level::exit(GameScreen* gs)
{
    m_game->reset();
    
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_JON_VAMPIRE_GLIDE);
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SPARROW_FLY);
    Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SAW_GRIND);
    
    m_fStateTime = 0;
    m_isReleasingShockwave = false;
    m_fShockwaveElapsedTime = 0;
    m_fShockwaveCenterX = 0;
    m_fShockwaveCenterY = 0;
    m_hasShownOpeningSequence = false;
    m_hasOpeningSequenceCompleted = false;
    m_hasSwiped = false;
    m_showDeathTransOut = false;
}

void Level::setSourceGame(Game* game)
{
    m_sourceGame = game;
}

Game& Level::getGame()
{
    return *m_game;
}

BackButton& Level::getBackButton()
{
    return *m_backButton;
}

bool Level::handleOpeningSequenceTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        switch (i->getTouchType())
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
    gs->processTouchEvents();
    
    Jon& jon = m_game->getJon();
    bool isJonAlive = jon.isAlive();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        switch (i->getTouchType())
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
                if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getMainBounds()))
                {
                    Assets::getInstance()->setMusicId(MUSIC_STOP);
                    
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

Level::Level(const char* json) : m_sourceGame(nullptr), m_fStateTime(0.0f), m_isReleasingShockwave(false), m_fShockwaveElapsedTime(0.0f), m_fShockwaveCenterX(0.0f), m_fShockwaveCenterY(0.0f), m_hasShownOpeningSequence(false), m_hasOpeningSequenceCompleted(false), m_activateRadialBlur(false), m_hasSwiped(false), m_showDeathTransOut(false)
{
    m_json = json;
    m_game = std::unique_ptr<Game>(new Game());
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
}

/// Chapter 1 Level 1 ///

Chapter1Level1 * Chapter1Level1::getInstance()
{
    static Chapter1Level1 *instance = new Chapter1Level1("{\"world\":1,\"level\":1,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":0},{\"gridX\":48,\"gridY\":96,\"type\":1},{\"gridX\":96,\"gridY\":96,\"type\":2},{\"gridX\":160,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":0},{\"gridX\":276,\"gridY\":96,\"type\":2},{\"gridX\":340,\"gridY\":96,\"type\":1},{\"gridX\":388,\"gridY\":96,\"type\":2},{\"gridX\":460,\"gridY\":96,\"type\":0},{\"gridX\":504,\"gridY\":96,\"type\":0},{\"gridX\":612,\"gridY\":96,\"type\":2},{\"gridX\":680,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":200,\"gridY\":0,\"type\":26},{\"gridX\":136,\"gridY\":0,\"type\":27},{\"gridX\":232,\"gridY\":0,\"type\":27},{\"gridX\":552,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":296,\"gridY\":0,\"type\":28},{\"gridX\":424,\"gridY\":0,\"type\":28},{\"gridX\":616,\"gridY\":0,\"type\":28},{\"gridX\":744,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":240,\"gridY\":0,\"type\":30},{\"gridX\":580,\"gridY\":0,\"type\":30}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":729,\"gridY\":96,\"type\":12},{\"gridX\":392,\"gridY\":96,\"type\":11},{\"gridX\":470,\"gridY\":96,\"type\":18},{\"gridX\":433,\"gridY\":96,\"type\":17},{\"gridX\":698,\"gridY\":96,\"type\":17}],\"enemies\":[],\"collectibles\":[{\"gridX\":52,\"gridY\":104,\"type\":0},{\"gridX\":60,\"gridY\":104,\"type\":0},{\"gridX\":68,\"gridY\":104,\"type\":0},{\"gridX\":76,\"gridY\":104,\"type\":0},{\"gridX\":84,\"gridY\":104,\"type\":0},{\"gridX\":92,\"gridY\":104,\"type\":0},{\"gridX\":100,\"gridY\":104,\"type\":0},{\"gridX\":108,\"gridY\":108,\"type\":0},{\"gridX\":116,\"gridY\":112,\"type\":0},{\"gridX\":124,\"gridY\":116,\"type\":0},{\"gridX\":132,\"gridY\":120,\"type\":0},{\"gridX\":148,\"gridY\":120,\"type\":0},{\"gridX\":156,\"gridY\":116,\"type\":0},{\"gridX\":164,\"gridY\":112,\"type\":0},{\"gridX\":172,\"gridY\":108,\"type\":0},{\"gridX\":196,\"gridY\":104,\"type\":0},{\"gridX\":180,\"gridY\":104,\"type\":0},{\"gridX\":188,\"gridY\":104,\"type\":0},{\"gridX\":140,\"gridY\":124,\"type\":0},{\"gridX\":256,\"gridY\":88,\"type\":1},{\"gridX\":304,\"gridY\":104,\"type\":0},{\"gridX\":316,\"gridY\":104,\"type\":0},{\"gridX\":328,\"gridY\":104,\"type\":0},{\"gridX\":392,\"gridY\":116,\"type\":0},{\"gridX\":444,\"gridY\":144,\"type\":1},{\"gridX\":340,\"gridY\":104,\"type\":0},{\"gridX\":596,\"gridY\":120,\"type\":1},{\"gridX\":512,\"gridY\":104,\"type\":0},{\"gridX\":520,\"gridY\":104,\"type\":0},{\"gridX\":528,\"gridY\":104,\"type\":0},{\"gridX\":620,\"gridY\":104,\"type\":0},{\"gridX\":628,\"gridY\":104,\"type\":0},{\"gridX\":636,\"gridY\":104,\"type\":0},{\"gridX\":620,\"gridY\":112,\"type\":0},{\"gridX\":668,\"gridY\":104,\"type\":0},{\"gridX\":676,\"gridY\":104,\"type\":0},{\"gridX\":684,\"gridY\":104,\"type\":0},{\"gridX\":692,\"gridY\":112,\"type\":0},{\"gridX\":612,\"gridY\":112,\"type\":0},{\"gridX\":684,\"gridY\":112,\"type\":0},{\"gridX\":652,\"gridY\":104,\"type\":0},{\"gridX\":660,\"gridY\":104,\"type\":0},{\"gridX\":644,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":16,\"gridY\":108}]}");
    
    return instance;
}

/// Chapter 1 Level 2 ///

Chapter1Level2 * Chapter1Level2::getInstance()
{
    static Chapter1Level2 *instance = new Chapter1Level2("{\"world\":1,\"level\":2,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":1},{\"gridX\":50,\"gridY\":96,\"type\":2},{\"gridX\":108,\"gridY\":96,\"type\":0},{\"gridX\":172,\"gridY\":96,\"type\":2},{\"gridX\":244,\"gridY\":96,\"type\":2},{\"gridX\":488,\"gridY\":96,\"type\":1},{\"gridX\":532,\"gridY\":96,\"type\":1},{\"gridX\":572,\"gridY\":96,\"type\":2},{\"gridX\":356,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":480,\"gridY\":0,\"type\":25},{\"gridX\":328,\"gridY\":0,\"type\":26},{\"gridX\":360,\"gridY\":0,\"type\":27},{\"gridX\":136,\"gridY\":0,\"type\":27},{\"gridX\":200,\"gridY\":0,\"type\":27},{\"gridX\":264,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":488,\"gridY\":0,\"type\":28},{\"gridX\":424,\"gridY\":0,\"type\":29},{\"gridX\":616,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":308,\"gridY\":0,\"type\":30},{\"gridX\":136,\"gridY\":0,\"type\":30}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":72,\"gridY\":112,\"type\":0},{\"gridX\":131,\"gridY\":96,\"type\":19},{\"gridX\":113,\"gridY\":96,\"type\":17},{\"gridX\":76,\"gridY\":112,\"type\":1},{\"gridX\":90,\"gridY\":112,\"type\":2},{\"gridX\":601,\"gridY\":96,\"type\":12},{\"gridX\":382,\"gridY\":96,\"type\":23},{\"gridX\":371,\"gridY\":96,\"type\":19},{\"gridX\":249,\"gridY\":96,\"type\":23}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":44,\"gridY\":104,\"type\":0},{\"gridX\":52,\"gridY\":104,\"type\":0},{\"gridX\":52,\"gridY\":112,\"type\":0},{\"gridX\":60,\"gridY\":112,\"type\":0},{\"gridX\":72,\"gridY\":120,\"type\":0},{\"gridX\":80,\"gridY\":120,\"type\":0},{\"gridX\":88,\"gridY\":120,\"type\":0},{\"gridX\":157,\"gridY\":168,\"type\":1},{\"gridX\":255,\"gridY\":109,\"type\":1},{\"gridX\":388,\"gridY\":152,\"type\":1},{\"gridX\":372,\"gridY\":124,\"type\":0},{\"gridX\":372,\"gridY\":112,\"type\":0},{\"gridX\":372,\"gridY\":104,\"type\":0},{\"gridX\":468,\"gridY\":120,\"type\":0},{\"gridX\":476,\"gridY\":112,\"type\":0},{\"gridX\":484,\"gridY\":104,\"type\":0},{\"gridX\":492,\"gridY\":104,\"type\":0},{\"gridX\":500,\"gridY\":104,\"type\":0},{\"gridX\":508,\"gridY\":104,\"type\":0},{\"gridX\":536,\"gridY\":104,\"type\":0},{\"gridX\":528,\"gridY\":104,\"type\":0},{\"gridX\":544,\"gridY\":120,\"type\":0},{\"gridX\":536,\"gridY\":112,\"type\":0},{\"gridX\":544,\"gridY\":112,\"type\":0},{\"gridX\":552,\"gridY\":120,\"type\":0},{\"gridX\":584,\"gridY\":112,\"type\":0},{\"gridX\":592,\"gridY\":104,\"type\":0},{\"gridX\":262,\"gridY\":113,\"type\":0},{\"gridX\":250,\"gridY\":114,\"type\":0}],\"jons\":[{\"gridX\":16,\"gridY\":104}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 3 ///

Chapter1Level3 * Chapter1Level3::getInstance()
{
    static Chapter1Level3 *instance = new Chapter1Level3("{\"world\":1,\"level\":3,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":68,\"gridY\":96,\"type\":2},{\"gridX\":132,\"gridY\":96,\"type\":0},{\"gridX\":168,\"gridY\":96,\"type\":0},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":488,\"gridY\":96,\"type\":2},{\"gridX\":544,\"gridY\":96,\"type\":0},{\"gridX\":724,\"gridY\":96,\"type\":0},{\"gridX\":892,\"gridY\":96,\"type\":0},{\"gridX\":816,\"gridY\":96,\"type\":1},{\"gridX\":960,\"gridY\":96,\"type\":1},{\"gridX\":1020,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":744,\"gridY\":0,\"type\":26},{\"gridX\":584,\"gridY\":0,\"type\":26},{\"gridX\":776,\"gridY\":0,\"type\":26},{\"gridX\":520,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":808,\"gridY\":0,\"type\":28},{\"gridX\":936,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":616,\"gridY\":0,\"type\":28},{\"gridX\":1064,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":636,\"gridY\":0,\"type\":31},{\"gridX\":776,\"gridY\":0,\"type\":30},{\"gridX\":280,\"gridY\":0,\"type\":32},{\"gridX\":464,\"gridY\":0,\"type\":30}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":292,\"gridY\":122,\"type\":0},{\"gridX\":296,\"gridY\":122,\"type\":1},{\"gridX\":310,\"gridY\":122,\"type\":1},{\"gridX\":324,\"gridY\":122,\"type\":1},{\"gridX\":338,\"gridY\":122,\"type\":2},{\"gridX\":177,\"gridY\":96,\"type\":8},{\"gridX\":91,\"gridY\":96,\"type\":22},{\"gridX\":122,\"gridY\":96,\"type\":22},{\"gridX\":364,\"gridY\":76,\"type\":3},{\"gridX\":368,\"gridY\":76,\"type\":4},{\"gridX\":382,\"gridY\":76,\"type\":4},{\"gridX\":396,\"gridY\":76,\"type\":5},{\"gridX\":393,\"gridY\":81,\"type\":19},{\"gridX\":400,\"gridY\":124,\"type\":0},{\"gridX\":404,\"gridY\":124,\"type\":1},{\"gridX\":418,\"gridY\":124,\"type\":1},{\"gridX\":432,\"gridY\":124,\"type\":2},{\"gridX\":581,\"gridY\":96,\"type\":7},{\"gridX\":1046,\"gridY\":96,\"type\":12}],\"enemies\":[{\"gridX\":939,\"gridY\":96,\"type\":0}],\"collectibles\":[{\"gridX\":56,\"gridY\":104,\"type\":0},{\"gridX\":64,\"gridY\":108,\"type\":0},{\"gridX\":72,\"gridY\":112,\"type\":0},{\"gridX\":112,\"gridY\":112,\"type\":0},{\"gridX\":120,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":120,\"type\":0},{\"gridX\":208,\"gridY\":100,\"type\":0},{\"gridX\":200,\"gridY\":100,\"type\":0},{\"gridX\":216,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":304,\"gridY\":132,\"type\":0},{\"gridX\":312,\"gridY\":132,\"type\":0},{\"gridX\":320,\"gridY\":132,\"type\":0},{\"gridX\":328,\"gridY\":132,\"type\":0},{\"gridX\":336,\"gridY\":132,\"type\":0},{\"gridX\":392,\"gridY\":88,\"type\":1},{\"gridX\":428,\"gridY\":132,\"type\":0},{\"gridX\":420,\"gridY\":132,\"type\":0},{\"gridX\":472,\"gridY\":100,\"type\":0},{\"gridX\":512,\"gridY\":104,\"type\":0},{\"gridX\":504,\"gridY\":104,\"type\":0},{\"gridX\":496,\"gridY\":104,\"type\":0},{\"gridX\":488,\"gridY\":100,\"type\":0},{\"gridX\":480,\"gridY\":96,\"type\":0},{\"gridX\":448,\"gridY\":116,\"type\":0},{\"gridX\":440,\"gridY\":124,\"type\":0},{\"gridX\":456,\"gridY\":108,\"type\":0},{\"gridX\":464,\"gridY\":104,\"type\":0},{\"gridX\":624,\"gridY\":152,\"type\":1},{\"gridX\":732,\"gridY\":104,\"type\":0},{\"gridX\":740,\"gridY\":104,\"type\":0},{\"gridX\":748,\"gridY\":104,\"type\":0},{\"gridX\":960,\"gridY\":168,\"type\":1},{\"gridX\":828,\"gridY\":104,\"type\":0},{\"gridX\":836,\"gridY\":104,\"type\":0},{\"gridX\":844,\"gridY\":104,\"type\":0},{\"gridX\":872,\"gridY\":104,\"type\":0},{\"gridX\":880,\"gridY\":104,\"type\":0},{\"gridX\":888,\"gridY\":104,\"type\":0},{\"gridX\":412,\"gridY\":132,\"type\":0},{\"gridX\":404,\"gridY\":132,\"type\":0}],\"jons\":[{\"gridX\":12,\"gridY\":100}]}");
    
    return instance;
}

/// Chapter 1 Level 4 ///

Chapter1Level4 * Chapter1Level4::getInstance()
{
    static Chapter1Level4 *instance = new Chapter1Level4("{\"world\":1,\"level\":5,\"midgrounds\":[{\"gridX\":1585,\"gridY\":96,\"type\":2},{\"gridX\":1161,\"gridY\":96,\"type\":2},{\"gridX\":1508,\"gridY\":96,\"type\":1},{\"gridX\":1476,\"gridY\":96,\"type\":0},{\"gridX\":1548,\"gridY\":96,\"type\":0},{\"gridX\":1082,\"gridY\":96,\"type\":1},{\"gridX\":1086,\"gridY\":96,\"type\":0},{\"gridX\":864,\"gridY\":96,\"type\":0},{\"gridX\":812,\"gridY\":96,\"type\":2},{\"gridX\":897,\"gridY\":96,\"type\":0},{\"gridX\":549,\"gridY\":96,\"type\":2},{\"gridX\":616,\"gridY\":96,\"type\":1},{\"gridX\":336,\"gridY\":96,\"type\":1},{\"gridX\":465,\"gridY\":96,\"type\":0},{\"gridX\":389,\"gridY\":96,\"type\":2},{\"gridX\":199,\"gridY\":96,\"type\":2},{\"gridX\":262,\"gridY\":96,\"type\":0},{\"gridX\":4,\"gridY\":96,\"type\":2},{\"gridX\":66,\"gridY\":96,\"type\":1},{\"gridX\":111,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":28},{\"gridX\":1160,\"gridY\":0,\"type\":28},{\"gridX\":1288,\"gridY\":0,\"type\":28},{\"gridX\":1416,\"gridY\":0,\"type\":28},{\"gridX\":1544,\"gridY\":0,\"type\":28},{\"gridX\":1672,\"gridY\":0,\"type\":29}],\"pits\":[{\"gridX\":161,\"gridY\":0,\"type\":30},{\"gridX\":309,\"gridY\":0,\"type\":30},{\"gridX\":367,\"gridY\":0,\"type\":30},{\"gridX\":490,\"gridY\":0,\"type\":31},{\"gridX\":663,\"gridY\":0,\"type\":31},{\"gridX\":743,\"gridY\":0,\"type\":31},{\"gridX\":927,\"gridY\":0,\"type\":32},{\"gridX\":1213,\"gridY\":0,\"type\":33}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":1594,\"gridY\":96,\"type\":12},{\"gridX\":154,\"gridY\":96,\"type\":19},{\"gridX\":1122,\"gridY\":96,\"type\":24}],\"midBossForegroundObjects\":[],\"enemies\":[{\"gridX\":458,\"gridY\":96,\"type\":4},{\"gridX\":855,\"gridY\":96,\"type\":4},{\"gridX\":1532,\"gridY\":96,\"type\":4}],\"collectibles\":[{\"gridX\":216,\"gridY\":154,\"type\":1},{\"gridX\":353,\"gridY\":121,\"type\":0},{\"gridX\":260,\"gridY\":105,\"type\":0},{\"gridX\":276,\"gridY\":105,\"type\":0},{\"gridX\":288,\"gridY\":105,\"type\":0},{\"gridX\":49,\"gridY\":101,\"type\":0},{\"gridX\":60,\"gridY\":101,\"type\":0},{\"gridX\":72,\"gridY\":100,\"type\":0},{\"gridX\":37,\"gridY\":101,\"type\":0},{\"gridX\":353,\"gridY\":113,\"type\":0},{\"gridX\":353,\"gridY\":104,\"type\":0},{\"gridX\":415,\"gridY\":113,\"type\":0},{\"gridX\":428,\"gridY\":104,\"type\":0},{\"gridX\":461,\"gridY\":122,\"type\":0},{\"gridX\":472,\"gridY\":120,\"type\":0},{\"gridX\":519,\"gridY\":126,\"type\":0},{\"gridX\":530,\"gridY\":126,\"type\":0},{\"gridX\":596,\"gridY\":107,\"type\":0},{\"gridX\":605,\"gridY\":107,\"type\":0},{\"gridX\":613,\"gridY\":106,\"type\":0},{\"gridX\":622,\"gridY\":106,\"type\":0},{\"gridX\":784,\"gridY\":81,\"type\":1},{\"gridX\":868,\"gridY\":119,\"type\":0},{\"gridX\":829,\"gridY\":105,\"type\":0},{\"gridX\":903,\"gridY\":105,\"type\":0},{\"gridX\":1281,\"gridY\":144,\"type\":1},{\"gridX\":1220,\"gridY\":113,\"type\":0},{\"gridX\":1231,\"gridY\":120,\"type\":0},{\"gridX\":1239,\"gridY\":125,\"type\":0},{\"gridX\":1246,\"gridY\":131,\"type\":0},{\"gridX\":1252,\"gridY\":135,\"type\":0},{\"gridX\":1260,\"gridY\":138,\"type\":0},{\"gridX\":1308,\"gridY\":124,\"type\":0},{\"gridX\":1322,\"gridY\":122,\"type\":0},{\"gridX\":1333,\"gridY\":119,\"type\":0},{\"gridX\":1346,\"gridY\":118,\"type\":0},{\"gridX\":1360,\"gridY\":116,\"type\":0},{\"gridX\":1374,\"gridY\":114,\"type\":0},{\"gridX\":1386,\"gridY\":112,\"type\":0},{\"gridX\":1400,\"gridY\":111,\"type\":0},{\"gridX\":942,\"gridY\":123,\"type\":0},{\"gridX\":961,\"gridY\":126,\"type\":0},{\"gridX\":984,\"gridY\":129,\"type\":0},{\"gridX\":1003,\"gridY\":129,\"type\":0},{\"gridX\":1026,\"gridY\":125,\"type\":0},{\"gridX\":1041,\"gridY\":121,\"type\":0},{\"gridX\":1122,\"gridY\":122,\"type\":0},{\"gridX\":1133,\"gridY\":122,\"type\":0},{\"gridX\":1144,\"gridY\":122,\"type\":0},{\"gridX\":1413,\"gridY\":109,\"type\":0},{\"gridX\":1426,\"gridY\":107,\"type\":0},{\"gridX\":1441,\"gridY\":106,\"type\":0},{\"gridX\":1521,\"gridY\":99,\"type\":0}],\"jons\":[{\"gridX\":11,\"gridY\":102}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 5 ///

Chapter1Level5 * Chapter1Level5::getInstance()
{
    static Chapter1Level5 *instance = new Chapter1Level5("{\"world\":1,\"level\":4,\"midgrounds\":[{\"gridX\":140,\"gridY\":96,\"type\":2},{\"gridX\":88,\"gridY\":96,\"type\":0},{\"gridX\":24,\"gridY\":96,\"type\":1},{\"gridX\":424,\"gridY\":96,\"type\":2},{\"gridX\":479,\"gridY\":96,\"type\":1},{\"gridX\":746,\"gridY\":96,\"type\":0},{\"gridX\":929,\"gridY\":96,\"type\":0},{\"gridX\":782,\"gridY\":96,\"type\":2},{\"gridX\":856,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":420,\"gridY\":0,\"type\":25},{\"gridX\":712,\"gridY\":0,\"type\":25},{\"gridX\":4,\"gridY\":0,\"type\":28},{\"gridX\":132,\"gridY\":0,\"type\":28},{\"gridX\":428,\"gridY\":0,\"type\":28},{\"gridX\":720,\"gridY\":0,\"type\":28},{\"gridX\":848,\"gridY\":0,\"type\":28},{\"gridX\":260,\"gridY\":0,\"type\":29},{\"gridX\":556,\"gridY\":0,\"type\":29},{\"gridX\":976,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":224,\"gridY\":96,\"type\":6},{\"gridX\":168,\"gridY\":96,\"type\":24},{\"gridX\":600,\"gridY\":120,\"type\":0},{\"gridX\":604,\"gridY\":120,\"type\":1},{\"gridX\":618,\"gridY\":120,\"type\":2},{\"gridX\":979,\"gridY\":96,\"type\":12},{\"gridX\":792,\"gridY\":106,\"type\":0},{\"gridX\":796,\"gridY\":106,\"type\":1},{\"gridX\":810,\"gridY\":106,\"type\":2},{\"gridX\":882,\"gridY\":96,\"type\":24},{\"gridX\":914,\"gridY\":96,\"type\":24},{\"gridX\":560,\"gridY\":96,\"type\":16},{\"gridX\":549,\"gridY\":96,\"type\":14},{\"gridX\":544,\"gridY\":96,\"type\":13}],\"midBossForegroundObjects\":[],\"enemies\":[{\"gridX\":786,\"gridY\":96,\"type\":2},{\"gridX\":797,\"gridY\":111,\"type\":2},{\"gridX\":100,\"gridY\":96,\"type\":2},{\"gridX\":428,\"gridY\":96,\"type\":2}],\"collectibles\":[{\"gridX\":36,\"gridY\":104,\"type\":0},{\"gridX\":44,\"gridY\":104,\"type\":0},{\"gridX\":52,\"gridY\":104,\"type\":0},{\"gridX\":160,\"gridY\":112,\"type\":0},{\"gridX\":168,\"gridY\":116,\"type\":0},{\"gridX\":176,\"gridY\":120,\"type\":0},{\"gridX\":184,\"gridY\":124,\"type\":0},{\"gridX\":324,\"gridY\":136,\"type\":1},{\"gridX\":686,\"gridY\":170,\"type\":1},{\"gridX\":634,\"gridY\":142,\"type\":0},{\"gridX\":649,\"gridY\":151,\"type\":0},{\"gridX\":664,\"gridY\":157,\"type\":0},{\"gridX\":674,\"gridY\":163,\"type\":0},{\"gridX\":880,\"gridY\":112,\"type\":0},{\"gridX\":888,\"gridY\":116,\"type\":0},{\"gridX\":896,\"gridY\":120,\"type\":0},{\"gridX\":904,\"gridY\":120,\"type\":0},{\"gridX\":912,\"gridY\":120,\"type\":0},{\"gridX\":920,\"gridY\":120,\"type\":0},{\"gridX\":928,\"gridY\":120,\"type\":0},{\"gridX\":935,\"gridY\":119,\"type\":0},{\"gridX\":974,\"gridY\":119,\"type\":1},{\"gridX\":451,\"gridY\":102,\"type\":0},{\"gridX\":459,\"gridY\":107,\"type\":0},{\"gridX\":467,\"gridY\":102,\"type\":0},{\"gridX\":475,\"gridY\":108,\"type\":0},{\"gridX\":482,\"gridY\":101,\"type\":0},{\"gridX\":490,\"gridY\":107,\"type\":0},{\"gridX\":496,\"gridY\":99,\"type\":0},{\"gridX\":561,\"gridY\":117,\"type\":0},{\"gridX\":570,\"gridY\":122,\"type\":0},{\"gridX\":580,\"gridY\":127,\"type\":0}],\"jons\":[{\"gridX\":2,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 6 ///

Chapter1Level6 * Chapter1Level6::getInstance()
{
    static Chapter1Level6 *instance = new Chapter1Level6("{\"world\":1,\"level\":6,\"midgrounds\":[{\"gridX\":40,\"gridY\":96,\"type\":2},{\"gridX\":0,\"gridY\":96,\"type\":0},{\"gridX\":178,\"gridY\":96,\"type\":1},{\"gridX\":56,\"gridY\":96,\"type\":0},{\"gridX\":64,\"gridY\":96,\"type\":2},{\"gridX\":923,\"gridY\":96,\"type\":2},{\"gridX\":878,\"gridY\":96,\"type\":1},{\"gridX\":912,\"gridY\":96,\"type\":0},{\"gridX\":812,\"gridY\":96,\"type\":0},{\"gridX\":830,\"gridY\":96,\"type\":2},{\"gridX\":977,\"gridY\":96,\"type\":1},{\"gridX\":818,\"gridY\":96,\"type\":1},{\"gridX\":868,\"gridY\":96,\"type\":0},{\"gridX\":574,\"gridY\":96,\"type\":2},{\"gridX\":511,\"gridY\":96,\"type\":1},{\"gridX\":349,\"gridY\":96,\"type\":0},{\"gridX\":309,\"gridY\":96,\"type\":2},{\"gridX\":16,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":184,\"gridY\":0,\"type\":25},{\"gridX\":308,\"gridY\":0,\"type\":25},{\"gridX\":516,\"gridY\":0,\"type\":25},{\"gridX\":809,\"gridY\":0,\"type\":25},{\"gridX\":192,\"gridY\":0,\"type\":26},{\"gridX\":316,\"gridY\":0,\"type\":27},{\"gridX\":945,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":524,\"gridY\":0,\"type\":28},{\"gridX\":817,\"gridY\":0,\"type\":28},{\"gridX\":134,\"gridY\":0,\"type\":29},{\"gridX\":224,\"gridY\":0,\"type\":29},{\"gridX\":380,\"gridY\":0,\"type\":29},{\"gridX\":652,\"gridY\":0,\"type\":29},{\"gridX\":1009,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":225,\"gridY\":96,\"type\":17},{\"gridX\":252,\"gridY\":132,\"type\":0},{\"gridX\":256,\"gridY\":132,\"type\":1},{\"gridX\":270,\"gridY\":132,\"type\":2},{\"gridX\":400,\"gridY\":148,\"type\":0},{\"gridX\":446,\"gridY\":148,\"type\":1},{\"gridX\":432,\"gridY\":148,\"type\":1},{\"gridX\":418,\"gridY\":148,\"type\":1},{\"gridX\":404,\"gridY\":148,\"type\":1},{\"gridX\":460,\"gridY\":148,\"type\":1},{\"gridX\":474,\"gridY\":148,\"type\":1},{\"gridX\":488,\"gridY\":148,\"type\":2},{\"gridX\":435,\"gridY\":153,\"type\":13},{\"gridX\":440,\"gridY\":153,\"type\":14},{\"gridX\":451,\"gridY\":153,\"type\":16},{\"gridX\":556,\"gridY\":186,\"type\":34},{\"gridX\":556,\"gridY\":172,\"type\":34},{\"gridX\":556,\"gridY\":158,\"type\":34},{\"gridX\":556,\"gridY\":144,\"type\":34},{\"gridX\":577,\"gridY\":96,\"type\":8},{\"gridX\":645,\"gridY\":96,\"type\":10},{\"gridX\":1009,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[{\"gridX\":160,\"gridY\":120,\"type\":3},{\"gridX\":248,\"gridY\":100,\"type\":3},{\"gridX\":276,\"gridY\":100,\"type\":3},{\"gridX\":324,\"gridY\":96,\"type\":2},{\"gridX\":354,\"gridY\":96,\"type\":0},{\"gridX\":482,\"gridY\":154,\"type\":2},{\"gridX\":411,\"gridY\":96,\"type\":3},{\"gridX\":432,\"gridY\":102,\"type\":3},{\"gridX\":453,\"gridY\":97,\"type\":3},{\"gridX\":474,\"gridY\":104,\"type\":3},{\"gridX\":698,\"gridY\":139,\"type\":3},{\"gridX\":726,\"gridY\":134,\"type\":3},{\"gridX\":751,\"gridY\":132,\"type\":3},{\"gridX\":777,\"gridY\":128,\"type\":3},{\"gridX\":914,\"gridY\":119,\"type\":3},{\"gridX\":917,\"gridY\":96,\"type\":2},{\"gridX\":789,\"gridY\":128,\"type\":2}],\"collectibles\":[{\"gridX\":260,\"gridY\":150,\"type\":1},{\"gridX\":516,\"gridY\":170,\"type\":1},{\"gridX\":941,\"gridY\":160,\"type\":1},{\"gridX\":59,\"gridY\":103,\"type\":0},{\"gridX\":90,\"gridY\":102,\"type\":0},{\"gridX\":111,\"gridY\":102,\"type\":0},{\"gridX\":161,\"gridY\":134,\"type\":0},{\"gridX\":233,\"gridY\":126,\"type\":0},{\"gridX\":243,\"gridY\":137,\"type\":0},{\"gridX\":308,\"gridY\":120,\"type\":0},{\"gridX\":316,\"gridY\":109,\"type\":0},{\"gridX\":340,\"gridY\":111,\"type\":0},{\"gridX\":363,\"gridY\":118,\"type\":0},{\"gridX\":369,\"gridY\":131,\"type\":0},{\"gridX\":375,\"gridY\":140,\"type\":0},{\"gridX\":383,\"gridY\":149,\"type\":0},{\"gridX\":394,\"gridY\":159,\"type\":0},{\"gridX\":406,\"gridY\":166,\"type\":0},{\"gridX\":452,\"gridY\":174,\"type\":0},{\"gridX\":473,\"gridY\":168,\"type\":0},{\"gridX\":537,\"gridY\":130,\"type\":0},{\"gridX\":541,\"gridY\":120,\"type\":0},{\"gridX\":543,\"gridY\":111,\"type\":0},{\"gridX\":577,\"gridY\":114,\"type\":0},{\"gridX\":609,\"gridY\":118,\"type\":0},{\"gridX\":617,\"gridY\":122,\"type\":0},{\"gridX\":623,\"gridY\":126,\"type\":0},{\"gridX\":692,\"gridY\":118,\"type\":0},{\"gridX\":711,\"gridY\":115,\"type\":0},{\"gridX\":732,\"gridY\":114,\"type\":0},{\"gridX\":747,\"gridY\":114,\"type\":0},{\"gridX\":770,\"gridY\":112,\"type\":0},{\"gridX\":786,\"gridY\":111,\"type\":0},{\"gridX\":839,\"gridY\":104,\"type\":0},{\"gridX\":859,\"gridY\":104,\"type\":0},{\"gridX\":871,\"gridY\":104,\"type\":0},{\"gridX\":849,\"gridY\":103,\"type\":0},{\"gridX\":423,\"gridY\":172,\"type\":0}],\"jons\":[{\"gridX\":5,\"gridY\":98}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}

/// Chapter 1 Level 7 ///

Chapter1Level7 * Chapter1Level7::getInstance()
{
    static Chapter1Level7 *instance = new Chapter1Level7("{\"world\":1,\"level\":7,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 8 ///

Chapter1Level8 * Chapter1Level8::getInstance()
{
    static Chapter1Level8 *instance = new Chapter1Level8("{\"world\":1,\"level\":8,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 9 ///

Chapter1Level9 * Chapter1Level9::getInstance()
{
    static Chapter1Level9 *instance = new Chapter1Level9("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 11 ///

Chapter1Level11 * Chapter1Level11::getInstance()
{
    static Chapter1Level11 *instance = new Chapter1Level11("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 12 ///

Chapter1Level12 * Chapter1Level12::getInstance()
{
    static Chapter1Level12 *instance = new Chapter1Level12("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 13 ///

Chapter1Level13 * Chapter1Level13::getInstance()
{
    static Chapter1Level13 *instance = new Chapter1Level13("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 14 ///

Chapter1Level14 * Chapter1Level14::getInstance()
{
    static Chapter1Level14 *instance = new Chapter1Level14("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 15 ///

Chapter1Level15 * Chapter1Level15::getInstance()
{
    static Chapter1Level15 *instance = new Chapter1Level15("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 16 ///

Chapter1Level16 * Chapter1Level16::getInstance()
{
    static Chapter1Level16 *instance = new Chapter1Level16("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 17 ///

Chapter1Level17 * Chapter1Level17::getInstance()
{
    static Chapter1Level17 *instance = new Chapter1Level17("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 18 ///

Chapter1Level18 * Chapter1Level18::getInstance()
{
    static Chapter1Level18 *instance = new Chapter1Level18("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 19 ///

Chapter1Level19 * Chapter1Level19::getInstance()
{
    static Chapter1Level19 *instance = new Chapter1Level19("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}

/// Chapter 1 Level 20 ///

Chapter1Level20 * Chapter1Level20::getInstance()
{
    static Chapter1Level20 *instance = new Chapter1Level20("{\"world\":1,\"level\":9,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}
