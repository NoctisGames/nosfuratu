//
//  DSLineBatcher.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/25/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__DSLineBatcher__
#define __noctisgames__DSLineBatcher__

#include "LineBatcher.h"

#include <vector>

#include <3ds.h>

struct LINE
{
    float oX, oY, eX, eY; // Vertices
    float r, g, b, a; // Color for Vertices
};

class DSLineBatcher : public LineBatcher
{
public:
    DSLineBatcher(gfxScreen_t screen, int screenWidth, int screenHeight);

    virtual void beginBatch();

    virtual void endBatch();
    
    virtual void endBatch(GpuProgramWrapper &gpuProgramWrapper);

    virtual void renderLine(float originX, float originY, float endX, float endY, Color &c);

private:
    std::vector<LINE> m_lines;
    gfxScreen_t m_screen;
    int m_iScreenWidth;
    int m_iScreenHeight;
};

#endif /* defined(__noctisgames__DSLineBatcher__) */
