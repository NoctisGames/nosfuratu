//
//  Hole.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Hole__
#define __nosfuratu__Hole__

#include "PhysicalEntity.h"
#include "HoleCover.h"

#include <vector>

class Hole : public PhysicalEntity
{
public:
    static Hole* create(float x, float y, int type);
    
    Hole(float x, float y = 8.095320623916809f, float width = 3.2046783625730995f, float height = 2.246100519930676f);
    
    virtual void update(float deltaTime);
    
    void triggerBurrow();
    
    bool hasCover();
    
    std::vector<std::unique_ptr<HoleCover>>& getHoleCovers();
    
    int getType();
    
private:
    std::vector<std::unique_ptr<HoleCover>> m_holeCovers;
};

#endif /* defined(__nosfuratu__Hole__) */
