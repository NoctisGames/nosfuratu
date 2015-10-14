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

/// Game Play ///

GamePlay * GamePlay::getInstance()
{
    static GamePlay *instance = new GamePlay();
    
    return instance;
}

void GamePlay::enter(GameScreen* gs)
{
    if (m_sourceGame == nullptr)
    {
        m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809},{\"x\":24,\"y\":19.9809},{\"x\":40,\"y\":19.9809}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623},{\"x\":24,\"y\":14.9623},{\"x\":40,\"y\":14.9623}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865},{\"x\":24,\"y\":5.63865},{\"x\":40,\"y\":5.63865}],\"trees\":[],\"caveSkeletons\":[],\"grounds\":[{\"x\":0.890625,\"y\":8.90251,\"type\":3},{\"x\":13.7444,\"y\":8.90251,\"type\":1},{\"x\":26.5397,\"y\":8.90251,\"type\":4},{\"x\":36.5859,\"y\":8.90251,\"type\":3},{\"x\":43.4514,\"y\":8.90251,\"type\":2},{\"x\":50.2585,\"y\":8.90251,\"type\":4},{\"x\":62.0054,\"y\":5.11222,\"type\":8},{\"x\":68.7656,\"y\":5.11222,\"type\":7},{\"x\":75.5726,\"y\":5.11222,\"type\":9}],\"logVerticalTalls\":[],\"logVerticalShorts\":[],\"thorns\":[],\"stumps\":[],\"sideSpikes\":[],\"upwardSpikes\":[],\"jumpSprings\":[],\"rocks\":[],\"platforms\":[],\"endSigns\":[{\"x\":75.2344,\"y\":9.59272}],\"carrots\":[{\"x\":6.98438,\"y\":10.3108},{\"x\":10.1953,\"y\":10.3108},{\"x\":12.9609,\"y\":10.3108},{\"x\":16.1484,\"y\":10.3108},{\"x\":20.3438,\"y\":10.3108},{\"x\":39.6323,\"y\":10.3108},{\"x\":43.125,\"y\":10.3108},{\"x\":45.8438,\"y\":10.3108},{\"x\":65.9543,\"y\":10.3108},{\"x\":67.0078,\"y\":10.3108},{\"x\":68.1562,\"y\":10.3108},{\"x\":69.3516,\"y\":10.3108},{\"x\":70.7812,\"y\":10.3108},{\"x\":72.375,\"y\":10.3108}],\"goldenCarrots\":[],\"jons\":[{\"x\":3.2,\"y\":10.1312}]}");
    }
    else
    {
        m_game->copy(m_sourceGame);
    }
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1);
}

void GamePlay::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->renderWorld(*m_game);
        
        gs->m_renderer->renderJon(*m_game);
        
        if (gs->m_isLevelEditor)
        {
            gs->m_renderer->renderBackButton(*m_backButton);
        }
        
        gs->m_renderer->renderToScreen(*m_game);
    }
    else
    {
        if (handleTouchInput(gs))
        {
            return;
        }
        
        Jon& jon = m_game->getJon();
        
        jon.update(gs->m_fDeltaTime, *m_game);
        
        if (EntityUtils::isCollected(jon, m_game->getCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
        }
        
        if (EntityUtils::isCollected(jon, m_game->getGoldenCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
        }
        
        m_game->updateAndClean(gs->m_fDeltaTime);
        
        gs->m_renderer->updateCameraToFollowJon(jon, *m_game, gs->m_fDeltaTime);
        
        EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
        
        if (jon.isDead() || jon.getPosition().getX() - jon.getWidth() > m_game->getFarRight())
        {
            gs->init();
        }
    }
}

void GamePlay::exit(GameScreen* gs)
{
    // TODO
}

void GamePlay::setSourceGame(Game* game)
{
    m_sourceGame = game;
}

bool GamePlay::handleTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    Jon& jon = m_game->getJon();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        switch (i->getTouchType())
        {
            case DOWN:
                if (gs->m_isLevelEditor)
                {
                    if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getBounds()))
                    {
                        gs->m_stateMachine->changeState(LevelEditor::getInstance());
                        return true;
                    }
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

GamePlay::GamePlay() : m_sourceGame(nullptr)
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
        m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809},{\"x\":24,\"y\":19.9809},{\"x\":40,\"y\":19.9809}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623},{\"x\":24,\"y\":14.9623},{\"x\":40,\"y\":14.9623}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865},{\"x\":24,\"y\":5.63865},{\"x\":40,\"y\":5.63865}],\"jons\":[{\"x\":3.2,\"y\":10.1312}]}");
    }
    
    gs->m_renderer->init(RENDERER_TYPE_LEVEL_EDITOR);
}

void LevelEditor::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->renderWorld(*m_game);
        
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
        
        gs->m_renderer->renderToScreen(*m_game);
    }
    else
    {
        handleTouchInput(gs);
        
        int oldSum = m_game->calcSum();
        
        m_game->updateAndClean(gs->m_fDeltaTime);
        
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

Game& LevelEditor::getGame()
{
    return *m_game;
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
            switch (rc)
            {
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_RESET:
                    m_game->reset();
                    enter(gs);
                    resetEntities(true);
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_TEST:
                    GamePlay::getInstance()->setSourceGame(m_game.get());
                    gs->m_stateMachine->changeState(GamePlay::getInstance());
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_LOAD:
                    resetEntities(true);
                    gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_EDITOR_LOAD;
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_SAVE:
                    resetEntities(true);
                    gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_EDITOR_SAVE;
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED:
                default:
                    break;
            }
            
            continue;
        }
        
        if ((rc = m_levelEditorEntitiesPanel->handleTouch(*i, *gs->m_touchPoint, *m_game, gs->m_renderer->getCameraPosition(), &m_lastAddedEntity)) != LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED)
        {
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
                
                int index = -1;
                if ((index = EntityUtils::isTouching(m_gameEntities, tp)) != -1)
                {
                    m_draggingEntity = m_gameEntities.at(index);
                    
                    if (dynamic_cast<Ground*>(m_draggingEntity))
                    {
                        m_isVerticalChangeAllowed = false;
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
                            float attachToHeight = m_attachToEntity->getBounds().getHeight();
                            float attachToTop = m_attachToEntity->getBounds().getLowerLeft().getY() + attachToHeight;
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
                m_draggingEntity = nullptr;
                m_attachToEntity = nullptr;
                
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