//
//  Game.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Game__
#define __nosfuratu__Game__

#include "BackgroundSky.h"
#include "BackgroundTrees.h"
#include "BackgroundCave.h"
#include "Tree.h"
#include "Ground.h"
#include "LogVerticalTall.h"
#include "GroundPlatform.h"
#include "EndSign.h"
#include "Carrot.h"
#include "GoldenCarrot.h"
#include "Jon.h"

#include <memory>
#include <vector>

class Game
{
public:
    Game();
    
    void update(float deltaTime);
    
    bool isJonGrounded();
    
    bool isJonBlockedHorizontally();
    
    bool isJonBlockedVertically();
    
    BackgroundSky& getBackgroundSky();
    
    BackgroundTrees& getBackgroundTrees();
    
    BackgroundCave& getBackgroundCave();
    
    std::vector<Tree>& getTrees();
    
    std::vector<Ground>& getGrounds();
    
    std::vector<LogVerticalTall>& getLogVerticalTalls();
    
    std::vector<GroundPlatform>& getPlatforms();
    
    std::vector<EndSign>& getEndSigns();
    
    std::vector<Carrot>& getCarrots();
    
    std::vector<GoldenCarrot>& getGoldenCarrots();
    
    Jon& getJon();
    
    float getFarRight();
    
    bool resetGame();
    
private:
    std::unique_ptr<BackgroundSky> m_backgroundSky;
    std::unique_ptr<BackgroundTrees> m_backgroundTrees;
    std::unique_ptr<BackgroundCave> m_backgroundCave;
    std::unique_ptr<std::vector<Tree>> m_trees;
    std::unique_ptr<std::vector<Ground>> m_grounds;
    std::unique_ptr<std::vector<LogVerticalTall>> m_logVerticalTalls;
    std::unique_ptr<std::vector<GroundPlatform>> m_platforms;
    std::unique_ptr<std::vector<EndSign>> m_endSigns;
    std::unique_ptr<std::vector<Carrot>> m_carrots;
    std::unique_ptr<std::vector<GoldenCarrot>> m_goldenCarrots;
    std::unique_ptr<Jon> m_jon;
    
    float m_fDeltaTime;
    bool m_resetGame;
};

#endif /* defined(__nosfuratu__Game__) */
