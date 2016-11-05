//
//  TitlePanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/8/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__TitlePanel__
#define __nosfuratu__TitlePanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "RTTI.h"

class TitlePanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    TitlePanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH, float height = CAM_HEIGHT);
    
    virtual void update(float deltaTime);
    
    bool isLightningStriking();
    
private:
    float m_fTimeBetweenStrikes;
    bool m_isLightningStriking;
};

#endif /* defined(__nosfuratu__TitlePanel__) */
