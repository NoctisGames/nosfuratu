//
//  Direct3DLineBatcher.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 11/16/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#define VERTICES_PER_LINE 2

#include "pch.h"
#include "Direct3DLineBatcher.h"
#include "GameConstants.h"
#include "Line.h"
#include "Vector2D.h"
#include "Direct3DProgram.h"
#include "Direct3DManager.h"
#include "GpuProgramWrapper.h"
#include "Direct3DGeometryGpuProgramWrapper.h"

Direct3DLineBatcher::Direct3DLineBatcher() : LineBatcher()
{
	// Empty
}

void Direct3DLineBatcher::beginBatch()
{
	D3DManager->m_colorVertices.clear();
	m_iNumLines = 0;
}

void Direct3DLineBatcher::endBatch()
{
	endBatch(*D3DManager->m_colorProgram);
}

void Direct3DLineBatcher::endBatch(GpuProgramWrapper &gpuProgramWrapper)
{
	if (m_iNumLines > 0)
	{
		// set the primitive topology
		D3DManager->m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		gpuProgramWrapper.bind();

		D3DManager->m_d3dContext->Draw(m_iNumLines * VERTICES_PER_LINE, 0);

		gpuProgramWrapper.unbind();
	}
}

void Direct3DLineBatcher::renderLine(float originX, float originY, float endX, float endY, Color &color)
{
	D3DManager->addVertexCoordinate(originX, originY, 0, color.red, color.green, color.blue, color.alpha);
	D3DManager->addVertexCoordinate(endX, endY, 0, color.red, color.green, color.blue, color.alpha);

	m_iNumLines++;
}