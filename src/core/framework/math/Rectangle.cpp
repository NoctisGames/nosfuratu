//
//  Rectangle.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Rectangle.h"
#include "Vector2D.h"

Rectangle::Rectangle(float x, float y, float width, float height, float angle)
{
    m_lowerLeft = std::unique_ptr<Vector2D>(new Vector2D(x, y));
    m_fWidth = width;
    m_fHeight = height;
    m_fAngle = angle;
}

Vector2D& Rectangle::getLowerLeft()
{
    return *m_lowerLeft;
}

float Rectangle::getRight()
{
    return m_lowerLeft->getX() + m_fWidth;
}

float Rectangle::getTop()
{
    return m_lowerLeft->getY() + m_fHeight;
}

float Rectangle::getLeft()
{
    return m_lowerLeft->getX();
}

float Rectangle::getBottom()
{
    return m_lowerLeft->getY();
}

float Rectangle::getWidth()
{
    return m_fWidth;
}

void Rectangle::setWidth(float width)   
{
    m_fWidth = width;
}

float Rectangle::getHeight()
{
    return m_fHeight;
}

void Rectangle::setHeight(float height)
{
    m_fHeight = height;
}

float Rectangle::getAngle()
{
    return m_fAngle;
}

void Rectangle::setAngle(float angle)
{
    m_fAngle = angle;
}
