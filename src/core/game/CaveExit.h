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

typedef enum
{
    CaveExitType_End,
    CaveExitType_Mid
} CaveExitType;

#define CAVE_EXIT_Y 9.347536617842877f
#define CAVE_EXIT_WIDTH 5.992509363295881f
#define CAVE_EXIT_HEIGHT 3.4513981358189083f

class CaveExit : public PhysicalEntity
{
public:
    static CaveExit* create(float x, float y, int type);
    
    CaveExit(float x, float y, float width, float height, CaveExitType type);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    void triggerEruption();
    
    bool hasCover();
    
    Rectangle& getHoleBounds();
    
    std::vector<std::unique_ptr<CaveExitCover>>& getCaveExitCovers();
    
    CaveExitType getEnumType();
    
    int getType();
    
private:
    CaveExitType m_type;
    std::unique_ptr<Rectangle> m_holeBounds;
    std::vector<std::unique_ptr<CaveExitCover>> m_caveExitCovers;
};

class CaveExitEnd : public CaveExit
{
public:
    CaveExitEnd(float x, float y = CAVE_EXIT_Y, float width = CAVE_EXIT_WIDTH, float height = CAVE_EXIT_HEIGHT) : CaveExit(x, y, width, height, CaveExitType_End) {}
};

class CaveExitMid : public CaveExit
{
public:
    CaveExitMid(float x, float y = CAVE_EXIT_Y, float width = CAVE_EXIT_WIDTH, float height = CAVE_EXIT_HEIGHT) : CaveExit(x, y, width, height, CaveExitType_Mid) {}
};

#endif /* defined(__nosfuratu__CaveExit__) */
