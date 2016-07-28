//
//  BackButton.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__BackButton__
#define __nosfuratu__BackButton__

#include "PhysicalEntity.h"
#include "GameConstants.h"

class BackButton : public PhysicalEntity
{
public:
    BackButton(float x = 0.9140625f / 2, float y = CAM_HEIGHT - 0.9140625f / 2, float width = 0.9140625f, float height = 0.9140625f);
};

#endif /* defined(__nosfuratu__BackButton__) */
