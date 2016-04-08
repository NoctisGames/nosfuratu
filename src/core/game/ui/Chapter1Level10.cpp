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
    static Chapter1Level10 *instance = new Chapter1Level10("{\"world\":1,\"level\":10,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":60,\"gridY\":96,\"type\":0},{\"gridX\":188,\"gridY\":96,\"type\":1},{\"gridX\":216,\"gridY\":96,\"type\":0},{\"gridX\":160,\"gridY\":96,\"type\":2},{\"gridX\":252,\"gridY\":96,\"type\":0},{\"gridX\":292,\"gridY\":96,\"type\":2},{\"gridX\":352,\"gridY\":96,\"type\":2},{\"gridX\":412,\"gridY\":96,\"type\":1},{\"gridX\":440,\"gridY\":96,\"type\":2},{\"gridX\":496,\"gridY\":96,\"type\":0},{\"gridX\":508,\"gridY\":96,\"type\":0},{\"gridX\":712,\"gridY\":96,\"type\":0},{\"gridX\":748,\"gridY\":96,\"type\":1},{\"gridX\":784,\"gridY\":96,\"type\":2},{\"gridX\":844,\"gridY\":96,\"type\":1},{\"gridX\":880,\"gridY\":96,\"type\":0},{\"gridX\":900,\"gridY\":96,\"type\":2},{\"gridX\":856,\"gridY\":96,\"type\":2},{\"gridX\":980,\"gridY\":96,\"type\":1},{\"gridX\":1004,\"gridY\":96,\"type\":0},{\"gridX\":1024,\"gridY\":96,\"type\":1},{\"gridX\":1084,\"gridY\":96,\"type\":2},{\"gridX\":1096,\"gridY\":96,\"type\":2},{\"gridX\":1096,\"gridY\":96,\"type\":1},{\"gridX\":1096,\"gridY\":96,\"type\":0},{\"gridX\":1096,\"gridY\":96,\"type\":1},{\"gridX\":1096,\"gridY\":96,\"type\":0},{\"gridX\":1096,\"gridY\":96,\"type\":2},{\"gridX\":1096,\"gridY\":96,\"type\":2},{\"gridX\":1096,\"gridY\":96,\"type\":0},{\"gridX\":1116,\"gridY\":96,\"type\":1},{\"gridX\":1132,\"gridY\":96,\"type\":1},{\"gridX\":1128,\"gridY\":96,\"type\":0},{\"gridX\":1172,\"gridY\":96,\"type\":2},{\"gridX\":1208,\"gridY\":96,\"type\":1},{\"gridX\":1196,\"gridY\":96,\"type\":0},{\"gridX\":1232,\"gridY\":96,\"type\":0},{\"gridX\":1257,\"gridY\":96,\"type\":2},{\"gridX\":1297,\"gridY\":96,\"type\":1},{\"gridX\":1321,\"gridY\":96,\"type\":2},{\"gridX\":1361,\"gridY\":96,\"type\":2},{\"gridX\":1340,\"gridY\":96,\"type\":1},{\"gridX\":1393,\"gridY\":96,\"type\":1},{\"gridX\":1425,\"gridY\":96,\"type\":0},{\"gridX\":1508,\"gridY\":96,\"type\":2},{\"gridX\":1552,\"gridY\":96,\"type\":1},{\"gridX\":1536,\"gridY\":96,\"type\":0},{\"gridX\":1640,\"gridY\":96,\"type\":2},{\"gridX\":1700,\"gridY\":96,\"type\":2},{\"gridX\":1752,\"gridY\":96,\"type\":1},{\"gridX\":1792,\"gridY\":96,\"type\":0},{\"gridX\":1808,\"gridY\":96,\"type\":1},{\"gridX\":1888,\"gridY\":96,\"type\":2},{\"gridX\":1876,\"gridY\":96,\"type\":0},{\"gridX\":1924,\"gridY\":96,\"type\":0},{\"gridX\":1844,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":264,\"gridY\":50,\"type\":11},{\"gridX\":808,\"gridY\":50,\"type\":11},{\"gridX\":1736,\"gridY\":50,\"type\":11},{\"gridX\":1608,\"gridY\":50,\"type\":11},{\"gridX\":1704,\"gridY\":50,\"type\":11},{\"gridX\":2536,\"gridY\":50,\"type\":11},{\"gridX\":1640,\"gridY\":50,\"type\":12},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":136,\"gridY\":50,\"type\":13},{\"gridX\":552,\"gridY\":50,\"type\":13},{\"gridX\":680,\"gridY\":50,\"type\":13},{\"gridX\":1096,\"gridY\":50,\"type\":13},{\"gridX\":1224,\"gridY\":50,\"type\":13},{\"gridX\":1352,\"gridY\":50,\"type\":13},{\"gridX\":1480,\"gridY\":50,\"type\":13},{\"gridX\":2024,\"gridY\":50,\"type\":13},{\"gridX\":2152,\"gridY\":50,\"type\":13},{\"gridX\":2280,\"gridY\":50,\"type\":13},{\"gridX\":2408,\"gridY\":50,\"type\":13},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":1608,\"gridY\":90,\"type\":21},{\"gridX\":1704,\"gridY\":90,\"type\":21},{\"gridX\":2536,\"gridY\":90,\"type\":21},{\"gridX\":1640,\"gridY\":90,\"type\":22},{\"gridX\":1480,\"gridY\":90,\"type\":23},{\"gridX\":1096,\"gridY\":90,\"type\":23},{\"gridX\":552,\"gridY\":90,\"type\":23},{\"gridX\":2152,\"gridY\":90,\"type\":23},{\"gridX\":680,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":2024,\"gridY\":90,\"type\":23},{\"gridX\":2280,\"gridY\":90,\"type\":23},{\"gridX\":1352,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":1224,\"gridY\":90,\"type\":23},{\"gridX\":2408,\"gridY\":90,\"type\":23},{\"gridX\":1760,\"gridY\":0,\"type\":25},{\"gridX\":288,\"gridY\":0,\"type\":25},{\"gridX\":832,\"gridY\":0,\"type\":25},{\"gridX\":1896,\"gridY\":0,\"type\":28},{\"gridX\":840,\"gridY\":0,\"type\":28},{\"gridX\":968,\"gridY\":0,\"type\":28},{\"gridX\":1768,\"gridY\":0,\"type\":28},{\"gridX\":296,\"gridY\":0,\"type\":28},{\"gridX\":424,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[{\"gridX\":264,\"gridY\":88,\"type\":1},{\"gridX\":808,\"gridY\":88,\"type\":1},{\"gridX\":1736,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":1448,\"gridY\":73,\"type\":2},{\"gridX\":1433,\"gridY\":73,\"type\":1},{\"gridX\":1417,\"gridY\":73,\"type\":1},{\"gridX\":1401,\"gridY\":73,\"type\":1},{\"gridX\":1385,\"gridY\":73,\"type\":1},{\"gridX\":1369,\"gridY\":73,\"type\":1},{\"gridX\":1353,\"gridY\":73,\"type\":1},{\"gridX\":1337,\"gridY\":73,\"type\":1},{\"gridX\":1321,\"gridY\":73,\"type\":1},{\"gridX\":1305,\"gridY\":73,\"type\":1},{\"gridX\":1289,\"gridY\":73,\"type\":1},{\"gridX\":1273,\"gridY\":73,\"type\":1},{\"gridX\":1257,\"gridY\":73,\"type\":1},{\"gridX\":1241,\"gridY\":73,\"type\":0},{\"gridX\":116,\"gridY\":80,\"type\":3},{\"gridX\":596,\"gridY\":80,\"type\":3},{\"gridX\":2376,\"gridY\":73,\"type\":2},{\"gridX\":2360,\"gridY\":73,\"type\":1},{\"gridX\":2344,\"gridY\":73,\"type\":1},{\"gridX\":2328,\"gridY\":73,\"type\":1},{\"gridX\":2312,\"gridY\":73,\"type\":1},{\"gridX\":2296,\"gridY\":73,\"type\":1},{\"gridX\":2280,\"gridY\":73,\"type\":1},{\"gridX\":2264,\"gridY\":73,\"type\":1},{\"gridX\":2248,\"gridY\":73,\"type\":1},{\"gridX\":2232,\"gridY\":73,\"type\":1},{\"gridX\":2216,\"gridY\":73,\"type\":1},{\"gridX\":2200,\"gridY\":73,\"type\":1},{\"gridX\":2184,\"gridY\":73,\"type\":1},{\"gridX\":2168,\"gridY\":73,\"type\":0}],\"foregroundObjects\":[{\"gridX\":2614,\"gridY\":124,\"type\":12},{\"gridX\":272,\"gridY\":55,\"type\":19},{\"gridX\":816,\"gridY\":55,\"type\":19},{\"gridX\":1744,\"gridY\":55,\"type\":19},{\"gridX\":1614,\"gridY\":84,\"type\":29},{\"gridX\":158,\"gridY\":70,\"type\":4},{\"gridX\":1622,\"gridY\":84,\"type\":29},{\"gridX\":1631,\"gridY\":84,\"type\":29}],\"midBossForegroundObjects\":[{\"gridX\":136,\"gridY\":96,\"type\":37},{\"gridX\":612,\"gridY\":96,\"type\":39},{\"gridX\":2396,\"gridY\":96,\"type\":38},{\"gridX\":1468,\"gridY\":96,\"type\":38}],\"countHissWithMinas\":[{\"gridX\":2578,\"gridY\":122}],\"enemies\":[],\"collectibles\":[{\"gridX\":636,\"gridY\":64,\"type\":0},{\"gridX\":648,\"gridY\":64,\"type\":0},{\"gridX\":660,\"gridY\":64,\"type\":0},{\"gridX\":672,\"gridY\":78,\"type\":1},{\"gridX\":1614,\"gridY\":64,\"type\":0},{\"gridX\":1630,\"gridY\":64,\"type\":0},{\"gridX\":1622,\"gridY\":72,\"type\":1},{\"gridX\":2550,\"gridY\":74,\"type\":1}],\"jons\":[{\"gridX\":20,\"gridY\":112}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":680,\"gridY\":0,\"type\":0},{\"gridX\":1608,\"gridY\":0,\"type\":1},{\"gridX\":1640,\"gridY\":0,\"type\":0},{\"gridX\":2536,\"gridY\":0,\"type\":1},{\"gridX\":2568,\"gridY\":0,\"type\":0}]}");
    
    return instance;
}

void Chapter1Level10::enter(GameScreen* gs)
{
    Level::enter(gs);
    
    m_iLastKnownOwlDamage = 0;
    m_hasRequestedPart2TexturesToBeLoaded = false;
    m_hasTriggeredBurrow = false;
    
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
            jon.setIdle(true);
            
            Rectangle jonBounds = Rectangle(jon.getMainBounds().getLeft(), jon.getMainBounds().getBottom(), jon.getWidth() * 1.5f, jon.getHeight());
            
            jon.getPosition().set(m_perchTree->getMainBounds().getLeft() - jonBounds.getWidth() / 2, m_fJonY);
            jon.updateBounds();
            
            jon.setNumBoosts(1);
            
            m_midBossOwl->awaken();
            
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
                Rectangle jonBounds = Rectangle(jon.getPosition().getX() - jon.getWidth() / 2, jon.getPosition().getY() - jon.getHeight() / 2, jon.getWidth(), jon.getHeight());
                
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
                
                if (m_fIdleWaitTime > 1.0f)
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
                
                Assets::getInstance()->addSoundIdToPlayQueue(SOUND_MID_BOSS_LOOP_INTRO);
                
                m_hasTriggeredMidBossMusicLoopIntro = true;
            }
            
            if (!m_hasTriggeredBurrow)
            {
                jon.setIdle(false);
                jon.triggerDownAction();
                
                m_hasTriggeredBurrow = true;
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
            
            m_isChaseCamActivated = false;
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
    m_hasTriggeredBurrow = false;
    
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
m_hasRequestedPart2TexturesToBeLoaded(false),
m_hasTriggeredBurrow(false)
{
    m_midBossOwl = std::unique_ptr<MidBossOwl>(new MidBossOwl(0, 0));
}