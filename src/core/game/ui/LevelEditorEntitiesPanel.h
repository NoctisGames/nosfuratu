//
//  LevelEditorEntitiesPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__LevelEditorEntitiesPanel__
#define __nosfuratu__LevelEditorEntitiesPanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
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
#include "OverlapTester.h"

#include <vector>

class Game;

#define LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED -1
#define LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED 0

class LevelEditorEntitiesPanel : public PhysicalEntity
{
public:
    LevelEditorEntitiesPanel(float x = CAM_WIDTH + (3.76608187134503f - 1.89473684210526) / 2, float y = CAM_HEIGHT / 2, float width = 3.76608187134503f, float height = CAM_HEIGHT);
    
    int handleTouch(TouchEvent& te, Vector2D& touchPoint, Game& game, Vector2D& camPos, PhysicalEntity** lastAddedEntity);
    
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
    
    float getEntitiesCameraPos();
    
    bool isOpen();
    
private:
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
    
    std::unique_ptr<Rectangle> m_openButton;
    std::unique_ptr<Rectangle> m_closeButton;
    std::unique_ptr<Vector2D> m_touchPointDown;
    std::unique_ptr<Vector2D> m_touchPointDown2;
    float m_fEntitiesCameraPos;
    float m_fEntitiesHeight;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelEditorEntitiesPanel__) */
