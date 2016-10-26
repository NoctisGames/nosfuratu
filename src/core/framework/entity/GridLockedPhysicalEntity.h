//
//  GridLockedPhysicalEntity.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GridLockedPhysicalEntity__
#define __nosfuratu__GridLockedPhysicalEntity__

#define GRID_MANAGER (GridManager::getInstance())
#define GM_GRID_CELL_SIZE (GRID_MANAGER->getGridCellSize())

extern "C"
{
#include "platform_gl.h"
#include "linmath.h"
}

#include <memory>
#include <vector>

class GpuProgramWrapper;

class GridManager
{
public:
    static GridManager* getInstance()
    {
        static GridManager *instance = new GridManager();
        
        return instance;
    }
    
    void setGridCellSize(float gridCellSize) { m_fGridCellSize = gridCellSize; }
    float getGridCellSize() { return m_fGridCellSize; }
    
private:
    float m_fGridCellSize;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    GridManager() : m_fGridCellSize(0) {}
    GridManager(const GridManager&);
    GridManager& operator=(const GridManager&);
    ~GridManager();
};

#include "PhysicalEntity.h"

class GridLockedPhysicalEntity : public PhysicalEntity
{
public:
    GridLockedPhysicalEntity(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
	virtual void updateBounds();

	virtual void placeOn(float itemTopY);
		
	virtual void snapToGrid(int gridCellSizeScalar = 1);

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

#endif /* defined(__nosfuratu__GridLockedPhysicalEntity__) */
