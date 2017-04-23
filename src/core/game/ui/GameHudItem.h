//
//  GameHudItem.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/1/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameHudItem__
#define __nosfuratu__GameHudItem__

#include "PhysicalEntity.h"

#include "RTTI.h"

class GameHudItem : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    static GameHudItem createCarrot();
    static GameHudItem createGoldenCarrot();
    static GameHudItem createVial();
    
    GameHudItem(int type);
    
    bool isCarrot();
    bool isGolden();
    bool isVial();
    
private:
    int m_iType;
};

#endif /* defined(__nosfuratu__GameHudItem__) */
