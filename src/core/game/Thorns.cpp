//
//  Thorns.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Thorns.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "EntityUtils.h"

#define THORNS_WIDTH 2.853801169590643f
#define THORNS_HEIGHT 1.474003466204506f

Thorns::Thorns(float x, EntityAnchor anchor) : PhysicalEntity(x + THORNS_WIDTH / 2, 0, THORNS_WIDTH, THORNS_HEIGHT)
{
    EntityUtils::applyAnchor(*this, anchor);
    
    resetBounds(THORNS_WIDTH * 0.50f, THORNS_HEIGHT * 0.50f);
}