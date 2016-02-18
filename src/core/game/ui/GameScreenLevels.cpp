//
//  GameScreenLevels.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
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
    static Level *instance = new Level(-1, nullptr);
    
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
    }
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1);
    
    if (m_hasShownOpeningSequence)
    {
        Jon& jon = m_game->getJon();
        jon.setAllowedToMove(true);
    }
    else
    {
        Assets::getInstance()->setMusicId(MUSIC_PLAY_DEMO);
        
        gs->processTouchEvents();
        gs->m_renderer->beginOpeningPanningSequence(*m_game);
        gs->m_iNumFramesToDiscard = 1;
        
        m_hasShownOpeningSequence = true;
    }
}

void Level::execute(GameScreen* gs)
{
    Jon& jon = m_game->getJon();
    
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame();
        
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
        
        gs->m_renderer->renderJon(*m_game);
        
        if (m_hasOpeningSequenceCompleted)
        {
            gs->m_renderer->renderHud(*m_game, *m_backButton, gs->m_iFPS);
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
        if (!m_hasOpeningSequenceCompleted)
        {
            if (gs->m_stateMachine->getPreviousState() == GameScreenLevelEditor::getInstance() && handleOpeningSequenceTouchInput(gs))
            {
                gs->m_renderer->zoomIn();
                m_hasOpeningSequenceCompleted = true;
                jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
                return;
            }
            else
            {
                gs->processTouchEvents();
            }
            
            jon.update(gs->m_fDeltaTime);
            
            int result = gs->m_renderer->updateCameraToFollowPathToJon(*m_game, gs->m_fDeltaTime);
            m_hasOpeningSequenceCompleted = result == 3;
            m_activateRadialBlur = result == 1;
            jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
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
                
                Assets::getInstance()->addSoundIdToPlayQueue(SOUND_STOP_JON_VAMPIRE_GLIDE);
                
                m_fStateTime += gs->m_fDeltaTime;
                
                if (m_fStateTime > 5)
                {
                    m_game->reset();
                    enter(gs);
                    
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
    
    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_STOP_JON_VAMPIRE_GLIDE);
    
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
                if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getBounds()))
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

Level::Level(int level, const char* json) : m_sourceGame(nullptr), m_fStateTime(0.0f), m_isReleasingShockwave(false), m_fShockwaveElapsedTime(0.0f), m_fShockwaveCenterX(0.0f), m_fShockwaveCenterY(0.0f), m_hasShownOpeningSequence(false), m_hasOpeningSequenceCompleted(false), m_activateRadialBlur(false), m_hasSwiped(false), m_showDeathTransOut(false)
{
    m_json = json;
    m_game = std::unique_ptr<Game>(new Game(level));
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
}

/// Chapter 1 Level 1 ///

Chapter1Level1 * Chapter1Level1::getInstance()
{
    static Chapter1Level1 *instance = new Chapter1Level1("{\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":0},{\"gridX\":48,\"gridY\":96,\"type\":1},{\"gridX\":96,\"gridY\":96,\"type\":2},{\"gridX\":160,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":0},{\"gridX\":276,\"gridY\":96,\"type\":2},{\"gridX\":340,\"gridY\":96,\"type\":1},{\"gridX\":388,\"gridY\":96,\"type\":2},{\"gridX\":460,\"gridY\":96,\"type\":0},{\"gridX\":504,\"gridY\":96,\"type\":0},{\"gridX\":652,\"gridY\":96,\"type\":2},{\"gridX\":720,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":276,\"gridY\":0,\"type\":25},{\"gridX\":636,\"gridY\":0,\"type\":25},{\"gridX\":200,\"gridY\":0,\"type\":26},{\"gridX\":136,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":284,\"gridY\":0,\"type\":28},{\"gridX\":412,\"gridY\":0,\"type\":28},{\"gridX\":644,\"gridY\":0,\"type\":28},{\"gridX\":232,\"gridY\":0,\"type\":29},{\"gridX\":540,\"gridY\":0,\"type\":29},{\"gridX\":772,\"gridY\":0,\"type\":29}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":765,\"gridY\":96,\"type\":12},{\"gridX\":392,\"gridY\":96,\"type\":11},{\"gridX\":470,\"gridY\":96,\"type\":18},{\"gridX\":433,\"gridY\":96,\"type\":17},{\"gridX\":732,\"gridY\":96,\"type\":17},{\"gridX\":748,\"gridY\":96,\"type\":18}],\"enemies\":[],\"collectibles\":[{\"gridX\":52,\"gridY\":104,\"type\":0},{\"gridX\":60,\"gridY\":104,\"type\":0},{\"gridX\":68,\"gridY\":104,\"type\":0},{\"gridX\":76,\"gridY\":104,\"type\":0},{\"gridX\":84,\"gridY\":104,\"type\":0},{\"gridX\":92,\"gridY\":104,\"type\":0},{\"gridX\":100,\"gridY\":104,\"type\":0},{\"gridX\":108,\"gridY\":108,\"type\":0},{\"gridX\":116,\"gridY\":112,\"type\":0},{\"gridX\":124,\"gridY\":116,\"type\":0},{\"gridX\":132,\"gridY\":120,\"type\":0},{\"gridX\":148,\"gridY\":120,\"type\":0},{\"gridX\":156,\"gridY\":116,\"type\":0},{\"gridX\":164,\"gridY\":112,\"type\":0},{\"gridX\":172,\"gridY\":108,\"type\":0},{\"gridX\":196,\"gridY\":104,\"type\":0},{\"gridX\":180,\"gridY\":104,\"type\":0},{\"gridX\":188,\"gridY\":104,\"type\":0},{\"gridX\":140,\"gridY\":124,\"type\":0},{\"gridX\":256,\"gridY\":88,\"type\":1},{\"gridX\":304,\"gridY\":104,\"type\":0},{\"gridX\":316,\"gridY\":104,\"type\":0},{\"gridX\":328,\"gridY\":104,\"type\":0},{\"gridX\":392,\"gridY\":116,\"type\":0},{\"gridX\":452,\"gridY\":168,\"type\":1},{\"gridX\":340,\"gridY\":104,\"type\":0},{\"gridX\":590,\"gridY\":112,\"type\":1},{\"gridX\":512,\"gridY\":104,\"type\":0},{\"gridX\":520,\"gridY\":104,\"type\":0},{\"gridX\":528,\"gridY\":104,\"type\":0},{\"gridX\":660,\"gridY\":104,\"type\":0},{\"gridX\":668,\"gridY\":104,\"type\":0},{\"gridX\":676,\"gridY\":104,\"type\":0},{\"gridX\":668,\"gridY\":112,\"type\":0},{\"gridX\":696,\"gridY\":104,\"type\":0},{\"gridX\":704,\"gridY\":104,\"type\":0},{\"gridX\":712,\"gridY\":104,\"type\":0},{\"gridX\":704,\"gridY\":112,\"type\":0}],\"jons\":[{\"gridX\":16,\"gridY\":108}]}");
    
    return instance;
}

/// Chapter 1 Level 2 ///

Chapter1Level2 * Chapter1Level2::getInstance()
{
    static Chapter1Level2 *instance = new Chapter1Level2("{\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":1},{\"gridX\":50,\"gridY\":96,\"type\":2},{\"gridX\":108,\"gridY\":96,\"type\":0},{\"gridX\":172,\"gridY\":96,\"type\":2},{\"gridX\":244,\"gridY\":96,\"type\":2},{\"gridX\":488,\"gridY\":96,\"type\":1},{\"gridX\":532,\"gridY\":96,\"type\":1},{\"gridX\":572,\"gridY\":96,\"type\":2},{\"gridX\":356,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":168,\"gridY\":0,\"type\":25},{\"gridX\":348,\"gridY\":0,\"type\":25},{\"gridX\":480,\"gridY\":0,\"type\":25},{\"gridX\":356,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":488,\"gridY\":0,\"type\":28},{\"gridX\":176,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":29},{\"gridX\":304,\"gridY\":0,\"type\":29},{\"gridX\":420,\"gridY\":0,\"type\":29},{\"gridX\":616,\"gridY\":0,\"type\":29}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":72,\"gridY\":112,\"type\":0},{\"gridX\":132,\"gridY\":96,\"type\":19},{\"gridX\":113,\"gridY\":96,\"type\":17},{\"gridX\":76,\"gridY\":112,\"type\":1},{\"gridX\":90,\"gridY\":112,\"type\":2},{\"gridX\":601,\"gridY\":96,\"type\":12},{\"gridX\":382,\"gridY\":96,\"type\":23},{\"gridX\":371,\"gridY\":96,\"type\":19},{\"gridX\":249,\"gridY\":96,\"type\":23}],\"enemies\":[],\"collectibles\":[{\"gridX\":44,\"gridY\":104,\"type\":0},{\"gridX\":52,\"gridY\":104,\"type\":0},{\"gridX\":52,\"gridY\":112,\"type\":0},{\"gridX\":60,\"gridY\":112,\"type\":0},{\"gridX\":72,\"gridY\":120,\"type\":0},{\"gridX\":80,\"gridY\":120,\"type\":0},{\"gridX\":88,\"gridY\":120,\"type\":0},{\"gridX\":156,\"gridY\":180,\"type\":1},{\"gridX\":256,\"gridY\":108,\"type\":1},{\"gridX\":388,\"gridY\":152,\"type\":1},{\"gridX\":372,\"gridY\":124,\"type\":0},{\"gridX\":372,\"gridY\":112,\"type\":0},{\"gridX\":372,\"gridY\":104,\"type\":0},{\"gridX\":466,\"gridY\":122,\"type\":0},{\"gridX\":472,\"gridY\":112,\"type\":0},{\"gridX\":480,\"gridY\":104,\"type\":0},{\"gridX\":492,\"gridY\":104,\"type\":0},{\"gridX\":500,\"gridY\":104,\"type\":0},{\"gridX\":508,\"gridY\":104,\"type\":0},{\"gridX\":536,\"gridY\":104,\"type\":0},{\"gridX\":528,\"gridY\":104,\"type\":0},{\"gridX\":544,\"gridY\":120,\"type\":0},{\"gridX\":536,\"gridY\":112,\"type\":0},{\"gridX\":544,\"gridY\":112,\"type\":0},{\"gridX\":552,\"gridY\":120,\"type\":0},{\"gridX\":584,\"gridY\":112,\"type\":0},{\"gridX\":592,\"gridY\":104,\"type\":0},{\"gridX\":262,\"gridY\":112,\"type\":0},{\"gridX\":250,\"gridY\":112,\"type\":0}],\"jons\":[{\"gridX\":16,\"gridY\":104}]}");
    
    return instance;
}

/// Chapter 1 Level 3 ///

Chapter1Level3 * Chapter1Level3::getInstance()
{
    static Chapter1Level3 *instance = new Chapter1Level3("{\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":68,\"gridY\":96,\"type\":2},{\"gridX\":132,\"gridY\":96,\"type\":0},{\"gridX\":172,\"gridY\":96,\"type\":0},{\"gridX\":208,\"gridY\":96,\"type\":2},{\"gridX\":488,\"gridY\":96,\"type\":2},{\"gridX\":544,\"gridY\":96,\"type\":0},{\"gridX\":720,\"gridY\":96,\"type\":0},{\"gridX\":892,\"gridY\":96,\"type\":0},{\"gridX\":816,\"gridY\":96,\"type\":1},{\"gridX\":960,\"gridY\":96,\"type\":1},{\"gridX\":1020,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":492,\"gridY\":0,\"type\":25},{\"gridX\":712,\"gridY\":0,\"type\":25},{\"gridX\":812,\"gridY\":0,\"type\":25},{\"gridX\":720,\"gridY\":0,\"type\":26},{\"gridX\":564,\"gridY\":0,\"type\":26},{\"gridX\":500,\"gridY\":0,\"type\":27},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":820,\"gridY\":0,\"type\":28},{\"gridX\":948,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":29},{\"gridX\":596,\"gridY\":0,\"type\":29},{\"gridX\":752,\"gridY\":0,\"type\":29},{\"gridX\":1076,\"gridY\":0,\"type\":29}],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":292,\"gridY\":122,\"type\":0},{\"gridX\":296,\"gridY\":122,\"type\":1},{\"gridX\":310,\"gridY\":122,\"type\":1},{\"gridX\":324,\"gridY\":122,\"type\":1},{\"gridX\":338,\"gridY\":122,\"type\":2},{\"gridX\":166,\"gridY\":96,\"type\":8},{\"gridX\":204,\"gridY\":96,\"type\":8},{\"gridX\":60,\"gridY\":96,\"type\":22},{\"gridX\":91,\"gridY\":96,\"type\":22},{\"gridX\":122,\"gridY\":96,\"type\":22},{\"gridX\":364,\"gridY\":76,\"type\":3},{\"gridX\":368,\"gridY\":76,\"type\":4},{\"gridX\":382,\"gridY\":76,\"type\":4},{\"gridX\":396,\"gridY\":76,\"type\":5},{\"gridX\":393,\"gridY\":81,\"type\":19},{\"gridX\":400,\"gridY\":124,\"type\":0},{\"gridX\":404,\"gridY\":124,\"type\":1},{\"gridX\":418,\"gridY\":124,\"type\":1},{\"gridX\":432,\"gridY\":124,\"type\":2},{\"gridX\":578,\"gridY\":96,\"type\":7},{\"gridX\":1059,\"gridY\":96,\"type\":12},{\"gridX\":412,\"gridY\":129,\"type\":17}],\"enemies\":[{\"gridX\":939,\"gridY\":96,\"type\":0}],\"collectibles\":[{\"gridX\":72,\"gridY\":116,\"type\":0},{\"gridX\":80,\"gridY\":120,\"type\":0},{\"gridX\":88,\"gridY\":124,\"type\":0},{\"gridX\":112,\"gridY\":112,\"type\":0},{\"gridX\":120,\"gridY\":116,\"type\":0},{\"gridX\":128,\"gridY\":120,\"type\":0},{\"gridX\":180,\"gridY\":100,\"type\":0},{\"gridX\":188,\"gridY\":100,\"type\":0},{\"gridX\":196,\"gridY\":100,\"type\":0},{\"gridX\":296,\"gridY\":132,\"type\":0},{\"gridX\":304,\"gridY\":132,\"type\":0},{\"gridX\":312,\"gridY\":132,\"type\":0},{\"gridX\":320,\"gridY\":132,\"type\":0},{\"gridX\":328,\"gridY\":132,\"type\":0},{\"gridX\":336,\"gridY\":132,\"type\":0},{\"gridX\":392,\"gridY\":88,\"type\":1},{\"gridX\":428,\"gridY\":132,\"type\":0},{\"gridX\":420,\"gridY\":132,\"type\":0},{\"gridX\":472,\"gridY\":100,\"type\":0},{\"gridX\":512,\"gridY\":104,\"type\":0},{\"gridX\":504,\"gridY\":104,\"type\":0},{\"gridX\":496,\"gridY\":104,\"type\":0},{\"gridX\":488,\"gridY\":100,\"type\":0},{\"gridX\":480,\"gridY\":96,\"type\":0},{\"gridX\":448,\"gridY\":116,\"type\":0},{\"gridX\":440,\"gridY\":124,\"type\":0},{\"gridX\":456,\"gridY\":108,\"type\":0},{\"gridX\":464,\"gridY\":104,\"type\":0},{\"gridX\":612,\"gridY\":156,\"type\":1},{\"gridX\":720,\"gridY\":104,\"type\":0},{\"gridX\":732,\"gridY\":104,\"type\":0},{\"gridX\":744,\"gridY\":104,\"type\":0},{\"gridX\":960,\"gridY\":168,\"type\":1},{\"gridX\":828,\"gridY\":104,\"type\":0},{\"gridX\":836,\"gridY\":104,\"type\":0},{\"gridX\":844,\"gridY\":104,\"type\":0},{\"gridX\":872,\"gridY\":104,\"type\":0},{\"gridX\":880,\"gridY\":104,\"type\":0},{\"gridX\":888,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":12,\"gridY\":104}]}");
    
    return instance;
}
