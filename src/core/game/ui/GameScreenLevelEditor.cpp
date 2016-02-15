//
//  GameScreenLevelEditor.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "GameScreenLevelEditor.h"
#include "GameScreenLevels.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenTitle.h"

#include <algorithm>    // std::sort

bool sortGrounds(Ground* i, Ground* j)
{
    return i->getType() < j->getType();
}

bool sortGameEntities(Entity* i, Entity* j)
{
    return i->getID() < j->getID();
}

GameScreenLevelEditor * GameScreenLevelEditor::getInstance()
{
    static GameScreenLevelEditor *instance = new GameScreenLevelEditor();
    
    return instance;
}

void GameScreenLevelEditor::enter(GameScreen* gs)
{
    gs->m_stateMachine->setPreviousState(Title::getInstance());
    
    if (!m_game->isLoaded())
    {
        load("{\"jons\":[{\"gridX\":200,\"gridY\":200}]}");
    }
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1);
    gs->m_renderer->zoomOut();
}

void GameScreenLevelEditor::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame();
        
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
        
        if (m_levelEditorActionsPanel->showBounds())
        {
            gs->m_renderer->renderBounds(*m_game, m_levelEditorActionsPanel->boundsLevelRequested());
        }
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->renderLevelEditor(*m_levelEditorActionsPanel, *m_levelEditorEntitiesPanel, *m_trashCan, *m_levelSelectorPanel);
        
        gs->m_renderer->endFrame();
    }
    else
    {
        handleTouchInput(gs);
        
        int oldSum = m_game->calcSum();
        
        m_game->update(gs->m_fDeltaTime);
        m_game->updateAndClean(gs->m_fDeltaTime);
        
        if (m_game->getJons().size() > 1)
        {
            for (std::vector<Jon *>::iterator i = m_game->getJons().begin(); i != m_game->getJons().end(); )
            {
                if ((*i)->isDead())
                {
                    (*i)->onDeletion();
                    
                    delete *i;
                    i = m_game->getJons().erase(i);
                }
                else
                {
                    i++;
                }
            }
        }
        
        int newSum = m_game->calcSum();
        
        if (oldSum != newSum)
        {
            resetEntities(true);
        }
        
        m_trashCan->update(gs->m_renderer->getCameraPosition());
        
        EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
        EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
        EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
        EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
    }
}

void GameScreenLevelEditor::exit(GameScreen* gs)
{
    // TODO
}

const char* GameScreenLevelEditor::save()
{
    return m_game->save();
}

void GameScreenLevelEditor::load(const char* json)
{
    m_game->load(json);
    
    resetEntities(true);
}

Game& GameScreenLevelEditor::getGame()
{
    return *m_game;
}

LevelEditorActionsPanel& GameScreenLevelEditor::getLevelEditorActionsPanel()
{
    return *m_levelEditorActionsPanel;
}

LevelEditorEntitiesPanel& GameScreenLevelEditor::getLevelEditorEntitiesPanel()
{
    return *m_levelEditorEntitiesPanel;
}

TrashCan& GameScreenLevelEditor::getTrashCan()
{
    return *m_trashCan;
}

LevelSelectorPanel& GameScreenLevelEditor::getLevelSelectorPanel()
{
    return *m_levelSelectorPanel;
}

#pragma mark private

void GameScreenLevelEditor::handleTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        int rc;
        if (m_levelSelectorPanel->isOpen())
        {
            if ((rc = m_levelSelectorPanel->handleTouch(*i, *gs->m_touchPoint)) != LEVEL_SELECTOR_PANEL_RC_UNHANDLED)
            {
                switch (rc)
                {
                    case LEVEL_SELECTOR_PANEL_RC_CONFIRM_SAVE:
                        resetEntities(true);
                        gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_EDITOR_SAVE * 1000;
                        gs->m_iRequestedAction += (m_levelSelectorPanel->getWorld() * 100);
                        gs->m_iRequestedAction += m_levelSelectorPanel->getLevel();
                        break;
                    case LEVEL_SELECTOR_PANEL_RC_CONFIRM_LOAD:
                        resetEntities(true);
                        gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_EDITOR_LOAD * 1000;
                        gs->m_iRequestedAction += (m_levelSelectorPanel->getWorld() * 100);
                        gs->m_iRequestedAction += m_levelSelectorPanel->getLevel();
                        break;
                    case LEVEL_SELECTOR_PANEL_RC_HANDLED:
                    default:
                        break;
                }
            }
            
            return;
        }
        
        if ((rc = m_levelEditorActionsPanel->handleTouch(*i, *gs->m_touchPoint)) != LEVEL_EDITOR_ACTIONS_PANEL_RC_UNHANDLED)
        {
            gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
            
            bool isLevelValid = true;
            if (m_game->getJons().size() == 0)
            {
                isLevelValid = false;
            }
            
            if (isLevelValid && m_game->getJon().getBounds().getRight() > m_game->getFarRight())
            {
                isLevelValid = false;
            }
            
            switch (rc)
            {
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_RESET:
                    m_game->reset();
                    enter(gs);
					return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_EXIT:
                    gs->m_stateMachine->revertToPreviousState();
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_TEST:
                    if (isLevelValid)
                    {
                        Level::getInstance()->setSourceGame(m_game.get());
                        gs->m_stateMachine->changeState(Level::getInstance());
						return;
                    }
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_LOAD:
                    m_levelSelectorPanel->openForMode(LEVEL_SELECTOR_PANEL_MODE_LOAD);
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_SAVE:
                    if (isLevelValid)
                    {
                        m_levelSelectorPanel->openForMode(LEVEL_SELECTOR_PANEL_MODE_SAVE);
                    }
                    return;
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
                if (dynamic_cast<Jon*>(m_lastAddedEntity))
                {
                    Jon* entity = dynamic_cast<Jon *>(m_lastAddedEntity);
                    entity->setGame(m_game.get());
                }
                else if (dynamic_cast<Enemy *>(m_lastAddedEntity))
                {
                    Enemy* entity = dynamic_cast<Enemy *>(m_lastAddedEntity);
                    entity->setGame(m_game.get());
                }
            }
            
            resetEntities(false);
            
            continue;
        }
        
        switch (i->getTouchType())
        {
            case DOWN:
            {
                Vector2D tp = gs->m_touchPoint->cpy();
                tp.mul(4);
                float camPosX = gs->m_renderer->getCameraPosition().getX();
                tp.add(camPosX, 0);
                
                m_isVerticalChangeAllowed = true;
                m_fDraggingEntityOriginalY = 0;
                m_allowPlaceOn = true;
                m_allowPlaceUnder = false;
                
                int index = -1;
                if ((index = EntityUtils::isTouching(m_gameEntities, tp)) != -1)
                {
                    m_draggingEntity = m_gameEntities.at(index);
                    
                    if (dynamic_cast<Midground *>(m_draggingEntity)
                        || dynamic_cast<Ground *>(m_draggingEntity)
                        || dynamic_cast<ExitGround *>(m_draggingEntity)
                        || dynamic_cast<Hole *>(m_draggingEntity))
                    {
                        m_isVerticalChangeAllowed = false;
                        m_allowPlaceOn = false;
                        m_fDraggingEntityOriginalY = m_draggingEntity->getPosition().getY();
                    }
                    else if (dynamic_cast<CollectibleItem *>(m_draggingEntity))
                    {
                        m_allowPlaceOn = false;
                    }
                    else if (dynamic_cast<MushroomCeiling *>(m_draggingEntity) || dynamic_cast<DeathFromAboveObject *>(m_draggingEntity))
                    {
                        m_allowPlaceUnder = true;
						m_allowPlaceOn = false;
                    }
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                gs->m_touchPointDown2->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
            }
                continue;
            case DRAGGED:
            {
                float xDelta = gs->m_touchPoint->getX() - gs->m_touchPointDown->getX();
                xDelta *= 4;
                float yDelta = gs->m_touchPoint->getY() - gs->m_touchPointDown->getY();
                yDelta *= 4;
                
                if (m_draggingEntity != nullptr)
                {
                    m_draggingEntity->getPosition().add(xDelta, yDelta);
                    m_draggingEntity->updateBounds();
                    
                    if (m_draggingEntity->getBounds().getLeft() < 0 && xDelta < 0)
                    {
                        m_draggingEntity->getPosition().sub(xDelta, 0);
                        m_draggingEntity->updateBounds();
                    }
                    
                    if (m_draggingEntity->getBounds().getLowerLeft().getY() < 0 && yDelta < 0)
                    {
                        m_draggingEntity->getPosition().sub(0, yDelta);
                        m_draggingEntity->updateBounds();
                    }
                    else if (m_draggingEntity->getBounds().getTop() > GAME_HEIGHT && yDelta > 0)
                    {
                        m_draggingEntity->getPosition().sub(0, yDelta);
                        m_draggingEntity->updateBounds();
                    }
                    
                    int index = -1;
                    if (m_allowPlaceOn)
                    {
                        if ((index = EntityUtils::indexOfOverlappingObjectThatCanBePlacedOn(m_draggingEntity, m_game->getGrounds())) != -1)
                        {
                            m_attachToEntity = m_game->getGrounds().at(index);
                        }
                        else if ((index = EntityUtils::indexOfOverlappingObjectThatCanBePlacedOn(m_draggingEntity, m_game->getExitGrounds())) != -1)
                        {
                            m_attachToEntity = m_game->getExitGrounds().at(index);
                        }
                        else if ((index = EntityUtils::indexOfOverlappingObjectThatCanBePlacedOn(m_draggingEntity, m_game->getForegroundObjects())) != -1)
                        {
                            m_attachToEntity = m_game->getForegroundObjects().at(index);
                        }
                    }
                    else if (m_allowPlaceUnder)
                    {
                        if ((index = EntityUtils::indexOfOverlappingObjectThatCanBePlacedUnder(m_draggingEntity, m_game->getGrounds())) != -1)
                        {
                            m_attachToEntity = m_game->getGrounds().at(index);
                        }
                    }
                    
                    if (index == -1)
                    {
                        m_attachToEntity = nullptr;
                    }
                    
                    m_trashCan->setHighlighted(OverlapTester::doRectanglesOverlap(m_draggingEntity->getBounds(), m_trashCan->getBounds()));
                    
                    if ((gs->m_touchPoint->getX() > (CAM_WIDTH * 0.8f) && xDelta > 0)
                        || (gs->m_touchPoint->getX() < (CAM_WIDTH * 0.2f) && xDelta < 0))
                    {
                        gs->m_renderer->moveCamera(xDelta);
                    }
                }
                else
                {
                    xDelta *= 4;
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
                        bool safeToDelete = true;
                        if (dynamic_cast<Jon*>(m_draggingEntity) && m_game->getJons().size() == 1)
                        {
                            safeToDelete = false;
                        }
                        
                        if (safeToDelete)
                        {
                            m_draggingEntity->requestDeletion();
                        }
                    }
                    else
                    {
						bool needsSnapToGrid = true;
                        if (m_attachToEntity != nullptr)
                        {
							if (m_allowPlaceOn)
							{
								EntityUtils::placeOn(*m_draggingEntity, *m_attachToEntity, 1);
								needsSnapToGrid = false;
							}
							else if (m_allowPlaceUnder)
							{
								EntityUtils::placeUnder(*m_draggingEntity, *m_attachToEntity, 1);
								needsSnapToGrid = false;
							}
                        }
                        
                        if (!m_isVerticalChangeAllowed)
                        {
                            m_draggingEntity->getPosition().setY(m_fDraggingEntityOriginalY);
                            m_draggingEntity->updateBounds();
                        }
                        
						if (needsSnapToGrid)
						{
							m_draggingEntity->snapToGrid(m_levelEditorActionsPanel->boundsLevelRequested());
						}
                    }
                }
                
                m_trashCan->setHighlighted(false);
                
                if (m_draggingEntity != nullptr && !m_isVerticalChangeAllowed)
                {
                    m_draggingEntity->getPosition().setY(m_fDraggingEntityOriginalY);
                    m_draggingEntity->updateBounds();
                }
                
                m_draggingEntity = nullptr;
                m_attachToEntity = nullptr;
                
                return;
        }
    }
}

void GameScreenLevelEditor::resetEntities(bool clearLastAddedEntity)
{
    m_gameEntities.clear();
    
    std::sort(m_game->getGrounds().begin(), m_game->getGrounds().end(), sortGrounds);
    
    EntityUtils::addAll(m_game->getMidgrounds(), m_gameEntities);
    EntityUtils::addAll(m_game->getGrounds(), m_gameEntities);
    EntityUtils::addAll(m_game->getExitGrounds(), m_gameEntities);
    EntityUtils::addAll(m_game->getHoles(), m_gameEntities);
    EntityUtils::addAll(m_game->getForegroundObjects(), m_gameEntities);
    EntityUtils::addAll(m_game->getEnemies(), m_gameEntities);
    EntityUtils::addAll(m_game->getCollectibleItems(), m_gameEntities);
	EntityUtils::addAll(m_game->getJons(), m_gameEntities);
    
    std::sort(m_gameEntities.begin(), m_gameEntities.end(), sortGameEntities);
    
    m_game->calcFarRight();
    
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

GameScreenLevelEditor::GameScreenLevelEditor() : m_lastAddedEntity(nullptr), m_draggingEntity(nullptr), m_attachToEntity(nullptr), m_fDraggingEntityOriginalY(0), m_isVerticalChangeAllowed(true), m_allowPlaceOn(true), m_allowPlaceUnder(false)
{
    m_game = std::unique_ptr<Game>(new Game(1337));
    m_levelEditorActionsPanel = std::unique_ptr<LevelEditorActionsPanel>(new LevelEditorActionsPanel());
    m_levelEditorEntitiesPanel = std::unique_ptr<LevelEditorEntitiesPanel>(new LevelEditorEntitiesPanel());
    m_trashCan = std::unique_ptr<TrashCan>(new TrashCan());
    m_levelSelectorPanel = std::unique_ptr<LevelSelectorPanel>(new LevelSelectorPanel());
}
