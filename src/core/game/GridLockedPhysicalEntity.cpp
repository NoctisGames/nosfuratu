//
//  GridLockedPhysicalEntity.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "GridLockedPhysicalEntity.h"

GridLockedPhysicalEntity::GridLockedPhysicalEntity(int gridX, int gridY, int gridWidth, int gridHeight) : PhysicalEntity(gridX * GRID_CELL_SIZE + (gridWidth * GRID_CELL_SIZE / 2), gridY * GRID_CELL_SIZE + (gridHeight * GRID_CELL_SIZE / 2), gridWidth * GRID_CELL_SIZE, gridHeight * GRID_CELL_SIZE), m_iGridX(gridX), m_iGridY(gridY), m_iGridWidth(gridWidth), m_iGridHeight(gridHeight)
{
    // Empty
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
}

int GridLockedPhysicalEntity::getGridX()
{
    return m_iGridX;
}

int GridLockedPhysicalEntity::getGridY()
{
    return m_iGridY;
}