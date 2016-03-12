//
//  Renderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Renderer.h"
#include "SpriteBatcher.h"
#include "LineBatcher.h"
#include "Font.h"
#include "TextureRegion.h"
#include "GpuTextureDataWrapper.h"
#include "Assets.h"
#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Game.h"
#include "BackButton.h"
#include "LevelEditorButton.h"
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
#include "FramebufferRadialBlurGpuProgramWrapper.h"
#include "CollectibleItem.h"
#include "TitlePanel.h"
#include "WorldMapPanel.h"
#include "GameScreenWorldMap.h"

#include <math.h>
#include <sstream>
#include <iomanip>

Renderer::Renderer() :
m_fStateTime(0),
m_iFramebufferIndex(0),
m_iNumAsyncLoads(0),
m_iRadialBlurDirection(RADIAL_BLUR_DIRECTION_LEFT),
m_compressed(Assets::getInstance()->isUsingCompressedTextureSet()),
m_areShadersLoaded(false),
m_stopCamera(false),
m_transScreenGpuProgramWrapper(nullptr),
m_sinWaveTextureProgram(nullptr),
m_backgroundGpuTextureProgramWrapper(nullptr),
m_snakeDeathTextureProgram(nullptr),
m_shockwaveTextureGpuProgramWrapper(nullptr),
m_transDeathInGpuProgramWrapper(nullptr),
m_transDeathOutGpuProgramWrapper(nullptr),
m_framebufferToScreenGpuProgramWrapper(nullptr),
m_framebufferTintGpuProgramWrapper(nullptr),
m_framebufferRadialBlurGpuProgramWrapper(nullptr)
{
    m_font = std::unique_ptr<Font>(new Font(0, 0, 16, 64, 73, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024));
    m_camBounds = std::unique_ptr<Rectangle>(new Rectangle(0, 0, CAM_WIDTH, CAM_HEIGHT));
    m_camPosAcceleration = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
    m_camPosVelocity = std::unique_ptr<Vector2D>(new Vector2D(0, 0));
    
    m_world_1_background_lower.repeat_s = 1;
    m_world_1_background_mid.repeat_s = 1;
    m_world_1_background_upper.repeat_s = 1;
    
    m_textureWrappers.push_back(&m_jon);
    m_textureWrappers.push_back(&m_level_editor);
    m_textureWrappers.push_back(&m_misc);
    m_textureWrappers.push_back(&m_title_screen);
    m_textureWrappers.push_back(&m_trans_death_shader_helper);
    m_textureWrappers.push_back(&m_vampire);
    m_textureWrappers.push_back(&m_world_1_background_lower);
    m_textureWrappers.push_back(&m_world_1_background_mid);
    m_textureWrappers.push_back(&m_world_1_background_upper);
    m_textureWrappers.push_back(&m_world_1_enemies);
    m_textureWrappers.push_back(&m_world_1_ground);
    m_textureWrappers.push_back(&m_world_1_mid_boss_part_1);
    m_textureWrappers.push_back(&m_world_1_mid_boss_part_2);
    m_textureWrappers.push_back(&m_world_1_mid_boss_part_3);
    m_textureWrappers.push_back(&m_world_1_mid_boss_part_4);
    m_textureWrappers.push_back(&m_world_1_objects);
    m_textureWrappers.push_back(&m_world_1_special);
    m_textureWrappers.push_back(&m_world_map_screen);
}

Renderer::~Renderer()
{
    cleanUp();
}

void Renderer::init(RendererType rendererType)
{
    m_fStateTime = 0;
    
    loadMisc();
    
    load(rendererType);
    
    if (!m_areShadersLoaded)
    {
        loadShaderPrograms();
        
        m_areShadersLoaded = true;
    }
    
    if (m_framebuffers.size() == 0)
    {
        addFramebuffers();
    }
}

void Renderer::load(RendererType rendererType)
{
    m_iNumAsyncLoads = 0;
    
    if (rendererType != RENDERER_TYPE_TITLE
        && rendererType != RENDERER_TYPE_WORLD_MAP
        && rendererType != RENDERER_TYPE_LEVEL_EDITOR)
    {
        loadJon();
    }
    
    switch (rendererType)
    {
        case RENDERER_TYPE_TITLE:
            loadTitle();
            break;
        case RENDERER_TYPE_WORLD_MAP:
            loadWorldMap();
            break;
        case RENDERER_TYPE_LEVEL_EDITOR:
            loadLevelEditor();
            break;
            
        case RENDERER_TYPE_WORLD_1_PRE_LEVEL_10:
            loadWorld1();
            loadWorld1Special();
            break;
        case RENDERER_TYPE_WORLD_1_MID_BOSS:
            loadWorld1();
            loadWorld1MidBoss();
            break;
        case RENDERER_TYPE_WORLD_1_POST_LEVEL_10:
            loadWorld1();
            break;
        case RENDERER_TYPE_WORLD_1_END_BOSS:
            loadWorld1();
            loadWorld1EndBoss();
            break;
            
        case RENDERER_TYPE_WORLD_2:
            loadWorld2();
            break;
        case RENDERER_TYPE_WORLD_2_MID_BOSS:
            loadWorld2();
            loadWorld2MidBoss();
            break;
        case RENDERER_TYPE_WORLD_2_END_BOSS:
            loadWorld2();
            loadWorld2EndBoss();
            break;
            
        case RENDERER_TYPE_WORLD_3:
            loadWorld3();
            break;
        case RENDERER_TYPE_WORLD_3_MID_BOSS:
            loadWorld3();
            loadWorld3MidBoss();
            break;
        case RENDERER_TYPE_WORLD_3_END_BOSS:
            loadWorld3();
            loadWorld3EndBoss();
            break;
            
        case RENDERER_TYPE_WORLD_4:
            loadWorld4();
            break;
        case RENDERER_TYPE_WORLD_4_MID_BOSS:
            loadWorld4();
            loadWorld4MidBoss();
            break;
        case RENDERER_TYPE_WORLD_4_END_BOSS:
            loadWorld4();
            loadWorld4EndBoss();
            break;
            
        case RENDERER_TYPE_WORLD_5:
            loadWorld5();
            break;
        case RENDERER_TYPE_WORLD_5_MID_BOSS:
            loadWorld5();
            loadWorld5MidBoss();
            break;
        case RENDERER_TYPE_WORLD_5_END_BOSS:
            loadWorld5();
            loadWorld5EndBoss();
            break;

		case RENDERER_TYPE_NONE:
        default:
            break;
    }
}

void Renderer::unload(RendererType rendererType)
{
    if (rendererType != RENDERER_TYPE_TITLE
        && rendererType != RENDERER_TYPE_WORLD_MAP
        && rendererType != RENDERER_TYPE_LEVEL_EDITOR)
    {
        unloadJon();
    }
    
    switch (rendererType)
    {
        case RENDERER_TYPE_TITLE:
            unloadTitle();
            break;
        case RENDERER_TYPE_WORLD_MAP:
            unloadWorldMap();
            break;
        case RENDERER_TYPE_LEVEL_EDITOR:
            unloadLevelEditor();
            break;
            
        case RENDERER_TYPE_WORLD_1_PRE_LEVEL_10:
            unloadWorld1();
            unloadWorld1Special();
            break;
        case RENDERER_TYPE_WORLD_1_MID_BOSS:
            unloadWorld1();
            unloadWorld1MidBoss();
            break;
        case RENDERER_TYPE_WORLD_1_POST_LEVEL_10:
            unloadWorld1();
            break;
        case RENDERER_TYPE_WORLD_1_END_BOSS:
            unloadWorld1();
            unloadWorld1EndBoss();
            break;
            
        case RENDERER_TYPE_WORLD_2:
            unloadWorld2();
            break;
        case RENDERER_TYPE_WORLD_2_MID_BOSS:
            unloadWorld2();
            unloadWorld2MidBoss();
            break;
        case RENDERER_TYPE_WORLD_2_END_BOSS:
            unloadWorld2();
            unloadWorld2EndBoss();
            break;
            
        case RENDERER_TYPE_WORLD_3:
            unloadWorld3();
            break;
        case RENDERER_TYPE_WORLD_3_MID_BOSS:
            unloadWorld3();
            unloadWorld3MidBoss();
            break;
        case RENDERER_TYPE_WORLD_3_END_BOSS:
            unloadWorld3();
            unloadWorld3EndBoss();
            break;
            
        case RENDERER_TYPE_WORLD_4:
            unloadWorld4();
            break;
        case RENDERER_TYPE_WORLD_4_MID_BOSS:
            unloadWorld4();
            unloadWorld4MidBoss();
            break;
        case RENDERER_TYPE_WORLD_4_END_BOSS:
            unloadWorld4();
            unloadWorld4EndBoss();
            break;
            
        case RENDERER_TYPE_WORLD_5:
            unloadWorld5();
            break;
        case RENDERER_TYPE_WORLD_5_MID_BOSS:
            unloadWorld5();
            unloadWorld5MidBoss();
            break;
        case RENDERER_TYPE_WORLD_5_END_BOSS:
            unloadWorld5();
            unloadWorld5EndBoss();
            break;

		case RENDERER_TYPE_NONE:
        default:
            break;
    }
}

bool Renderer::isLoadingAdditionalTextures()
{
    return m_iNumAsyncLoads > 0;
}

bool Renderer::isLoaded()
{
    return m_misc.gpuTextureWrapper && areShadersLoaded();
}

void Renderer::beginFrame(float deltaTime)
{
    setFramebuffer(0);
    
    m_fStateTime += deltaTime;
    
	if (m_iNumAsyncLoads > 0)
	{
		handleAsyncTextureLoads();
	}
}

void Renderer::setFramebuffer(int framebufferIndex)
{
    m_iFramebufferIndex = framebufferIndex;
    bindToOffscreenFramebuffer(framebufferIndex);
    clearFramebufferWithColor(0, 0, 0, 1);
}

void Renderer::beginOpeningPanningSequence(Game& game)
{
    zoomIn();
    
    m_fStateTime = 0;
    
    updateCameraToFollowJon(game, 1337);
    
    m_camBounds->getLowerLeft().setX(getCamPosFarRight(game));
    m_camBounds->getLowerLeft().setY(game.getFarRightBottom());
    
    Jon& jon = game.getJon();
    float farLeft = jon.getPosition().getX() - CAM_WIDTH / 5;
    float farLeftBottom = jon.getPosition().getY() - jon.getHeight() / 4 * 3;
    
    float changeInX = farLeft - getCamPosFarRight(game);
    float changeInY = farLeftBottom - game.getFarRightBottom();
    
    if (changeInY > (GAME_HEIGHT / 6))
    {
        m_iRadialBlurDirection = RADIAL_BLUR_DIRECTION_TOP_LEFT;
    }
    else if (changeInY < -(GAME_HEIGHT / 6))
    {
        m_iRadialBlurDirection = RADIAL_BLUR_DIRECTION_BOTTOM_LEFT;
    }
    else
    {
        m_iRadialBlurDirection = RADIAL_BLUR_DIRECTION_LEFT;
    }
    
    m_camPosVelocity->set(changeInX, changeInY);
}

int Renderer::updateCameraToFollowPathToJon(Game& game)
{
    if (m_fStateTime >= 5.0f && m_fStateTime <= 5.265f)
    {
        bool isComplete = false;
        float progress = (m_fStateTime - 5.0f) / 0.2f;
        if (progress > 1)
        {
            progress = 1;
            isComplete = true;
        }
        
        Jon& jon = game.getJon();
        float farLeft = jon.getPosition().getX() - CAM_WIDTH / 5;
        float farLeftBottom = jon.getPosition().getY() - jon.getHeight() / 4 * 3;
        
        float changeInX = farLeft - getCamPosFarRight(game);
        float changeInY = farLeftBottom - game.getFarRightBottom();
        
        m_camBounds->getLowerLeft().set(getCamPosFarRight(game) + changeInX * progress, game.getFarRightBottom() + changeInY * progress);

        if (m_camBounds->getLowerLeft().getX() < farLeft)
        {
            m_camBounds->getLowerLeft().setX(farLeft);
        }
        
        if ((m_camPosVelocity->getY() < 0 && m_camBounds->getLowerLeft().getY() < farLeftBottom)
            || (m_camPosVelocity->getY() > 0 && m_camBounds->getLowerLeft().getY() > farLeftBottom))
        {
            m_camBounds->getLowerLeft().setY(farLeftBottom);
        }
        
        if (isComplete)
        {
            updateCameraToFollowJon(game, 1337);
        }
        
        return isComplete ? 2 : 1;
    }
    
    return m_fStateTime >= 8.065f ? 3 : 0;
}

void Renderer::updateCameraToFollowJon(Game& game, float deltaTime)
{
    Jon& jon = game.getJon();
    m_camBounds->getLowerLeft().setX(jon.getPosition().getX() - CAM_WIDTH / 5);
    float jy = jon.getPosition().getY() - jon.getHeight() / 4 * 3;
    float jonHeightPlusPadding = jon.getHeight() * 1.5f;
    
    float regionBottomY;
    if (jon.getAbilityState() == ABILITY_BURROW)
    {
        regionBottomY = jy - 1.22451534f;
    }
    else if (jon.isFalling())
    {
        if (jy < 6.9979978125f)
        {
            regionBottomY = 0;
        }
        else if (jy < 12.76362286085f)
        {
            regionBottomY = 6.9979978125f;
        }
        else if (jy < 21.76362286085f)
        {
            regionBottomY = 12.76362286085f;
        }
        else
        {
            regionBottomY = 21.76362286085f;
        }
    }
    else
    {
        if (jy < (6.9979978125f - jonHeightPlusPadding))
        {
            regionBottomY = 0;
        }
        else if (jy < (12.76362286085 - jonHeightPlusPadding))
        {
            regionBottomY = 6.9979978125f;
        }
        else if (jy < (21.76362286085f - jonHeightPlusPadding))
        {
            regionBottomY = 12.76362286085f;
        }
        else
        {
            regionBottomY = 21.76362286085f;
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
    if (m_stopCamera && m_camBounds->getLowerLeft().getX() > farCamPos)
    {
        m_camBounds->getLowerLeft().setX(farCamPos);
    }
}

void Renderer::moveCamera(float x)
{
    m_camBounds->getLowerLeft().add(x, 0);
}

void Renderer::stopCamera()
{
    m_stopCamera = true;
}

void Renderer::zoomOut()
{
    m_camBounds->getLowerLeft().set(0, 0);
    m_camBounds->setWidth(ZOOMED_OUT_CAM_WIDTH);
    m_camBounds->setHeight(GAME_HEIGHT);
}

void Renderer::zoomIn()
{
    m_camBounds->getLowerLeft().set(0, 0);
    m_camBounds->setWidth(CAM_WIDTH);
    m_camBounds->setHeight(CAM_HEIGHT);
}

void Renderer::renderTitleScreenBackground(TitlePanel* panel)
{
    if (m_title_screen.gpuTextureWrapper == nullptr)
    {
        return;
    }
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*panel, Assets::getInstance()->get(panel));
    m_spriteBatcher->endBatch(*m_title_screen.gpuTextureWrapper);
}

void Renderer::renderTitleScreenUi(LevelEditorButton* levelEditorButton)
{
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    if (m_title_screen.gpuTextureWrapper)
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*levelEditorButton, Assets::getInstance()->get(levelEditorButton));
        m_spriteBatcher->endBatch(*m_title_screen.gpuTextureWrapper);
    }
    
    static Color fontColor = Color(1, 1, 1, 1);
    static float fgWidth = CAM_WIDTH / 32;
    static float fgHeight = fgWidth * 1.140625f;
    
    /// Version
    
    m_spriteBatcher->beginBatch();
    
    std::stringstream ss;
    ss << "v" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_BUILD << "." << VERSION_REVISION;
    std::string text = ss.str();
    m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH - fgWidth / 2, fgHeight / 2, fgWidth, fgHeight, fontColor, false, true);
    
    m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
}

void Renderer::renderWorldMapScreenBackground(WorldMapPanel* panel)
{
    if (m_world_map_screen.gpuTextureWrapper == nullptr)
    {
        return;
    }
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*panel, Assets::getInstance()->get(panel));
    m_spriteBatcher->endBatch(*m_world_map_screen.gpuTextureWrapper);
}

void Renderer::renderWorldMapScreenUi(std::vector<std::unique_ptr<LevelThumbnail>>& levelThumbnails, BackButton* backButton)
{
    if (m_world_map_screen.gpuTextureWrapper == nullptr)
    {
        return;
    }
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<std::unique_ptr<LevelThumbnail>>::iterator j = levelThumbnails.begin(); j != levelThumbnails.end(); j++)
    {
        LevelThumbnail* pLt = (*j).get();
        LevelThumbnail& lt = *pLt;
        
        renderPhysicalEntity(lt, Assets::getInstance()->get(pLt));
    }
    m_spriteBatcher->endBatch(*m_world_map_screen.gpuTextureWrapper);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*backButton, Assets::getInstance()->get(backButton));
    m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
}

void Renderer::renderWorld(Game& game)
{
    if (m_world_1_objects.gpuTextureWrapper == nullptr)
    {
        return;
    }
    
    /// Render Background
    
    updateMatrix(0, m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundUppers());
    m_spriteBatcher->endBatch(*m_world_1_background_upper.gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundMids());
    m_spriteBatcher->endBatch(*m_world_1_background_mid.gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundLowers());
    m_spriteBatcher->endBatch(*m_world_1_background_lower.gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    
    /// Render Midground
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getMidgrounds());
    m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    
    /// Render Exit Ground
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    for (std::vector<ExitGround *>::iterator i = game.getExitGrounds().begin(); i != game.getExitGrounds().end(); i++)
    {
        renderPhysicalEntity(*(*i), Assets::getInstance()->get((*i)));
        if ((*i)->hasCover())
        {
            ExitGroundCover& egc = (*i)->getExitCover();
            renderPhysicalEntityWithColor(egc, Assets::getInstance()->get(&egc), egc.getColor());
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_ground.gpuTextureWrapper);
    
    /// Render Background Midground Cover
    
    updateMatrix(0, m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundMidgroundCovers());
    m_spriteBatcher->endBatch(*m_world_1_background_lower.gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    
    /// Render World
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getGrounds());
    m_spriteBatcher->endBatch(*m_world_1_ground.gpuTextureWrapper);
    
    if (m_world_1_special.gpuTextureWrapper)
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getPits());
        m_spriteBatcher->endBatch(*m_world_1_special.gpuTextureWrapper);
    }
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Hole *>::iterator i = game.getHoles().begin(); i != game.getHoles().end(); i++)
    {
        renderPhysicalEntity(*(*i), Assets::getInstance()->get((*i)));
        if ((*i)->hasCover())
        {
            HoleCover& hc = (*i)->getHoleCover();
            renderPhysicalEntity(hc, Assets::getInstance()->get(&hc));
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<ExtraForegroundObject *>::iterator i = game.getExtraForegroundObjects().begin(); i != game.getExtraForegroundObjects().end(); i++)
    {
        ForegroundObject& shadow = (*i)->getShadow();
        renderPhysicalEntity(shadow, Assets::getInstance()->get(&shadow));
    }
    m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getCollectibleItems());
    renderPhysicalEntities(game.getForegroundObjects());
    m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    
    if (m_world_1_mid_boss_part_4.gpuTextureWrapper)
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getBossForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_4.gpuTextureWrapper);
    }
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntitiesWithColor(game.getEnemies());
    m_spriteBatcher->endBatch(*m_world_1_enemies.gpuTextureWrapper, *m_snakeDeathTextureProgram);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Enemy *>::iterator i = game.getEnemies().begin(); i != game.getEnemies().end(); i++)
    {
        if ((*i)->hasSpirit())
        {
            EnemySpirit& spirit = (*i)->getSpirit();
            renderPhysicalEntity(spirit, Assets::getInstance()->get(&spirit));
        }
    }
    m_spriteBatcher->endBatch(*m_world_1_enemies.gpuTextureWrapper);
}

void Renderer::renderJonAndExtraForegroundObjects(Game& game)
{
    if (m_vampire.gpuTextureWrapper && game.getJons().size() > 0)
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
        m_spriteBatcher->endBatch((isVampire || jon.isRevertingToRabbit()) ? *m_vampire.gpuTextureWrapper : *m_jon.gpuTextureWrapper);
        
        /// Render Jon After Images
        
        m_spriteBatcher->beginBatch();
        for (std::vector<Jon *>::iterator i = jon.getAfterImages().begin(); i != jon.getAfterImages().end(); i++)
        {
            Jon* pItem = *i;
            Jon& item = *pItem;
            renderPhysicalEntityWithColor(item, Assets::getInstance()->get(pItem), item.getColor());
        }
        m_spriteBatcher->endBatch(*m_vampire.gpuTextureWrapper);
        
        if (!jon.isConsumed())
        {
            /// Render Jon
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntitiesWithColor(game.getJons());
            m_spriteBatcher->endBatch(isVampire || isTransforming ? *m_vampire.gpuTextureWrapper : *m_jon.gpuTextureWrapper);
        }
    }
    
    if (m_world_1_objects.gpuTextureWrapper)
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getExtraForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    }
}

void Renderer::renderBounds(Game& game, int boundsLevelRequested)
{
    updateMatrix(m_camBounds->getLeft(), m_camBounds->getRight(), m_camBounds->getBottom(), m_camBounds->getTop());
    
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getMidgrounds());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getExitGrounds());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getGrounds());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getPits());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getHoles());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getForegroundObjects());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getBossForegroundObjects());
	renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getEnemies());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getCollectibleItems());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getJons());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getExtraForegroundObjects());
    
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
    
    m_lineBatcher->endBatch();
}

void Renderer::renderEntityHighlighted(PhysicalEntity& entity, Color& c)
{
	updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_highlightRectangleBatcher->beginBatch();
    renderHighlightForPhysicalEntity(entity, c);
    m_highlightRectangleBatcher->endBatch();
}

void Renderer::renderHud(Game& game, BackButton* backButton, int fps)
{
	updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    /// Render Back Button
    
    m_spriteBatcher->beginBatch();

    renderPhysicalEntity(*backButton, Assets::getInstance()->get(backButton));
    
    static Color fontColor = Color(1, 1, 1, 1);
    static float fgWidth = CAM_WIDTH / 24;
    static float fgHeight = fgWidth * 1.140625f;
    
    /// Render Play Time
    
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << game.getStateTime();
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 2, CAM_HEIGHT - fgHeight / 2, fgWidth, fgHeight, fontColor, true);
    }
    
    std::stringstream ss;
    ss << game.getNumTotalCarrots();
    std::string totalNumGoldenCarrots = ss.str();
    int offset = 1 + (int) totalNumGoldenCarrots.size();
    
    /// Render Num / Total Carrots
    
    {
        std::stringstream ss;
        ss << (game.getNumTotalCarrots() - game.getNumRemainingCarrots());
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
    
    /// Render Num / Total Golden Carrots
    
    {
        std::stringstream ss;
        ss << (game.getNumTotalGoldenCarrots() - game.getNumRemainingGoldenCarrots()) << "/" << game.getNumTotalGoldenCarrots();
        std::string text = ss.str();
        m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH - 3 * fgWidth - fgWidth / 2, CAM_HEIGHT - fgHeight - fgHeight / 2, fgWidth, fgHeight, fontColor);
    }

    m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
    
    if (m_world_1_objects.gpuTextureWrapper)
    {
        static CollectibleItem uiCarrot = Carrot(0, 0);
        static CollectibleItem uiGoldenCarrot = GoldenCarrot(0, 0);
        
        uiCarrot.getPosition().set(CAM_WIDTH - fgWidth / 2, CAM_HEIGHT - fgHeight / 2);
        uiCarrot.setWidth(fgWidth);
        uiCarrot.setHeight(fgHeight);
        
        uiGoldenCarrot.getPosition().set(CAM_WIDTH - fgWidth / 2, CAM_HEIGHT - fgHeight - fgHeight / 2);
        uiGoldenCarrot.setWidth(fgWidth);
        uiGoldenCarrot.setHeight(fgHeight);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(uiCarrot, Assets::getInstance()->get(&uiCarrot));
        renderPhysicalEntity(uiGoldenCarrot, Assets::getInstance()->get(&uiGoldenCarrot));
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    }

	{
		updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

		std::stringstream ss;
		ss << fps << " FPS";
		std::string fps_string = ss.str();

		// Render FPS

		m_spriteBatcher->beginBatch();
		m_font->renderText(*m_spriteBatcher, fps_string, CAM_WIDTH / 4, CAM_HEIGHT - fgHeight / 2, fgWidth / 2, fgHeight / 2, fontColor, true);
		m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
	}
}

void Renderer::renderMarkers(Game& game)
{
    static Color originMarkerColor = Color(0, 1, 0, 0.5f);
    static Color endMarkerColor = Color(1, 0, 0, 0.5f);
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_highlightRectangleBatcher->beginBatch();
    
    for (std::vector<Marker *>::iterator i = game.getMarkers().begin(); i != game.getMarkers().end(); i++)
    {
        Rectangle& marker = (*i)->getMainBounds();
        m_highlightRectangleBatcher->renderRectangle(marker, (*i)->getType() == 0 ? originMarkerColor : endMarkerColor);
    }
    
    m_highlightRectangleBatcher->endBatch();
}

void Renderer::renderLevelEditor(LevelEditorActionsPanel* leap, LevelEditorEntitiesPanel* leep, TrashCan* tc, LevelSelectorPanel* lsp)
{
    if (m_level_editor.gpuTextureWrapper == nullptr)
    {
        return;
    }
    
	static Rectangle originMarker = Rectangle(0, 0, 0.1f, GAME_HEIGHT);
    static Color originMarkerColor = Color(0, 0, 0, 0.7f);
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_highlightRectangleBatcher->beginBatch();
    m_highlightRectangleBatcher->renderRectangle(originMarker, originMarkerColor);
    m_highlightRectangleBatcher->endBatch();
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    /// Render Level Editor
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*leap, Assets::getInstance()->get(leap));
    renderPhysicalEntity(*leep, Assets::getInstance()->get(leep));
    m_spriteBatcher->endBatch(*m_level_editor.gpuTextureWrapper);
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*tc, Assets::getInstance()->get(tc));
    m_spriteBatcher->endBatch(*m_level_editor.gpuTextureWrapper);
    
    if (leep->isOpen() && m_world_1_objects.gpuTextureWrapper)
    {
        updateMatrix(0, CAM_WIDTH, leep->getEntitiesCameraPos(), leep->getEntitiesCameraPos() + CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getMidgrounds());
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getGrounds());
        renderPhysicalEntities(leep->getExitGrounds());
        m_spriteBatcher->endBatch(*m_world_1_ground.gpuTextureWrapper);
        
        if (m_world_1_special.gpuTextureWrapper)
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntities(leep->getPits());
            m_spriteBatcher->endBatch(*m_world_1_special.gpuTextureWrapper);
        }
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getHoles());
        renderPhysicalEntities(leep->getCollectibleItems());
        renderPhysicalEntities(leep->getForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
        
        if (m_world_1_mid_boss_part_4.gpuTextureWrapper)
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntities(leep->getBossForegroundObjects());
            m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_4.gpuTextureWrapper);
        }
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getEnemies());
        m_spriteBatcher->endBatch(*m_world_1_enemies.gpuTextureWrapper);
        
        if (m_vampire.gpuTextureWrapper)
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntities(leep->getJons());
            m_spriteBatcher->endBatch(*m_jon.gpuTextureWrapper);
        }
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getExtraForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    }
    
    if (lsp->isOpen())
    {
        updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*lsp, Assets::getInstance()->get(lsp));
        m_spriteBatcher->endBatch(*m_level_editor.gpuTextureWrapper);
        
        static Color fontColor = Color(1, 1, 1, 1);
        
        float fgWidth = lsp->getTextSize();
        float fgHeight = fgWidth * 1.140625f;
        
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
        m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
    }

	{
		static Color fontColor = Color(1, 1, 1, 1);
		static float fgWidth = CAM_WIDTH / 24;
		static float fgHeight = fgWidth * 1.140625f;

		updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

		std::stringstream ss;
		ss << "World " << lsp->getWorld() << " / Level " << lsp->getLevel();
		std::string text_string = ss.str();

		// Render FPS

		m_spriteBatcher->beginBatch();
		m_font->renderText(*m_spriteBatcher, text_string, CAM_WIDTH / 4, CAM_HEIGHT - fgHeight / 2, fgWidth / 2, fgHeight / 2, fontColor, true);
		m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
	}
}

void Renderer::renderLoading()
{
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static Animation anim = Animation(0, 700, 182, 176, 364, 176, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024, true, 0.1f, 2);
    TextureRegion tr = anim.getTextureRegion(m_fStateTime);
    
    static float width = 1.599609375f;
    static float height = 1.546875f;
    static float x = CAM_WIDTH - width / 2;
    static float y = height / 2;
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(x, y, width, height, 0, tr);
    m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
}

void Renderer::renderToSecondFramebufferWithShockwave(float centerX, float centerY, float timeElapsed, bool isTransforming)
{
    m_shockwaveTextureGpuProgramWrapper->configure(centerX, centerY, timeElapsed + 0.1f, isTransforming);
    
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
    
    m_transDeathInGpuProgramWrapper->configure(m_trans_death_shader_helper.gpuTextureWrapper, timeElapsed);
    
    bindToScreenFramebuffer();
    
    clearFramebufferWithColor(0, 0, 0, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(m_iFramebufferIndex), *m_transDeathInGpuProgramWrapper);
}

void Renderer::renderToScreenWithTransDeathOut(float timeElapsed)
{
    /// Render the new game to the screen
    
    m_transDeathOutGpuProgramWrapper->configure(m_trans_death_shader_helper.gpuTextureWrapper, timeElapsed);
    
    bindToScreenFramebuffer();
    
    clearFramebufferWithColor(0, 0, 0, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(m_iFramebufferIndex), *m_transDeathOutGpuProgramWrapper);
}

void Renderer::renderToScreenTransition(float progress)
{
    /// Render the screen transition to the screen
    
    m_transScreenGpuProgramWrapper->configure(&m_framebuffers.at(1), progress);
    
    bindToScreenFramebuffer();
    
    clearFramebufferWithColor(0, 0, 0, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(0), *m_transScreenGpuProgramWrapper);
}

void Renderer::renderToScreenWithRadialBlur()
{
    /// Render everything to the screen with a radial blur
    
    m_framebufferRadialBlurGpuProgramWrapper->configure(m_iRadialBlurDirection);
    
    bindToScreenFramebuffer();
    
    clearFramebufferWithColor(0, 0, 0, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(m_iFramebufferIndex), *m_framebufferRadialBlurGpuProgramWrapper);
}

void Renderer::renderToScreen()
{
	/// Render everything to the screen
    
    bindToScreenFramebuffer();
    
    clearFramebufferWithColor(0, 0, 0, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(m_iFramebufferIndex), *m_framebufferToScreenGpuProgramWrapper);
}

void Renderer::cleanUp()
{
    cleanUpTextures();
    
    for (std::vector<std::thread>::iterator i = m_threads.begin(); i != m_threads.end(); i++)
    {
        (*i).join();
    }
    
    m_threads.clear();
    
    if (m_areShadersLoaded)
    {
        m_transScreenGpuProgramWrapper->cleanUp();
        m_sinWaveTextureProgram->cleanUp();
        m_backgroundGpuTextureProgramWrapper->cleanUp();
        m_snakeDeathTextureProgram->cleanUp();
        m_shockwaveTextureGpuProgramWrapper->cleanUp();
        m_transDeathInGpuProgramWrapper->cleanUp();
        m_transDeathOutGpuProgramWrapper->cleanUp();
        m_framebufferToScreenGpuProgramWrapper->cleanUp();
        m_framebufferTintGpuProgramWrapper->cleanUp();
        m_framebufferRadialBlurGpuProgramWrapper->cleanUp();
        
        m_areShadersLoaded = false;
    }
    
    m_framebuffers.clear();
}

Rectangle& Renderer::getCameraBounds()
{
    return *m_camBounds;
}

Vector2D& Renderer::getCameraPosition()
{
    return m_camBounds->getLowerLeft();
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
    
    renderBoundsWithColor(pe.getMainBounds(), red);
}

void Renderer::renderBoundsWithColor(Rectangle &r, Color& c)
{
    m_boundsRectangleBatcher->renderRectangle(r, c);
}

void Renderer::renderHighlightForPhysicalEntity(PhysicalEntity &pe, Color &c)
{
    m_highlightRectangleBatcher->renderRectangle(pe.getMainBounds(), c);
}

float Renderer::getCamPosFarRight(Game &game)
{
    float farRight = game.getFarRight();
    float farCamPos = farRight - CAM_WIDTH;
    
    return farCamPos;
}

void Renderer::loadMisc()
{
    if (m_misc.gpuTextureWrapper == nullptr)
    {
        m_misc.gpuTextureDataWrapper = loadTextureData("misc");
        m_misc.gpuTextureWrapper = loadTexture(m_misc.gpuTextureDataWrapper);
        
        delete m_misc.gpuTextureDataWrapper;
        m_misc.gpuTextureDataWrapper = nullptr;
    }
}

void Renderer::loadTitle()
{
    if (m_title_screen.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_title_screen.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_title_screen" : "title_screen");
        }, this));
    }
}

void Renderer::loadWorldMap()
{
    if (m_world_map_screen.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_map_screen.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_map_screen" : "world_map_screen");
        }, this));
    }
}

void Renderer::loadLevelEditor()
{
    if (m_level_editor.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_level_editor.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_level_editor" : "level_editor");
        }, this));
    }
}

void Renderer::loadJon()
{
    if (m_vampire.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads += 3;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_jon.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_jon" : "jon");
            r->m_trans_death_shader_helper.gpuTextureDataWrapper = r->loadTextureData("trans_death_shader_helper");
            r->m_vampire.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_vampire" : "vampire");
        }, this));
    }
}

void Renderer::loadWorld1()
{
    if (m_world_1_objects.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads += 6;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_background_lower.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_background_lower" : "world_1_background_lower");
            r->m_world_1_background_mid.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_background_mid" : "world_1_background_mid");
            r->m_world_1_background_upper.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_background_upper" : "world_1_background_upper");
            
            r->m_world_1_enemies.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_enemies" : "world_1_enemies");
            r->m_world_1_ground.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_ground" : "world_1_ground");
            r->m_world_1_objects.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_objects" : "world_1_objects");
        }, this));
    }
}

void Renderer::loadWorld1Special()
{
    if (m_world_1_special.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_special.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_special" : "world_1_special");
        }, this));
    }
}

void Renderer::loadWorld1MidBoss()
{
    if (m_world_1_mid_boss_part_4.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads += 4;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_mid_boss_part_1.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_mid_boss_part_1" : "world_1_mid_boss_part_1");
            r->m_world_1_mid_boss_part_2.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_mid_boss_part_2" : "world_1_mid_boss_part_2");
            r->m_world_1_mid_boss_part_3.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_mid_boss_part_3" : "world_1_mid_boss_part_3");
            r->m_world_1_mid_boss_part_4.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_mid_boss_part_4" : "world_1_mid_boss_part_4");
        }, this));
    }
}

void Renderer::loadWorld1EndBoss()
{
    // TODO
}

void Renderer::loadWorld2()
{
    // TODO
}

void Renderer::loadWorld2MidBoss()
{
    // TODO
}

void Renderer::loadWorld2EndBoss()
{
    // TODO
}

void Renderer::loadWorld3()
{
    // TODO
}

void Renderer::loadWorld3MidBoss()
{
    // TODO
}

void Renderer::loadWorld3EndBoss()
{
    // TODO
}

void Renderer::loadWorld4()
{
    // TODO
}

void Renderer::loadWorld4MidBoss()
{
    // TODO
}

void Renderer::loadWorld4EndBoss()
{
    // TODO
}

void Renderer::loadWorld5()
{
    // TODO
}

void Renderer::loadWorld5MidBoss()
{
    // TODO
}

void Renderer::loadWorld5EndBoss()
{
    // TODO
}

void Renderer::unloadTitle()
{
    destroyTexture(&m_title_screen);
}

void Renderer::unloadWorldMap()
{
    destroyTexture(&m_world_map_screen);
}

void Renderer::unloadLevelEditor()
{
    destroyTexture(&m_level_editor);
}

void Renderer::unloadJon()
{
    destroyTexture(&m_jon);
    destroyTexture(&m_trans_death_shader_helper);
    destroyTexture(&m_vampire);
}

void Renderer::unloadWorld1()
{
    destroyTexture(&m_world_1_background_lower);
    destroyTexture(&m_world_1_background_mid);
    destroyTexture(&m_world_1_background_upper);
    
    destroyTexture(&m_world_1_enemies);
    destroyTexture(&m_world_1_ground);
    destroyTexture(&m_world_1_objects);
}

void Renderer::unloadWorld1Special()
{
    destroyTexture(&m_world_1_special);
}

void Renderer::unloadWorld1MidBoss()
{
    destroyTexture(&m_world_1_mid_boss_part_1);
    destroyTexture(&m_world_1_mid_boss_part_2);
    destroyTexture(&m_world_1_mid_boss_part_3);
    destroyTexture(&m_world_1_mid_boss_part_4);
}

void Renderer::unloadWorld1EndBoss()
{
    // TODO
}

void Renderer::unloadWorld2()
{
    // TODO
}

void Renderer::unloadWorld2MidBoss()
{
    // TODO
}

void Renderer::unloadWorld2EndBoss()
{
    // TODO
}

void Renderer::unloadWorld3()
{
    // TODO
}

void Renderer::unloadWorld3MidBoss()
{
    // TODO
}

void Renderer::unloadWorld3EndBoss()
{
    // TODO
}

void Renderer::unloadWorld4()
{
    // TODO
}

void Renderer::unloadWorld4MidBoss()
{
    // TODO
}

void Renderer::unloadWorld4EndBoss()
{
    // TODO
}

void Renderer::unloadWorld5()
{
    // TODO
}

void Renderer::unloadWorld5MidBoss()
{
    // TODO
}

void Renderer::unloadWorld5EndBoss()
{
    // TODO
}

void Renderer::handleAsyncTextureLoads()
{
    for (std::vector<TextureWrapper *>::iterator i = m_textureWrappers.begin(); i != m_textureWrappers.end(); i++)
    {
        if ((*i)->gpuTextureDataWrapper)
        {
            (*i)->gpuTextureWrapper = loadTexture((*i)->gpuTextureDataWrapper, (*i)->repeat_s);
            
            delete (*i)->gpuTextureDataWrapper;
            (*i)->gpuTextureDataWrapper = nullptr;
            
            m_iNumAsyncLoads--;
        }
    }
}

void Renderer::cleanUpTextures()
{
    for (std::vector<TextureWrapper *>::iterator i = m_textureWrappers.begin(); i != m_textureWrappers.end(); i++)
    {
        destroyTexture((*i));
    }
}

void Renderer::destroyTexture(TextureWrapper* textureWrapper)
{
    if (textureWrapper->gpuTextureWrapper)
    {
        destroyTexture(*textureWrapper->gpuTextureWrapper);
        
        delete textureWrapper->gpuTextureWrapper;
        textureWrapper->gpuTextureWrapper = nullptr;
    }
    
    if (textureWrapper->gpuTextureDataWrapper)
    {
        delete textureWrapper->gpuTextureDataWrapper;
        textureWrapper->gpuTextureDataWrapper = nullptr;
    }
}
