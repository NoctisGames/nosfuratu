//
//  Thorns.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Thorns.h"

Thorns* Thorns::create(float x, float y, int type)
{
    return new Thorns(x, y);
}

Thorns::Thorns(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    updateBounds();
}

void Thorns::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.0983606557377f, getHeight() * 0.03174603174603f);
    
    m_bounds->setWidth(getWidth() * 0.66393442622951f);
    m_bounds->setHeight(getHeight() * 0.71428571428571f);
}

int Thorns::getType()
{
    return -1;
}