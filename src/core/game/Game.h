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
#include "CaveSkeleton.h"
#include "Ground.h"
#include "LogVerticalTall.h"
#include "LogVerticalShort.h"
#include "Thorns.h"
#include "Stump.h"
#include "SideSpike.h"
#include "UpwardSpike.h"
#include "JumpSpring.h"
#include "Rock.h"
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
    
    bool isJonHit();
    
    bool isJonLandingOnSpring();
    
    bool isSpinningBackFistDelivered();
    
    std::vector<BackgroundSky>& getBackgroundSkies();
    
    std::vector<BackgroundTrees>& getBackgroundTrees();
    
    std::vector<BackgroundCave>& getBackgroundCaves();
    
    std::vector<Tree>& getTrees();
    
    std::vector<CaveSkeleton>& getCaveSkeletons();
    
    std::vector<Ground>& getGrounds();
    
    std::vector<LogVerticalTall>& getLogVerticalTalls();
    
    std::vector<LogVerticalShort>& getLogVerticalShorts();
    
    std::vector<Thorns>& getThorns();
    
    std::vector<Stump>& getStumps();
    
    std::vector<SideSpike>& getSideSpikes();
    
    std::vector<UpwardSpike>& getUpwardSpikes();
    
    std::vector<JumpSpring>& getJumpSprings();
    
    std::vector<Rock>& getRocks();
    
    std::vector<GroundPlatform>& getPlatforms();
    
    std::vector<EndSign>& getEndSigns();
    
    std::vector<Carrot>& getCarrots();
    
    std::vector<GoldenCarrot>& getGoldenCarrots();
    
    Jon& getJon();
    
    float getFarRight();
    
    bool resetGame();
    
private:
    std::unique_ptr<std::vector<BackgroundSky>> m_backgroundSkies;
    std::unique_ptr<std::vector<BackgroundTrees>> m_backgroundTrees;
    std::unique_ptr<std::vector<BackgroundCave>> m_backgroundCaves;
    std::unique_ptr<std::vector<Tree>> m_trees;
    std::unique_ptr<std::vector<CaveSkeleton>> m_caveSkeletons;
    std::unique_ptr<std::vector<Ground>> m_grounds;
    std::unique_ptr<std::vector<LogVerticalTall>> m_logVerticalTalls;
    std::unique_ptr<std::vector<LogVerticalShort>> m_logVerticalShorts;
    std::unique_ptr<std::vector<Thorns>> m_thorns;
    std::unique_ptr<std::vector<Stump>> m_stumps;
    std::unique_ptr<std::vector<SideSpike>> m_sideSpikes;
    std::unique_ptr<std::vector<UpwardSpike>> m_upwardSpikes;
    std::unique_ptr<std::vector<JumpSpring>> m_jumpSprings;
    std::unique_ptr<std::vector<Rock>> m_rocks;
    std::unique_ptr<std::vector<GroundPlatform>> m_platforms;
    std::unique_ptr<std::vector<EndSign>> m_endSigns;
    std::unique_ptr<std::vector<Carrot>> m_carrots;
    std::unique_ptr<std::vector<GoldenCarrot>> m_goldenCarrots;
    std::unique_ptr<Jon> m_jon;
    
    float m_fDeltaTime;
    bool m_resetGame;
};

#endif /* defined(__nosfuratu__Game__) */
