//
//  BatPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 4/4/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "BatPanel.h"

#include "Game.h"
#include "GameScreen.h"
#include "OverlapTester.h"

BatGoalType calcBatGoalType(int world, int level)
{
    switch (world)
    {
        case 1:
        {
            switch (level)
            {
                case 1:
                    return BatGoalType_Jump;
                case 2:
                    return BatGoalType_DoubleJump;
                case 3:
                    return BatGoalType_Vampire;
                case 10:
                    return BatGoalType_Drill;
                case 11:
                    return BatGoalType_Stomp;
                case 21:
                    return BatGoalType_Dash;
            }
        }
    }
    
    return BatGoalType_None;
}

BatPanel::BatPanel(Game* game, int world, int level) : m_isRequestingInput(false)
{
    m_bat = std::unique_ptr<Bat>(new Bat());
    m_batInstruction = std::unique_ptr<BatInstruction>(new BatInstruction());
    m_game = game;
    m_type = calcBatGoalType(world, level);
}

void BatPanel::update(GameScreen* gs)
{
    
}
