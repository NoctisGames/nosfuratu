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
#include "GameButton.h"
#include "GameScreenLevelEditor.h"
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
#include "GameScreenWorldMap.h"
#include "macros.h"
#include "BatPanel.h"
#include "MidBossOwl.h"
#include "FlagUtil.h"
#include "MathUtil.h"
#include "GameScreen.h"
#include "GameTracker.h"
#include "EndBossSnake.h"

#include <math.h>
#include <sstream>
#include <iomanip>

Renderer::Renderer() :
m_fStateTime(0),
m_fCamPosX(0),
m_iFramebufferIndex(0),
m_iNumAsyncLoads(0),
m_fRadialBlurDirection(0.5f),
m_compressed(Assets::getInstance()->isUsingCompressedTextureSet()),
m_areShadersLoaded(false),
m_stopCamera(false),
m_hasCompletedRadialBlur(false),
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
m_framebufferToScreenGpuProgramWrapper(nullptr),
m_framebufferTintGpuProgramWrapper(nullptr),
m_framebufferRadialBlurGpuProgramWrapper(nullptr)
{
    m_font = std::unique_ptr<Font>(new Font(0, 0, 16, 64, 75, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024));
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
    m_textureWrappers.push_back(&m_world_1_cutscene_1);
    m_textureWrappers.push_back(&m_world_1_cutscene_2);
    m_textureWrappers.push_back(&m_world_1_end_boss_part_1);
    m_textureWrappers.push_back(&m_world_1_end_boss_part_2);
    m_textureWrappers.push_back(&m_world_1_end_boss_part_3);
    m_textureWrappers.push_back(&m_world_1_enemies);
    m_textureWrappers.push_back(&m_world_1_ground);
    m_textureWrappers.push_back(&m_world_1_mid_boss_part_1);
    m_textureWrappers.push_back(&m_world_1_mid_boss_part_2);
    m_textureWrappers.push_back(&m_world_1_mid_boss_part_3);
    m_textureWrappers.push_back(&m_world_1_objects);
    m_textureWrappers.push_back(&m_world_1_special);
    m_textureWrappers.push_back(&m_world_map_screen_part_1);
    m_textureWrappers.push_back(&m_world_map_screen_part_2);
}

Renderer::~Renderer()
{
    cleanUp();
}

void Renderer::init(RendererType rendererType)
{
	m_stopCamera = false;
    m_fStateTime = 0;
    
    loadMiscTextures();
    
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
    
    switch (rendererType)
    {
        case RENDERER_TYPE_TITLE:
            loadTitleTextures();
            break;
        case RENDERER_TYPE_WORLD_MAP:
            loadWorldMapTextures();
            break;
        case RENDERER_TYPE_LEVEL_EDITOR:
            loadLevelEditorTextures();
            break;
            
        case RENDERER_TYPE_WORLD_1_CUTSCENE:
            loadWorld1CutsceneTextures();
            break;
            
        case RENDERER_TYPE_WORLD_1:
            loadWorld1Textures();
            break;
        case RENDERER_TYPE_WORLD_1_MID_BOSS:
            loadWorld1MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_1_END_BOSS:
            loadWorld1EndBossTextures();
            break;
            
        case RENDERER_TYPE_WORLD_2:
            loadWorld2Textures();
            break;
        case RENDERER_TYPE_WORLD_2_MID_BOSS:
            loadWorld2MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_2_END_BOSS:
            loadWorld2EndBossTextures();
            break;
            
        case RENDERER_TYPE_WORLD_3:
            loadWorld3Textures();
            break;
        case RENDERER_TYPE_WORLD_3_MID_BOSS:
            loadWorld3MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_3_END_BOSS:
            loadWorld3EndBossTextures();
            break;
            
        case RENDERER_TYPE_WORLD_4:
            loadWorld4Textures();
            break;
        case RENDERER_TYPE_WORLD_4_MID_BOSS:
            loadWorld4MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_4_END_BOSS:
            loadWorld4EndBossTextures();
            break;
            
        case RENDERER_TYPE_WORLD_5:
            loadWorld5Textures();
            break;
        case RENDERER_TYPE_WORLD_5_MID_BOSS:
            loadWorld5MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_5_END_BOSS:
            loadWorld5EndBossTextures();
            break;

		case RENDERER_TYPE_NONE:
        default:
            break;
    }
}

void Renderer::unload(RendererType rendererType)
{
    switch (rendererType)
    {
        case RENDERER_TYPE_TITLE:
            unloadTitleTextures();
            break;
        case RENDERER_TYPE_WORLD_MAP:
            unloadWorldMapTextures();
            break;
        case RENDERER_TYPE_LEVEL_EDITOR:
            unloadLevelEditorTextures();
            break;
            
        case RENDERER_TYPE_WORLD_1_CUTSCENE:
            unloadWorld1CutsceneTextures();
            break;
            
        case RENDERER_TYPE_WORLD_1:
            unloadWorld1Textures();
            break;
        case RENDERER_TYPE_WORLD_1_MID_BOSS:
            unloadWorld1MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_1_END_BOSS:
            unloadWorld1EndBossTextures();
            break;
            
        case RENDERER_TYPE_WORLD_2:
            unloadWorld2Textures();
            break;
        case RENDERER_TYPE_WORLD_2_MID_BOSS:
            unloadWorld2MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_2_END_BOSS:
            unloadWorld2EndBossTextures();
            break;
            
        case RENDERER_TYPE_WORLD_3:
            unloadWorld3Textures();
            break;
        case RENDERER_TYPE_WORLD_3_MID_BOSS:
            unloadWorld3MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_3_END_BOSS:
            unloadWorld3EndBossTextures();
            break;
            
        case RENDERER_TYPE_WORLD_4:
            unloadWorld4Textures();
            break;
        case RENDERER_TYPE_WORLD_4_MID_BOSS:
            unloadWorld4MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_4_END_BOSS:
            unloadWorld4EndBossTextures();
            break;
            
        case RENDERER_TYPE_WORLD_5:
            unloadWorld5Textures();
            break;
        case RENDERER_TYPE_WORLD_5_MID_BOSS:
            unloadWorld5MidBossTextures();
            break;
        case RENDERER_TYPE_WORLD_5_END_BOSS:
            unloadWorld5EndBossTextures();
            break;

		case RENDERER_TYPE_NONE:
        default:
            break;
    }
}

bool Renderer::isLoadingAdditionalTextures()
{
    return m_iNumAsyncLoads > 0 || m_pendingLoadFunctions.size() > 0;
}

bool Renderer::isLoaded()
{
    return m_misc.gpuTextureWrapper
    && m_transScreenGpuProgramWrapper->isLoaded()
    && m_fadeScreenGpuProgramWrapper->isLoaded()
    && m_pointTransScreenGpuProgramWrapper->isLoaded()
    && m_sinWaveTextureProgram->isLoaded()
    && m_backgroundGpuTextureProgramWrapper->isLoaded()
    && m_snakeDeathTextureProgram->isLoaded()
    && m_endBossSnakeTextureProgram->isLoaded()
    && m_shockwaveTextureGpuProgramWrapper->isLoaded()
    && m_framebufferToScreenGpuProgramWrapper->isLoaded()
    && m_framebufferTintGpuProgramWrapper->isLoaded()
    && m_framebufferObfuscationGpuProgramWrapper->isLoaded()
    && m_framebufferRadialBlurGpuProgramWrapper->isLoaded()
    && m_transDeathInGpuProgramWrapper->isLoaded()
    && m_transDeathOutGpuProgramWrapper->isLoaded();
}

void Renderer::beginFrame(float deltaTime)
{
    setFramebuffer(0);
    
    m_fStateTime += deltaTime;
    
	if (m_iNumAsyncLoads > 0)
	{
		handleAsyncTextureLoads();
	}
    else if (m_pendingLoadFunctions.size() > 0)
    {
        void (Renderer::*function)() = m_pendingLoadFunctions.at(0);
        ((*this).*function)();
        m_pendingLoadFunctions.erase(m_pendingLoadFunctions.begin());
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
    m_hasCompletedRadialBlur = false;
    
    updateCameraToFollowJon(game, nullptr, 1337);
    
    m_camBounds->getLowerLeft().setX(getCamPosFarRight(game));
    m_camBounds->getLowerLeft().setY(game.getFarRightBottom());
    
    Jon& jon = game.getJon();
    float farLeft = jon.getPosition().getX() - CAM_WIDTH / 6;
	float farLeftBottom = jon.getMainBounds().getBottom() - 0.5625f;
    
    float changeInX = farLeft - getCamPosFarRight(game);
    float changeInY = farLeftBottom - game.getFarRightBottom();
    
    m_camPosVelocity->set(changeInX, changeInY);
    
    Vector2D camPosVelocityNormal = m_camPosVelocity->cpy().nor();
    float angle = camPosVelocityNormal.angle();
    float tanfAngle = tanf(DEGREES_TO_RADIANS(angle));
    float y = tanfAngle * camPosVelocityNormal.getX();
    m_fRadialBlurDirection = y / 2.0f + 0.5f;
}

int Renderer::updateCameraToFollowPathToJon(Game& game)
{
    if (m_fStateTime >= 1.75f)
    {
        bool isComplete = false;
        float progress = (m_fStateTime - 1.75f) / 0.2f;
        if (progress > 1)
        {
            progress = 1;
            isComplete = true;
        }
        
        Jon& jon = game.getJon();
        float farLeft = jon.getPosition().getX() - CAM_WIDTH / 6;
        float farLeftBottom = jon.getMainBounds().getBottom() - 0.5625f;
        
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
            updateCameraToFollowJon(game, nullptr, 1337);
            
            if (!m_hasCompletedRadialBlur)
            {
                m_hasCompletedRadialBlur = true;
                
                return 2;
            }
        }
        else if (m_fStateTime < 2)
        {
            return 1;
        }
        
        return m_fStateTime >= 4.0f ? 3 : 0;
    }
    
    return 0;
}

void Renderer::updateCameraToFollowJon(Game& game, BatPanel* batPanel, float deltaTime, float paddingX, bool chase, bool ignoreY, bool instant)
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
        else if (yFactor < 21.76362286085f)
        {
            regionBottomY = 12.76362286085f;
        }
		else if (yFactor < 28)
		{
			regionBottomY = 21.76362286085f;
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
        else if (yFactor < (21.76362286085f - heightPlusPadding))
        {
            regionBottomY = 12.76362286085f;
        }
		else if (yFactor < (28 - heightPlusPadding))
		{
			regionBottomY = 21.76362286085f;
		}
        else
        {
            regionBottomY = GAME_HEIGHT - CAM_HEIGHT;
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

void Renderer::moveCamera(float x)
{
    m_camBounds->getLowerLeft().add(x, 0);
}

void Renderer::stopCamera()
{
	m_fCamPosX = m_camBounds->getLeft();
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
    if (!ensureTitleTextures())
    {
        return;
    }
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*panel, Assets::getInstance()->get(panel), true);
    m_spriteBatcher->endBatch(*m_title_screen.gpuTextureWrapper);
}

void Renderer::renderTitleScreenUi(GameButton* levelEditorButton)
{
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
#if NG_LEVEL_EDITOR
    if (ensureTitleTextures())
    {
        m_spriteBatcher->beginBatch();
        
        renderPhysicalEntity(*levelEditorButton, Assets::getInstance()->get(levelEditorButton), true);
        
        m_spriteBatcher->endBatch(*m_title_screen.gpuTextureWrapper);
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
    
    m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
}

void Renderer::renderCutscene(std::vector<CutscenePanel*> cutscenePanels)
{
    if (!ensureWorld1CutsceneTextures())
    {
        return;
    }
    
    for (std::vector<CutscenePanel *>::iterator i = cutscenePanels.begin(); i != cutscenePanels.end(); i++)
    {
        m_spriteBatcher->beginBatch();
        
        Rectangle& cb = (*i)->getCamBounds();
        updateMatrix(cb.getLowerLeft().getX(), cb.getWidth(), cb.getLowerLeft().getY(), cb.getHeight());
        
        renderPhysicalEntityWithColor(*(*i), Assets::getInstance()->get((*i)), (*i)->getColor(), true);
        
        m_spriteBatcher->endBatch(*m_world_1_cutscene_1.gpuTextureWrapper);
        
        m_spriteBatcher->beginBatch();
        
        renderPhysicalEntitiesWithColor((*i)->getCutsceneEffects(), true);
        
        m_spriteBatcher->endBatch(*m_world_1_cutscene_2.gpuTextureWrapper);
    }
}

void Renderer::renderWorldMapScreenBackground(WorldMapPanel* panel)
{
    if (!ensureWorldMapTextures())
    {
        return;
    }
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*panel, Assets::getInstance()->get(panel), true);
    m_spriteBatcher->endBatch(*m_world_map_screen_part_1.gpuTextureWrapper);
}

void Renderer::renderWorldMapScreenUi(WorldMap& wm)
{
    if (!ensureWorldMapTextures())
    {
        return;
    }
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntitiesWithColor(wm.getAbilitySlots(), true);
    renderPhysicalEntitiesWithColor(wm.getLevelThumbnails(), true);
    renderPhysicalEntityWithColor(*wm.getGoldenCarrotsMarker(), Assets::getInstance()->get(wm.getGoldenCarrotsMarker()), wm.getGoldenCarrotsMarker()->getColor(), true);
    m_spriteBatcher->endBatch(*m_world_map_screen_part_2.gpuTextureWrapper);
    
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
    
    m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
}

void Renderer::renderWorldMapScreenButtons(WorldMap& wm)
{
    if (!ensureWorldMapTextures())
    {
        return;
    }
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntityWithColor(*wm.getBackButton(), Assets::getInstance()->get(wm.getBackButton()), wm.getBackButton()->getColor(), true);
    renderPhysicalEntityWithColor(*wm.getToggleMusicButton(), Assets::getInstance()->get(wm.getToggleMusicButton()), wm.getToggleMusicButton()->getColor(), true);
    renderPhysicalEntityWithColor(*wm.getToggleSoundButton(), Assets::getInstance()->get(wm.getToggleSoundButton()), wm.getToggleSoundButton()->getColor(), true);
    //renderPhysicalEntityWithColor(*wm.getLeaderBoardsButton(), Assets::getInstance()->get(wm.getLeaderBoardsButton()), wm.getLeaderBoardsButton()->getColor(), true);
    renderPhysicalEntityWithColor(*wm.getViewOpeningCutsceneButton(), Assets::getInstance()->get(wm.getViewOpeningCutsceneButton()), wm.getViewOpeningCutsceneButton()->getColor(), true);
    renderPhysicalEntityWithColor(*wm.getSpendGoldenCarrotsBubble(), Assets::getInstance()->get(wm.getSpendGoldenCarrotsBubble()), wm.getSpendGoldenCarrotsBubble()->getColor(), true);
    m_spriteBatcher->endBatch(*m_world_map_screen_part_1.gpuTextureWrapper);
}

void Renderer::renderWorld(Game& game)
{
    if (!ensureWorld1Textures())
    {
        return;
    }
    
    /// Render Background
    
    updateMatrix(0, m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundUppers(), true);
    m_spriteBatcher->endBatch(*m_world_1_background_upper.gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundMids(), true);
    m_spriteBatcher->endBatch(*m_world_1_background_mid.gpuTextureWrapper, *m_backgroundGpuTextureProgramWrapper);
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getBackgroundLowers(), true);
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
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntities(game.getPits());
    m_spriteBatcher->endBatch(*m_world_1_special.gpuTextureWrapper);
    
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
    
    for (std::vector<ExtraForegroundObject *>::iterator i = game.getExtraForegroundObjects().begin(); i != game.getExtraForegroundObjects().end(); i++)
    {
        ForegroundObject& shadow = (*i)->getShadow();
        renderPhysicalEntity(shadow, Assets::getInstance()->get(&shadow));
    }
    
    renderPhysicalEntitiesWithColor(game.getCollectibleItems());
    renderPhysicalEntities(game.getForegroundObjects());
    m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    
    if (ensureWorld1MidBossTextures())
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getMidBossForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_3.gpuTextureWrapper);
    }
    
    if (ensureWorld1EndBossTextures())
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntitiesWithColor(game.getEndBossForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper);
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

	for (std::vector<EndBossSnake *>::iterator i = game.getEndBossSnakes().begin(); i != game.getEndBossSnakes().end(); i++)
	{
		renderEndBossSnake(*(*i));
	}
}

void Renderer::renderJonAndExtraForegroundObjects(Game& game)
{
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    if (ensureJonTextures()
        && game.getJons().size() > 0)
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

	if (game.getCountHissWithMinas().size() > 0)
	{
		CountHissWithMina *chwm = game.getCountHissWithMinas().at(0);
		if ((!chwm->isFacingLeft() && m_jon.gpuTextureWrapper)
			|| (chwm->isFacingLeft() && m_world_1_end_boss_part_1.gpuTextureWrapper))
		{
			m_spriteBatcher->beginBatch();
			renderPhysicalEntities(game.getCountHissWithMinas());
			m_spriteBatcher->endBatch(chwm->isFacingLeft() ? *m_world_1_end_boss_part_1.gpuTextureWrapper : *m_jon.gpuTextureWrapper);
		}
	}
    
    if (m_world_1_objects.gpuTextureWrapper)
    {
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(game.getExtraForegroundObjects());
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    }
}

void Renderer::renderMidBossOwl(MidBossOwl& midBossOwl)
{
    if (ensureWorld1MidBossTextures())
    {
        return;
    }
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    switch (midBossOwl.getState())
    {
        case MidBossOwlState_Sleeping:
        case MidBossOwlState_Awakening:
        case MidBossOwlState_Screeching:
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntity(midBossOwl, Assets::getInstance()->get(&midBossOwl));
            m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_3.gpuTextureWrapper);
        }
            break;
        case MidBossOwlState_Pursuing:
        case MidBossOwlState_FlyingOverTree:
        case MidBossOwlState_SwoopingDown:
        case MidBossOwlState_FlyingAwayAfterCatchingJon:
        case MidBossOwlState_SlammingIntoTree:
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntity(midBossOwl, Assets::getInstance()->get(&midBossOwl));
            m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_1.gpuTextureWrapper);
        }
            break;
        case MidBossOwlState_Dying:
        case MidBossOwlState_Dead:
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntity(midBossOwl, Assets::getInstance()->get(&midBossOwl));
            m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_2.gpuTextureWrapper);
        }
            break;
        default:
            break;
    }
}

void Renderer::renderEndBossSnake(EndBossSnake& endBossSnake)
{
    if (!m_world_1_end_boss_part_1.gpuTextureWrapper
        || !m_world_1_end_boss_part_2.gpuTextureWrapper
        || !m_world_1_end_boss_part_3.gpuTextureWrapper)
    {
        return;
    }
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
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
                renderPhysicalEntityWithColor(item.getSnakeBody(), Assets::getInstance()->get(&item.getSnakeBody()), item.getSnakeBody().getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
                
                if (item.getSnakeTonque().isMouthOpen())
                {
                    m_spriteBatcher->beginBatch();
                    renderPhysicalEntity(item.getSnakeTonque(), Assets::getInstance()->get(&item.getSnakeTonque()));
                    m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper);
                }
                
                m_spriteBatcher->beginBatch();
                renderPhysicalEntityWithColor(item, Assets::getInstance()->get(&item), item.getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
                
                m_spriteBatcher->beginBatch();
                renderPhysicalEntity(item.getSnakeEye(), Assets::getInstance()->get(&item.getSnakeEye()));
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper);
            }
                break;
            case EndBossSnakeState_ChargingRight:
            {
                m_spriteBatcher->beginBatch();
                renderPhysicalEntityWithColor(item.getSnakeBody(), Assets::getInstance()->get(&item.getSnakeBody()), item.getSnakeBody().getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
                
                if (item.getSnakeTonque().isMouthOpen())
                {
                    m_spriteBatcher->beginBatch();
                    renderPhysicalEntity(item.getSnakeTonque(), Assets::getInstance()->get(&item.getSnakeTonque()));
                    m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper);
                }
                
                m_spriteBatcher->beginBatch();
                renderPhysicalEntityWithColor(item, Assets::getInstance()->get(&item), item.getColor());
                renderPhysicalEntityWithColor(item.getSnakeSkin(), Assets::getInstance()->get(&item.getSnakeSkin()), item.getSnakeSkin().getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
                
                m_spriteBatcher->beginBatch();
                renderPhysicalEntityWithColor(item.getSnakeHeadImpact(), Assets::getInstance()->get(&item.getSnakeHeadImpact()), item.getSnakeHeadImpact().getColor());
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper);
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
            renderPhysicalEntityWithColor(endBossSnake.getSnakeBody(), Assets::getInstance()->get(&endBossSnake.getSnakeBody()), endBossSnake.getSnakeBody().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            if (endBossSnake.getSnakeTonque().isMouthOpen())
            {
                m_spriteBatcher->beginBatch();
                renderPhysicalEntity(endBossSnake.getSnakeTonque(), Assets::getInstance()->get(&endBossSnake.getSnakeTonque()));
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper);
            }
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake, Assets::getInstance()->get(&endBossSnake), endBossSnake.getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntity(endBossSnake.getSnakeEye(), Assets::getInstance()->get(&endBossSnake.getSnakeEye()));
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper);
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
			renderPhysicalEntityWithColor(endBossSnake.getSnakeBody(), Assets::getInstance()->get(&endBossSnake.getSnakeBody()), endBossSnake.getSnakeBody().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            if (endBossSnake.getSnakeTonque().isMouthOpen())
            {
                m_spriteBatcher->beginBatch();
                renderPhysicalEntity(endBossSnake.getSnakeTonque(), Assets::getInstance()->get(&endBossSnake.getSnakeTonque()));
                m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper);
            }
            
			m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake, Assets::getInstance()->get(&endBossSnake), endBossSnake.getColor());
			renderPhysicalEntityWithColor(endBossSnake.getSnakeSkin(), Assets::getInstance()->get(&endBossSnake.getSnakeSkin()), endBossSnake.getSnakeSkin().getColor());
			m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake.getSnakeHeadImpact(), Assets::getInstance()->get(&endBossSnake.getSnakeHeadImpact()), endBossSnake.getSnakeHeadImpact().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper);
        }
            break;
        case EndBossSnakeState_Dying:
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake.getSnakeBody(), Assets::getInstance()->get(&endBossSnake.getSnakeBody()), endBossSnake.getSnakeBody().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake, Assets::getInstance()->get(&endBossSnake), endBossSnake.getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_3.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
        }
            break;
		case EndBossSnakeState_DeadSpiritReleasing:
        case EndBossSnakeState_Dead:
        {
            m_spriteBatcher->beginBatch();
			renderPhysicalEntityWithColor(endBossSnake, Assets::getInstance()->get(&endBossSnake), endBossSnake.getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_3.gpuTextureWrapper, *m_endBossSnakeTextureProgram);
            
            m_spriteBatcher->beginBatch();
            renderPhysicalEntityWithColor(endBossSnake.getSnakeSpirit(), Assets::getInstance()->get(&endBossSnake.getSnakeSpirit()), endBossSnake.getSnakeSpirit().getColor());
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_2.gpuTextureWrapper);
        }
            break;
        default:
            break;
    }
}

void Renderer::renderBatPanel(BatPanel& batPanel)
{
    if (!m_jon.gpuTextureWrapper
        || !m_vampire.gpuTextureWrapper)
    {
        // All Bat Bubbles require at least jon texture for the opening animation
        // and the vampire texture for the bat poofing in animation
        return;
    }
    
    Bat* bat = batPanel.getBat();
    BatInstruction* batInstruction = batPanel.getBatInstruction();
    
    if ((batInstruction->getType() == BatInstructionType_Tap
        || batInstruction->getType() == BatInstructionType_TapHold)
        && !m_world_1_special.gpuTextureWrapper)
    {
        return;
    }
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*bat, Assets::getInstance()->get(bat));
    m_spriteBatcher->endBatch(*m_vampire.gpuTextureWrapper);
    
    if (batInstruction->isOpening() || batInstruction->isOpen())
    {
        m_spriteBatcher->beginBatch();
        
        renderPhysicalEntityWithColor(*batInstruction, Assets::getInstance()->get(batInstruction), batInstruction->getColor());
        
        if (batInstruction->isOpening()
            || batInstruction->getType() == BatInstructionType_SwipeDown)
        {
            m_spriteBatcher->endBatch(*m_jon.gpuTextureWrapper);
        }
        else if (batInstruction->getType() == BatInstructionType_SwipeRight)
        {
            m_spriteBatcher->endBatch(*m_vampire.gpuTextureWrapper);
        }
        else if (batInstruction->getType() == BatInstructionType_Tap
                 || batInstruction->getType() == BatInstructionType_TapHold)
        {
            m_spriteBatcher->endBatch(*m_world_1_special.gpuTextureWrapper);
        }
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
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getMidBossForegroundObjects());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getEndBossForegroundObjects());
	renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getCountHissWithMinas());
    renderBoundsForPhysicalEntities(*m_boundsRectangleBatcher, game.getEndBossSnakes());
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

void Renderer::renderBlackOverlay(float opacity)
{
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static Rectangle rect = Rectangle(0, 0, CAM_WIDTH, CAM_HEIGHT);
    
    static Color blackColor = Color(0, 0, 0, 0);
    blackColor.alpha = opacity;
    
    m_highlightRectangleBatcher->beginBatch();
    m_highlightRectangleBatcher->renderRectangle(rect, blackColor);
    m_highlightRectangleBatcher->endBatch();
}

void Renderer::renderHud(Game& game, GameButton* backButton, GameButton* continueButton, int score)
{
	updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static Color fontColor = Color(1, 1, 1, 1);
    static float fgWidth = CAM_WIDTH / 32;
    static float fgHeight = fgWidth * 1.171875f;
    
    float textY = CAM_HEIGHT - fgHeight;
    
    if (m_world_1_objects.gpuTextureWrapper)
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
        renderPhysicalEntity(uiGoldenCarrot, Assets::getInstance()->get(&uiGoldenCarrot), true);
        renderPhysicalEntity(uiCarrot, Assets::getInstance()->get(&uiCarrot), true);
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    }
    
    m_spriteBatcher->beginBatch();
    
    static TextureRegion xTr = TextureRegion(256, 0, 32, 32, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024);
    
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
        static TextureRegion clockTr = TextureRegion(512, 0, 72, 72, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024);
        
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
        renderPhysicalEntity(*backButton, Assets::getInstance()->get(backButton), true);
    }
    
    if (continueButton)
    {
        renderPhysicalEntityWithColor(*continueButton, Assets::getInstance()->get(continueButton), continueButton->getColor(), true);
    }

    m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
}

void Renderer::renderResumeButtonOverlay()
{
    if (!m_vampire.gpuTextureWrapper)
    {
        return;
    }
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static TextureRegion resumeButtonTr = TextureRegion(2048, 1792, 192, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, CAM_HEIGHT / 2, 2, 2, 0, resumeButtonTr);
    m_spriteBatcher->endBatch(*m_vampire.gpuTextureWrapper);
}

void Renderer::renderDebugInfo(Game& game, int fps)
{
	updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

	static Color fontColor = Color(1, 1, 1, 1);
	static float fgWidth = CAM_WIDTH / 24 / 2;
	static float fgHeight = fgWidth * 1.171875f;

	{
		// Render FPS

		updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

		std::stringstream ss;
		ss << fps << " FPS";
		std::string fps_string = ss.str();

		m_spriteBatcher->beginBatch();
		m_font->renderText(*m_spriteBatcher, fps_string, CAM_WIDTH / 5, CAM_HEIGHT - fgHeight * 4, fgWidth, fgHeight, fontColor, true);
		m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
	}

	if (game.getJons().size() > 0)
	{
		// Render Jon's Speed

		updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

		Jon& jon = game.getJon();

		float speed = jon.getVelocity().getX();

		std::stringstream ss;
		ss << std::fixed << std::setprecision(3) << speed << " m/s";
		std::string text = ss.str();

		m_spriteBatcher->beginBatch();
		m_font->renderText(*m_spriteBatcher, text, CAM_WIDTH / 5, CAM_HEIGHT - fgHeight * 5, fgWidth, fgHeight, fontColor, true);
		m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
	}
}

void Renderer::renderComingSoonScreenBackground()
{
    if (!ensureWorld1Textures())
    {
        return;
    }
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static TextureRegion tr = Assets::getInstance()->createTextureRegion(0, 0, 1280, 720, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH, CAM_HEIGHT, 0, tr);
    m_spriteBatcher->endBatch(*m_world_1_background_mid.gpuTextureWrapper);
}

void Renderer::renderComingSoonScreenUi(GameButton* nextArrowButton)
{
    m_spriteBatcher->beginBatch();
    renderPhysicalEntityWithColor(*nextArrowButton, Assets::getInstance()->get(nextArrowButton), nextArrowButton->getColor(), true);
    m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
}

void Renderer::renderMarkers(Game& game)
{
    static Color originMarkerColor = Color(0, 1, 0, 0.5f);
    static Color endMarkerColor = Color(1, 0, 0, 0.5f);
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_highlightRectangleBatcher->beginBatch();
    
    for (std::vector<GameMarker *>::iterator i = game.getMarkers().begin(); i != game.getMarkers().end(); i++)
    {
        Rectangle& marker = (*i)->getMainBounds();
        m_highlightRectangleBatcher->renderRectangle(marker, (*i)->getType() == 0 ? originMarkerColor : endMarkerColor);
    }
    
    m_highlightRectangleBatcher->endBatch();
}

void Renderer::renderLevelEditor(GameScreenLevelEditor* gameScreenLevelEditor)
{
	LevelEditorActionsPanel* leap = gameScreenLevelEditor->getLevelEditorActionsPanel();
	LevelEditorEntitiesPanel* leep = gameScreenLevelEditor->getLevelEditorEntitiesPanel();
	TrashCan* tc = gameScreenLevelEditor->getTrashCan();
	LevelSelectorPanel* lsp = gameScreenLevelEditor->getLevelSelectorPanel();
	OffsetPanel* osp = gameScreenLevelEditor->getOffsetPanel();
	ConfirmResetPanel* crp = gameScreenLevelEditor->getConfirmResetPanel();
	ConfirmExitPanel* cep = gameScreenLevelEditor->getConfirmExitPanel();

    if (!ensureLevelEditorTextures())
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
    renderPhysicalEntity(*leap, Assets::getInstance()->get(leap), true);
    renderPhysicalEntity(*leep, Assets::getInstance()->get(leep), true);
    m_spriteBatcher->endBatch(*m_level_editor.gpuTextureWrapper);
    
    updateMatrix(m_camBounds->getLowerLeft().getX(), m_camBounds->getLowerLeft().getX() + m_camBounds->getWidth(), m_camBounds->getLowerLeft().getY(), m_camBounds->getLowerLeft().getY() + m_camBounds->getHeight());
    
    m_spriteBatcher->beginBatch();
    renderPhysicalEntity(*tc, Assets::getInstance()->get(tc), true);
    m_spriteBatcher->endBatch(*m_level_editor.gpuTextureWrapper);
    
    if (leep->isOpen() && m_world_1_objects.gpuTextureWrapper)
    {
        updateMatrix(0, CAM_WIDTH, leep->getEntitiesCameraPos(), leep->getEntitiesCameraPos() + CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getMidgrounds(), true);
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getGrounds(), true);
        renderPhysicalEntities(leep->getExitGrounds(), true);
        m_spriteBatcher->endBatch(*m_world_1_ground.gpuTextureWrapper);
        
        if (m_world_1_special.gpuTextureWrapper)
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntities(leep->getPits(), true);
            m_spriteBatcher->endBatch(*m_world_1_special.gpuTextureWrapper);
        }
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getHoles(), true);
        renderPhysicalEntities(leep->getCollectibleItems(), true);
        renderPhysicalEntities(leep->getForegroundObjects(), true);
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
        
        if (m_world_1_mid_boss_part_3.gpuTextureWrapper)
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntities(leep->getMidBossForegroundObjects(), true);
            m_spriteBatcher->endBatch(*m_world_1_mid_boss_part_3.gpuTextureWrapper);
        }
        
        if (m_world_1_end_boss_part_1.gpuTextureWrapper)
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntities(leep->getEndBossForegroundObjects(), true);
            renderPhysicalEntities(leep->getEndBossSnakes(), true);
            m_spriteBatcher->endBatch(*m_world_1_end_boss_part_1.gpuTextureWrapper);
        }

		if (m_vampire.gpuTextureWrapper)
		{
			m_spriteBatcher->beginBatch();
			renderPhysicalEntities(leep->getCountHissWithMinas(), true);
			m_spriteBatcher->endBatch(*m_jon.gpuTextureWrapper);
		}
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getEnemies(), true);
        m_spriteBatcher->endBatch(*m_world_1_enemies.gpuTextureWrapper);
        
        if (m_vampire.gpuTextureWrapper)
        {
            m_spriteBatcher->beginBatch();
            renderPhysicalEntities(leep->getJons(), true);
            m_spriteBatcher->endBatch(*m_jon.gpuTextureWrapper);
        }
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntities(leep->getExtraForegroundObjects(), true);
        m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
    }
    
    if (lsp->isOpen())
    {
        updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*lsp, Assets::getInstance()->get(lsp), true);
        m_spriteBatcher->endBatch(*m_level_editor.gpuTextureWrapper);
        
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
        m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
    }
    
    if (osp->isOpen())
    {
        updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*osp, Assets::getInstance()->get(osp), true);
        m_spriteBatcher->endBatch(*m_level_editor.gpuTextureWrapper);
        
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
        m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
    }
    
    if (crp->isOpen())
    {
        updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*crp, Assets::getInstance()->get(crp), true);
        m_spriteBatcher->endBatch(*m_level_editor.gpuTextureWrapper);
    }
    
    if (cep->isOpen())
    {
        updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
        
        m_spriteBatcher->beginBatch();
        renderPhysicalEntity(*cep, Assets::getInstance()->get(cep), true);
        m_spriteBatcher->endBatch(*m_level_editor.gpuTextureWrapper);
    }

	{
		static Color fontColor = Color(1, 1, 1, 1);
		static float fgWidth = CAM_WIDTH / 24;
		static float fgHeight = fgWidth * 1.171875f;

		updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);

		std::stringstream ss;
		ss << "World " << lsp->getWorld() << " / Level " << lsp->getLevel();
		std::string text_string = ss.str();

		// Render World/Level Info

		m_spriteBatcher->beginBatch();
		m_font->renderText(*m_spriteBatcher, text_string, CAM_WIDTH - 4, CAM_HEIGHT - fgHeight / 2, fgWidth / 2, fgHeight / 2, fontColor, true);
		m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
	}

	static TextureRegion xTr = TextureRegion(256, 0, 32, 32, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024);

	{
		static Color fontColor = Color(1, 1, 1, 1);
		static float fgWidth = CAM_WIDTH / 32;
		static float fgHeight = fgWidth * 1.171875f;

		float textY = CAM_HEIGHT - fgHeight;

		if (m_world_1_objects.gpuTextureWrapper)
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
			renderPhysicalEntity(uiGoldenCarrot, Assets::getInstance()->get(&uiGoldenCarrot), true);
			renderPhysicalEntity(uiCarrot, Assets::getInstance()->get(&uiCarrot), true);
			m_spriteBatcher->endBatch(*m_world_1_objects.gpuTextureWrapper);
		}

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

		m_spriteBatcher->endBatch(*m_misc.gpuTextureWrapper);
	}
}

void Renderer::renderLoading()
{
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    static Animation anim = Animation(0, 900, 144, 96, 1296, 96, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024, true, 0.06f, 7);
    
    static float width = 1.265625f;
    static float height = 0.84375f;
    static float x = CAM_WIDTH - width / 2;
    static float y = CAM_HEIGHT - height / 2;
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(x, y, width, height, 0, anim.getTextureRegion(m_fStateTime));
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
    
    Jon& jon = game.getJon();
    bool isVampire = jon.isVampire();
    m_spriteBatcher->endBatch(m_framebuffers.at(0), (isVampire || jon.isRevertingToRabbit()) ? *m_framebufferTintGpuProgramWrapper : *m_framebufferToScreenGpuProgramWrapper);
}

void Renderer::renderToThirdFramebufferWithObfuscation()
{
    setFramebuffer(2);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(1), *m_framebufferObfuscationGpuProgramWrapper);
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

void Renderer::renderToScreenFadeTransition(float progress)
{
    /// Render the screen transition to the screen
    
    m_fadeScreenGpuProgramWrapper->configure(&m_framebuffers.at(1), progress);
    
    bindToScreenFramebuffer();
    
    clearFramebufferWithColor(0, 0, 0, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(0), *m_fadeScreenGpuProgramWrapper);
}

void Renderer::renderToScreenPointTransition(float centerX, float centerY, float progress)
{
    /// Render the screen transition to the screen
    
    m_pointTransScreenGpuProgramWrapper->configure(&m_framebuffers.at(1), centerX, centerY, progress);
    
    updateMatrix(0, CAM_WIDTH, 0, CAM_HEIGHT);
    
    bindToScreenFramebuffer();
    
    clearFramebufferWithColor(0, 0, 0, 1);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH, CAM_HEIGHT, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    m_spriteBatcher->endBatch(m_framebuffers.at(0), *m_pointTransScreenGpuProgramWrapper);
}

void Renderer::renderToScreenWithRadialBlur()
{
    /// Render everything to the screen with a radial blur
    
    m_framebufferRadialBlurGpuProgramWrapper->configure(m_fRadialBlurDirection);
    
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
    
    m_pendingLoadFunctions.clear();
    
    if (m_areShadersLoaded)
    {
        m_transScreenGpuProgramWrapper->cleanUp();
        m_fadeScreenGpuProgramWrapper->cleanUp();
        m_pointTransScreenGpuProgramWrapper->cleanUp();
        m_sinWaveTextureProgram->cleanUp();
        m_backgroundGpuTextureProgramWrapper->cleanUp();
        m_snakeDeathTextureProgram->cleanUp();
        m_endBossSnakeTextureProgram->cleanUp();
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

#pragma mark protected

void Renderer::renderPhysicalEntity(PhysicalEntity &pe, TextureRegion& tr, bool ignoreCamBounds)
{
    Rectangle bounds = Rectangle(pe.getPosition().getX() - pe.getWidth() / 2, pe.getPosition().getY() - pe.getHeight() / 2, pe.getWidth(), pe.getHeight());
    
    if (ignoreCamBounds || OverlapTester::doRectanglesOverlap(*m_camBounds, bounds))
    {
        m_spriteBatcher->drawSprite(pe.getPosition().getX(), pe.getPosition().getY(), pe.getWidth(), pe.getHeight(), pe.getAngle(), tr);
    }
}

void Renderer::renderPhysicalEntityWithColor(PhysicalEntity &pe, TextureRegion& tr, Color c, bool ignoreCamBounds)
{
    Rectangle bounds = Rectangle(pe.getPosition().getX() - pe.getWidth() / 2, pe.getPosition().getY() - pe.getHeight() / 2, pe.getWidth(), pe.getHeight());
    
    if (ignoreCamBounds || OverlapTester::doRectanglesOverlap(*m_camBounds, bounds))
    {
        m_spriteBatcher->drawSprite(pe.getPosition().getX(), pe.getPosition().getY(), pe.getWidth(), pe.getHeight(), pe.getAngle(), c, tr);
    }
}

#pragma mark private

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
	float farCamPos = farRight;
    
    return farCamPos;
}

void Renderer::loadMiscTextures()
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
            r->m_title_screen.gpuTextureDataWrapper = r->loadTextureData("title_screen");
        }, this));
    }
}

void Renderer::loadTitleTextures()
{
    m_pendingLoadFunctions.push_back(&Renderer::loadTitle);
}

bool Renderer::ensureTitleTextures()
{
    if (m_title_screen.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadTitle);
        }
        
        return false;
    }
    
    return true;
}

void Renderer::loadWorldMapPart1()
{
    if (m_world_map_screen_part_1.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_map_screen_part_1.gpuTextureDataWrapper = r->loadTextureData("world_map_screen_part_1");
        }, this));
    }
}

void Renderer::loadWorldMapPart2()
{
    if (m_world_map_screen_part_2.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_map_screen_part_2.gpuTextureDataWrapper = r->loadTextureData("world_map_screen_part_2");
        }, this));
    }
}

void Renderer::loadWorldMapTextures()
{
    m_pendingLoadFunctions.push_back(&Renderer::loadWorldMapPart1);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorldMapPart2);
}

bool Renderer::ensureWorldMapTextures()
{
    if (m_world_map_screen_part_1.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorldMapPart1);
        }
        
        return false;
    }
    
    if (m_world_map_screen_part_2.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorldMapPart2);
        }
        
        return false;
    }
    
    return true;
}

void Renderer::loadLevelEditor()
{
    if (m_level_editor.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_level_editor.gpuTextureDataWrapper = r->loadTextureData("level_editor");
        }, this));
    }
}

void Renderer::loadLevelEditorTextures()
{
    m_pendingLoadFunctions.push_back(&Renderer::loadLevelEditor);
}

bool Renderer::ensureLevelEditorTextures()
{
    if (m_level_editor.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadLevelEditor);
        }
        
        return false;
    }
    
    return true;
}

void Renderer::loadWorld1Cutscene1()
{
    if (m_world_1_cutscene_1.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_cutscene_1.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_cutscene_1" : "world_1_cutscene_1");
        }, this));
    }
}

void Renderer::loadWorld1Cutscene2()
{
    if (m_world_1_cutscene_2.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_cutscene_2.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_cutscene_2" : "world_1_cutscene_2");
        }, this));
    }
}

void Renderer::loadWorld1CutsceneTextures()
{
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Cutscene1);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Cutscene2);
}

bool Renderer::ensureWorld1CutsceneTextures()
{
    if (m_world_1_cutscene_1.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Cutscene1);
        }
        
        return false;
    }
    
    if (m_world_1_cutscene_2.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Cutscene2);
        }
        
        return false;
    }
    
    return true;
}

void Renderer::loadJon()
{
    if (m_jon.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_jon.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_jon" : "jon");
        }, this));
    }
}

void Renderer::loadTransDeath()
{
    if (m_trans_death_shader_helper.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_trans_death_shader_helper.gpuTextureDataWrapper = r->loadTextureData("trans_death_shader_helper");
        }, this));
    }
}

void Renderer::loadVampire()
{
    if (m_vampire.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_vampire.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_vampire" : "vampire");
        }, this));
    }
}

void Renderer::loadJonTextures()
{
    m_pendingLoadFunctions.push_back(&Renderer::loadJon);
    m_pendingLoadFunctions.push_back(&Renderer::loadTransDeath);
    m_pendingLoadFunctions.push_back(&Renderer::loadVampire);
}

bool Renderer::ensureJonTextures()
{
    if (m_jon.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadJon);
        }
        
        return false;
    }
    
    if (m_trans_death_shader_helper.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadTransDeath);
        }
        
        return false;
    }
    
    if (m_vampire.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadVampire);
        }
        
        return false;
    }
    
    return true;
}

void Renderer::loadWorld1BackgroundLower()
{
    if (m_world_1_background_lower.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_background_lower.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_background_lower" : "world_1_background_lower");
        }, this));
    }
}

void Renderer::loadWorld1BackgroundMid()
{
    if (m_world_1_background_mid.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_background_mid.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_background_mid" : "world_1_background_mid");
        }, this));
    }
}

void Renderer::loadWorld1BackgroundUpper()
{
    if (m_world_1_background_upper.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_background_upper.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_background_upper" : "world_1_background_upper");
        }, this));
    }
}

void Renderer::loadWorld1Enemies()
{
    if (m_world_1_enemies.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_enemies.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_enemies" : "world_1_enemies");
        }, this));
    }
}

void Renderer::loadWorld1Ground()
{
    if (m_world_1_ground.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_ground.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_ground" : "world_1_ground");
        }, this));
    }
}

void Renderer::loadWorld1Objects()
{
    if (m_world_1_objects.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
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

void Renderer::loadWorld1Textures()
{
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundLower);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundMid);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundUpper);
    
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Enemies);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Ground);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Objects);
    
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Special);
    
    loadJonTextures();
}

bool Renderer::ensureWorld1Textures()
{
    if (m_world_1_background_lower.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundLower);
        }
        
        return false;
    }
    
    if (m_world_1_background_mid.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundMid);
        }
        
        return false;
    }
    
    if (m_world_1_background_upper.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundUpper);
        }
        
        return false;
    }
    
    if (m_world_1_enemies.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Enemies);
        }
        
        return false;
    }
    
    if (m_world_1_ground.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Ground);
        }
        
        return false;
    }
    
    if (m_world_1_objects.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Objects);
        }
        
        return false;
    }
    
    if (m_world_1_special.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Special);
        }
        
        return false;
    }
    
    return true;
}

void Renderer::loadWorld1MidBossPart1()
{
    if (m_world_1_mid_boss_part_1.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_mid_boss_part_1.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_mid_boss_part_1" : "world_1_mid_boss_part_1");
        }, this));
    }
}

void Renderer::loadWorld1MidBossPart2()
{
    if (m_world_1_mid_boss_part_2.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_mid_boss_part_2.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_mid_boss_part_2" : "world_1_mid_boss_part_2");
        }, this));
    }
}

void Renderer::loadWorld1MidBossPart3()
{
    if (m_world_1_mid_boss_part_3.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_mid_boss_part_3.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_mid_boss_part_3" : "world_1_mid_boss_part_3");
        }, this));
    }
}

void Renderer::loadWorld1MidBossTextures()
{
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1MidBossPart1);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1MidBossPart2);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1MidBossPart3);
    
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundLower);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundMid);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundUpper);
    
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Ground);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Objects);
    
    loadJonTextures();
}

bool Renderer::ensureWorld1MidBossTextures()
{
    if (m_world_1_mid_boss_part_1.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1MidBossPart1);
        }
        
        return false;
    }
    
    if (m_world_1_mid_boss_part_2.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1MidBossPart2);
        }
        
        return false;
    }
    
    if (m_world_1_mid_boss_part_3.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1MidBossPart3);
        }
        
        return false;
    }
    
    return true;
}

void Renderer::loadWorld1EndBossPart1()
{
    if (m_world_1_end_boss_part_1.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_end_boss_part_1.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_end_boss_part_1" : "world_1_end_boss_part_1");
        }, this));
    }
}

void Renderer::loadWorld1EndBossPart2()
{
    if (m_world_1_end_boss_part_2.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_end_boss_part_2.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_end_boss_part_2" : "world_1_end_boss_part_2");
        }, this));
    }
}

void Renderer::loadWorld1EndBossPart3()
{
    if (m_world_1_end_boss_part_3.gpuTextureWrapper == nullptr)
    {
        m_iNumAsyncLoads++;
        
        m_threads.push_back(std::thread([](Renderer* r)
        {
            r->m_world_1_end_boss_part_3.gpuTextureDataWrapper = r->loadTextureData(r->m_compressed ? "c_world_1_end_boss_part_3" : "world_1_end_boss_part_3");
        }, this));
    }
}

void Renderer::loadWorld1EndBossTextures()
{
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1EndBossPart1);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1EndBossPart2);
    m_pendingLoadFunctions.push_back(&Renderer::loadWorld1EndBossPart3);
    
	m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundLower);
	m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundMid);
	m_pendingLoadFunctions.push_back(&Renderer::loadWorld1BackgroundUpper);

	m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Ground);
	m_pendingLoadFunctions.push_back(&Renderer::loadWorld1Objects);

	loadJonTextures();
}

bool Renderer::ensureWorld1EndBossTextures()
{
    if (m_world_1_end_boss_part_1.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1EndBossPart1);
        }
        
        return false;
    }
    
    if (m_world_1_end_boss_part_2.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1EndBossPart2);
        }
        
        return false;
    }
    
    if (m_world_1_end_boss_part_3.gpuTextureWrapper == nullptr)
    {
        if (m_iNumAsyncLoads == 0)
        {
            m_pendingLoadFunctions.push_back(&Renderer::loadWorld1EndBossPart3);
        }
        
        return false;
    }
    
    return true;
}

void Renderer::loadWorld2Textures()
{
    // TODO
}

void Renderer::loadWorld2MidBossTextures()
{
    // TODO
}

void Renderer::loadWorld2EndBossTextures()
{
    // TODO
}

void Renderer::loadWorld3Textures()
{
    // TODO
}

void Renderer::loadWorld3MidBossTextures()
{
    // TODO
}

void Renderer::loadWorld3EndBossTextures()
{
    // TODO
}

void Renderer::loadWorld4Textures()
{
    // TODO
}

void Renderer::loadWorld4MidBossTextures()
{
    // TODO
}

void Renderer::loadWorld4EndBossTextures()
{
    // TODO
}

void Renderer::loadWorld5Textures()
{
    // TODO
}

void Renderer::loadWorld5MidBossTextures()
{
    // TODO
}

void Renderer::loadWorld5EndBossTextures()
{
    // TODO
}

void Renderer::unloadTitleTextures()
{
    destroyTexture(&m_title_screen);
}

void Renderer::unloadWorldMapTextures()
{
    destroyTexture(&m_world_map_screen_part_1);
    destroyTexture(&m_world_map_screen_part_2);
}

void Renderer::unloadLevelEditorTextures()
{
    destroyTexture(&m_level_editor);
}

void Renderer::unloadJonTextures()
{
    destroyTexture(&m_jon);
    destroyTexture(&m_trans_death_shader_helper);
    destroyTexture(&m_vampire);
}

void Renderer::unloadWorld1CutsceneTextures()
{
    destroyTexture(&m_world_1_cutscene_1);
    destroyTexture(&m_world_1_cutscene_2);
}

void Renderer::unloadWorld1Textures()
{
    destroyTexture(&m_world_1_background_lower);
    destroyTexture(&m_world_1_background_mid);
    destroyTexture(&m_world_1_background_upper);
    
    destroyTexture(&m_world_1_enemies);
    destroyTexture(&m_world_1_ground);
    destroyTexture(&m_world_1_objects);
    
    destroyTexture(&m_world_1_special);
    
    unloadJonTextures();
}

void Renderer::unloadWorld1MidBossTextures()
{
    destroyTexture(&m_world_1_mid_boss_part_1);
    destroyTexture(&m_world_1_mid_boss_part_2);
    destroyTexture(&m_world_1_mid_boss_part_3);
    
    unloadWorld1Textures();
}

void Renderer::unloadWorld1EndBossTextures()
{
    destroyTexture(&m_world_1_end_boss_part_1);
    destroyTexture(&m_world_1_end_boss_part_2);
    destroyTexture(&m_world_1_end_boss_part_3);
    
	unloadWorld1Textures();
}

void Renderer::unloadWorld2Textures()
{
    // TODO
}

void Renderer::unloadWorld2MidBossTextures()
{
    // TODO
}

void Renderer::unloadWorld2EndBossTextures()
{
    // TODO
}

void Renderer::unloadWorld3Textures()
{
    // TODO
}

void Renderer::unloadWorld3MidBossTextures()
{
    // TODO
}

void Renderer::unloadWorld3EndBossTextures()
{
    // TODO
}

void Renderer::unloadWorld4Textures()
{
    // TODO
}

void Renderer::unloadWorld4MidBossTextures()
{
    // TODO
}

void Renderer::unloadWorld4EndBossTextures()
{
    // TODO
}

void Renderer::unloadWorld5Textures()
{
    // TODO
}

void Renderer::unloadWorld5MidBossTextures()
{
    // TODO
}

void Renderer::unloadWorld5EndBossTextures()
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
