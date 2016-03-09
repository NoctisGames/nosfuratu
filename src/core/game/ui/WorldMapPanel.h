//
//  WorldMapPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/8/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__WorldMapPanel__
#define __nosfuratu__WorldMapPanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"

class WorldMapPanel : public PhysicalEntity
{
public:
    WorldMapPanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH, float height = CAM_HEIGHT);
};

#endif /* defined(__nosfuratu__WorldMapPanel__) */
