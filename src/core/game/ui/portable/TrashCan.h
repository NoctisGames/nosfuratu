//
//  TrashCan.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/5/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__TrashCan__
#define __nosfuratu__TrashCan__

#include "PhysicalEntity.h"

#include "GameConstants.h"

#include "RTTI.h"

class Vector2D;

class TrashCan : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    TrashCan(float x = ZOOMED_OUT_CAM_WIDTH / 2, float y = GAME_HEIGHT - 1.4974003466204506f / 2, float width = 1.4970760233918128f, float height = 1.4974003466204506f);
    
    void update(Vector2D& cameraPosition);
    
    void setHighlighted(bool isHighlighted);
    
    bool isHighlighted();
    
private:
    bool m_isHighlighted;
};

#endif /* defined(__nosfuratu__TrashCan__) */
