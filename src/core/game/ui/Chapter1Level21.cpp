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

/// Chapter 1 Level 21 End Boss ///

Chapter1Level21 * Chapter1Level21::getInstance()
{
    static Chapter1Level21 *instance = new Chapter1Level21("{\"world\":1,\"level\":21,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":354,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":386,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":353,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":392,\"gridY\":0,\"type\":0},{\"gridX\":520,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}