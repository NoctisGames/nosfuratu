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
            m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809},{\"x\":24,\"y\":19.9809},{\"x\":40,\"y\":19.9809}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623},{\"x\":24,\"y\":14.9623},{\"x\":40,\"y\":14.9623}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865},{\"x\":24,\"y\":5.63865},{\"x\":40,\"y\":5.63865}],\"trees\":[{\"x\":9.23438,\"y\":13.0789,\"type\":0},{\"x\":24.3047,\"y\":14.1668,\"type\":1},{\"x\":40.5234,\"y\":14.1668,\"type\":1},{\"x\":49.4531,\"y\":13.0789,\"type\":0},{\"x\":59.6953,\"y\":14.1668,\"type\":1},{\"x\":68.9297,\"y\":13.0789,\"type\":0},{\"x\":83.3906,\"y\":14.1668,\"type\":1},{\"x\":97.8047,\"y\":14.1668,\"type\":1},{\"x\":112.406,\"y\":13.8393,\"type\":2},{\"x\":120.398,\"y\":13.8393,\"type\":2},{\"x\":147.68,\"y\":13.0789,\"type\":0},{\"x\":134.227,\"y\":14.1668,\"type\":1},{\"x\":168.234,\"y\":13.0789,\"type\":0}],\"grounds\":[{\"x\":0.784449,\"y\":5.11222,\"type\":8},{\"x\":25.4979,\"y\":5.11222,\"type\":5},{\"x\":55.4511,\"y\":8.90251,\"type\":2},{\"x\":61.4394,\"y\":2.36309,\"type\":11},{\"x\":128.717,\"y\":5.11222,\"type\":6},{\"x\":152.694,\"y\":5.11222,\"type\":6},{\"x\":67.4511,\"y\":8.90251,\"type\":2},{\"x\":79.4277,\"y\":2.36309,\"type\":12},{\"x\":79.9688,\"y\":5.11222,\"type\":8},{\"x\":74.2581,\"y\":8.90251,\"type\":4},{\"x\":86.729,\"y\":5.11222,\"type\":7},{\"x\":98.729,\"y\":5.11222,\"type\":7},{\"x\":110.729,\"y\":5.11222,\"type\":7},{\"x\":170.682,\"y\":5.11222,\"type\":7},{\"x\":200.635,\"y\":5.11222,\"type\":5},{\"x\":225.395,\"y\":5.11222,\"type\":9}],\"holes\":[{\"x\":55.6654,\"y\":8.09532}],\"logVerticalTalls\":[{\"x\":64.2086,\"y\":10.0958},{\"x\":135.343,\"y\":10.0958}],\"logVerticalShorts\":[{\"x\":22.5474,\"y\":9.63951},{\"x\":33.0737,\"y\":9.63951},{\"x\":44.5865,\"y\":9.63951},{\"x\":51.1837,\"y\":9.63951},{\"x\":128.721,\"y\":9.63951},{\"x\":57.3777,\"y\":9.63951}],\"thorns\":[],\"stumps\":[{\"x\":15.7267,\"y\":10.7041},{\"x\":155.073,\"y\":10.7041}],\"sideSpikes\":[],\"upwardSpikes\":[],\"jumpSprings\":[{\"x\":77.3154,\"y\":1.98873,\"type\":1},{\"x\":78.6003,\"y\":1.98873,\"type\":1},{\"x\":75.9844,\"y\":1.98873,\"type\":1}],\"rocks\":[{\"x\":82.7109,\"y\":10.2478,\"type\":0},{\"x\":104.412,\"y\":10.2478,\"type\":1},{\"x\":98.8419,\"y\":15.0614,\"type\":0}],\"platforms\":[{\"x\":89.8181,\"y\":13.5286,\"type\":1},{\"x\":91.5257,\"y\":13.5286,\"type\":2},{\"x\":98.5678,\"y\":13.4938,\"type\":2},{\"x\":96.8602,\"y\":13.4938,\"type\":1},{\"x\":93.2333,\"y\":13.5286,\"type\":3},{\"x\":100.275,\"y\":13.4938,\"type\":3},{\"x\":125.356,\"y\":13.9586,\"type\":0},{\"x\":139.944,\"y\":16.1422,\"type\":0},{\"x\":162.584,\"y\":12.2771,\"type\":0},{\"x\":75.416,\"y\":8.68024,\"type\":1},{\"x\":77.1236,\"y\":8.68024,\"type\":2},{\"x\":78.8312,\"y\":8.68024,\"type\":3}],\"endSigns\":[{\"x\":173.464,\"y\":9.59272}],\"carrots\":[{\"x\":6.73301,\"y\":10.7522},{\"x\":7.94938,\"y\":10.7522},{\"x\":9.16576,\"y\":10.7522},{\"x\":10.3821,\"y\":10.7522},{\"x\":11.5985,\"y\":10.7522},{\"x\":15.5937,\"y\":14.5263},{\"x\":15.5311,\"y\":13.0633},{\"x\":19.1659,\"y\":10.1765},{\"x\":28.1596,\"y\":11.112},{\"x\":38.9689,\"y\":10.9233},{\"x\":32.8701,\"y\":13.096},{\"x\":22.4451,\"y\":13.4017},{\"x\":59.9404,\"y\":6.37331},{\"x\":63.2144,\"y\":2.47103},{\"x\":70.6841,\"y\":6.39017},{\"x\":97.4979,\"y\":9.94008},{\"x\":100.437,\"y\":9.91887},{\"x\":99.0622,\"y\":11.1644},{\"x\":116.204,\"y\":10.3108},{\"x\":76.8219,\"y\":10.3108},{\"x\":76.0079,\"y\":5.51936},{\"x\":77.7159,\"y\":14.0288},{\"x\":77.3429,\"y\":12.7138},{\"x\":77.9199,\"y\":15.421},{\"x\":78.5278,\"y\":16.6057},{\"x\":78.8841,\"y\":17.8984},{\"x\":80.0578,\"y\":20.3318},{\"x\":80.938,\"y\":21.2959},{\"x\":120.835,\"y\":14.3516},{\"x\":125.308,\"y\":16.1422},{\"x\":133.835,\"y\":20.3102},{\"x\":129.373,\"y\":18.4107},{\"x\":118.915,\"y\":12.7392},{\"x\":98.8659,\"y\":10.0451},{\"x\":79.4356,\"y\":19.1166},{\"x\":84.4532,\"y\":21.7541},{\"x\":44.4376,\"y\":11.764},{\"x\":44.2877,\"y\":13.4563},{\"x\":32.8719,\"y\":11.6658},{\"x\":22.3883,\"y\":11.7313},{\"x\":115.549,\"y\":11.328},{\"x\":114.346,\"y\":10.3108},{\"x\":88.7442,\"y\":18.6206},{\"x\":87.432,\"y\":19.7232},{\"x\":90.7269,\"y\":16.0767},{\"x\":95.5519,\"y\":13.5703},{\"x\":60.3071,\"y\":5.33474},{\"x\":58.6823,\"y\":5.4086},{\"x\":69.9311,\"y\":5.29699},{\"x\":71.6362,\"y\":5.19363},{\"x\":85.5107,\"y\":21.0772},{\"x\":86.5216,\"y\":20.361},{\"x\":82.9132,\"y\":22.4411},{\"x\":55.5008,\"y\":9.92067},{\"x\":150.668,\"y\":21.8319},{\"x\":152.297,\"y\":19.6095},{\"x\":153.571,\"y\":17.3568},{\"x\":154.163,\"y\":16.25},{\"x\":153.008,\"y\":18.4974},{\"x\":158.116,\"y\":10.5958},{\"x\":162.266,\"y\":13.4055},{\"x\":163.453,\"y\":13.3182},{\"x\":163.38,\"y\":14.4332},{\"x\":168.668,\"y\":9.87856},{\"x\":167.644,\"y\":10.0172},{\"x\":168.467,\"y\":11.0231},{\"x\":154.634,\"y\":15.3384},{\"x\":151.476,\"y\":20.9024},{\"x\":127.478,\"y\":17.4087},{\"x\":131.458,\"y\":19.4638},{\"x\":136.028,\"y\":20.3457},{\"x\":137.458,\"y\":19.5929},{\"x\":122.86,\"y\":15.2469},{\"x\":89.5206,\"y\":17.2558},{\"x\":65.0625,\"y\":2.41768},{\"x\":64.2487,\"y\":3.46088},{\"x\":149.793,\"y\":22.8616},{\"x\":142.459,\"y\":19.8012},{\"x\":138.462,\"y\":18.304},{\"x\":140.909,\"y\":18.3831},{\"x\":143.588,\"y\":21.0335},{\"x\":146.919,\"y\":23.3809},{\"x\":145.217,\"y\":22.1908},{\"x\":94.4981,\"y\":13.5677},{\"x\":81.4623,\"y\":22.3682},{\"x\":38.9272,\"y\":12.168},{\"x\":28.095,\"y\":9.74417},{\"x\":38.9512,\"y\":13.3908},{\"x\":19.0394,\"y\":11.3164},{\"x\":19.1251,\"y\":12.5829},{\"x\":28.1101,\"y\":12.7576},{\"x\":54.1331,\"y\":10.9901},{\"x\":76.3546,\"y\":7.31641},{\"x\":106.875,\"y\":12.7522},{\"x\":76.0706,\"y\":4.04133},{\"x\":5.46933,\"y\":10.7486},{\"x\":158.861,\"y\":11.831},{\"x\":159.168,\"y\":10.6395},{\"x\":91.4297,\"y\":15.1592},{\"x\":77.0625,\"y\":11.4258}],\"goldenCarrots\":[{\"x\":55.7174,\"y\":6.49266},{\"x\":104.691,\"y\":9.76957},{\"x\":148.853,\"y\":24.1561}],\"jons\":[{\"x\":3.42598,\"y\":9.92452}]}");
        }
    }
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1);
    
    if (!m_hasShownOpeningSequence)
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
            if (handleOpeningSequenceTouchInput(gs))
            {
                gs->m_renderer->zoomIn();
                m_hasOpeningSequenceCompleted = true;
                return;
            }
            
            Jon& jon = m_game->getJon();
            
            jon.update(gs->m_fDeltaTime, *m_game, false);
            
            m_hasOpeningSequenceCompleted = gs->m_renderer->updateCameraToFollowPathToJon(*m_game, gs->m_fDeltaTime);
            
            EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
            
            return;
        }
        
        if (handleTouchInput(gs))
        {
            return;
        }
        
        if (!m_hasPlayerCompletedLevel)
        {
            m_game->updateAndClean(gs->m_fDeltaTime);
        }
        
        Jon& jon = m_game->getJon();
        
        if (EntityUtils::isCollected(jon, m_game->getCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
        }
        
        if (EntityUtils::isCollected(jon, m_game->getGoldenCarrots(), gs->m_fDeltaTime))
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
        }
        
        if (jon.getPosition().getX() - jon.getWidth() > m_game->getFarRight())
        {
            m_hasPlayerCompletedLevel = true;
        }
        
        if (m_hasPlayerCompletedLevel)
        {
            m_fStateTime += gs->m_fDeltaTime;
        }
        
        if (jon.isDead() || m_fStateTime > 5)
        {
            m_game->reset();
            
            m_fStateTime = 0;
            m_hasPlayerCompletedLevel = false;
            
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
    m_hasPlayerCompletedLevel = false;
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
                if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getBounds()))
                {
                    Assets::getInstance()->setMusicId(MUSIC_STOP);
                    gs->m_stateMachine->revertToPreviousState();
                    return true;
                }
                
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

GamePlay::GamePlay() : m_sourceGame(nullptr), m_fStateTime(0.0f), m_hasShownOpeningSequence(false), m_hasOpeningSequenceCompleted(false), m_hasPlayerCompletedLevel(false)
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
                    m_levelSelectorPanel->openForMode(LEVEL_SELECTOR_PANEL_MODE_LOAD);
                    break;
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