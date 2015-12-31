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
        
        gs->m_renderer->endFrame();
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
            m_game->load("{\"backgroundSkies\":[{\"x\":8,\"y\":19.9809},{\"x\":24,\"y\":19.9809},{\"x\":40,\"y\":19.9809}],\"backgroundTrees\":[{\"x\":8,\"y\":14.9623},{\"x\":24,\"y\":14.9623},{\"x\":40,\"y\":14.9623}],\"backgroundCaves\":[{\"x\":8,\"y\":5.63865},{\"x\":24,\"y\":5.63865},{\"x\":40,\"y\":5.63865}],\"trees\":[{\"x\":5.625,\"y\":14.1352,\"type\":1},{\"x\":15.2109,\"y\":13.8236,\"type\":2},{\"x\":27.1875,\"y\":14.1352,\"type\":1},{\"x\":37.7812,\"y\":13.0566,\"type\":0},{\"x\":74.5547,\"y\":13.0566,\"type\":0},{\"x\":83.625,\"y\":13.0566,\"type\":0},{\"x\":92.0157,\"y\":14.1352,\"type\":1},{\"x\":103.617,\"y\":13.0566,\"type\":0},{\"x\":113.484,\"y\":14.1352,\"type\":1},{\"x\":123.328,\"y\":13.8236,\"type\":2},{\"x\":131.508,\"y\":13.8587,\"type\":2},{\"x\":140.297,\"y\":13.8236,\"type\":2},{\"x\":150.891,\"y\":14.1352,\"type\":1},{\"x\":161.18,\"y\":13.8236,\"type\":2},{\"x\":168.234,\"y\":13.0566,\"type\":0},{\"x\":177.117,\"y\":14.1352,\"type\":1}],\"grounds\":[{\"x\":11.9958,\"y\":8.91611,\"type\":0},{\"x\":-0.852131,\"y\":8.91611,\"type\":3},{\"x\":35.9658,\"y\":8.91611,\"type\":1},{\"x\":42.7614,\"y\":8.91611,\"type\":4},{\"x\":23.2266,\"y\":2.247,\"type\":10},{\"x\":35.9786,\"y\":2.247,\"type\":14},{\"x\":83.7188,\"y\":5.0992,\"type\":5},{\"x\":70.9907,\"y\":5.0992,\"type\":8},{\"x\":10.4745,\"y\":2.247,\"type\":13},{\"x\":107.689,\"y\":2.247,\"type\":10},{\"x\":101.696,\"y\":8.91611,\"type\":1},{\"x\":113.681,\"y\":8.91611,\"type\":1},{\"x\":131.659,\"y\":8.91611,\"type\":0},{\"x\":131.659,\"y\":2.247,\"type\":10},{\"x\":141.903,\"y\":3.16378,\"type\":18},{\"x\":149.636,\"y\":8.91611,\"type\":1},{\"x\":155.629,\"y\":2.247,\"type\":10},{\"x\":148.992,\"y\":3.16378,\"type\":16},{\"x\":156.081,\"y\":3.16378,\"type\":19},{\"x\":158.625,\"y\":8.91611,\"type\":2},{\"x\":176.602,\"y\":5.0992,\"type\":7},{\"x\":172.863,\"y\":5.0992,\"type\":8},{\"x\":198.397,\"y\":5.0992,\"type\":9},{\"x\":170.61,\"y\":2.247,\"type\":12},{\"x\":185.591,\"y\":5.0992,\"type\":6},{\"x\":194.58,\"y\":5.0992,\"type\":7}],\"holes\":[{\"x\":19.3828,\"y\":8.09532},{\"x\":105.938,\"y\":8.09532},{\"x\":109.289,\"y\":8.09532},{\"x\":112.594,\"y\":8.09532}],\"caveExits\":[{\"x\":26.9771,\"y\":9.34754,\"type\":1},{\"x\":170.61,\"y\":9.34754,\"type\":0},{\"x\":164.618,\"y\":9.34754,\"type\":1}],\"logVerticalTalls\":[{\"x\":189.492,\"y\":10.486}],\"logVerticalShorts\":[{\"x\":193.992,\"y\":9.64714}],\"thorns\":[{\"x\":32.1328,\"y\":9.69098},{\"x\":35.0625,\"y\":9.69098},{\"x\":116.32,\"y\":9.69098},{\"x\":124.828,\"y\":9.69098},{\"x\":134.133,\"y\":9.69098},{\"x\":143.25,\"y\":9.69098}],\"stumps\":[{\"x\":23.5781,\"y\":10.549}],\"sideSpikes\":[{\"x\":140.391,\"y\":4.86914},{\"x\":140.438,\"y\":4.28907}],\"upwardSpikes\":[{\"x\":89.7422,\"y\":9.48092,\"type\":1},{\"x\":91.0312,\"y\":9.48092,\"type\":1},{\"x\":92.3906,\"y\":9.48092,\"type\":1},{\"x\":105.352,\"y\":1.69091,\"type\":2},{\"x\":105.844,\"y\":1.69091,\"type\":2},{\"x\":106.477,\"y\":1.69091,\"type\":2},{\"x\":107.227,\"y\":1.69091,\"type\":2},{\"x\":107.883,\"y\":1.69091,\"type\":2},{\"x\":108.633,\"y\":1.69091,\"type\":2},{\"x\":109.5,\"y\":1.69091,\"type\":2},{\"x\":110.227,\"y\":1.69091,\"type\":2},{\"x\":119.109,\"y\":9.45185,\"type\":0},{\"x\":120.516,\"y\":9.45185,\"type\":0},{\"x\":122.062,\"y\":9.45185,\"type\":0},{\"x\":127.758,\"y\":9.45185,\"type\":0},{\"x\":129.281,\"y\":9.45185,\"type\":0},{\"x\":130.734,\"y\":9.45185,\"type\":0},{\"x\":130.289,\"y\":1.69091,\"type\":2},{\"x\":131.227,\"y\":1.69091,\"type\":2},{\"x\":132.141,\"y\":1.69091,\"type\":2},{\"x\":133.125,\"y\":1.69091,\"type\":2},{\"x\":137.438,\"y\":9.45185,\"type\":0},{\"x\":138.797,\"y\":9.45185,\"type\":0},{\"x\":140.227,\"y\":9.45185,\"type\":0}],\"jumpSprings\":[{\"x\":30.0468,\"y\":16.6257,\"type\":0},{\"x\":26.9062,\"y\":2.46912,\"type\":2},{\"x\":164.531,\"y\":1.66619,\"type\":0},{\"x\":171.31,\"y\":1.69016,\"type\":1}],\"rocks\":[{\"x\":86.7187,\"y\":10.2212,\"type\":1}],\"platforms\":[{\"x\":30,\"y\":15.7676,\"type\":0},{\"x\":46.2656,\"y\":17.4375,\"type\":2},{\"x\":48.9503,\"y\":17.4375,\"type\":2},{\"x\":51.6349,\"y\":17.4375,\"type\":2},{\"x\":54.3196,\"y\":17.4375,\"type\":2},{\"x\":57.0042,\"y\":17.4375,\"type\":2},{\"x\":41.8971,\"y\":17.4375,\"type\":1},{\"x\":61.3727,\"y\":17.4375,\"type\":3},{\"x\":54.7266,\"y\":21.9902,\"type\":1},{\"x\":56.4105,\"y\":21.9902,\"type\":2},{\"x\":63.4636,\"y\":21.9902,\"type\":3},{\"x\":59.0951,\"y\":21.9902,\"type\":2},{\"x\":61.7797,\"y\":21.9902,\"type\":2},{\"x\":59.6888,\"y\":17.4375,\"type\":2},{\"x\":43.581,\"y\":17.4375,\"type\":2},{\"x\":81.4219,\"y\":12.0937,\"type\":0},{\"x\":119.953,\"y\":3.60352,\"type\":4},{\"x\":121.679,\"y\":3.60352,\"type\":5},{\"x\":124.364,\"y\":3.60352,\"type\":5},{\"x\":128.774,\"y\":3.60352,\"type\":6},{\"x\":127.049,\"y\":3.60352,\"type\":5},{\"x\":98.1599,\"y\":11.6367,\"type\":1},{\"x\":99.8438,\"y\":11.6367,\"type\":2},{\"x\":102.528,\"y\":11.6367,\"type\":2},{\"x\":104.212,\"y\":11.6367,\"type\":3},{\"x\":106.57,\"y\":14.5898,\"type\":1},{\"x\":108.254,\"y\":14.5898,\"type\":2},{\"x\":109.938,\"y\":14.5898,\"type\":3}],\"endSigns\":[{\"x\":196.453,\"y\":9.5228}],\"carrots\":[{\"x\":9.82031,\"y\":10.2973},{\"x\":12.4219,\"y\":10.2973},{\"x\":15.7969,\"y\":10.2973},{\"x\":19.4297,\"y\":6.41601},{\"x\":19.4297,\"y\":2.44223},{\"x\":19.4062,\"y\":4.48242},{\"x\":44.8594,\"y\":14.4492},{\"x\":46.4531,\"y\":14.1328},{\"x\":60.2578,\"y\":12.041},{\"x\":49.6406,\"y\":13.6582},{\"x\":51.1641,\"y\":13.3242},{\"x\":52.6641,\"y\":13.1484},{\"x\":54.375,\"y\":12.9023},{\"x\":58.8984,\"y\":12.2871},{\"x\":48.1172,\"y\":13.9043},{\"x\":57.5625,\"y\":12.5508},{\"x\":56.0625,\"y\":12.6211},{\"x\":61.7578,\"y\":11.707},{\"x\":63.2109,\"y\":11.4258},{\"x\":64.9453,\"y\":11.2148},{\"x\":66.6797,\"y\":11.2324},{\"x\":73.8984,\"y\":10.2973},{\"x\":76.1953,\"y\":10.2973},{\"x\":78.6094,\"y\":10.2973},{\"x\":112.477,\"y\":2.44223},{\"x\":112.5,\"y\":4.3418},{\"x\":112.406,\"y\":6.53907},{\"x\":122.976,\"y\":5.23761},{\"x\":124.875,\"y\":5.23761},{\"x\":126.82,\"y\":5.23761},{\"x\":152.461,\"y\":6.62863},{\"x\":148.57,\"y\":6.62863},{\"x\":144.398,\"y\":6.62863},{\"x\":164.508,\"y\":3.46176},{\"x\":170.625,\"y\":5.21957},{\"x\":170.648,\"y\":3.12891},{\"x\":186.188,\"y\":9.73484},{\"x\":186.234,\"y\":10.7848}],\"goldenCarrots\":[{\"x\":68.7421,\"y\":11.0918},{\"x\":62.5078,\"y\":23.4325},{\"x\":172.781,\"y\":20.707}],\"snakeGruntEnemies\":[{\"x\":82.1718,\"y\":10.2943},{\"x\":58.4297,\"y\":23.6213},{\"x\":60.539,\"y\":23.6212},{\"x\":62.5547,\"y\":23.6212},{\"x\":146.695,\"y\":6.62557},{\"x\":150.68,\"y\":6.62557},{\"x\":154.594,\"y\":6.62557},{\"x\":98.8594,\"y\":13.2677},{\"x\":100.078,\"y\":13.2677}],\"snakeHornedEnemies\":[{\"x\":56.2969,\"y\":24.2204},{\"x\":121.711,\"y\":3.03836}],\"jons\":[{\"x\":2.67188,\"y\":9.9053}]}");
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
        
        if (m_isReleasingShockwave)
        {
            gs->m_renderer->renderToScreenWithShockwave(m_fShockwaveCenterX, m_fShockwaveCenterY, m_fShockwaveElapsedTime);
        }
        else
        {
            gs->m_renderer->renderToScreen();
        }
        
        gs->m_renderer->renderJon(*m_game);
        
        gs->m_renderer->renderHud(*m_game, *m_backButton);
        
        gs->m_renderer->endFrame();
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
            if (jon.isTransformingIntoVampire() || jon.isRevertingToRabbit())
            {
                if (jon.getTransformStateTime() < 0.125f)
                {
                    gs->m_fDeltaTime /= 8;
                }
                else
                {
                    if (!m_isReleasingShockwave)
                    {
                        m_fShockwaveCenterX = jon.getPosition().getX();
                        m_fShockwaveCenterY = jon.getPosition().getY();
                        m_fShockwaveElapsedTime = 0.0f;
                        m_isReleasingShockwave = true;
                    }
                }
            }
            
            if (m_isReleasingShockwave)
            {
                m_fShockwaveElapsedTime += gs->m_fDeltaTime;
                
                if (m_fShockwaveElapsedTime > 4)
                {
                    m_fShockwaveElapsedTime = 0;
                    m_isReleasingShockwave = false;
                }
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
            
            if (gs->m_fScreenHeldTime > 0.4f)
            {
                jon.triggerTransform();
                gs->m_isScreenHeldDown = false;
                gs->m_fScreenHeldTime = 0;
                m_fShockwaveElapsedTime = 0;
                m_isReleasingShockwave = false;
            }
        }
        
        if (jon.getState() == JON_DYING_FADING && (jon.isTransformingIntoVampire() || jon.isRevertingToRabbit()))
        {
            jon.triggerCancelTransform();
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
    m_isReleasingShockwave = false;
    m_fShockwaveElapsedTime = 0;
    m_fShockwaveCenterX = 0;
    m_fShockwaveCenterY = 0;
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
                    
                    if (hasSwiped)
                    {
                        gs->m_fScreenHeldTime = 0.0f;
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
                
                if ((jon.isTransformingIntoVampire() || jon.isRevertingToRabbit()) && jon.getTransformStateTime() < 0.125f)
                {
                    jon.triggerCancelTransform();
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                
                break;
        }
    }
    
    return false;
}

GamePlay::GamePlay() : m_sourceGame(nullptr), m_fStateTime(0.0f), m_isReleasingShockwave(false), m_fShockwaveElapsedTime(0.0f), m_fShockwaveCenterX(0.0f), m_fShockwaveCenterY(0.0f), m_hasShownOpeningSequence(false), m_hasOpeningSequenceCompleted(false)
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
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->renderLevelEditor(*m_levelEditorActionsPanel, *m_levelEditorEntitiesPanel, *m_trashCan, *m_levelSelectorPanel);
        
        gs->m_renderer->endFrame();
    }
    else
    {
        handleTouchInput(gs);
        
        int oldSum = m_game->calcSum();
        
        m_game->updateAndClean(gs->m_fDeltaTime / 8);
        
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