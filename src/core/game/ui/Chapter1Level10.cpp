//
//  Chapter1Level10.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/7/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenLevels.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "MidBossOwl.h"
#include "Assets.h"

/// Chapter 1 Level 10 Mid Boss ///

Chapter1Level10 * Chapter1Level10::getInstance()
{
    static Chapter1Level10 *instance = new Chapter1Level10("{\"world\":1,\"level\":10,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":72,\"gridY\":96,\"type\":0},{\"gridX\":116,\"gridY\":96,\"type\":1},{\"gridX\":160,\"gridY\":96,\"type\":0},{\"gridX\":312,\"gridY\":96,\"type\":1},{\"gridX\":344,\"gridY\":96,\"type\":0},{\"gridX\":268,\"gridY\":96,\"type\":2},{\"gridX\":380,\"gridY\":96,\"type\":0},{\"gridX\":420,\"gridY\":96,\"type\":2},{\"gridX\":480,\"gridY\":96,\"type\":2},{\"gridX\":540,\"gridY\":96,\"type\":1},{\"gridX\":568,\"gridY\":96,\"type\":2},{\"gridX\":632,\"gridY\":96,\"type\":0},{\"gridX\":668,\"gridY\":96,\"type\":0},{\"gridX\":716,\"gridY\":96,\"type\":2},{\"gridX\":788,\"gridY\":96,\"type\":1},{\"gridX\":968,\"gridY\":96,\"type\":0},{\"gridX\":1004,\"gridY\":96,\"type\":1},{\"gridX\":1040,\"gridY\":96,\"type\":2},{\"gridX\":1100,\"gridY\":96,\"type\":1},{\"gridX\":1136,\"gridY\":96,\"type\":0},{\"gridX\":1156,\"gridY\":96,\"type\":2},{\"gridX\":1112,\"gridY\":96,\"type\":2},{\"gridX\":1236,\"gridY\":96,\"type\":1},{\"gridX\":1260,\"gridY\":96,\"type\":0},{\"gridX\":1280,\"gridY\":96,\"type\":1},{\"gridX\":1340,\"gridY\":96,\"type\":2},{\"gridX\":1420,\"gridY\":96,\"type\":2},{\"gridX\":1388,\"gridY\":96,\"type\":1},{\"gridX\":1488,\"gridY\":96,\"type\":0},{\"gridX\":1512,\"gridY\":96,\"type\":1},{\"gridX\":1544,\"gridY\":96,\"type\":0},{\"gridX\":1600,\"gridY\":96,\"type\":2},{\"gridX\":1688,\"gridY\":96,\"type\":2},{\"gridX\":1656,\"gridY\":96,\"type\":0},{\"gridX\":1756,\"gridY\":96,\"type\":1},{\"gridX\":1772,\"gridY\":96,\"type\":1},{\"gridX\":1768,\"gridY\":96,\"type\":0},{\"gridX\":1812,\"gridY\":96,\"type\":2},{\"gridX\":1848,\"gridY\":96,\"type\":1},{\"gridX\":1836,\"gridY\":96,\"type\":0},{\"gridX\":1872,\"gridY\":96,\"type\":0},{\"gridX\":1900,\"gridY\":96,\"type\":2},{\"gridX\":1940,\"gridY\":96,\"type\":1},{\"gridX\":1964,\"gridY\":96,\"type\":2},{\"gridX\":2004,\"gridY\":96,\"type\":2},{\"gridX\":1984,\"gridY\":96,\"type\":1},{\"gridX\":2036,\"gridY\":96,\"type\":1},{\"gridX\":2068,\"gridY\":96,\"type\":0},{\"gridX\":2148,\"gridY\":96,\"type\":2},{\"gridX\":2192,\"gridY\":96,\"type\":1},{\"gridX\":2176,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":392,\"gridY\":50,\"type\":11},{\"gridX\":1064,\"gridY\":50,\"type\":11},{\"gridX\":2376,\"gridY\":50,\"type\":11},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":136,\"gridY\":50,\"type\":13},{\"gridX\":264,\"gridY\":50,\"type\":13},{\"gridX\":680,\"gridY\":50,\"type\":13},{\"gridX\":808,\"gridY\":50,\"type\":13},{\"gridX\":936,\"gridY\":50,\"type\":13},{\"gridX\":1352,\"gridY\":50,\"type\":13},{\"gridX\":1480,\"gridY\":50,\"type\":13},{\"gridX\":1608,\"gridY\":50,\"type\":13},{\"gridX\":1736,\"gridY\":50,\"type\":13},{\"gridX\":1864,\"gridY\":50,\"type\":13},{\"gridX\":1992,\"gridY\":50,\"type\":13},{\"gridX\":2120,\"gridY\":50,\"type\":13},{\"gridX\":2248,\"gridY\":50,\"type\":13},{\"gridX\":2664,\"gridY\":50,\"type\":13},{\"gridX\":2792,\"gridY\":50,\"type\":13},{\"gridX\":2920,\"gridY\":50,\"type\":13},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":1736,\"gridY\":90,\"type\":23},{\"gridX\":1864,\"gridY\":90,\"type\":23},{\"gridX\":1992,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":2120,\"gridY\":90,\"type\":23},{\"gridX\":680,\"gridY\":90,\"type\":23},{\"gridX\":2248,\"gridY\":90,\"type\":23},{\"gridX\":808,\"gridY\":90,\"type\":23},{\"gridX\":936,\"gridY\":90,\"type\":23},{\"gridX\":1352,\"gridY\":90,\"type\":23},{\"gridX\":2664,\"gridY\":90,\"type\":23},{\"gridX\":1480,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":1608,\"gridY\":90,\"type\":23},{\"gridX\":2792,\"gridY\":90,\"type\":23},{\"gridX\":2920,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":2400,\"gridY\":0,\"type\":25},{\"gridX\":416,\"gridY\":0,\"type\":25},{\"gridX\":1088,\"gridY\":0,\"type\":25},{\"gridX\":2536,\"gridY\":0,\"type\":28},{\"gridX\":552,\"gridY\":0,\"type\":28},{\"gridX\":1096,\"gridY\":0,\"type\":28},{\"gridX\":1224,\"gridY\":0,\"type\":28},{\"gridX\":2408,\"gridY\":0,\"type\":28},{\"gridX\":424,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[{\"gridX\":392,\"gridY\":88,\"type\":1},{\"gridX\":1064,\"gridY\":88,\"type\":1},{\"gridX\":2376,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":2092,\"gridY\":73,\"type\":2},{\"gridX\":2076,\"gridY\":73,\"type\":1},{\"gridX\":2060,\"gridY\":73,\"type\":1},{\"gridX\":2044,\"gridY\":73,\"type\":1},{\"gridX\":2028,\"gridY\":73,\"type\":1},{\"gridX\":2012,\"gridY\":73,\"type\":1},{\"gridX\":1996,\"gridY\":73,\"type\":1},{\"gridX\":1980,\"gridY\":73,\"type\":1},{\"gridX\":1964,\"gridY\":73,\"type\":1},{\"gridX\":1948,\"gridY\":73,\"type\":1},{\"gridX\":1932,\"gridY\":73,\"type\":1},{\"gridX\":1916,\"gridY\":73,\"type\":1},{\"gridX\":1900,\"gridY\":73,\"type\":1},{\"gridX\":1884,\"gridY\":73,\"type\":0},{\"gridX\":208,\"gridY\":80,\"type\":3},{\"gridX\":852,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":2912,\"gridY\":96,\"type\":12},{\"gridX\":404,\"gridY\":55,\"type\":19},{\"gridX\":1076,\"gridY\":55,\"type\":19},{\"gridX\":2388,\"gridY\":55,\"type\":19}],\"midBossForegroundObjects\":[{\"gridX\":225,\"gridY\":96,\"type\":37},{\"gridX\":868,\"gridY\":96,\"type\":38},{\"gridX\":2108,\"gridY\":96,\"type\":37}],\"countHissWithMinas\":[{\"gridX\":2876,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":892,\"gridY\":64,\"type\":0},{\"gridX\":904,\"gridY\":64,\"type\":0},{\"gridX\":916,\"gridY\":64,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":104}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":936,\"gridY\":0,\"type\":0},{\"gridX\":2248,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

void Chapter1Level10::enter(GameScreen* gs)
{
    Level::enter(gs);
    
    m_iLastKnownOwlDamage = 0;
    m_hasRequestedPart2TexturesToBeLoaded = false;
    
    for (std::vector<ForegroundObject*>::iterator i = m_game->getMidBossForegroundObjects().begin(); i != m_game->getMidBossForegroundObjects().end(); i++)
    {
        if ((*i)->getType() == ForegroundObjectType_GiantPerchTree)
        {
            m_perchTree = (*i);
            Vector2D lowerLeft = Vector2D(m_perchTree->getPosition().getX() - m_perchTree->getWidth() / 2, m_perchTree->getPosition().getY() - m_perchTree->getHeight() / 2);
            m_midBossOwl->getPosition().set(lowerLeft.getX() + MID_BOSS_OWL_SLEEPING_WIDTH / 2, lowerLeft.getY() + MID_BOSS_OWL_SLEEPING_HEIGHT / 2);
            m_midBossOwl->updateBounds();
            m_midBossOwl->goBackToSleep();
        }
    }
    
    m_midBossOwl->setGame(m_game.get());
    
    if (m_hasTriggeredMidBossMusicLoopIntro)
    {
        if (m_game->getJons().size() > 0)
        {
            Jon& jon = m_game->getJon();
            
            jon.getAcceleration().set(0, 0);
            jon.getVelocity().set(0, 0);
            jon.setIdle(false);
            
            Rectangle jonBounds = Rectangle(jon.getMainBounds().getLeft(), jon.getMainBounds().getBottom(), jon.getWidth() * 1.5f, jon.getHeight());
            
            jon.getPosition().set(m_perchTree->getMainBounds().getLeft() - jonBounds.getWidth() / 2, m_fJonY);
            jon.updateBounds();
            
            jon.setNumBoosts(1);
            
            jon.triggerDownAction();
            
            m_midBossOwl->setState(MidBossOwlState_Screeching);
            
            m_game->setStateTime(m_fGameStateTime);
        }
    }
}

void Chapter1Level10::execute(GameScreen* gs)
{
    Level::execute(gs);
    
    if (!gs->m_isRequestingRender)
    {
        if (m_game->getJons().size() == 0)
        {
            return;
        }
        
        m_midBossOwl->update(gs->m_fDeltaTime);
        
        Jon& jon = m_game->getJon();
        
        if (m_midBossOwl->getState() == MidBossOwlState_Sleeping)
        {
            if (jon.getNumBoosts() >= 1)
            {
                m_fMusicVolume -= gs->m_fDeltaTime / 8;
				if (m_fMusicVolume < 0)
				{
					m_fMusicVolume = 0;
				}

                short musicId = MUSIC_SET_VOLUME * 1000 + (short) (m_fMusicVolume * 100);
                Assets::getInstance()->setMusicId(musicId);
            }
            
            if (m_perchTree)
            {
                Rectangle jonBounds = Rectangle(jon.getMainBounds().getLeft(), jon.getMainBounds().getBottom(), jon.getWidth() * 1.5f, jon.getHeight());
                
                if (OverlapTester::doRectanglesOverlap(m_perchTree->getMainBounds(), jonBounds))
                {
                    if (jon.isTransformingIntoVampire())
                    {
                        jon.triggerCancelTransform();
                    }
                    
                    if (jon.isVampire())
                    {
                        jon.triggerTransform();
                    }
                    
                    jon.getAcceleration().set(0, 0);
                    jon.getVelocity().set(0, 0);
                    jon.setIdle(true);
                    
                    jon.getPosition().setX(m_perchTree->getMainBounds().getLeft() - jonBounds.getWidth() / 2);
                    jon.updateBounds();
                    
                    m_isIdleWaitingForOwl = true;
                }
            }
            
            if (m_isIdleWaitingForOwl)
            {
                m_fIdleWaitTime += gs->m_fDeltaTime;
                
                if (m_fIdleWaitTime > 2.0f)
                {
                    m_midBossOwl->awaken();
                }
            }
            
            m_isChaseCamActivated = false;
        }
        else if (m_midBossOwl->getState() == MidBossOwlState_Awakening)
        {
            m_isChaseCamActivated = false;
        }
        else if (m_midBossOwl->getState() == MidBossOwlState_Screeching)
        {
            if (!m_hasTriggeredMidBossMusicLoopIntro)
            {
                m_fJonY = jon.getPosition().getY();
                m_fGameStateTime = m_game->getStateTime();
                
                jon.setIdle(false);
                jon.triggerDownAction();
                
                Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MID_BOSS_LOOP_INTRO);
                
                m_hasTriggeredMidBossMusicLoopIntro = true;
            }
            
            if (!m_hasTriggeredMidBossMusicLoop && m_midBossOwl->getStateTime() > 4.80f)
            {
                Assets::getInstance()->setMusicId(MUSIC_PLAY_MID_BOSS_LOOP);
                
                m_hasTriggeredMidBossMusicLoop = true;
            }
            
            if (jon.getNumBoosts() >= 2)
            {
                m_midBossOwl->beginPursuit();
            }
            
            m_isChaseCamActivated = false;
        }
        else if (m_midBossOwl->getState() == MidBossOwlState_Pursuing)
        {
            m_isChaseCamActivated = jon.getPosition().getY() > 12;
        }
        else if (m_midBossOwl->getState() == MidBossOwlState_SlammingIntoTree)
        {
            m_isChaseCamActivated = jon.getPosition().getY() > 12;
            
            if (m_midBossOwl->getDamage() > m_iLastKnownOwlDamage)
            {
                m_iLastKnownOwlDamage = m_midBossOwl->getDamage();
                
                m_exitLoop = true;
            }
        }
        else if (m_midBossOwl->getState() == MidBossOwlState_FlyingOverTree)
        {
            if (!m_hasRequestedPart2TexturesToBeLoaded && m_midBossOwl->getDamage() == 1)
            {
                gs->m_renderer->init(RENDERER_TYPE_WORLD_1_MID_BOSS_PART_2);
                
                m_hasRequestedPart2TexturesToBeLoaded = true;
            }
        }
        else if (m_midBossOwl->getState() == MidBossOwlState_Dead)
        {
            m_isChaseCamActivated = false;
        }
    }
}

void Chapter1Level10::exit(GameScreen* gs)
{
    m_isIdleWaitingForOwl = false;
    m_fJonY = 0.0f;
    m_fGameStateTime = 0.0f;
    m_fIdleWaitTime = 0.0f;
    m_perchTree = nullptr;
    m_fMusicVolume = 0.5f;
    m_iLastKnownOwlDamage = 0;
    m_hasTriggeredMidBossMusicLoopIntro = false;
    m_hasTriggeredMidBossMusicLoop = false;
    m_isChaseCamActivated = false;
    m_hasRequestedPart2TexturesToBeLoaded = false;
    
    Level::exit(gs);
}

void Chapter1Level10::updateCamera(GameScreen* gs, bool instant)
{
    if (m_isChaseCamActivated)
    {
        gs->m_renderer->updateCameraToFollowJon(*m_game, gs->m_fDeltaTime, true, instant);
    }
    else
    {
        Level::updateCamera(gs, instant);
    }
}

void Chapter1Level10::additionalRenderingBeforeHud(GameScreen* gs)
{
    gs->m_renderer->renderMidBossOwl(*m_midBossOwl);
}

Chapter1Level10::Chapter1Level10(const char* json) : Level(json),
m_perchTree(nullptr),
m_fJonY(0),
m_fGameStateTime(0.0f),
m_fIdleWaitTime(0.0f),
m_fMusicVolume(0.5f),
m_iLastKnownOwlDamage(0),
m_isIdleWaitingForOwl(false),
m_hasTriggeredMidBossMusicLoopIntro(false),
m_hasTriggeredMidBossMusicLoop(false),
m_isChaseCamActivated(false),
m_hasRequestedPart2TexturesToBeLoaded(false)
{
    m_midBossOwl = std::unique_ptr<MidBossOwl>(new MidBossOwl(0, 0));
}