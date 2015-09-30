//
//  BackgroundCave.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "BackgroundCave.h"
#include "GameConstants.h"
#include "EntityUtils.h"
#include "EntityAnchor.h"
#include "Jon.h"
#include "Game.h"

BackgroundCave::BackgroundCave(float x) : PhysicalEntity(x, 0, CAM_WIDTH, 11.277296360485268f), m_fX(0)
{
    EntityUtils::applyAnchor(*this, ANCHOR_BOTTOM);
}

void BackgroundCave::update(Game& game)
{
    float x = game.getJon().getPosition().getX() * 24;
    float farRight = game.getFarRight();
    float farCamPos = farRight - CAM_WIDTH + JON_STARTING_X;
    if (game.getJon().getPosition().getX() > farCamPos)
    {
        x = farCamPos * 24;
    }
    
    m_fX = x;
}

float BackgroundCave::getX()
{
    return m_fX;
}