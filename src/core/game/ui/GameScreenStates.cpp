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
        m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809},{\"x\":24,\"y\":19.9809},{\"x\":40,\"y\":19.9809}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623},{\"x\":24,\"y\":14.9623},{\"x\":40,\"y\":14.9623}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865},{\"x\":24,\"y\":5.63865},{\"x\":40,\"y\":5.63865}],\"trees\":[{\"x\":6.75,\"y\":13.8393,\"type\":2},{\"x\":20.0156,\"y\":14.1668,\"type\":1},{\"x\":37.6406,\"y\":15.6662,\"type\":0},{\"x\":114.024,\"y\":14.0327,\"type\":2},{\"x\":160.734,\"y\":14.1668,\"type\":1},{\"x\":77.9062,\"y\":13.0789,\"type\":0},{\"x\":94.2188,\"y\":14.1668,\"type\":1}],\"caveSkeletons\":[],\"grounds\":[{\"x\":6.02344,\"y\":5.11222,\"type\":7},{\"x\":12.1641,\"y\":5.11222,\"type\":6},{\"x\":24.9594,\"y\":5.11222,\"type\":9},{\"x\":31.2188,\"y\":3.17028,\"type\":17},{\"x\":43.2188,\"y\":3.17028,\"type\":17},{\"x\":50.3182,\"y\":3.17028,\"type\":19},{\"x\":55.0078,\"y\":3.17028,\"type\":18},{\"x\":57.2066,\"y\":3.17028,\"type\":19},{\"x\":74.9796,\"y\":5.11222,\"type\":8},{\"x\":98.9431,\"y\":5.11222,\"type\":5},{\"x\":128.896,\"y\":5.11222,\"type\":7},{\"x\":145.734,\"y\":3.17028,\"type\":18},{\"x\":147.933,\"y\":3.17028,\"type\":19},{\"x\":135.703,\"y\":5.11222,\"type\":9},{\"x\":153.633,\"y\":8.90251,\"type\":3},{\"x\":160.498,\"y\":8.90251,\"type\":2},{\"x\":167.305,\"y\":8.90251,\"type\":4},{\"x\":198.283,\"y\":3.17028,\"type\":18},{\"x\":210.797,\"y\":3.17028,\"type\":16},{\"x\":230.452,\"y\":3.17028,\"type\":18},{\"x\":231.863,\"y\":3.17028,\"type\":19},{\"x\":223.885,\"y\":3.17028,\"type\":19},{\"x\":239.812,\"y\":3.17028,\"type\":18},{\"x\":242.011,\"y\":3.17028,\"type\":19},{\"x\":299.484,\"y\":5.11222,\"type\":7},{\"x\":292.724,\"y\":5.11222,\"type\":8},{\"x\":311.484,\"y\":5.11222,\"type\":7},{\"x\":318.291,\"y\":5.11222,\"type\":9},{\"x\":-0.736794,\"y\":5.11222,\"type\":8}],\"logVerticalTalls\":[{\"x\":95.1562,\"y\":10.0958}],\"logVerticalShorts\":[{\"x\":130.688,\"y\":9.69225}],\"thorns\":[{\"x\":89.7188,\"y\":9.7682},{\"x\":92.4375,\"y\":9.7682},{\"x\":108.422,\"y\":9.7682},{\"x\":159.984,\"y\":9.7682}],\"stumps\":[{\"x\":14.1328,\"y\":10.7041}],\"sideSpikes\":[{\"x\":53.6953,\"y\":4.79883},{\"x\":73.875,\"y\":8.73633},{\"x\":229.078,\"y\":4.7461},{\"x\":238.453,\"y\":4.02539}],\"upwardSpikes\":[{\"x\":31.8281,\"y\":12.1567,\"type\":1},{\"x\":29.7891,\"y\":5.93335,\"type\":2},{\"x\":30.6094,\"y\":5.91577,\"type\":2},{\"x\":54.9141,\"y\":12.5434,\"type\":1},{\"x\":54.9609,\"y\":5.91577,\"type\":2},{\"x\":185.484,\"y\":11.5708,\"type\":0},{\"x\":210.023,\"y\":5.91577,\"type\":2},{\"x\":212.719,\"y\":5.91577,\"type\":2},{\"x\":226.898,\"y\":16.1176,\"type\":2},{\"x\":228.047,\"y\":16.1176,\"type\":2},{\"x\":228.844,\"y\":16.1176,\"type\":2},{\"x\":198.609,\"y\":5.91577,\"type\":2},{\"x\":199.477,\"y\":5.91577,\"type\":2},{\"x\":198.047,\"y\":5.91577,\"type\":2},{\"x\":186.352,\"y\":11.5708,\"type\":0},{\"x\":187.359,\"y\":11.5708,\"type\":0},{\"x\":184.711,\"y\":11.5708,\"type\":0},{\"x\":255.727,\"y\":12.7192,\"type\":1},{\"x\":288.038,\"y\":21.977,\"type\":0},{\"x\":289.102,\"y\":21.977,\"type\":0}],\"jumpSprings\":[{\"x\":66.1406,\"y\":7.25782,\"type\":1},{\"x\":97.8047,\"y\":9.53423,\"type\":0},{\"x\":146.742,\"y\":6.12634,\"type\":1},{\"x\":211.289,\"y\":6.12634,\"type\":1}],\"rocks\":[{\"x\":104.414,\"y\":10.2478,\"type\":1},{\"x\":119.484,\"y\":10.2478,\"type\":1},{\"x\":172.828,\"y\":11.5871,\"type\":0},{\"x\":180.891,\"y\":13.1164,\"type\":0},{\"x\":189.984,\"y\":14.5227,\"type\":0}],\"platforms\":[{\"x\":29.8359,\"y\":11.2676,\"type\":1},{\"x\":31.5435,\"y\":11.2676,\"type\":2},{\"x\":34.257,\"y\":11.2676,\"type\":2},{\"x\":36.9704,\"y\":11.2676,\"type\":2},{\"x\":39,\"y\":11.25,\"type\":2},{\"x\":40.7076,\"y\":11.25,\"type\":3},{\"x\":46.7109,\"y\":8.17383,\"type\":5},{\"x\":48.4536,\"y\":8.17383,\"type\":6},{\"x\":44.9566,\"y\":8.17383,\"type\":4},{\"x\":51.2645,\"y\":11.6191,\"type\":2},{\"x\":49.5569,\"y\":11.6191,\"type\":1},{\"x\":53.9779,\"y\":11.6191,\"type\":2},{\"x\":62.5781,\"y\":12.5859,\"type\":0},{\"x\":55.6855,\"y\":11.6191,\"type\":3},{\"x\":65.7188,\"y\":6.13477,\"type\":4},{\"x\":66.5024,\"y\":6.13477,\"type\":6},{\"x\":113.91,\"y\":20.4961,\"type\":1},{\"x\":115.617,\"y\":20.4961,\"type\":2},{\"x\":117.325,\"y\":20.4961,\"type\":3},{\"x\":141.07,\"y\":8.96484,\"type\":2},{\"x\":139.363,\"y\":8.96484,\"type\":1},{\"x\":142.778,\"y\":8.96484,\"type\":3},{\"x\":144.961,\"y\":11.584,\"type\":1},{\"x\":146.461,\"y\":11.584,\"type\":2},{\"x\":148.169,\"y\":11.584,\"type\":3},{\"x\":181.296,\"y\":11.5488,\"type\":2},{\"x\":171.516,\"y\":10.0195,\"type\":1},{\"x\":173.223,\"y\":10.0195,\"type\":2},{\"x\":174.931,\"y\":10.0195,\"type\":3},{\"x\":179.588,\"y\":11.5488,\"type\":1},{\"x\":183.003,\"y\":11.5488,\"type\":3},{\"x\":184.383,\"y\":10.7051,\"type\":1},{\"x\":186.09,\"y\":10.7051,\"type\":2},{\"x\":187.798,\"y\":10.7051,\"type\":3},{\"x\":188.766,\"y\":12.9902,\"type\":1},{\"x\":189.984,\"y\":12.9551,\"type\":2},{\"x\":191.692,\"y\":12.9551,\"type\":3},{\"x\":226.316,\"y\":15.2051,\"type\":4},{\"x\":228.07,\"y\":15.2051,\"type\":5},{\"x\":229.813,\"y\":15.2051,\"type\":6},{\"x\":247.078,\"y\":8.36719,\"type\":4},{\"x\":248.833,\"y\":8.36719,\"type\":5},{\"x\":250.575,\"y\":8.36719,\"type\":6},{\"x\":255.375,\"y\":11.8301,\"type\":1},{\"x\":257.083,\"y\":11.8301,\"type\":2},{\"x\":258.79,\"y\":11.8301,\"type\":3},{\"x\":266.742,\"y\":14.4141,\"type\":0},{\"x\":275.062,\"y\":16.8398,\"type\":0},{\"x\":281.859,\"y\":19.2657,\"type\":0},{\"x\":288.609,\"y\":21.1114,\"type\":0}],\"endSigns\":[{\"x\":318.258,\"y\":9.80859}],\"carrots\":[{\"x\":52.2891,\"y\":13.3594},{\"x\":99.9375,\"y\":17.3496},{\"x\":100.453,\"y\":18.7734},{\"x\":131.438,\"y\":11.127},{\"x\":173.133,\"y\":13.1133},{\"x\":181.078,\"y\":14.748},{\"x\":190.383,\"y\":16.1016},{\"x\":165.492,\"y\":10.3887},{\"x\":140.648,\"y\":10.9687},{\"x\":115.359,\"y\":22.8164},{\"x\":119.977,\"y\":10.002},{\"x\":13.8516,\"y\":13.957},{\"x\":261.398,\"y\":14.9063},{\"x\":275.297,\"y\":18.0528},{\"x\":284.672,\"y\":23.9765},{\"x\":294.61,\"y\":15.8555},{\"x\":294.633,\"y\":14.6777},{\"x\":294.656,\"y\":13.5},{\"x\":296.18,\"y\":15.9082},{\"x\":295.828,\"y\":13.8867},{\"x\":294.797,\"y\":12.4805},{\"x\":298.969,\"y\":15.8555},{\"x\":298.898,\"y\":14.8184},{\"x\":298.711,\"y\":13.7988},{\"x\":298.781,\"y\":12.7441},{\"x\":300.07,\"y\":12.6914},{\"x\":301.594,\"y\":12.6035},{\"x\":301.336,\"y\":13.4824},{\"x\":301.477,\"y\":14.625},{\"x\":301.57,\"y\":15.8027},{\"x\":304.148,\"y\":12.4805},{\"x\":304.148,\"y\":13.4297},{\"x\":304.125,\"y\":14.5547},{\"x\":304.242,\"y\":15.5742},{\"x\":305.484,\"y\":12.4453},{\"x\":305.672,\"y\":15.8027},{\"x\":308.391,\"y\":15.5742},{\"x\":308.25,\"y\":14.5371},{\"x\":308.016,\"y\":13.5},{\"x\":308.133,\"y\":12.3926},{\"x\":309.352,\"y\":14.4668},{\"x\":310.477,\"y\":15.1172},{\"x\":309.047,\"y\":13.6582},{\"x\":309.797,\"y\":13.0781},{\"x\":311.109,\"y\":15.5566},{\"x\":310.758,\"y\":12.2695},{\"x\":314.672,\"y\":15.4336},{\"x\":314.555,\"y\":14.4668},{\"x\":314.508,\"y\":13.8164},{\"x\":314.625,\"y\":12.1992}],\"goldenCarrots\":[{\"x\":36.7266,\"y\":7.11914},{\"x\":68.9531,\"y\":17.7188},{\"x\":101.625,\"y\":21.2344},{\"x\":153.422,\"y\":19.3184},{\"x\":235.922,\"y\":4.76367},{\"x\":316.992,\"y\":9.91406}],\"jons\":[{\"x\":5.25,\"y\":11.0918}]}");
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
        
        resetEntities(true);
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
                    if (m_game->getJons().size() > 0)
                    {
                        GamePlay::getInstance()->setSourceGame(m_game.get());
                        gs->m_stateMachine->changeState(GamePlay::getInstance());
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
    
    EntityUtils::addAll(m_game->getJons(), m_gameEntities);
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