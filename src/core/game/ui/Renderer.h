//
//  Renderer.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Renderer__
#define __nosfuratu__Renderer__

#include "Assets.h"
#include "RendererType.h"
#include "RectangleBatcher.h"
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
class GameScreenLevelEditor;
class TitlePanel;
class CutscenePanel;
class WorldMap;
class WorldMapPanel;
class AbilitySlot;
class LevelThumbnail;
class BatPanel;
class MidBossOwl;
class EndBossSnake;
class GameScreenSpriteTester;

class Renderer
{
public:
    Renderer();
    
    ~Renderer();
    
    void init(RendererType rendererType);
    
    void load(RendererType rendererType);
    
    void unload(RendererType rendererType);

    bool isLoadingAdditionalTextures();
    
    virtual bool isLoaded();
    
    virtual void beginFrame(float deltaTime);
    
    virtual void endFrame() = 0;
    
    void setFramebuffer(int framebufferIndex);
    
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
    
    void renderTitleScreenUi(GameButton* levelEditorButton, GameButton* spriteTesterButton);
    
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
    
    void renderLevelEditor(GameScreenLevelEditor* gameScreenLevelEditor);
    
    void renderSpriteTester(GameScreenSpriteTester* gameScreenSpriteTester);
    
    void renderLoading();
    
    void renderToSecondFramebufferWithShockwave(float centerX, float centerY, float timeElapsed, bool isTransforming);
    
    void renderToSecondFramebuffer(Game& game);
    
    void renderToThirdFramebufferWithObfuscation();
    
    void renderToScreenWithTransDeathIn(float timeElapsed);
    
    void renderToScreenWithTransDeathOut(float timeElapsed);
    
    void renderToScreenTransition(float progress);
    
    void renderToScreenFadeTransition(float progress);
    
    void renderToScreenPointTransition(float centerX, float centerY, float progress);
    
    void renderToScreenWithRadialBlur();
    
    void renderToScreen();

    void cleanUp();
    
    std::vector<TextureWrapper *>& getTextureWrappers();
    
    Rectangle& getCameraBounds();
    
    Vector2D& getCameraPosition();
    
protected:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    std::unique_ptr<RectangleBatcher> m_boundsRectangleBatcher;
    std::unique_ptr<RectangleBatcher> m_highlightRectangleBatcher;
    std::unique_ptr<LineBatcher> m_lineBatcher;
    std::unique_ptr<Font> m_font;
    
    TextureWrapper m_jon;
    TextureWrapper m_level_editor;
    TextureWrapper m_misc;
    TextureWrapper m_title_screen;
    TextureWrapper m_trans_death_shader_helper;
    TextureWrapper m_vampire;
    TextureWrapper m_world_1_background_lower_part_1;
    TextureWrapper m_world_1_background_lower_part_2;
    TextureWrapper m_world_1_background_mid_part_1;
    TextureWrapper m_world_1_background_mid_part_2;
    TextureWrapper m_world_1_background_upper_part_1;
    TextureWrapper m_world_1_background_upper_part_2;
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
    TextureWrapper m_world_1_objects;
    TextureWrapper m_world_1_special;
    TextureWrapper m_world_map_screen_part_1;
    TextureWrapper m_world_map_screen_part_2;
    TextureWrapper m_sprite_tester;
    
    std::vector<TextureWrapper *> m_textureWrappers;
    std::vector<void (Renderer::*)()> m_pendingLoadFunctions;
    std::vector<GpuTextureWrapper> m_framebuffers;
    
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

	int m_iFramebufferIndex;
    
    virtual void loadShaderPrograms() = 0;
    
    virtual void addFramebuffers() = 0;
    
    virtual GpuTextureDataWrapper* loadTextureData(const char* textureName) = 0;
    
    virtual GpuTextureWrapper* loadTexture(GpuTextureDataWrapper* textureData, int repeatS = 0) = 0;
    
    virtual void updateMatrix(float left, float right, float bottom, float top) = 0;
    
    virtual void bindToOffscreenFramebuffer(int index = 0) = 0;
    
    virtual void clearFramebufferWithColor(float r, float g, float b, float a) = 0;
    
    virtual void bindToScreenFramebuffer() = 0;

    virtual void destroyTexture(GpuTextureWrapper& textureWrapper) = 0;

	virtual void renderPhysicalEntity(PhysicalEntity &go, TextureRegion& tr, bool ignoreCamBounds = false);

	virtual void renderPhysicalEntityWithColor(PhysicalEntity &go, TextureRegion& tr, Color c, bool ignoreCamBounds = false);
    
private:
    std::vector<std::thread> m_threads;
	std::unique_ptr<Rectangle> m_camBounds;
    std::unique_ptr<Vector2D> m_camPosAcceleration;
    std::unique_ptr<Vector2D> m_camPosVelocity;
    float m_fStateTime;
	float m_fCamPosX;
    int m_iNumAsyncLoads;
    float m_fRadialBlurDirection;
    bool m_compressed;
    bool m_desktop;
    bool m_areShadersLoaded;
    bool m_stopCamera;
    bool m_hasCompletedRadialBlur;
    
    template<typename T>
    void renderPhysicalEntities(std::vector<T*>& items, bool ignoreCamBounds = false)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            T* pItem = *i;
            T& item = *pItem;
            renderPhysicalEntity(item, ASSETS->get(pItem), ignoreCamBounds);
        }
    }
    
    template<typename T>
    void renderPhysicalEntitiesWithColor(std::vector<T*>& items, bool ignoreCamBounds = false)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            T* pItem = *i;
            T& item = *pItem;
            renderPhysicalEntityWithColor(item, ASSETS->get(pItem), item.getColor(), ignoreCamBounds);
        }
    }
    
    template<typename T>
    void renderBoundsForPhysicalEntities(RectangleBatcher& rectangleBatcher, std::vector<T*>& items)
    {
        static Color red = Color(1, 0, 0, 1);
        
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            for (typename std::vector<std::unique_ptr<Rectangle>>::iterator j = (*i)->getBounds().begin(); j != (*i)->getBounds().end(); j++)
            {
                rectangleBatcher.beginBatch();
                renderBoundsWithColor(*(*j), red);
                rectangleBatcher.endBatch();
            }
        }
    }
    
    void renderBoundsForPhysicalEntity(PhysicalEntity &go);
    
    void renderBoundsWithColor(Rectangle &r, Color& c);
    
    void renderHighlightForPhysicalEntity(PhysicalEntity &go, Color& c);
    
    float getCamPosFarRight(Game& game);
    
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
    
    void loadSpriteTester();
    void loadSpriteTesterTextures();
    bool ensureSpriteTesterTextures();
    
    void loadWorld1Cutscene1();
    void loadWorld1Cutscene2();
    void loadWorld1CutsceneTextures();
    bool ensureWorld1CutsceneTextures();
    
    void loadJon();
    void loadTransDeath();
    void loadVampire();
    void loadJonTextures();
    bool ensureJonTextures();
    
    void loadWorld1BackgroundLowerPart1();
    void loadWorld1BackgroundLowerPart2();
    void loadWorld1BackgroundMidPart1();
    void loadWorld1BackgroundMidPart2();
    void loadWorld1BackgroundUpperPart1();
    void loadWorld1BackgroundUpperPart2();
    void loadWorld1Enemies();
    void loadWorld1Ground();
    void loadWorld1Objects();
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
    
    void loadWorld2Textures();
    void loadWorld2MidBossTextures();
    void loadWorld2EndBossTextures();
    
    void loadWorld3Textures();
    void loadWorld3MidBossTextures();
    void loadWorld3EndBossTextures();
    
    void loadWorld4Textures();
    void loadWorld4MidBossTextures();
    void loadWorld4EndBossTextures();
    
    void loadWorld5Textures();
    void loadWorld5MidBossTextures();
    void loadWorld5EndBossTextures();
    
    void unloadTitleTextures();
    void unloadWorldMapTextures();
    void unloadLevelEditorTextures();
    void unloadSpriteTesterTextures();
    
    void unloadJonTextures();
    
    void unloadWorld1CutsceneTextures();
    
    void unloadWorld1Textures();
    void unloadWorld1MidBossTextures();
    void unloadWorld1EndBossTextures();
    
    void unloadWorld2Textures();
    void unloadWorld2MidBossTextures();
    void unloadWorld2EndBossTextures();
    
    void unloadWorld3Textures();
    void unloadWorld3MidBossTextures();
    void unloadWorld3EndBossTextures();
    
    void unloadWorld4Textures();
    void unloadWorld4MidBossTextures();
    void unloadWorld4EndBossTextures();
    
    void unloadWorld5Textures();
    void unloadWorld5MidBossTextures();
    void unloadWorld5EndBossTextures();
    
    void handleAsyncTextureLoads();
    
    void cleanUpTextures();
    
    void destroyTexture(TextureWrapper* textureWrapper);

	bool isQueueEmpty();
};

#endif /* defined(__nosfuratu__Renderer__) */
