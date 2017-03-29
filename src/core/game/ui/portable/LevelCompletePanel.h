//
//  LevelCompletePanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/28/17.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__LevelCompletePanel__
#define __nosfuratu__LevelCompletePanel__

#include "PhysicalEntity.h"

#include "GameConstants.h"

#include "RTTI.h"

class ScoreIcon;

class LevelCompletePanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    LevelCompletePanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH * 0.7f, float height = CAM_HEIGHT * 0.7f);
    
    virtual ~LevelCompletePanel();
    
    virtual void update(float deltaTime);
    
    ScoreIcon* getClockIcon();
    ScoreIcon* getCarrotIcon();
    ScoreIcon* getGoldenCarrotIcon();
    ScoreIcon* getVialIcon();
    
private:
    ScoreIcon* m_clockIcon;
    ScoreIcon* m_carrotIcon;
    ScoreIcon* m_goldenCarrotIcon;
    ScoreIcon* m_vialIcon;
};

class ScoreIcon : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    ScoreIcon(float x, float y, float width, float height, int type);
    
    virtual void update(float deltaTime);
    
    void activate();
    
private:
    int m_type;
};

#endif /* defined(__nosfuratu__LevelCompletePanel__) */
