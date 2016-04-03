//
//  MidBossOwl.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 4/1/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MidBossOwl.h"
#include "Game.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "Jon.h"

#include <math.h>

MidBossOwl::MidBossOwl(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_game(nullptr), m_iDamage(0), m_state(MidBossOwlState_Sleeping)
{
    // Empty
}

void MidBossOwl::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    switch (m_state)
    {
        case MidBossOwlState_Dying:
        {
            // TODO
        }
            break;
        case MidBossOwlState_SlammingIntoTree:
        {
            // TODO
        }
            break;
        case MidBossOwlState_Pursuing:
        {
            // TODO
        }
            break;
        case MidBossOwlState_Awakening:
        {
            if (m_fStateTime > 0.20f)
            {
                setState(MidBossOwlState_Screeching);
            }
        }
            break;
        case MidBossOwlState_Dead:
        case MidBossOwlState_Screeching:
        case MidBossOwlState_Sleeping:
        default:
            break;
    }
}

MidBossOwlState MidBossOwl::getState()
{
    return m_state;
}

void MidBossOwl::goBackToSleep()
{
    m_fWidth = MID_BOSS_OWL_SLEEPING_WIDTH;
    m_fHeight = MID_BOSS_OWL_SLEEPING_HEIGHT;
    setState(MidBossOwlState_Sleeping);
}

void MidBossOwl::awaken()
{
    setState(MidBossOwlState_Awakening);
}

void MidBossOwl::beginPursuit()
{
    m_fWidth = MID_BOSS_OWL_NORMAL_WIDTH;
    m_fHeight = MID_BOSS_OWL_NORMAL_HEIGHT;
    
    setState(MidBossOwlState_Pursuing);
}

void MidBossOwl::setGame(Game* game)
{
    m_game = game;
}

#pragma mark private

void MidBossOwl::setState(MidBossOwlState state)
{
    m_state = state;
    m_fStateTime = 0.0f;
}