//
//  Chapter1Levels.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "Chapter1Levels.h"
#include "State.h"
#include "GameScreen.h"

/// Chapter 1 Level 1 ///

Chapter1Level1 * Chapter1Level1::getInstance()
{
    static Chapter1Level1 *instance = new Chapter1Level1("ZZZZZZZZZZ");
    
    return instance;
}

/// Chapter 1 Level 2 ///

Chapter1Level2 * Chapter1Level2::getInstance()
{
    static Chapter1Level2 *instance = new Chapter1Level2("ZZZZZZZZZZ");
    
    return instance;
}

/// Chapter 1 Level 3 ///

Chapter1Level3 * Chapter1Level3::getInstance()
{
    static Chapter1Level3 *instance = new Chapter1Level3("ZZZZZZZZZZ");
    
    return instance;
}
