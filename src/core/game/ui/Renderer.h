//
//  Renderer.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Renderer__
#define __nosfuratu__Renderer__

#include "Assets.h"
#include "RendererType.h"
#include "Font.h"

#include <memory>
#include <vector>

class SpriteBatcher;
class RectangleBatcher;
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
class Vector2D;
class BackButton;
class LevelEditorActionsPanel;
class LevelEditorEntitiesPanel;
class TrashCan;
class LevelSelectorPanel;

struct Color;

class Renderer
{
public:
    Renderer();
    
    void init(RendererType type);

	virtual bool isLoaded() = 0;
    
    virtual void beginFrame();
    
    virtual void endFrame() = 0;
    
    void setFramebuffer(int framebufferIndex);
    
    void reinit();
    
    void beginOpeningPanningSequence(Game& game);
    
    // Return true if the beginning of the stage has been reached
    bool updateCameraToFollowPathToJon(Game& game, float deltaTime);
    
    void updateCameraToFollowJon(Game& game, float deltaTime);
    
    void moveCamera(float x);
    
    void zoomOut();
    
    void zoomIn();
    
    void renderTitleScreen();
    
    void renderLoadingTextOnTitleScreen();
    
    void renderWorldMapScreenBackground();
    
    void renderWorldMapScreenUi(BackButton& backButton);
    
    void renderLoadingTextOnWorldMapScreen();
    
    void renderWorld(Game& game);
    
    void renderJon(Game& game);
    
    void renderBounds(Game& game);
    
    void renderEntityHighlighted(PhysicalEntity& entity, Color& c);
    
    void renderHud(Game& game, BackButton& backButton);
    
    void renderLevelEditor(LevelEditorActionsPanel& leap, LevelEditorEntitiesPanel& leep, TrashCan& tc, LevelSelectorPanel& lsp);
    
    void renderToSecondFramebufferWithShockwave(float centerX, float centerY, float timeElapsed, bool isTransforming);
    
    void renderToSecondFramebuffer(Game& game);
    
    void renderToScreenWithTransDeathIn(float timeElapsed);
    
    void renderToScreenWithTransDeathOut(float timeElapsed);
    
    void renderToScreenTitleToWorldMapTransition(float progress);
    
    void renderToScreenWorldMapToLevelTransition(float progress);
    
    void renderToScreen();

    void cleanUp();
    
    Vector2D& getCameraPosition();
    
protected:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    std::unique_ptr<RectangleBatcher> m_boundsRectangleBatcher;
    std::unique_ptr<RectangleBatcher> m_highlightRectangleBatcher;
    std::unique_ptr<Font> m_font;
    
    TextureWrapper* m_jon_ability;
    TextureWrapper* m_jon_poses;
    TextureWrapper* m_jon;
    TextureWrapper* m_level_editor;
    TextureWrapper* m_title_font;
    TextureWrapper* m_vampire_poses;
    TextureWrapper* m_vampire_transform;
    TextureWrapper* m_vampire;
    TextureWrapper* m_world_1_background;
    TextureWrapper* m_world_1_cave;
    TextureWrapper* m_world_1_enemies;
    TextureWrapper* m_world_1_snake_cave;
    TextureWrapper* m_world_1_ground_w_cave;
    TextureWrapper* m_world_1_ground_wo_cave;
    TextureWrapper* m_world_1_misc;
    TextureWrapper* m_world_1_objects;
    TextureWrapper* m_game_objects;
    TextureWrapper* m_world_map;
    TextureWrapper* m_trans_death_shader_helper;
    
    std::vector<TextureWrapper> m_framebuffers;
    
    TransitionGpuProgramWrapper* m_transTitleToWorldMapGpuProgramWrapper;
    TransitionGpuProgramWrapper* m_transWorldMapToLevelGpuProgramWrapper;
    SinWaveTextureGpuProgramWrapper* m_sinWaveTextureProgram;
    SnakeDeathTextureGpuProgramWrapper* m_snakeDeathTextureProgram;
    ShockwaveTextureGpuProgramWrapper* m_shockwaveTextureGpuProgramWrapper;
    TransDeathGpuProgramWrapper* m_transDeathInGpuProgramWrapper;
    TransDeathGpuProgramWrapper* m_transDeathOutGpuProgramWrapper;
    GpuProgramWrapper* m_framebufferToScreenGpuProgramWrapper;
    GpuProgramWrapper* m_framebufferTintGpuProgramWrapper;
    
    virtual void loadShaders() = 0;
    
    virtual void addFramebuffers() = 0;
    
    virtual TextureWrapper* loadTexture(const char* textureName, int repeatS = 0) = 0;
    
    virtual void updateMatrix(float left, float right, float bottom, float top) = 0;
    
    virtual void bindToOffscreenFramebuffer(int index = 0) = 0;
    
    virtual void clearFrameBufferWithColor(float r, float g, float b, float a) = 0;
    
    virtual void bindToScreenFramebuffer() = 0;

    virtual void destroyTexture(TextureWrapper& textureWrapper) = 0;
    
private:
    std::unique_ptr<Rectangle> m_camBounds;
    std::unique_ptr<Vector2D> m_camPosAcceleration;
    std::unique_ptr<Vector2D> m_camPosVelocity;
    std::unique_ptr<Vector2D> m_camAcceleration;
    std::unique_ptr<Vector2D> m_camVelocity;
    float m_fOriginalY;
    float m_fOriginalWidth;
    float m_fOriginalHeight;
    float m_fStateTime;
    RendererType m_rendererType;
    int m_iFramebufferIndex;
    bool m_areTitleTexturesLoaded;
    bool m_areWorld1TexturesLoaded;
    bool m_areLevelEditorTexturesLoaded;
    bool m_areShadersLoaded;
    
    template<typename T>
    void renderPhysicalEntities(std::vector<std::unique_ptr<T>>& items, bool performBoundsCheck = false)
    {
        for (typename std::vector<std::unique_ptr<T>>::iterator i = items.begin(); i != items.end(); i++)
        {
            std::unique_ptr<T>& upItem = *i;
            T* pItem = upItem.get();
            T& item = *pItem;
            renderPhysicalEntity(item, Assets::getInstance()->get(item), performBoundsCheck);
        }
    }
    
    template<typename T>
    void renderPhysicalEntitiesWithColor(std::vector<std::unique_ptr<T>>& items, bool performBoundsCheck = false)
    {
        for (typename std::vector<std::unique_ptr<T>>::iterator i = items.begin(); i != items.end(); i++)
        {
            std::unique_ptr<T>& upItem = *i;
            T* pItem = upItem.get();
            T& item = *pItem;
            renderPhysicalEntityWithColor(item, Assets::getInstance()->get(item), item.getColor(), performBoundsCheck);
        }
    }
    
    template<typename T>
    void renderBoundsForPhysicalEntities(std::vector<std::unique_ptr<T>>& items)
    {
        for (typename std::vector<std::unique_ptr<T>>::iterator i = items.begin(); i != items.end(); i++)
        {
            std::unique_ptr<T>& upItem = *i;
            T* pItem = upItem.get();
            T& item = *pItem;
            renderBoundsForPhysicalEntity(item);
        }
    }
    
    void tearDownTexture(TextureWrapper* textureWrapper);
    
    void tearDownGpuProgramWrapper(GpuProgramWrapper* gpuProgramWrapper);
    
    void renderPhysicalEntity(PhysicalEntity &go, TextureRegion& tr, bool performBoundsCheck = false);
    
    void renderPhysicalEntityWithColor(PhysicalEntity &go, TextureRegion& tr, Color c, bool performBoundsCheck = false);
    
    void renderBoundsForPhysicalEntity(PhysicalEntity &go);
    
    void renderHighlightForPhysicalEntity(PhysicalEntity &go, Color& c);
    
    float getCamPosFarRight(Game& game);
};

#endif /* defined(__nosfuratu__Renderer__) */