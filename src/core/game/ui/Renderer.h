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

class Renderer
{
public:
    Renderer();
    
    void render(Game& game);

	virtual void cleanUp() = 0;
    
protected:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    
    std::unique_ptr<TextureWrapper> m_background;
    std::unique_ptr<TextureWrapper> m_jon;
    std::unique_ptr<TextureWrapper> m_framebuffer;
    
    virtual void updateMatrix(float left, float right, float bottom, float top) = 0;
    
    virtual void bindToOffscreenFramebuffer() = 0;
    
    virtual void beginFrame() = 0;
    
    virtual void clearFrameBufferWithColor(float r, float g, float b, float a) = 0;
    
    virtual void bindToScreenFramebuffer() = 0;
    
    virtual void endFrame() = 0;
    
    virtual GpuProgramWrapper& getFramebufferToScreenGpuProgramWrapper() = 0;
    
private:
    void renderPhysicalEntity(PhysicalEntity &go, TextureRegion tr);
    
    void updateCameraToFollowJon(Jon& jon, bool isBackground);
};

#endif /* defined(__nosfuratu__Renderer__) */