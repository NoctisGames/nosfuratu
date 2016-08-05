//
//  CutscenePanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__CutscenePanel__
#define __nosfuratu__CutscenePanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"

class CutscenePanel : public PhysicalEntity
{
public:
    CutscenePanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH, float height = CAM_HEIGHT);
    
    virtual void update(float deltaTime);
    
    bool isLightningStriking();
    
private:
    float m_fTimeBetweenStrikes;
    bool m_isLightningStriking;
};

#endif /* defined(__nosfuratu__CutscenePanel__) */
