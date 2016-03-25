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
    static Chapter1Level10 *instance = new Chapter1Level10("{\"world\":1,\"level\":10,\"midgrounds\":[{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":72,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":50,\"type\":10},{\"gridX\":392,\"gridY\":50,\"type\":11},{\"gridX\":1064,\"gridY\":50,\"type\":11},{\"gridX\":2376,\"gridY\":48,\"type\":11},{\"gridX\":8,\"gridY\":50,\"type\":13},{\"gridX\":136,\"gridY\":50,\"type\":13},{\"gridX\":264,\"gridY\":50,\"type\":13},{\"gridX\":680,\"gridY\":50,\"type\":13},{\"gridX\":808,\"gridY\":50,\"type\":13},{\"gridX\":936,\"gridY\":50,\"type\":13},{\"gridX\":1352,\"gridY\":50,\"type\":13},{\"gridX\":1480,\"gridY\":50,\"type\":13},{\"gridX\":1608,\"gridY\":50,\"type\":13},{\"gridX\":1736,\"gridY\":50,\"type\":13},{\"gridX\":1864,\"gridY\":50,\"type\":13},{\"gridX\":1992,\"gridY\":50,\"type\":13},{\"gridX\":2120,\"gridY\":50,\"type\":13},{\"gridX\":2248,\"gridY\":48,\"type\":13},{\"gridX\":2664,\"gridY\":48,\"type\":13},{\"gridX\":2792,\"gridY\":48,\"type\":13},{\"gridX\":2920,\"gridY\":48,\"type\":13},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":1736,\"gridY\":90,\"type\":23},{\"gridX\":1864,\"gridY\":90,\"type\":23},{\"gridX\":1992,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":2120,\"gridY\":90,\"type\":23},{\"gridX\":680,\"gridY\":90,\"type\":23},{\"gridX\":2248,\"gridY\":88,\"type\":23},{\"gridX\":808,\"gridY\":90,\"type\":23},{\"gridX\":936,\"gridY\":88,\"type\":23},{\"gridX\":1352,\"gridY\":90,\"type\":23},{\"gridX\":2664,\"gridY\":88,\"type\":23},{\"gridX\":1480,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":1608,\"gridY\":90,\"type\":23},{\"gridX\":2792,\"gridY\":88,\"type\":23},{\"gridX\":2920,\"gridY\":88,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":2400,\"gridY\":0,\"type\":25},{\"gridX\":416,\"gridY\":0,\"type\":25},{\"gridX\":1088,\"gridY\":0,\"type\":25},{\"gridX\":2536,\"gridY\":0,\"type\":28},{\"gridX\":552,\"gridY\":0,\"type\":28},{\"gridX\":1096,\"gridY\":0,\"type\":28},{\"gridX\":1224,\"gridY\":0,\"type\":28},{\"gridX\":2408,\"gridY\":0,\"type\":28},{\"gridX\":424,\"gridY\":0,\"type\":28}],\"pits\":[],\"exitGrounds\":[{\"gridX\":392,\"gridY\":88,\"type\":1},{\"gridX\":1064,\"gridY\":88,\"type\":1},{\"gridX\":2376,\"gridY\":88,\"type\":1}],\"holes\":[{\"gridX\":208,\"gridY\":73,\"type\":0},{\"gridX\":848,\"gridY\":73,\"type\":0},{\"gridX\":2092,\"gridY\":73,\"type\":2},{\"gridX\":2076,\"gridY\":73,\"type\":1},{\"gridX\":2060,\"gridY\":73,\"type\":1},{\"gridX\":2044,\"gridY\":73,\"type\":1},{\"gridX\":2028,\"gridY\":73,\"type\":1},{\"gridX\":2012,\"gridY\":73,\"type\":1},{\"gridX\":1996,\"gridY\":73,\"type\":1},{\"gridX\":1980,\"gridY\":73,\"type\":1},{\"gridX\":1964,\"gridY\":73,\"type\":1},{\"gridX\":1948,\"gridY\":73,\"type\":1},{\"gridX\":1932,\"gridY\":73,\"type\":1},{\"gridX\":1916,\"gridY\":73,\"type\":1},{\"gridX\":1900,\"gridY\":73,\"type\":1},{\"gridX\":1884,\"gridY\":73,\"type\":0}],\"foregroundObjects\":[{\"gridX\":2912,\"gridY\":96,\"type\":12},{\"gridX\":404,\"gridY\":55,\"type\":19},{\"gridX\":1076,\"gridY\":55,\"type\":19},{\"gridX\":2388,\"gridY\":55,\"type\":19}],\"midBossForegroundObjects\":[{\"gridX\":225,\"gridY\":96,\"type\":37},{\"gridX\":868,\"gridY\":96,\"type\":38},{\"gridX\":2108,\"gridY\":96,\"type\":37}],\"countHissWithMinas\":[{\"gridX\":2876,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":80,\"gridY\":104,\"type\":0},{\"gridX\":96,\"gridY\":104,\"type\":0},{\"gridX\":112,\"gridY\":104,\"type\":0}],\"jons\":[{\"gridX\":24,\"gridY\":104}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":936,\"gridY\":0,\"type\":0},{\"gridX\":2248,\"gridY\":0,\"type\":1}]}");
    
    return instance;
}

void Chapter1Level10::execute(GameScreen* gs)
{
    Level::execute(gs);
    
    if (m_game->getJons().size() > 0)
    {
        Jon& jon = m_game->getJon();
        m_isChaseCamActivated = jon.getPosition().getY() > 12;
    }
}

void Chapter1Level10::exit(GameScreen* gs)
{
    Level::exit(gs);
    
    m_isChaseCamActivated = false;
}

void Chapter1Level10::updateCamera(GameScreen* gs, bool instant)
{
    if (m_isChaseCamActivated)
    {
        gs->m_renderer->updateCameraToFollowJon(*m_game, gs->m_fDeltaTime, true, instant);
    }
    else
    {
        Level::updateCamera(gs, instant);
    }
}