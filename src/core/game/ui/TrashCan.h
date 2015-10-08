//
//  TrashCan.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/5/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__TrashCan__
#define __nosfuratu__TrashCan__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"

class TrashCan : public PhysicalEntity
{
public:
    TrashCan(float x = ZOOMED_OUT_CAM_WIDTH / 2, float y = GAME_HEIGHT - 1.4974003466204506f / 2, float width = 1.4970760233918128f, float height = 1.4974003466204506f);
    
    void update(Vector2D& cameraPosition);
    
    void setHighlighted(bool isHighlighted);
    
    bool isHighlighted();
    
private:
    bool m_isHighlighted;
};

#endif /* defined(__nosfuratu__TrashCan__) */
