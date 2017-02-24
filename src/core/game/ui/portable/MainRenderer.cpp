//
//  MainRenderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainRenderer.h"

#include "SpriteBatcher.h"
#include "LineBatcher.h"
#include "NGRectBatcher.h"
#include "Font.h"
#include "TextureWrapper.h"
#include "TextureRegion.h"
#include "GpuTextureDataWrapper.h"
#include "Assets.h"
#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "NGRect.h"
#include "Game.h"
#include "GameButton.h"
#include "MainScreenLevelEditor.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "TrashCan.h"
#include "LevelSelectorPanel.h"
#include "OffsetPanel.h"
#include "ConfirmResetPanel.h"
#include "ConfirmExitPanel.h"
#include "GpuProgramWrapper.h"
#include "TransitionGpuProgramWrapper.h"
#include "PointTransitionGpuProgramWrapper.h"
#include "SinWaveTextureGpuProgramWrapper.h"
#include "SnakeDeathTextureGpuProgramWrapper.h"
#include "EndBossSnakeTextureGpuProgramWrapper.h"
#include "ShockwaveTextureGpuProgramWrapper.h"
#include "TransDeathGpuProgramWrapper.h"
#include "FramebufferRadialBlurGpuProgramWrapper.h"
#include "CollectibleItem.h"
#include "TitlePanel.h"
#include "CutscenePanel.h"
#include "WorldMapPanel.h"
#include "MainScreenWorldMap.h"
#include "macros.h"
#include "BatPanel.h"
#include "MidBossOwl.h"
#include "FlagUtil.h"
#include "MathUtil.h"
#include "MainScreen.h"
#include "GameTracker.h"
#include "EndBossSnake.h"
#include "MainAssetsMapper.h"
#include "IRendererHelper.h"
#include "Animation.h"
#include "IAssetsMapper.h"
#include "MainGpuProgramWrapperFactory.h"

#include <math.h>
#include <sstream>
#include <iomanip>

MainRenderer::MainRenderer() : Renderer(),
m_font(new Font("misc", 0, 0, 16, 64, 75, TEXTURE_SIZE_1024)),
m_jon(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_jon" : "jon")),
m_level_editor(new TextureWrapper("level_editor")),
m_misc(new TextureWrapper("misc")),
m_title_screen(new TextureWrapper("title_screen")),
m_trans_death_shader_helper(new TextureWrapper("trans_death_shader_helper")),
m_tutorial(new TextureWrapper(MAIN_ASSETS->isUsingGamePadTextureSet() ? "game_pad_tutorial" : MAIN_ASSETS->isUsingDesktopTextureSet() ? "d_tutorial" : MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_tutorial" : "tutorial")),
m_vampire(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_vampire" : "vampire")),
m_world_1_background_lower_part_1(new TextureWrapper("world_1_background_lower_part_1", 1)),
m_world_1_background_lower_part_2(new TextureWrapper("world_1_background_lower_part_2", 1)),
m_world_1_background_mid(new TextureWrapper("world_1_background_mid", 1)),
m_world_1_background_upper(new TextureWrapper("world_1_background_upper", 1)),
m_world_1_cutscene_1(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_cutscene_1" : "world_1_cutscene_1")),
m_world_1_cutscene_2(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_cutscene_2" : "world_1_cutscene_2")),
m_world_1_end_boss_part_1(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_end_boss_part_1" : "world_1_end_boss_part_1")),
m_world_1_end_boss_part_2(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_end_boss_part_2" : "world_1_end_boss_part_2")),
m_world_1_end_boss_part_3(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_end_boss_part_3" : "world_1_end_boss_part_3")),
m_world_1_enemies(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_enemies" : "world_1_enemies")),
m_world_1_ground(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_ground" : "world_1_ground")),
m_world_1_mid_boss_part_1(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_mid_boss_part_1" : "world_1_mid_boss_part_1")),
m_world_1_mid_boss_part_2(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_mid_boss_part_2" : "world_1_mid_boss_part_2")),
m_world_1_mid_boss_part_3(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_mid_boss_part_3" : "world_1_mid_boss_part_3")),
m_world_1_objects_part_1(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_objects_part_1" : "world_1_objects_part_1")),
m_world_1_objects_part_2(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_objects_part_2" : "world_1_objects_part_2")),
m_world_1_special(new TextureWrapper(MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_world_1_special" : "world_1_special")),
m_world_map_screen_part_1(new TextureWrapper("world_map_screen_part_1")),
m_world_map_screen_part_2(new TextureWrapper("world_map_screen_part_2")),
m_transScreenGpuProgramWrapper(nullptr),
m_fadeScreenGpuProgramWrapper(nullptr),
m_pointTransScreenGpuProgramWrapper(nullptr),
m_sinWaveTextureProgram(nullptr),
m_backgroundGpuTextureProgramWrapper(nullptr),
m_snakeDeathTextureProgram(nullptr),
m_endBossSnakeTextureProgram(nullptr),
m_shockwaveTextureGpuProgramWrapper(nullptr),
m_transDeathInGpuProgramWrapper(nullptr),
m_transDeathOutGpuProgramWrapper(nullptr),
m_framebufferTintGpuProgramWrapper(nullptr),
m_framebufferObfuscationGpuProgramWrapper(nullptr),
m_framebufferRadialBlurGpuProgramWrapper(nullptr),
m_camBounds(new NGRect(0, 0, CAM_WIDTH, CAM_HEIGHT)),
m_camPosVelocity(new Vector2D()),
m_fStateTime(0),
m_fCamPosX(0),
m_fGroundedCamY(0),
m_fLowestGroundedCamY(1337),
m_fLastKnownCamY(0),
m_fRadialBlurDirection(0.5f),
m_stopCamera(false),
m_hasCompletedRadialBlur(false)
{
    ASSETS->init(new MainAssetsMapper());
}

MainRenderer::~MainRenderer()
{
    releaseDeviceDependentResources();
    
    delete m_font;
    
    delete m_jon;
    delete m_level_editor;
    delete m_misc;
    delete m_title_screen;
    delete m_trans_death_shader_helper;
    delete m_tutorial;
    delete m_vampire;
    delete m_world_1_background_lower_part_1;
    delete m_world_1_background_lower_part_2;
    delete m_world_1_background_mid;
    delete m_world_1_background_upper;
    delete m_world_1_cutscene_1;
    delete m_world_1_cutscene_2;
    delete m_world_1_end_boss_part_1;
    delete m_world_1_end_boss_part_2;
    delete m_world_1_end_boss_part_3;
    delete m_world_1_enemies;
    delete m_world_1_ground;
    delete m_world_1_mid_boss_part_1;
    delete m_world_1_mid_boss_part_2;
    delete m_world_1_mid_boss_part_3;
    delete m_world_1_objects_part_1;
    delete m_world_1_objects_part_2;
    delete m_world_1_special;
    delete m_world_map_screen_part_1;
    delete m_world_map_screen_part_2;
    
    delete m_camBounds;
    delete m_camPosVelocity;
}

void MainRenderer::createDeviceDependentResources()
{
    Renderer::createDeviceDependentResources();
    
    m_transScreenGpuProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createTransScreenGpuProgramWrapper();
    m_fadeScreenGpuProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createFadeScreenGpuProgramWrapper();
    m_pointTransScreenGpuProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createPointTransScreenGpuProgramWrapper();
    m_sinWaveTextureProgram = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createSinWaveTextureProgram();
    m_backgroundGpuTextureProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createBackgroundGpuTextureProgramWrapper();
    m_snakeDeathTextureProgram = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createSnakeDeathTextureProgram();
    m_endBossSnakeTextureProgram = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createEndBossSnakeTextureProgram();
    m_shockwaveTextureGpuProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createShockwaveTextureGpuProgramWrapper();
    m_transDeathInGpuProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createTransDeathInGpuProgramWrapper();
    m_transDeathOutGpuProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createTransDeathOutGpuProgramWrapper();
    m_framebufferTintGpuProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createFramebufferTintGpuProgramWrapper();
    m_framebufferObfuscationGpuProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createFramebufferObfuscationGpuProgramWrapper();
    m_framebufferRadialBlurGpuProgramWrapper = MAIN_GPU_PROGRAM_WRAPPER_FACTORY->createFramebufferRadialBlurGpuProgramWrapper();
    
    loadTextureSync(m_misc);
}

void MainRenderer::releaseDeviceDependentResources()
{
    Renderer::releaseDeviceDependentResources();
    
    unloadTexture(m_jon);
    unloadTexture(m_level_editor);
    unloadTexture(m_misc);
    unloadTexture(m_title_screen);
    unloadTexture(m_trans_death_shader_helper);
    unloadTexture(m_tutorial);
    unloadTexture(m_vampire);
    unloadTexture(m_world_1_background_lower_part_1);
    unloadTexture(m_world_1_background_lower_part_2);
    unloadTexture(m_world_1_background_mid);
    unloadTexture(m_world_1_background_upper);
    unloadTexture(m_world_1_cutscene_1);
    unloadTexture(m_world_1_cutscene_2);
    unloadTexture(m_world_1_end_boss_part_1);
    unloadTexture(m_world_1_end_boss_part_2);
    unloadTexture(m_world_1_end_boss_part_3);
    unloadTexture(m_world_1_enemies);
    unloadTexture(m_world_1_ground);
    unloadTexture(m_world_1_mid_boss_part_1);
    unloadTexture(m_world_1_mid_boss_part_2);
    unloadTexture(m_world_1_mid_boss_part_3);
    unloadTexture(m_world_1_objects_part_1);
    unloadTexture(m_world_1_objects_part_2);
    unloadTexture(m_world_1_special);
    unloadTexture(m_world_map_screen_part_1);
    unloadTexture(m_world_map_screen_part_2);
    
    delete m_transScreenGpuProgramWrapper;
    delete m_fadeScreenGpuProgramWrapper;
    delete m_pointTransScreenGpuProgramWrapper;
    delete m_sinWaveTextureProgram;
    delete m_backgroundGpuTextureProgramWrapper;
    delete m_snakeDeathTextureProgram;
    delete m_endBossSnakeTextureProgram;
    delete m_shockwaveTextureGpuProgramWrapper;
    delete m_transDeathInGpuProgramWrapper;
    delete m_transDeathOutGpuProgramWrapper;
    delete m_framebufferTintGpuProgramWrapper;
    delete m_framebufferObfuscationGpuProgramWrapper;
    delete m_framebufferRadialBlurGpuProgramWrapper;
}

void MainRenderer::load(RendererType rendererType)
{
    m_tutorial->name = MAIN_ASSETS->isUsingGamePadTextureSet() ? "game_pad_tutorial" : MAIN_ASSETS->isUsingDesktopTextureSet() ? "d_tutorial" : MAIN_ASSETS->isUsingCompressedTextureSet() ? "c_tutorial" : "tutorial";
    
    switch (rendererType)
    {
        case RENDERER_TYPE_TITLE:
            loadTextureAsync(m_title_screen);
            break;
        case RENDERER_TYPE_WORLD_MAP:
            loadTextureAsync(m_world_map_screen_part_1);
            loadTextureAsync(m_world_map_screen_part_2);
            break;
        case RENDERER_TYPE_LEVEL_EDITOR:
            loadTextureAsync(m_level_editor);
            break;
            
        case RENDERER_TYPE_WORLD_1_CUTSCENE:
            loadTextureAsync(m_world_1_cutscene_1);
            loadTextureAsync(m_world_1_cutscene_2);
            break;
            
        case RENDERER_TYPE_WORLD_1:
            loadTextureAsync(m_world_1_background_lower_part_1);
            loadTextureAsync(m_world_1_background_lower_part_2);
            loadTextureAsync(m_world_1_background_mid);
            loadTextureAsync(m_world_1_background_upper);
            loadTextureAsync(m_world_1_enemies);
            loadTextureAsync(m_world_1_ground);
            loadTextureAsync(m_world_1_objects_part_1);
            loadTextureAsync(m_world_1_objects_part_2);
            loadTextureAsync(m_world_1_special);
            loadTextureAsync(m_jon);
            loadTextureAsync(m_trans_death_shader_helper);
            loadTextureAsync(m_vampire);
            loadTextureAsync(m_tutorial);
            break;
        case RENDERER_TYPE_WORLD_1_MID_BOSS:
            loadTextureAsync(m_world_1_background_lower_part_1);
            loadTextureAsync(m_world_1_background_lower_part_2);
            loadTextureAsync(m_world_1_background_mid);
            loadTextureAsync(m_world_1_background_upper);
            loadTextureAsync(m_world_1_ground);
            loadTextureAsync(m_world_1_objects_part_1);
            loadTextureAsync(m_world_1_objects_part_2);
            loadTextureAsync(m_jon);
            loadTextureAsync(m_trans_death_shader_helper);
            loadTextureAsync(m_vampire);
            loadTextureAsync(m_tutorial);
            loadTextureAsync(m_world_1_mid_boss_part_1);
            loadTextureAsync(m_world_1_mid_boss_part_2);
            loadTextureAsync(m_world_1_mid_boss_part_3);
            break;
        case RENDERER_TYPE_WORLD_1_END_BOSS:
            loadTextureAsync(m_world_1_background_lower_part_1);
            loadTextureAsync(m_world_1_background_lower_part_2);
            loadTextureAsync(m_world_1_background_mid);
            loadTextureAsync(m_world_1_background_upper);
            loadTextureAsync(m_world_1_ground);
            loadTextureAsync(m_world_1_objects_part_1);
            loadTextureAsync(m_world_1_objects_part_2);
            loadTextureAsync(m_jon);
            loadTextureAsync(m_trans_death_shader_helper);
            loadTextureAsync(m_vampire);
            loadTextureAsync(m_tutorial);
            loadTextureAsync(m_world_1_end_boss_part_1);
            loadTextureAsync(m_world_1_end_boss_part_2);
            loadTextureAsync(m_world_1_end_boss_part_3);
            break;

		case RENDERER_TYPE_NONE:
        default:
            break;
    }
}

void MainRenderer::unload(RendererType rendererType)
{
    switch (rendererType)
    {
        case RENDERER_TYPE_TITLE:
            unloadTexture(m_title_screen);
            break;
        case RENDERER_TYPE_WORLD_MAP:
            unloadTexture(m_world_map_screen_part_1);
            unloadTexture(m_world_map_screen_part_2);
            break;
        case RENDERER_TYPE_LEVEL_EDITOR:
            unloadTexture(m_level_editor);
            break;
            
        case RENDERER_TYPE_WORLD_1_CUTSCENE:
            unloadTexture(m_world_1_cutscene_1);
            unloadTexture(m_world_1_cutscene_2);
            break;
            
        case RENDERER_TYPE_WORLD_1:
            unloadTexture(m_world_1_background_lower_part_1);
            unloadTexture(m_world_1_background_lower_part_2);
            unloadTexture(m_world_1_background_mid);
            unloadTexture(m_world_1_background_upper);
            unloadTexture(m_world_1_enemies);
            unloadTexture(m_world_1_ground);
            unloadTexture(m_world_1_objects_part_1);
            unloadTexture(m_world_1_objects_part_2);
            unloadTexture(m_world_1_special);
            unloadTexture(m_jon);
            unloadTexture(m_trans_death_shader_helper);
            unloadTexture(m_vampire);
            unloadTexture(m_tutorial);
            break;
        case RENDERER_TYPE_WORLD_1_MID_BOSS:
            unloadTexture(m_world_1_background_lower_part_1);
            unloadTexture(m_world_1_background_lower_part_2);
            unloadTexture(m_world_1_background_mid);
            unloadTexture(m_world_1_background_upper);
            unloadTexture(m_world_1_ground);
            unloadTexture(m_world_1_objects_part_1);
            unloadTexture(m_world_1_objects_part_2);
            unloadTexture(m_jon);
            unloadTexture(m_trans_death_shader_helper);
            unloadTexture(m_vampire);
            unloadTexture(m_tutorial);
            unloadTexture(m_world_1_mid_boss_part_1);
            unloadTexture(m_world_1_mid_boss_part_2);
            unloadTexture(m_world_1_mid_boss_part_3);
            break;
        case RENDERER_TYPE_WORLD_1_END_BOSS:
            unloadTexture(m_world_1_background_lower_part_1);
            unloadTexture(m_world_1_background_lower_part_2);
            unloadTexture(m_world_1_background_mid);
            unloadTexture(m_world_1_background_upper);
            unloadTexture(m_world_1_ground);
            unloadTexture(m_world_1_objects_part_1);
            unloadTexture(m_world_1_objects_part_2);
            unloadTexture(m_jon);
            unloadTexture(m_trans_death_shader_helper);
            unloadTexture(m_vampire);
            unloadTexture(m_tutorial);
            unloadTexture(m_world_1_end_boss_part_1);
            unloadTexture(m_world_1_end_boss_part_2);
            unloadTexture(m_world_1_end_boss_part_3);
            break;
            
        case RENDERER_TYPE_NONE:
        default:
            break;
    }
}

void MainRenderer::beginFrame()
{
    Renderer::beginFrame();
    
    setFramebuffer(0);
}

void MainRenderer::beginOpeningPanningSequence(Game& game)
{
    m_stopCamera = false;
    
    zoomIn();
    
    m_fStateTime = 0;
    m_hasCompletedRadialBlur = false;
    
    updateCameraToFollowJon(game, nullptr, 1337);
    
    m_camBounds->getLowerLeft().setX(game.getCamFarRight());
    m_camBounds->getLowerLeft().setY(game.getCamFarRightBottom());
    
    Jon& jon = game.getJon();
    float farLeft = jon.getPosition().getX() - CAM_WIDTH / 6;
	m_fGroundedCamY = jon.getMainBounds().getBottom() - 0.8f;
    m_fLowestGroundedCamY = fminf(m_fGroundedCamY, m_fLowestGroundedCamY);
    m_fLastKnownCamY = m_fGroundedCamY;
    
    float changeInX = farLeft - game.getCamFarRight();
    float changeInY = m_fGroundedCamY - game.getCamFarRightBottom();
    
    m_camPosVelocity->set(changeInX, changeInY);
    
    Vector2D camPosVelocityNormal = m_camPosVelocity->cpy().nor();
    float angle = camPosVelocityNormal.angle();
    float tanfAngle = tanf(DEGREES_TO_RADIANS(angle));
    float y = tanfAngle * camPosVelocityNormal.getX();
    m_fRadialBlurDirection = y / 2.0f + 0.5f;
}

int MainRenderer::updateCameraToFollowPathToJon(Game& game, float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if (m_fStateTime >= 4.10f)
    {
        return 3;
    }
    else if (m_fStateTime >= 1.90f)
    {
        bool isComplete = false;
        float progress = (m_fStateTime - 1.90f) / 0.2f;
        if (progress >= 1)
        {
            progress = 1;
            isComplete = true;
        }
        
        Jon& jon = game.getJon();
        float farLeft = jon.getPosition().getX() - CAM_WIDTH / 6;
        m_fGroundedCamY = jon.getMainBounds().getBottom() - 0.8f;
        m_fLowestGroundedCamY = fminf(m_fGroundedCamY, m_fLowestGroundedCamY);
        m_fLastKnownCamY = m_fGroundedCamY;
        
        float changeInX = farLeft - game.getCamFarRight();
        float changeInY = m_fGroundedCamY - game.getCamFarRightBottom();
        
        m_camBounds->getLowerLeft().set(game.getCamFarRight() + changeInX * progress, game.getCamFarRightBottom() + changeInY * progress);

        if (m_camBounds->getLeft() < farLeft)
        {
            m_camBounds->getLowerLeft().setX(farLeft);
        }
        
        if ((m_camPosVelocity->getY() < 0 && m_camBounds->getBottom() < m_fGroundedCamY)
            || (m_camPosVelocity->getY() > 0 && m_camBounds->getBottom() > m_fGroundedCamY))
        {
            m_camBounds->getLowerLeft().setY(m_fGroundedCamY);
        }
        
        if (isComplete)
        {
            updateCameraToFollowJon(game, nullptr, 1337);
            
            if (!m_hasCompletedRadialBlur)
            {
                m_hasCompletedRadialBlur = true;
                
                return 2;
            }
        }
        else
        {
            return 1;
        }
    }
    
    return 0;
}

void MainRenderer::updateCameraToFollowJon(Game& game, BatPanel* batPanel, float deltaTime, float paddingX, bool chase, bool ignoreY, bool instant)
{
    bool isFalling;
    bool isGrounded;
    float yFactor;
    PhysicalEntity* entity;
    Jon& jon = game.getJon();
    if (batPanel && batPanel->isRequestingInput())
    {
        isFalling = false;
        isGrounded = false;
        entity = batPanel->getBat();
        yFactor = entity->getMainBounds().getBottom() + 1;
    }
    else
    {
        isFalling = jon.isFalling();
        isGrounded = jon.getPhysicalState() == PHYSICAL_GROUNDED;
        entity = game.getJonP();
        yFactor = entity->getMainBounds().getBottom() - 0.5625f;
    }
    
    float idealCamX = jon.getPosition().getX() - CAM_WIDTH / 6 + paddingX;
    if (chase)
    {
        idealCamX = jon.getPosition().getX() - CAM_WIDTH * 5 / 6 + paddingX;
    }
    
    float camVelocityX = idealCamX - m_camBounds->getLowerLeft().getX();
    camVelocityX = camVelocityX < 0 ? camVelocityX * 2 : camVelocityX * 2 + entity->getVelocity().getX();
    camVelocityX *= instant ? 1337 : deltaTime;
    m_camBounds->getLowerLeft().add(camVelocityX, 0);
    
    if (camVelocityX < 0 && m_camBounds->getLowerLeft().getX() < idealCamX)
    {
        m_camBounds->getLowerLeft().setX(idealCamX);
    }
    else if (camVelocityX > 0 && m_camBounds->getLowerLeft().getX() > idealCamX)
    {
        m_camBounds->getLowerLeft().setX(idealCamX);
    }
    
    float heightPlusPadding = entity->getHeight() * 1.5f;
    
    float regionBottomY;
    if (isFalling)
    {
        if (yFactor < 6.9979978125f)
        {
            regionBottomY = 0;
        }
        else if (yFactor < 12.76362286085f)
        {
            regionBottomY = 6.9979978125f;
        }
        else if (yFactor < 21.265625f)
        {
            regionBottomY = 12.76362286085f;
        }
        else if (yFactor < 28)
        {
            regionBottomY = 21.265625f;
        }
        else
        {
            regionBottomY = GAME_HEIGHT - CAM_HEIGHT;
        }
    }
    else
    {
        if (yFactor < (6.9979978125f - heightPlusPadding))
        {
            regionBottomY = 0;
        }
        else if (yFactor < (12.76362286085 - heightPlusPadding))
        {
            regionBottomY = 6.9979978125f;
        }
        else if (yFactor < (21.265625f - heightPlusPadding))
        {
            regionBottomY = 12.76362286085f;
        }
        else if (yFactor < (28 - heightPlusPadding))
        {
            regionBottomY = 21.265625f;
        }
        else
        {
            regionBottomY = GAME_HEIGHT - CAM_HEIGHT;
        }
        
        if (regionBottomY > yFactor)
        {
            regionBottomY = yFactor;
        }
    }
    
    if (batPanel && batPanel->isRequestingInput())
    {
        Jon& jon = game.getJon();
        float jy = jon.getMainBounds().getBottom() - 0.5625f;
        if (regionBottomY > jy)
        {
            regionBottomY = jy;
        }
    }
    
    float camSpeed = regionBottomY - m_camBounds->getLowerLeft().getY();
    float camVelocityY = regionBottomY > m_camBounds->getLowerLeft().getY() ? camSpeed : regionBottomY == m_camBounds->getLowerLeft().getY() ? 0 : camSpeed * 4;
    if (ignoreY)
    {
        camVelocityY = 0;
    }
    
    m_camBounds->getLowerLeft().add(0, camVelocityY * deltaTime);
    
    if (camVelocityY > 0)
    {
        if (!isGrounded)
        {
            float newCamPos = yFactor + heightPlusPadding - CAM_HEIGHT;
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
    
    if (m_stopCamera && m_camBounds->getLowerLeft().getX() > m_fCamPosX)
    {
        m_camBounds->getLowerLeft().setX(m_fCamPosX);
    }
}

void MainRenderer::moveCamera(float x)
{
    m_camBounds->getLowerLeft().add(x, 0);
}

void MainRenderer::stopCamera()
{
	m_fCamPosX = m_camBounds->getLeft();
    m_stopCamera = true;
}

void MainRenderer::zoomOut()
{
    m_camBounds->getLowerLeft().set(0, 0);
    m_camBounds->setWidth(ZOOMED_OUT_CAM_WIDTH);
    m_camBounds->setHeight(GAME_HEIGHT);
}

void MainRenderer::zoomIn()
{
    m_camBounds->getLowerLeft().set(0, 0);
    m_camBounds->setWidth(CAM_WIDTH);
    m_camBounds->setHeight(CAM_HEIGHT);
}

void MainRenderer::renderTitleScreenBackground(TitlePanel* panel)
{
    if (!ensureTexture(m_title_screen))
    {
        return;
    }
    
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    
    renderPhysicalEntity(*panel, MAIN_ASSETS->get(panel));
    renderPhysicalEntity(*panel->getCastle(), MAIN_ASSETS->get(panel->getCastle()));
    renderPhysicalEntity(*panel->getBigCloud(), MAIN_ASSETS->get(panel->getBigCloud()));
    renderPhysicalEntity(*panel->getSmallCloud(), MAIN_ASSETS->get(panel->getSmallCloud()));
    
    if (panel->isLightningStriking())
    {
        renderPhysicalEntity(*panel->getLightning(), MAIN_ASSETS->get(panel->getLightning()));
    }
    
    renderPhysicalEntity(*panel->getNosfuratuLogo(), MAIN_ASSETS->get(panel->getNosfuratuLogo()));
    
    if (panel->isLightningStriking())
    {
        renderPhysicalEntity(*panel->getNosfuratuLogoLightEffect(), MAIN_ASSETS->get(panel->getNosfuratuLogoLightEffect()));
        renderPhysicalEntity(*panel->getCastleLightEffect(), MAIN_ASSETS->get(panel->getCastleLightEffect()));
    }
    
    m_spriteBatcher->endBatch(*m_title_screen->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderTitleScreenUi(GameButton* levelEditorButton)
{
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
#if NG_LEVEL_EDITOR
    if (ensureTexture(m_title_screen))
    {
        m_spriteBatcher->beginBatch();
        
        renderPhysicalEntity(*levelEditorButton, MAIN_ASSETS->get(levelEditorButton));
        
        m_spriteBatcher->endBatch(*m_title_screen->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
#endif
    
    static Color fontColor = Color(0.7f, 0.7f, 0.7f, 1);
    static float fgWidth = CAM_WIDTH / 60;
    static float fgHeight = fgWidth * 1.171875f;
    
    m_spriteBatcher->beginBatch();
    
    {
        /// Version
        std::stringstream ss;
        ss << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_BUILD;
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, fgWidth * 3 / 4, fgHeight * 3 / 4, fgWidth, fgHeight, fontColor, false, false);
    }
    
    {
        /// Company Info
        std::stringstream ss;
        ss << "2016 Noctis Games";
        std::string text = ss.str();
        
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH - fgWidth * 3 / 4, fgHeight * 3 / 4, fgWidth, fgHeight, fontColor, false, true);
        
        int copyrightSymbol = 169; // 169 is the ASCII value for ©
        m_font->renderAsciiChar(*m_spriteBatcher, copyrightSymbol, CAM_WIDTH - fgWidth * 3 / 4 - text.length() * fgWidth, fgHeight * 3 / 4, fgWidth, fgHeight, fontColor);
    }
    
    m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderCutscene(std::vector<CutscenePanel*> cutscenePanels)
{
    if (!(ensureTexture(m_world_1_cutscene_1)
          && ensureTexture(m_world_1_cutscene_2)))
    {
        return;
    }
    
    for (std::vector<CutscenePanel *>::iterator i = cutscenePanels.begin(); i != cutscenePanels.end(); i++)
    {
        m_spriteBatcher->beginBatch();
        
        NGRect& cb = (*i)->getCamBounds();
        m_rendererHelper->updateMatrix(cb.getLeft(), cb.getWidth(), cb.getBottom(), cb.getHeight());
        
        renderPhysicalEntityWithColor(*(*i), MAIN_ASSETS->get((*i)), (*i)->getColor());
        
        m_spriteBatcher->endBatch(*m_world_1_cutscene_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        m_spriteBatcher->beginBatch();
        
        renderPhysicalEntitiesWithColor((*i)->getCutsceneEffects());
        
        m_spriteBatcher->endBatch(*m_world_1_cutscene_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
}

void MainRenderer::renderWorldMapScreenBackground(WorldMapPanel* panel)
{
    if (!(ensureTexture(m_world_map_screen_part_1)
          && ensureTexture(m_world_map_screen_part_2)))
    {
        return;
    }
    
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*panel, MAIN_ASSETS->get(panel));
    m_spriteBatcher->endBatch(*m_world_map_screen_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderWorldMapScreenUi(WorldMap& wm)
{
    if (!(ensureTexture(m_world_map_screen_part_1)
         && ensureTexture(m_world_map_screen_part_2)))
    {
        return;
    }
    
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntitiesWithColor(wm.getAbilitySlots());
    renderPhysicalEntitiesWithColor(wm.getLevelThumbnails());
    renderPhysicalEntityWithColor(*wm.getGoldenCarrotsMarker(), MAIN_ASSETS->get(wm.getGoldenCarrotsMarker()), wm.getGoldenCarrotsMarker()->getColor());
    m_spriteBatcher->endBatch(*m_world_map_screen_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    
    static float fgWidth = CAM_WIDTH / 40;
    static float fgHeight = fgWidth * 1.171875f;
    
    m_spriteBatcher->beginBatch();
    
    {
        static Color fontColor = Color(1, 1, 1, 1);
        
        fontColor.alpha = 1;
        
        float flicker = wm.getGoldenCarrotCountFlickerTime();
        if (flicker < 0.5f)
        {
            fontColor.alpha = 1 - flicker * 2;
        }
        else if (flicker < 1)
        {
            fontColor.alpha = (flicker - 0.5f) * 2;
        }
        
        std::stringstream ss;
        ss << wm.getNumCollectedGoldenCarrots();
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, CAM_HEIGHT * 0.79084967320261f, fgWidth, fgHeight, fontColor, true);
    }
    
    {
        std::stringstream ss;
        
        ScoreMarker* sm = wm.getScoreMarker();
        // the number is converted to string with the help of stringstream
        ss << sm->getScore();
        std::string paddedScore;
        ss >> paddedScore;
        
        // Append zero chars
        unsigned long str_length = paddedScore.length();
        for (int i = 0; i < 6 - str_length; i++)
        {
            paddedScore = "0" + paddedScore;
        }
        
        m_font->renderText(*m_spriteBatcher, paddedScore, sm->getX(), sm->getY(), fgWidth, fgHeight, sm->getColor(), true);
    }
    
    LevelThumbnail* lt;
    if ((lt = wm.getSelectedLevelThumbnail()))
    {
        static Color fontColor = Color(1, 1, 1, 1);
        
        std::stringstream ss;
        ss << lt->getWorld() << " - " << lt->getLevel();
        std::string text = ss.str();
        
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, CAM_HEIGHT * .92f, fgWidth, fgHeight, fontColor, true);
    }
    
    m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderWorldMapScreenButtons(WorldMap& wm)
{
    if (!(ensureTexture(m_world_map_screen_part_1)
          && ensureTexture(m_world_map_screen_part_2)))
    {
        return;
    }
    
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntityWithColor(*wm.getBackButton(), MAIN_ASSETS->get(wm.getBackButton()), wm.getBackButton()->getColor());
    renderPhysicalEntityWithColor(*wm.getToggleMusicButton(), MAIN_ASSETS->get(wm.getToggleMusicButton()), wm.getToggleMusicButton()->getColor());
    renderPhysicalEntityWithColor(*wm.getToggleSoundButton(), MAIN_ASSETS->get(wm.getToggleSoundButton()), wm.getToggleSoundButton()->getColor());
    //renderPhysicalEntityWithColor(*wm.getLeaderBoardsButton(), MAIN_ASSETS->get(wm.getLeaderBoardsButton()), wm.getLeaderBoardsButton()->getColor());
    renderPhysicalEntityWithColor(*wm.getViewOpeningCutsceneButton(), MAIN_ASSETS->get(wm.getViewOpeningCutsceneButton()), wm.getViewOpeningCutsceneButton()->getColor());
    renderPhysicalEntityWithColor(*wm.getSpendGoldenCarrotsBubble(), MAIN_ASSETS->get(wm.getSpendGoldenCarrotsBubble()), wm.getSpendGoldenCarrotsBubble()->getColor());
    m_spriteBatcher->endBatch(*m_world_map_screen_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderWorld(Game& game)
{
    if (!(ensureTexture(m_world_1_background_lower_part_1)
          && ensureTexture(m_world_1_background_lower_part_2)
          && ensureTexture(m_world_1_background_mid)
          && ensureTexture(m_world_1_background_upper)
          && ensureTexture(m_world_1_ground)
          && ensureTexture(m_world_1_objects_part_1)
          && ensureTexture(m_world_1_objects_part_2)))
    {
        return;
    }
    
    /// Render Background
    
    m_rendererHelper->updateMatrix(0, m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundUppers());
    m_spriteBatcher->endBatch(*m_world_1_background_upper->gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundMids());
    m_spriteBatcher->endBatch(*m_world_1_background_mid->gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    
    if (game.getLevel() >= 10)
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getBackgroundLowerBacks());
        m_spriteBatcher->endBatch(*m_world_1_background_lower_part_2->gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    }
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundLowers());
    m_spriteBatcher->endBatch(*m_world_1_background_lower_part_1->gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    
    /// Render Midground
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getMidgrounds());
    m_spriteBatcher->endBatch(*m_world_1_objects_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    
    /// Render Exit Ground
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    for (std::vector<ExitGround *>::iterator i = game.getExitGrounds().begin(); i != game.getExitGrounds().end(); i++)
    {
        renderPhysicalEntity(*(*i), MAIN_ASSETS->get((*i)));
        if ((*i)->hasCover())
        {
            ExitGroundCover& egc = (*i)->getExitCover();
            renderPhysicalEntityWithColor(egc, MAIN_ASSETS->get(&egc), egc.getColor());
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_ground->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    
    if (game.getLevel() >= 10)
    {
        /// Render Background Midground Cover
        
        m_rendererHelper->updateMatrix(0, m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getBackgroundMidgroundCovers());
        m_spriteBatcher->endBatch(*m_world_1_background_lower_part_2->gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    }
    
    /// Render World
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getGrounds());
    m_spriteBatcher->endBatch(*m_world_1_ground->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    
    if (game.getLevel() < 10
        && ensureTexture(m_world_1_special))
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getPits());
        m_spriteBatcher->endBatch(*m_world_1_special->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Hole *>::iterator i = game.getHoles().begin(); i != game.getHoles().end(); i++)
    {
        renderPhysicalEntity(*(*i), MAIN_ASSETS->get((*i)));
        if ((*i)->hasCover())
        {
            HoleCover& hc = (*i)->getHoleCover();
            renderPhysicalEntity(hc, MAIN_ASSETS->get(&hc));
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_objects_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<ExtraForegroundObject *>::iterator i = game.getExtraForegroundObjects().begin(); i != game.getExtraForegroundObjects().end(); i++)
    {
        ForegroundObject& shadow = (*i)->getShadow();
        renderPhysicalEntity(shadow, MAIN_ASSETS->get(&shadow));
    }
    m_spriteBatcher->endBatch(*m_world_1_objects_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    
    m_spriteBatcher->beginBatch();
    
    for (std::vector<CollectibleItem *>::iterator i = game.getCollectibleItems().begin(); i != game.getCollectibleItems().end(); i++)
    {
        CollectibleItem* pItem = *i;
        CollectibleItem& item = *pItem;
        renderPhysicalEntityWithColor(item, MAIN_ASSETS->get(pItem), item.getColor());
        
        if (item.getType() == CollectibleItemType_GoldenCarrot)
        {
            GoldenCarrot* gc = reinterpret_cast<GoldenCarrot *>((*i));
            
            GoldenCarrotTwinkle& twinkle = gc->getGoldenCarrotTwinkle();
            renderPhysicalEntity(twinkle, MAIN_ASSETS->get(&twinkle));
        }
    }
    
    for (std::vector<ForegroundObject *>::iterator i = game.getForegroundObjects().begin(); i != game.getForegroundObjects().end(); i++)
    {
        if ((*i)->getType() == ForegroundObjectType_Floating_Platform)
        {
            FloatingPlatformObject* fpo = reinterpret_cast<FloatingPlatformObject *>((*i));
            
            if (fpo->isIdle())
            {
                PhysicalEntity& pe = fpo->getIdlePoof();
                static Animation anim = ASSETS->findAnimation("FloatingPlatformIdlePoof");
                TextureRegion tr = anim.getTextureRegion(pe.getStateTime());
                m_spriteBatcher->drawSprite(pe.getPosition().getX(), pe.getPosition().getY(), pe.getWidth(), pe.getHeight(), 0, tr);
            }
            else if (fpo->isWeighted())
            {
                PhysicalEntity& pe = fpo->getAddedWeightPoof();
                static Animation anim = ASSETS->findAnimation("FloatingPlatformWeightedPoof");
                TextureRegion tr = anim.getTextureRegion(pe.getStateTime());
                m_spriteBatcher->drawSprite(pe.getPosition().getX(), pe.getPosition().getY(), pe.getWidth(), pe.getHeight(), 0, tr);
            }
        }
        
        ForegroundObject* pItem = *i;
        ForegroundObject& item = *pItem;
        renderPhysicalEntity(item, MAIN_ASSETS->get(pItem));
    }
    
    m_spriteBatcher->endBatch(*m_world_1_objects_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    
    if (game.getLevel() == 10
        && ensureTexture(m_world_1_mid_boss_part_3))
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getMidBossForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_3->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
    
    if (game.getLevel() == 21
        && ensureTexture(m_world_1_end_boss_part_1))
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntitiesWithColor(game.getEndBossForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
    
    if (ensureTexture(m_world_1_enemies))
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntitiesWithColor(game.getEnemies());
        m_spriteBatcher->endBatch(*m_world_1_enemies->gpuTextureWrapper, *m_snakeDeathTextureProgram);
        
        m_spriteBatcher->beginBatch();
        for (std::vector<Enemy *>::iterator i = game.getEnemies().begin(); i != game.getEnemies().end(); i++)
        {
            if ((*i)->hasSpirit())
            {
                EnemySpirit& spirit = (*i)->getSpirit();
                renderPhysicalEntity(spirit, MAIN_ASSETS->get(&spirit));
            }
        }
        m_spriteBatcher->endBatch(*m_world_1_enemies->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }

    if (game.getLevel() == 21)
    {
        for (std::vector<EndBossSnake *>::iterator i = game.getEndBossSnakes().begin(); i != game.getEndBossSnakes().end(); i++)
        {
            renderEndBossSnake(*(*i));
        }
    }
}

void MainRenderer::renderJonAndExtraForegroundObjects(Game& game)
{
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    if (game.getJons().size() > 0
        && ensureTexture(m_jon)
        && ensureTexture(m_vampire))
    {
        Jon& jon = game.getJon();
        bool isTransforming = jon.isTransformingIntoVampire() || jon.isRevertingToRabbit();
        bool isVampire = jon.isVampire();
        
        /// Render Jon Effects (e.g. Dust Clouds)
        
        m_spriteBatcher->beginBatch();
        for (std::vector<Jon *>::iterator i = game.getJons().begin(); i != game.getJons().end(); i++)
        {
            renderPhysicalEntitiesWithColor((*i)->getDustClouds());
        }
        
        if (jon.getJonShadow()->getState() != JonShadowState_Invisible)
        {
            JonShadow* pItem = jon.getJonShadow();
            JonShadow& item = *pItem;
            renderPhysicalEntity(item, MAIN_ASSETS->get(pItem));
        }
        
        m_spriteBatcher->endBatch((isVampire || jon.isRevertingToRabbit()) ? *m_vampire->gpuTextureWrapper : *m_jon->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        /// Render Jon After Images
        
        m_spriteBatcher->beginBatch();
        for (std::vector<Jon *>::iterator i = jon.getAfterImages().begin(); i != jon.getAfterImages().end(); i++)
        {
            Jon* pItem = *i;
            Jon& item = *pItem;
            renderPhysicalEntityWithColor(item, MAIN_ASSETS->get(pItem), item.getColor());
        }
        m_spriteBatcher->endBatch(*m_vampire->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        if (!jon.isConsumed())
        {
            /// Render Jon
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntitiesWithColor(game.getJons());
            m_spriteBatcher->endBatch(isVampire || isTransforming ? *m_vampire->gpuTextureWrapper : *m_jon->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        }
    }

	if (game.getCountHissWithMinas().size() > 0)
	{
		CountHissWithMina *chwm = game.getCountHissWithMinas().at(0);
		if ((!chwm->isFacingLeft() && ensureTexture(m_jon))
			|| (chwm->isFacingLeft() && ensureTexture(m_world_1_end_boss_part_1)))
		{
			m_spriteBatcher->beginBatch();
			renderPhysicalEntities(game.getCountHissWithMinas());
			m_spriteBatcher->endBatch(chwm->isFacingLeft() ? *m_world_1_end_boss_part_1->gpuTextureWrapper : *m_jon->gpuTextureWrapper, *m_textureGpuProgramWrapper);
		}
	}
    
    if (ensureTexture(m_world_1_objects_part_2))
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getExtraForegroundObjects());
        renderPhysicalEntities(game.getForegroundCoverObjects());
        m_spriteBatcher->endBatch(*m_world_1_objects_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
}

void MainRenderer::renderMidBossOwl(MidBossOwl& midBossOwl)
{
    if (!(ensureTexture(m_world_1_mid_boss_part_1)
          && ensureTexture(m_world_1_mid_boss_part_2)
          && ensureTexture(m_world_1_mid_boss_part_3)))
    {
        return;
    }
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    switch (midBossOwl.getState())
    {
        case MidBossOwlState_Sleeping:
        case MidBossOwlState_Awakening:
        case MidBossOwlState_Screeching:
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntity(midBossOwl, MAIN_ASSETS->get(&midBossOwl));
            m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_3->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        }
            break;
        case MidBossOwlState_Pursuing:
        case MidBossOwlState_FlyingOverTree:
        case MidBossOwlState_SwoopingDown:
        case MidBossOwlState_FlyingAwayAfterCatchingJon:
        case MidBossOwlState_SlammingIntoTree:
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntity(midBossOwl, MAIN_ASSETS->get(&midBossOwl));
            m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        }
            break;
        case MidBossOwlState_Dying:
        case MidBossOwlState_Dead:
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntity(midBossOwl, MAIN_ASSETS->get(&midBossOwl));
            m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        }
            break;
        default:
            break;
    }
}

void MainRenderer::renderEndBossSnake(EndBossSnake& endBossSnake)
{
    if (!(ensureTexture(m_world_1_end_boss_part_1)
          && ensureTexture(m_world_1_end_boss_part_2)
          && ensureTexture(m_world_1_end_boss_part_3)))
    {
        return;
    }
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    /// Render Jon After Images
    
    for (std::vector<EndBossSnake *>::iterator i = endBossSnake.getAfterImages().begin(); i != endBossSnake.getAfterImages().end(); i++)
    {
        EndBossSnake* pItem = *i;
        EndBossSnake& item = *pItem;
        
        switch (item.getState())
        {
            case EndBossSnakeState_ChargingLeft:
            {
                m_spriteBatcher->beginBatch();
                renderPhysicalEntityWithColor(item.getSnakeBody(), MAIN_ASSETS->get(&item.getSnakeBody()), item.getSnakeBody().getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
                
                if (item.getSnakeTonque().isMouthOpen())
                {
                    m_spriteBatcher->beginBatch();
                    renderPhysicalEntity(item.getSnakeTonque(), MAIN_ASSETS->get(&item.getSnakeTonque()));
                    m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
                }
                
                m_spriteBatcher->beginBatch();
                renderPhysicalEntityWithColor(item, MAIN_ASSETS->get(&item), item.getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
                
                m_spriteBatcher->beginBatch();
                renderPhysicalEntity(item.getSnakeEye(), MAIN_ASSETS->get(&item.getSnakeEye()));
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
            }
                break;
            case EndBossSnakeState_ChargingRight:
            {
                m_spriteBatcher->beginBatch();
                renderPhysicalEntityWithColor(item.getSnakeBody(), MAIN_ASSETS->get(&item.getSnakeBody()), item.getSnakeBody().getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
                
                if (item.getSnakeTonque().isMouthOpen())
                {
                    m_spriteBatcher->beginBatch();
                    renderPhysicalEntity(item.getSnakeTonque(), MAIN_ASSETS->get(&item.getSnakeTonque()));
                    m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
                }
                
                m_spriteBatcher->beginBatch();
                renderPhysicalEntityWithColor(item, MAIN_ASSETS->get(&item), item.getColor());
                renderPhysicalEntityWithColor(item.getSnakeSkin(), MAIN_ASSETS->get(&item.getSnakeSkin()), item.getSnakeSkin().getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
                
                m_spriteBatcher->beginBatch();
                renderPhysicalEntityWithColor(item.getSnakeHeadImpact(), MAIN_ASSETS->get(&item.getSnakeHeadImpact()), item.getSnakeHeadImpact().getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
            }
                break;
            default:
                break;
        }
    }
    
    switch (endBossSnake.getState())
    {
        case EndBossSnakeState_Sleeping:
        case EndBossSnakeState_Awakening:
        case EndBossSnakeState_OpeningMouthLeft:
		case EndBossSnakeState_OpenMouthLeft:
        case EndBossSnakeState_ChargingLeft:
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake.getSnakeBody(), MAIN_ASSETS->get(&endBossSnake.getSnakeBody()), endBossSnake.getSnakeBody().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            if (endBossSnake.getSnakeTonque().isMouthOpen())
            {
                m_spriteBatcher->beginBatch();
                renderPhysicalEntity(endBossSnake.getSnakeTonque(), MAIN_ASSETS->get(&endBossSnake.getSnakeTonque()));
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
            }
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake, MAIN_ASSETS->get(&endBossSnake), endBossSnake.getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntity(endBossSnake.getSnakeEye(), MAIN_ASSETS->get(&endBossSnake.getSnakeEye()));
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        }
            break;
        case EndBossSnakeState_Pursuing:
		case EndBossSnakeState_Waiting:
        case EndBossSnakeState_Damaged:
        case EndBossSnakeState_OpeningMouthRight:
		case EndBossSnakeState_OpenMouthRight:
        case EndBossSnakeState_ChargingRight:
        {
            m_spriteBatcher->beginBatch();
			renderPhysicalEntityWithColor(endBossSnake.getSnakeBody(), MAIN_ASSETS->get(&endBossSnake.getSnakeBody()), endBossSnake.getSnakeBody().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            if (endBossSnake.getSnakeTonque().isMouthOpen())
            {
                m_spriteBatcher->beginBatch();
                renderPhysicalEntity(endBossSnake.getSnakeTonque(), MAIN_ASSETS->get(&endBossSnake.getSnakeTonque()));
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
            }
            
			m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake, MAIN_ASSETS->get(&endBossSnake), endBossSnake.getColor());
			renderPhysicalEntityWithColor(endBossSnake.getSnakeSkin(), MAIN_ASSETS->get(&endBossSnake.getSnakeSkin()), endBossSnake.getSnakeSkin().getColor());
			m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake.getSnakeHeadImpact(), MAIN_ASSETS->get(&endBossSnake.getSnakeHeadImpact()), endBossSnake.getSnakeHeadImpact().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        }
            break;
        case EndBossSnakeState_Dying:
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake.getSnakeBody(), MAIN_ASSETS->get(&endBossSnake.getSnakeBody()), endBossSnake.getSnakeBody().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake, MAIN_ASSETS->get(&endBossSnake), endBossSnake.getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_3->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
        }
            break;
		case EndBossSnakeState_DeadSpiritReleasing:
        case EndBossSnakeState_Dead:
        {
            m_spriteBatcher->beginBatch();
			renderPhysicalEntityWithColor(endBossSnake, MAIN_ASSETS->get(&endBossSnake), endBossSnake.getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_3->gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake.getSnakeSpirit(), MAIN_ASSETS->get(&endBossSnake.getSnakeSpirit()), endBossSnake.getSnakeSpirit().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        }
            break;
        default:
            break;
    }
}

void MainRenderer::renderBatPanel(BatPanel& batPanel)
{
    if (!ensureTexture(m_tutorial))
    {
        return;
    }
    
    Bat* bat = batPanel.getBat();
    BatInstruction* batInstruction = batPanel.getBatInstruction();
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*bat, MAIN_ASSETS->get(bat));
    
    if (batInstruction->isOpening() || batInstruction->isOpen())
    {
        renderPhysicalEntityWithColor(*batInstruction, MAIN_ASSETS->get(batInstruction), batInstruction->getColor());
    }
    
    m_spriteBatcher->endBatch(*m_tutorial->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderBounds(Game& game, int boundsLevelRequested)
{
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getRight(), m_camBounds->getBottom(), m_camBounds->getTop());
    
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getMidgrounds());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getExitGrounds());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getGrounds());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getPits());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getHoles());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getForegroundObjects());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getMidBossForegroundObjects());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getEndBossForegroundObjects());
	renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getCountHissWithMinas());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getEndBossSnakes());
	renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getEnemies());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getCollectibleItems());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getJons());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getExtraForegroundObjects());
    renderBoundsForPhysicalEntities(*m_boundsNGRectBatcher, game.getForegroundCoverObjects());
    
    static Color gridColor = Color(1, 1, 1, 0.4f);
    
    int right = m_camBounds->getRight() / GRID_CELL_SIZE;
    int len = right;
    
    m_lineBatcher->beginBatch();
    
    for (int i = 0; i < len; i+= boundsLevelRequested)
    {
        float x = i * GRID_CELL_SIZE;
        m_lineBatcher->renderLine(x, 0, x, GAME_HEIGHT, gridColor);
    }
    
    for (int j = 0; j < 256; j+= boundsLevelRequested)
    {
        float y = j * GRID_CELL_SIZE;
        m_lineBatcher->renderLine(m_camBounds->getLeft(), y, m_camBounds->getRight(), y, gridColor);
    }
    
    m_lineBatcher->endBatch(*m_colorGpuProgramWrapper);
}

void MainRenderer::renderEntityHighlighted(PhysicalEntity& entity, Color& c)
{
	m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    m_fillNGRectBatcher->beginBatch();
    renderHighlightForPhysicalEntity(entity, c);
    m_fillNGRectBatcher->endBatch(*m_colorGpuProgramWrapper);
}

void MainRenderer::renderBlackOverlay(float opacity)
{
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static NGRect rect = NGRect(0, 0, CAM_WIDTH, CAM_HEIGHT);
    
    static Color blackColor = Color(0, 0, 0, 0);
    blackColor.alpha = opacity;
    
    m_fillNGRectBatcher->beginBatch();
    m_fillNGRectBatcher->renderNGRect(rect, blackColor);
    m_fillNGRectBatcher->endBatch(*m_colorGpuProgramWrapper);
}

void MainRenderer::renderHud(Game& game, GameButton* backButton, GameButton* continueButton, int score)
{
	m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static Color fontColor = Color(1, 1, 1, 1);
    static float fgWidth = CAM_WIDTH / 32;
    static float fgHeight = fgWidth * 1.171875f;
    
    float textY = CAM_HEIGHT - fgHeight;
    
    if (ensureTexture(m_world_1_objects_part_1))
    {
        static GameHudCarrot uiCarrot = GameHudCarrot(false);
        static GameHudCarrot uiGoldenCarrot = GameHudCarrot(true);
        
        uiGoldenCarrot.getPosition().set(2.7f, textY);
        uiGoldenCarrot.setWidth(fgWidth);
        uiGoldenCarrot.setHeight(fgHeight);
        
        uiCarrot.getPosition().set(4.3f, textY);
        uiCarrot.setWidth(fgWidth);
        uiCarrot.setHeight(fgHeight);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(uiGoldenCarrot, MAIN_ASSETS->get(&uiGoldenCarrot));
        renderPhysicalEntity(uiCarrot, MAIN_ASSETS->get(&uiCarrot));
        m_spriteBatcher->endBatch(*m_world_1_objects_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
    
    m_spriteBatcher->beginBatch();
    
    static TextureRegion xTr = ASSETS->findTextureRegion("CarrotCountMarker");
    
    /// Render Num Golden Carrots Collected
    
    {
        m_spriteBatcher->drawSprite(2.94f, textY - 0.1f, fgWidth / 2, fgHeight / 2, 0, xTr);
        
        std::stringstream ss;
        ss << game.getNumGoldenCarrotsCollected();
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, 3.34f, textY - 0.14f, fgWidth, fgHeight, fontColor);
    }
    
    /// Render Num Carrots Collected
    
    {
        m_spriteBatcher->drawSprite(4.54f, textY - 0.1f, fgWidth / 2, fgHeight / 2, 0, xTr);
        
        std::stringstream ss;
        ss << game.getNumCarrotsCollected();
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, 4.94f, textY - 0.14f, fgWidth, fgHeight, fontColor);
    }
    
    /// Render Score
    
    {
        std::stringstream ss;
        
        // the number is converted to string with the help of stringstream
        ss << score;
        std::string paddedScore;
        ss >> paddedScore;
        
        // Append zero chars
        unsigned long str_length = paddedScore.length();
        for (int i = 0; i < 6 - str_length; i++)
        {
            paddedScore = "0" + paddedScore;
        }
        
        m_font->renderText(*m_spriteBatcher, paddedScore, CAM_WIDTH * 0.5f, textY, fgWidth, fgHeight, fontColor);
    }
    
    /// Render Game Tracker
    
    for (std::vector<DelayText *>::iterator i = GameTracker::getInstance()->getTexts().begin(); i != GameTracker::getInstance()->getTexts().end(); i++)
    {
        std::string value = (*i)->getText();
        m_font->renderText(*m_spriteBatcher, value, (*i)->getPosition().getX(), (*i)->getPosition().getY(), fgWidth, fgHeight, (*i)->getColor(), false, true);
    }
    
    /// Render Time
    
    {
        static TextureRegion clockTr = ASSETS->findTextureRegion("ClockIcon");
        
        m_spriteBatcher->drawSprite(CAM_WIDTH * 0.72f, textY + fgHeight * 0.08f, fgWidth * 2 / 3, fgHeight * 2 / 3, 0, clockTr);
        
        float seconds = game.getStateTime();
        
        if (seconds > 599.999f)
        {
            seconds = 599.999f;
        }
        
        int minutesLeft = 0;
        while (seconds >= 60)
        {
            seconds -= 60;
            minutesLeft++;
        }
        
        std::stringstream ss;
        ss << minutesLeft << ":";
        if (seconds < 10)
        {
            ss << "0";
        }
        ss << std::fixed << std::setprecision(3) << seconds;
        std::string text = ss.str();
        
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH * 0.72f + fgWidth, textY, fgWidth, fgHeight, fontColor);
    }
    
    /// Render Back Button
    
    if (backButton)
    {
        renderPhysicalEntity(*backButton, MAIN_ASSETS->get(backButton));
        
        static TextureRegion speedBarTr = ASSETS->findTextureRegion("SpeedBar");
        static TextureRegion speedBarFrameTr = ASSETS->findTextureRegion("SpeedBarFrame");
        
        float jonSpeed = game.getJon().getVelocity().getX();
        jonSpeed = clamp(jonSpeed, VAMP_DEFAULT_MAX_SPEED, 0);
        
        float jonSpeedRatioToMaxSpeed = jonSpeed / VAMP_DEFAULT_MAX_SPEED;
        
        float speedBarRegionWidth = speedBarTr.m_fRegionWidth;
        float speedBarRegionWidthToRender = speedBarRegionWidth * jonSpeedRatioToMaxSpeed;
        
        speedBarTr.init(speedBarTr.m_fX, speedBarRegionWidthToRender, speedBarTr.m_fTextureWidth);
        
        m_spriteBatcher->drawSprite(0.2f + 2 * jonSpeedRatioToMaxSpeed, 0.27751937984496, 4 * jonSpeedRatioToMaxSpeed, 0.15503875968992f, 0, speedBarTr);
        m_spriteBatcher->drawSprite(2.2f, 0.27751937984496, 4, 0.15503875968992f, 0, speedBarFrameTr);
        
        speedBarTr.init(speedBarTr.m_fX, speedBarRegionWidth, speedBarTr.m_fTextureWidth);
    }
    
    if (continueButton)
    {
        renderPhysicalEntityWithColor(*continueButton, MAIN_ASSETS->get(continueButton), continueButton->getColor());
    }

    m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderResumeButtonOverlay()
{
    if (!m_vampire->gpuTextureWrapper)
    {
        return;
    }
    
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static TextureRegion resumeButtonTr = ASSETS->findTextureRegion("ResumeButton");
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, CAM_HEIGHT / 2, 2, 2, 0, resumeButtonTr);
    m_spriteBatcher->endBatch(*m_vampire->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderDebugInfo(Game& game, int fps)
{
	m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

	static Color fontColor = Color(1, 1, 1, 1);
	static float fgWidth = CAM_WIDTH / 24 / 2;
	static float fgHeight = fgWidth * 1.171875f;

	{
		// Render FPS

		m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

		std::stringstream ss;
		ss << fps << " FPS";
		std::string fps_string = ss.str();

		m_spriteBatcher->beginBatch();
		m_font->renderText(*m_spriteBatcher, fps_string, CAM_WIDTH / 5, CAM_HEIGHT - fgHeight * 4, fgWidth, fgHeight, fontColor, true);
		m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
	}

	if (game.getJons().size() > 0)
	{
		// Render Jon's Speed

		m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

		Jon& jon = game.getJon();

		float speed = jon.getVelocity().getX();

		std::stringstream ss;
		ss << std::fixed << std::setprecision(3) << speed << " m/s";
		std::string text = ss.str();

		m_spriteBatcher->beginBatch();
		m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 5, CAM_HEIGHT - fgHeight * 5, fgWidth, fgHeight, fontColor, true);
		m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
	}
}

void MainRenderer::renderComingSoonScreenBackground()
{
    if (!ensureTexture(m_world_1_special))
    {
        return;
    }
    
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static TextureRegion tr = ASSETS->findTextureRegion("ComingSoonScreen");
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH, CAM_HEIGHT, 0, tr);
    m_spriteBatcher->endBatch(*m_world_1_special->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderMarkers(Game& game)
{
    static Color originMarkerColor = Color(0, 1, 0, 0.5f);
    static Color endMarkerColor = Color(1, 0, 0, 0.5f);
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    m_fillNGRectBatcher->beginBatch();
    
    for (std::vector<GameMarker *>::iterator i = game.getMarkers().begin(); i != game.getMarkers().end(); i++)
    {
        NGRect& marker = (*i)->getMainBounds();
        m_fillNGRectBatcher->renderNGRect(marker, (*i)->getType() == 0 ? originMarkerColor : endMarkerColor);
    }
    
    m_fillNGRectBatcher->endBatch(*m_colorGpuProgramWrapper);
}

void MainRenderer::renderLevelEditor(MainScreenLevelEditor* gameScreenLevelEditor)
{
	LevelEditorActionsPanel* leap = gameScreenLevelEditor->getLevelEditorActionsPanel();
	LevelEditorEntitiesPanel* leep = gameScreenLevelEditor->getLevelEditorEntitiesPanel();
	TrashCan* tc = gameScreenLevelEditor->getTrashCan();
	LevelSelectorPanel* lsp = gameScreenLevelEditor->getLevelSelectorPanel();
	OffsetPanel* osp = gameScreenLevelEditor->getOffsetPanel();
	ConfirmResetPanel* crp = gameScreenLevelEditor->getConfirmResetPanel();
	ConfirmExitPanel* cep = gameScreenLevelEditor->getConfirmExitPanel();

    if (!ensureTexture(m_level_editor))
    {
        return;
    }
    
	static NGRect originMarker = NGRect(0, 0, 0.1f, GAME_HEIGHT);
    static Color originMarkerColor = Color(0, 0, 0, 0.7f);
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    m_fillNGRectBatcher->beginBatch();
    m_fillNGRectBatcher->renderNGRect(originMarker, originMarkerColor);
    m_fillNGRectBatcher->endBatch(*m_colorGpuProgramWrapper);
    
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    /// Render Level Editor
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*leap, MAIN_ASSETS->get(leap));
    renderPhysicalEntity(*leep, MAIN_ASSETS->get(leep));
    m_spriteBatcher->endBatch(*m_level_editor->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*tc, MAIN_ASSETS->get(tc));
    m_spriteBatcher->endBatch(*m_level_editor->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    
    if (leep->isOpen()
        && ensureTexture(m_world_1_objects_part_2)
        && ensureTexture(m_world_1_ground)
        && ensureTexture(m_world_1_special)
        && ensureTexture(m_world_1_objects_part_1)
        && ensureTexture(m_world_1_mid_boss_part_3)
        && ensureTexture(m_world_1_end_boss_part_1)
        && ensureTexture(m_jon)
        && ensureTexture(m_world_1_enemies))
    {
        m_rendererHelper->updateMatrix(0, CAM_WIDTH, leep->getEntitiesCameraPos(), leep->getEntitiesCameraPos() + CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getMidgrounds());
        m_spriteBatcher->endBatch(*m_world_1_objects_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getGrounds());
        renderPhysicalEntities(leep->getExitGrounds());
        m_spriteBatcher->endBatch(*m_world_1_ground->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getPits());
        m_spriteBatcher->endBatch(*m_world_1_special->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getHoles());
        renderPhysicalEntities(leep->getCollectibleItems());
        renderPhysicalEntities(leep->getForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_objects_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getMidBossForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_3->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getEndBossForegroundObjects());
        renderPhysicalEntities(leep->getEndBossSnakes());
        m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);

        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getCountHissWithMinas());
        m_spriteBatcher->endBatch(*m_jon->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getEnemies());
        m_spriteBatcher->endBatch(*m_world_1_enemies->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getJons());
        m_spriteBatcher->endBatch(*m_jon->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getExtraForegroundObjects());
        renderPhysicalEntities(leep->getForegroundCoverObjects());
        m_spriteBatcher->endBatch(*m_world_1_objects_part_2->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
    
    if (lsp->isOpen()
        && ensureTexture(m_misc))
    {
        m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*lsp, MAIN_ASSETS->get(lsp));
        m_spriteBatcher->endBatch(*m_level_editor->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        static Color fontColor = Color(1, 1, 1, 1);
        
        float fgWidth = lsp->getTextSize();
        float fgHeight = fgWidth * 1.171875f;
        
        m_spriteBatcher->beginBatch();
        {
            std::stringstream ss;
            ss << lsp->getWorld();
            std::string text = ss.str();
            m_font->renderText(*m_spriteBatcher, text, lsp->getWorldTextPosition().getX(), lsp->getWorldTextPosition().getY(), fgWidth, fgHeight, fontColor, false, true);
        }
        {
            std::stringstream ss;
            ss << lsp->getLevel();
            std::string text = ss.str();
            m_font->renderText(*m_spriteBatcher, text, lsp->getLevelTextPosition().getX(), lsp->getLevelTextPosition().getY(), fgWidth, fgHeight, fontColor, false, true);
        }
        m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
    
    if (osp->isOpen()
        && ensureTexture(m_misc))
    {
        m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*osp, MAIN_ASSETS->get(osp));
        m_spriteBatcher->endBatch(*m_level_editor->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        
        static Color fontColor = Color(1, 1, 1, 1);
        
        float fgWidth = osp->getTextSize();
        float fgHeight = fgWidth * 1.171875f;
        
        int offset = osp->getOffset();
        m_spriteBatcher->beginBatch();
        if (offset <= 0)
        {
            std::stringstream ss;
            ss << (offset * -1);
            std::string text = ss.str();
            m_font->renderText(*m_spriteBatcher, text, osp->getLeftTextPosition().getX(), osp->getLeftTextPosition().getY(), fgWidth, fgHeight, fontColor, false, true);
        }
        
        if (offset >= 0)
        {
            std::stringstream ss;
            ss << offset;
            std::string text = ss.str();
            m_font->renderText(*m_spriteBatcher, text, osp->getRightTextPosition().getX(), osp->getRightTextPosition().getY(), fgWidth, fgHeight, fontColor, false, true);
        }
        m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
    
    if (crp->isOpen())
    {
        m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*crp, MAIN_ASSETS->get(crp));
        m_spriteBatcher->endBatch(*m_level_editor->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }
    
    if (cep->isOpen())
    {
        m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*cep, MAIN_ASSETS->get(cep));
        m_spriteBatcher->endBatch(*m_level_editor->gpuTextureWrapper, *m_textureGpuProgramWrapper);
    }

    if (ensureTexture(m_misc))
	{
		static Color fontColor = Color(1, 1, 1, 1);
		static float fgWidth = CAM_WIDTH / 24;
		static float fgHeight = fgWidth * 1.171875f;

		m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

		std::stringstream ss;
		ss << "World " << lsp->getWorld() << " / Level " << lsp->getLevel();
		std::string text_string = ss.str();

		// Render World/Level Info

		m_spriteBatcher->beginBatch();
		m_font->renderText(*m_spriteBatcher, text_string, CAM_WIDTH - 4, CAM_HEIGHT - fgHeight / 2, fgWidth / 2, fgHeight / 2, fontColor, true);
		m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
	}

	static TextureRegion xTr = ASSETS->findTextureRegion("CarrotCountMarker");

	{
		static Color fontColor = Color(1, 1, 1, 1);
		static float fgWidth = CAM_WIDTH / 32;
		static float fgHeight = fgWidth * 1.171875f;

		float textY = CAM_HEIGHT - fgHeight;

		if (ensureTexture(m_world_1_objects_part_1))
		{
			static GameHudCarrot uiCarrot = GameHudCarrot(false);
			static GameHudCarrot uiGoldenCarrot = GameHudCarrot(true);

			uiGoldenCarrot.getPosition().set(2.7f, textY);
			uiGoldenCarrot.setWidth(fgWidth);
			uiGoldenCarrot.setHeight(fgHeight);

			uiCarrot.getPosition().set(4.3f, textY);
			uiCarrot.setWidth(fgWidth);
			uiCarrot.setHeight(fgHeight);

			m_spriteBatcher->beginBatch();
			renderPhysicalEntity(uiGoldenCarrot, MAIN_ASSETS->get(&uiGoldenCarrot));
			renderPhysicalEntity(uiCarrot, MAIN_ASSETS->get(&uiCarrot));
			m_spriteBatcher->endBatch(*m_world_1_objects_part_1->gpuTextureWrapper, *m_textureGpuProgramWrapper);
		}

        if (ensureTexture(m_misc))
        {
            int numCarrots = 0;
            int numGoldenCarrots = 0;
            for (
                 std::vector<CollectibleItem *>::iterator i = gameScreenLevelEditor->getGame().getCollectibleItems().begin();
                 i != gameScreenLevelEditor->getGame().getCollectibleItems().end();
                 i++)
            {
                if ((*i)->getType() == CollectibleItemType_Carrot)
                {
                    numCarrots++;
                }
                else if ((*i)->getType() == CollectibleItemType_BigCarrot)
                {
                    numCarrots += 10;
                }
                else if ((*i)->getType() == CollectibleItemType_GoldenCarrot)
                {
                    numGoldenCarrots++;
                }
            }
            
            m_spriteBatcher->beginBatch();
            
            /// Render Num Golden Carrots Added
            
            {
                m_spriteBatcher->drawSprite(2.94f, textY - 0.1f, fgWidth / 2, fgHeight / 2, 0, xTr);
                
                std::stringstream ss;
                ss << numGoldenCarrots;
                std::string text = ss.str();
                m_font->renderText(*m_spriteBatcher, text, 3.34f, textY - 0.14f, fgWidth, fgHeight, fontColor);
            }
            
            /// Render Num Carrots Added
            
            {
                m_spriteBatcher->drawSprite(4.54f, textY - 0.1f, fgWidth / 2, fgHeight / 2, 0, xTr);
                
                std::stringstream ss;
                ss << numCarrots;
                std::string text = ss.str();
                m_font->renderText(*m_spriteBatcher, text, 4.94f, textY - 0.14f, fgWidth, fgHeight, fontColor);
            }
            
            m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
        }

		if (gameScreenLevelEditor->getMessage())
		{
			static NGRect fontBg = NGRect(0, CAM_HEIGHT / 2 - fgHeight / 2, CAM_WIDTH, fgHeight);
			static Color fontBgColor = Color(0, 0, 0, 1);

			m_fillNGRectBatcher->beginBatch();

			m_fillNGRectBatcher->renderNGRect(fontBg, fontBgColor);

			m_fillNGRectBatcher->endBatch(*m_colorGpuProgramWrapper);

			static Color fontMessageColor = Color(1, 0, 0, 1);

			m_spriteBatcher->beginBatch();

			std::stringstream ss;
			ss << gameScreenLevelEditor->getMessage();
			std::string text = ss.str();
			m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, CAM_HEIGHT / 2, fgWidth / 2, fgHeight / 2, fontMessageColor, true);

			m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
		}
	}
}

void MainRenderer::renderLoading(float deltaTime)
{
    m_fStateTime += deltaTime;
    
    if (!ensureTexture(m_misc))
    {
        return;
    }
    
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static Animation anim = Animation("misc", 0, 900, 144, 96, 1296, 96, TEXTURE_SIZE_1024, true, 0.06f, 7);
    
    static float width = 1.265625f;
    static float height = 0.84375f;
    static float x = CAM_WIDTH - width / 2;
    static float y = CAM_HEIGHT - height / 2;
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(x, y, width, height, 0, anim.getTextureRegion(m_fStateTime));
    m_spriteBatcher->endBatch(*m_misc->gpuTextureWrapper, *m_textureGpuProgramWrapper);
}

void MainRenderer::renderToSecondFramebufferWithShockwave(float centerX, float centerY, float timeElapsed, bool isTransforming)
{
    m_shockwaveTextureGpuProgramWrapper->configure(centerX, centerY, timeElapsed + 0.1f, isTransforming);
    
    m_rendererHelper->updateMatrix(m_camBounds->getLeft(), m_camBounds->getLeft() + m_camBounds->getWidth(), m_camBounds->getBottom(), m_camBounds->getBottom() + m_camBounds->getHeight());
    
    setFramebuffer(1);
    
    float x = m_camBounds->getLeft() + m_camBounds->getWidth() / 2;
    float y = m_camBounds->getBottom() + m_camBounds->getHeight() / 2;
    
    static TextureRegion tr = TextureRegion("framebuffer", 0, 0, 1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(x, y, m_camBounds->getWidth(), m_camBounds->getHeight(), 0, tr);
    m_spriteBatcher->endBatch(*m_rendererHelper->getFramebuffer(0), *m_shockwaveTextureGpuProgramWrapper);
}

void MainRenderer::renderToSecondFramebuffer(Game& game)
{
    setFramebuffer(1);
    
    static TextureRegion tr = TextureRegion("framebuffer", 0, 0, 1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, tr);
    
    Jon& jon = game.getJon();
    bool isVampire = jon.isVampire();
    m_spriteBatcher->endBatch(*m_rendererHelper->getFramebuffer(0), (isVampire || jon.isRevertingToRabbit()) ? *m_framebufferTintGpuProgramWrapper : *m_framebufferToScreenGpuProgramWrapper);
}

void MainRenderer::renderToThirdFramebufferWithObfuscation()
{
    setFramebuffer(2);
    
    static TextureRegion tr = TextureRegion("framebuffer", 0, 0, 1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, tr);
    m_spriteBatcher->endBatch(*m_rendererHelper->getFramebuffer(1), *m_framebufferObfuscationGpuProgramWrapper);
}

void MainRenderer::renderToThirdFramebufferWithTransDeathIn(float timeElapsed)
{
    /// Render the death transition to the screen
    
    m_transDeathInGpuProgramWrapper->configure(m_trans_death_shader_helper->gpuTextureWrapper, timeElapsed);
    
    int fbIndex = m_iFramebufferIndex;
    setFramebuffer(fbIndex + 1);
    
    m_rendererHelper->clearFramebufferWithColor(0, 0, 0, 1);
    
    static TextureRegion tr = TextureRegion("framebuffer", 0, 0, 1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, tr);
    m_spriteBatcher->endBatch(*m_rendererHelper->getFramebuffer(fbIndex), *m_transDeathInGpuProgramWrapper);
}

void MainRenderer::renderToThirdFramebufferWithTransDeathOut(float timeElapsed)
{
    /// Render the new game to the screen
    
    m_transDeathOutGpuProgramWrapper->configure(m_trans_death_shader_helper->gpuTextureWrapper, timeElapsed);
    
    int fbIndex = m_iFramebufferIndex;
    setFramebuffer(fbIndex + 1);
    
    m_rendererHelper->clearFramebufferWithColor(0, 0, 0, 1);
    
    static TextureRegion tr = TextureRegion("framebuffer", 0, 0, 1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, tr);
    m_spriteBatcher->endBatch(*m_rendererHelper->getFramebuffer(fbIndex), *m_transDeathOutGpuProgramWrapper);
}

void MainRenderer::renderToThirdFramebufferTransition(float progress)
{
    /// Render the screen transition to the screen
    
    m_transScreenGpuProgramWrapper->configure(m_rendererHelper->getFramebuffer(1), progress);
    
    setFramebuffer(2);
    
    m_rendererHelper->clearFramebufferWithColor(0, 0, 0, 1);
    
    static TextureRegion tr = TextureRegion("framebuffer", 0, 0, 1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, tr);
    m_spriteBatcher->endBatch(*m_rendererHelper->getFramebuffer(0), *m_transScreenGpuProgramWrapper);
}

void MainRenderer::renderToThirdFramebufferFadeTransition(float progress)
{
    /// Render the screen transition to the screen
    
    m_fadeScreenGpuProgramWrapper->configure(m_rendererHelper->getFramebuffer(1), progress);
    
    setFramebuffer(2);
    
    m_rendererHelper->clearFramebufferWithColor(0, 0, 0, 1);
    
    static TextureRegion tr = TextureRegion("framebuffer", 0, 0, 1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, tr);
    m_spriteBatcher->endBatch(*m_rendererHelper->getFramebuffer(0), *m_fadeScreenGpuProgramWrapper);
}

void MainRenderer::renderToThirdFramebufferPointTransition(float centerX, float centerY, float progress)
{
    /// Render the screen transition to the screen
    
    m_pointTransScreenGpuProgramWrapper->configure(m_rendererHelper->getFramebuffer(1), centerX, centerY, progress);
    
    m_rendererHelper->updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    setFramebuffer(2);
    
    m_rendererHelper->clearFramebufferWithColor(0, 0, 0, 1);
    
    static TextureRegion tr = TextureRegion("framebuffer", 0, 0, 1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH, CAM_HEIGHT, 0, tr);
    m_spriteBatcher->endBatch(*m_rendererHelper->getFramebuffer(0), *m_pointTransScreenGpuProgramWrapper);
}

void MainRenderer::renderToThirdFramebufferWithRadialBlur()
{
    /// Render everything to the screen with a radial blur
    
    m_framebufferRadialBlurGpuProgramWrapper->configure(m_fRadialBlurDirection);
    
    int fbIndex = m_iFramebufferIndex;
    setFramebuffer(fbIndex + 1);
    
    m_rendererHelper->clearFramebufferWithColor(0, 0, 0, 1);
    
    static TextureRegion tr = TextureRegion("framebuffer", 0, 0, 1, 1, 1, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, tr);
    m_spriteBatcher->endBatch(*m_rendererHelper->getFramebuffer(fbIndex), *m_framebufferRadialBlurGpuProgramWrapper);
}

void MainRenderer::renderToScreen()
{
	/// Render everything to the screen
    
    renderFramebufferToScreen(m_iFramebufferIndex);
}

NGRect& MainRenderer::getCameraBounds()
{
    return *m_camBounds;
}

Vector2D& MainRenderer::getCameraPosition()
{
    return m_camBounds->getLowerLeft();
}
