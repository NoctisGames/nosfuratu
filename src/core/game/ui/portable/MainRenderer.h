//
//  MainRenderer.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainRenderer__
#define __nosfuratu__MainRenderer__

#include "Renderer.h"

#include "Assets.h"
#include "RendererType.h"
#include "NGRectBatcher.h"
#include "TextureWrapper.h"

#include "Color.h"

#include <memory>
#include <vector>
#include <thread>

class SpriteBatcher;
class LineBatcher;
class Font;
struct GpuTextureWrapper;
struct GpuTextureDataWrapper;
class PhysicalEntity;
class TextureRegion;
class Game;
class Jon;
class GpuProgramWrapper;
class TransitionGpuProgramWrapper;
class PointTransitionGpuProgramWrapper;
class SinWaveTextureGpuProgramWrapper;
class SnakeDeathTextureGpuProgramWrapper;
class EndBossSnakeTextureGpuProgramWrapper;
class ShockwaveTextureGpuProgramWrapper;
class TransDeathGpuProgramWrapper;
class FramebufferRadialBlurGpuProgramWrapper;
class Vector2D;
class GoldenCarrotsMarker;
class ScoreMarker;
class GameButton;
class MainScreenLevelEditor;
class TitlePanel;
class CutscenePanel;
class WorldMap;
class WorldMapPanel;
class AbilitySlot;
class LevelThumbnail;
class BatPanel;
class MidBossOwl;
class EndBossSnake;

class MainRenderer : public Renderer
{
public:
    MainRenderer();
    
    virtual ~MainRenderer();
    
    void init(RendererType rendererType);
    
    void load(RendererType rendererType);
    
    void unload(RendererType rendererType);

    bool isLoadingData();
    
    virtual bool isLoaded();
    
    virtual void beginFrame(float deltaTime);
    
    void beginOpeningPanningSequence(Game& game);
    
    // Return 3 if the beginning of the stage has been reached
    // Return 2 if Jon should begin warming up
    // Return 1 to activate the radial blur effect
    // Return 0 to do nothing
    int updateCameraToFollowPathToJon(Game& game);
    
    void updateCameraToFollowJon(Game& game, BatPanel* batPanel, float deltaTime, float paddingX = 0, bool chase = false, bool ignoreY = false, bool instant = false);
    
    void moveCamera(float x);
    
    void stopCamera();
    
    void zoomOut();
    
    void zoomIn();
    
    void renderTitleScreenBackground(TitlePanel* panel);
    
    void renderTitleScreenUi(GameButton* levelEditorButton);
    
    void renderCutscene(std::vector<CutscenePanel*> cutscenePanels);
    
    void renderWorldMapScreenBackground(WorldMapPanel* panel);
    
    void renderWorldMapScreenUi(WorldMap& wm);
    
    void renderWorldMapScreenButtons(WorldMap& wm);
    
    void renderWorld(Game& game);
    
    void renderJonAndExtraForegroundObjects(Game& game);
    
    void renderMidBossOwl(MidBossOwl& midBossOwl);
    
    void renderEndBossSnake(EndBossSnake& endBossSnake);
    
    void renderBatPanel(BatPanel& batPanel);
    
    void renderBounds(Game& game, int boundsLevelRequested);
    
    void renderEntityHighlighted(PhysicalEntity& entity, Color& c);
    
    void renderBlackOverlay(float opacity);
    
    void renderHud(Game& game, GameButton* backButton, GameButton* continueButton, int score);
    
    void renderResumeButtonOverlay();

	void renderDebugInfo(Game& game, int fps);
    
    void renderComingSoonScreenBackground();
    
    void renderMarkers(Game& game);
    
    void renderLevelEditor(MainScreenLevelEditor* gameScreenLevelEditor);
    
    void renderLoading();
    
    void renderToSecondFramebufferWithShockwave(float centerX, float centerY, float timeElapsed, bool isTransforming);
    
    void renderToSecondFramebuffer(Game& game);
    
    void renderToThirdFramebufferWithObfuscation();
    
    void renderToThirdFramebufferWithTransDeathIn(float timeElapsed);
    
    void renderToThirdFramebufferWithTransDeathOut(float timeElapsed);
    
    void renderToThirdFramebufferTransition(float progress);
    
    void renderToThirdFramebufferFadeTransition(float progress);
    
    void renderToThirdFramebufferPointTransition(float centerX, float centerY, float progress);
    
    void renderToThirdFramebufferWithRadialBlur();
    
    void renderToScreen();
    
    NGRect& getCameraBounds();
    
    Vector2D& getCameraPosition();
    
private:
    Font* m_font;
    
    TextureWrapper m_jon;
    TextureWrapper m_level_editor;
    TextureWrapper m_misc;
    TextureWrapper m_title_screen;
    TextureWrapper m_trans_death_shader_helper;
    TextureWrapper m_tutorial;
    TextureWrapper m_vampire;
    TextureWrapper m_world_1_background_lower_part_1;
    TextureWrapper m_world_1_background_lower_part_2;
    TextureWrapper m_world_1_background_mid;
    TextureWrapper m_world_1_background_upper;
    TextureWrapper m_world_1_cutscene_1;
    TextureWrapper m_world_1_cutscene_2;
    TextureWrapper m_world_1_end_boss_part_1;
    TextureWrapper m_world_1_end_boss_part_2;
    TextureWrapper m_world_1_end_boss_part_3;
    TextureWrapper m_world_1_enemies;
    TextureWrapper m_world_1_ground;
    TextureWrapper m_world_1_mid_boss_part_1;
    TextureWrapper m_world_1_mid_boss_part_2;
    TextureWrapper m_world_1_mid_boss_part_3;
    TextureWrapper m_world_1_objects_part_1;
    TextureWrapper m_world_1_objects_part_2;
    TextureWrapper m_world_1_special;
    TextureWrapper m_world_map_screen_part_1;
    TextureWrapper m_world_map_screen_part_2;
    
    TransitionGpuProgramWrapper* m_transScreenGpuProgramWrapper;
    TransitionGpuProgramWrapper* m_fadeScreenGpuProgramWrapper;
    PointTransitionGpuProgramWrapper* m_pointTransScreenGpuProgramWrapper;
    SinWaveTextureGpuProgramWrapper* m_sinWaveTextureProgram;
    GpuProgramWrapper* m_backgroundGpuTextureProgramWrapper;
    SnakeDeathTextureGpuProgramWrapper* m_snakeDeathTextureProgram;
    EndBossSnakeTextureGpuProgramWrapper* m_endBossSnakeTextureProgram;
    ShockwaveTextureGpuProgramWrapper* m_shockwaveTextureGpuProgramWrapper;
    TransDeathGpuProgramWrapper* m_transDeathInGpuProgramWrapper;
    TransDeathGpuProgramWrapper* m_transDeathOutGpuProgramWrapper;
    GpuProgramWrapper* m_framebufferToScreenGpuProgramWrapper;
    GpuProgramWrapper* m_framebufferTintGpuProgramWrapper;
    GpuProgramWrapper* m_framebufferObfuscationGpuProgramWrapper;
    FramebufferRadialBlurGpuProgramWrapper* m_framebufferRadialBlurGpuProgramWrapper;
    
    NGRect m_camBounds;
    Vector2D m_camPosAcceleration;
    Vector2D m_camPosVelocity;
    
    RendererType m_loadedRendererType;
    
    float m_fStateTime;
	float m_fCamPosX;
    float m_fGroundedCamY;
    float m_fLowestGroundedCamY;
    float m_fLastKnownCamY;
    float m_fRadialBlurDirection;

    bool m_stopCamera;
    bool m_hasCompletedRadialBlur;
    
    template<typename T>
    void renderPhysicalEntities(std::vector<T*>& items)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            T* pItem = *i;
            T& item = *pItem;
            renderPhysicalEntity(item, MAIN_ASSETS->get(pItem));
        }
    }
    
    template<typename T>
    void renderPhysicalEntitiesWithColor(std::vector<T*>& items)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            T* pItem = *i;
            T& item = *pItem;
            renderPhysicalEntityWithColor(item, MAIN_ASSETS->get(pItem), item.getColor());
        }
    }
    
    template<typename T>
    void renderBoundsForPhysicalEntities(NGRectBatcher& rectangleBatcher, std::vector<T*>& items)
    {
        static Color red = Color(1, 0, 0, 1);
        
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            for (typename std::vector<std::unique_ptr<NGRect>>::iterator j = (*i)->getBounds().begin(); j != (*i)->getBounds().end(); j++)
            {
                rectangleBatcher.beginBatch();
                renderBoundsWithColor(*(*j), red);
                rectangleBatcher.endBatch(*m_colorGpuProgramWrapper);
            }
        }
    }
    
    void renderBoundsForPhysicalEntity(PhysicalEntity &go);
    
    void renderBoundsWithColor(NGRect &r, Color& c);
    
    void renderHighlightForPhysicalEntity(PhysicalEntity &go, Color& c);
    
    void loadMiscTextures();
    
    void loadTitle();
    void loadTitleTextures();
    bool ensureTitleTextures();
    
    void loadWorldMapPart1();
    void loadWorldMapPart2();
    void loadWorldMapTextures();
    bool ensureWorldMapTextures();
    
    void loadLevelEditor();
    void loadLevelEditorTextures();
    bool ensureLevelEditorTextures();
    
    void loadWorld1Cutscene1();
    void loadWorld1Cutscene2();
    void loadWorld1CutsceneTextures();
    bool ensureWorld1CutsceneTextures();
    
    void loadJon();
    void loadTransDeath();
    void loadVampire();
    void loadJonTextures();
    bool ensureJonTextures();
    
    void loadTutorial();
    void loadTutorialTextures();
    bool ensureTutorialTextures();
    
    void loadWorld1BackgroundLowerPart1();
    void loadWorld1BackgroundLowerPart2();
    void loadWorld1BackgroundMid();
    void loadWorld1BackgroundUpper();
    void loadWorld1Enemies();
    void loadWorld1Ground();
    void loadWorld1ObjectsPart1();
    void loadWorld1ObjectsPart2();
    void loadWorld1Special();
    void loadWorld1Textures();
    bool ensureWorld1Textures();
    bool ensureWorld1Objects();
    bool ensureWorld1Special();
    
    void loadWorld1MidBossPart1();
    void loadWorld1MidBossPart2();
    void loadWorld1MidBossPart3();
    void loadWorld1MidBossTextures();
    bool ensureWorld1MidBossTextures();
    bool ensureWorld1MidBossPart3();
    
    void loadWorld1EndBossPart1();
    void loadWorld1EndBossPart2();
    void loadWorld1EndBossPart3();
    void loadWorld1EndBossTextures();
    bool ensureWorld1EndBossTextures();
    bool ensureWorld1EndBossPart1();
    
    void unloadTitleTextures();
    void unloadWorldMapTextures();
    void unloadLevelEditorTextures();
    
    void unloadJonTextures();
    
    void unloadTutorialTextures();
    
    void unloadWorld1CutsceneTextures();
    
    void unloadWorld1Textures();
    void unloadWorld1MidBossTextures();
    void unloadWorld1EndBossTextures();
};

#endif /* defined(__nosfuratu__MainRenderer__) */
