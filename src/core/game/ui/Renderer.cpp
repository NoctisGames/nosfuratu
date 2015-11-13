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
#include "Hole.h"
#include "HoleCover.h"
#include "CaveExit.h"
#include "CaveExitCover.h"
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
#include "LevelSelectorPanel.h"
#include "GpuProgramWrapper.h"
#include "SinWaveTextureGpuProgramWrapper.h"

#include <math.h>
#include <sstream>
#include <iomanip>

#define aboveGroundRegionBottomY 8.750433275563259f

Renderer::Renderer() : m_fCamWidth(CAM_WIDTH), m_fCamHeight(CAM_HEIGHT), m_fStateTime(0), m_areTitleTexturesLoaded(false), m_areJonTexturesLoaded(false), m_areVampireAndAbilityTexturesLoaded(false), m_areWorld1TexturesLoaded(false), m_areLevelEditorTexturesLoaded(false)
{
    m_font = std::unique_ptr<Font>(new Font(0, 0, 16, 64, 73, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048));
    m_camPos = std::unique_ptr<Vector2D>(new Vector2D(0, aboveGroundRegionBottomY));
    m_camPosAcceleration = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
    m_camPosVelocity = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
    m_camAcceleration = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
    m_camVelocity = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
}

void Renderer::init(RendererType type)
{
    m_rendererType = type;
    
    switch (type)
    {
        case RENDERER_TYPE_TITLE:
            if (m_areJonTexturesLoaded)
            {
                destroyTexture(*m_jon);
                
                m_areJonTexturesLoaded = false;
            }
            
            if (m_areVampireAndAbilityTexturesLoaded)
            {
                destroyTexture(*m_jon_ability);
                destroyTexture(*m_vampire);
                
                m_areVampireAndAbilityTexturesLoaded = false;
            }
            
            if (m_areWorld1TexturesLoaded)
            {
                destroyTexture(*m_world_1_background);
                destroyTexture(*m_world_1_foreground_3);
                destroyTexture(*m_world_1_foreground_2);
                destroyTexture(*m_world_1_foreground_1);
                
                m_areWorld1TexturesLoaded = false;
            }
            
            if (!m_areTitleTexturesLoaded)
            {
                m_title_font = std::unique_ptr<TextureWrapper>(loadTexture("title_font"));
                
                m_areTitleTexturesLoaded = true;
            }
            
            break;
        case RENDERER_TYPE_WORLD_1:            
            if (m_areLevelEditorTexturesLoaded)
            {
                destroyTexture(*m_level_editor);
                
                m_areLevelEditorTexturesLoaded = false;
            }
            
            if (!m_areJonTexturesLoaded)
            {
                m_jon = std::unique_ptr<TextureWrapper>(loadTexture("jon"));
                
                m_areJonTexturesLoaded = true;
            }
            
            if (!m_areVampireAndAbilityTexturesLoaded)
            {
                m_jon_ability = std::unique_ptr<TextureWrapper>(loadTexture("jon_ability"));
                m_vampire = std::unique_ptr<TextureWrapper>(loadTexture("vampire"));
                
                m_areVampireAndAbilityTexturesLoaded = true;
            }
            
            if (!m_areWorld1TexturesLoaded)
            {
                m_world_1_background = std::unique_ptr<TextureWrapper>(loadTexture("world_1_background", 1));
                m_world_1_foreground_3 = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_3"));
                m_world_1_foreground_2 = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_2"));
                m_world_1_foreground_1 = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_1"));
                
                m_areWorld1TexturesLoaded = true;
            }
            
            if (!m_areTitleTexturesLoaded)
            {
                m_title_font = std::unique_ptr<TextureWrapper>(loadTexture("title_font"));
                
                m_areTitleTexturesLoaded = true;
            }
            
            break;
        case RENDERER_TYPE_LEVEL_EDITOR:
            if (m_areVampireAndAbilityTexturesLoaded)
            {
                destroyTexture(*m_jon_ability);
                destroyTexture(*m_vampire);
                
                m_areVampireAndAbilityTexturesLoaded = false;
            }
            
            if (!m_areTitleTexturesLoaded)
            {
                m_title_font = std::unique_ptr<TextureWrapper>(loadTexture("title_font"));
                
                m_areTitleTexturesLoaded = true;
            }
            
            if (!m_areLevelEditorTexturesLoaded)
            {
                m_level_editor = std::unique_ptr<TextureWrapper>(loadTexture("level_editor"));
                
                m_areLevelEditorTexturesLoaded = true;
            }
            
            if (!m_areJonTexturesLoaded)
            {
                m_jon = std::unique_ptr<TextureWrapper>(loadTexture("jon"));
                
                m_areJonTexturesLoaded = true;
            }
            
            if (!m_areWorld1TexturesLoaded)
            {
                m_world_1_background = std::unique_ptr<TextureWrapper>(loadTexture("world_1_background", 1));
                m_world_1_foreground_3 = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_3"));
                m_world_1_foreground_2 = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_2"));
                m_world_1_foreground_1 = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_1"));
                
                m_areWorld1TexturesLoaded = true;
            }
            
            break;
        default:
            break;
    }
}

void Renderer::reinit()
{
    init(m_rendererType);
}

void Renderer::beginOpeningPanningSequence(Game& game)
{
    zoomIn();
    
    m_fStateTime = 0;
    
    updateCameraToFollowJon(game, 1337);
    
    m_camPos->setX(getCamPosFarRight(game));
    
    Jon& jon = game.getJon();
    float farLeft = jon.getPosition().getX() - CAM_WIDTH / 5;
    
    float changeInX = getCamPosFarRight(game) - farLeft;
    
    m_camPosAcceleration->set(changeInX / 8, m_camPos->getY() * 0.3f);
    m_camAcceleration->set((ZOOMED_OUT_CAM_WIDTH - m_fCamWidth) * 0.3f, (GAME_HEIGHT - m_fCamHeight) * 0.3f);
    m_camVelocity->set(0, 0);
    m_camPosVelocity->set(0, 0);
    
    m_fOriginalY = m_camPos->getY();
    m_fOriginalWidth = m_fCamWidth;
    m_fOriginalHeight = m_fCamHeight;
}

bool Renderer::updateCameraToFollowPathToJon(Game& game, float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if (m_fStateTime < 1.5f)
    {
        return false;
    }
    
    if (m_fStateTime <= 4.4825f)
    {
        m_camPosVelocity->add(m_camPosAcceleration->getX() * deltaTime, 0);
        m_camPosVelocity->add(0, m_camPosAcceleration->getY() * deltaTime);
        m_camVelocity->add(m_camAcceleration->getX() * deltaTime, m_camAcceleration->getY() * deltaTime);
        m_fCamWidth += m_camVelocity->getX() * deltaTime;
        m_fCamHeight += m_camVelocity->getY() * deltaTime;
        m_camPos->sub(0, m_camPosVelocity->getY() * deltaTime);
        
        if (m_camPos->getY() < 0)
        {
            m_camPos->setY(0);
        }
        
        if (m_fCamWidth > ZOOMED_OUT_CAM_WIDTH)
        {
            m_fCamWidth = ZOOMED_OUT_CAM_WIDTH;
        }
        
        if (m_fCamHeight > GAME_HEIGHT)
        {
            m_fCamHeight = GAME_HEIGHT;
        }
    }
    else if (m_fStateTime >= 5.0825f)
    {
        m_camPosVelocity->sub(m_camPosAcceleration->getX() * deltaTime, 0);
        m_camPosVelocity->add(0, m_camPosAcceleration->getY() * deltaTime);
        m_camVelocity->add(m_camAcceleration->getX() * deltaTime, m_camAcceleration->getY() * deltaTime);
        m_fCamWidth -= m_camVelocity->getX() * deltaTime;
        m_fCamHeight -= m_camVelocity->getY() * deltaTime;
        m_camPos->add(0, m_camPosVelocity->getY() * deltaTime);
        
        if (m_camPos->getY() > m_fOriginalY)
        {
            m_camPos->setY(m_fOriginalY);
        }
        
        if (m_fCamWidth < m_fOriginalWidth)
        {
            m_fCamWidth = m_fOriginalWidth;
        }
        
        if (m_fCamHeight < m_fOriginalHeight)
        {
            m_fCamHeight = m_fOriginalHeight;
        }
        
        if (m_camPosVelocity->getX() < 0)
        {
            m_camPosVelocity->setX(0);
        }
    }
    else
    {
        m_camVelocity->set(0, 0);
        m_camPosVelocity->setY(0);
    }
    
    Jon& jon = game.getJon();
    float farLeft = jon.getPosition().getX() - CAM_WIDTH / 5;
    
    float changeInX = getCamPosFarRight(game) - farLeft;
    
    if (m_camPosVelocity->getX() > changeInX * 0.28f)
    {
        m_camPosVelocity->setX(changeInX * 0.28f);
    }
    
    m_camPos->sub(m_camPosVelocity->getX() * deltaTime, 0);
    
    if (m_camPos->getX() < farLeft)
    {
        m_camPos->setX(farLeft);
    }
    
    return m_fStateTime >= 8.065f;
}

void Renderer::updateCameraToFollowJon(Game& game, float deltaTime)
{
    Jon& jon = game.getJon();
    m_camPos->setX(jon.getPosition().getX() - CAM_WIDTH / 5);
    float jy = jon.getPosition().getY() - jon.getHeight() / 2;
    float jonHeightPlusPadding = jon.getHeight() * 1.5f;
    
    float regionBottomY;
    if (jon.getAbilityState() == ABILITY_BURROW)
    {
        regionBottomY = jy - 1.22451534f;
    }
    else if (jon.isFalling())
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
        if (jon.getPhysicalState() != PHYSICAL_GROUNDED)
        {
            float newCamPos = jy + jonHeightPlusPadding - CAM_HEIGHT;
            if (newCamPos > m_camPos->getY())
            {
                m_camPos->setY(newCamPos);
            }
        }
        
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
    
    float farCamPos = getCamPosFarRight(game);
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

void Renderer::renderTitleScreen()
{
    beginFrame();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    bindToOffscreenFramebuffer();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    /// Render Title Logo
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    static float tlWidth = CAM_WIDTH * 2 / 3;
    static float tlHeight = tlWidth * 0.4921875f;
    static TextureRegion tlTr = TextureRegion(0, 1040, 2048, 1008, 2048, 2048);
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, CAM_HEIGHT * 2 / 3, tlWidth, tlHeight, 0, tlTr);
    m_spriteBatcher->endBatch(*m_title_font);
    
    m_spriteBatcher->beginBatch();
    
    static Color fontColor = Color(1, 1, 1, 1);
    static float fgWidth = CAM_WIDTH / 24;
    static float fgHeight = fgWidth * 1.140625f;
    
    float fontStartingY = CAM_HEIGHT * 2 / 5;
    
    {
        static std::string text = std::string("My best time: 27.46");
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, fontStartingY -= fgHeight, fgWidth, fgHeight, fontColor, true);
    }
    
    {
        static std::string text = std::string("Happy Halloween!");
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, fontStartingY -= fgHeight, fgWidth, fgHeight, fontColor, true);
    }
    
    fontStartingY -= fgHeight / 2;
    
    {
        static std::string text = std::string("Tap to begin");
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, fontStartingY -= fgHeight, fgWidth, fgHeight, fontColor, true);
    }
    
    m_spriteBatcher->endBatch(*m_title_font);
}

void Renderer::renderLoadingTextOnTitleScreen()
{
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    
    static Color fontColor = Color(1, 1, 1, 1);
    static float fgWidth = CAM_WIDTH / 30;
    static float fgHeight = fgWidth * 1.140625f;
    
    {
        static std::string text = std::string("Loading...");
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH - fgWidth / 2, fgHeight / 2, fgWidth, fgHeight, fontColor, false, true);
    }
    
    m_spriteBatcher->endBatch(*m_title_font);
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
    
    /// Render World midground Trees
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getTrees());
    m_spriteBatcher->endBatch(*m_world_1_foreground_2);
    
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
    m_spriteBatcher->endBatch(*m_world_1_foreground_1);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<Ground>>::iterator i = game.getGrounds().begin(); i != game.getGrounds().end(); i++)
    {
        if ((*i)->isForegroundMore())
        {
            renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
        }
    }
    
    for (std::vector<std::unique_ptr<Hole>>::iterator i = game.getHoles().begin(); i != game.getHoles().end(); i++)
    {
        renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
        
        for (std::vector<std::unique_ptr<HoleCover>>::iterator j = (*i)->getHoleCovers().begin(); j != (*i)->getHoleCovers().end(); j++)
        {
            renderPhysicalEntity(*(*j).get(), Assets::get(*(*j).get()));
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground_2);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<CaveExit>>::iterator i = game.getCaveExits().begin(); i != game.getCaveExits().end(); i++)
    {
        renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
        
        for (std::vector<std::unique_ptr<CaveExitCover>>::iterator j = (*i)->getCaveExitCovers().begin(); j != (*i)->getCaveExitCovers().end(); j++)
        {
            renderPhysicalEntity(*(*j).get(), Assets::get(*(*j).get()));
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground_3);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getLogVerticalTalls());
    renderPhysicalEntities(game.getLogVerticalShorts());
    renderPhysicalEntities(game.getThorns());
    renderPhysicalEntities(game.getStumps());
    renderPhysicalEntities(game.getSideSpikes());
    renderPhysicalEntities(game.getUpwardSpikes());
    renderPhysicalEntities(game.getJumpSprings());
    renderPhysicalEntities(game.getPlatforms());
    renderPhysicalEntities(game.getEndSigns());
    renderPhysicalEntities(game.getCarrots());
    renderPhysicalEntities(game.getGoldenCarrots());
    renderPhysicalEntitiesWithColor(game.getRocks());
    m_spriteBatcher->endBatch(*m_world_1_foreground_1);
    
    /// Render Jon Effects (e.g. Dust Clouds)
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<Jon>>::iterator i = game.getJons().begin(); i != game.getJons().end(); i++)
    {
        renderPhysicalEntitiesWithColor((*i)->getDustClouds());
    }
    m_spriteBatcher->endBatch(*m_world_1_foreground_2);
}

void Renderer::renderJon(Game& game)
{
    /// Render Jon
    
    if (game.getJons().size() > 0)
    {
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
}

void Renderer::renderBounds(Game& game)
{
    /// Render World midground Trees
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
    m_boundsRectangleBatcher->beginBatch();
    renderBoundsForPhysicalEntities(game.getJons());
    m_boundsRectangleBatcher->endBatch();
    
    m_boundsRectangleBatcher->beginBatch();
    renderBoundsForPhysicalEntities(game.getTrees());
    m_boundsRectangleBatcher->endBatch();
    
    /// Render World
    
    m_boundsRectangleBatcher->beginBatch();
    renderBoundsForPhysicalEntities(game.getGrounds());
    renderBoundsForPhysicalEntities(game.getHoles());
    renderBoundsForPhysicalEntities(game.getCaveExits());
    m_boundsRectangleBatcher->endBatch();
    
    m_boundsRectangleBatcher->beginBatch();
    renderBoundsForPhysicalEntities(game.getLogVerticalTalls());
    renderBoundsForPhysicalEntities(game.getLogVerticalShorts());
    renderBoundsForPhysicalEntities(game.getThorns());
    renderBoundsForPhysicalEntities(game.getStumps());
    renderBoundsForPhysicalEntities(game.getSideSpikes());
    renderBoundsForPhysicalEntities(game.getUpwardSpikes());
    renderBoundsForPhysicalEntities(game.getJumpSprings());
    renderBoundsForPhysicalEntities(game.getPlatforms());
    renderBoundsForPhysicalEntities(game.getEndSigns());
    renderBoundsForPhysicalEntities(game.getCarrots());
    renderBoundsForPhysicalEntities(game.getGoldenCarrots());
    renderBoundsForPhysicalEntities(game.getRocks());
    m_boundsRectangleBatcher->endBatch();
}

void Renderer::renderEntityHighlighted(PhysicalEntity& entity, Color& c)
{
    updateMatrix(m_camPos->getX(), m_camPos->getX() + m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
    m_highlightRectangleBatcher->beginBatch();
    renderHighlightForPhysicalEntity(entity, c);
    m_highlightRectangleBatcher->endBatch();
}

void Renderer::renderHud(Game& game, BackButton &backButton)
{
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    /// Render Back Button
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(backButton, Assets::get(backButton));
    m_spriteBatcher->endBatch(*m_world_1_foreground_2);
    
    static Color fontColor = Color(1, 1, 1, 1);
    static float fgWidth = CAM_WIDTH / 24;
    static float fgHeight = fgWidth * 1.140625f;
    
    /// Render Play Time
    
    m_spriteBatcher->beginBatch();
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << game.getStateTime();
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, CAM_HEIGHT - fgHeight / 2, fgWidth, fgHeight, fontColor, true);
    }
    m_spriteBatcher->endBatch(*m_title_font);
    
    std::stringstream ss;
    ss << game.getNumTotalCarrots();
    std::string totalNumGoldenCarrots = ss.str();
    int offset = 1 + totalNumGoldenCarrots.size();
    
    /// Render Num / Total Carrots
    
    m_spriteBatcher->beginBatch();
    {
        std::stringstream ss;
        ss << (game.getNumTotalCarrots() - game.getCarrots().size());
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH - (offset + text.size()) * fgWidth - fgWidth / 2, CAM_HEIGHT - fgHeight / 2, fgWidth, fgHeight, fontColor);
    }
    {
        std::stringstream ss;
        ss << "/";
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH - offset * fgWidth - fgWidth / 2, CAM_HEIGHT - fgHeight / 2, fgWidth, fgHeight, fontColor);
    }
    {
        std::stringstream ss;
        ss << game.getNumTotalCarrots();
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH - text.size() * fgWidth - fgWidth / 2, CAM_HEIGHT - fgHeight / 2, fgWidth, fgHeight, fontColor);
    }
    m_spriteBatcher->endBatch(*m_title_font);
    
    /// Render Num / Total Golden Carrots
    
    m_spriteBatcher->beginBatch();
    {
        std::stringstream ss;
        ss << (game.getNumTotalGoldenCarrots() - game.getGoldenCarrots().size()) << "/" << game.getNumTotalGoldenCarrots();
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH - 3 * fgWidth - fgWidth / 2, CAM_HEIGHT - fgHeight - fgHeight / 2, fgWidth, fgHeight, fontColor);
    }
    m_spriteBatcher->endBatch(*m_title_font);
    
    static Carrot uiCarrot = Carrot(CAM_WIDTH - fgWidth / 2, CAM_HEIGHT - fgHeight / 2, fgWidth, fgHeight);
    static GoldenCarrot uiGoldenCarrot = GoldenCarrot(CAM_WIDTH - fgWidth / 2, CAM_HEIGHT - fgHeight - fgHeight / 2, fgWidth, fgHeight);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(uiCarrot, Assets::get(uiCarrot));
    renderPhysicalEntity(uiGoldenCarrot, Assets::get(uiGoldenCarrot));
    m_spriteBatcher->endBatch(*m_world_1_foreground_1);
}

void Renderer::renderLevelEditor(LevelEditorActionsPanel& leap, LevelEditorEntitiesPanel& leep, TrashCan& tc, LevelSelectorPanel& lsp)
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
    renderPhysicalEntity(leap, Assets::get(leap));
    renderPhysicalEntity(leep, Assets::get(leep));
    m_spriteBatcher->endBatch(*m_level_editor);
    
    updateMatrix(m_camPos->getX(), m_camPos->getX() + m_fCamWidth, m_camPos->getY(), m_camPos->getY() + m_fCamHeight);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(tc, Assets::get(tc));
    m_spriteBatcher->endBatch(*m_level_editor);
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    if (leep.isOpen())
    {
        updateMatrix(0, CAM_WIDTH, leep.getEntitiesCameraPos(), leep.getEntitiesCameraPos() + CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep.getJons());
        m_spriteBatcher->endBatch(*m_jon);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep.getTrees());
        m_spriteBatcher->endBatch(*m_world_1_foreground_2);
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<Ground>>::iterator i = leep.getGrounds().begin(); i != leep.getGrounds().end(); i++)
        {
            if ((*i)->isForegroundMore())
            {
                continue;
            }
            
            renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
        }
        m_spriteBatcher->endBatch(*m_world_1_foreground_1);
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<Ground>>::iterator i = leep.getGrounds().begin(); i != leep.getGrounds().end(); i++)
        {
            if ((*i)->isForegroundMore())
            {
                renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
            }
        }
        
        for (std::vector<std::unique_ptr<Hole>>::iterator i = leep.getHoles().begin(); i != leep.getHoles().end(); i++)
        {
            renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
            
            for (std::vector<std::unique_ptr<HoleCover>>::iterator j = (*i)->getHoleCovers().begin(); j != (*i)->getHoleCovers().end(); j++)
            {
                renderPhysicalEntity(*(*j).get(), Assets::get(*(*j).get()));
            }
        }
        m_spriteBatcher->endBatch(*m_world_1_foreground_2);
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<CaveExit>>::iterator i = leep.getCaveExits().begin(); i != leep.getCaveExits().end(); i++)
        {
            renderPhysicalEntity(*(*i).get(), Assets::get(*(*i).get()));
            
            for (std::vector<std::unique_ptr<CaveExitCover>>::iterator j = (*i)->getCaveExitCovers().begin(); j != (*i)->getCaveExitCovers().end(); j++)
            {
                renderPhysicalEntity(*(*j).get(), Assets::get(*(*j).get()));
            }
        }
        m_spriteBatcher->endBatch(*m_world_1_foreground_3);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep.getLogVerticalTalls());
        renderPhysicalEntities(leep.getLogVerticalShorts());
        renderPhysicalEntities(leep.getThorns());
        renderPhysicalEntities(leep.getStumps());
        renderPhysicalEntities(leep.getSideSpikes());
        renderPhysicalEntities(leep.getUpwardSpikes());
        renderPhysicalEntities(leep.getJumpSprings());
        renderPhysicalEntities(leep.getPlatforms());
        renderPhysicalEntities(leep.getEndSigns());
        renderPhysicalEntities(leep.getCarrots());
        renderPhysicalEntities(leep.getGoldenCarrots());
        renderPhysicalEntitiesWithColor(leep.getRocks());
        m_spriteBatcher->endBatch(*m_world_1_foreground_1);
    }
    
    if (lsp.isOpen())
    {
        updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(lsp, Assets::get(lsp));
        m_spriteBatcher->endBatch(*m_level_editor);
        
        static Color fontColor = Color(1, 1, 1, 1);
        
        float fgWidth = lsp.getTextSize();
        float fgHeight = fgWidth * 1.140625f;
        
        m_spriteBatcher->beginBatch();
        {
            std::stringstream ss;
            ss << lsp.getWorld();
            std::string text = ss.str();
            m_font->renderText(*m_spriteBatcher, text, lsp.getWorldTextPosition().getX(), lsp.getWorldTextPosition().getY(), fgWidth, fgHeight, fontColor, false, true);
        }
        {
            std::stringstream ss;
            ss << lsp.getLevel();
            std::string text = ss.str();
            m_font->renderText(*m_spriteBatcher, text, lsp.getLevelTextPosition().getX(), lsp.getLevelTextPosition().getY(), fgWidth, fgHeight, fontColor, false, true);
        }
        m_spriteBatcher->endBatch(*m_title_font);
    }
}

void Renderer::renderToScreen()
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
    if (m_areTitleTexturesLoaded)
    {
        destroyTexture(*m_title_font);
        
        m_areTitleTexturesLoaded = false;
    }
    
    if (m_areJonTexturesLoaded)
    {
        destroyTexture(*m_jon);
        
        m_areJonTexturesLoaded = false;
    }
    
    if (m_areVampireAndAbilityTexturesLoaded)
    {
        destroyTexture(*m_vampire);
        destroyTexture(*m_jon_ability);
        
        m_areVampireAndAbilityTexturesLoaded = false;
    }
    
    if (m_areWorld1TexturesLoaded)
    {
        destroyTexture(*m_world_1_background);
        destroyTexture(*m_world_1_foreground_3);
        destroyTexture(*m_world_1_foreground_2);
        destroyTexture(*m_world_1_foreground_1);
        
        m_areWorld1TexturesLoaded = false;
    }
    
    if (m_areLevelEditorTexturesLoaded)
    {
        destroyTexture(*m_level_editor);
        
        m_areLevelEditorTexturesLoaded = false;
    }
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

float Renderer::getCamPosFarRight(Game &game)
{
    float farRight = game.getFarRight();
    float farCamPos = farRight - CAM_WIDTH;
    
    return farCamPos;
}