//
//  GameScreenLevelEditor.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenLevelEditor.h"
#include "GameScreenLevels.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenTitle.h"

#include <algorithm> // std::sort

#include <sstream>

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
    
    loadIfNecessary(gs);
    
    gs->m_renderer->init(RENDERER_TYPE_LEVEL_EDITOR);
    gs->m_renderer->zoomOut();
    
    if (m_iWorld == 0)
    {
        m_levelSelectorPanel->open();
    }
}

void GameScreenLevelEditor::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderWorld(*m_game);
        
        gs->m_renderer->renderJonAndExtraForegroundObjects(*m_game);
        
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
        
        gs->m_renderer->renderMarkers(*m_game);
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->renderLevelEditor(this);
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->endFrame();
    }
    else
    {
        handleTouchInput(gs);
        
        int oldSum = m_game->calcSum();
        
        m_game->update(gs->m_fDeltaTime);

		if (m_game->getJons().size() >= 1)
		{
			m_game->updateAndClean(gs->m_fDeltaTime);
		}
        
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
	m_draggingEntity = nullptr;
	m_attachToEntity = nullptr;
	m_lastAddedEntity = nullptr;
}

const char* GameScreenLevelEditor::save()
{
    return m_game->save();
}

void GameScreenLevelEditor::load(const char* json, GameScreen* gs)
{
    m_game->load(json);
    
    m_game->setCameraBounds(&gs->m_renderer->getCameraBounds());
    
    resetEntities(true);
}

Game& GameScreenLevelEditor::getGame()
{
    return *m_game;
}

LevelEditorActionsPanel* GameScreenLevelEditor::getLevelEditorActionsPanel()
{
    return m_levelEditorActionsPanel.get();
}

LevelEditorEntitiesPanel* GameScreenLevelEditor::getLevelEditorEntitiesPanel()
{
    return m_levelEditorEntitiesPanel.get();
}

TrashCan* GameScreenLevelEditor::getTrashCan()
{
    return m_trashCan.get();
}

LevelSelectorPanel* GameScreenLevelEditor::getLevelSelectorPanel()
{
    return m_levelSelectorPanel.get();
}

OffsetPanel* GameScreenLevelEditor::getOffsetPanel()
{
	return m_offsetPanel.get();
}

ConfirmResetPanel* GameScreenLevelEditor::getConfirmResetPanel()
{
	return m_confirmResetPanel.get();
}

ConfirmExitPanel* GameScreenLevelEditor::getConfirmExitPanel()
{
	return m_confirmExitPanel.get();
}

#pragma mark private

void GameScreenLevelEditor::handleTouchInput(GameScreen* gs)
{
    for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld(*(*i));
        
        int rc;
        if (m_levelSelectorPanel->isOpen())
        {
            if ((rc = m_levelSelectorPanel->handleTouch(*(*i), *gs->m_touchPoint)) != LEVEL_SELECTOR_PANEL_RC_UNHANDLED)
            {
                switch (rc)
                {
                    case LEVEL_SELECTOR_PANEL_RC_CONFIRM:
                    {
						RendererType oldRendererType = calcRendererTypeFromLevel(m_iWorld, m_iLevel);
                        
                        m_iWorld = m_levelSelectorPanel->getWorld();
                        m_iLevel = m_levelSelectorPanel->getLevel();

						RendererType newRendererType = calcRendererTypeFromLevel(m_iWorld, m_iLevel);

                        if (oldRendererType != newRendererType)
                        {
                            gs->m_renderer->unload(oldRendererType);
							gs->m_renderer->load(newRendererType);
                        }
                        
                        m_levelEditorEntitiesPanel->initForLevel(m_iWorld, m_iLevel);
                        
                        m_game->reset();
                        
                        loadIfNecessary(gs);
                        
                        resetEntities(true);
                        
                        gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_EDITOR_LOAD * 1000;
                        gs->m_iRequestedAction += m_iWorld * 100;
                        gs->m_iRequestedAction += m_iLevel;
                    }
                        break;
                    case LEVEL_SELECTOR_PANEL_RC_HANDLED:
                    default:
                        break;
                }
            }
            
            return;
        }
        
        if (m_offsetPanel->isOpen())
        {
            if ((rc = m_offsetPanel->handleTouch(*(*i), *gs->m_touchPoint)) != OFFSET_PANEL_RC_UNHANDLED)
            {
                switch (rc)
                {
                    case OFFSET_PANEL_RC_HANDLED:
                    {
						int rawOffset = m_offsetPanel->getOffset();

						int offset = rawOffset > m_iLastOffset ? 1 : -1;

						m_iLastOffset = rawOffset;
                        
                        Marker* endLoopMarker = m_game->getMarkers().at(m_game->getMarkers().size() - 1);
                        Marker* beginLoopMarker = m_game->getMarkers().at(m_game->getMarkers().size() - 2);
                        
                        int beginGridX = beginLoopMarker->getGridX();
                        int endGridX = endLoopMarker->getGridX();
                        
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getMidgrounds(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getGrounds(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getPits(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getExitGrounds(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getHoles(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getForegroundObjects(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getMidBossForegroundObjects(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getCountHissWithMinas(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getEnemies(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getCollectibleItems(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getExtraForegroundObjects(), beginGridX, endGridX, offset);
                        
                        EntityUtils::offsetAllInRangeClosedEnd(m_game->getMarkers(), beginGridX, endGridX, offset);
                    }
                        break;
                    case OFFSET_PANEL_RC_CONFIRM:
						m_iLastOffset = 0;
                    default:
                        break;
                }
            }
            
            return;
        }
        
        if (m_confirmResetPanel->isOpen())
        {
            if ((rc = m_confirmResetPanel->handleTouch(*(*i), *gs->m_touchPoint)) != CONFIRM_RESET_PANEL_RC_UNHANDLED)
            {
                switch (rc)
                {
                    case CONFIRM_RESET_PANEL_RC_RESET:
                    {
                        m_game->reset();
                        resetEntities(true);
                        enter(gs);
                    }
                        break;
                    case CONFIRM_RESET_PANEL_RC_CANCEL:
                    default:
                        break;
                }
            }
            
            return;
        }
        
        if (m_confirmExitPanel->isOpen())
        {
            if ((rc = m_confirmExitPanel->handleTouch(*(*i), *gs->m_touchPoint)) != CONFIRM_EXIT_PANEL_RC_UNHANDLED)
            {
                switch (rc)
                {
                    case CONFIRM_EXIT_PANEL_RC_EXIT:
                    {
                        m_iWorld = 0;
                        m_iLevel = 0;
                        gs->m_stateMachine->revertToPreviousState();
                    }
                        break;
                    case CONFIRM_EXIT_PANEL_RC_CANCEL:
                    default:
                        break;
                }
            }
            
            return;
        }
        
        if ((rc = m_levelEditorActionsPanel->handleTouch(*(*i), *gs->m_touchPoint)) != LEVEL_EDITOR_ACTIONS_PANEL_RC_UNHANDLED)
        {
            gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
            
            switch (rc)
            {
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_MARKER:
                {
                    int gridX = (gs->m_renderer->getCameraPosition().getX() + ZOOMED_OUT_CAM_WIDTH / 2) / GRID_CELL_SIZE;
                    int type = 0; // begin loop
                    if (m_game->getMarkers().size() == 1 || m_game->getMarkers().size() % 2 == 1)
                    {
                        type = 1; // end loop
                    }
                    Marker* m = Marker::create(gridX, 0, type);
                    m_game->getMarkers().push_back(m);
                    resetEntities(true);
                }
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_OFFSET:
                    if (m_game->getMarkers().size() < 2)
                    {
                        gs->m_iRequestedAction = REQUESTED_ACTION_SHOW_MESSAGE * 1000 + MESSAGE_OFFSET_NEEDS_MARKERS_KEY;
                    }
                    else
                    {
                        m_offsetPanel->open();
                    }
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_RESET:
                    m_confirmResetPanel->open();
					return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_EXIT:
                    m_confirmExitPanel->open();
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_TEST:
                    if (isLevelValid(gs))
                    {
                        Level::getInstance()->setSourceGame(m_game.get());
						Level::getInstance()->setBestStats(0, 0, 0, 0, FLAG_ABILITY_ALL);
                        gs->m_stateMachine->changeState(Level::getInstance());
						return;
                    }
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_LOAD:
                    m_levelSelectorPanel->open();
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_SAVE:
                    if (isLevelValid(gs))
                    {
                        resetEntities(true);
                        gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_EDITOR_SAVE * 1000;
                        gs->m_iRequestedAction += m_iWorld * 100;
                        gs->m_iRequestedAction += m_iLevel;
                    }
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED:
                default:
                    break;
            }
            
            return;
        }
        
        if ((rc = m_levelEditorEntitiesPanel->handleTouch(*(*i), *gs->m_touchPoint, *m_game, gs->m_renderer->getCameraPosition(), &m_lastAddedEntity)) != LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED)
        {
            if (rc == LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED)
            {
                if (dynamic_cast<Jon*>(m_lastAddedEntity))
                {
                    Jon* jon = dynamic_cast<Jon *>(m_lastAddedEntity);
					m_game->setCameraBounds(&gs->m_renderer->getCameraBounds());
                    jon->setGame(m_game.get());
                }
                else if (dynamic_cast<Enemy *>(m_lastAddedEntity))
                {
                    Enemy* e = dynamic_cast<Enemy *>(m_lastAddedEntity);
					m_game->setCameraBounds(&gs->m_renderer->getCameraBounds());
                    e->setGame(m_game.get());
                }
                else if (dynamic_cast<ForegroundObject *>(m_lastAddedEntity))
                {
                    ForegroundObject* fo = dynamic_cast<ForegroundObject *>(m_lastAddedEntity);
                    m_game->setCameraBounds(&gs->m_renderer->getCameraBounds());
                    fo->setGame(m_game.get());
                }
            }
            
            resetEntities(false);
            
            return;
        }
        
        switch ((*i)->getTouchType())
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
                
				CollectibleItem *lastAddedCollectibleItem;
                int index = -1;
                if ((index = EntityUtils::isTouching(m_gameEntities, tp)) != -1)
                {
                    m_draggingEntity = m_gameEntities.at(index);
                    
                    if (dynamic_cast<Midground *>(m_draggingEntity)
                        || dynamic_cast<Ground *>(m_draggingEntity)
                        || dynamic_cast<ExitGround *>(m_draggingEntity)
                        || dynamic_cast<Hole *>(m_draggingEntity)
                        || dynamic_cast<SpikeTower *>(m_draggingEntity)
                        || dynamic_cast<VerticalSaw *>(m_draggingEntity)
						|| dynamic_cast<Marker *>(m_draggingEntity))
                    {
                        m_isVerticalChangeAllowed = false;
                        m_allowPlaceOn = false;
                        m_fDraggingEntityOriginalY = m_draggingEntity->getPosition().getY();
                    }
                    else if (dynamic_cast<RunningIntoDeathObject *>(m_draggingEntity))
                    {
                        m_allowPlaceOn = false;
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
				else if (m_lastAddedEntity && (lastAddedCollectibleItem = dynamic_cast<CollectibleItem *>(m_lastAddedEntity)))
				{
					int gridX = tp.getX() / GRID_CELL_SIZE;
					int gridY = tp.getY() / GRID_CELL_SIZE;
					CollectibleItem* collectibleItem = CollectibleItem::create(gridX, gridY, lastAddedCollectibleItem->getType());
					m_game->getCollectibleItems().push_back(collectibleItem);

					m_lastAddedEntity = collectibleItem;

					resetEntities(false);
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
                    
                    if (m_draggingEntity->getMainBounds().getLeft() < 0 && xDelta < 0)
                    {
                        m_draggingEntity->getPosition().sub(xDelta, 0);
                        m_draggingEntity->updateBounds();
                    }
                    
                    if (m_draggingEntity->getMainBounds().getLowerLeft().getY() < 0 && yDelta < 0)
                    {
                        m_draggingEntity->getPosition().sub(0, yDelta);
                        m_draggingEntity->updateBounds();
                    }
                    else if (m_draggingEntity->getMainBounds().getTop() > GAME_HEIGHT && yDelta > 0)
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
                    
                    m_trashCan->setHighlighted(OverlapTester::doRectanglesOverlap(m_draggingEntity->getMainBounds(), m_trashCan->getMainBounds()));
                    
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
                    if (OverlapTester::doRectanglesOverlap(m_draggingEntity->getMainBounds(), m_trashCan->getMainBounds()))
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
	m_draggingEntity = nullptr;
	m_attachToEntity = nullptr;

	if (clearLastAddedEntity)
	{
		m_lastAddedEntity = nullptr;
	}

    m_gameEntities.clear();
    
    std::sort(m_game->getGrounds().begin(), m_game->getGrounds().end(), sortGrounds);
    
    EntityUtils::addAll(m_game->getMidgrounds(), m_gameEntities);
    EntityUtils::addAll(m_game->getGrounds(), m_gameEntities);
    EntityUtils::addAll(m_game->getPits(), m_gameEntities);
    EntityUtils::addAll(m_game->getExitGrounds(), m_gameEntities);
    EntityUtils::addAll(m_game->getHoles(), m_gameEntities);
    EntityUtils::addAll(m_game->getForegroundObjects(), m_gameEntities);
    EntityUtils::addAll(m_game->getMidBossForegroundObjects(), m_gameEntities);
    EntityUtils::addAll(m_game->getCountHissWithMinas(), m_gameEntities);
    EntityUtils::addAll(m_game->getEnemies(), m_gameEntities);
    EntityUtils::addAll(m_game->getCollectibleItems(), m_gameEntities);
	EntityUtils::addAll(m_game->getJons(), m_gameEntities);
    EntityUtils::addAll(m_game->getExtraForegroundObjects(), m_gameEntities);
    EntityUtils::addAll(m_game->getMarkers(), m_gameEntities);
    
    std::sort(m_gameEntities.begin(), m_gameEntities.end(), sortGameEntities);
    
    m_game->calcFarRight();
}

void GameScreenLevelEditor::loadIfNecessary(GameScreen* gs)
{
    if (!m_game->isLoaded())
    {
        std::stringstream ss;
        ss << "{\"world\":" << m_iWorld << ",\"level\":" << m_iLevel << ", \"jons\":[{\"gridX\":200,\"gridY\":200}]}";
        std::string jsonString = ss.str();
        
        load(jsonString.c_str(), gs);
    }
}

bool GameScreenLevelEditor::isLevelValid(GameScreen *gs)
{
    if (!m_game->hasEndSign())
    {
        gs->m_iRequestedAction = REQUESTED_ACTION_SHOW_MESSAGE * 1000 + MESSAGE_NO_END_SIGN_KEY;
        return false;
    }
    
    if (m_game->getJons().size() == 0)
    {
        gs->m_iRequestedAction = REQUESTED_ACTION_SHOW_MESSAGE * 1000 + MESSAGE_NO_JON_KEY;
        return false;
    }
    
    if (m_game->getJon().getMainBounds().getRight() > m_game->getFarRight())
    {
        gs->m_iRequestedAction = REQUESTED_ACTION_SHOW_MESSAGE * 1000 + MESSAGE_INVALID_JON_KEY;
        return false;
    }
    
    if (m_game->getLevel() < 21)
    {
        if (m_game->getCountHissWithMinas().size() == 0)
        {
            gs->m_iRequestedAction = REQUESTED_ACTION_SHOW_MESSAGE * 1000 + MESSAGE_NO_COUNT_HISS_KEY;
            return false;
        }
        
        if (m_game->getCountHissWithMina().getMainBounds().getRight() > m_game->getFarRight())
        {
            gs->m_iRequestedAction = REQUESTED_ACTION_SHOW_MESSAGE * 1000 + MESSAGE_INVALID_COUNT_HISS_KEY;
            return false;
        }
    }
    
    return true;
}

GameScreenLevelEditor::GameScreenLevelEditor() :
	m_lastAddedEntity(nullptr),
	m_draggingEntity(nullptr),
	m_attachToEntity(nullptr),
	m_fDraggingEntityOriginalY(0),
	m_iWorld(0),
	m_iLevel(0),
	m_iLastOffset(0),
	m_isVerticalChangeAllowed(true),
	m_allowPlaceOn(true),
	m_allowPlaceUnder(false)
{
    m_game = std::unique_ptr<Game>(new Game());
    m_levelEditorActionsPanel = std::unique_ptr<LevelEditorActionsPanel>(new LevelEditorActionsPanel());
    m_levelEditorEntitiesPanel = std::unique_ptr<LevelEditorEntitiesPanel>(new LevelEditorEntitiesPanel());
    m_trashCan = std::unique_ptr<TrashCan>(new TrashCan());
    m_levelSelectorPanel = std::unique_ptr<LevelSelectorPanel>(new LevelSelectorPanel());
    m_offsetPanel = std::unique_ptr<OffsetPanel>(new OffsetPanel());
    m_confirmResetPanel = std::unique_ptr<ConfirmResetPanel>(new ConfirmResetPanel());
    m_confirmExitPanel = std::unique_ptr<ConfirmExitPanel>(new ConfirmExitPanel());
}
