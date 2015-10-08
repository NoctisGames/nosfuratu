//
//  OpenGLESRectangleBatcher.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESRectangleBatcher.h"
#include "macros.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GpuProgramWrapper.h"

OpenGLESRectangleBatcher::OpenGLESRectangleBatcher(bool isFill) : RectangleBatcher(isFill)
{
    // Empty
}

void OpenGLESRectangleBatcher::beginBatch()
{
    OGLESManager->m_colorVertices.clear();
    m_iNumRectangles = 0;
}

void OpenGLESRectangleBatcher::endBatch()
{
    if (m_iNumRectangles > 0)
    {
        OGLESManager->m_colorProgram->bind();
        
        if (m_isFill)
        {
            glDrawElements(GL_TRIANGLES, m_iNumRectangles * INDICES_PER_RECTANGLE, GL_UNSIGNED_SHORT, &OGLESManager->m_indices[0]);
        }
        else
        {
            glDrawArrays(GL_LINES, 0, VERTICES_PER_LINE * m_iNumRectangles * 4);
        }
        
        OGLESManager->m_colorProgram->unbind();
    }
}

void OpenGLESRectangleBatcher::renderRectangle(float x1, float y1, float x2, float y2, Color &c)
{
    if (m_isFill)
    {
        OGLESManager->addVertexCoordinate(x1, y1, 0, c.red, c.green, c.blue, c.alpha);
        OGLESManager->addVertexCoordinate(x1, y2, 0, c.red, c.green, c.blue, c.alpha);
        OGLESManager->addVertexCoordinate(x2, y2, 0, c.red, c.green, c.blue, c.alpha);
        OGLESManager->addVertexCoordinate(x2, y1, 0, c.red, c.green, c.blue, c.alpha);
    }
    else
    {
        OGLESManager->addVertexCoordinate(x1, y1, 0, c.red, c.green, c.blue, c.alpha);
        OGLESManager->addVertexCoordinate(x2, y1, 0, c.red, c.green, c.blue, c.alpha);
        
        OGLESManager->addVertexCoordinate(x2, y1, 0, c.red, c.green, c.blue, c.alpha);
        OGLESManager->addVertexCoordinate(x2, y2, 0, c.red, c.green, c.blue, c.alpha);
        
        OGLESManager->addVertexCoordinate(x2, y2, 0, c.red, c.green, c.blue, c.alpha);
        OGLESManager->addVertexCoordinate(x1, y2, 0, c.red, c.green, c.blue, c.alpha);
        
        OGLESManager->addVertexCoordinate(x1, y2, 0, c.red, c.green, c.blue, c.alpha);
        OGLESManager->addVertexCoordinate(x1, y1, 0, c.red, c.green, c.blue, c.alpha);
    }

    m_iNumRectangles++;
}