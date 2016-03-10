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
    static Chapter1Level10 *instance = new Chapter1Level10("{\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":80,\"gridY\":96,\"type\":0},{\"gridX\":128,\"gridY\":96,\"type\":1},{\"gridX\":200,\"gridY\":96,\"type\":2},{\"gridX\":264,\"gridY\":96,\"type\":0},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":360,\"gridY\":96,\"type\":1}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":369,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":104,\"gridY\":104,\"type\":0},{\"gridX\":128,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":32,\"gridY\":120}],\"extraForegroundObjects\":[]}");
    
    return instance;
}