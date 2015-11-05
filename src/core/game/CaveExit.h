//
//  CaveExit.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/3/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__CaveExit__
#define __nosfuratu__CaveExit__

#include "PhysicalEntity.h"
#include "CaveExitCover.h"

#include <vector>

#define CAVE_EXIT_WIDTH 3.2046783625730995f * 1.75849056603774f
#define CAVE_EXIT_HEIGHT 2.246100519930676f * 1.62365591397849f

class CaveExit : public PhysicalEntity
{
public:
    static CaveExit* create(float x, float y, int type);
    
    CaveExit(float x, float y = 9.34f, float width = CAVE_EXIT_WIDTH, float height = CAVE_EXIT_HEIGHT);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    void triggerEruption();
    
    bool hasCover();
    
    std::vector<std::unique_ptr<CaveExitCover>>& getCaveExitCovers();
    
    int getType();
    
private:
    std::vector<std::unique_ptr<CaveExitCover>> m_CaveExitCovers;
};

#endif /* defined(__nosfuratu__CaveExit__) */
