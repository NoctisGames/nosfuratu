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
    static Chapter1Level21 *instance = new Chapter1Level21("{\"world\":1,\"level\":21,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":60,\"gridY\":96,\"type\":0},{\"gridX\":168,\"gridY\":96,\"type\":2},{\"gridX\":308,\"gridY\":96,\"type\":1},{\"gridX\":444,\"gridY\":96,\"type\":0},{\"gridX\":568,\"gridY\":96,\"type\":1},{\"gridX\":696,\"gridY\":96,\"type\":1},{\"gridX\":656,\"gridY\":96,\"type\":0},{\"gridX\":776,\"gridY\":96,\"type\":2},{\"gridX\":832,\"gridY\":96,\"type\":1},{\"gridX\":872,\"gridY\":96,\"type\":0},{\"gridX\":904,\"gridY\":96,\"type\":1},{\"gridX\":944,\"gridY\":96,\"type\":2}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":25},{\"gridX\":8,\"gridY\":0,\"type\":28},{\"gridX\":136,\"gridY\":0,\"type\":28},{\"gridX\":264,\"gridY\":0,\"type\":28},{\"gridX\":392,\"gridY\":0,\"type\":28},{\"gridX\":520,\"gridY\":0,\"type\":28},{\"gridX\":648,\"gridY\":0,\"type\":28},{\"gridX\":776,\"gridY\":0,\"type\":28},{\"gridX\":904,\"gridY\":0,\"type\":28},{\"gridX\":1032,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[],\"holes\":[],\"foregroundObjects\":[{\"gridX\":776,\"gridY\":96,\"type\":12}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":744,\"gridY\":96}],\"enemies\":[],\"collectibles\":[],\"jons\":[{\"gridX\":28,\"gridY\":104}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}
