//
//  Renderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Renderer.h"
#include "SpriteBatcher.h"
#include "RectangleBatcher.h"
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
#include "BackButton.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "TrashCan.h"
#include "GpuProgramWrapper.h"
#include "SinWaveTextureGpuProgramWrapper.h"

#include <math.h>

#define aboveGroundRegionBottomY 8.750433275563259f

Renderer::Renderer() : m_areLevelEditorTexturesLoaded(false), m_areWorld1TexturesLoaded(false), m_areJonTexturesLoaded(false)
{
    m_camPos = std::unique_ptr<Vector2D>(new Vector2D(0, aboveGroundRegionBottomY));
    
    zoomIn();
}

void Renderer::init(RendererType type)
{
    switch (type)
    {
        case RENDERER_TYPE_LEVEL_EDITOR:
            zoomOut();
            
            if (!m_areWorld1TexturesLoaded)
            {
                m_world_1_background = std::unique_ptr<TextureWrapper>(loadTexture("world_1_background"));
                m_world_1_foreground_more = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_more"));
                m_world_1_foreground = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground"));
                
                m_areWorld1TexturesLoaded = true;
            }
            
            if (!m_areLevelEditorTexturesLoaded)
            {
                m_level_editor = std::unique_ptr<TextureWrapper>(loadTexture("level_editor"));
                
                m_areLevelEditorTexturesLoaded = true;
            }
            break;
        case RENDERER_TYPE_WORLD_1:
            zoomIn();
            
            if (!m_areWorld1TexturesLoaded)
            {
                m_world_1_background = std::unique_ptr<TextureWrapper>(loadTexture("world_1_background"));
                m_world_1_foreground_more = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_more"));
                m_world_1_foreground = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground"));
                
                m_areWorld1TexturesLoaded = true;
            }
            
            if (!m_areJonTexturesLoaded)
            {
                m_jon_ability = std::unique_ptr<TextureWrapper>(loadTexture("jon_ability"));
                m_jon = std::unique_ptr<TextureWrapper>(loadTexture("jon"));
                m_vampire = std::unique_ptr<TextureWrapper>(loadTexture("vampire"));
                
                m_areJonTexturesLoaded = true;
            }
            break;
        default:
            break;
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

void Renderer::moveCamera(float x)
{
    m_camPos->add(x, 0);
}

void Renderer::zoomOut()
{
    m_camPos->set(0, 0);
    m_fCamWidth = ZOOMED_OUT_CAM_WIDTH;
    m_fCamHeight = GAME_HEIGHT;
}

void Renderer::zoomIn()
{
    m_camPos->set(0, aboveGroundRegionBottomY);
    m_fCamWidth = CAM_WIDTH;
    m_fCamHeight = CAM_HEIGHT;
}

void Renderer::renderWorld(Game& game)
{
    beginFrame();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    bindToOffscreenFramebuffer();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    /// Render Background
    
    updateMatrix(0, m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
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
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getTrees());
    renderPhysicalEntities(game.getCaveSkeletons());
    m_spriteBatcher->endBatch(*m_world_1_foreground_more);
    
    /// Render World
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<Ground>>::iterator i = game.getGrounds().begin(); i != game.getGrounds().end(); i++)
    {
        if ((*i)->isForegroundMore())
        {
            continue;
        }
        
        renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<Ground>>::iterator i = game.getGrounds().begin(); i != game.getGrounds().end(); i++)
    {
        if ((*i)->isForegroundMore())
        {
            renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
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
    for (std::vector<std::unique_ptr<Jon>>::iterator i = game.getJons().begin(); i != game.getJons().end(); i++)
    {
        renderPhysicalEntitiesWithColor((*i)->getDustClouds());
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground_more);
}

void Renderer::renderJon(Game& game)
{
    /// Render Jon
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntitiesWithColor(game.getJons());
    if (game.getJon().getState() == JON_DYING_FADING)
    {
        m_sinWaveTextureProgram->setOffset(game.getStateTime());
        m_spriteBatcher->endBatch(game.getJon().getAbilityState() == ABILITY_NONE ? *m_jon : *m_jon_ability, *m_sinWaveTextureProgram);
    }
    else
    {
        m_spriteBatcher->endBatch(game.getJon().getAbilityState() == ABILITY_NONE ? *m_jon : *m_jon_ability);
    }
}

void Renderer::renderBounds(Game& game)
{
    /// Render World midground Trees/Skeletons/etc
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
    m_boundsRectangleBatcher->beginBatch();
    renderBoundsForPhysicalEntities(game.getTrees());
    renderBoundsForPhysicalEntities(game.getCaveSkeletons());
    m_boundsRectangleBatcher->endBatch();
    
    /// Render World
    
    m_boundsRectangleBatcher->beginBatch();
    renderBoundsForPhysicalEntities(game.getGrounds());
    m_boundsRectangleBatcher->endBatch();
    
    m_boundsRectangleBatcher->beginBatch();
    renderBoundsForPhysicalEntities(game.getLogVerticalTalls());
    renderBoundsForPhysicalEntities(game.getLogVerticalShorts());
    renderBoundsForPhysicalEntities(game.getThorns());
    renderBoundsForPhysicalEntities(game.getStumps());
    renderBoundsForPhysicalEntities(game.getSideSpikes());
    renderBoundsForPhysicalEntities(game.getUpwardSpikes());
    renderBoundsForPhysicalEntities(game.getJumpSprings());
    renderBoundsForPhysicalEntities(game.getRocks());
    renderBoundsForPhysicalEntities(game.getPlatforms());
    renderBoundsForPhysicalEntities(game.getEndSigns());
    renderBoundsForPhysicalEntities(game.getCarrots());
    renderBoundsForPhysicalEntities(game.getGoldenCarrots());
    m_boundsRectangleBatcher->endBatch();
}

void Renderer::renderEntityHighlighted(PhysicalEntity& entity, Color& c)
{
    updateMatrix(m_camPos->getX(), m_camPos->getX() + m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
    m_highlightRectangleBatcher->beginBatch();
    renderHighlightForPhysicalEntity(entity, c);
    m_highlightRectangleBatcher->endBatch();
}

void Renderer::renderBackButton(BackButton &backButton)
{
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    /// Render Back Button
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(backButton, Assets::get(backButton));
    m_spriteBatcher->endBatch(*m_world_1_foreground_more);
}

void Renderer::renderLevelEditor(LevelEditorActionsPanel& levelEditorActionsPanel, LevelEditorEntitiesPanel& levelEditorEntitiesPanel, TrashCan& trashCan)
{
    static Rectangle originMarker = Rectangle(0, 0, 0.1f, GAME_HEIGHT);
    static Color originMarkerColor = Color(0, 0, 0, 0.7f);
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
    m_highlightRectangleBatcher->beginBatch();
    m_highlightRectangleBatcher->renderRectangle(originMarker, originMarkerColor);
    m_highlightRectangleBatcher->endBatch();
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    /// Render Level Editor
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(levelEditorActionsPanel, Assets::get(levelEditorActionsPanel));
    renderPhysicalEntity(levelEditorEntitiesPanel, Assets::get(levelEditorEntitiesPanel));
    m_spriteBatcher->endBatch(*m_level_editor);
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(trashCan, Assets::get(trashCan));
    m_spriteBatcher->endBatch(*m_level_editor);
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    if (levelEditorEntitiesPanel.isOpen())
    {
        updateMatrix(0, CAM_WIDTH, levelEditorEntitiesPanel.getEntitiesCameraPos(), levelEditorEntitiesPanel.getEntitiesCameraPos() + CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(levelEditorEntitiesPanel.getTrees());
        renderPhysicalEntities(levelEditorEntitiesPanel.getCaveSkeletons());
        m_spriteBatcher->endBatch(*m_world_1_foreground_more);
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<Ground>>::iterator i = levelEditorEntitiesPanel.getGrounds().begin(); i != levelEditorEntitiesPanel.getGrounds().end(); i++)
        {
            if ((*i)->isForegroundMore())
            {
                continue;
            }
            
            renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
        }
        m_spriteBatcher->endBatch(*m_world_1_foreground);
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<Ground>>::iterator i = levelEditorEntitiesPanel.getGrounds().begin(); i != levelEditorEntitiesPanel.getGrounds().end(); i++)
        {
            if ((*i)->isForegroundMore())
            {
                renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
            }
        }
        m_spriteBatcher->endBatch(*m_world_1_foreground_more);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(levelEditorEntitiesPanel.getLogVerticalTalls());
        renderPhysicalEntities(levelEditorEntitiesPanel.getLogVerticalShorts());
        renderPhysicalEntities(levelEditorEntitiesPanel.getThorns());
        renderPhysicalEntities(levelEditorEntitiesPanel.getStumps());
        renderPhysicalEntities(levelEditorEntitiesPanel.getSideSpikes());
        renderPhysicalEntities(levelEditorEntitiesPanel.getUpwardSpikes());
        renderPhysicalEntities(levelEditorEntitiesPanel.getJumpSprings());
        renderPhysicalEntitiesWithColor(levelEditorEntitiesPanel.getRocks());
        renderPhysicalEntities(levelEditorEntitiesPanel.getPlatforms());
        renderPhysicalEntities(levelEditorEntitiesPanel.getEndSigns());
        renderPhysicalEntities(levelEditorEntitiesPanel.getCarrots());
        renderPhysicalEntities(levelEditorEntitiesPanel.getGoldenCarrots());
        m_spriteBatcher->endBatch(*m_world_1_foreground);
    }
}

void Renderer::renderToScreen(Game& game)
{
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

void Renderer::renderPhysicalEntity(PhysicalEntity &pe, TextureRegion& tr)
{
    m_spriteBatcher->drawSprite(pe.getPosition().getX(), pe.getPosition().getY(), pe.getWidth(), pe.getHeight(), pe.getAngle(), tr);
}

void Renderer::renderPhysicalEntityWithColor(PhysicalEntity &pe, TextureRegion& tr, Color c)
{
    m_spriteBatcher->drawSprite(pe.getPosition().getX(), pe.getPosition().getY(), pe.getWidth(), pe.getHeight(), pe.getAngle(), c, tr);
}

void Renderer::renderBoundsForPhysicalEntity(PhysicalEntity &pe)
{
    static Color red = Color(1, 0, 0, 1);
    
    m_boundsRectangleBatcher->renderRectangle(pe.getBounds(), red);
}

void Renderer::renderHighlightForPhysicalEntity(PhysicalEntity &pe, Color &c)
{
    m_highlightRectangleBatcher->renderRectangle(pe.getBounds(), c);
}