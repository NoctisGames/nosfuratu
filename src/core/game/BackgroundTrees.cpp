//
//  BackgroundTrees.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "BackgroundTrees.h"
#include "GameConstants.h"
#include "Jon.h"
#include "Game.h"

BackgroundTrees::BackgroundTrees(float x) : PhysicalEntity(x, 14.962305025996535f, CAM_WIDTH, 11.909012131715771f), m_fX(0)
{
    // Empty
}

void BackgroundTrees::update(Game& game)
{
    float x = game.getJon().getPosition().getX() * 12;
    float farRight = game.getFarRight();
    float farCamPos = farRight - CAM_WIDTH + JON_STARTING_X;
    if (game.getJon().getPosition().getX() > farCamPos)
    {
        x = farCamPos * 12;
    }
    
    m_fX = x;
}

float BackgroundTrees::getX()
{
    return m_fX;
}