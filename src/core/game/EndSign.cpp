//
//  EndSign.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "EndSign.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "EntityUtils.h"

#define END_SIGN_WIDTH 0.7953216374269005f

EndSign::EndSign(float x, EntityAnchor anchor) : PhysicalEntity(x + END_SIGN_WIDTH / 2, 0, END_SIGN_WIDTH, 1.123050259965338f)
{
    EntityUtils::applyAnchor(*this, anchor);
    
    updateBounds();
}