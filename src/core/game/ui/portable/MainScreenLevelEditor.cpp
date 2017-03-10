//
//  MainScreenLevelEditor.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreenLevelEditor.h"

#include "MainScreen.h"
#include "Game.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "TrashCan.h"
#include "LevelSelectorPanel.h"
#include "OffsetPanel.h"
#include "ConfirmResetPanel.h"
#include "ConfirmExitPanel.h"
#include "GridLockedPhysicalEntity.h"
#include "PhysicalEntity.h"

#include "RendererType.h"
#include "MainScreenLevels.h"
#include "MainScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "MainScreenTitle.h"
#include "BatPanel.h"
#include "ScreenInputManager.h"
#include "TouchConverter.h"
#include "MainRenderer.h"
#include "ScreenEvent.h"
#include "ForegroundCoverObject.h"
#include "Midground.h"
#include "CollectibleItem.h"
#include "macros.h"

#include <algorithm> // std::sort

#include <sstream>
#include <stdio.h>
#include <stdlib.h>

bool sortGrounds(Ground* i, Ground* j)
{
    return i->getType() < j->getType();
}

bool sortGameEntities(Entity* i, Entity* j)
{
    return i->getID() < j->getID();
}

MainScreenLevelEditor * MainScreenLevelEditor::getInstance()
{
    static MainScreenLevelEditor *instance = new MainScreenLevelEditor();
    
    return instance;
}

void MainScreenLevelEditor::enter(MainScreen* ms)
{
    ms->m_stateMachine.setPreviousState(Title::getInstance());
    
    loadIfNecessary(ms);
    
    initRenderer(ms);
    
    ms->m_renderer->zoomOut();
    
    m_game->setCameraBounds(&ms->m_renderer->getCameraBounds());
    
    if (m_iWorld == 0)
    {
        m_levelSelectorPanel->open();
    }
}

void MainScreenLevelEditor::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();

		if (m_game->isLoaded())
		{
			ms->m_renderer->renderWorld(*m_game);

			ms->m_renderer->renderJonAndExtraForegroundObjects(*m_game);

			if (m_lastAddedEntity != nullptr)
			{
				static Color highlight = Color(1, 1, 1, 0.25f);
				ms->m_renderer->renderEntityHighlighted(*m_lastAddedEntity, highlight);
			}

			if (m_draggingEntity != nullptr)
			{
				static Color highlight = Color(1, 1, 1, 0.5f);
				ms->m_renderer->renderEntityHighlighted(*m_draggingEntity, highlight);

				if (m_attachToEntity != nullptr)
				{
					ms->m_renderer->renderEntityHighlighted(*m_attachToEntity, highlight);
				}
			}

			if (m_levelEditorActionsPanel->showBounds())
			{
				ms->m_renderer->renderBounds(*m_game, m_levelEditorActionsPanel->boundsLevelRequested());
			}

			ms->m_renderer->renderMarkers(*m_game);
		}
        
        ms->m_renderer->renderToScreen();
        
		ms->m_renderer->renderLevelEditor(this);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->endFrame();
    }
    else
    {
		if (m_fMessageTime > 0)
		{
			m_fMessageTime -= ms->m_fDeltaTime;
			if (m_fMessageTime < 0)
			{
				m_message = nullptr;
			}
		}

        handleInput(ms);
        
        int oldSum = m_game->calcSum();
        
        m_game->update(ms->m_fDeltaTime);

		if (m_game->getJons().size() > 0)
		{
			m_game->updateAndClean(ms->m_fDeltaTime);
		}
        
        if (m_game->getJons().size() > 1)
        {
            for (std::vector<Jon *>::iterator i = m_game->getJons().begin(); i != m_game->getJons().end(); )
            {
                if ((*i)->isDead())
                {
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
        
        m_trashCan->update(ms->m_renderer->getCameraPosition());
        
        m_game->updateBackgrounds(ms->m_renderer->getCameraPosition(), ms->m_fDeltaTime);
    }
}

void MainScreenLevelEditor::exit(MainScreen* ms)
{
	m_draggingEntity = nullptr;
	m_attachToEntity = nullptr;
	m_lastAddedEntity = nullptr;
}

void MainScreenLevelEditor::initRenderer(MainScreen* ms)
{
    ms->m_renderer->load(RENDERER_TYPE_LEVEL_EDITOR);
}

const char* MainScreenLevelEditor::save()
{
    return m_game->save();
}

Game& MainScreenLevelEditor::getGame()
{
    return *m_game;
}

LevelEditorActionsPanel* MainScreenLevelEditor::getLevelEditorActionsPanel()
{
    return m_levelEditorActionsPanel;
}

LevelEditorEntitiesPanel* MainScreenLevelEditor::getLevelEditorEntitiesPanel()
{
    return m_levelEditorEntitiesPanel;
}

TrashCan* MainScreenLevelEditor::getTrashCan()
{
    return m_trashCan;
}

LevelSelectorPanel* MainScreenLevelEditor::getLevelSelectorPanel()
{
    return m_levelSelectorPanel;
}

OffsetPanel* MainScreenLevelEditor::getOffsetPanel()
{
	return m_offsetPanel;
}

ConfirmResetPanel* MainScreenLevelEditor::getConfirmResetPanel()
{
	return m_confirmResetPanel;
}

ConfirmExitPanel* MainScreenLevelEditor::getConfirmExitPanel()
{
	return m_confirmExitPanel;
}

const char* MainScreenLevelEditor::getMessage()
{
	return m_message;
}

#pragma mark private

void MainScreenLevelEditor::handleInput(MainScreen* ms)
{
    for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); ++i)
    {
        Vector2D& touchPoint = TOUCH_CONVERTER->touchToWorld(*(*i));
        
        int rc;
        if (m_levelSelectorPanel->isOpen())
        {
            if ((rc = m_levelSelectorPanel->handleTouch(*(*i), touchPoint)) != LEVEL_SELECTOR_PANEL_RC_UNHANDLED)
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
                            ms->m_renderer->unload(oldRendererType);
							ms->m_renderer->load(newRendererType);
                        }
                        
                        m_levelEditorEntitiesPanel->initForLevel(m_iWorld, m_iLevel);
                        
                        m_game->reset();
                        
                        resetEntities(true);
                        
                        loadLevel(m_iWorld, m_iLevel, ms);
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
            if ((rc = m_offsetPanel->handleTouch(*(*i), touchPoint)) != OFFSET_PANEL_RC_UNHANDLED)
            {
                switch (rc)
                {
                    case OFFSET_PANEL_RC_HANDLED:
                    {
						int rawOffset = m_offsetPanel->getOffset();

						int offset = rawOffset > m_iLastOffset ? 1 : -1;

						m_iLastOffset = rawOffset;
                        
                        GameMarker* endLoopMarker = m_game->getMarkers().at(m_game->getMarkers().size() - 1);
                        GameMarker* beginLoopMarker = m_game->getMarkers().at(m_game->getMarkers().size() - 2);
                        
                        int beginGridX = beginLoopMarker->getGridX();
                        int endGridX = endLoopMarker->getGridX();
                        
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getMidgrounds(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getGrounds(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getPits(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getExitGrounds(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getHoles(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getForegroundObjects(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getMidBossForegroundObjects(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getEndBossForegroundObjects(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getCountHissWithMinas(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getEndBossSnakes(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getEnemies(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getCollectibleItems(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getExtraForegroundObjects(), beginGridX, endGridX, offset);
                        EntityUtils::offsetAllInRangeOpenEnd(m_game->getForegroundCoverObjects(), beginGridX, endGridX, offset);
                        
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
            if ((rc = m_confirmResetPanel->handleTouch(*(*i), touchPoint)) != CONFIRM_RESET_PANEL_RC_UNHANDLED)
            {
                switch (rc)
                {
                    case CONFIRM_RESET_PANEL_RC_RESET:
                    {
                        m_game->reset();
                        resetEntities(true);
                        enter(ms);
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
            if ((rc = m_confirmExitPanel->handleTouch(*(*i), touchPoint)) != CONFIRM_EXIT_PANEL_RC_UNHANDLED)
            {
                switch (rc)
                {
                    case CONFIRM_EXIT_PANEL_RC_EXIT:
                    {
                        m_iWorld = 0;
                        m_iLevel = 0;
                        ms->m_stateMachine.revertToPreviousState();
                    }
                        break;
                    case CONFIRM_EXIT_PANEL_RC_CANCEL:
                    default:
                        break;
                }
            }
            
            return;
        }
        
        if ((rc = m_levelEditorActionsPanel->handleTouch(*(*i), touchPoint)) != LEVEL_EDITOR_ACTIONS_PANEL_RC_UNHANDLED)
        {
            ms->m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
            
            switch (rc)
            {
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_MARKER:
                {
                    int gridX = (ms->m_renderer->getCameraPosition().getX() + ZOOMED_OUT_CAM_WIDTH / 2) / GRID_CELL_SIZE;
                    int type = 0; // begin loop
                    if (m_game->getMarkers().size() == 1 || m_game->getMarkers().size() % 2 == 1)
                    {
                        type = 1; // end loop
                    }
                    GameMarker* m = GameMarker::create(gridX, 0, type);
                    m_game->getMarkers().push_back(m);
                    resetEntities(true);
                }
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_OFFSET:
                    if (m_game->getMarkers().size() < 2)
                    {
                        setMessage("2 markers must be placed to denote the section to offset");
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
                    if (isLevelValid(ms))
                    {
                        Jon* jon = m_game->getJonP();
                        jon->enableAbility(FLAG_ABILITY_ALL);
                        Level* levelState = LevelUtil::getInstanceForWorldAndLevel(m_iWorld, m_iLevel);
                        levelState->setSourceGame(m_game);
						levelState->setBestStats(0, 0, 0, 0, FLAG_ABILITY_ALL);
                        ms->m_stateMachine.changeState(levelState);
						return;
                    }
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_LOAD:
                    m_levelSelectorPanel->open();
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_SAVE:
                    if (isLevelValid(ms))
                    {
                        resetEntities(true);
                        
                        saveLevel(m_iWorld, m_iLevel);
                    }
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED:
                default:
                    break;
            }
            
            return;
        }
        
        if ((rc = m_levelEditorEntitiesPanel->handleTouch(*(*i), touchPoint, *m_game, ms->m_renderer->getCameraPosition(), &m_lastAddedEntity)) != LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED)
        {
            if (rc == LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED)
            {
                if (m_lastAddedEntity->getRTTI().derivesFrom(Ground::rtti))
                {
                    Ground* ground = reinterpret_cast<Ground *>(m_lastAddedEntity);
                    ground->setGame(m_game);
                }
                else if (m_lastAddedEntity->getRTTI().derivesFrom(Jon::rtti))
                {
                    Jon* jon = reinterpret_cast<Jon *>(m_lastAddedEntity);
                    jon->setGame(m_game);
                }
                else if (m_lastAddedEntity->getRTTI().derivesFrom(Enemy::rtti))
                {
                    Enemy* e = reinterpret_cast<Enemy *>(m_lastAddedEntity);
                    e->setGame(m_game);
                }
                else if (m_lastAddedEntity->getRTTI().derivesFrom(ForegroundObject::rtti))
                {
                    ForegroundObject* fo = reinterpret_cast<ForegroundObject *>(m_lastAddedEntity);
                    fo->setGame(m_game);
                }
                else if (m_lastAddedEntity->getRTTI().derivesFrom(CollectibleItem::rtti))
                {
                    CollectibleItem* ci = reinterpret_cast<CollectibleItem *>(m_lastAddedEntity);
                    ci->setGame(m_game);
                }
            }
            
            resetEntities(false);
            
            return;
        }
        
        switch ((*i)->getType())
        {
            case ScreenEventType_DOWN:
            {
                Vector2D tp = touchPoint.cpy();
                tp.mul(4);
                float camPosX = ms->m_renderer->getCameraPosition().getX();
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
                    
                    bool isGroundAndNotPitTunnel = m_draggingEntity->getRTTI().derivesFrom(Ground::rtti) && !m_draggingEntity->getRTTI().derivesFrom(PitTunnel::rtti);
                    
                    if (isGroundAndNotPitTunnel
                        || m_draggingEntity->getRTTI().derivesFrom(ExitGround::rtti)
                        || m_draggingEntity->getRTTI().derivesFrom(Hole::rtti)
                        || m_draggingEntity->getRTTI().derivesFrom(SpikeTower::rtti)
                        || m_draggingEntity->getRTTI().derivesFrom(VerticalSaw::rtti)
						|| m_draggingEntity->getRTTI().derivesFrom(GameMarker::rtti))
                    {
                        m_isVerticalChangeAllowed = false;
                        m_allowPlaceOn = false;
                        m_fDraggingEntityOriginalY = m_draggingEntity->getPosition().getY();
                    }
					else if (m_draggingEntity->getRTTI().derivesFrom(ForegroundCoverObject::rtti))
					{
						ForegroundCoverObject* fco = reinterpret_cast<ForegroundCoverObject *>(m_draggingEntity);
						if (fco->getType() != ForegroundCoverObjectType_Wall
							&& fco->getType() != ForegroundCoverObjectType_Wall_Window
							&& fco->getType() != ForegroundCoverObjectType_Roof_Side_Left
							&& fco->getType() != ForegroundCoverObjectType_Roof_Side_Right
							&& fco->getType() != ForegroundCoverObjectType_Roof_Plain
							&& fco->getType() != ForegroundCoverObjectType_Roof_Chimney)
						{
							m_isVerticalChangeAllowed = false;
							m_allowPlaceOn = false;
							m_fDraggingEntityOriginalY = m_draggingEntity->getPosition().getY();
						}
					}
                    else if (m_draggingEntity->getRTTI().derivesFrom(Midground::rtti))
                    {
                        Midground* midground = reinterpret_cast<Midground *>(m_draggingEntity);
                        if (midground->getType() != MidgroundType_Metal_Tower_Section
                            && midground->getType() != MidgroundType_Billboard_Count_Hiss
                            && midground->getType() != MidgroundType_Billboard_Slag_Town
                            && midground->getType() != MidgroundType_Billboard_Jon_Wanted
							&& midground->getType() != MidgroundType_Stone_Square
							&& midground->getType() != MidgroundType_Stone_Diamond
							&& midground->getType() != MidgroundType_Wall
							&& midground->getType() != MidgroundType_Roof)
                        {
                            m_isVerticalChangeAllowed = false;
                            m_allowPlaceOn = false;
                            m_fDraggingEntityOriginalY = m_draggingEntity->getPosition().getY();
                        }
                    }
                    else if (m_draggingEntity->getRTTI().derivesFrom(PlatformObject::rtti)
                             || m_draggingEntity->getRTTI().derivesFrom(RunningIntoDeathObject::rtti)
                             || m_draggingEntity->getRTTI().derivesFrom(CollectibleItem::rtti)
                             || m_draggingEntity->getRTTI().derivesFrom(PitTunnel::rtti))
                    {
                        m_allowPlaceOn = false;
                    }
                    else if (m_draggingEntity->getRTTI().derivesFrom(MushroomCeiling::rtti)
						|| m_draggingEntity->getRTTI().derivesFrom(DeathFromAboveObject::rtti)
						|| m_draggingEntity->getRTTI().derivesFrom(SpikedBall::rtti)
						|| m_draggingEntity->getRTTI().derivesFrom(BigMushroomCeiling::rtti))
                    {
                        m_allowPlaceUnder = true;
						m_allowPlaceOn = false;
                    }
                }
                else if (m_lastAddedEntity && m_lastAddedEntity->getRTTI().derivesFrom(CollectibleItem::rtti))
				{
                    lastAddedCollectibleItem = reinterpret_cast<CollectibleItem *>(m_lastAddedEntity);
					int gridX = tp.getX() / GRID_CELL_SIZE;
					int gridY = tp.getY() / GRID_CELL_SIZE;
					CollectibleItem* collectibleItem = CollectibleItem::create(gridX, gridY, lastAddedCollectibleItem->getType());
					m_game->getCollectibleItems().push_back(collectibleItem);

					m_lastAddedEntity = collectibleItem;

					resetEntities(false);
				}
                
                ms->m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
                ms->m_touchPointDown2->set(touchPoint.getX(), touchPoint.getY());
            }
                continue;
            case ScreenEventType_DRAGGED:
            {
                float xDelta = touchPoint.getX() - ms->m_touchPointDown->getX();
                xDelta *= 4;
                float yDelta = touchPoint.getY() - ms->m_touchPointDown->getY();
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
                    
                    if (m_draggingEntity->getMainBounds().getBottom() < 0 && yDelta < 0)
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
						if ((index = EntityUtils::indexOfOverlappingObjectThatCanBePlacedOn(m_draggingEntity, m_game->getForegroundObjects())) != -1)
                        {
                            m_attachToEntity = m_game->getForegroundObjects().at(index);
                        }
                        else if ((index = EntityUtils::indexOfOverlappingObjectThatCanBePlacedOn(m_draggingEntity, m_game->getGrounds())) != -1)
                        {
                            m_attachToEntity = m_game->getGrounds().at(index);
                        }
                        else if ((index = EntityUtils::indexOfOverlappingObjectThatCanBePlacedOn(m_draggingEntity, m_game->getExitGrounds())) != -1)
                        {
                            m_attachToEntity = m_game->getExitGrounds().at(index);
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
                    
                    m_trashCan->setHighlighted(OverlapTester::doNGRectsOverlap(m_draggingEntity->getMainBounds(), m_trashCan->getMainBounds()));
                    
                    if ((touchPoint.getX() > (CAM_WIDTH * 0.8f) && xDelta > 0)
                        || (touchPoint.getX() < (CAM_WIDTH * 0.2f) && xDelta < 0))
                    {
                        ms->m_renderer->moveCamera(xDelta);
                    }
                }
                else
                {
                    xDelta *= 4;
                    ms->m_renderer->moveCamera(-xDelta);
                }
                
                ms->m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
            }
                continue;
            case ScreenEventType_UP:
                if (m_draggingEntity != nullptr)
                {
                    if (OverlapTester::doNGRectsOverlap(m_draggingEntity->getMainBounds(), m_trashCan->getMainBounds()))
                    {
                        bool safeToDelete = true;
                        if (m_draggingEntity->getRTTI().derivesFrom(Jon::rtti) && m_game->getJons().size() == 1)
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

						if (m_draggingEntity->getRTTI().derivesFrom(Sparrow::rtti))
						{
							Sparrow *sparrow = reinterpret_cast<Sparrow *>(m_draggingEntity);
							sparrow->onMoved();
						}
                        else if (m_draggingEntity->getRTTI().derivesFrom(FloatingPlatformObject::rtti))
                        {
                            FloatingPlatformObject *fpo = reinterpret_cast<FloatingPlatformObject *>(m_draggingEntity);
                            fpo->onMoved();
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

void MainScreenLevelEditor::resetEntities(bool clearLastAddedEntity)
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
    EntityUtils::addAll(m_game->getEndBossForegroundObjects(), m_gameEntities);
    EntityUtils::addAll(m_game->getCountHissWithMinas(), m_gameEntities);
    EntityUtils::addAll(m_game->getEndBossSnakes(), m_gameEntities);
    EntityUtils::addAll(m_game->getEnemies(), m_gameEntities);
    EntityUtils::addAll(m_game->getCollectibleItems(), m_gameEntities);
	EntityUtils::addAll(m_game->getJons(), m_gameEntities);
    EntityUtils::addAll(m_game->getExtraForegroundObjects(), m_gameEntities);
    EntityUtils::addAll(m_game->getForegroundCoverObjects(), m_gameEntities);
    EntityUtils::addAll(m_game->getMarkers(), m_gameEntities);
    
    std::sort(m_gameEntities.begin(), m_gameEntities.end(), sortGameEntities);
    
    m_game->calcFarRight();
}

void MainScreenLevelEditor::loadIfNecessary(MainScreen* ms)
{
    if (!m_game->isLoaded())
    {
        std::stringstream ss;
        ss << "{\"world\":" << m_iWorld << ",\"level\":" << m_iLevel << ", \"jons\":[{\"gridX\":200,\"gridY\":200}]}";
        std::string jsonString = ss.str();
        
        load(jsonString.c_str(), ms);
    }
}

bool MainScreenLevelEditor::isLevelValid(MainScreen *ms)
{
    if (!m_game->hasEndSign())
    {
        setMessage("Cannot save a level that does not contain an End Sign");
        return false;
    }
    
    if (m_game->getJons().size() == 0)
    {
		setMessage("Cannot save a level that does not contain a Jon");
        return false;
    }
    
    if (m_game->getJon().getMainBounds().getRight() > m_game->getFarRight())
    {
        setMessage("Cannot save a level unless Jon is to the left of the end sign");
        return false;
    }
    
    if (m_game->getLevel() < 21)
    {
        if (m_game->getCountHissWithMinas().size() == 0)
        {
            setMessage("Cannot save a level that does not contain a Count Hiss");
            return false;
        }
        
        if (m_game->getCountHissWithMina().getMainBounds().getRight() > m_game->getFarRight())
        {
            setMessage("Cannot save a level unless Count Hiss is left of the end sign");
            return false;
        }
    }
    
    return true;
}

void MainScreenLevelEditor::setMessage(const char *message)
{
    m_message = message;
    m_fMessageTime = 2;
}

void MainScreenLevelEditor::saveLevel(int world, int level)
{
    const char *level_json = MainScreenLevelEditor::getInstance()->save();
    if (level_json)
    {
        std::stringstream ss;
        
#ifdef WIN32
        ss << "..\\nosfuratu-levels\\";
#else
        ss << "../nosfuratu-levels/";
#endif
        
        if (world > 0 && level > 0)
        {
            ss << "nosfuratu_c" << world << "_l" << level << ".json";
        }
        else
        {
            ss << "nosfuratu.json";
        }
        
        std::string ret = ss.str();
        
        const char *jsonFilePath = ret.c_str();
        
        FILE *file;
#ifdef WIN32
        errno_t err;
        if((err = fopen_s(&file, jsonFilePath, "r")) != 0)
        {
#else
        if ((file = fopen(jsonFilePath, "r")) == NULL)
        {
#endif
            setMessage("Could not find json file...");
        }
        else
        {
            int sum = fprintf(file, "%s", level_json);
            
            UNUSED(sum);
            
            fclose(file);
        }
        
        setMessage("Level Saved Successfully!");
    }
    else
    {
        setMessage("Error Saving Level...");
    }
}

void MainScreenLevelEditor::loadLevel(int world, int level, MainScreen* ms)
{
    std::stringstream ss;
    
#ifdef WIN32
    ss << "..\\nosfuratu-levels\\";
#else
    ss << "../nosfuratu-levels/";
#endif
    
    if (world > 0 && level > 0)
    {
        ss << "nosfuratu_c" << world << "_l" << level << ".json";
    }
    else
    {
        ss << "nosfuratu.json";
    }
    
    std::string ret = ss.str();
    
    const char *jsonFilePath = ret.c_str();
    
    FILE *file;
#ifdef WIN32
    errno_t err;
    if((err = fopen_s(&file, jsonFilePath, "r")) != 0)
    {
#else
    if ((file = fopen(jsonFilePath, "r")) == NULL)
    {
#endif
        setMessage("Could not find json file...");
    }
    else
    {
        // seek to end of file
        fseek(file, 0, SEEK_END);
        
        // get current file position which is end from seek
        size_t size = ftell(file);
        
        std::string jsonContent;
        
        // allocate string space and set length
        jsonContent.resize(size);
        
        // go back to beginning of file for read
        rewind(file);
        
        // read 1*size bytes from sfile into ss
        fread(&jsonContent[0], 1, size, file);
        
        // close the file
        fclose(file);
        
        load(jsonContent.c_str(), ms);
    }
}

void MainScreenLevelEditor::load(const char* json, MainScreen* ms)
{
    m_game->load(json);
    
    m_game->setCameraBounds(&ms->m_renderer->getCameraBounds());
    
    resetEntities(true);
    
    setMessage("Level Loaded Successfully!");
}

MainScreenLevelEditor::MainScreenLevelEditor() :
m_game(new Game()),
m_levelEditorActionsPanel(new LevelEditorActionsPanel()),
m_levelEditorEntitiesPanel(new LevelEditorEntitiesPanel()),
m_trashCan(new TrashCan()),
m_levelSelectorPanel(new LevelSelectorPanel()),
m_offsetPanel(new OffsetPanel()),
m_confirmResetPanel(new ConfirmResetPanel()),
m_confirmExitPanel(new ConfirmExitPanel()),
m_lastAddedEntity(nullptr),
m_draggingEntity(nullptr),
m_attachToEntity(nullptr),
m_message(nullptr),
m_fMessageTime(-1),
m_fDraggingEntityOriginalY(0),
m_iWorld(0),
m_iLevel(0),
m_iLastOffset(0),
m_isVerticalChangeAllowed(true),
m_allowPlaceOn(true),
m_allowPlaceUnder(false)
{
    m_game->setIsLevelEditor(true);
}

MainScreenLevelEditor::~MainScreenLevelEditor()
{
    delete m_game;
    delete m_levelEditorActionsPanel;
    delete m_levelEditorEntitiesPanel;
    delete m_trashCan;
    delete m_levelSelectorPanel;
    delete m_offsetPanel;
    delete m_confirmResetPanel;
    delete m_confirmExitPanel;
}

RTTI_IMPL(MainScreenLevelEditor, MainScreenState);
