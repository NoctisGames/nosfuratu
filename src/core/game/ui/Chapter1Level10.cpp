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

/// Chapter 1 Level 10 Mid Boss ///

Chapter1Level10 * Chapter1Level10::getInstance()
{
    static Chapter1Level10 *instance = new Chapter1Level10("TODO");
    
    return instance;
}