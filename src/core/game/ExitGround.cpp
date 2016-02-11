//
//  ExitGround.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#include "ExitGround.h"
#include "EntityUtils.h"

ExitGround* ExitGround::create(int gridX, int gridY, int type)
{
    switch ((ExitGroundType)type)
    {
        case ExitGroundType_GrassWithCaveSmallExitMid:
            return new GrassWithCaveSmallExitMid(gridX);
        case ExitGroundType_GrassWithCaveSmallExitEnd:
            return new GrassWithCaveSmallExitEnd(gridX);
        case ExitGroundType_CaveSmallExit:
            return new CaveSmallExit(gridX);
            
        case ExitGroundType_CaveDeepSmallWaterfall:
            return new CaveDeepSmallWaterfall(gridX);
    }
    
    assert(false);
}

ExitGround::ExitGround(int gridX, int gridY, int gridWidth, int gridHeight, float boundsHeightFactor, bool hasCover, ExitGroundType type, GroundSoundType groundSoundType) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_fBoundsHeightFactor(boundsHeightFactor), m_type(type), m_groundSoundType(groundSoundType), m_exitCover(nullptr)
{
    updateBounds();
    
    if (hasCover)
    {
        ExitGroundCoverType t = m_type == ExitGroundType_GrassWithCaveSmallExitMid || m_type == ExitGroundType_GrassWithCaveSmallExitEnd ? ExitGroundCoverType_Grass : ExitGroundCoverType_Cave;
        m_exitCover = new ExitGroundCover(m_position->getX(), m_position->getY(), m_fWidth, m_fHeight, t);
    }
}

void ExitGround::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_exitCover)
    {
        m_exitCover->update(deltaTime);
        m_exitCover->getPosition().set(getPosition());
        m_exitCover->updateBounds();
        
        if (m_exitCover->isRequestingDeletion())
        {
            delete m_exitCover;
            m_exitCover = nullptr;
        }
    }
}

void ExitGround::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * m_fBoundsHeightFactor);
}

bool ExitGround::isJonLanding(Jon& jon, float deltaTime)
{
    float jonVelocityY = jon.getVelocity().getY();
    
    if (jonVelocityY <= 0)
    {
        if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
        {
            float jonBottomY = jon.getBounds().getBottom();
            float jonLeftX = jon.getBounds().getLeft();
            float jonRightX = jon.getBounds().getRight();
            float jonYDelta = fabsf(jonVelocityY * deltaTime);
            
            float itemTop = getBounds().getTop();
            float padding = itemTop * .01f;
            padding += jonYDelta;
            float itemTopReq = itemTop - padding;
            float pitEntranceLeft = getBounds().getLeft() + getBounds().getWidth() * 0.2421875f;
            float pitEntranceRight = getBounds().getLeft() + getBounds().getWidth() * 0.71875f;
            
            bool isLanding = jonBottomY >= itemTopReq;
            if (!hasCover())
            {
                isLanding = jonRightX < pitEntranceLeft || jonLeftX > pitEntranceRight;
            }
            
            if (isLanding)
            {
                jon.getPosition().setY(itemTop + jon.getBounds().getHeight() / 2 * .99f);
                jon.updateBounds();
                jon.setGroundSoundType(getGroundSoundType());
                
                return true;
            }
        }
    }
    
    return false;
}

bool ExitGround::isJonBlockedOnRight(Jon &jon, float deltaTime)
{
    if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
    {
        float entityVelocityX = jon.getVelocity().getX();
        float entityBottom = jon.getBounds().getLowerLeft().getY();
        float entityRight = jon.getBounds().getRight();
        float entityXDelta = fabsf(entityVelocityX * deltaTime);
        
        float itemTop = getBounds().getTop();
        float itemTopReq = itemTop * 0.99f;
        
        float itemLeft = getBounds().getLowerLeft().getX();
        float padding = itemLeft * .01f;
        padding += entityXDelta;
        float itemLeftReq = itemLeft + padding;
        
        if (entityRight <= itemLeftReq && entityBottom < itemTopReq)
        {
            jon.getPosition().setX(itemLeft - jon.getBounds().getWidth() / 2 * 1.01f);
            jon.updateBounds();
            
            return true;
        }
    }
    
    return false;
}

bool ExitGround::isJonBlockedAbove(Jon &jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
    {
        float entityLeft = jon.getBounds().getLeft();
        float entityRight = jon.getBounds().getRight();
        float exitLeft = getBounds().getLeft() + getBounds().getWidth() * 0.2421875f;
        float exitRight = getBounds().getLeft() + getBounds().getWidth() * 0.71875f;
        
        if (entityVelocityY > 13.0f && entityRight < exitRight && entityLeft > exitLeft)
        {
            if (hasCover())
            {
                m_exitCover->triggerHit();
            }
            
            return false;
        }
        else
        {
            jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
            jon.updateBounds();
            
            return true;
        }
    }
    
    return false;
}

bool ExitGround::canObjectBePlacedOn()
{
    return true;
}

bool ExitGround::hasCover()
{
    return m_exitCover ? true : false;
}

ExitGroundCover& ExitGround::getExitCover()
{
    return *m_exitCover;
}

ExitGroundType ExitGround::getType()
{
    return m_type;
}

GroundSoundType ExitGround::getGroundSoundType()
{
    return m_groundSoundType;
}

bool CaveDeepSmallWaterfall::isJonLanding(Jon& jon, float deltaTime)
{
    float jonVelocityY = jon.getVelocity().getY();
    
    if (jonVelocityY <= 0)
    {
        if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
        {
            float jonBottomY = jon.getBounds().getBottom();
            float jonLeftX = jon.getBounds().getLeft();
            float jonRightX = jon.getBounds().getRight();
            float jonYDelta = fabsf(jonVelocityY * deltaTime);
            
            float itemTop = getBounds().getTop();
            float padding = itemTop * .01f;
            padding += jonYDelta;
            float itemTopReq = itemTop - padding;
            float pitEntranceLeft = getBounds().getLeft() + getBounds().getWidth() * 0.328125f;
            float pitEntranceRight = getBounds().getLeft() + getBounds().getWidth() * 0.75f;
            
            if (jonBottomY >= itemTopReq && (jonRightX < pitEntranceLeft || jonLeftX > pitEntranceRight))
            {
                jon.getPosition().setY(itemTop + jon.getBounds().getHeight() / 2 * .99f);
                jon.updateBounds();
                jon.setGroundSoundType(getGroundSoundType());
                
                return true;
            }
            else
            {
                jon.getAcceleration().setY(jon.getGravity() * 2);
            }
        }
    }
    
    return false;
}

bool CaveDeepSmallWaterfall::canObjectBePlacedOn()
{
    return false;
}
