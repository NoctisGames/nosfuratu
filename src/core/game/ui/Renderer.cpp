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
#include "TransitionGpuProgramWrapper.h"
#include "SinWaveTextureGpuProgramWrapper.h"
#include "SnakeDeathTextureGpuProgramWrapper.h"
#include "ShockwaveTextureGpuProgramWrapper.h"
#include "TransDeathGpuProgramWrapper.h"

#include <math.h>
#include <sstream>
#include <iomanip>

#define aboveGroundRegionBottomY 8.750433275563259f

Renderer::Renderer() : m_fStateTime(0), m_iFramebufferIndex(0), m_areTitleTexturesLoaded(false), m_areWorld1TexturesLoaded(false), m_areLevelEditorTexturesLoaded(false), m_areShadersLoaded(false), m_sinWaveTextureProgram(nullptr), m_snakeDeathTextureProgram(nullptr), m_shockwaveTextureGpuProgramWrapper(nullptr), m_framebufferToScreenGpuProgramWrapper(nullptr), m_framebufferTintGpuProgramWrapper(nullptr)
{
    int x = 0;
    int y = 0;
    int glyphWidth = 64;
    int glyphHeight = 73;
    
    if (Assets::getInstance()->isUsingCompressedTextureSet())
    {
        x = x / 2.0;
        y = y / 2.0;
        glyphWidth = glyphWidth / 2.0;
        glyphHeight = glyphHeight / 2.0;
    }
    
    m_font = std::unique_ptr<Font>(new Font(x, y, 16, glyphWidth, glyphHeight, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048));
    m_camBounds = std::unique_ptr<Rectangle>(new Rectangle(0, aboveGroundRegionBottomY, CAM_WIDTH, CAM_HEIGHT));
    m_camPosAcceleration = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
    m_camPosVelocity = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
    m_camAcceleration = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
    m_camVelocity = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
}

void Renderer::init(RendererType type)
{
    m_rendererType = type;
    
    bool compressed = Assets::getInstance()->isUsingCompressedTextureSet();
    
    switch (type)
    {
        case RENDERER_TYPE_LEVEL_EDITOR:
            if (!m_areLevelEditorTexturesLoaded)
            {
                m_level_editor = loadTexture("level_editor");
                
                m_areLevelEditorTexturesLoaded = true;
            }
        case RENDERER_TYPE_WORLD_1:
            if (!m_areWorld1TexturesLoaded)
            {
                m_world_1_background = loadTexture("world_1_background", 1);
                
                m_world_1_ground_w_cave = loadTexture(compressed ? "compressed_world_1_ground" : "world_1_ground_w_cave");
                m_world_1_ground_wo_cave = compressed ? m_world_1_ground_w_cave : loadTexture("world_1_ground_wo_cave");
                m_world_1_cave = compressed ? m_world_1_ground_w_cave : loadTexture("world_1_cave");
                
                m_world_1_objects = loadTexture(compressed ? "compressed_world_1_objects" : "world_1_objects");
                m_game_objects = compressed ? m_world_1_objects : loadTexture("game_objects");
                m_world_1_enemies = compressed ? m_world_1_objects : loadTexture("world_1_enemies");
                m_world_1_snake_cave = compressed ? m_world_1_objects : loadTexture("world_1_snake_cave");
                
                m_jon = loadTexture(compressed ? "compressed_jon" : "jon");
                m_jon_ability = compressed ? m_jon : loadTexture("jon_ability");
                m_jon_poses = compressed ? m_jon : loadTexture("jon_poses");
                
                m_vampire = loadTexture(compressed ? "compressed_vampire" : "vampire");
                m_vampire_poses = compressed ? m_vampire : loadTexture("vampire_poses");
                m_vampire_transform = compressed ? m_vampire : loadTexture("vampire_transform");
                
                m_trans_death_shader_helper = loadTexture("trans_death_shader_helper");
                
                m_areWorld1TexturesLoaded = true;
            }
        case RENDERER_TYPE_TITLE:
            if (!m_areTitleTexturesLoaded)
            {
                m_title_font = loadTexture(compressed ? "compressed_misc" : "title_font");
                m_world_1_misc = compressed ? m_title_font : loadTexture("world_1_misc");
                m_world_map = compressed ? m_title_font : loadTexture("world_map");
                
                m_areTitleTexturesLoaded = true;
            }
        default:
            break;
    }
    
    if (m_framebuffers.size() == 0)
    {
        addFramebuffers();
    }
    
    if (!m_areShadersLoaded)
    {
        loadShaders();
        
        m_areShadersLoaded = true;
    }
}

void Renderer::beginFrame()
{
    clearFramebufferWithColor(0, 0, 0, 1);
    
    setFramebuffer(0);
    
    clearFramebufferWithColor(0, 0, 0, 1);
}

void Renderer::setFramebuffer(int framebufferIndex)
{
    m_iFramebufferIndex = framebufferIndex;
    bindToOffscreenFramebuffer(framebufferIndex);
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
    
    m_camBounds->getLowerLeft().setX(getCamPosFarRight(game));
    
    Jon& jon = game.getJon();
    float farLeft = jon.getPosition().getX() - CAM_WIDTH / 5;
    
    float changeInX = getCamPosFarRight(game) - farLeft;
    
    m_camPosAcceleration->set(changeInX / 8, m_camBounds->getLowerLeft().getY() * 0.3f);
    m_camAcceleration->set((ZOOMED_OUT_CAM_WIDTH - m_camBounds->getWidth()) * 0.3f, (GAME_HEIGHT - m_camBounds->getHeight()) * 0.3f);
    m_camVelocity->set(0, 0);
    m_camPosVelocity->set(0, 0);
    
    m_fOriginalY = m_camBounds->getLowerLeft().getY();
    m_fOriginalWidth = m_camBounds->getWidth();
    m_fOriginalHeight = m_camBounds->getHeight();
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
        m_camBounds->setWidth(m_camBounds->getWidth() + m_camVelocity->getX() * deltaTime);
        m_camBounds->setHeight(m_camBounds->getHeight() + m_camVelocity->getY() * deltaTime);
        m_camBounds->getLowerLeft().sub(0, m_camPosVelocity->getY() * deltaTime);
        
        if (m_camBounds->getLowerLeft().getY() < 0)
        {
            m_camBounds->getLowerLeft().setY(0);
        }
        
        if (m_camBounds->getWidth() > ZOOMED_OUT_CAM_WIDTH)
        {
            m_camBounds->setWidth(ZOOMED_OUT_CAM_WIDTH);
        }
        
        if (m_camBounds->getHeight() > GAME_HEIGHT)
        {
            m_camBounds->setHeight(GAME_HEIGHT);
        }
    }
    else if (m_fStateTime >= 5.0825f)
    {
        m_camPosVelocity->sub(m_camPosAcceleration->getX() * deltaTime, 0);
        m_camPosVelocity->add(0, m_camPosAcceleration->getY() * deltaTime);
        m_camVelocity->add(m_camAcceleration->getX() * deltaTime, m_camAcceleration->getY() * deltaTime);
        m_camBounds->setWidth(m_camBounds->getWidth() - m_camVelocity->getX() * deltaTime);
        m_camBounds->setHeight(m_camBounds->getHeight() - m_camVelocity->getY() * deltaTime);
        m_camBounds->getLowerLeft().add(0, m_camPosVelocity->getY() * deltaTime);
        
        if (m_camBounds->getLowerLeft().getY() > m_fOriginalY)
        {
            m_camBounds->getLowerLeft().setY(m_fOriginalY);
        }
        
        if (m_camBounds->getWidth() < m_fOriginalWidth)
        {
            m_camBounds->setWidth(m_fOriginalWidth);
        }
        
        if (m_camBounds->getHeight() < m_fOriginalHeight)
        {
            m_camBounds->setHeight(m_fOriginalHeight);
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
    
    m_camBounds->getLowerLeft().sub(m_camPosVelocity->getX() * deltaTime, 0);
    
    if (m_camBounds->getLowerLeft().getX() < farLeft)
    {
        m_camBounds->getLowerLeft().setX(farLeft);
    }
    
    return m_fStateTime >= 8.065f;
}

void Renderer::updateCameraToFollowJon(Game& game, float deltaTime)
{
    Jon& jon = game.getJon();
    m_camBounds->getLowerLeft().setX(jon.getPosition().getX() - CAM_WIDTH / 5);
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
    
    float camSpeed = regionBottomY - m_camBounds->getLowerLeft().getY();
    float camVelocityY = regionBottomY > m_camBounds->getLowerLeft().getY() ? camSpeed : regionBottomY == m_camBounds->getLowerLeft().getY() ? 0 : camSpeed * 4;
    m_camBounds->getLowerLeft().add(0, camVelocityY * deltaTime);
    
    if (camVelocityY > 0)
    {
        if (jon.getPhysicalState() != PHYSICAL_GROUNDED)
        {
            float newCamPos = jy + jonHeightPlusPadding - CAM_HEIGHT;
            if (newCamPos > m_camBounds->getLowerLeft().getY())
            {
                m_camBounds->getLowerLeft().setY(newCamPos);
            }
        }
        
        if (m_camBounds->getLowerLeft().getY() > regionBottomY)
        {
            m_camBounds->getLowerLeft().setY(regionBottomY);
        }
    }
    
    if (camVelocityY < 0)
    {
        if (m_camBounds->getLowerLeft().getY() < regionBottomY)
        {
            m_camBounds->getLowerLeft().setY(regionBottomY);
        }
    }
    
    if (m_camBounds->getLowerLeft().getY() < 0)
    {
        m_camBounds->getLowerLeft().setY(0);
    }
    
    if (m_camBounds->getLowerLeft().getY() > GAME_HEIGHT - CAM_HEIGHT)
    {
        m_camBounds->getLowerLeft().setY(GAME_HEIGHT - CAM_HEIGHT);
    }
    
    float farCamPos = getCamPosFarRight(game);
    if (m_camBounds->getLowerLeft().getX() > farCamPos)
    {
        m_camBounds->getLowerLeft().setX(farCamPos);
    }
}

void Renderer::moveCamera(float x)
{
    m_camBounds->getLowerLeft().add(x, 0);
}

void Renderer::zoomOut()
{
    m_camBounds->getLowerLeft().set(0, 0);
    m_camBounds->setWidth(ZOOMED_OUT_CAM_WIDTH);
    m_camBounds->setHeight(GAME_HEIGHT);
}

void Renderer::zoomIn()
{
    m_camBounds->getLowerLeft().set(0, aboveGroundRegionBottomY);
    m_camBounds->setWidth(CAM_WIDTH);
    m_camBounds->setHeight(CAM_HEIGHT);
}

void Renderer::renderTitleScreen()
{
    /// Render Title Logo
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    static float tlWidth = CAM_WIDTH * 2 / 3;
    static float tlHeight = tlWidth * 0.4921875f;
    
    int x = 0;
    int y = 1040;
    int regionWidth = 2048;
    int regionHeight = 1008;
    
    if (Assets::getInstance()->isUsingCompressedTextureSet())
    {
        x = x / 2.0;
        y = y / 2.0;
        regionWidth = regionWidth / 2.0;
        regionHeight = regionHeight / 2.0;
    }
    
    static TextureRegion tlTr = TextureRegion(x, y, regionWidth, regionHeight, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, CAM_HEIGHT * 2 / 3, tlWidth, tlHeight, 0, tlTr);
    m_spriteBatcher->endBatch(*m_title_font);
    
    m_spriteBatcher->beginBatch();
    
    static Color fontColor = Color(1, 1, 1, 1);
    static float fgWidth = CAM_WIDTH / 28;
    static float fgHeight = fgWidth * 1.140625f;
    
    float fontStartingY = CAM_HEIGHT * 2 / 5;
    
    {
        static std::string text = std::string("Swipe down to dig");
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, fontStartingY -= fgHeight, fgWidth, fgHeight, fontColor, true);
    }
    
    {
        static std::string text = std::string("Swipe right to punch");
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, fontStartingY -= fgHeight, fgWidth, fgHeight, fontColor, true);
    }
    
    {
        static std::string text = std::string("Hold screen to transform");
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

void Renderer::renderWorldMapScreenBackground()
{
    /// Render Title Logo
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    
    int x = 0;
    int y = 0;
    int regionWidth = 2048;
    int regionHeight = 766;
    
    if (Assets::getInstance()->isUsingCompressedTextureSet())
    {
        x = x / 2.0;
        y = y / 2.0;
        y += 1024;
        regionWidth = regionWidth / 2.0;
        regionHeight = regionHeight / 2.0;
    }
    
    static TextureRegion tr = TextureRegion(x, y, regionWidth, regionHeight, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static Color bgColor = Color(1, 1, 1, 1);
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH, CAM_HEIGHT, 0, bgColor, tr);
    m_spriteBatcher->endBatch(*m_world_map);
}

void Renderer::renderWorldMapScreenUi(BackButton& backButton)
{
    /// Render Back Button
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(backButton, Assets::getInstance()->get(backButton));
    m_spriteBatcher->endBatch(*m_title_font);
    
    m_spriteBatcher->beginBatch();
    
    static Color fontColor = Color(0, 0, 0, 1);
    static float fgWidth = CAM_WIDTH / 28;
    static float fgHeight = fgWidth * 1.140625f;
    
    m_highlightRectangleBatcher->beginBatch();
    
    float fontStartingY = CAM_HEIGHT - fgHeight;
    
    {
        Rectangle r = Rectangle(CAM_WIDTH / 2 - fgWidth * 8, fontStartingY - fgHeight / 2, fgWidth * 16, fgHeight);
        static Color c = Color(0, 0.4f, 1, 0.9f);
        m_highlightRectangleBatcher->renderRectangle(r, c);
        
        static std::string text = std::string("Select a Level:");
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, fontStartingY, fgWidth, fgHeight, fontColor, true);
        fontStartingY -= fgHeight;
    }
    
    fontStartingY -= fgHeight * 8;
    
    {
        Rectangle r = Rectangle(0.6f, fontStartingY, CAM_WIDTH / 4, CAM_HEIGHT / 2);
        static Color c = Color(0, 0.4f, 1, 0.9f);
        m_highlightRectangleBatcher->renderRectangle(r, c);
        
        static std::string text = std::string("1");
        m_font->renderText(*m_spriteBatcher, text, 0.6f + CAM_WIDTH / 8, fontStartingY + CAM_HEIGHT / 4, fgWidth, fgHeight, fontColor, true);
    }
    
    {
        Rectangle r = Rectangle(0.6f + CAM_WIDTH / 3, fontStartingY, CAM_WIDTH / 4, CAM_HEIGHT / 2);
        static Color c = Color(0, 0.4f, 1, 0.9f);
        m_highlightRectangleBatcher->renderRectangle(r, c);
        
        static std::string text = std::string("2");
        m_font->renderText(*m_spriteBatcher, text, 0.6f + CAM_WIDTH / 3 + CAM_WIDTH / 8, fontStartingY + CAM_HEIGHT / 4, fgWidth, fgHeight, fontColor, true);
    }
    
    {
        Rectangle r = Rectangle(0.6f + CAM_WIDTH / 3 * 2, fontStartingY, CAM_WIDTH / 4, CAM_HEIGHT / 2);
        static Color c = Color(0, 0.4f, 1, 0.9f);
        m_highlightRectangleBatcher->renderRectangle(r, c);
        
        static std::string text = std::string("3");
        m_font->renderText(*m_spriteBatcher, text, 0.6f + CAM_WIDTH / 3 * 2 + CAM_WIDTH / 8, fontStartingY + CAM_HEIGHT / 4, fgWidth, fgHeight, fontColor, true);
    }
    
    m_highlightRectangleBatcher->endBatch();
    m_spriteBatcher->endBatch(*m_title_font);
}

void Renderer::renderLoadingTextOnWorldMapScreen()
{
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    
    static Color fontColor = Color(0, 0, 0, 1);
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
    /// Render Background
    
    updateMatrix(0, m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
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
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getTrees(), true);
    m_spriteBatcher->endBatch(*m_world_1_misc);
    
    /// Render World
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<Ground>>::iterator i = game.getGrounds().begin(); i != game.getGrounds().end(); i++)
    {
        if ((*i)->is_world_1_cave())
        {
            renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()), true);
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_cave);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<CaveExit>>::iterator i = game.getCaveExits().begin(); i != game.getCaveExits().end(); i++)
    {
        renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()), true);
        renderPhysicalEntitiesWithColor((*i)->getCaveExitCovers(), true);
    }
    
    for (std::vector<std::unique_ptr<Ground>>::iterator i = game.getGrounds().begin(); i != game.getGrounds().end(); i++)
    {
        if ((*i)->is_world_1_ground_w_cave())
        {
            renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()), true);
        }
    }
    
    for (std::vector<std::unique_ptr<Hole>>::iterator i = game.getHoles().begin(); i != game.getHoles().end(); i++)
    {
        renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()), true);
        renderPhysicalEntities((*i)->getHoleCovers(), true);
    }
    m_spriteBatcher->endBatch(*m_world_1_ground_w_cave);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<Ground>>::iterator i = game.getGrounds().begin(); i != game.getGrounds().end(); i++)
    {
        if ((*i)->is_world_1_ground_wo_cave())
        {
            renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()), true);
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_ground_wo_cave);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getJumpSprings(), true);
    renderPhysicalEntities(game.getCarrots(), true);
    renderPhysicalEntities(game.getGoldenCarrots(), true);
    m_spriteBatcher->endBatch(*m_game_objects);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getPlatforms(), true);
    renderPhysicalEntities(game.getLogVerticalTalls(), true);
    renderPhysicalEntities(game.getLogVerticalShorts(), true);
    renderPhysicalEntities(game.getThorns(), true);
    renderPhysicalEntities(game.getStumps(), true);
    renderPhysicalEntities(game.getSideSpikes(), true);
    renderPhysicalEntities(game.getUpwardSpikes(), true);
    renderPhysicalEntities(game.getEndSigns(), true);
    renderPhysicalEntitiesWithColor(game.getRocks(), true);
    m_spriteBatcher->endBatch(*m_world_1_objects);
    
    for (std::vector<std::unique_ptr<SnakeGrunt>>::iterator i = game.getSnakeGruntEnemies().begin(); i != game.getSnakeGruntEnemies().end(); i++)
    {
        std::unique_ptr<SnakeGrunt>& upItem = *i;
        SnakeGrunt* pItem = upItem.get();
        SnakeGrunt& item = *pItem;
        
        m_spriteBatcher->beginBatch();
        m_snakeDeathTextureProgram->setColorAdditive(item.getColorAdditive());
        renderPhysicalEntityWithColor(item, Assets::getInstance()->get(item), item.getColor(), true);
        m_spriteBatcher->endBatch(*m_world_1_enemies, *m_snakeDeathTextureProgram);
    }
    
    for (std::vector<std::unique_ptr<SnakeHorned>>::iterator i = game.getSnakeHornedEnemies().begin(); i != game.getSnakeHornedEnemies().end(); i++)
    {
        std::unique_ptr<SnakeHorned>& upItem = *i;
        SnakeHorned* pItem = upItem.get();
        SnakeHorned& item = *pItem;
        
        m_spriteBatcher->beginBatch();
        m_snakeDeathTextureProgram->setColorAdditive(item.getColorAdditive());
        renderPhysicalEntityWithColor(item, Assets::getInstance()->get(item), item.getColor(), true);
        m_spriteBatcher->endBatch(*m_world_1_enemies, *m_snakeDeathTextureProgram);
    }
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<SnakeGrunt>>::iterator i = game.getSnakeGruntEnemies().begin(); i != game.getSnakeGruntEnemies().end(); i++)
    {
        renderPhysicalEntities((*i)->getSnakeSpirits(), true);
    }
    
    for (std::vector<std::unique_ptr<SnakeHorned>>::iterator i = game.getSnakeHornedEnemies().begin(); i != game.getSnakeHornedEnemies().end(); i++)
    {
        renderPhysicalEntities((*i)->getSnakeSpirits(), true);
    }
    m_spriteBatcher->endBatch(*m_world_1_enemies);
}

void Renderer::renderJon(Game& game)
{
    if (game.getJons().size() > 0)
    {
        Jon& jon = game.getJon();
        bool isTransforming = jon.isTransformingIntoVampire() || jon.isRevertingToRabbit();
        bool isVampire = jon.isVampire();
        bool isUsingAbility = jon.getAbilityState() != ABILITY_NONE;
		bool isDying = jon.getState() != JON_ALIVE;
        
        /// Render Jon Effects (e.g. Dust Clouds)
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<Jon>>::iterator i = game.getJons().begin(); i != game.getJons().end(); i++)
        {
            renderPhysicalEntitiesWithColor((*i)->getDustClouds());
        }
        m_spriteBatcher->endBatch((isVampire || jon.isRevertingToRabbit()) ? *m_vampire : *m_jon);
        
        /// Render Jon After Images
        
        for (std::vector<std::unique_ptr<Jon>>::iterator i = jon.getAfterImages().begin(); i != jon.getAfterImages().end(); i++)
        {
            m_spriteBatcher->beginBatch();
            
            std::unique_ptr<Jon>& upItem = *i;
            Jon* pItem = upItem.get();
            Jon& item = *pItem;
            renderPhysicalEntityWithColor(item, Assets::getInstance()->get(item), item.getColor());
            
            bool iisTransforming = item.isTransformingIntoVampire() || item.isRevertingToRabbit();
            bool iisVampire = item.isVampire();
            bool iisUsingAbility = item.getAbilityState() != ABILITY_NONE;
            bool iisDying = item.getState() != JON_ALIVE;
            
            if (iisDying)
            {
                m_spriteBatcher->endBatch(iisVampire ? *m_vampire_poses : *m_jon_poses);
            }
            else if (iisTransforming)
            {
                m_spriteBatcher->endBatch(*m_vampire_transform);
            }
            else if (item.isAllowedToMove() || item.isFalling() || item.isLanding())
            {
                m_spriteBatcher->endBatch(iisVampire ? *m_vampire : iisUsingAbility ?  *m_jon_ability : *m_jon);
            }
            else
            {
                m_spriteBatcher->endBatch(iisVampire ? *m_vampire_poses : *m_jon_poses);
            }
        }
        
        /// Render Jon
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntitiesWithColor(game.getJons());
		if (isDying)
		{
			m_spriteBatcher->endBatch(isVampire ? *m_vampire_poses : *m_jon_poses);
		}
		else if (isTransforming)
        {
            m_spriteBatcher->endBatch(*m_vampire_transform);
        }
        else if (jon.isAllowedToMove() || jon.isFalling() || jon.isLanding())
        {
            m_spriteBatcher->endBatch(isVampire ? *m_vampire : isUsingAbility ?  *m_jon_ability : *m_jon);
        }
        else
        {
            m_spriteBatcher->endBatch(isVampire ? *m_vampire_poses : *m_jon_poses);
        }
    }
}

void Renderer::renderBounds(Game& game)
{
	/// Render World midground Trees
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
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
    renderBoundsForPhysicalEntities(game.getPlatforms());
    renderBoundsForPhysicalEntities(game.getLogVerticalTalls());
    renderBoundsForPhysicalEntities(game.getLogVerticalShorts());
    renderBoundsForPhysicalEntities(game.getThorns());
    renderBoundsForPhysicalEntities(game.getStumps());
    renderBoundsForPhysicalEntities(game.getSideSpikes());
    renderBoundsForPhysicalEntities(game.getUpwardSpikes());
    renderBoundsForPhysicalEntities(game.getJumpSprings());
    renderBoundsForPhysicalEntities(game.getEndSigns());
    renderBoundsForPhysicalEntities(game.getCarrots());
    renderBoundsForPhysicalEntities(game.getGoldenCarrots());
    renderBoundsForPhysicalEntities(game.getRocks());
    renderBoundsForPhysicalEntities(game.getSnakeGruntEnemies());
    renderBoundsForPhysicalEntities(game.getSnakeHornedEnemies());
    m_boundsRectangleBatcher->endBatch();
}

void Renderer::renderEntityHighlighted(PhysicalEntity& entity, Color& c)
{
	updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_highlightRectangleBatcher->beginBatch();
    renderHighlightForPhysicalEntity(entity, c);
    m_highlightRectangleBatcher->endBatch();
}

void Renderer::renderHud(Game& game, BackButton &backButton)
{
	updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    /// Render Back Button
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(backButton, Assets::getInstance()->get(backButton));
    m_spriteBatcher->endBatch(*m_title_font);
    
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
    int offset = 1 + (int) totalNumGoldenCarrots.size();
    
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
    renderPhysicalEntity(uiCarrot, Assets::getInstance()->get(uiCarrot));
    renderPhysicalEntity(uiGoldenCarrot, Assets::getInstance()->get(uiGoldenCarrot));
    m_spriteBatcher->endBatch(*m_world_1_objects);
}

void Renderer::renderLevelEditor(LevelEditorActionsPanel& leap, LevelEditorEntitiesPanel& leep, TrashCan& tc, LevelSelectorPanel& lsp)
{
	static Rectangle originMarker = Rectangle(0, 0, 0.1f, GAME_HEIGHT);
    static Color originMarkerColor = Color(0, 0, 0, 0.7f);
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_highlightRectangleBatcher->beginBatch();
    m_highlightRectangleBatcher->renderRectangle(originMarker, originMarkerColor);
    m_highlightRectangleBatcher->endBatch();
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    /// Render Level Editor
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(leap, Assets::getInstance()->get(leap));
    renderPhysicalEntity(leep, Assets::getInstance()->get(leep));
    m_spriteBatcher->endBatch(*m_level_editor);
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(tc, Assets::getInstance()->get(tc));
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
        m_spriteBatcher->endBatch(*m_world_1_misc);
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<Ground>>::iterator i = leep.getGrounds().begin(); i != leep.getGrounds().end(); i++)
        {
            if ((*i)->is_world_1_cave())
            {
                renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()));
            }
        }
        m_spriteBatcher->endBatch(*m_world_1_cave);
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<CaveExit>>::iterator i = leep.getCaveExits().begin(); i != leep.getCaveExits().end(); i++)
        {
            renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()));
            renderPhysicalEntitiesWithColor((*i)->getCaveExitCovers());
        }
        
        for (std::vector<std::unique_ptr<Ground>>::iterator i = leep.getGrounds().begin(); i != leep.getGrounds().end(); i++)
        {
            if ((*i)->is_world_1_ground_w_cave())
            {
                renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()));
            }
        }
        
        for (std::vector<std::unique_ptr<Hole>>::iterator i = leep.getHoles().begin(); i != leep.getHoles().end(); i++)
        {
            renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()));
            renderPhysicalEntities((*i)->getHoleCovers());
        }
        m_spriteBatcher->endBatch(*m_world_1_ground_w_cave);
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<Ground>>::iterator i = leep.getGrounds().begin(); i != leep.getGrounds().end(); i++)
        {
            if ((*i)->is_world_1_ground_wo_cave())
            {
                renderPhysicalEntity(*(*i).get(), Assets::getInstance()->get(*(*i).get()));
            }
        }
        m_spriteBatcher->endBatch(*m_world_1_ground_wo_cave);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep.getJumpSprings());
        renderPhysicalEntities(leep.getCarrots());
        renderPhysicalEntities(leep.getGoldenCarrots());
        m_spriteBatcher->endBatch(*m_game_objects);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep.getPlatforms());
        renderPhysicalEntities(leep.getLogVerticalTalls());
        renderPhysicalEntities(leep.getLogVerticalShorts());
        renderPhysicalEntities(leep.getThorns());
        renderPhysicalEntities(leep.getStumps());
        renderPhysicalEntities(leep.getSideSpikes());
        renderPhysicalEntities(leep.getUpwardSpikes());
        renderPhysicalEntities(leep.getEndSigns());
        renderPhysicalEntitiesWithColor(leep.getRocks());
        m_spriteBatcher->endBatch(*m_world_1_objects);
        
        for (std::vector<std::unique_ptr<SnakeGrunt>>::iterator i = leep.getSnakeGruntEnemies().begin(); i != leep.getSnakeGruntEnemies().end(); i++)
        {
            std::unique_ptr<SnakeGrunt>& upItem = *i;
            SnakeGrunt* pItem = upItem.get();
            SnakeGrunt& item = *pItem;
            
            m_spriteBatcher->beginBatch();
            m_snakeDeathTextureProgram->setColorAdditive(item.getColorAdditive());
            renderPhysicalEntityWithColor(item, Assets::getInstance()->get(item), item.getColor());
            m_spriteBatcher->endBatch(*m_world_1_enemies, *m_snakeDeathTextureProgram);
        }
        
        for (std::vector<std::unique_ptr<SnakeHorned>>::iterator i = leep.getSnakeHornedEnemies().begin(); i != leep.getSnakeHornedEnemies().end(); i++)
        {
            std::unique_ptr<SnakeHorned>& upItem = *i;
            SnakeHorned* pItem = upItem.get();
            SnakeHorned& item = *pItem;
            
            m_spriteBatcher->beginBatch();
            m_snakeDeathTextureProgram->setColorAdditive(item.getColorAdditive());
            renderPhysicalEntityWithColor(item, Assets::getInstance()->get(item), item.getColor());
            m_spriteBatcher->endBatch(*m_world_1_enemies, *m_snakeDeathTextureProgram);
        }
        
        m_spriteBatcher->beginBatch();
        for (std::vector<std::unique_ptr<SnakeGrunt>>::iterator i = leep.getSnakeGruntEnemies().begin(); i != leep.getSnakeGruntEnemies().end(); i++)
        {
            renderPhysicalEntities((*i)->getSnakeSpirits());
        }
        
        for (std::vector<std::unique_ptr<SnakeHorned>>::iterator i = leep.getSnakeHornedEnemies().begin(); i != leep.getSnakeHornedEnemies().end(); i++)
        {
            renderPhysicalEntities((*i)->getSnakeSpirits());
        }
        m_spriteBatcher->endBatch(*m_world_1_enemies);
    }
    
    if (lsp.isOpen())
    {
        updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(lsp, Assets::getInstance()->get(lsp));
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

void Renderer::renderToSecondFramebufferWithShockwave(float centerX, float centerY, float timeElapsed, bool isTransforming)
{
    m_shockwaveTextureGpuProgramWrapper->configure(centerX, centerY, timeElapsed, isTransforming);
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    setFramebuffer(1);
    
    float x = m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth() / 2;
    float y = m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight() / 2;
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(x, y, m_camBounds->getWidth(), m_camBounds->getHeight(), 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(0), *m_shockwaveTextureGpuProgramWrapper);
}

void Renderer::renderToSecondFramebuffer(Game& game)
{
    setFramebuffer(1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    
    if (game.getJons().size() > 0)
    {
        Jon& jon = game.getJon();
        bool isVampire = jon.isVampire();
        m_spriteBatcher->endBatch(m_framebuffers.at(0), (isVampire || jon.isRevertingToRabbit()) ? *m_framebufferTintGpuProgramWrapper : *m_framebufferToScreenGpuProgramWrapper);
    }
    else
    {
        m_spriteBatcher->endBatch(m_framebuffers.at(0), *m_framebufferToScreenGpuProgramWrapper);
    }
}

void Renderer::renderToScreenWithTransDeathIn(float timeElapsed)
{
    /// Render the death transition to the screen
    
    m_transDeathInGpuProgramWrapper->configure(m_trans_death_shader_helper, timeElapsed);
    
    bindToScreenFramebuffer();
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(m_iFramebufferIndex), *m_transDeathInGpuProgramWrapper);
}

void Renderer::renderToScreenWithTransDeathOut(float timeElapsed)
{
    /// Render the new game to the screen
    
    m_transDeathOutGpuProgramWrapper->configure(m_trans_death_shader_helper, timeElapsed);
    
    bindToScreenFramebuffer();
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(m_iFramebufferIndex), *m_transDeathOutGpuProgramWrapper);
}

void Renderer::renderToScreenTitleToWorldMapTransition(float progress)
{
    /// Render the Title to World Map transition to the screen
    
    m_transTitleToWorldMapGpuProgramWrapper->configure(&m_framebuffers.at(1), progress);
    
    bindToScreenFramebuffer();
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(0), *m_transTitleToWorldMapGpuProgramWrapper);
}

void Renderer::renderToScreenWorldMapToLevelTransition(float progress)
{
    /// Render the World Map to Level transition to the screen
    
    m_transWorldMapToLevelGpuProgramWrapper->configure(&m_framebuffers.at(1), progress);
    
    bindToScreenFramebuffer();
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(0), *m_transWorldMapToLevelGpuProgramWrapper);
}

void Renderer::renderToScreenWithRadialBlur()
{
    /// Render everything to the screen
    
    bindToScreenFramebuffer();
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(m_iFramebufferIndex), *m_framebufferRadialBlurGpuProgramWrapper);
}

void Renderer::renderToScreen()
{
	/// Render everything to the screen
    
    bindToScreenFramebuffer();
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(m_iFramebufferIndex), *m_framebufferToScreenGpuProgramWrapper);
}

void Renderer::cleanUp()
{
    bool compressed = Assets::getInstance()->isUsingCompressedTextureSet();
    
    if (m_areTitleTexturesLoaded)
    {
        tearDownTexture(m_title_font);
        tearDownTexture(m_world_1_misc);
        tearDownTexture(m_world_map);
        
        m_areTitleTexturesLoaded = false;
    }
    
    if (m_areWorld1TexturesLoaded)
    {
        tearDownTexture(m_world_1_background);
        
        tearDownTexture(m_world_1_ground_w_cave);
        
        tearDownTexture(m_world_1_objects);
        
        tearDownTexture(m_jon);
        
        tearDownTexture(m_vampire);
        
        tearDownTexture(m_trans_death_shader_helper);
        
        if (!compressed)
        {
            tearDownTexture(m_world_1_ground_wo_cave);
            tearDownTexture(m_world_1_cave);
            
            tearDownTexture(m_game_objects);
            tearDownTexture(m_world_1_enemies);
            tearDownTexture(m_world_1_snake_cave);
            
            tearDownTexture(m_jon_ability);
            tearDownTexture(m_jon_poses);
            
            tearDownTexture(m_vampire_poses);
            tearDownTexture(m_vampire_transform);
        }
        
        m_areWorld1TexturesLoaded = false;
    }
    
    if (m_areLevelEditorTexturesLoaded)
    {
        tearDownTexture(m_level_editor);
        
        m_areLevelEditorTexturesLoaded = false;
    }

	m_sinWaveTextureProgram->cleanUp();
    m_snakeDeathTextureProgram->cleanUp();
    m_shockwaveTextureGpuProgramWrapper->cleanUp();
    m_framebufferTintGpuProgramWrapper->cleanUp();
}

Vector2D& Renderer::getCameraPosition()
{
    return m_camBounds->getLowerLeft();
}

#pragma mark private

void Renderer::tearDownTexture(TextureWrapper* textureWrapper)
{
    destroyTexture(*textureWrapper);
    delete textureWrapper;
    textureWrapper = nullptr;
}

void Renderer::tearDownGpuProgramWrapper(GpuProgramWrapper* gpuProgramWrapper)
{
    delete gpuProgramWrapper;
    gpuProgramWrapper = nullptr;
}

void Renderer::renderPhysicalEntity(PhysicalEntity &pe, TextureRegion& tr, bool performBoundsCheck)
{
    if (performBoundsCheck)
    {
        Rectangle renderBounds = Rectangle(pe.getPosition().getX() - pe.getWidth() / 2, pe.getPosition().getY() - pe.getHeight() / 2, pe.getWidth(), pe.getHeight());
        if (!OverlapTester::doRectanglesOverlap(*m_camBounds, renderBounds))
        {
            return;
        }
    }
    
    m_spriteBatcher->drawSprite(pe.getPosition().getX(), pe.getPosition().getY(), pe.getWidth(), pe.getHeight(), pe.getAngle(), tr);
}

void Renderer::renderPhysicalEntityWithColor(PhysicalEntity &pe, TextureRegion& tr, Color c, bool performBoundsCheck)
{
    if (performBoundsCheck)
    {
        Rectangle renderBounds = Rectangle(pe.getPosition().getX() - pe.getWidth() / 2, pe.getPosition().getY() - pe.getHeight() / 2, pe.getWidth(), pe.getHeight());
        if (!OverlapTester::doRectanglesOverlap(*m_camBounds, renderBounds))
        {
            return;
        }
    }
    
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