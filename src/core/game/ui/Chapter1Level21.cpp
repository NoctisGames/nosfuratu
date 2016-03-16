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
    static Chapter1Level21 *instance = new Chapter1Level21("{\"world\":1,\"level\":10,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":72,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":392,\"gridY\":50,\"type\":11},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":136,\"gridY\":50,\"type\":13},{\"gridX\":264,\"gridY\":50,\"type\":13},{\"gridX\":680,\"gridY\":48,\"type\":13},{\"gridX\":808,\"gridY\":48,\"type\":13},{\"gridX\":936,\"gridY\":48,\"type\":13},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":680,\"gridY\":88,\"type\":23},{\"gridX\":808,\"gridY\":88,\"type\":23},{\"gridX\":936,\"gridY\":88,\"type\":23},{\"gridX\":416,\"gridY\":0,\"type\":25},{\"gridX\":424,\"gridY\":0,\"type\":28},{\"gridX\":552,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[{\"gridX\":392,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":208,\"gridY\":80,\"type\":0},{\"gridX\":848,\"gridY\":80,\"type\":0}],\"foregroundObjects\":[{\"gridX\":1051,\"gridY\":96,\"type\":12},{\"gridX\":404,\"gridY\":55,\"type\":19}],\"midBossForegroundObjects\":[{\"gridX\":225,\"gridY\":96,\"type\":37},{\"gridX\":868,\"gridY\":96,\"type\":38}],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":96,\"gridY\":104,\"type\":0},{\"gridX\":112,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":104}],\"extraForegroundObjects\":[]}");
    
    return instance;
}