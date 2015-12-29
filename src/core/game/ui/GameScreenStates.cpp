//
//  GameScreenStates.cpp
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
        
        if (m_isRequestingEnterGamePlay)
        {
            gs->m_renderer->renderLoadingTextOnTitleScreen();
        }
        
        gs->m_renderer->renderToScreen();
    }
    else
    {
        if (m_isRequestingEnterGamePlay)
        {
            gs->m_stateMachine->changeState(GamePlay::getInstance());
        }
        
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
                    m_isRequestingEnterGamePlay = true;
            }
        }
    }
}

void Title::exit(GameScreen* gs)
{
    m_isRequestingEnterGamePlay = false;
}

Title::Title() : m_isRequestingEnterGamePlay(false)
{
    // Empty
}

/// Game Play ///

GamePlay * GamePlay::getInstance()
{
    static GamePlay *instance = new GamePlay();
    
    return instance;
}

void GamePlay::enter(GameScreen* gs)
{
    if (!m_game->isLoaded())
    {
        if (m_sourceGame)
        {
            m_game->copy(m_sourceGame);
        }
        else
        {
            m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809},{\"x\":24,\"y\":19.9809},{\"x\":40,\"y\":19.9809}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623},{\"x\":24,\"y\":14.9623},{\"x\":40,\"y\":14.9623}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865},{\"x\":24,\"y\":5.63865},{\"x\":40,\"y\":5.63865}],\"trees\":[{\"x\":5.625,\"y\":14.1352,\"type\":1},{\"x\":16.5,\"y\":13.0566,\"type\":0},{\"x\":26.0625,\"y\":13.8236,\"type\":2},{\"x\":33.7031,\"y\":13.0566,\"type\":0},{\"x\":41.6016,\"y\":13.0566,\"type\":0},{\"x\":50.4375,\"y\":14.1352,\"type\":1},{\"x\":61.711,\"y\":13.8236,\"type\":2},{\"x\":69.6562,\"y\":13.0566,\"type\":0},{\"x\":76.2422,\"y\":13.0566,\"type\":0},{\"x\":85.1719,\"y\":14.1352,\"type\":1},{\"x\":93.6562,\"y\":13.0566,\"type\":0}],\"grounds\":[{\"x\":12,\"y\":8.91611,\"type\":0},{\"x\":-0.84794,\"y\":8.91611,\"type\":3},{\"x\":12.0234,\"y\":2.247,\"type\":10},{\"x\":-0.728622,\"y\":2.247,\"type\":13},{\"x\":30.001,\"y\":2.247,\"type\":11},{\"x\":29.9775,\"y\":8.91611,\"type\":1},{\"x\":47.9785,\"y\":2.247,\"type\":10},{\"x\":60.7306,\"y\":2.247,\"type\":14},{\"x\":53.9476,\"y\":8.91611,\"type\":0},{\"x\":83.9101,\"y\":5.0992,\"type\":5},{\"x\":72.6591,\"y\":2.247,\"type\":11},{\"x\":65.8996,\"y\":2.247,\"type\":13},{\"x\":71.182,\"y\":5.0992,\"type\":8},{\"x\":96.7161,\"y\":5.0992,\"type\":9}],\"holes\":[{\"x\":9.82031,\"y\":8.09532},{\"x\":55.0312,\"y\":8.09532}],\"caveExits\":[{\"x\":38.9663,\"y\":9.34754,\"type\":1},{\"x\":68.9288,\"y\":9.34754,\"type\":0}],\"logVerticalTalls\":[{\"x\":36.7547,\"y\":10.486},{\"x\":43.1125,\"y\":10.486},{\"x\":77.9594,\"y\":10.486}],\"logVerticalShorts\":[{\"x\":82.9547,\"y\":9.64714}],\"thorns\":[{\"x\":17.5078,\"y\":9.70219},{\"x\":23.9766,\"y\":9.70219},{\"x\":30.3281,\"y\":9.70219}],\"stumps\":[{\"x\":64.4297,\"y\":10.5445}],\"sideSpikes\":[],\"upwardSpikes\":[{\"x\":72.961,\"y\":9.47992,\"type\":1},{\"x\":71.7656,\"y\":9.47992,\"type\":1},{\"x\":36.3984,\"y\":1.695,\"type\":2},{\"x\":36.9609,\"y\":1.695,\"type\":2},{\"x\":40.1484,\"y\":1.695,\"type\":2},{\"x\":40.7109,\"y\":1.695,\"type\":2},{\"x\":57.6328,\"y\":9.47992,\"type\":1},{\"x\":58.8985,\"y\":9.47992,\"type\":1},{\"x\":60.0469,\"y\":9.47992,\"type\":1},{\"x\":61.2187,\"y\":9.47992,\"type\":1},{\"x\":56.5312,\"y\":9.47992,\"type\":1}],\"jumpSprings\":[{\"x\":69.606,\"y\":1.69016,\"type\":1},{\"x\":67.3188,\"y\":1.69016,\"type\":1},{\"x\":38.9312,\"y\":1.69016,\"type\":1},{\"x\":79.8453,\"y\":9.52131,\"type\":0}],\"rocks\":[{\"x\":12,\"y\":10.2286,\"type\":1}],\"platforms\":[{\"x\":20.4375,\"y\":15.0293,\"type\":0},{\"x\":28.9922,\"y\":18.5098,\"type\":0},{\"x\":41.0264,\"y\":20.3731,\"type\":2},{\"x\":39.3425,\"y\":20.3731,\"type\":1},{\"x\":43.7111,\"y\":20.3731,\"type\":2},{\"x\":46.3957,\"y\":20.3731,\"type\":2},{\"x\":50.7642,\"y\":20.3731,\"type\":3},{\"x\":49.0803,\"y\":20.3731,\"type\":2}],\"endSigns\":[{\"x\":95.086,\"y\":9.52672}],\"carrots\":[{\"x\":5.95312,\"y\":10.2916},{\"x\":7.45312,\"y\":10.2916},{\"x\":9.53906,\"y\":9.79941},{\"x\":8.76563,\"y\":10.8029},{\"x\":10.3125,\"y\":10.793},{\"x\":9.82031,\"y\":6.55664},{\"x\":9.70312,\"y\":3.23438},{\"x\":9.63281,\"y\":4.99219},{\"x\":46.5,\"y\":22.0036},{\"x\":47.7422,\"y\":22.0036},{\"x\":48.8204,\"y\":22.0036},{\"x\":50.2968,\"y\":22.0036},{\"x\":38.6484,\"y\":5.64258},{\"x\":41.0156,\"y\":13.3594},{\"x\":43.8281,\"y\":18.5977},{\"x\":17.0156,\"y\":2.43651},{\"x\":19.6172,\"y\":2.43651},{\"x\":26.2734,\"y\":2.43651},{\"x\":23.9531,\"y\":2.43651},{\"x\":21.7031,\"y\":2.43651},{\"x\":31.3359,\"y\":2.43651},{\"x\":28.6875,\"y\":2.43651}],\"goldenCarrots\":[{\"x\":55.5468,\"y\":20.6191},{\"x\":87.8031,\"y\":19.1446},{\"x\":63.3984,\"y\":0.298827}],\"jons\":[{\"x\":3.72656,\"y\":9.9053}]}");
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

void GamePlay::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->renderWorld(*m_game);
        
        gs->m_renderer->renderJon(*m_game);
        
        gs->m_renderer->renderHud(*m_game, *m_backButton);
        
        gs->m_renderer->renderToScreen();
    }
    else
    {
        if (!m_hasOpeningSequenceCompleted)
        {
            Jon& jon = m_game->getJon();
            
            if (handleOpeningSequenceTouchInput(gs))
            {
                gs->m_renderer->zoomIn();
                m_hasOpeningSequenceCompleted = true;
                jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
                return;
            }
            
            jon.update(gs->m_fDeltaTime);
            
            m_hasOpeningSequenceCompleted = gs->m_renderer->updateCameraToFollowPathToJon(*m_game, gs->m_fDeltaTime);
            jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
            
            EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
            
            return;
        }
        
        if (handleTouchInput(gs))
        {
            return;
        }
        
        Jon& jon = m_game->getJon();
        if (jon.getPosition().getX() - jon.getWidth() > m_game->getFarRight())
        {
            m_fStateTime += gs->m_fDeltaTime;
        }
        else
        {
            if ((jon.isTransformingIntoVampire() || jon.isRevertingToRabbit()) && jon.getTransformStateTime() < 0.125f)
            {
                gs->m_fDeltaTime /= 8;
            }
            
            m_game->updateAndClean(gs->m_fDeltaTime);
        }
        
        if (EntityUtils::isCollected(jon, m_game->getCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
        }
        
        if (EntityUtils::isCollected(jon, m_game->getGoldenCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
        }
        
        if (gs->m_isScreenHeldDown)
        {
            gs->m_fScreenHeldTime += gs->m_fDeltaTime;
            
            if (gs->m_fScreenHeldTime > 0.5f)
            {
                jon.triggerTransform();
                gs->m_isScreenHeldDown = false;
                gs->m_fScreenHeldTime = 0;
            }
        }
        
        if (jon.isDead() || m_fStateTime > 5)
        {
            m_game->reset();
            
            m_fStateTime = 0;
            
            enter(gs);
            
            return;
        }
        
        gs->m_renderer->updateCameraToFollowJon(*m_game, gs->m_fDeltaTime);
        
        EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
    }
}

void GamePlay::exit(GameScreen* gs)
{
    m_game->reset();
    
    m_fStateTime = 0;
    m_hasShownOpeningSequence = false;
    m_hasOpeningSequenceCompleted = false;
}

void GamePlay::setSourceGame(Game* game)
{
    m_sourceGame = game;
}

bool GamePlay::handleOpeningSequenceTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        switch (i->getTouchType())
        {
            case DOWN:
                return true;
            case DRAGGED:
                continue;
            case UP:
                continue;
        }
    }
    
    return false;
}

bool GamePlay::handleTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    Jon& jon = m_game->getJon();
    bool hasSwiped = false;
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        switch (i->getTouchType())
        {
            case DOWN:
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                gs->m_isScreenHeldDown = true;
                continue;
            case DRAGGED:
                if (!hasSwiped)
                {
                    if (gs->m_touchPoint->getX() >= (gs->m_touchPointDown->getX() + SWIPE_WIDTH))
                    {
                        // Swipe Right
                        jon.triggerRightAction();
                        hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getX() <= (gs->m_touchPointDown->getX() - SWIPE_WIDTH))
                    {
                        // Swipe Left
                        jon.triggerLeftAction();
                        hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() >= (gs->m_touchPointDown->getY() + SWIPE_HEIGHT))
                    {
                        // Swipe Up
                        jon.triggerUpAction();
                        hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() <= (gs->m_touchPointDown->getY() - SWIPE_HEIGHT))
                    {
                        // Swipe Down
                        jon.triggerDownAction();
                        hasSwiped = true;
                    }
                }
                continue;
            case UP:
                gs->m_isScreenHeldDown = false;
                gs->m_fScreenHeldTime = 0;
                if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getBounds()))
                {
                    Assets::getInstance()->setMusicId(MUSIC_STOP);
                    gs->m_stateMachine->revertToPreviousState();
                    return true;
                }
                
                if (gs->m_touchPoint->getX() < (gs->m_touchPointDown->getX() + SWIPE_WIDTH)
                    && gs->m_touchPoint->getX() > (gs->m_touchPointDown->getX() - SWIPE_WIDTH)
                         && gs->m_touchPoint->getY() < (gs->m_touchPointDown->getY() + SWIPE_HEIGHT)
                         && gs->m_touchPoint->getY() > (gs->m_touchPointDown->getY() - SWIPE_HEIGHT))
                {
                    jon.triggerJump();
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                
                break;
        }
    }
    
    return false;
}

GamePlay::GamePlay() : m_sourceGame(nullptr), m_fStateTime(0.0f), m_hasShownOpeningSequence(false), m_hasOpeningSequenceCompleted(false)
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
        
        gs->m_renderer->renderLevelEditor(*m_levelEditorActionsPanel, *m_levelEditorEntitiesPanel, *m_trashCan, *m_levelSelectorPanel);
        
        gs->m_renderer->renderToScreen();
    }
    else
    {
        handleTouchInput(gs);
        
        int oldSum = m_game->calcSum();
        
        m_game->updateAndClean(gs->m_fDeltaTime / 8);
        
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