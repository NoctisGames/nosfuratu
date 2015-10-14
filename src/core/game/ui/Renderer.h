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

struct Color;

class Renderer
{
public:
    Renderer();
    
	void init(RendererType type);
    
    void updateCameraToFollowJon(Jon& jon, Game& game, float deltaTime);
    
    void moveCamera(float x);
    
    void zoomOut();
    
    void zoomIn();
    
    void renderWorld(Game& game);
    
    void renderJon(Game& game);
    
    void renderBounds(Game& game);
    
    void renderEntityHighlighted(PhysicalEntity& entity, Color& c);
    
    void renderBackButton(BackButton& backButton);
    
    void renderLevelEditor(LevelEditorActionsPanel& levelEditorActionsPanel, LevelEditorEntitiesPanel& levelEditorEntitiesPanel, TrashCan& trashCan);
    
    void renderToScreen(Game& game);

    void cleanUp();
    
    Vector2D& getCameraPosition();
    
protected:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    std::unique_ptr<RectangleBatcher> m_boundsRectangleBatcher;
    std::unique_ptr<RectangleBatcher> m_highlightRectangleBatcher;
    
    std::unique_ptr<TextureWrapper> m_jon_ability;
    std::unique_ptr<TextureWrapper> m_jon;
    std::unique_ptr<TextureWrapper> m_level_editor;
    std::unique_ptr<TextureWrapper> m_vampire;
    std::unique_ptr<TextureWrapper> m_world_1_background;
    std::unique_ptr<TextureWrapper> m_world_1_foreground_more;
    std::unique_ptr<TextureWrapper> m_world_1_foreground;
    std::unique_ptr<TextureWrapper> m_framebuffer;
    
    std::unique_ptr<SinWaveTextureGpuProgramWrapper> m_sinWaveTextureProgram;

	bool m_areLevelEditorTexturesLoaded;
    bool m_areWorld1TexturesLoaded;
    bool m_areJonTexturesLoaded;
    
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
};

#endif /* defined(__nosfuratu__Renderer__) */