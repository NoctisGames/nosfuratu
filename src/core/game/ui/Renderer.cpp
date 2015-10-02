//
//  Renderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Renderer.h"
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
#include "DustCloud.h"
#include <math.h>

#define aboveGroundRegionBottomY 8.750433275563259f

Renderer::Renderer() : m_areTexturesLoaded(false)
{
    m_camPos = std::unique_ptr<Vector2D>(new Vector2D(0, aboveGroundRegionBottomY));
}

void Renderer::init()
{
    m_camPos->set(0, aboveGroundRegionBottomY);
    
    if (!m_areTexturesLoaded)
    {
        m_jon_ability = std::unique_ptr<TextureWrapper>(loadTexture("jon_ability"));
        m_jon = std::unique_ptr<TextureWrapper>(loadTexture("jon"));
        m_vampire = std::unique_ptr<TextureWrapper>(loadTexture("vampire"));
        m_world_1_background = std::unique_ptr<TextureWrapper>(loadTexture("world_1_background"));
        m_world_1_foreground_more = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_more"));
        m_world_1_foreground = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground"));
        
        m_areTexturesLoaded = true;
    }
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
    
    float camSpeed = regionBottomY - m_camPos->getY();
    float camVelocityY = regionBottomY > m_camPos->getY() ? camSpeed : regionBottomY == m_camPos->getY() ? 0 : camSpeed * 4;
    m_camPos->add(0, camVelocityY * deltaTime);
    
    if (camVelocityY > 0)
    {
        float newCamPos = jy + jonHeightPlusPadding - CAM_HEIGHT;
        m_camPos->setY(newCamPos);
        
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

void Renderer::render(Game& game)
{
    beginFrame();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    bindToOffscreenFramebuffer();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    /// Render Background
    
    updateMatrix(0, CAM_WIDTH, m_camPos->getY(), m_camPos->getY() + CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundSkies());
    m_spriteBatcher->endBatch(*m_world_1_background);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundTrees());
    m_spriteBatcher->endBatch(*m_world_1_background);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundCaves());
    m_spriteBatcher->endBatch(*m_world_1_background);
    
    /// Render World midground Trees/Skeletons/etc
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + CAM_WIDTH, m_camPos->getY(), m_camPos->getY() + CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getTrees());
    renderPhysicalEntities(game.getCaveSkeletons());
    m_spriteBatcher->endBatch(*m_world_1_foreground_more);
    
    /// Render World
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Ground>::iterator i = game.getGrounds().begin(); i != game.getGrounds().end(); i++)
    {
        if (isGroundForegroundMore(*i))
        {
            continue;
        }
        
        renderPhysicalEntity(*i, Assets::get(*i));
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Ground>::iterator i = game.getGrounds().begin(); i != game.getGrounds().end(); i++)
    {
        if (isGroundForegroundMore(*i))
        {
            renderPhysicalEntity(*i, Assets::get(*i));
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground_more);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getLogVerticalTalls());
    renderPhysicalEntities(game.getLogVerticalShorts());
    renderPhysicalEntities(game.getThorns());
    renderPhysicalEntities(game.getStumps());
    renderPhysicalEntities(game.getSideSpikes());
    renderPhysicalEntities(game.getUpwardSpikes());
    renderPhysicalEntities(game.getJumpSprings());
    renderPhysicalEntitiesWithColor(game.getRocks());
    renderPhysicalEntities(game.getPlatforms());
    renderPhysicalEntities(game.getEndSigns());
    renderPhysicalEntities(game.getCarrots());
    renderPhysicalEntities(game.getGoldenCarrots());
    m_spriteBatcher->endBatch(*m_world_1_foreground);
    
    /// Render Jon Effects (e.g. Dust Clouds)
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Jon>::iterator i = game.getJons().begin(); i != game.getJons().end(); i++)
    {
        renderPhysicalEntitiesWithColor((*i).getDustClouds());
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground_more);
    
    /// Render Jon
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntitiesWithColor(game.getJons());
    m_spriteBatcher->endBatch(game.getJon().getAbilityState() == ABILITY_NONE ? *m_jon : *m_jon_ability);
    
    /// Render everything to the screen
    
    bindToScreenFramebuffer();
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(*m_framebuffer, getFramebufferToScreenGpuProgramWrapper());
    
    endFrame();
}

void Renderer::cleanUp()
{
    destroyTexture(*m_jon_ability);
    destroyTexture(*m_jon);
    destroyTexture(*m_vampire);
    destroyTexture(*m_world_1_background);
    destroyTexture(*m_world_1_foreground_more);
    destroyTexture(*m_world_1_foreground);
}

Vector2D& Renderer::getCameraPosition()
{
    return *m_camPos;
}

#pragma mark private

void Renderer::renderPhysicalEntity(PhysicalEntity &go, TextureRegion tr)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), tr);
}

void Renderer::renderPhysicalEntityWithColor(PhysicalEntity &go, TextureRegion tr, Color c)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), c, tr);
}

bool Renderer::isGroundForegroundMore(Ground &g)
{
    return g.getGroundType() == GROUND_CAVE_RAISED_END_LEFT
    || g.getGroundType() == GROUND_CAVE_RAISED_MEDIUM
    || g.getGroundType() == GROUND_CAVE_RAISED_SMALL
    || g.getGroundType() == GROUND_CAVE_RAISED_END_RIGHT
    || g.getGroundType() == GROUND_CAVE_RAISED_LARGE;
}