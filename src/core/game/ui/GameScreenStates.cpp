//
//  UpwardSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "GameScreenStates.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"

/// Title Screen ///

Title * Title::getInstance()
{
    static Title *instance = new Title();
    
    return instance;
}

void Title::enter(GameScreen* gs)
{
    gs->m_renderer->init(RENDERER_TYPE_TITLE);
}

void Title::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->renderTitleScreen();
        
        gs->m_renderer->renderToScreen();
    }
    else
    {
        gs->processTouchEvents();
        
        if (gs->m_touchEvents.size() > 0)
        {
            gs->m_stateMachine->changeState(GamePlay::getInstance());
        }
    }
}

void Title::exit(GameScreen* gs)
{
    // TODO
}

/// Game Play ///

GamePlay * GamePlay::getInstance()
{
    static GamePlay *instance = new GamePlay();
    
    return instance;
}

void GamePlay::enter(GameScreen* gs)
{
    m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809},{\"x\":24,\"y\":19.9809},{\"x\":40,\"y\":19.9809}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623},{\"x\":24,\"y\":14.9623},{\"x\":40,\"y\":14.9623}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865},{\"x\":24,\"y\":5.63865},{\"x\":40,\"y\":5.63865}],\"trees\":[{\"x\":4.05,\"y\":13.8393,\"type\":2},{\"x\":14.15,\"y\":14.1668,\"type\":1},{\"x\":65.05,\"y\":13.8393,\"type\":2},{\"x\":72.7,\"y\":13.0789,\"type\":0},{\"x\":81.7001,\"y\":14.1668,\"type\":1},{\"x\":105.85,\"y\":13.8393,\"type\":2},{\"x\":112.9,\"y\":13.0789,\"type\":0},{\"x\":135.15,\"y\":14.1668,\"type\":1},{\"x\":143.85,\"y\":13.0789,\"type\":0}],\"caveSkeletons\":[],\"grounds\":[{\"x\":24.1,\"y\":8.90251,\"type\":0},{\"x\":-0.718715,\"y\":8.90251,\"type\":3},{\"x\":48.8602,\"y\":8.90251,\"type\":4},{\"x\":11.35,\"y\":2.36309,\"type\":13},{\"x\":36.0868,\"y\":2.36309,\"type\":10},{\"x\":60.812,\"y\":2.36309,\"type\":14},{\"x\":60.8003,\"y\":5.11222,\"type\":8},{\"x\":67.5605,\"y\":5.11222,\"type\":7},{\"x\":85.5488,\"y\":8.90251,\"type\":1},{\"x\":98.3441,\"y\":8.90251,\"type\":4},{\"x\":88.5163,\"y\":2.36309,\"type\":13},{\"x\":95.3,\"y\":2.36309,\"type\":12},{\"x\":102.072,\"y\":2.36309,\"type\":14},{\"x\":102.06,\"y\":5.11222,\"type\":8},{\"x\":108.82,\"y\":5.11222,\"type\":7},{\"x\":115.627,\"y\":5.11222,\"type\":9},{\"x\":132,\"y\":5.11222,\"type\":8},{\"x\":138.76,\"y\":5.11222,\"type\":7},{\"x\":145.567,\"y\":5.11222,\"type\":9}],\"holes\":[{\"x\":13.65,\"y\":8.09532},{\"x\":88.8,\"y\":8.09532}],\"logVerticalTalls\":[],\"logVerticalShorts\":[],\"thorns\":[{\"x\":17,\"y\":9.7682},{\"x\":20.2,\"y\":9.7682},{\"x\":23.45,\"y\":9.7682},{\"x\":26.5,\"y\":9.7682},{\"x\":29.6,\"y\":9.7682},{\"x\":32.6,\"y\":9.7682},{\"x\":35.7,\"y\":9.7682},{\"x\":38.85,\"y\":9.7682},{\"x\":41.7,\"y\":9.7682},{\"x\":44.7,\"y\":9.7682},{\"x\":47.65,\"y\":9.7682},{\"x\":92,\"y\":9.7682},{\"x\":94.65,\"y\":9.7682},{\"x\":97.3,\"y\":9.7682},{\"x\":103.35,\"y\":9.7682},{\"x\":106.15,\"y\":9.7682},{\"x\":108.9,\"y\":9.7682},{\"x\":111.9,\"y\":9.7682},{\"x\":114.8,\"y\":9.7682}],\"stumps\":[],\"sideSpikes\":[],\"upwardSpikes\":[{\"x\":98.1,\"y\":11.9422,\"type\":1},{\"x\":99.3,\"y\":11.9422,\"type\":1}],\"jumpSprings\":[{\"x\":59.4319,\"y\":1.98873,\"type\":1},{\"x\":58.25,\"y\":1.98873,\"type\":1},{\"x\":57.05,\"y\":1.98873,\"type\":1},{\"x\":100.692,\"y\":1.98873,\"type\":1}],\"rocks\":[],\"platforms\":[{\"x\":103.05,\"y\":19.3641,\"type\":0},{\"x\":106.35,\"y\":18.9844,\"type\":0},{\"x\":109.55,\"y\":18.6047,\"type\":0},{\"x\":98.7,\"y\":11.0531,\"type\":0},{\"x\":112.7,\"y\":18.1828,\"type\":0},{\"x\":115.95,\"y\":17.8031,\"type\":0},{\"x\":119.05,\"y\":17.5078,\"type\":0},{\"x\":122.2,\"y\":17.1703,\"type\":0},{\"x\":125.3,\"y\":16.875,\"type\":0}],\"endSigns\":[{\"x\":145.1,\"y\":9.59272}],\"carrots\":[{\"x\":103.05,\"y\":20.9946},{\"x\":106.35,\"y\":20.615},{\"x\":109.65,\"y\":20.2353},{\"x\":112.65,\"y\":19.8134},{\"x\":115.9,\"y\":19.4337},{\"x\":119.05,\"y\":19.1384},{\"x\":122.2,\"y\":18.8009},{\"x\":125.3,\"y\":18.5056}],\"goldenCarrots\":[{\"x\":143.65,\"y\":10.4044},{\"x\":142.15,\"y\":10.4044},{\"x\":140.55,\"y\":10.4044},{\"x\":139,\"y\":10.4044},{\"x\":137.5,\"y\":10.4044}],\"jons\":[{\"x\":3.2,\"y\":10.1312}]}");
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1);
    
    if (!m_hasShownOpeningSequence)
    {
        gs->m_renderer->beginOpeningPanningSequence(*m_game);
        
        m_hasShownOpeningSequence = true;
    }
}

void GamePlay::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->renderWorld(*m_game);
        
        gs->m_renderer->renderJon(*m_game);
        
        gs->m_renderer->renderToScreen();
    }
    else
    {
        if (!m_hasOpeningSequenceCompleted)
        {
            gs->processTouchEvents();
            
            Jon& jon = m_game->getJon();
            
            jon.update(gs->m_fDeltaTime, *m_game, false);
            
            m_hasOpeningSequenceCompleted = gs->m_renderer->updateCameraToFollowPathToJon(*m_game, gs->m_fDeltaTime);
            
            EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
            
            return;
        }
        
        // TODO, now wait for player to tap the screen to begin
        
        handleTouchInput(gs);
        
        m_game->updateAndClean(gs->m_fDeltaTime);
        
        Jon& jon = m_game->getJon();
        
        if (jon.isDead() || jon.getPosition().getX() - jon.getWidth() > m_game->getFarRight())
        {
            enter(gs);
            
            return;
        }
        
        if (EntityUtils::isCollected(jon, m_game->getCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
        }
        
        if (EntityUtils::isCollected(jon, m_game->getGoldenCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
        }
        
        gs->m_renderer->updateCameraToFollowJon(*m_game, gs->m_fDeltaTime);
        
        EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
    }
}

void GamePlay::exit(GameScreen* gs)
{
    m_hasShownOpeningSequence = false;
    m_hasOpeningSequenceCompleted = false;
}

void GamePlay::handleTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    Jon& jon = m_game->getJon();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        switch (i->getTouchType())
        {
            case DOWN:
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                continue;
            case DRAGGED:
                continue;
            case UP:
                if (gs->m_touchPointDown->getX() + SWIPE_WIDTH <= gs->m_touchPoint->getX())
                {
                    // Swipe Right
                    jon.triggerRightAction();
                }
                else if (gs->m_touchPointDown->getX() - SWIPE_WIDTH >= gs->m_touchPoint->getX())
                {
                    // Swipe Left
                    jon.triggerLeftAction();
                }
                else if (gs->m_touchPointDown->getY() + SWIPE_HEIGHT <= gs->m_touchPoint->getY())
                {
                    // Swipe Up
                    jon.triggerUpAction();
                }
                else if (gs->m_touchPointDown->getY() - SWIPE_HEIGHT >= gs->m_touchPoint->getY())
                {
                    // Swipe Down
                    jon.triggerDownAction();
                }
                else
                {
                    jon.triggerJump();
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                
                break;
        }
    }
}

GamePlay::GamePlay() : m_hasShownOpeningSequence(false), m_hasOpeningSequenceCompleted(false)
{
    m_game = std::unique_ptr<Game>(new Game());
}

/// Test Level ///

TestLevel * TestLevel::getInstance()
{
    static TestLevel *instance = new TestLevel();
    
    return instance;
}

void TestLevel::enter(GameScreen* gs)
{
    m_game->copy(m_sourceGame);
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1);
    
    if (!m_hasShownOpeningSequence)
    {
        gs->m_renderer->beginOpeningPanningSequence(*m_game);
        
        m_hasShownOpeningSequence = true;
    }
}

void TestLevel::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->renderWorld(*m_game);
        
        gs->m_renderer->renderJon(*m_game);
        
        gs->m_renderer->renderBackButton(*m_backButton);
        
        gs->m_renderer->renderToScreen();
    }
    else
    {
        if (!m_hasOpeningSequenceCompleted)
        {
            gs->processTouchEvents();
            
            Jon& jon = m_game->getJon();
            
            jon.update(gs->m_fDeltaTime, *m_game, false);
            
            m_hasOpeningSequenceCompleted = gs->m_renderer->updateCameraToFollowPathToJon(*m_game, gs->m_fDeltaTime);
            
            EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
            
            return;
        }
        
        // TODO, now wait for player to tap the screen to begin
        
        if (handleTouchInput(gs))
        {
            return;
        }
        
        m_game->updateAndClean(gs->m_fDeltaTime);
        
        Jon& jon = m_game->getJon();
        
        if (jon.isDead() || jon.getPosition().getX() - jon.getWidth() > m_game->getFarRight())
        {
            enter(gs);
            
            return;
        }
        
        if (EntityUtils::isCollected(jon, m_game->getCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
        }
        
        if (EntityUtils::isCollected(jon, m_game->getGoldenCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
        }
        
        gs->m_renderer->updateCameraToFollowJon(*m_game, gs->m_fDeltaTime);
        
        EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
    }
}

void TestLevel::exit(GameScreen* gs)
{
    m_hasShownOpeningSequence = false;
    m_hasOpeningSequenceCompleted = false;
}

void TestLevel::setSourceGame(Game* game)
{
    m_sourceGame = game;
}

bool TestLevel::handleTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    Jon& jon = m_game->getJon();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        switch (i->getTouchType())
        {
            case DOWN:
                if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getBounds()))
                {
                    gs->m_stateMachine->changeState(LevelEditor::getInstance());
                    return true;
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                continue;
            case DRAGGED:
                continue;
            case UP:
                if (gs->m_touchPointDown->getX() + SWIPE_WIDTH <= gs->m_touchPoint->getX())
                {
                    // Swipe Right
                    jon.triggerRightAction();
                }
                else if (gs->m_touchPointDown->getX() - SWIPE_WIDTH >= gs->m_touchPoint->getX())
                {
                    // Swipe Left
                    jon.triggerLeftAction();
                }
                else if (gs->m_touchPointDown->getY() + SWIPE_HEIGHT <= gs->m_touchPoint->getY())
                {
                    // Swipe Up
                    jon.triggerUpAction();
                }
                else if (gs->m_touchPointDown->getY() - SWIPE_HEIGHT >= gs->m_touchPoint->getY())
                {
                    // Swipe Down
                    jon.triggerDownAction();
                }
                else
                {
                    jon.triggerJump();
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                
                break;
        }
    }
    
    return false;
}

TestLevel::TestLevel() : m_sourceGame(nullptr), m_hasShownOpeningSequence(false), m_hasOpeningSequenceCompleted(false)
{
    m_game = std::unique_ptr<Game>(new Game());
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
}

/// Level Editor ///

LevelEditor * LevelEditor::getInstance()
{
    static LevelEditor *instance = new LevelEditor();
    
    return instance;
}

void LevelEditor::enter(GameScreen* gs)
{
    if (!m_game->isLoaded())
    {
        load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809},{\"x\":24,\"y\":19.9809},{\"x\":40,\"y\":19.9809}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623},{\"x\":24,\"y\":14.9623},{\"x\":40,\"y\":14.9623}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865},{\"x\":24,\"y\":5.63865},{\"x\":40,\"y\":5.63865}],\"jons\":[{\"x\":3.2,\"y\":10.1312}]}");
    }
    
    gs->m_renderer->init(RENDERER_TYPE_LEVEL_EDITOR);
}

void LevelEditor::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->renderWorld(*m_game);
        
        gs->m_renderer->renderJon(*m_game);
        
        if (m_lastAddedEntity != nullptr)
        {
            static Color highlight = Color(1, 1, 1, 0.25f);
            gs->m_renderer->renderEntityHighlighted(*m_lastAddedEntity, highlight);
        }
        
        if (m_draggingEntity != nullptr)
        {
            static Color highlight = Color(1, 1, 1, 0.5f);
            gs->m_renderer->renderEntityHighlighted(*m_draggingEntity, highlight);
            
            if (m_attachToEntity != nullptr)
            {
                gs->m_renderer->renderEntityHighlighted(*m_attachToEntity, highlight);
            }
        }
        
        if (m_levelEditorActionsPanel->isShowEntityBoundsRequested())
        {
            gs->m_renderer->renderBounds(*m_game);
        }
        
        gs->m_renderer->renderLevelEditor(*m_levelEditorActionsPanel, *m_levelEditorEntitiesPanel, *m_trashCan);
        
        gs->m_renderer->renderToScreen();
    }
    else
    {
        handleTouchInput(gs);
        
        int oldSum = m_game->calcSum();
        
        m_game->updateAndClean(gs->m_fDeltaTime, false);
        
        if (m_game->getJons().size() > 0)
        {
            Jon& jon = m_game->getJon();
            
            if (jon.isDead() || jon.getPosition().getX() - jon.getWidth() > m_game->getFarRight())
            {
                jon.requestDeletion();
            }
        }
        
        int newSum = m_game->calcSum();
        
        if (oldSum != newSum)
        {
            resetEntities(true);
        }
        
        m_trashCan->update(gs->m_renderer->getCameraPosition());
        
        EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
    }
}

void LevelEditor::exit(GameScreen* gs)
{
    // TODO
}

const char* LevelEditor::save()
{
    return m_game->save();
}

void LevelEditor::load(const char* json)
{
    m_game->load(json);
    
    resetEntities(true);
}

void LevelEditor::handleTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        int rc;
        if ((rc = m_levelEditorActionsPanel->handleTouch(*i, *gs->m_touchPoint)) != LEVEL_EDITOR_ACTIONS_PANEL_RC_UNHANDLED)
        {
            gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
            
            switch (rc)
            {
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_RESET:
                    m_game->reset();
                    enter(gs);
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_TEST:
                    if (m_game->getJons().size() > 0)
                    {
                        TestLevel::getInstance()->setSourceGame(m_game.get());
                        gs->m_stateMachine->changeState(TestLevel::getInstance());
                    }
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_UNDO:
                    if (m_addedEntities.size() > 0)
                    {
                        m_addedEntities.at(m_addedEntities.size() - 1)->requestDeletion();
                        m_addedEntities.pop_back();
                        
                        resetEntities(true);
                    }
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_LOAD:
                    resetEntities(true);
                    gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_EDITOR_LOAD;
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_SAVE:
                    if (m_game->getJons().size() > 0)
                    {
                        resetEntities(true);
                        gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_EDITOR_SAVE;
                    }
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED:
                default:
                    break;
            }
            
            continue;
        }
        
        if ((rc = m_levelEditorEntitiesPanel->handleTouch(*i, *gs->m_touchPoint, *m_game, gs->m_renderer->getCameraPosition(), &m_lastAddedEntity)) != LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED)
        {
            if (rc == LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED)
            {
                m_addedEntities.push_back(m_lastAddedEntity);
            }
            
            resetEntities(false);
            
            continue;
        }
        
        switch (i->getTouchType())
        {
            case DOWN:
            {
                Vector2D tp = gs->m_touchPoint->cpy();
                tp.mul(3);
                float camPosX = gs->m_renderer->getCameraPosition().getX();
                tp.add(camPosX, 0);
                
                m_isVerticalChangeAllowed = true;
                m_useYCorrection = false;
                m_allowAttachment = true;
                m_allowPlaceOn = true;
                m_fYOffset = 0;
                m_fDraggingEntityOriginalY = 0;
                
                int index = -1;
                if ((index = EntityUtils::isTouching(m_gameEntities, tp)) != -1)
                {
                    m_draggingEntity = m_gameEntities.at(index);
                    
                    if (dynamic_cast<Ground*>(m_draggingEntity))
                    {
                        m_isVerticalChangeAllowed = false;
                        m_fDraggingEntityOriginalY = m_draggingEntity->getPosition().getY();
                    }
                    else if (dynamic_cast<Hole*>(m_draggingEntity))
                    {
                        m_isVerticalChangeAllowed = false;
                        m_allowAttachment = false;
                        m_allowPlaceOn = false;
                        m_fDraggingEntityOriginalY = m_draggingEntity->getPosition().getY();
                    }
                    else if (dynamic_cast<GroundPlatform*>(m_draggingEntity))
                    {
                        m_useYCorrection = true;
                    }
                    else if (dynamic_cast<SideSpike*>(m_draggingEntity))
                    {
                        m_allowAttachment = false;
                        m_allowPlaceOn = false;
                    }
                    else if (dynamic_cast<Carrot*>(m_draggingEntity) || dynamic_cast<GoldenCarrot*>(m_draggingEntity))
                    {
                        m_fYOffset = 0.8f;
                    }
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
            }
                continue;
            case DRAGGED:
            {
                float xDelta = gs->m_touchPoint->getX() - gs->m_touchPointDown->getX();
                xDelta *= 3;
                float yDelta = gs->m_touchPoint->getY() - gs->m_touchPointDown->getY();
                yDelta *= 3;
                
                if (m_draggingEntity != nullptr)
                {
                    m_draggingEntity->getPosition().add(xDelta, yDelta);
                    m_draggingEntity->updateBounds();
                    
                    if (m_draggingEntity->getBounds().getLowerLeft().getX() < 0 && xDelta < 0)
                    {
                        m_draggingEntity->getPosition().sub(xDelta, 0);
                        m_draggingEntity->updateBounds();
                    }
                    
                    if (m_draggingEntity->getBounds().getLowerLeft().getY() < 0 && yDelta < 0)
                    {
                        m_draggingEntity->getPosition().sub(0, yDelta);
                        m_draggingEntity->updateBounds();
                    }
                    else if (m_draggingEntity->getBounds().getLowerLeft().getY() > GAME_HEIGHT && yDelta > 0)
                    {
                        m_draggingEntity->getPosition().sub(0, yDelta);
                        m_draggingEntity->updateBounds();
                    }
                    
                    int index = -1;
                    if ((index = EntityUtils::doRectanglesOverlap(m_game->getGrounds(), m_draggingEntity)) != -1)
                    {
                        m_attachToEntity = m_game->getGrounds().at(index).get();
                    }
                    else if ((index = EntityUtils::doRectanglesOverlap(m_game->getPlatforms(), m_draggingEntity)) != -1)
                    {
                        m_attachToEntity = m_game->getPlatforms().at(index).get();
                    }
                    else
                    {
                        m_attachToEntity = nullptr;
                    }
                    
                    m_trashCan->setHighlighted(OverlapTester::doRectanglesOverlap(m_draggingEntity->getBounds(), m_trashCan->getBounds()));
                    
                    if ((gs->m_touchPoint->getX() > (CAM_WIDTH * 0.666f) && xDelta > 0)
                        || (gs->m_touchPoint->getX() < (CAM_WIDTH * 0.333f) && xDelta < 0))
                    {
                        gs->m_renderer->moveCamera(xDelta);
                    }
                }
                else
                {
                    xDelta *= 3;
                    gs->m_renderer->moveCamera(-xDelta);
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
            }
                continue;
            case UP:
                if (m_draggingEntity != nullptr)
                {
                    if (OverlapTester::doRectanglesOverlap(m_draggingEntity->getBounds(), m_trashCan->getBounds()))
                    {
                        m_draggingEntity->requestDeletion();
                    }
                    else
                    {
                        if (m_attachToEntity != nullptr)
                        {
                            float draggingPosY = m_draggingEntity->getPosition().getY();
                            float draggingPosX = m_draggingEntity->getBounds().getLowerLeft().getX();
                            float attachToWidth = m_attachToEntity->getBounds().getWidth();
                            float attachToTop = m_attachToEntity->getBounds().getTop();
                            float attachToLeft = m_attachToEntity->getBounds().getLowerLeft().getX();
                            float attachToRight = attachToLeft + attachToWidth * 0.75f;
                            attachToLeft += attachToWidth * 0.25f;
                            
                            if (m_allowAttachment && (!m_isVerticalChangeAllowed || draggingPosY < attachToTop))
                            {
                                if (draggingPosX > attachToRight)
                                {
                                    EntityUtils::attach(*m_draggingEntity, *m_attachToEntity, false, m_useYCorrection);
                                }
                                else if (draggingPosX < attachToLeft)
                                {
                                    EntityUtils::attach(*m_draggingEntity, *m_attachToEntity, true, m_useYCorrection);
                                }
                            }
                            else if (m_isVerticalChangeAllowed && draggingPosY > attachToTop && m_allowPlaceOn)
                            {
                                EntityUtils::placeOn(*m_draggingEntity, *m_attachToEntity, m_fYOffset);
                            }
                        }
                        
                        if (!m_isVerticalChangeAllowed)
                        {
                            m_draggingEntity->getPosition().setY(m_fDraggingEntityOriginalY);
                            m_draggingEntity->updateBounds();
                        }
                    }
                }
                
                m_trashCan->setHighlighted(false);
                m_attachToEntity = nullptr;
                
                if (m_draggingEntity != nullptr && !m_isVerticalChangeAllowed)
                {
                    m_draggingEntity->getPosition().setY(m_fDraggingEntityOriginalY);
                    m_draggingEntity->updateBounds();
                }
                
                m_draggingEntity = nullptr;
                
                return;
        }
    }
}

void LevelEditor::resetEntities(bool clearLastAddedEntity)
{
    m_gameEntities.clear();
    
    EntityUtils::addAll(m_game->getTrees(), m_gameEntities);
    EntityUtils::addAll(m_game->getCaveSkeletons(), m_gameEntities);
    EntityUtils::addAll(m_game->getGrounds(), m_gameEntities);
    EntityUtils::addAll(m_game->getHoles(), m_gameEntities);
    EntityUtils::addAll(m_game->getLogVerticalTalls(), m_gameEntities);
    EntityUtils::addAll(m_game->getLogVerticalShorts(), m_gameEntities);
    EntityUtils::addAll(m_game->getThorns(), m_gameEntities);
    EntityUtils::addAll(m_game->getStumps(), m_gameEntities);
    EntityUtils::addAll(m_game->getSideSpikes(), m_gameEntities);
    EntityUtils::addAll(m_game->getUpwardSpikes(), m_gameEntities);
    EntityUtils::addAll(m_game->getJumpSprings(), m_gameEntities);
    EntityUtils::addAll(m_game->getRocks(), m_gameEntities);
    EntityUtils::addAll(m_game->getPlatforms(), m_gameEntities);
    EntityUtils::addAll(m_game->getCarrots(), m_gameEntities);
    EntityUtils::addAll(m_game->getGoldenCarrots(), m_gameEntities);
    EntityUtils::addAll(m_game->getEndSigns(), m_gameEntities);
	EntityUtils::addAll(m_game->getJons(), m_gameEntities);
    
    for (std::vector<PhysicalEntity*>::iterator i = m_addedEntities.begin(); i != m_addedEntities.end(); )
    {
        bool containedInGameEntities = false;
        for (std::vector<PhysicalEntity*>::iterator j = m_gameEntities.begin(); j != m_gameEntities.end(); j++)
        {
            if ((*i) == (*j))
            {
                containedInGameEntities = true;
                break;
            }
        }
        
        if (containedInGameEntities)
        {
            i++;
        }
        else
        {
            i = m_addedEntities.erase(i);
        }
    }
    
    if (m_draggingEntity != nullptr && !m_isVerticalChangeAllowed)
    {
        m_draggingEntity->getPosition().setY(m_fDraggingEntityOriginalY);
        m_draggingEntity->updateBounds();
    }
    
    m_draggingEntity = nullptr;
    m_attachToEntity = nullptr;
    
    if (clearLastAddedEntity)
    {
        m_lastAddedEntity = nullptr;
    }
}

LevelEditor::LevelEditor() : m_lastAddedEntity(nullptr), m_draggingEntity(nullptr), m_attachToEntity(nullptr), m_fDraggingEntityOriginalY(0), m_isVerticalChangeAllowed(true), m_useYCorrection(false), m_allowAttachment(true), m_allowPlaceOn(true), m_fYOffset(0)
{
    m_game = std::unique_ptr<Game>(new Game());
    m_levelEditorActionsPanel = std::unique_ptr<LevelEditorActionsPanel>(new LevelEditorActionsPanel());
    m_levelEditorEntitiesPanel = std::unique_ptr<LevelEditorEntitiesPanel>(new LevelEditorEntitiesPanel());
    m_trashCan = std::unique_ptr<TrashCan>(new TrashCan());
}