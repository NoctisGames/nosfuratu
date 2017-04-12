//
//  OpenGLSpriteBatcher.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLSpriteBatcher.h"

#include "macros.h"
#include "TextureRegion.h"
#include "NGRect.h"
#include "Vector2D.h"
#include "OpenGLManager.h"
#include "GpuProgramWrapper.h"
#include "GpuTextureWrapper.h"
#include "Color.h"

#include <math.h>

OpenGLSpriteBatcher::OpenGLSpriteBatcher()
{
    m_iNumSprites = 0;
}

void OpenGLSpriteBatcher::beginBatch()
{
    OGLManager->getTextureVertices().clear();
    m_iNumSprites = 0;
}

void OpenGLSpriteBatcher::endBatch(GpuTextureWrapper& textureWrapper, GpuProgramWrapper &gpuProgramWrapper)
{
    if (m_iNumSprites > 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureWrapper.texture);
        
        gpuProgramWrapper.bind();
        
        glDrawElements(GL_TRIANGLES, m_iNumSprites * INDICES_PER_RECTANGLE, GL_UNSIGNED_SHORT, &OGLManager->getIndices()[0]);
        
        gpuProgramWrapper.unbind();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void OpenGLSpriteBatcher::drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr)
{
    if (angle != 0)
    {
        float halfWidth = width / 2;
        float halfHeight = height / 2;
        
        float rad = DEGREES_TO_RADIANS(angle);
        float cos = cosf(rad);
        float sin = sinf(rad);
        
        float x1 = -halfWidth * cos - (-halfHeight) * sin;
        float y1 = -halfWidth * sin + (-halfHeight) * cos;
        
        float x2 = halfWidth * cos - (-halfHeight) * sin;
        float y2 = halfWidth * sin + (-halfHeight) * cos;
        
        float x3 = halfWidth * cos - halfHeight * sin;
        float y3 = halfWidth * sin + halfHeight * cos;
        
        float x4 = -halfWidth * cos - halfHeight * sin;
        float y4 = -halfWidth * sin + halfHeight * cos;
        
        x1 += x;
        y1 += y;
        
        x2 += x;
        y2 += y;
        
        x3 += x;
        y3 += y;
        
        x4 += x;
        y4 += y;
        
        OGLManager->addVertexCoordinate(x1, y1, 0, 1, 1, 1, 1, tr.u1, tr.v2);
        OGLManager->addVertexCoordinate(x4, y4, 0, 1, 1, 1, 1, tr.u1, tr.v1);
        OGLManager->addVertexCoordinate(x3, y3, 0, 1, 1, 1, 1, tr.u2, tr.v1);
        OGLManager->addVertexCoordinate(x2, y2, 0, 1, 1, 1, 1, tr.u2, tr.v2);
    }
    else
    {
        drawSprite(x, y, width, height, tr);
    }
    
    m_iNumSprites++;
}

void OpenGLSpriteBatcher::drawSprite(float x, float y, float width, float height, float angle, Color &c, TextureRegion tr)
{
    if (angle != 0)
    {
        float halfWidth = width / 2;
        float halfHeight = height / 2;
        
        float rad = DEGREES_TO_RADIANS(angle);
        float cos = cosf(rad);
        float sin = sinf(rad);
        
        float x1 = -halfWidth * cos - (-halfHeight) * sin;
        float y1 = -halfWidth * sin + (-halfHeight) * cos;
        
        float x2 = halfWidth * cos - (-halfHeight) * sin;
        float y2 = halfWidth * sin + (-halfHeight) * cos;
        
        float x3 = halfWidth * cos - halfHeight * sin;
        float y3 = halfWidth * sin + halfHeight * cos;
        
        float x4 = -halfWidth * cos - halfHeight * sin;
        float y4 = -halfWidth * sin + halfHeight * cos;
        
        x1 += x;
        y1 += y;
        
        x2 += x;
        y2 += y;
        
        x3 += x;
        y3 += y;
        
        x4 += x;
        y4 += y;
        
        OGLManager->addVertexCoordinate(x1, y1, 0, c.red, c.green, c.blue, c.alpha, tr.u1, tr.v2);
        OGLManager->addVertexCoordinate(x4, y4, 0, c.red, c.green, c.blue, c.alpha, tr.u1, tr.v1);
        OGLManager->addVertexCoordinate(x3, y3, 0, c.red, c.green, c.blue, c.alpha, tr.u2, tr.v1);
        OGLManager->addVertexCoordinate(x2, y2, 0, c.red, c.green, c.blue, c.alpha, tr.u2, tr.v2);
    }
    else
    {
        drawSprite(x, y, width, height, c, tr);
    }
    
    m_iNumSprites++;
}

#pragma private methods

void OpenGLSpriteBatcher::drawSprite(float x, float y, float width, float height, TextureRegion tr)
{
    GLfloat halfWidth = width / 2;
    GLfloat halfHeight = height / 2;
    GLfloat x1 = x - halfWidth;
    GLfloat y1 = y - halfHeight;
    GLfloat x2 = x + halfWidth;
    GLfloat y2 = y + halfHeight;
    
    OGLManager->addVertexCoordinate(x1, y1, 0, 1, 1, 1, 1, tr.u1, tr.v2);
    OGLManager->addVertexCoordinate(x1, y2, 0, 1, 1, 1, 1, tr.u1, tr.v1);
    OGLManager->addVertexCoordinate(x2, y2, 0, 1, 1, 1, 1, tr.u2, tr.v1);
    OGLManager->addVertexCoordinate(x2, y1, 0, 1, 1, 1, 1, tr.u2, tr.v2);
}

void OpenGLSpriteBatcher::drawSprite(float x, float y, float width, float height, Color &c, TextureRegion tr)
{
    GLfloat halfWidth = width / 2;
    GLfloat halfHeight = height / 2;
    GLfloat x1 = x - halfWidth;
    GLfloat y1 = y - halfHeight;
    GLfloat x2 = x + halfWidth;
    GLfloat y2 = y + halfHeight;
    
    OGLManager->addVertexCoordinate(x1, y1, 0, c.red, c.green, c.blue, c.alpha, tr.u1, tr.v2);
    OGLManager->addVertexCoordinate(x1, y2, 0, c.red, c.green, c.blue, c.alpha, tr.u1, tr.v1);
    OGLManager->addVertexCoordinate(x2, y2, 0, c.red, c.green, c.blue, c.alpha, tr.u2, tr.v1);
    OGLManager->addVertexCoordinate(x2, y1, 0, c.red, c.green, c.blue, c.alpha, tr.u2, tr.v2);
}
