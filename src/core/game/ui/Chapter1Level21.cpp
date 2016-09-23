//
//  Chapter1Level21.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/16/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenLevels.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"

/// Chapter 1 Level 21 ///

Chapter1Level21 * Chapter1Level21::getInstance()
{
    static Chapter1Level21 *instance = new Chapter1Level21("{\"world\":1,\"level\":21,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":88,\"type\":20},{\"gridX\":8,\"gridY\":88,\"type\":23},{\"gridX\":168,\"gridY\":88,\"type\":23}],\"pits\":[],\"exitGrounds\":[{\"gridX\":136,\"gridY\":88,\"type\":0}],\"holes\":[],\"foregroundObjects\":[{\"gridX\":224,\"gridY\":124,\"type\":0},{\"gridX\":228,\"gridY\":124,\"type\":1},{\"gridX\":242,\"gridY\":124,\"type\":1},{\"gridX\":256,\"gridY\":124,\"type\":2},{\"gridX\":288,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":226,\"gridY\":129}],\"enemies\":[],\"collectibles\":[],\"jons\":[{\"gridX\":24,\"gridY\":108}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}
