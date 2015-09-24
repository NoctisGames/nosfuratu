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
#include "GroundPlatform.h"
#include "Carrot.h"
#include "GoldenCarrot.h"
#include "Jon.h"
#include "Tree.h"
#include <math.h>

#define aboveGroundRegionBottomY 8.750433275563259f

Renderer::Renderer()
{
    m_camPos = std::unique_ptr<Vector2D>(new Vector2D(0, aboveGroundRegionBottomY));
}

void Renderer::render(Game& game, float deltaTime)
{
    beginFrame();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    bindToOffscreenFramebuffer();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    /// Render Background
    
    updateCameraToFollowJon(game.getJon(), game, deltaTime);
    
    updateMatrix(0, CAM_WIDTH, m_camPos->getY(), m_camPos->getY() + CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(game.getBackgroundSky(), Assets::getBackgroundSky(game.getJon(), game));
    m_spriteBatcher->endBatch(*m_world_1_background);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(game.getBackgroundTrees(), Assets::getBackgroundTrees(game.getJon(), game));
    m_spriteBatcher->endBatch(*m_world_1_background);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(game.getBackgroundCave(), Assets::getBackgroundCave(game.getJon(), game));
    m_spriteBatcher->endBatch(*m_world_1_background);
    
    /// Render World midground Trees/Skeletons/etc
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + CAM_WIDTH, m_camPos->getY(), m_camPos->getY() + CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Tree>::iterator itr = game.getTrees().begin(); itr != game.getTrees().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getMidgroundTree(*itr));
    }
    m_spriteBatcher->endBatch(*m_world_1_midground);
    
    /// Render World
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Ground>::iterator itr = game.getGrounds().begin(); itr != game.getGrounds().end(); itr++)
    {
        if (itr->getGroundType() == GROUND_CAVE_RAISED_END_LEFT
            || itr->getGroundType() == GROUND_CAVE_RAISED_MEDIUM
            || itr->getGroundType() == GROUND_CAVE_RAISED_SMALL
            || itr->getGroundType() == GROUND_CAVE_RAISED_END_RIGHT
            || itr->getGroundType() == GROUND_CAVE_RAISED_LARGE)
        {
            continue;
        }
        
        renderPhysicalEntity(*itr, Assets::getGround(*itr));
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Ground>::iterator itr = game.getGrounds().begin(); itr != game.getGrounds().end(); itr++)
    {
        if (itr->getGroundType() == GROUND_CAVE_RAISED_END_LEFT
            || itr->getGroundType() == GROUND_CAVE_RAISED_MEDIUM
            || itr->getGroundType() == GROUND_CAVE_RAISED_SMALL
            || itr->getGroundType() == GROUND_CAVE_RAISED_END_RIGHT
            || itr->getGroundType() == GROUND_CAVE_RAISED_LARGE)
        {
            renderPhysicalEntity(*itr, Assets::getGround(*itr));
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground_more);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<LogVerticalTall>::iterator itr = game.getLogVerticalTalls().begin(); itr != game.getLogVerticalTalls().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getLogVerticalTall(*itr));
    }
    
    for (std::vector<GroundPlatform>::iterator itr = game.getPlatforms().begin(); itr != game.getPlatforms().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getGroundPlatform(*itr));
    }
    
    for (std::vector<EndSign>::iterator itr = game.getEndSigns().begin(); itr != game.getEndSigns().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getEndSign());
    }
    
    for (std::vector<Carrot>::iterator itr = game.getCarrots().begin(); itr != game.getCarrots().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getCarrot(*itr));
    }
    
    for (std::vector<GoldenCarrot>::iterator itr = game.getGoldenCarrots().begin(); itr != game.getGoldenCarrots().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getGoldenCarrot(*itr));
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground);
    
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

void Renderer::reset(Game& game)
{
    m_camPos->set(0, aboveGroundRegionBottomY);
}

#pragma mark private

void Renderer::renderPhysicalEntity(PhysicalEntity &go, TextureRegion tr)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), tr);
}

void Renderer::updateCameraToFollowJon(Jon& jon, Game& game, float deltaTime)
{
    m_camPos->setX(jon.getPosition().getX() - CAM_WIDTH / 5);
    float jy = jon.getPosition().getY() - jon.getHeight() / 2;
    float jonHeightPlusPadding = jon.getHeight() * 1.5f;
    
    float regionBottomY;
    if (jon.isFalling())
    {
        if (jy < aboveGroundRegionBottomY)
        {
            regionBottomY = 0;
        }
        else if (jy >= aboveGroundRegionBottomY && jy < 18.0f)
        {
            regionBottomY = aboveGroundRegionBottomY;
        }
        else
        {
            regionBottomY = 18.0f;
        }
    }
    else
    {
        if (jy < (aboveGroundRegionBottomY - jonHeightPlusPadding))
        {
            regionBottomY = 0;
        }
        else if (jy >= (aboveGroundRegionBottomY - jonHeightPlusPadding) && jy < (18.0f - jonHeightPlusPadding))
        {
            regionBottomY = aboveGroundRegionBottomY;
        }
        else
        {
            regionBottomY = 18.0f;
        }
    }
    
    float camVelocityY = regionBottomY > m_camPos->getY() ? 1337 : regionBottomY == m_camPos->getY() ? 0 : -(m_camPos->getY() - regionBottomY) * 4;
    m_camPos->add(0, camVelocityY * deltaTime);
    
    if (camVelocityY > 0)
    {
        m_camPos->setY(jy + jonHeightPlusPadding - CAM_HEIGHT);
        
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
    
    if (m_camPos->getY() < 0)
    {
        m_camPos->setY(0);
    }
    
    if (m_camPos->getY() > GAME_HEIGHT - CAM_HEIGHT)
    {
        m_camPos->setY(GAME_HEIGHT - CAM_HEIGHT);
    }
    
    float farRight = game.getFarRight();
    float farCamPos = farRight - CAM_WIDTH;
    if (m_camPos->getX() > farCamPos)
    {
        m_camPos->setX(farCamPos);
    }
}