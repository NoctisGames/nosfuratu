//
//  Rectangle.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef Rectangle_H
#define Rectangle_H

#include <memory>

class Vector2D;

class Rectangle
{
public:
    Rectangle(float x, float y, float width, float height, float angle = 0);

	Rectangle(const Rectangle &) {}
    
    Vector2D& getLowerLeft();
    
    float getRight();
    
    float getTop();
    
    float getLeft();
    
    float getBottom();
    
    float getWidth();
    
    void setWidth(float width);
    
    float getHeight();
    
    void setHeight(float height);
    
    float getAngle();
    
    void setAngle(float angle);
    
private:
    std::unique_ptr<Vector2D> m_lowerLeft;
    float m_fWidth;
    float m_fHeight;
    float m_fAngle;
};

#endif /* Rectangle_H */
