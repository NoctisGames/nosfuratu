//
//  GridLockedPhysicalEntity.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "GridLockedPhysicalEntity.h"

GridLockedPhysicalEntity::GridLockedPhysicalEntity(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : PhysicalEntity(gridX * GRID_CELL_SIZE, gridY * GRID_CELL_SIZE, gridWidth * GRID_CELL_SIZE, gridHeight * GRID_CELL_SIZE), m_iGridX(gridX), m_iGridY(gridY), m_iGridWidth(gridWidth), m_iGridHeight(gridHeight), m_fBoundsX(boundsX), m_fBoundsY(boundsY), m_fBoundsWidth(boundsWidth), m_fBoundsHeight(boundsHeight)
{
    updateBounds();
    m_bounds->getLowerLeft().set(gridX * GRID_CELL_SIZE, gridY * GRID_CELL_SIZE);
    m_position->set(m_iGridX * GRID_CELL_SIZE, m_iGridY * GRID_CELL_SIZE);
    m_position->sub(getWidth() * m_fBoundsX, getHeight() * m_fBoundsY);
    m_position->add(getWidth() / 2, getHeight() / 2);
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
    
	m_bounds->getLowerLeft().set(m_iGridX * gridCellSize, m_iGridY * gridCellSize);
    m_position->set(m_iGridX * gridCellSize, m_iGridY * gridCellSize);
    m_position->sub(getWidth() * m_fBoundsX, getHeight() * m_fBoundsY);
    m_position->add(getWidth() / 2, getHeight() / 2);
    
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