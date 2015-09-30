//
//  BackgroundSky.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "BackgroundSky.h"
#include "GameConstants.h"
#include "EntityUtils.h"
#include "EntityAnchor.h"
#include "Jon.h"
#include "Game.h"

BackgroundSky::BackgroundSky(float x) : PhysicalEntity(x, 0, CAM_WIDTH, 14.038128249566725f), m_fX(0), m_fY(0)
{
    EntityUtils::applyAnchor(*this, ANCHOR_TOP);
}

void BackgroundSky::update(Game& game)
{
    float y = GAME_HEIGHT - game.getJon().getPosition().getY();
    y /= GAME_HEIGHT;
    y *= 169;
    y =  y < 0 ? 0 : y;
    
    float x = game.getJon().getPosition().getX() * 6;
    float farRight = game.getFarRight();
    float farCamPos = farRight - CAM_WIDTH + JON_STARTING_X;
    if (game.getJon().getPosition().getX() > farCamPos)
    {
        x = farCamPos * 6;
    }
    
    m_fX = x;
    m_fY = y;
}

float BackgroundSky::getX()
{
    return m_fX;
}

float BackgroundSky::getY()
{
    return m_fY;
}