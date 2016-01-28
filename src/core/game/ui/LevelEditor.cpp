//
//  LevelEditor.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "LevelEditor.h"
#include "GameScreenStates.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"

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
    gs->m_renderer->zoomOut();
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
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->renderLevelEditor(*m_levelEditorActionsPanel, *m_levelEditorEntitiesPanel, *m_trashCan, *m_levelSelectorPanel);
        
        gs->m_renderer->endFrame();
    }
    else
    {
        handleTouchInput(gs);
        
        int oldSum = m_game->calcSum();
        
        m_game->update(gs->m_fDeltaTime);
        m_game->updateAndClean(gs->m_fDeltaTime / 4);
        
        if (m_game->getJons().size() > 1)
        {
            for (std::vector<std::unique_ptr<Jon>>::iterator i = m_game->getJons().begin(); i != m_game->getJons().end(); )
            {
                if (m_game->getJons().size() == 1)
                {
                    break;
                }
                
                if ((*i)->isRequestingDeletion())
                {
                    (*i)->onDeletion();
                    
                    i = m_game->getJons().erase(i);
                }
                else
                {
                    i++;
                }
            }
            
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
            
            switch (rc)
            {
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_RESET:
                    m_game->reset();
                    enter(gs);
					return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_TEST:
                    if (m_game->getJons().size() > 0)
                    {
                        GamePlay::getInstance()->setSourceGame(m_game.get());
                        gs->m_stateMachine->changeState(GamePlay::getInstance());
						return;
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
                    m_levelSelectorPanel->openForMode(LEVEL_SELECTOR_PANEL_MODE_LOAD);
                    return;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_SAVE:
                    if (m_game->getJons().size() > 0)
                    {
                        m_levelSelectorPanel->openForMode(LEVEL_SELECTOR_PANEL_MODE_SAVE);
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
                
                if (dynamic_cast<Jon*>(m_lastAddedEntity))
                {
                    Jon* entity = dynamic_cast<Jon*>(m_lastAddedEntity);
                    entity->setGame(m_game.get());
                }
                else if (dynamic_cast<SnakeGrunt*>(m_lastAddedEntity))
                {
                    SnakeGrunt* entity = dynamic_cast<SnakeGrunt*>(m_lastAddedEntity);
                    entity->setGame(m_game.get());
                }
                else if (dynamic_cast<SnakeHorned*>(m_lastAddedEntity))
                {
                    SnakeHorned* entity = dynamic_cast<SnakeHorned*>(m_lastAddedEntity);
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
                    
                    if (dynamic_cast<Ground*>(m_draggingEntity) || dynamic_cast<CaveExit*>(m_draggingEntity))
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
                        m_allowAttachment = false;
                        m_fYOffset = 0.8f;
                    }
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                gs->m_touchPointDown2->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
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
                    else if (m_draggingEntity->getBounds().getTop() > GAME_HEIGHT && yDelta > 0)
                    {
                        m_draggingEntity->getPosition().sub(0, yDelta);
                        m_draggingEntity->updateBounds();
                    }
                    
                    int index = -1;
                    if ((index = EntityUtils::doRectanglesOverlap(m_game->getGrounds(), m_draggingEntity)) != -1)
                    {
                        m_attachToEntity = m_game->getGrounds().at(index).get();
                    }
                    else if ((index = EntityUtils::doRectanglesOverlap(m_game->getCaveExits(), m_draggingEntity)) != -1)
                    {
                        m_attachToEntity = m_game->getCaveExits().at(index).get();
                    }
                    else if ((index = EntityUtils::doRectanglesOverlap(m_game->getPlatforms(), m_draggingEntity)) != -1)
                    {
                        m_attachToEntity = m_game->getPlatforms().at(index).get();
                    }
                    else if (dynamic_cast<Carrot*>(m_draggingEntity) && (index = EntityUtils::doRectanglesOverlap(m_game->getCarrots(), m_draggingEntity)) != -1)
                    {
                        m_attachToEntity = m_game->getCarrots().at(index).get();
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
                else if (m_lastAddedEntity != nullptr && gs->m_touchPoint->getX() < gs->m_touchPointDown2->getX() + 0.5f && gs->m_touchPoint->getX() > gs->m_touchPointDown2->getX() - 0.5f)
                {
                    if (dynamic_cast<Carrot*>(m_lastAddedEntity))
                    {
                        Vector2D tp = gs->m_touchPoint->cpy();
                        tp.mul(3);
                        float camPosX = gs->m_renderer->getCameraPosition().getX();
                        tp.add(camPosX, 0);
                        
                        m_lastAddedEntity = new Carrot(tp.getX(), m_lastAddedEntity->getPosition().getY());
                        m_game->getCarrots().push_back(std::unique_ptr<Carrot>(dynamic_cast<Carrot*>(m_lastAddedEntity)));
                        m_addedEntities.push_back(m_lastAddedEntity);
                        resetEntities(false);
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
    EntityUtils::addAll(m_game->getGrounds(), m_gameEntities);
    EntityUtils::addAll(m_game->getHoles(), m_gameEntities);
    EntityUtils::addAll(m_game->getCaveExits(), m_gameEntities);
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
    EntityUtils::addAll(m_game->getSnakeGruntEnemies(), m_gameEntities);
    EntityUtils::addAll(m_game->getSnakeHornedEnemies(), m_gameEntities);
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
    m_levelSelectorPanel = std::unique_ptr<LevelSelectorPanel>(new LevelSelectorPanel());
}