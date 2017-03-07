//
//  Chapter1Level10.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/7/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreenLevels.h"
#include "State.h"
#include "MainScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "MidBossOwl.h"
#include "Assets.h"
#include "BatPanel.h"
#include "MathUtil.h"
#include "SoundManager.h"
#include "MainRenderer.h"
#include "FlagUtil.h"

void Chapter1Level10::enter(MainScreen* ms)
{
    Level::enter(ms);
    
    m_iLastKnownOwlDamage = 0;
    m_iLastKnownJonNumBoosts = 1;
    m_hasTriggeredBurrow = false;
    m_hasShownHintPopup = false;
    
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
    
    m_midBossOwl->setGame(m_game);
    
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
            
            NGRect jonBounds = NGRect(jon.getMainBounds().getLeft(), jon.getMainBounds().getBottom(), jon.getWidth() * 1.5f, jon.getHeight());
            
            jon.getPosition().set(m_perchTree->getMainBounds().getLeft() - jonBounds.getWidth() / 2, m_fJonY);
            jon.updateBounds();
            
            jon.setNumBoosts(1);
            
            m_midBossOwl->awaken();
            
            m_game->setStateTime(m_fGameStateTime);
            m_game->setNumCarrotsCollected(m_iNumCarrotsCollectedAtCheckpoint);
            m_game->setNumGoldenCarrotsCollected(m_iNumGoldenCarrotsCollectedAtCheckpoint);
        }
    }
}

void Chapter1Level10::exit(MainScreen* ms)
{
    m_isIdleWaitingForOwl = false;
    m_fJonY = 0.0f;
    m_fGameStateTime = 0.0f;
    m_fIdleWaitTime = 0.0f;
    m_perchTree = nullptr;
    m_fMusicVolume = 1;
    m_iLastKnownOwlDamage = 0;
    m_iLastKnownJonNumBoosts = 0;
    m_hasTriggeredMidBossMusicLoopIntro = false;
    m_hasTriggeredMidBossMusicLoop = false;
    m_isChaseCamActivated = false;
    m_hasTriggeredBurrow = false;
    m_showHintBecauseJonHasBeenCaptured = false;
    m_hasShownHintPopup = false;
    
    Level::exit(ms);
}

void Chapter1Level10::update(MainScreen* ms)
{
    Level::update(ms);
    
    if (m_game->getJons().size() == 0)
    {
        return;
    }
    
    if (m_batPanel->isRequestingInput())
    {
        return;
    }
    
    m_midBossOwl->update(ms->m_fDeltaTime);
    
    Jon& jon = m_game->getJon();
    
    if (m_midBossOwl->getState() == MidBossOwlState_Sleeping)
    {
        if (jon.getNumBoosts() >= 1)
        {
            m_fMusicVolume -= ms->m_fDeltaTime / 8;
            m_fMusicVolume = clamp(m_fMusicVolume, 1, 0);
            
            short musicId = MUSIC_SET_VOLUME * 1000 + (short) (m_fMusicVolume * 100);
            SOUND_MANAGER->addMusicIdToPlayQueue(musicId);
        }
        
        if (m_perchTree)
        {
            NGRect jonBounds = NGRect(jon.getPosition().getX() - jon.getWidth() / 2, jon.getPosition().getY() - jon.getHeight() / 2, jon.getWidth(), jon.getHeight());
            
            if (jon.getPosition().getX() < m_perchTree->getPosition().getX()
                && m_perchTree->getPosition().dist(jon.getPosition()) < 12)
            {
                jon.setUserActionPrevented(true);
            }
            
            if (OverlapTester::doNGRectsOverlap(m_perchTree->getMainBounds(), jonBounds))
            {
                if (jon.isTransformingIntoVampire())
                {
                    jon.triggerCancelTransform();
                }
                
                if (jon.isVampire())
                {
                    jon.setUserActionPrevented(false);
                    jon.triggerTransform();
                    jon.setUserActionPrevented(true);
                }
                
                jon.getAcceleration().set(0, 0);
                jon.getVelocity().set(0, 0);
                
                if (!jon.isIdle())
                {
                    jon.setIdle(true);
                }
                
                jon.getPosition().setX(m_perchTree->getMainBounds().getLeft() - jon.getMainBounds().getWidth());
                jon.updateBounds();
                
                m_isIdleWaitingForOwl = true;
            }
        }
        
        if (m_isIdleWaitingForOwl)
        {
            m_fIdleWaitTime += ms->m_fDeltaTime;
            
            if (m_fIdleWaitTime > 1.0f)
            {
                m_iLastKnownJonNumBoosts = jon.getNumBoosts();
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
            m_iNumCarrotsCollectedAtCheckpoint = m_game->getNumCarrotsCollected();
            m_iNumGoldenCarrotsCollectedAtCheckpoint = m_game->getNumGoldenCarrotsCollected();
            
            if (SOUND_MANAGER->isMusicEnabled())
            {
                SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_MID_BOSS_LOOP_INTRO);
                SOUND_MANAGER->addMusicIdToPlayQueue(MUSIC_LOAD_MID_BOSS_LOOP);
            }
            
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
            jon.setIdle(false);
            jon.setUserActionPrevented(false);
        }
        
        if (!m_hasTriggeredMidBossMusicLoop && (m_game->getStateTime() - m_fGameStateTime) > 4.80f)
        {
            SOUND_MANAGER->addMusicIdToPlayQueue(MUSIC_PLAY_LOOP);
            
            m_fMusicVolume = 1;
            
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

		if (jon.getNumBoosts() > m_iLastKnownJonNumBoosts)
		{
			m_iLastKnownJonNumBoosts = jon.getNumBoosts();
			m_midBossOwl->beginPursuit();
		}
        
        if (!m_hasTriggeredMidBossMusicLoop && (m_game->getStateTime() - m_fGameStateTime) > 4.80f)
        {
            SOUND_MANAGER->addMusicIdToPlayQueue(MUSIC_PLAY_LOOP);
            
            m_fMusicVolume = 1;
            
            m_hasTriggeredMidBossMusicLoop = true;
        }
    }
    else if (m_midBossOwl->getState() == MidBossOwlState_SwoopingDown)
    {
		if (jon.getPosition().getX() > 204
			&& jon.getPosition().getX() < 207
			&& !FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_LEVEL_COMPLETE)
			&& m_showHintBecauseJonHasBeenCaptured
			&& !m_hasShownHintPopup
			&& !m_midBossOwl->didJonTransform()
			&& jon.getAbilityState() == ABILITY_NONE
            && jon.getPhysicalState() == PHYSICAL_GROUNDED)
		{
			m_midBossOwl->givePlayerAFreeHit();

			m_batPanel->config(m_game, BatGoalType_DrillToDamageOwl);

			m_hasShownHintPopup = true;

			m_showHintBecauseJonHasBeenCaptured = false;
		}

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
            m_fMusicVolume -= ms->m_fDeltaTime;
            m_fMusicVolume = clamp(m_fMusicVolume, 1, 0);
            
            short musicId = MUSIC_SET_VOLUME * 1000 + (short) (m_fMusicVolume * 100);
            SOUND_MANAGER->addMusicIdToPlayQueue(musicId);
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
    else if (m_midBossOwl->getState() == MidBossOwlState_FlyingAwayAfterCatchingJon)
    {
        m_showHintBecauseJonHasBeenCaptured = m_midBossOwl->getDamage() == 0;
        
        m_isChaseCamActivated = true;
    }
}

void Chapter1Level10::updateCamera(MainScreen* ms, float paddingX, bool ignoreY, bool instant)
{
    ignoreY = m_midBossOwl->getState() == MidBossOwlState_SlammingIntoTree || m_midBossOwl->getState() == MidBossOwlState_Dying;
    
    if (m_isChaseCamActivated
        && m_game->getJon().getPosition().getY() > 9)
    {
        ms->m_renderer->updateCameraToFollowJon(*m_game, m_batPanel, ms->m_fDeltaTime, paddingX, true, ignoreY, instant);
    }
    else
    {
        if (ignoreY)
        {
            paddingX = -6;
        }
        
        Level::updateCamera(ms, paddingX, ignoreY, instant);
    }
}

void Chapter1Level10::additionalRenderingBeforeHud(MainScreen* ms)
{
    ms->m_renderer->renderMidBossOwl(*m_midBossOwl);
}

bool Chapter1Level10::isInSlowMotionMode()
{
    return (m_midBossOwl->getState() == MidBossOwlState_SlammingIntoTree && m_midBossOwl->getStateTime() < 0.25f)
    || (m_midBossOwl->getState() == MidBossOwlState_Dying && m_midBossOwl->getStateTime() < 0.5f);
}

void Chapter1Level10::configBatPanel()
{
    if (!m_hasTriggeredMidBossMusicLoopIntro
        && m_iNumTimesBatPanelDisplayed < 2)
    {
        Level::configBatPanel();
    }
}

Chapter1Level10::Chapter1Level10(const char* json) : Level(json),
m_midBossOwl(new MidBossOwl(0, 0)),
m_perchTree(nullptr),
m_fJonY(0),
m_fGameStateTime(0.0f),
m_fIdleWaitTime(0.0f),
m_fMusicVolume(1),
m_iLastKnownOwlDamage(0),
m_iLastKnownJonNumBoosts(0),
m_iNumCarrotsCollectedAtCheckpoint(0),
m_iNumGoldenCarrotsCollectedAtCheckpoint(0),
m_isIdleWaitingForOwl(false),
m_hasTriggeredMidBossMusicLoopIntro(false),
m_hasTriggeredMidBossMusicLoop(false),
m_isChaseCamActivated(false),
m_hasTriggeredBurrow(false),
m_showHintBecauseJonHasBeenCaptured(false),
m_hasShownHintPopup(false)
{
    // Empty
}

Chapter1Level10::~Chapter1Level10()
{
    delete m_midBossOwl;
}

RTTI_IMPL(Chapter1Level10, Level);

#include <sstream>

/// Chapter 1 Level 10 ///

Chapter1Level10 * Chapter1Level10::s_pInstance = nullptr;

void Chapter1Level10::create()
{
    assert(!s_pInstance);
    
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":10,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":168,\"gridY\":96,\"type\":2},{\"gridX\":288,\"gridY\":96,\"type\":2},{\"gridX\":348,\"gridY\":96,\"type\":2},{\"gridX\":448,\"gridY\":96,\"type\":2},{\"gridX\":752,\"gridY\":96,\"type\":2},{\"gridX\":936,\"gridY\":96,\"type\":2},{\"gridX\":1304,\"gridY\":96,\"type\":2},{\"gridX\":1855,\"gridY\":96,\"type\":2},{\"gridX\":1843,\"gridY\":96,\"type\":2},{\"gridX\":2668,\"gridY\":96,\"type\":2},{\"gridX\":2732,\"gridY\":96,\"type\":2},{\"gridX\":2876,\"gridY\":96,\"type\":2},{\"gridX\":3004,\"gridY\":96,\"type\":2},{\"gridX\":3116,\"gridY\":96,\"type\":2},{\"gridX\":3228,\"gridY\":96,\"type\":2},{\"gridX\":3487,\"gridY\":96,\"type\":2},{\"gridX\":3648,\"gridY\":96,\"type\":2},{\"gridX\":3728,\"gridY\":96,\"type\":2},{\"gridX\":3832,\"gridY\":96,\"type\":2},{\"gridX\":2184,\"gridY\":96,\"type\":0},{\"gridX\":2604,\"gridY\":96,\"type\":2},{\"gridX\":3383,\"gridY\":96,\"type\":2},{\"gridX\":44,\"gridY\":96,\"type\":13},{\"gridX\":89,\"gridY\":96,\"type\":14},{\"gridX\":61,\"gridY\":96,\"type\":15},{\"gridX\":19,\"gridY\":96,\"type\":16},{\"gridX\":399,\"gridY\":96,\"type\":13},{\"gridX\":258,\"gridY\":96,\"type\":14},{\"gridX\":559,\"gridY\":96,\"type\":13},{\"gridX\":515,\"gridY\":96,\"type\":14},{\"gridX\":577,\"gridY\":96,\"type\":15},{\"gridX\":482,\"gridY\":96,\"type\":16},{\"gridX\":382,\"gridY\":96,\"type\":15},{\"gridX\":326,\"gridY\":96,\"type\":11},{\"gridX\":849,\"gridY\":96,\"type\":13},{\"gridX\":979,\"gridY\":96,\"type\":14},{\"gridX\":1004,\"gridY\":96,\"type\":11},{\"gridX\":1055,\"gridY\":96,\"type\":13},{\"gridX\":1086,\"gridY\":96,\"type\":14},{\"gridX\":1004,\"gridY\":96,\"type\":15},{\"gridX\":1130,\"gridY\":96,\"type\":16},{\"gridX\":1234,\"gridY\":96,\"type\":15},{\"gridX\":1279,\"gridY\":96,\"type\":16},{\"gridX\":1201,\"gridY\":96,\"type\":14},{\"gridX\":1178,\"gridY\":96,\"type\":13},{\"gridX\":405,\"gridY\":96,\"type\":6},{\"gridX\":1290,\"gridY\":96,\"type\":14},{\"gridX\":1446,\"gridY\":96,\"type\":13},{\"gridX\":1354,\"gridY\":96,\"type\":15},{\"gridX\":1902,\"gridY\":96,\"type\":16},{\"gridX\":1319,\"gridY\":96,\"type\":13},{\"gridX\":1364,\"gridY\":96,\"type\":11},{\"gridX\":1962,\"gridY\":96,\"type\":13},{\"gridX\":2097,\"gridY\":96,\"type\":14},{\"gridX\":2034,\"gridY\":96,\"type\":13},{\"gridX\":2226,\"gridY\":96,\"type\":13},{\"gridX\":2001,\"gridY\":96,\"type\":14},{\"gridX\":2277,\"gridY\":96,\"type\":15},{\"gridX\":2057,\"gridY\":96,\"type\":15},{\"gridX\":2146,\"gridY\":96,\"type\":16},{\"gridX\":2387,\"gridY\":96,\"type\":13},{\"gridX\":2326,\"gridY\":96,\"type\":14},{\"gridX\":2492,\"gridY\":96,\"type\":16},{\"gridX\":2339,\"gridY\":96,\"type\":16},{\"gridX\":2298,\"gridY\":96,\"type\":2},{\"gridX\":2970,\"gridY\":96,\"type\":7},{\"gridX\":2970,\"gridY\":108,\"type\":7},{\"gridX\":3035,\"gridY\":96,\"type\":7},{\"gridX\":3035,\"gridY\":108,\"type\":7},{\"gridX\":3174,\"gridY\":96,\"type\":15},{\"gridX\":2988,\"gridY\":96,\"type\":16},{\"gridX\":2981,\"gridY\":96,\"type\":14},{\"gridX\":2896,\"gridY\":96,\"type\":13},{\"gridX\":3213,\"gridY\":96,\"type\":14},{\"gridX\":3401,\"gridY\":96,\"type\":13},{\"gridX\":3344,\"gridY\":96,\"type\":14},{\"gridX\":3295,\"gridY\":96,\"type\":13},{\"gridX\":3370,\"gridY\":96,\"type\":15},{\"gridX\":3321,\"gridY\":96,\"type\":16},{\"gridX\":3409,\"gridY\":96,\"type\":11},{\"gridX\":3455,\"gridY\":96,\"type\":6},{\"gridX\":2894,\"gridY\":96,\"type\":1},{\"gridX\":2423,\"gridY\":96,\"type\":11},{\"gridX\":2406,\"gridY\":96,\"type\":14},{\"gridX\":2427,\"gridY\":96,\"type\":15},{\"gridX\":2476,\"gridY\":96,\"type\":13}],\"grounds\":[{\"gridX\":2664,\"gridY\":20,\"type\":5},{\"gridX\":2672,\"gridY\":20,\"type\":8},{\"gridX\":2800,\"gridY\":20,\"type\":8},{\"gridX\":2928,\"gridY\":20,\"type\":8},{\"gridX\":3056,\"gridY\":20,\"type\":8},{\"gridX\":3184,\"gridY\":20,\"type\":9},{\"gridX\":2720,\"gridY\":50,\"type\":10},{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":264,\"gridY\":50,\"type\":11},{\"gridX\":808,\"gridY\":50,\"type\":11},{\"gridX\":1864,\"gridY\":50,\"type\":11},{\"gridX\":1608,\"gridY\":50,\"type\":11},{\"gridX\":1832,\"gridY\":50,\"type\":11},{\"gridX\":2792,\"gridY\":50,\"type\":11},{\"gridX\":3112,\"gridY\":50,\"type\":11},{\"gridX\":1768,\"gridY\":50,\"type\":12},{\"gridX\":3528,\"gridY\":50,\"type\":12},{\"gridX\":3592,\"gridY\":50,\"type\":12},{\"gridX\":2728,\"gridY\":50,\"type\":12},{\"gridX\":136,\"gridY\":50,\"type\":13},{\"gridX\":552,\"gridY\":50,\"type\":13},{\"gridX\":3400,\"gridY\":50,\"type\":13},{\"gridX\":1352,\"gridY\":50,\"type\":13},{\"gridX\":2280,\"gridY\":50,\"type\":13},{\"gridX\":1096,\"gridY\":50,\"type\":13},{\"gridX\":680,\"gridY\":50,\"type\":13},{\"gridX\":1480,\"gridY\":50,\"type\":13},{\"gridX\":1224,\"gridY\":50,\"type\":13},{\"gridX\":840,\"gridY\":50,\"type\":13},{\"gridX\":968,\"gridY\":50,\"type\":13},{\"gridX\":1640,\"gridY\":50,\"type\":13},{\"gridX\":2152,\"gridY\":50,\"type\":13},{\"gridX\":2536,\"gridY\":50,\"type\":13},{\"gridX\":2408,\"gridY\":50,\"type\":13},{\"gridX\":3144,\"gridY\":50,\"type\":13},{\"gridX\":2856,\"gridY\":50,\"type\":13},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":3272,\"gridY\":50,\"type\":13},{\"gridX\":1896,\"gridY\":50,\"type\":13},{\"gridX\":2024,\"gridY\":50,\"type\":13},{\"gridX\":2984,\"gridY\":50,\"type\":13},{\"gridX\":2664,\"gridY\":50,\"type\":14},{\"gridX\":832,\"gridY\":48,\"type\":15},{\"gridX\":2880,\"gridY\":48,\"type\":15},{\"gridX\":1888,\"gridY\":48,\"type\":15},{\"gridX\":2784,\"gridY\":48,\"type\":15},{\"gridX\":2792,\"gridY\":48,\"type\":16},{\"gridX\":3016,\"gridY\":48,\"type\":18},{\"gridX\":3144,\"gridY\":48,\"type\":18},{\"gridX\":840,\"gridY\":48,\"type\":18},{\"gridX\":968,\"gridY\":48,\"type\":18},{\"gridX\":1096,\"gridY\":48,\"type\":18},{\"gridX\":1896,\"gridY\":48,\"type\":18},{\"gridX\":2024,\"gridY\":48,\"type\":18},{\"gridX\":2888,\"gridY\":48,\"type\":18},{\"gridX\":1224,\"gridY\":48,\"type\":19},{\"gridX\":2824,\"gridY\":48,\"type\":19},{\"gridX\":2152,\"gridY\":48,\"type\":19},{\"gridX\":3272,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":1832,\"gridY\":90,\"type\":21},{\"gridX\":1608,\"gridY\":90,\"type\":21},{\"gridX\":3592,\"gridY\":90,\"type\":21},{\"gridX\":1768,\"gridY\":90,\"type\":22},{\"gridX\":2664,\"gridY\":90,\"type\":22},{\"gridX\":3528,\"gridY\":90,\"type\":22},{\"gridX\":1352,\"gridY\":90,\"type\":23},{\"gridX\":1480,\"gridY\":90,\"type\":23},{\"gridX\":3400,\"gridY\":90,\"type\":23},{\"gridX\":2280,\"gridY\":90,\"type\":23},{\"gridX\":552,\"gridY\":90,\"type\":23},{\"gridX\":3272,\"gridY\":90,\"type\":23},{\"gridX\":1224,\"gridY\":90,\"type\":23},{\"gridX\":968,\"gridY\":90,\"type\":23},{\"gridX\":2728,\"gridY\":90,\"type\":23},{\"gridX\":2536,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":840,\"gridY\":90,\"type\":23},{\"gridX\":2888,\"gridY\":90,\"type\":23},{\"gridX\":1640,\"gridY\":90,\"type\":23},{\"gridX\":680,\"gridY\":90,\"type\":23},{\"gridX\":1096,\"gridY\":90,\"type\":23},{\"gridX\":2152,\"gridY\":90,\"type\":23},{\"gridX\":3144,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":3016,\"gridY\":90,\"type\":23},{\"gridX\":1896,\"gridY\":90,\"type\":23},{\"gridX\":2408,\"gridY\":90,\"type\":23},{\"gridX\":2024,\"gridY\":90,\"type\":23},{\"gridX\":3648,\"gridY\":0,\"type\":25},{\"gridX\":288,\"gridY\":0,\"type\":25},{\"gridX\":424,\"gridY\":0,\"type\":28},{\"gridX\":296,\"gridY\":0,\"type\":28},{\"gridX\":3784,\"gridY\":0,\"type\":28},{\"gridX\":3656,\"gridY\":0,\"type\":28},{\"gridX\":3912,\"gridY\":0,\"type\":29},{\"gridX\":552,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":264,\"gridY\":88,\"type\":1},{\"gridX\":3624,\"gridY\":88,\"type\":1},{\"gridX\":2824,\"gridY\":48,\"type\":2},{\"gridX\":808,\"gridY\":88,\"type\":0},{\"gridX\":2856,\"gridY\":88,\"type\":0},{\"gridX\":1864,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":1448,\"gridY\":73,\"t" << "ype\":2},{\"gridX\":1432,\"gridY\":73,\"type\":1},{\"gridX\":1416,\"gridY\":73,\"type\":1},{\"gridX\":1400,\"gridY\":73,\"type\":1},{\"gridX\":1384,\"gridY\":73,\"type\":1},{\"gridX\":1368,\"gridY\":73,\"type\":1},{\"gridX\":1352,\"gridY\":73,\"type\":1},{\"gridX\":1336,\"gridY\":73,\"type\":1},{\"gridX\":1320,\"gridY\":73,\"type\":1},{\"gridX\":1304,\"gridY\":73,\"type\":1},{\"gridX\":1288,\"gridY\":73,\"type\":1},{\"gridX\":1272,\"gridY\":73,\"type\":1},{\"gridX\":1256,\"gridY\":73,\"type\":1},{\"gridX\":840,\"gridY\":73,\"type\":0},{\"gridX\":116,\"gridY\":80,\"type\":3},{\"gridX\":596,\"gridY\":80,\"type\":3},{\"gridX\":2504,\"gridY\":73,\"type\":2},{\"gridX\":2488,\"gridY\":73,\"type\":1},{\"gridX\":2472,\"gridY\":73,\"type\":1},{\"gridX\":2456,\"gridY\":73,\"type\":1},{\"gridX\":2440,\"gridY\":73,\"type\":1},{\"gridX\":2424,\"gridY\":73,\"type\":1},{\"gridX\":2408,\"gridY\":73,\"type\":1},{\"gridX\":2392,\"gridY\":73,\"type\":1},{\"gridX\":2376,\"gridY\":73,\"type\":1},{\"gridX\":2360,\"gridY\":73,\"type\":1},{\"gridX\":2344,\"gridY\":73,\"type\":1},{\"gridX\":2328,\"gridY\":73,\"type\":1},{\"gridX\":2312,\"gridY\":73,\"type\":1},{\"gridX\":1896,\"gridY\":73,\"type\":0},{\"gridX\":3496,\"gridY\":73,\"type\":2},{\"gridX\":3480,\"gridY\":73,\"type\":1},{\"gridX\":3464,\"gridY\":73,\"type\":1},{\"gridX\":3448,\"gridY\":73,\"type\":1},{\"gridX\":3432,\"gridY\":73,\"type\":1},{\"gridX\":3416,\"gridY\":73,\"type\":1},{\"gridX\":3400,\"gridY\":73,\"type\":1},{\"gridX\":3384,\"gridY\":73,\"type\":1},{\"gridX\":3368,\"gridY\":73,\"type\":1},{\"gridX\":3352,\"gridY\":73,\"type\":1},{\"gridX\":3336,\"gridY\":73,\"type\":1},{\"gridX\":3320,\"gridY\":73,\"type\":1},{\"gridX\":3304,\"gridY\":73,\"type\":1},{\"gridX\":1240,\"gridY\":73,\"type\":1},{\"gridX\":1224,\"gridY\":73,\"type\":1},{\"gridX\":1208,\"gridY\":73,\"type\":1},{\"gridX\":1192,\"gridY\":73,\"type\":1},{\"gridX\":1176,\"gridY\":73,\"type\":1},{\"gridX\":856,\"gridY\":73,\"type\":1},{\"gridX\":872,\"gridY\":73,\"type\":1},{\"gridX\":888,\"gridY\":73,\"type\":1},{\"gridX\":904,\"gridY\":73,\"type\":1},{\"gridX\":920,\"gridY\":73,\"type\":1},{\"gridX\":936,\"gridY\":73,\"type\":1},{\"gridX\":952,\"gridY\":73,\"type\":1},{\"gridX\":968,\"gridY\":73,\"type\":1},{\"gridX\":984,\"gridY\":73,\"type\":1},{\"gridX\":1000,\"gridY\":73,\"type\":1},{\"gridX\":1016,\"gridY\":73,\"type\":1},{\"gridX\":1032,\"gridY\":73,\"type\":1},{\"gridX\":1048,\"gridY\":73,\"type\":1},{\"gridX\":1064,\"gridY\":73,\"type\":1},{\"gridX\":1080,\"gridY\":73,\"type\":1},{\"gridX\":1096,\"gridY\":73,\"type\":1},{\"gridX\":1112,\"gridY\":73,\"type\":1},{\"gridX\":1128,\"gridY\":73,\"type\":1},{\"gridX\":1144,\"gridY\":73,\"type\":1},{\"gridX\":1160,\"gridY\":73,\"type\":1},{\"gridX\":1912,\"gridY\":73,\"type\":1},{\"gridX\":1928,\"gridY\":73,\"type\":1},{\"gridX\":1944,\"gridY\":73,\"type\":1},{\"gridX\":1960,\"gridY\":73,\"type\":1},{\"gridX\":1976,\"gridY\":73,\"type\":1},{\"gridX\":1992,\"gridY\":73,\"type\":1},{\"gridX\":2008,\"gridY\":73,\"type\":1},{\"gridX\":2024,\"gridY\":73,\"type\":1},{\"gridX\":2040,\"gridY\":73,\"type\":1},{\"gridX\":2056,\"gridY\":73,\"type\":1},{\"gridX\":2072,\"gridY\":73,\"type\":1},{\"gridX\":2088,\"gridY\":73,\"type\":1},{\"gridX\":2104,\"gridY\":73,\"type\":1},{\"gridX\":2120,\"gridY\":73,\"type\":1},{\"gridX\":2136,\"gridY\":73,\"type\":1},{\"gridX\":2152,\"gridY\":73,\"type\":1},{\"gridX\":2168,\"gridY\":73,\"type\":1},{\"gridX\":2184,\"gridY\":73,\"type\":1},{\"gridX\":2200,\"gridY\":73,\"type\":1},{\"gridX\":2216,\"gridY\":73,\"type\":1},{\"gridX\":2232,\"gridY\":73,\"type\":1},{\"gridX\":2248,\"gridY\":73,\"type\":1},{\"gridX\":2264,\"gridY\":73,\"type\":1},{\"gridX\":2280,\"gridY\":73,\"type\":1},{\"gridX\":2296,\"gridY\":73,\"type\":1},{\"gridX\":2888,\"gridY\":73,\"type\":0},{\"gridX\":2904,\"gridY\":73,\"type\":1},{\"gridX\":2920,\"gridY\":73,\"type\":1},{\"gridX\":2936,\"gridY\":73,\"type\":1},{\"gridX\":2952,\"gridY\":73,\"type\":1},{\"gridX\":2968,\"gridY\":73,\"type\":1},{\"gridX\":2984,\"gridY\":73,\"type\":1},{\"gridX\":3000,\"gridY\":73,\"type\":1},{\"gridX\":3016,\"gridY\":73,\"type\":1},{\"gridX\":3032,\"gridY\":73,\"type\":1},{\"gridX\":3048,\"gridY\":73,\"type\":1},{\"gridX\":3064,\"gridY\":73,\"type\":1},{\"gridX\":3080,\"gridY\":73,\"type\":1},{\"gridX\":3096,\"gridY\":73,\"type\":1},{\"gridX\":3112,\"gridY\":73,\"type\":1},{\"gridX\":3128,\"gridY\":73,\"type\":1},{\"gridX\":3144,\"gridY\":73,\"type\":1},{\"gridX\":3160,\"gridY\":73,\"type\":1},{\"gridX\":3176,\"gridY\":73,\"type\":1},{\"gridX\":3192,\"gridY\":73,\"type\":1},{\"gridX\":3208,\"gridY\":73,\"type\":1},{\"gridX\":3224,\"gridY\":73,\"type\":1},{\"gridX\":3240,\"gridY\":73,\"type\":1},{\"gridX\":3256,\"gridY\":73,\"type\":1},{\"gridX\":3272,\"gridY\":73,\"type\":1},{\"gridX\":3288,\"gridY\":73,\"type\":1}],\"foregroundObjects\":[{\"gridX\":2588,\"gridY\":55,\"type\":26},{\"gridX\":3688,\"gridY\":96,\"type\":12},{\"gridX\":1684,\"gridY\":68,\"type\":3},{\"gridX\":1702,\"gridY\":68,\"type\":4},{\"gridX\":1744,\"gridY\":68,\"type\":5},{\"gridX\":1688,\"gridY\":68,\"type\":4},{\"gridX\":1716,\"gridY\":68,\"type\":4},{\"gridX\":1730,\"gridY\":68,\"type\":4},{\"gridX\":1613,\"gridY\":84,\"type\":30},{\"gridX\":278,\"gridY\":55,\"type\":41},{\"gridX\":822,\"gridY\":55,\"type\":41},{\"gridX\":1878,\"gridY\":55,\"type\":41},{\"gridX\":2870,\"gridY\":55,\"type\":41},{\"gridX\":3638,\"gridY\":55,\"type\":41},{\"gridX\":2780,\"gridY\":68,\"type\":31},{\"gridX\":2780,\"gridY\":64,\"type\":31},{\"gridX\":2628,\"gridY\":84,\"type\":29},{\"gridX\":2836,\"gridY\":27,\"type\":41},{\"gridX\":836,\"gridY\":75,\"type\":27},{\"gridX\":868,\"gridY\":75,\"type\":27},{\"gridX\":900,\"gridY\":75,\"type\":27},{\"gridX\":932,\"gridY\":75,\"type\":27},{\"gridX\":832,\"gridY\":96,\"type\":22},{\"gridX\":836,\"gridY\":96,\"type\":22},{\"gridX\":964,\"gridY\":75,\"type\":27},{\"gridX\":996,\"gridY\":75,\"type\":27},{\"gridX\":1028,\"gridY\":75,\"type\":27},{\"gridX\":1060,\"gridY\":75,\"type\":27},{\"gridX\":1092,\"gridY\":75,\"type\":27},{\"gridX\":1124,\"gridY\":75,\"type\":27},{\"gridX\":1156,\"gridY\":75,\"type\":27},{\"gridX\":1219,\"gridY\":75,\"type\":25},{\"gridX\":1225,\"gridY\":75,\"type\":25},{\"gridX\":2852,\"gridY\":27,\"type\":27},{\"gridX\":2856,\"gridY\":44,\"type\":30},{\"gridX\":2888,\"gridY\":44,\"type\":30},{\"gridX\":2884,\"gridY\":27,\"type\":27},{\"gridX\":2920,\"gridY\":44,\"type\":30},{\"gridX\":2916,\"gridY\":27,\"type\":27},{\"gridX\":1892,\"gridY\":75,\"type\":27},{\"gridX\":1924,\"gridY\":75,\"type\":27},{\"gridX\":1956,\"gridY\":75,\"type\":27},{\"gridX\":1988,\"gridY\":75,\"type\":27},{\"gridX\":2020,\"gridY\":75,\"type\":27},{\"gridX\":2052,\"gridY\":75,\"type\":27},{\"gridX\":2084,\"gridY\":75,\"type\":27},{\"gridX\":2116,\"gridY\":75,\"type\":27},{\"gridX\":2147,\"gridY\":75,\"type\":25},{\"gridX\":2153,\"gridY\":75,\"type\":25},{\"gridX\":1888,\"gridY\":96,\"type\":22},{\"gridX\":1892,\"gridY\":96,\"type\":22},{\"gridX\":1188,\"gridY\":75,\"type\":27},{\"gridX\":2884,\"gridY\":75,\"type\":27},{\"gridX\":2880,\"gridY\":96,\"type\":22},{\"gridX\":2884,\"gridY\":96,\"type\":22},{\"gridX\":2916,\"gridY\":75,\"type\":27},{\"gridX\":2948,\"gridY\":75,\"type\":27},{\"gridX\":2980,\"gridY\":75,\"type\":27},{\"gridX\":3012,\"gridY\":75,\"type\":27},{\"gridX\":3044,\"gridY\":75,\"type\":27},{\"gridX\":3076,\"gridY\":75,\"type\":27},{\"gridX\":3108,\"gridY\":75,\"type\":27},{\"gridX\":3140,\"gridY\":75,\"type\":27},{\"gridX\":3172,\"gridY\":75,\"type\":27},{\"gridX\":3204,\"gridY\":75,\"type\":27},{\"gridX\":3236,\"gridY\":75,\"type\":27},{\"gridX\":3267,\"gridY\":75,\"type\":25},{\"gridX\":3273,\"gridY\":75,\"type\":25},{\"gridX\":2966,\"gridY\":116,\"type\":46},{\"gridX\":3031,\"gridY\":116,\"type\":46}],\"midBossForegroundObjects\":[{\"gridX\":3516,\"gridY\":96,\"type\":37},{\"gridX\":2524,\"gridY\":96,\"type\":37},{\"gridX\":1468,\"gridY\":96,\"type\":37},{\"gridX\":612,\"gridY\":96,\"type\":38},{\"gridX\":136,\"" << "gridY\":96,\"type\":37}],\"endBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":3656,\"gridY\":96}],\"endBossSnakes\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":640,\"gridY\":64,\"type\":0},{\"gridX\":648,\"gridY\":64,\"type\":0},{\"gridX\":656,\"gridY\":64,\"type\":0},{\"gridX\":1044,\"gridY\":152,\"type\":1},{\"gridX\":1616,\"gridY\":64,\"type\":0},{\"gridX\":1632,\"gridY\":64,\"type\":0},{\"gridX\":1624,\"gridY\":72,\"type\":1},{\"gridX\":2692,\"gridY\":48,\"type\":1},{\"gridX\":1688,\"gridY\":80,\"type\":0},{\"gridX\":1696,\"gridY\":80,\"type\":0},{\"gridX\":1704,\"gridY\":80,\"type\":0},{\"gridX\":2640,\"gridY\":64,\"type\":0},{\"gridX\":2744,\"gridY\":36,\"type\":0},{\"gridX\":2624,\"gridY\":64,\"type\":0},{\"gridX\":2632,\"gridY\":64,\"type\":0},{\"gridX\":3620,\"gridY\":80,\"type\":0},{\"gridX\":356,\"gridY\":104,\"type\":0},{\"gridX\":348,\"gridY\":104,\"type\":0},{\"gridX\":176,\"gridY\":64,\"type\":0},{\"gridX\":160,\"gridY\":64,\"type\":0},{\"gridX\":168,\"gridY\":64,\"type\":0},{\"gridX\":208,\"gridY\":72,\"type\":0},{\"gridX\":216,\"gridY\":80,\"type\":0},{\"gridX\":472,\"gridY\":104,\"type\":0},{\"gridX\":464,\"gridY\":104,\"type\":0},{\"gridX\":456,\"gridY\":104,\"type\":0},{\"gridX\":340,\"gridY\":104,\"type\":0},{\"gridX\":224,\"gridY\":72,\"type\":0},{\"gridX\":252,\"gridY\":64,\"type\":0},{\"gridX\":260,\"gridY\":64,\"type\":0},{\"gridX\":268,\"gridY\":64,\"type\":0},{\"gridX\":728,\"gridY\":64,\"type\":0},{\"gridX\":736,\"gridY\":64,\"type\":0},{\"gridX\":744,\"gridY\":64,\"type\":0},{\"gridX\":908,\"gridY\":104,\"type\":0},{\"gridX\":916,\"gridY\":104,\"type\":0},{\"gridX\":900,\"gridY\":104,\"type\":0},{\"gridX\":1108,\"gridY\":104,\"type\":0},{\"gridX\":1116,\"gridY\":104,\"type\":0},{\"gridX\":1100,\"gridY\":104,\"type\":0},{\"gridX\":1524,\"gridY\":64,\"type\":0},{\"gridX\":1516,\"gridY\":64,\"type\":0},{\"gridX\":1508,\"gridY\":64,\"type\":0},{\"gridX\":1624,\"gridY\":64,\"type\":0},{\"gridX\":1712,\"gridY\":80,\"type\":0},{\"gridX\":1720,\"gridY\":80,\"type\":0},{\"gridX\":1728,\"gridY\":80,\"type\":0},{\"gridX\":1736,\"gridY\":80,\"type\":0},{\"gridX\":1804,\"gridY\":64,\"type\":0},{\"gridX\":1812,\"gridY\":64,\"type\":0},{\"gridX\":1796,\"gridY\":64,\"type\":0},{\"gridX\":1948,\"gridY\":104,\"type\":0},{\"gridX\":1956,\"gridY\":104,\"type\":0},{\"gridX\":1964,\"gridY\":104,\"type\":0},{\"gridX\":3612,\"gridY\":72,\"type\":0},{\"gridX\":3628,\"gridY\":72,\"type\":0},{\"gridX\":2088,\"gridY\":104,\"type\":0},{\"gridX\":2096,\"gridY\":104,\"type\":0},{\"gridX\":2104,\"gridY\":104,\"type\":0},{\"gridX\":2752,\"gridY\":36,\"type\":0},{\"gridX\":2736,\"gridY\":36,\"type\":0},{\"gridX\":2860,\"gridY\":72,\"type\":0},{\"gridX\":2868,\"gridY\":72,\"type\":0},{\"gridX\":2860,\"gridY\":80,\"type\":0},{\"gridX\":2912,\"gridY\":104,\"type\":0},{\"gridX\":2844,\"gridY\":64,\"type\":0},{\"gridX\":2852,\"gridY\":72,\"type\":0},{\"gridX\":2796,\"gridY\":36,\"type\":0},{\"gridX\":2804,\"gridY\":36,\"type\":0},{\"gridX\":2812,\"gridY\":36,\"type\":0},{\"gridX\":2868,\"gridY\":64,\"type\":0},{\"gridX\":2920,\"gridY\":104,\"type\":0},{\"gridX\":2928,\"gridY\":104,\"type\":0},{\"gridX\":2964,\"gridY\":124,\"type\":0},{\"gridX\":2972,\"gridY\":124,\"type\":0},{\"gridX\":2980,\"gridY\":124,\"type\":0},{\"gridX\":3044,\"gridY\":124,\"type\":0},{\"gridX\":3028,\"gridY\":124,\"type\":0},{\"gridX\":3036,\"gridY\":124,\"type\":0},{\"gridX\":2996,\"gridY\":108,\"type\":0},{\"gridX\":3004,\"gridY\":104,\"type\":0},{\"gridX\":3012,\"gridY\":108,\"type\":0},{\"gridX\":800,\"gridY\":80,\"type\":0},{\"gridX\":792,\"gridY\":72,\"type\":0},{\"gridX\":808,\"gridY\":72,\"type\":0},{\"gridX\":1848,\"gridY\":72,\"type\":0},{\"gridX\":1856,\"gridY\":80,\"type\":0},{\"gridX\":1864,\"gridY\":72,\"type\":0},{\"gridX\":3096,\"gridY\":104,\"type\":0},{\"gridX\":3088,\"gridY\":104,\"type\":0},{\"gridX\":3104,\"gridY\":104,\"type\":0},{\"gridX\":3556,\"gridY\":64,\"type\":0},{\"gridX\":3540,\"gridY\":64,\"type\":0},{\"gridX\":3548,\"gridY\":64,\"type\":0},{\"gridX\":3640,\"gridY\":64,\"type\":0},{\"gridX\":3640,\"gridY\":72,\"type\":0},{\"gridX\":3640,\"gridY\":80,\"type\":0},{\"gridX\":3640,\"gridY\":104,\"type\":0},{\"gridX\":3644,\"gridY\":112,\"type\":0},{\"gridX\":3648,\"gridY\":120,\"type\":0},{\"gridX\":3656,\"gridY\":120,\"type\":0},{\"gridX\":3660,\"gridY\":112,\"type\":0},{\"gridX\":3664,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":27,\"gridY\":96}],\"extraForegroundObjects\":[],\"foregroundCoverObjects\":[{\"gridX\":1685,\"gridY\":85,\"type\":0},{\"gridX\":790,\"gridY\":85,\"type\":0},{\"gridX\":805,\"gridY\":92,\"type\":3},{\"gridX\":420,\"gridY\":92,\"type\":2},{\"gridX\":320,\"gridY\":92,\"type\":1},{\"gridX\":353,\"gridY\":85,\"type\":0},{\"gridX\":446,\"gridY\":92,\"type\":3},{\"gridX\":8,\"gridY\":92,\"type\":1},{\"gridX\":46,\"gridY\":92,\"type\":3}],\"markers\":[{\"gridX\":680,\"gridY\":0,\"type\":0},{\"gridX\":1608,\"gridY\":0,\"type\":1},{\"gridX\":1768,\"gridY\":0,\"type\":0},{\"gridX\":2664,\"gridY\":0,\"type\":1},{\"gridX\":2728,\"gridY\":0,\"type\":0},{\"gridX\":3592,\"gridY\":0,\"type\":1}]}";
    
    static std::string text = ss.str();
    
    s_pInstance = new Chapter1Level10(text.c_str());
}

void Chapter1Level10::destroy()
{
    assert(s_pInstance);
    
    delete s_pInstance;
    s_pInstance = nullptr;
}

Chapter1Level10 * Chapter1Level10::getInstance()
{
    return s_pInstance;
}
