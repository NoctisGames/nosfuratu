//
//  Renderer.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Renderer__
#define __nosfuratu__Renderer__

#include <memory>

class SpriteBatcher;
struct TextureWrapper;
class PhysicalEntity;
class TextureRegion;
class Game;
class Jon;
class GpuProgramWrapper;
class Vector2D;

class Renderer
{
public:
    Renderer();
    
    void render(Game& game, float deltaTime);
    
    void init();

	virtual void cleanUp() = 0;
    
protected:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    
    std::unique_ptr<TextureWrapper> m_jon_ability;
    std::unique_ptr<TextureWrapper> m_jon;
    std::unique_ptr<TextureWrapper> m_misc;
    std::unique_ptr<TextureWrapper> m_vampire;
    std::unique_ptr<TextureWrapper> m_world_1_background;
    std::unique_ptr<TextureWrapper> m_world_1_foreground_more;
    std::unique_ptr<TextureWrapper> m_world_1_foreground;
    std::unique_ptr<TextureWrapper> m_world_1_midground;
    std::unique_ptr<TextureWrapper> m_framebuffer;
    
    virtual TextureWrapper* loadTexture(const char* textureName) = 0;
    
    virtual void updateMatrix(float left, float right, float bottom, float top) = 0;
    
    virtual void bindToOffscreenFramebuffer() = 0;
    
    virtual void beginFrame() = 0;
    
    virtual void clearFrameBufferWithColor(float r, float g, float b, float a) = 0;
    
    virtual void bindToScreenFramebuffer() = 0;
    
    virtual void endFrame() = 0;
    
    virtual GpuProgramWrapper& getFramebufferToScreenGpuProgramWrapper() = 0;
    
private:
    std::unique_ptr<Vector2D> m_camPos;
    bool m_areTexturesLoaded;
    
    void renderPhysicalEntity(PhysicalEntity &go, TextureRegion tr);
    
    void updateCameraToFollowJon(Jon& jon, Game& game, float deltaTime);
};

#endif /* defined(__nosfuratu__Renderer__) */