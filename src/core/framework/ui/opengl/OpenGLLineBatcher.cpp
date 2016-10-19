//
//  OpenGLLineBatcher.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLLineBatcher.h"
#include "macros.h"
#include "Line.h"
#include "Vector2D.h"
#include "OpenGLManager.h"
#include "GpuProgramWrapper.h"

OpenGLLineBatcher::OpenGLLineBatcher() : LineBatcher()
{
    m_iNumLines = 0;
}

void OpenGLLineBatcher::beginBatch()
{
    OGLESManager->m_colorVertices.clear();
    m_iNumLines = 0;
}

void OpenGLLineBatcher::endBatch()
{
    endBatch(*OGLESManager->m_colorProgram);
}

void OpenGLLineBatcher::endBatch(GpuProgramWrapper &gpuProgramWrapper)
{
    if (m_iNumLines > 0)
    {
        gpuProgramWrapper.bind();
        
        glDrawArrays(GL_LINES, 0, VERTICES_PER_LINE * m_iNumLines);
        
        gpuProgramWrapper.unbind();
    }
}

void OpenGLLineBatcher::renderLine(float originX, float originY, float endX, float endY, Color &c)
{
    OGLESManager->addVertexCoordinate(originX, originY, 0, c.red, c.green, c.blue, c.alpha);
    OGLESManager->addVertexCoordinate(endX, endY, 0, c.red, c.green, c.blue, c.alpha);
    
    m_iNumLines++;
}
