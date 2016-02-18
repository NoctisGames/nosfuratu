//
//  BackButton.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__BackButton__
#define __nosfuratu__BackButton__

#include "PhysicalEntity.h"
#include "GameConstants.h"

class BackButton : public PhysicalEntity
{
public:
    BackButton(float x = 1, float y = CAM_HEIGHT - 0.6456424857638f / 2, float width = 2, float height = 0.6456424857638f);
};

#endif /* defined(__nosfuratu__BackButton__) */
