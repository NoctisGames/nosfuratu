//
//  GridLockedPhysicalEntity.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "GridLockedPhysicalEntity.h"

GridLockedPhysicalEntity::GridLockedPhysicalEntity(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : PhysicalEntity(gridX * GRID_CELL_SIZE + (gridWidth * GRID_CELL_SIZE / 2), gridY * GRID_CELL_SIZE + (gridHeight * GRID_CELL_SIZE / 2), gridWidth * GRID_CELL_SIZE, gridHeight * GRID_CELL_SIZE), m_iGridX(gridX), m_iGridY(gridY), m_iGridWidth(gridWidth), m_iGridHeight(gridHeight), m_fBoundsX(boundsX), m_fBoundsY(boundsY), m_fBoundsWidth(boundsWidth), m_fBoundsHeight(boundsHeight)
{
	updateBounds();
}

void GridLockedPhysicalEntity::updateBounds()
{
	m_bounds->setWidth(getWidth());
	m_bounds->setHeight(getHeight());

	PhysicalEntity::updateBounds();

	m_bounds->getLowerLeft().add(getWidth() * m_fBoundsX, getHeight() * m_fBoundsY);
	m_bounds->setWidth(getWidth() * m_fBoundsWidth);
	m_bounds->setHeight(getHeight() * m_fBoundsHeight);
}

void GridLockedPhysicalEntity::snapToGrid(int gridCellSizeScalar)
{
    float left = getBounds().getLeft();
    float bottom = getBounds().getBottom();
    
    float gridCellSize = GRID_CELL_SIZE * gridCellSizeScalar;
    m_iGridX = left / gridCellSize;
    
    m_iGridY = bottom / gridCellSize;
    
	m_position->set(m_iGridX * gridCellSize + (m_iGridWidth * GRID_CELL_SIZE / 2), m_iGridY * gridCellSize + (m_iGridHeight * GRID_CELL_SIZE / 2));
    
    m_iGridX *= gridCellSizeScalar;
    m_iGridY *= gridCellSizeScalar;

	updateBounds();
}

int GridLockedPhysicalEntity::getGridX()
{
    return m_iGridX;
}

int GridLockedPhysicalEntity::getGridY()
{
    return m_iGridY;
}