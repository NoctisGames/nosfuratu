//
//  Direct3DRectangleBatcher.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "pch.h"
#include "Direct3DRectangleBatcher.h"
#include "GameConstants.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "Direct3DProgram.h"
#include "Direct3DManager.h"
#include "GpuProgramWrapper.h"
#include "Direct3DGeometryGpuProgramWrapper.h"

Direct3DRectangleBatcher::Direct3DRectangleBatcher(const std::shared_ptr<DX::DeviceResources>& deviceResources, bool isFill) : RectangleBatcher(isFill), m_deviceResources(deviceResources)
{
    // Empty
}

void Direct3DRectangleBatcher::beginBatch()
{
	D3DManager->m_colorVertices.clear();
	m_iNumRectangles = 0;
}

void Direct3DRectangleBatcher::endBatch()
{
	if (m_iNumRectangles > 0)
	{
		// set the primitive topology
		m_deviceResources->GetD3DDeviceContext()->IASetPrimitiveTopology(m_isFill ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		D3DManager->m_colorProgram->bind();

		m_deviceResources->GetD3DDeviceContext()->DrawIndexed(m_iNumRectangles * INDICES_PER_RECTANGLE, 0, 0);

        D3DManager->m_colorProgram->unbind();
	}
}

void Direct3DRectangleBatcher::renderRectangle(Rectangle &rectangle, Color &color)
{
	float x1 = rectangle.getLowerLeft().getX();
	float y1 = rectangle.getLowerLeft().getY();
	float x2 = x1 + rectangle.getWidth();
	float y2 = y1 + rectangle.getHeight();

	renderRectangle(x1, y1, x2, y2, color);
}

void Direct3DRectangleBatcher::renderRectangle(float x1, float y1, float x2, float y2, Color &color)
{
	D3DManager->addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha);
	D3DManager->addVertexCoordinate(x1, y2, 0, color.red, color.green, color.blue, color.alpha);
	D3DManager->addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha);
	D3DManager->addVertexCoordinate(x2, y1, 0, color.red, color.green, color.blue, color.alpha);

	m_iNumRectangles++;
}