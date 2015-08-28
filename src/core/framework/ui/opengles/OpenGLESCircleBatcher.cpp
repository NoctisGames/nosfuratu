//
//  OpenGLESCircleBatcher.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#define DEGREE_SPACING 6

#include "OpenGLESCircleBatcher.h"
#include "macros.h"
#include "Circle.h"
#include "Vector2D.h"
#include "OpenGLESManager.h"
#include "DummyGpuProgramWrapper.h"

OpenGLESCircleBatcher::OpenGLESCircleBatcher() : CircleBatcher()
{
    // Empty
}

void OpenGLESCircleBatcher::renderCircle(Circle &circle, Color &color)
{
    renderCircle(circle, color, *OGLESManager->m_colorProgram);
}

void OpenGLESCircleBatcher::renderPartialCircle(Circle &circle, int arcDegrees, Color &color)
{
    renderPartialCircle(circle, arcDegrees, color, *OGLESManager->m_colorProgram);
}

void OpenGLESCircleBatcher::renderCircle(Circle &circle, Color &color, GpuProgramWrapper &gpuProgramWrapper)
{
    OGLESManager->m_colorVertices.clear();
    
    m_iNumPoints = 0;
    
    for (int i = 0; i < 360; i += DEGREE_SPACING)
    {
        float rad = DEGREES_TO_RADIANS(i);
        float cos = cosf(rad);
        float sin = sinf(rad);
        
        OGLESManager->addVertexCoordinate(cos * circle.m_fRadius + circle.getCenter().getX(), sin * circle.m_fRadius + circle.getCenter().getY(), 0, color.red, color.green, color.blue, color.alpha);
        
        m_iNumPoints++;
    }
    
    endBatch(gpuProgramWrapper);
}

void OpenGLESCircleBatcher::renderPartialCircle(Circle &circle, int arcDegrees, Color &color, GpuProgramWrapper &gpuProgramWrapper)
{
    OGLESManager->m_colorVertices.clear();
    
    OGLESManager->addVertexCoordinate(circle.getCenter().getX(), circle.getCenter().getY(), 0, color.red, color.green, color.blue, color.alpha);
    
    m_iNumPoints = 1;
    
    for (int i = 90 - arcDegrees; i > -270; i -= DEGREE_SPACING)
    {
        float rad = DEGREES_TO_RADIANS(i);
        float cos = cosf(rad);
        float sin = sinf(rad);
        
        OGLESManager->addVertexCoordinate(cos * circle.m_fRadius + circle.getCenter().getX(), sin * circle.m_fRadius + circle.getCenter().getY(), 0, color.red, color.green, color.blue, color.alpha);
        
        m_iNumPoints++;
    }
    
    float rad = DEGREES_TO_RADIANS(-270);
    float cos = cosf(rad);
    float sin = sinf(rad);
    
    OGLESManager->addVertexCoordinate(cos * circle.m_fRadius + circle.getCenter().getX(), sin * circle.m_fRadius + circle.getCenter().getY(), 0, color.red, color.green, color.blue, color.alpha);
    
    m_iNumPoints++;
    
    endBatch(gpuProgramWrapper);
}

void OpenGLESCircleBatcher::endBatch(GpuProgramWrapper &gpuProgramWrapper)
{
    gpuProgramWrapper.bind();
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_iNumPoints);
    
    gpuProgramWrapper.unbind();
}