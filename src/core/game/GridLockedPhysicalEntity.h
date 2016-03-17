//
//  GridLockedPhysicalEntity.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GridLockedPhysicalEntity__
#define __nosfuratu__GridLockedPhysicalEntity__

#include "PhysicalEntity.h"

#define GRID_CELL_SIZE 0.140625f

class GridLockedPhysicalEntity : public PhysicalEntity
{
public:
    GridLockedPhysicalEntity(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
	virtual void updateBounds();

    void snapToGrid(int gridCellSizeScalar = 1);
    
    int getGridX();
    
    int getGridY();
    
protected:
	float m_fBoundsX;
	float m_fBoundsY;
	float m_fBoundsWidth;
	float m_fBoundsHeight;
    int m_iGridX;
    int m_iGridY;
    int m_iGridWidth;
    int m_iGridHeight;
};

class Marker : public GridLockedPhysicalEntity
{
public:
    static Marker* create(int gridX, int gridY, int type)
    {
        return new Marker(gridX, type);
    }
    
    Marker(int gridX, int type) : GridLockedPhysicalEntity(gridX, 0, 2, 256), m_type(type) {}
    
    int getType() { return m_type; };
    
private:
    int m_type;
};

class CountHissWithMina : public GridLockedPhysicalEntity
{
public:
    static CountHissWithMina* create(int gridX, int gridY, int type)
    {
        return new CountHissWithMina(gridX, gridY);
    }
    
    CountHissWithMina(int gridX, int gridY) : GridLockedPhysicalEntity(gridX, gridY, 32, 32, 0, 0.1953125f, 1, 0.8046875f), m_type(-1), m_isMoving(false)
    {
        m_velocity->setX(6.0f);
    }
    
    virtual void update(float deltaTime)
    {
        if (m_isMoving)
        {
            GridLockedPhysicalEntity::update(deltaTime);
        }
    }
    
    void setMoving() { m_isMoving = true; }
    
    int getType() { return m_type; };
    
private:
    int m_type;
    bool m_isMoving;
};

#endif /* defined(__nosfuratu__GridLockedPhysicalEntity__) */
