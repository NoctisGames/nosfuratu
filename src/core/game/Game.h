//
//  Game.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Game__
#define __nosfuratu__Game__

#include "Jon.h"
#include "GamePlatform.h"
#include "Carrot.h"
#include "Ground.h"
#include "Tree.h"

#include <memory>
#include <vector>

class Game
{
public:
    Game();
    
    void update(float deltaTime);
    
    Jon& getJon();
    
    std::vector<Tree>& getTrees();
    
    std::vector<GamePlatform>& getPlatforms();
    
    std::vector<Carrot>& getCarrots();
    
    std::vector<Ground>& getGrounds();
    
    bool resetGame();
    
private:
    std::unique_ptr<Jon> m_jon;
    std::unique_ptr<std::vector<Tree>> m_trees;
    std::unique_ptr<std::vector<GamePlatform>> m_platforms;
    std::unique_ptr<std::vector<Carrot>> m_carrots;
    std::unique_ptr<std::vector<Ground>> m_grounds;
    
    bool m_resetGame;
};

#endif /* defined(__nosfuratu__Game__) */
