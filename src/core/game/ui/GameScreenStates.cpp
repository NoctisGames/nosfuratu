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
        m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809,\"width\":16,\"height\":14.0381},{\"x\":24,\"y\":19.9809,\"width\":16,\"height\":14.0381},{\"x\":40,\"y\":19.9809,\"width\":16,\"height\":14.0381}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623,\"width\":16,\"height\":11.909},{\"x\":24,\"y\":14.9623,\"width\":16,\"height\":11.909},{\"x\":40,\"y\":14.9623,\"width\":16,\"height\":11.909}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865,\"width\":16,\"height\":11.2773},{\"x\":24,\"y\":5.63865,\"width\":16,\"height\":11.2773},{\"x\":40,\"y\":5.63865,\"width\":16,\"height\":11.2773}],\"trees\":[{\"x\":4.14844,\"y\":13.0789,\"width\":6.31579,\"height\":8.09532,\"treeType\":0},{\"x\":15.8906,\"y\":14.1668,\"width\":11.2281,\"height\":10.2712,\"treeType\":1},{\"x\":31.0547,\"y\":13.8392,\"width\":7.92982,\"height\":9.61612,\"treeType\":2},{\"x\":44.6484,\"y\":14.1668,\"width\":11.2281,\"height\":10.2712,\"treeType\":1},{\"x\":73.6172,\"y\":14.1668,\"width\":11.2281,\"height\":10.2712,\"treeType\":1},{\"x\":87.1172,\"y\":13.8392,\"width\":7.92982,\"height\":9.61612,\"treeType\":2},{\"x\":105.539,\"y\":14.1668,\"width\":11.2281,\"height\":10.2712,\"treeType\":1},{\"x\":119.203,\"y\":13.8393,\"width\":7.92982,\"height\":9.61612,\"treeType\":2}],\"caveSkeletons\":[],\"grounds\":[{\"x\":0.890625,\"y\":5.11222,\"width\":1.52047,\"height\":10.2244,\"groundType\":3,\"boundsHeightFactor\":0.883295},{\"x\":25.6041,\"y\":5.11222,\"width\":47.9064,\"height\":10.2244,\"groundType\":2,\"boundsHeightFactor\":0.883295},{\"x\":50.3643,\"y\":5.11222,\"width\":1.61404,\"height\":10.2244,\"groundType\":6,\"boundsHeightFactor\":0.883295},{\"x\":79.7912,\"y\":5.11222,\"width\":23.9766,\"height\":10.2244,\"groundType\":4,\"boundsHeightFactor\":0.883295},{\"x\":51.955,\"y\":2.36309,\"width\":1.56725,\"height\":4.72617,\"groundType\":11,\"boundsHeightFactor\":0.257426},{\"x\":58.7386,\"y\":2.36309,\"width\":12,\"height\":4.72617,\"groundType\":13,\"boundsHeightFactor\":0.257426},{\"x\":65.5105,\"y\":2.36309,\"width\":1.54386,\"height\":4.72617,\"groundType\":14,\"boundsHeightFactor\":0.257426},{\"x\":67.0427,\"y\":5.11222,\"width\":1.52047,\"height\":10.2244,\"groundType\":3,\"boundsHeightFactor\":0.883295},{\"x\":92.5865,\"y\":5.11222,\"width\":1.61404,\"height\":10.2244,\"groundType\":6,\"boundsHeightFactor\":0.883295},{\"x\":98.8828,\"y\":8.90251,\"width\":1.73099,\"height\":2.64385,\"groundType\":7,\"boundsHeightFactor\":0.548673},{\"x\":111.737,\"y\":8.90251,\"width\":23.9766,\"height\":2.64385,\"groundType\":8,\"boundsHeightFactor\":0.548673},{\"x\":124.532,\"y\":8.90251,\"width\":1.61404,\"height\":2.64385,\"groundType\":10,\"boundsHeightFactor\":0.548673},{\"x\":124.664,\"y\":2.36309,\"width\":1.56725,\"height\":4.72617,\"groundType\":11,\"boundsHeightFactor\":0.257426},{\"x\":137.436,\"y\":2.36309,\"width\":23.9766,\"height\":4.72617,\"groundType\":12,\"boundsHeightFactor\":0.257426},{\"x\":148.672,\"y\":3.17028,\"width\":2.19883,\"height\":6.34055,\"groundType\":15,\"boundsHeightFactor\":0.844444},{\"x\":155.772,\"y\":3.17028,\"width\":12,\"height\":6.34055,\"groundType\":17,\"boundsHeightFactor\":0.844444},{\"x\":162.871,\"y\":3.17028,\"width\":2.19883,\"height\":6.34055,\"groundType\":18,\"boundsHeightFactor\":0.844444},{\"x\":167.953,\"y\":5.11222,\"width\":1.52047,\"height\":10.2244,\"groundType\":3,\"boundsHeightFactor\":0.883295},{\"x\":181.52,\"y\":5.11222,\"width\":1.61404,\"height\":10.2244,\"groundType\":6,\"boundsHeightFactor\":0.883295},{\"x\":174.713,\"y\":5.11222,\"width\":12,\"height\":10.2244,\"groundType\":5,\"boundsHeightFactor\":0.883295}],\"logVerticalTalls\":[{\"x\":48.2344,\"y\":10.0958,\"width\":1.66082,\"height\":2.12912}],\"logVerticalShorts\":[{\"x\":42.7031,\"y\":9.63951,\"width\":1.52047,\"height\":1.21664}],\"thorns\":[{\"x\":109.148,\"y\":9.76819,\"width\":2.8538,\"height\":1.474},{\"x\":117.399,\"y\":9.76819,\"width\":2.8538,\"height\":1.474}],\"stumps\":[{\"x\":20.5312,\"y\":10.7041,\"width\":3.92982,\"height\":3.34575}],\"sideSpikes\":[{\"x\":147.329,\"y\":4.83398,\"width\":1.12281,\"height\":0.421144},{\"x\":147.258,\"y\":3.05859,\"width\":1.12281,\"height\":0.421144},{\"x\":147.188,\"y\":3.93751,\"width\":1.12281,\"height\":0.421144},{\"x\":147.469,\"y\":2.17969,\"width\":1.12281,\"height\":0.421144}],\"upwardSpikes\":[{\"x\":51.4453,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":52.0078,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":52.5703,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":53.1562,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":53.8125,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":54.5156,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":55.1953,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":55.8984,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":56.6953,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":57.3516,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":58.1953,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":59.1094,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":59.9297,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":62.1094,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":62.9062,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":63.5859,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":64.3594,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":64.9922,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":65.6016,\"y\":1.77816,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":132.352,\"y\":1.77817,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":133.453,\"y\":1.77817,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":134.578,\"y\":1.77817,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":131.391,\"y\":1.77817,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":130.477,\"y\":1.77817,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":129.352,\"y\":1.77817,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2},{\"x\":128.273,\"y\":1.77817,\"width\":0.421053,\"height\":1.12305,\"upwardSpikeType\":2}],\"jumpSprings\":[{\"x\":60.9609,\"y\":1.98873,\"width\":1.21637,\"height\":1.54419,\"jumpSpringType\":1}],\"rocks\":[{\"x\":90.6797,\"y\":10.2478,\"width\":4.49123,\"height\":4.30503,\"isCracked\":true},{\"x\":77.3906,\"y\":10.2478,\"width\":4.49123,\"height\":4.30503,\"isCracked\":false}],\"platforms\":[{\"x\":74.1094,\"y\":19.5117,\"width\":2.73684,\"height\":1.16984,\"groundPlatformType\":0},{\"x\":58.9922,\"y\":12.4629,\"width\":2.73684,\"height\":1.16984,\"groundPlatformType\":0},{\"x\":139.082,\"y\":4.81641,\"width\":0.795322,\"height\":1.16984,\"groundPlatformType\":4},{\"x\":140.836,\"y\":4.81641,\"width\":2.71345,\"height\":1.16984,\"groundPlatformType\":5},{\"x\":142.579,\"y\":4.81641,\"width\":0.77193,\"height\":1.16984,\"groundPlatformType\":6}],\"endSigns\":[{\"x\":176.789,\"y\":9.59272,\"width\":0.795322,\"height\":1.12305}],\"carrots\":[{\"x\":65.4375,\"y\":14.9062,\"width\":1.21637,\"height\":0.959272},{\"x\":66.0703,\"y\":16.1016,\"width\":1.21637,\"height\":0.959272},{\"x\":66.6797,\"y\":17.332,\"width\":1.21637,\"height\":0.959272},{\"x\":67.1953,\"y\":18.5449,\"width\":1.21637,\"height\":0.959272},{\"x\":67.7344,\"y\":19.6523,\"width\":1.21637,\"height\":0.959272},{\"x\":68.2266,\"y\":20.8652,\"width\":1.21637,\"height\":0.959272},{\"x\":68.6719,\"y\":22.0254,\"width\":1.21637,\"height\":0.959272},{\"x\":14.7422,\"y\":10.4238,\"width\":1.21637,\"height\":0.959272},{\"x\":20.1328,\"y\":13.4473,\"width\":1.21637,\"height\":0.959272},{\"x\":23.4609,\"y\":16.207,\"width\":1.21637,\"height\":0.959272},{\"x\":105.938,\"y\":10.2129,\"width\":1.21637,\"height\":0.959272},{\"x\":113.273,\"y\":10.0898,\"width\":1.21637,\"height\":0.959272},{\"x\":122.929,\"y\":9.89644,\"width\":1.21637,\"height\":0.959272},{\"x\":140.789,\"y\":6.32812,\"width\":1.21637,\"height\":0.959272},{\"x\":173.133,\"y\":14.1856,\"width\":1.21637,\"height\":0.959272},{\"x\":171.07,\"y\":12.2871,\"width\":1.21637,\"height\":0.959272},{\"x\":168.938,\"y\":10.459,\"width\":1.21637,\"height\":0.959272},{\"x\":81.7031,\"y\":10.5293,\"width\":1.21637,\"height\":0.959272},{\"x\":84.6328,\"y\":10.5117,\"width\":1.21637,\"height\":0.959272},{\"x\":87.2812,\"y\":10.4766,\"width\":1.21637,\"height\":0.959272}],\"goldenCarrots\":[{\"x\":74.0625,\"y\":21.252,\"width\":1.21637,\"height\":1.14645},{\"x\":95.8359,\"y\":8.47266,\"width\":1.21637,\"height\":1.14645},{\"x\":26.0859,\"y\":18.8437,\"width\":1.21637,\"height\":1.14645}],\"jons\":[{\"x\":3.2,\"y\":10.1312,\"width\":2.2,\"height\":2.2}]}");
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
        m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809,\"width\":16,\"height\":14.0381},{\"x\":24,\"y\":19.9809,\"width\":16,\"height\":14.0381},{\"x\":40,\"y\":19.9809,\"width\":16,\"height\":14.0381}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623,\"width\":16,\"height\":11.909},{\"x\":24,\"y\":14.9623,\"width\":16,\"height\":11.909},{\"x\":40,\"y\":14.9623,\"width\":16,\"height\":11.909}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865,\"width\":16,\"height\":11.2773},{\"x\":24,\"y\":5.63865,\"width\":16,\"height\":11.2773},{\"x\":40,\"y\":5.63865,\"width\":16,\"height\":11.2773}],\"jons\":[{\"x\":3.2,\"y\":10.1312,\"width\":2.2,\"height\":2.2}]}");
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
                    gs->m_iRequestedAction = REQUESTED_ACTION_LEVEL_EDITOR_LOAD;
                    break;
                case LEVEL_EDITOR_ACTIONS_PANEL_RC_SAVE:
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
                    if ((index = EntityUtils::doRectanglesOverlap(m_game->getGrounds(), *m_draggingEntity)) != -1)
                    {
                        m_attachToEntity = &m_game->getGrounds().at(index);
                    }
                    else if ((index = EntityUtils::doRectanglesOverlap(m_game->getPlatforms(), *m_draggingEntity)) != -1)
                    {
                        m_attachToEntity = &m_game->getPlatforms().at(index);
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
                                EntityUtils::placeOn(*m_draggingEntity, *m_attachToEntity);
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

LevelEditor::LevelEditor() : m_lastAddedEntity(nullptr), m_draggingEntity(nullptr), m_attachToEntity(nullptr), m_fDraggingEntityOriginalY(0), m_isVerticalChangeAllowed(true), m_useYCorrection(false), m_allowAttachment(true), m_allowPlaceOn(true)
{
    m_game = std::unique_ptr<Game>(new Game());
    m_levelEditorActionsPanel = std::unique_ptr<LevelEditorActionsPanel>(new LevelEditorActionsPanel());
    m_levelEditorEntitiesPanel = std::unique_ptr<LevelEditorEntitiesPanel>(new LevelEditorEntitiesPanel());
    m_trashCan = std::unique_ptr<TrashCan>(new TrashCan());
}