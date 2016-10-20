//
//  DSRectangleBatcher.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/25/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__DSRectangleBatcher__
#define __noctisgames__DSRectangleBatcher__

#include "RectangleBatcher.h"

#include <vector>

#include <3ds.h>

struct RECT
{
    float x1, y1, x2, y2; // Rectangle Data
    float r, g, b, a; // Color for Vertices
};

class DSRectangleBatcher : public RectangleBatcher
{
public:
    DSRectangleBatcher(gfxScreen_t screen, int screenWidth, int screenHeight, bool isFill);
    
    virtual void beginBatch();
    
    virtual void endBatch();
    
    virtual void endBatch(GpuProgramWrapper &gpuProgramWrapper);
    
    virtual void renderRectangle(float x1, float y1, float x2, float y2, Color &color);
    
private:
    std::vector<RECT> m_rects;
    gfxScreen_t m_screen;
    int m_iScreenWidth;
    int m_iScreenHeight;
};

#endif /* defined(__noctisgames__DSRectangleBatcher__) */
