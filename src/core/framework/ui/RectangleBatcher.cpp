//
//  RectangleBatcher.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "RectangleBatcher.h"
#include "Rectangle.h"
#include "Vector2D.h"

RectangleBatcher::RectangleBatcher(bool isFill) : m_isFill(isFill), m_iNumRectangles(0)
{
    // Empty
}

void RectangleBatcher::renderRectangle(Rectangle &rectangle, Color &color)
{
    float x1 = rectangle.getLeft();
    float y1 = rectangle.getBottom();
    float x2 = x1 + rectangle.getWidth();
    float y2 = y1 + rectangle.getHeight();
    
    renderRectangle(x1, y1, x2, y2, color);
}
