//
//  RectangleBatcher.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__RectangleBatcher__
#define __noctisgames__RectangleBatcher__

#include "Color.h"

class Rectangle;

class RectangleBatcher
{
public:
    RectangleBatcher(bool isFill = false);
    
    virtual void beginBatch() = 0;
    
    virtual void endBatch() = 0;
    
    void renderRectangle(Rectangle &rectangle, Color &color);
    
    virtual void renderRectangle(float leftX, float bottomY, float rightX, float topY, Color &color) = 0;
    
protected:
    bool m_isFill;
    int m_iNumRectangles;
};

#endif /* defined(__noctisgames__RectangleBatcher__) */
