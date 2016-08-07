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

/// Chapter 1 Level 10 ///

Chapter1Level10 * Chapter1Level10::getInstance()
{
    static Chapter1Level10 *instance = new Chapter1Level10("{\"world\":1,\"level\":10,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":60,\"gridY\":96,\"type\":0},{\"gridX\":188,\"gridY\":96,\"type\":1},{\"gridX\":216,\"gridY\":96,\"type\":0},{\"gridX\":160,\"gridY\":96,\"type\":2},{\"gridX\":252,\"gridY\":96,\"type\":0},{\"gridX\":292,\"gridY\":96,\"type\":2},{\"gridX\":352,\"gridY\":96,\"type\":2},{\"gridX\":412,\"gridY\":96,\"type\":1},{\"gridX\":440,\"gridY\":96,\"type\":2},{\"gridX\":496,\"gridY\":96,\"type\":0},{\"gridX\":508,\"gridY\":96,\"type\":0},{\"gridX\":712,\"gridY\":96,\"type\":0},{\"gridX\":748,\"gridY\":96,\"type\":1},{\"gridX\":784,\"gridY\":96,\"type\":2},{\"gridX\":844,\"gridY\":96,\"type\":1},{\"gridX\":880,\"gridY\":96,\"type\":0},{\"gridX\":900,\"gridY\":96,\"type\":2},{\"gridX\":856,\"gridY\":96,\"type\":2},{\"gridX\":980,\"gridY\":96,\"type\":1},{\"gridX\":1004,\"gridY\":96,\"type\":0},{\"gridX\":1024,\"gridY\":96,\"type\":1},{\"gridX\":1090,\"gridY\":96,\"type\":2},{\"gridX\":1068,\"gridY\":96,\"type\":1},{\"gridX\":1052,\"gridY\":96,\"type\":0},{\"gridX\":1124,\"gridY\":96,\"type\":1},{\"gridX\":1144,\"gridY\":96,\"type\":1},{\"gridX\":1118,\"gridY\":96,\"type\":0},{\"gridX\":1172,\"gridY\":96,\"type\":2},{\"gridX\":1208,\"gridY\":96,\"type\":1},{\"gridX\":1196,\"gridY\":96,\"type\":0},{\"gridX\":1232,\"gridY\":96,\"type\":0},{\"gridX\":1257,\"gridY\":96,\"type\":2},{\"gridX\":1297,\"gridY\":96,\"type\":1},{\"gridX\":1321,\"gridY\":96,\"type\":2},{\"gridX\":1361,\"gridY\":96,\"type\":2},{\"gridX\":1340,\"gridY\":96,\"type\":1},{\"gridX\":1393,\"gridY\":96,\"type\":1},{\"gridX\":1425,\"gridY\":96,\"type\":0},{\"gridX\":1508,\"gridY\":96,\"type\":2},{\"gridX\":1552,\"gridY\":96,\"type\":1},{\"gridX\":1536,\"gridY\":96,\"type\":0},{\"gridX\":1768,\"gridY\":96,\"type\":2},{\"gridX\":1813,\"gridY\":96,\"type\":2},{\"gridX\":1853,\"gridY\":96,\"type\":1},{\"gridX\":1920,\"gridY\":96,\"type\":0},{\"gridX\":1883,\"gridY\":96,\"type\":1},{\"gridX\":1921,\"gridY\":96,\"type\":2},{\"gridX\":1968,\"gridY\":96,\"type\":0},{\"gridX\":2002,\"gridY\":96,\"type\":0},{\"gridX\":1944,\"gridY\":96,\"type\":0},{\"gridX\":1616,\"gridY\":96,\"type\":0},{\"gridX\":1660,\"gridY\":96,\"type\":0},{\"gridX\":1708,\"gridY\":96,\"type\":1},{\"gridX\":2039,\"gridY\":96,\"type\":1},{\"gridX\":2085,\"gridY\":96,\"type\":2},{\"gridX\":2146,\"gridY\":96,\"type\":1},{\"gridX\":2115,\"gridY\":96,\"type\":1},{\"gridX\":2065,\"gridY\":96,\"type\":0},{\"gridX\":2196,\"gridY\":96,\"type\":1},{\"gridX\":2224,\"gridY\":96,\"type\":2},{\"gridX\":2263,\"gridY\":96,\"type\":0},{\"gridX\":2304,\"gridY\":96,\"type\":0},{\"gridX\":2271,\"gridY\":96,\"type\":2},{\"gridX\":2347,\"gridY\":96,\"type\":1},{\"gridX\":2397,\"gridY\":96,\"type\":2},{\"gridX\":2327,\"gridY\":96,\"type\":0},{\"gridX\":2166,\"gridY\":96,\"type\":2},{\"gridX\":2373,\"gridY\":96,\"type\":1},{\"gridX\":2458,\"gridY\":96,\"type\":1},{\"gridX\":2440,\"gridY\":96,\"type\":0},{\"gridX\":2537,\"gridY\":96,\"type\":0},{\"gridX\":2491,\"gridY\":96,\"type\":0},{\"gridX\":2557,\"gridY\":96,\"type\":2},{\"gridX\":2619,\"gridY\":96,\"type\":1},{\"gridX\":2668,\"gridY\":96,\"type\":2},{\"gridX\":2732,\"gridY\":96,\"type\":2},{\"gridX\":2771,\"gridY\":96,\"type\":0},{\"gridX\":2792,\"gridY\":96,\"type\":1},{\"gridX\":2823,\"gridY\":96,\"type\":2},{\"gridX\":2869,\"gridY\":96,\"type\":0},{\"gridX\":2892,\"gridY\":96,\"type\":2},{\"gridX\":2922,\"gridY\":96,\"type\":1},{\"gridX\":2944,\"gridY\":96,\"type\":1},{\"gridX\":2971,\"gridY\":96,\"type\":0},{\"gridX\":2990,\"gridY\":96,\"type\":1},{\"gridX\":3018,\"gridY\":96,\"type\":2},{\"gridX\":3098,\"gridY\":96,\"type\":2},{\"gridX\":3177,\"gridY\":96,\"type\":2},{\"gridX\":3255,\"gridY\":96,\"type\":1},{\"gridX\":3275,\"gridY\":96,\"type\":1},{\"gridX\":3269,\"gridY\":96,\"type\":0},{\"gridX\":3208,\"gridY\":96,\"type\":2},{\"gridX\":3147,\"gridY\":96,\"type\":0},{\"gridX\":3068,\"gridY\":96,\"type\":0},{\"gridX\":3324,\"gridY\":96,\"type\":0},{\"gridX\":3333,\"gridY\":96,\"type\":1},{\"gridX\":3351,\"gridY\":96,\"type\":0},{\"gridX\":3403,\"gridY\":96,\"type\":2},{\"gridX\":3380,\"gridY\":96,\"type\":0},{\"gridX\":3387,\"gridY\":96,\"type\":1},{\"gridX\":3425,\"gridY\":96,\"type\":1},{\"gridX\":3445,\"gridY\":96,\"type\":0},{\"gridX\":3466,\"gridY\":96,\"type\":2},{\"gridX\":3531,\"gridY\":96,\"type\":2},{\"gridX\":3660,\"gridY\":96,\"type\":2},{\"gridX\":3742,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":2720,\"gridY\":50,\"type\":10},{\"gridX\":264,\"gridY\":50,\"type\":11},{\"gridX\":808,\"gridY\":50,\"type\":11},{\"gridX\":1864,\"gridY\":50,\"type\":11},{\"gridX\":1608,\"gridY\":50,\"type\":11},{\"gridX\":1832,\"gridY\":50,\"type\":11},{\"gridX\":2856,\"gridY\":50,\"type\":11},{\"gridX\":1768,\"gridY\":50,\"type\":12},{\"gridX\":3528,\"gridY\":50,\"type\":12},{\"gridX\":3592,\"gridY\":50,\"type\":12},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":136,\"gridY\":50,\"type\":13},{\"gridX\":552,\"gridY\":50,\"type\":13},{\"gridX\":680,\"gridY\":50,\"type\":13},{\"gridX\":1096,\"gridY\":50,\"type\":13},{\"gridX\":1352,\"gridY\":50,\"type\":13},{\"gridX\":2280,\"gridY\":50,\"type\":13},{\"gridX\":2408,\"gridY\":50,\"type\":13},{\"gridX\":3144,\"gridY\":50,\"type\":13},{\"gridX\":3272,\"gridY\":50,\"type\":13},{\"gridX\":3400,\"gridY\":50,\"type\":13},{\"gridX\":1480,\"gridY\":50,\"type\":13},{\"gridX\":1224,\"gridY\":50,\"type\":13},{\"gridX\":2152,\"gridY\":50,\"type\":13},{\"gridX\":1640,\"gridY\":50,\"type\":13},{\"gridX\":2536,\"gridY\":50,\"type\":13},{\"gridX\":2728,\"gridY\":50,\"type\":13},{\"gridX\":2664,\"gridY\":50,\"type\":14},{\"gridX\":2804,\"gridY\":48,\"type\":15},{\"gridX\":2812,\"gridY\":48,\"type\":16},{\"gridX\":2844,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":1832,\"gridY\":90,\"type\":21},{\"gridX\":1608,\"gridY\":90,\"type\":21},{\"gridX\":3592,\"gridY\":90,\"type\":21},{\"gridX\":2664,\"gridY\":90,\"type\":22},{\"gridX\":3528,\"gridY\":90,\"type\":22},{\"gridX\":1768,\"gridY\":90,\"type\":22},{\"gridX\":3144,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":2152,\"gridY\":90,\"type\":23},{\"gridX\":2408,\"gridY\":90,\"type\":23},{\"gridX\":1352,\"gridY\":90,\"type\":23},{\"gridX\":1480,\"gridY\":90,\"type\":23},{\"gridX\":1224,\"gridY\":90,\"type\":23},{\"gridX\":3400,\"gridY\":90,\"type\":23},{\"gridX\":2280,\"gridY\":90,\"type\":23},{\"gridX\":552,\"gridY\":90,\"type\":23},{\"gridX\":3272,\"gridY\":90,\"type\":23},{\"gridX\":680,\"gridY\":90,\"type\":23},{\"gridX\":1096,\"gridY\":90,\"type\":23},{\"gridX\":2728,\"gridY\":90,\"type\":23},{\"gridX\":2536,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":1640,\"gridY\":90,\"type\":23},{\"gridX\":832,\"gridY\":0,\"type\":25},{\"gridX\":2880,\"gridY\":0,\"type\":25},{\"gridX\":1888,\"gridY\":0,\"type\":25},{\"gridX\":3648,\"gridY\":0,\"type\":25},{\"gridX\":288,\"gridY\":0,\"type\":25},{\"gridX\":296,\"gridY\":0,\"type\":28},{\"gridX\":3656,\"gridY\":0,\"type\":28},{\"gridX\":1896,\"gridY\":0,\"type\":28},{\"gridX\":968,\"gridY\":0,\"type\":28},{\"gridX\":424,\"gridY\":0,\"type\":28},{\"gridX\":840,\"gridY\":0,\"type\":28},{\"gridX\":2888,\"gridY\":0,\"type\":28},{\"gridX\":3016,\"gridY\":0,\"type\":28},{\"gridX\":2024,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[{\"gridX\":264,\"gridY\":88,\"type\":1},{\"gridX\":808,\"gridY\":88,\"type\":1},{\"gridX\":1864,\"gridY\":88,\"type\":1},{\"gridX\":2856,\"gridY\":88,\"type\":1},{\"gridX\":3624,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":1448,\"gridY\":73,\"type\":2},{\"gridX\":1432,\"gridY\":73,\"type\":1},{\"gridX\":1416,\"gridY\":73,\"type\":1},{\"gridX\":1400,\"gridY\":73,\"type\":1},{\"gridX\":1384,\"gridY\":73,\"type\":1},{\"gridX\":1368,\"gridY\":73,\"type\":1},{\"gridX\":1352,\"gridY\":73,\"type\":1},{\"gridX\":1336,\"gridY\":73,\"type\":1},{\"gridX\":1320,\"gridY\":73,\"type\":1},{\"gridX\":1304,\"gridY\":73,\"type\":1},{\"gridX\":1288,\"gridY\":73,\"type\":1},{\"gridX\":1272,\"gridY\":73,\"type\":1},{\"gridX\":1256,\"gridY\":73,\"type\":1},{\"gridX\":1240,\"gridY\":73,\"type\":0},{\"gridX\":116,\"gridY\":80,\"type\":3},{\"gridX\":596,\"gridY\":80,\"type\":3},{\"gridX\":2504,\"gridY\":73,\"type\":2},{\"gridX\":2488,\"gridY\":73,\"type\":1},{\"gridX\":2472,\"gridY\":73,\"type\":1},{\"gridX\":2456,\"gridY\":73,\"type\":1},{\"gridX\":2440,\"gridY\":73,\"type\":1},{\"gridX\":2424,\"gridY\":73,\"type\":1},{\"gridX\":2408,\"gridY\":73,\"type\":1},{\"gridX\":2392,\"gridY\":73,\"type\":1},{\"gridX\":2376,\"gridY\":73,\"type\":1},{\"gridX\":2360,\"gridY\":73,\"type\":1},{\"gridX\":2344,\"gridY\":73,\"type\":1},{\"gridX\":2328,\"gridY\":73,\"type\":1},{\"gridX\":2312,\"gridY\":73,\"type\":1},{\"gridX\":2296,\"gridY\":73,\"type\":0},{\"gridX\":3496,\"gridY\":73,\"type\":2},{\"gridX\":3480,\"gridY\":73,\"type\":1},{\"gridX\":3464,\"gridY\":73,\"type\":1},{\"gridX\":3448,\"gridY\":73,\"type\":1},{\"gridX\":3432,\"gridY\":73,\"type\":1},{\"gridX\":3416,\"gridY\":73,\"type\":1},{\"gridX\":3400,\"gridY\":73,\"type\":1},{\"gridX\":3384,\"gridY\":73,\"type\":1},{\"gridX\":3368,\"gridY\":73,\"type\":1},{\"gridX\":3352,\"gridY\":73,\"type\":1},{\"gridX\":3336,\"gridY\":73,\"type\":1},{\"gridX\":3320,\"gridY\":73,\"type\":1},{\"gridX\":3304,\"gridY\":73,\"type\":1},{\"gridX\":3288,\"gridY\":73,\"type\":0}],\"foregroundObjects\":[{\"gridX\":2616,\"gridY\":55,\"type\":26},{\"gridX\":3652,\"gridY\":96,\"type\":12},{\"gridX\":1684,\"gridY\":68,\"type\":3},{\"gridX\":1702,\"gridY\":68,\"type\":4},{\"gridX\":1744,\"gridY\":68,\"type\":5},{\"gridX\":1688,\"gridY\":68,\"type\":4},{\"gridX\":1716,\"gridY\":68,\"type\":4},{\"gridX\":1730,\"gridY\":68,\"type\":4},{\"gridX\":1613,\"gridY\":84,\"type\":30},{\"gridX\":278,\"gridY\":55,\"type\":41},{\"gridX\":822,\"gridY\":55,\"type\":41},{\"gridX\":1878,\"gridY\":55,\"type\":41},{\"gridX\":2870,\"gridY\":55,\"type\":41},{\"gridX\":3638,\"gridY\":55,\"type\":41},{\"gridX\":2799,\"gridY\":69,\"type\":31},{\"gridX\":2799,\"gridY\":64,\"type\":31},{\"gridX\":2557,\"gridY\":55,\"type\":26}],\"midBossForegroundObjects\":[{\"gridX\":3516,\"gridY\":96,\"type\":37},{\"gridX\":2524,\"gridY\":96,\"type\":37},{\"gridX\":1468,\"gridY\":96,\"type\":37},{\"gridX\":612,\"gridY\":96,\"type\":38},{\"gridX\":136,\"gridY\":96,\"type\":37}],\"countHissWithMinas\":[{\"gridX\":3620,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":636,\"gridY\":64,\"type\":0},{\"gridX\":648,\"gridY\":64,\"type\":0},{\"gridX\":660,\"gridY\":64,\"type\":0},{\"gridX\":672,\"gridY\":80,\"type\":1},{\"gridX\":1616,\"gridY\":64,\"type\":0},{\"gridX\":1632,\"gridY\":64,\"type\":0},{\"gridX\":1624,\"gridY\":72,\"type\":1},{\"gridX\":2692,\"gridY\":48,\"type\":1},{\"gridX\":1688,\"gridY\":76,\"type\":0},{\"gridX\":1712,\"gridY\":76,\"type\":0},{\"gridX\":1736,\"gridY\":76,\"type\":0},{\"gridX\":2668,\"gridY\":64,\"type\":0},{\"gridX\":2720,\"gridY\":64,\"type\":0},{\"gridX\":2676,\"gridY\":56,\"type\":0},{\"gridX\":2712,\"gridY\":56,\"type\":0},{\"gridX\":3640,\"gridY\":72,\"type\":0},{\"gridX\":54,\"gridY\":101,\"type\":0},{\"gridX\":84,\"gridY\":101,\"type\":0},{\"gridX\":122,\"gridY\":76,\"type\":0},{\"gridX\":122,\"gridY\":67,\"type\":0},{\"gridX\":167,\"gridY\":62,\"type\":0},{\"gridX\":207,\"gridY\":81,\"type\":0},{\"gridX\":249,\"gridY\":58,\"type\":0},{\"gridX\":322,\"gridY\":106,\"type\":0},{\"gridX\":397,\"gridY\":105,\"type\":0},{\"gridX\":478,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":20,\"gridY\":112}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":680,\"gridY\":0,\"type\":0},{\"gridX\":1608,\"gridY\":0,\"type\":1},{\"gridX\":1768,\"gridY\":0,\"type\":0},{\"gridX\":2664,\"gridY\":0,\"type\":1},{\"gridX\":2728,\"gridY\":0,\"type\":0},{\"gridX\":3592,\"gridY\":0,\"type\":1},{\"gridX\":3656,\"gridY\":0,\"type\":0},{\"gridX\":3784,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

void Chapter1Level10::enter(GameScreen* gs)
{
    Level::enter(gs);
    
    m_iLastKnownOwlDamage = 0;
    m_iLastKnownJonNumBoosts = 1;
    m_hasTriggeredBurrow = false;
    m_iNumAttempts++;
    
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
    
    Jon& jon = m_game->getJon();
    
    jon.enableAbility(FLAG_ABILITY_RABBIT_DOWN);
    
    if (m_hasTriggeredMidBossMusicLoopIntro)
    {
        if (m_game->getJons().size() > 0)
        {
            jon.getAcceleration().set(0, 0);
            jon.getVelocity().set(0, 0);
            jon.setIdle(true);
            jon.setUserActionPrevented(true);
            
            Rectangle jonBounds = Rectangle(jon.getMainBounds().getLeft(), jon.getMainBounds().getBottom(), jon.getWidth() * 1.5f, jon.getHeight());
            
            jon.getPosition().set(m_perchTree->getMainBounds().getLeft() - jonBounds.getWidth() / 2, m_fJonY);
            jon.updateBounds();
            
            jon.setNumBoosts(1);
            
            m_midBossOwl->awaken();
            
            m_game->setStateTime(m_fGameStateTime);
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
    m_iNumAttempts = 0;
    m_iLastKnownOwlDamage = 0;
    m_iLastKnownJonNumBoosts = 0;
    m_hasTriggeredMidBossMusicLoopIntro = false;
    m_hasTriggeredMidBossMusicLoop = false;
    m_isChaseCamActivated = false;
    m_hasTriggeredBurrow = false;
    m_hasShownDrillPopup = false;
    
    Level::exit(gs);
}

void Chapter1Level10::update(GameScreen* gs)
{
    Level::update(gs);
    
    if (m_game->getJons().size() == 0)
    {
        return;
    }
    
    m_midBossOwl->update(gs->m_fDeltaTime);
    
    Jon& jon = m_game->getJon();
    
    if (m_midBossOwl->getState() == MidBossOwlState_Sleeping)
    {
        if (!m_hasShownDrillPopup
            && !m_batPanel->isOpening()
            && !m_batPanel->isOpen()
            && jon.getPosition().getX() > GRID_CELL_SIZE * 80)
        {
            m_batPanel->open(BatPanelType_Burrow);
            m_hasShownDrillPopup = true;
        }
        
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
            
            if (jon.getPosition().getX() < m_perchTree->getPosition().getX()
                && m_perchTree->getPosition().dist(jon.getPosition()) < 12)
            {
                jon.setUserActionPrevented(true);
            }
                
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
            jon.setUserActionPrevented(false);
            jon.triggerDownAction();
            jon.setIdle(true);
            jon.setUserActionPrevented(true);
            
            m_hasTriggeredBurrow = true;
        }
        
        if (m_midBossOwl->getStateTime() > 0.50f && jon.isIdle())
        {
            if (m_iNumAttempts > 1)
            {
                m_batPanel->open(BatPanelType_OwlDig);
            }
            
            jon.setIdle(false);
            jon.setUserActionPrevented(false);
        }
        
        if (!m_hasTriggeredMidBossMusicLoop && (m_game->getStateTime() - m_fGameStateTime) > 4.80f)
        {
            Assets::getInstance()->setMusicId(MUSIC_PLAY_MID_BOSS_LOOP);
            
            m_fMusicVolume = 0.5f;
            
            m_hasTriggeredMidBossMusicLoop = true;
        }
        
        if (jon.getNumBoosts() > m_iLastKnownJonNumBoosts)
        {
            m_iLastKnownJonNumBoosts = jon.getNumBoosts();
            m_midBossOwl->beginPursuit();
        }
        
        m_isChaseCamActivated = false;
    }
    else if (m_midBossOwl->getState() == MidBossOwlState_Pursuing)
    {
        m_isChaseCamActivated = true;
    }
    else if (m_midBossOwl->getState() == MidBossOwlState_SwoopingDown)
    {
        m_isChaseCamActivated = true;
        
        if (jon.getPosition().getY() < 12)
        {
            jon.getAcceleration().setX(0);
            jon.getVelocity().setX(0);
            jon.setIdle(true);
        }
    }
    else if (m_midBossOwl->getState() == MidBossOwlState_SlammingIntoTree
             || m_midBossOwl->getState() == MidBossOwlState_Dying)
    {
        m_iLastKnownJonNumBoosts = jon.getNumBoosts();
        
        if (m_midBossOwl->getDamage() > m_iLastKnownOwlDamage)
        {
            m_iLastKnownOwlDamage = m_midBossOwl->getDamage();
            
            m_exitLoop = true;
        }
        
        m_isChaseCamActivated = false;
        
        if (m_midBossOwl->getState() == MidBossOwlState_Dying
            && m_fMusicVolume > 0)
        {
            m_fMusicVolume -= gs->m_fDeltaTime;
            if (m_fMusicVolume < 0)
            {
                m_fMusicVolume = 0;
            }
            
            short musicId = MUSIC_SET_VOLUME * 1000 + (short) (m_fMusicVolume * 100);
            Assets::getInstance()->setMusicId(musicId);
        }
        
        if (jon.getPosition().getY() < 12)
        {
            jon.getAcceleration().setX(0);
            jon.getVelocity().setX(0);
            jon.setIdle(true);
        }
    }
    else if (m_midBossOwl->getState() == MidBossOwlState_FlyingOverTree)
    {
        if (jon.getNumBoosts() > m_iLastKnownJonNumBoosts)
        {
            m_iLastKnownJonNumBoosts = jon.getNumBoosts();
            m_midBossOwl->beginPursuit();
        }
        
        if (jon.isIdle())
        {
            jon.setIdle(false);
            jon.setUserActionPrevented(false);
        }
        
        m_isChaseCamActivated = false;
    }
    else if (m_midBossOwl->getState() == MidBossOwlState_Dead)
    {
        if (jon.isIdle())
        {
            jon.setIdle(false);
            jon.setUserActionPrevented(false);
        }
  
        m_isChaseCamActivated = false;
    }
}

void Chapter1Level10::updateCamera(GameScreen* gs, float paddingX, bool ignoreY, bool instant)
{
    ignoreY = m_midBossOwl->getState() == MidBossOwlState_SlammingIntoTree || m_midBossOwl->getState() == MidBossOwlState_Dying;
    
    if (m_isChaseCamActivated)
    {
        gs->m_renderer->updateCameraToFollowJon(*m_game, gs->m_fDeltaTime, paddingX, true, ignoreY, instant);
    }
    else
    {
        if (ignoreY)
        {
            paddingX = -6;
        }
        
        Level::updateCamera(gs, paddingX, ignoreY, instant);
    }
}

void Chapter1Level10::additionalRenderingBeforeHud(GameScreen* gs)
{
    gs->m_renderer->renderMidBossOwl(*m_midBossOwl);
}

bool Chapter1Level10::isInSlowMotionMode()
{
    return (m_midBossOwl->getState() == MidBossOwlState_SlammingIntoTree && m_midBossOwl->getStateTime() < 0.25f)
    || (m_midBossOwl->getState() == MidBossOwlState_Dying && m_midBossOwl->getStateTime() < 0.5f);
}

Chapter1Level10::Chapter1Level10(const char* json) : Level(json),
m_perchTree(nullptr),
m_fJonY(0),
m_fGameStateTime(0.0f),
m_fIdleWaitTime(0.0f),
m_fMusicVolume(0.5f),
m_iNumAttempts(0),
m_iLastKnownOwlDamage(0),
m_iLastKnownJonNumBoosts(0),
m_isIdleWaitingForOwl(false),
m_hasTriggeredMidBossMusicLoopIntro(false),
m_hasTriggeredMidBossMusicLoop(false),
m_isChaseCamActivated(false),
m_hasTriggeredBurrow(false),
m_hasShownDrillPopup(false)
{
    m_midBossOwl = std::unique_ptr<MidBossOwl>(new MidBossOwl(0, 0));
}