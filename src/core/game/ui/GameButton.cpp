//
//  GameButton.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameButton.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"

#include <assert.h>

GameButton* GameButton::create(GameButtonType type)
{
    switch (type)
    {
        case GameButtonType_BackToTitle:
        case GameButtonType_BackToLevelSelect:
            return new GameButton(1.51895680147063f / 2,
                                  CAM_HEIGHT - 1.52941176470592f / 2,
                                  1.51895680147063f,
                                  1.52941176470592f,
                                  type);
        case GameButtonType_Leaderboards:
            return new GameButton(CAM_WIDTH - 1.51895680147063f / 2,
                                  CAM_HEIGHT - 1.52941176470592f / 2,
                                  1.51895680147063f,
                                  1.52941176470592f,
                                  type);
        case GameButtonType_ViewOpeningCutscene:
            return new GameButton(CAM_WIDTH * 0.08639705882353f,
                                  CAM_HEIGHT * 0.56209150326797f,
                                  1.08079618566172f,
                                  1.08823529411766f,
                                  type);
        case GameButtonType_LevelEditor:
            return new GameButton(CAM_WIDTH - 1,
                                  CAM_HEIGHT - 0.6456424857638f / 2,
                                  2,
                                  0.6456424857638f,
                                  type);
    }
    
    assert(false);
}

GameButton::GameButton(float x, float y, float width, float height, GameButtonType type) : PhysicalEntity(x, y, width, height)
{
    m_type = type;
}

GameButtonType GameButton::getType()
{
    return m_type;
}