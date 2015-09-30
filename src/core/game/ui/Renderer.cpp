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
    for (std::vector<BackgroundSky>::iterator itr = game.getBackgroundSkies().begin(); itr != game.getBackgroundSkies().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getBackgroundSky(game.getJon(), game));
    }
    m_spriteBatcher->endBatch(*m_world_1_background);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<BackgroundTrees>::iterator itr = game.getBackgroundTrees().begin(); itr != game.getBackgroundTrees().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getBackgroundTrees(game.getJon(), game));
    }
    m_spriteBatcher->endBatch(*m_world_1_background);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<BackgroundCave>::iterator itr = game.getBackgroundCaves().begin(); itr != game.getBackgroundCaves().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getBackgroundCave(game.getJon(), game));
    }
    m_spriteBatcher->endBatch(*m_world_1_background);
    
    /// Render World midground Trees/Skeletons/etc
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + CAM_WIDTH, m_camPos->getY(), m_camPos->getY() + CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Tree>::iterator itr = game.getTrees().begin(); itr != game.getTrees().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getMidgroundTree(*itr));
    }
    
    for (std::vector<CaveSkeleton>::iterator itr = game.getCaveSkeletons().begin(); itr != game.getCaveSkeletons().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getMidgroundCaveSkeleton(*itr));
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground_more);
    
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
        renderPhysicalEntity(*itr, Assets::getLogVerticalTall());
    }
    
    for (std::vector<LogVerticalShort>::iterator itr = game.getLogVerticalShorts().begin(); itr != game.getLogVerticalShorts().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getLogVerticalShort());
    }
    
    for (std::vector<Thorns>::iterator itr = game.getThorns().begin(); itr != game.getThorns().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getThorns());
    }
    
    for (std::vector<Stump>::iterator itr = game.getStumps().begin(); itr != game.getStumps().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getStump());
    }
    
    for (std::vector<SideSpike>::iterator itr = game.getSideSpikes().begin(); itr != game.getSideSpikes().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getSideSpike());
    }
    
    for (std::vector<UpwardSpike>::iterator itr = game.getUpwardSpikes().begin(); itr != game.getUpwardSpikes().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getUpwardSpike((*itr)));
    }
    
    for (std::vector<JumpSpring>::iterator itr = game.getJumpSprings().begin(); itr != game.getJumpSprings().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getJumpSpring((*itr)));
    }
    
    for (std::vector<Rock>::iterator itr = game.getRocks().begin(); itr != game.getRocks().end(); itr++)
    {
        renderPhysicalEntityWithColor(*itr, Assets::getRock(*itr), (*itr).getColor());
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
    
    /// Render Jon Effects (e.g. Dust Clouds)
    
    m_spriteBatcher->beginBatch();
    for (std::vector<DustCloud>::iterator itr = game.getJon().getDustClouds().begin(); itr != game.getJon().getDustClouds().end(); itr++)
    {
        renderPhysicalEntityWithColor(*itr, Assets::getDustCloud(*itr), (*itr).getColor());
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground_more);
    
    /// Render Jon
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntityWithColor(game.getJon(), Assets::getJon(game.getJon()), game.getJon().getColor());
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

#pragma mark private

void Renderer::renderPhysicalEntity(PhysicalEntity &go, TextureRegion tr)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), tr);
}

void Renderer::renderPhysicalEntityWithColor(PhysicalEntity &go, TextureRegion tr, Color c)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), c, tr);
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