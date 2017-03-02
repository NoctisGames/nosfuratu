//
//  GameMarker.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/26/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameMarker__
#define __nosfuratu__GameMarker__

#include "GridLockedPhysicalEntity.h"

#include "RTTI.h"

class GameMarker : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static GameMarker* create(int gridX, int gridY, int type);
    
    GameMarker(int gridX, int type);
    
    int getType();
    
private:
    int m_type;
};

#endif /* defined(__nosfuratu__GameMarker__) */
