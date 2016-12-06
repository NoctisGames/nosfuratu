//
//  GameScreenSpriteTester.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 11/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenSpriteTester.h"

#include "GameScreen.h"
#include "GameScreenTitle.h"
#include "EntityUtils.h"
#include "Vector2D.h"

#include <algorithm> // std::sort

#include <sstream>

GameScreenSpriteTester * GameScreenSpriteTester::getInstance()
{
    static GameScreenSpriteTester *instance = new GameScreenSpriteTester();
    
    return instance;
}

void GameScreenSpriteTester::enter(GameScreen* gs)
{
    gs->m_stateMachine->setPreviousState(Title::getInstance());
    
    initRenderer(gs);
    
    gs->m_renderer->zoomOut();
    
    if (m_textureSelectorPanel->getSelectedTexture() == nullptr)
    {
        m_textureSelectorPanel->open();
    }
}

void GameScreenSpriteTester::initRenderer(GameScreen* gs)
{
    gs->m_renderer->init(RENDERER_TYPE_SPRITE_TESTER);
}

void GameScreenSpriteTester::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderSpriteTester(this);
        
        gs->m_renderer->renderToScreen();
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->endFrame();
    }
    else
    {
        handleTouchInput(gs);
        
        EntityUtils::updateAndClean(m_gameEntities, gs->m_fDeltaTime);
        
        m_trashCan->update(gs->m_renderer->getCameraPosition());
    }
}

void GameScreenSpriteTester::exit(GameScreen* gs)
{
	m_draggingEntity = nullptr;
	m_lastAddedEntity = nullptr;
}

std::vector<UnknownEntity*>& GameScreenSpriteTester::getEntities()
{
    return m_gameEntities;
}

UnknownEntity* GameScreenSpriteTester::getLastAddedEntity()
{
    return m_lastAddedEntity;
}

UnknownEntity* GameScreenSpriteTester::getDraggingEntity()
{
    return m_draggingEntity;
}

SpriteTesterActionsPanel* GameScreenSpriteTester::getSpriteTesterActionsPanel()
{
    return m_spriteTesterActionsPanel.get();
}

SpriteTesterEntitiesPanel* GameScreenSpriteTester::getSpriteTesterEntitiesPanel()
{
    return m_spriteTesterEntitiesPanel.get();
}

TrashCan* GameScreenSpriteTester::getTrashCan()
{
    return m_trashCan.get();
}

TextureSelectorPanel* GameScreenSpriteTester::getTextureSelectorPanel()
{
    return m_textureSelectorPanel.get();
}

ConfirmResetPanel* GameScreenSpriteTester::getConfirmResetPanel()
{
	return m_confirmResetPanel.get();
}

ConfirmExitPanel* GameScreenSpriteTester::getConfirmExitPanel()
{
	return m_confirmExitPanel.get();
}

#pragma mark private

void GameScreenSpriteTester::handleTouchInput(GameScreen* gs)
{
    for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld(*(*i));
        
        int rc;
        if (m_textureSelectorPanel->isOpen())
        {
            std::vector<TextureWrapper *>& textureWrappers = gs->m_renderer->getTextureWrappers();
            if ((rc = m_textureSelectorPanel->handleTouch(*(*i), *gs->m_touchPoint, textureWrappers)) != TEXTURE_SELECTOR_PANEL_RC_UNHANDLED)
            {
                switch (rc)
                {
                    case TEXTURE_SELECTOR_PANEL_RC_CONFIRM:
                        if (m_textureSelectorPanel->getSelectedTexture())
                        {
                            m_spriteTesterEntitiesPanel->initForTextureName(m_textureSelectorPanel->getSelectedTexture()->name);
                        }
                        break;
                    case TEXTURE_SELECTOR_PANEL_RC_HANDLED:
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
                        resetEntities();
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
        
        if ((rc = m_spriteTesterActionsPanel->handleTouch(*(*i), *gs->m_touchPoint)) != SPRITE_TESTER_ACTIONS_PANEL_RC_UNHANDLED)
        {
            gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
            
            switch (rc)
            {
                case SPRITE_TESTER_ACTIONS_PANEL_RC_RESET:
                    m_confirmResetPanel->open();
					return;
                case SPRITE_TESTER_ACTIONS_PANEL_RC_EXIT:
                    m_confirmExitPanel->open();
                    return;
                case SPRITE_TESTER_ACTIONS_PANEL_RC_LOAD:
                    m_textureSelectorPanel->open();
                    return;
                case SPRITE_TESTER_ACTIONS_PANEL_RC_HANDLED:
                default:
                    break;
            }
            
            return;
        }
        
        if ((rc = m_spriteTesterEntitiesPanel->handleTouch(*(*i), *gs->m_touchPoint, gs->m_renderer->getCameraPosition(), m_gameEntities, &m_lastAddedEntity)) != SPRITE_TESTER_ENTITIES_PANEL_RC_UNHANDLED)
        {
            if (rc == SPRITE_TESTER_ENTITIES_PANEL_RC_ENTITY_ADDED)
            {
                // Empty
            }
            
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
                
                int index = -1;
                if ((index = EntityUtils::isTouching(m_gameEntities, tp)) != -1)
                {
                    m_draggingEntity = m_gameEntities.at(index);
                    m_draggingEntity->reset();
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
                        m_draggingEntity->requestDeletion();
                        
                        if (m_draggingEntity == m_lastAddedEntity)
                        {
                            m_lastAddedEntity = nullptr;
                        }
                    }
                }
                
                m_trashCan->setHighlighted(false);
                
                if (m_draggingEntity != nullptr)
                {
                    m_draggingEntity->updateBounds();
                }
                
                m_draggingEntity = nullptr;
                
                return;
        }
    }
}

void GameScreenSpriteTester::resetEntities()
{
	m_draggingEntity = nullptr;
	m_lastAddedEntity = nullptr;

    m_gameEntities.clear();
}

GameScreenSpriteTester::GameScreenSpriteTester() : m_lastAddedEntity(nullptr), m_draggingEntity(nullptr)
{
    m_spriteTesterActionsPanel = std::unique_ptr<SpriteTesterActionsPanel>(new SpriteTesterActionsPanel());
    m_spriteTesterEntitiesPanel = std::unique_ptr<SpriteTesterEntitiesPanel>(new SpriteTesterEntitiesPanel());
    m_trashCan = std::unique_ptr<TrashCan>(new TrashCan());
    m_textureSelectorPanel = std::unique_ptr<TextureSelectorPanel>(new TextureSelectorPanel());
    m_confirmResetPanel = std::unique_ptr<ConfirmResetPanel>(new ConfirmResetPanel());
    m_confirmExitPanel = std::unique_ptr<ConfirmExitPanel>(new ConfirmExitPanel());
}

RTTI_IMPL(GameScreenSpriteTester, GameScreenState);
