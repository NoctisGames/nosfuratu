//
//  Renderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Renderer.h"
#include "ResourceConstants.h"
#include "SpriteBatcher.h"
#include "TextureRegion.h"
#include "Assets.h"
#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Game.h"
#include "Ground.h"
#include "GamePlatform.h"
#include "Carrot.h"
#include "Jon.h"
#include "Tree.h"
#include <math.h>

Renderer::Renderer()
{
    m_camPos = std::unique_ptr<Vector2D>(new Vector2D(CAM_WIDTH / 5, 8.86776145203123f));
}

void Renderer::render(Game& game, float deltaTime)
{
    beginFrame();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    bindToOffscreenFramebuffer();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    /// Render Background
    
    updateCameraToFollowJon(game.getJon(), deltaTime);
    
    updateMatrix(0, CAM_WIDTH, m_camPos->getY(), m_camPos->getY() + CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, 19.99913569576492f, CAM_WIDTH, 14.00172860847015f, 0, Assets::getSky(game.getJon()));
    m_spriteBatcher->endBatch(*m_background);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, 15.00518582541045f, CAM_WIDTH, 11.8547968885047f, 0, Assets::getBackgroundTrees(game.getJon()));
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, 10.19792566983569f, CAM_WIDTH, 2.24027657735517f, 0, Assets::getMidgroundBushes(game.getJon()));
    m_spriteBatcher->endBatch(*m_background);
    
    /// Render World Foreground Trees/Rocks/etc
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + CAM_WIDTH, m_camPos->getY(), m_camPos->getY() + CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Tree>::iterator itr = game.getTrees().begin(); itr != game.getTrees().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getMidgroundTree(*itr));
    }
    m_spriteBatcher->endBatch(*m_trees);
    
    /// Render World
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Ground>::iterator itr = game.getGrounds().begin(); itr != game.getGrounds().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getForeground(*itr));
    }
    
    for (std::vector<GamePlatform>::iterator itr = game.getPlatforms().begin(); itr != game.getPlatforms().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getPlatform(*itr));
    }
    
    for (std::vector<Carrot>::iterator itr = game.getCarrots().begin(); itr != game.getCarrots().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getCarrot(*itr));
    }
    m_spriteBatcher->endBatch(*m_background);
    
    /// Render Jon
    
    m_spriteBatcher->beginBatch();
    
    renderPhysicalEntity(game.getJon(), Assets::getJon(game.getJon()));
    
    m_spriteBatcher->endBatch(*m_jon);
    
    /// Render everything to the screen
    
    bindToScreenFramebuffer();
    
    m_spriteBatcher->beginBatch();
    
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    
    m_spriteBatcher->endBatch(*m_framebuffer, getFramebufferToScreenGpuProgramWrapper());
    
    endFrame();
}

void Renderer::reset()
{
    m_camPos->set(CAM_WIDTH / 5, 8.86776145203123f);
}

#pragma mark private

void Renderer::renderPhysicalEntity(PhysicalEntity &go, TextureRegion tr)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), tr);
}

void Renderer::updateCameraToFollowJon(Jon& jon, float deltaTime)
{
    m_camPos->setX(jon.getPosition().getX() - CAM_WIDTH / 5);
    float jy = jon.getPosition().getY() - jon.getHeight() / 2;
    
    float regionBottomY;
    if (jon.isFalling())
    {
        if (jy < 8.8f)
        {
            regionBottomY = 0;
        }
        else if (jy >= 8.8f && jy < 18.0f)
        {
            regionBottomY = 8.8f;
        }
        else
        {
            regionBottomY = 18.0f;
        }
    }
    else
    {
        if (jy < 5.2f)
        {
            regionBottomY = 0;
        }
        else if (jy >= 5.2f && jy < 14.4f)
        {
            regionBottomY = 8.8f;
        }
        else
        {
            regionBottomY = 18.0f;
        }
    }
    
    float camVelocityY = regionBottomY > m_camPos->getY() ? (regionBottomY - m_camPos->getY()) * 2 : regionBottomY == m_camPos->getY() ? 0 : -(m_camPos->getY() - regionBottomY) * 2;
    m_camPos->add(0, camVelocityY * deltaTime);
    
    if (camVelocityY > 0)
    {
        m_camPos->setY(jy + 3.6f - CAM_HEIGHT);
        
        if (m_camPos->getY() > regionBottomY)
        {
            m_camPos->setY(regionBottomY);
        }
    }
    
    if (camVelocityY < 0)
    {
        if (m_camPos->getY() < regionBottomY)
        {
            m_camPos->setY(regionBottomY);
        }
    }
}