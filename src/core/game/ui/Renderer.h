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

#include <memory>
#include <vector>

class SpriteBatcher;
class LineBatcher;
class Font;
struct TextureWrapper;
class PhysicalEntity;
class TextureRegion;
class Game;
class Jon;
class GpuProgramWrapper;
class TransitionGpuProgramWrapper;
class SinWaveTextureGpuProgramWrapper;
class SnakeDeathTextureGpuProgramWrapper;
class ShockwaveTextureGpuProgramWrapper;
class TransDeathGpuProgramWrapper;
class FramebufferRadialBlurGpuProgramWrapper;
class Vector2D;
class BackButton;
class LevelEditorButton;
class LevelEditorActionsPanel;
class LevelEditorEntitiesPanel;
class TrashCan;
class LevelSelectorPanel;
class TitlePanel;
class WorldMapPanel;

struct Color;

class Renderer
{
public:
    Renderer();
    
    ~Renderer();
    
    void init(RendererType rendererType);

	virtual bool isLoaded() = 0;
    
    virtual void beginFrame();
    
    virtual void endFrame() = 0;
    
    void setFramebuffer(int framebufferIndex);
    
    void beginOpeningPanningSequence(Game& game);
    
    // Return 3 if the beginning of the stage has been reached
    // Return 2 if Jon should begin warming up
    // Return 1 to activate the radial blur effect
    // Return 0 to do nothing
    int updateCameraToFollowPathToJon(Game& game, float deltaTime);
    
    void updateCameraToFollowJon(Game& game, float deltaTime);
    
    void moveCamera(float x);
    
    void zoomOut();
    
    void zoomIn();
    
    void renderTitleScreenBackground(TitlePanel* panel);
    
    void renderTitleScreenUi(LevelEditorButton* levelEditorButton);
    
    void renderTitleScreenLoading();
    
    void renderWorldMapScreenBackground(WorldMapPanel* panel);
    
    void renderWorldMapScreenUi(BackButton* backButton);
    
    void renderWorldMapScreenLoading();
    
    void renderWorld(Game& game);
    
    void renderJonAndExtraForegroundObjects(Game& game);
    
    void renderBounds(Game& game, int boundsLevelRequested);
    
    void renderEntityHighlighted(PhysicalEntity& entity, Color& c);
    
    void renderHud(Game& game, BackButton* backButton, int fps);
    
    void renderLevelEditor(LevelEditorActionsPanel* leap, LevelEditorEntitiesPanel* leep, TrashCan* tc, LevelSelectorPanel* lsp);
    
    void renderToSecondFramebufferWithShockwave(float centerX, float centerY, float timeElapsed, bool isTransforming);
    
    void renderToSecondFramebuffer(Game& game);
    
    void renderToScreenWithTransDeathIn(float timeElapsed);
    
    void renderToScreenWithTransDeathOut(float timeElapsed);
    
    void renderToScreenTransition(float progress);
    
    void renderToScreenWithRadialBlur();
    
    void renderToScreen();

    void cleanUp();
    
    Rectangle& getCameraBounds();
    
    Vector2D& getCameraPosition();
    
protected:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    std::unique_ptr<RectangleBatcher> m_boundsRectangleBatcher;
    std::unique_ptr<RectangleBatcher> m_highlightRectangleBatcher;
    std::unique_ptr<LineBatcher> m_lineBatcher;
    std::unique_ptr<Font> m_font;
    
    TextureWrapper* m_jon;
    TextureWrapper* m_level_editor;
    TextureWrapper* m_title_screen;
    TextureWrapper* m_trans_death_shader_helper;
    TextureWrapper* m_vampire;
    TextureWrapper* m_world_1_background_lower;
    TextureWrapper* m_world_1_background_mid;
    TextureWrapper* m_world_1_background_upper;
    TextureWrapper* m_world_1_enemies;
    TextureWrapper* m_world_1_ground;
    TextureWrapper* m_world_1_mid_boss_part_1;
    TextureWrapper* m_world_1_mid_boss_part_2;
    TextureWrapper* m_world_1_mid_boss_part_3;
    TextureWrapper* m_world_1_mid_boss_part_4;
    TextureWrapper* m_world_1_objects;
    TextureWrapper* m_world_1_special;
    TextureWrapper* m_world_map_screen;
    
    std::vector<TextureWrapper> m_framebuffers;
    
    TransitionGpuProgramWrapper* m_transScreenGpuProgramWrapper;
    SinWaveTextureGpuProgramWrapper* m_sinWaveTextureProgram;
	GpuProgramWrapper* m_backgroundTextureWrapper;
    SnakeDeathTextureGpuProgramWrapper* m_snakeDeathTextureProgram;
    ShockwaveTextureGpuProgramWrapper* m_shockwaveTextureGpuProgramWrapper;
    TransDeathGpuProgramWrapper* m_transDeathInGpuProgramWrapper;
    TransDeathGpuProgramWrapper* m_transDeathOutGpuProgramWrapper;
    GpuProgramWrapper* m_framebufferToScreenGpuProgramWrapper;
    GpuProgramWrapper* m_framebufferTintGpuProgramWrapper;
    FramebufferRadialBlurGpuProgramWrapper* m_framebufferRadialBlurGpuProgramWrapper;

	int m_iFramebufferIndex;
    
    virtual void loadShaderPrograms() = 0;
    
    virtual void addFramebuffers() = 0;
    
    virtual TextureWrapper* loadTexture(const char* textureName, int repeatS = 0) = 0;
    
    virtual void updateMatrix(float left, float right, float bottom, float top) = 0;
    
    virtual void bindToOffscreenFramebuffer(int index = 0) = 0;
    
    virtual void clearFramebufferWithColor(float r, float g, float b, float a) = 0;
    
    virtual void bindToScreenFramebuffer() = 0;

    virtual void destroyTexture(TextureWrapper& textureWrapper) = 0;
    
private:
	std::unique_ptr<Rectangle> m_camBounds;
    std::unique_ptr<Vector2D> m_camPosAcceleration;
    std::unique_ptr<Vector2D> m_camPosVelocity;
    float m_fStateTime;
    int m_iRadialBlurDirection;
    bool m_compressed;
    
    bool m_areTitleTexturesLoaded;
    bool m_areWorldMapTexturesLoaded;
    bool m_areLevelEditorTexturesLoaded;
    bool m_areWorld1TexturesLoaded;
    bool m_areWorld1MidBossTexturesLoaded;
    bool m_areWorld1EndBossTexturesLoaded;
    bool m_areWorld2TexturesLoaded;
    bool m_areWorld2MidBossTexturesLoaded;
    bool m_areWorld2EndBossTexturesLoaded;
    bool m_areWorld3TexturesLoaded;
    bool m_areWorld3MidBossTexturesLoaded;
    bool m_areWorld3EndBossTexturesLoaded;
    bool m_areWorld4TexturesLoaded;
    bool m_areWorld4MidBossTexturesLoaded;
    bool m_areWorld4EndBossTexturesLoaded;
    bool m_areWorld5TexturesLoaded;
    bool m_areWorld5MidBossTexturesLoaded;
    bool m_areWorld5EndBossTexturesLoaded;
    
    bool m_areShadersLoaded;
    
    template<typename T>
    void renderPhysicalEntities(std::vector<T*>& items)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            T* pItem = *i;
            T& item = *pItem;
            renderPhysicalEntity(item, Assets::getInstance()->get(pItem));
        }
    }
    
    template<typename T>
    void renderPhysicalEntitiesWithColor(std::vector<T*>& items)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            T* pItem = *i;
            T& item = *pItem;
            renderPhysicalEntityWithColor(item, Assets::getInstance()->get(pItem), item.getColor());
        }
    }
    
    template<typename T>
    void renderBoundsForPhysicalEntities(RectangleBatcher& rectangleBatcher, std::vector<T*>& items)
    {
        static Color red = Color(1, 0, 0, 1);
        
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            for (typename std::vector<Rectangle>::iterator j = (*i)->getBounds().begin(); j != (*i)->getBounds().end(); j++)
            {
                rectangleBatcher.beginBatch();
                renderBoundsWithColor((*j), red);
                rectangleBatcher.endBatch();
            }
        }
    }
    
    void renderPhysicalEntity(PhysicalEntity &go, TextureRegion& tr);
    
    void renderPhysicalEntityWithColor(PhysicalEntity &go, TextureRegion& tr, Color c);
    
    void renderBoundsForPhysicalEntity(PhysicalEntity &go);
    
    void renderBoundsWithColor(Rectangle &r, Color& c);
    
    void renderHighlightForPhysicalEntity(PhysicalEntity &go, Color& c);
    
    float getCamPosFarRight(Game& game);
    
    Font* loadFont();
    
    void loadTitle();
    void loadWorldMap();
    void loadLevelEditor();
    
    void loadWorld1();
    void loadWorld1MidBoss();
    void loadWorld1EndBoss();
    
    void loadWorld2();
    void loadWorld2MidBoss();
    void loadWorld2EndBoss();
    
    void loadWorld3();
    void loadWorld3MidBoss();
    void loadWorld3EndBoss();
    
    void loadWorld4();
    void loadWorld4MidBoss();
    void loadWorld4EndBoss();
    
    void loadWorld5();
    void loadWorld5MidBoss();
    void loadWorld5EndBoss();
    
    void cleanUpTextures();
};

#endif /* defined(__nosfuratu__Renderer__) */