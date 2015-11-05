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
class SinWaveTextureGpuProgramWrapper;
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
    
	virtual void init(RendererType type);
    
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
    
    void renderWorld(Game& game);
    
    void renderJon(Game& game);
    
    void renderBounds(Game& game);
    
    void renderEntityHighlighted(PhysicalEntity& entity, Color& c);
    
    void renderHud(Game& game, BackButton& backButton);
    
    void renderLevelEditor(LevelEditorActionsPanel& leap, LevelEditorEntitiesPanel& leep, TrashCan& tc, LevelSelectorPanel& lsp);
    
    void renderToScreen();

    void cleanUp();
    
    Vector2D& getCameraPosition();
    
protected:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    std::unique_ptr<RectangleBatcher> m_boundsRectangleBatcher;
    std::unique_ptr<RectangleBatcher> m_highlightRectangleBatcher;
    std::unique_ptr<Font> m_font;
    
    std::unique_ptr<TextureWrapper> m_title_font;
    std::unique_ptr<TextureWrapper> m_jon;
    std::unique_ptr<TextureWrapper> m_vampire;
    std::unique_ptr<TextureWrapper> m_jon_ability;
    std::unique_ptr<TextureWrapper> m_world_1_background;
    std::unique_ptr<TextureWrapper> m_world_1_foreground_more;
    std::unique_ptr<TextureWrapper> m_world_1_foreground;
    std::unique_ptr<TextureWrapper> m_level_editor;
    std::unique_ptr<TextureWrapper> m_framebuffer;
    
    std::unique_ptr<SinWaveTextureGpuProgramWrapper> m_sinWaveTextureProgram;
    
    virtual TextureWrapper* loadTexture(const char* textureName) = 0;
    
    virtual void updateMatrix(float left, float right, float bottom, float top) = 0;
    
    virtual void bindToOffscreenFramebuffer() = 0;
    
    virtual void beginFrame() = 0;
    
    virtual void clearFrameBufferWithColor(float r, float g, float b, float a) = 0;
    
    virtual void bindToScreenFramebuffer() = 0;
    
    virtual void endFrame() = 0;
    
    virtual GpuProgramWrapper& getFramebufferToScreenGpuProgramWrapper() = 0;
    
    virtual void destroyTexture(TextureWrapper& textureWrapper) = 0;
    
private:
    std::unique_ptr<Vector2D> m_camPos;
    float m_fCamWidth;
    float m_fCamHeight;
    std::unique_ptr<Vector2D> m_camPosAcceleration;
    std::unique_ptr<Vector2D> m_camPosVelocity;
    std::unique_ptr<Vector2D> m_camAcceleration;
    std::unique_ptr<Vector2D> m_camVelocity;
    float m_fOriginalY;
    float m_fOriginalWidth;
    float m_fOriginalHeight;
    float m_fStateTime;
    RendererType m_rendererType;
    bool m_areTitleTexturesLoaded;
    bool m_areJonTexturesLoaded;
    bool m_areVampireAndAbilityTexturesLoaded;
    bool m_areWorld1TexturesLoaded;
    bool m_areLevelEditorTexturesLoaded;
    
    template<typename T>
    void renderPhysicalEntities(std::vector<std::unique_ptr<T>>& items)
    {
        for (typename std::vector<std::unique_ptr<T>>::iterator i = items.begin(); i != items.end(); i++)
        {
            std::unique_ptr<T>& upItem = *i;
            T* pItem = upItem.get();
            T& item = *pItem;
            renderPhysicalEntity(item, Assets::get(item));
        }
    }
    
    template<typename T>
    void renderPhysicalEntitiesWithColor(std::vector<std::unique_ptr<T>>& items)
    {
        for (typename std::vector<std::unique_ptr<T>>::iterator i = items.begin(); i != items.end(); i++)
        {
            std::unique_ptr<T>& upItem = *i;
            T* pItem = upItem.get();
            T& item = *pItem;
            renderPhysicalEntityWithColor(item, Assets::get(item), item.getColor());
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
    
    void renderPhysicalEntity(PhysicalEntity &go, TextureRegion& tr);
    
    void renderPhysicalEntityWithColor(PhysicalEntity &go, TextureRegion& tr, Color c);
    
    void renderBoundsForPhysicalEntity(PhysicalEntity &go);
    
    void renderHighlightForPhysicalEntity(PhysicalEntity &go, Color& c);
    
    float getCamPosFarRight(Game& game);
};

#endif /* defined(__nosfuratu__Renderer__) */