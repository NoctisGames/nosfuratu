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
class GameObject;
class TextureRegion;

class Renderer
{
public:
    Renderer();
    
    virtual void clearScreenWithColor(float r, float g, float b, float a) = 0;
    
    virtual void beginFrame() = 0;
    
    virtual void endFrame() = 0;

	virtual void cleanUp() = 0;
    
    void renderBackground();
    
protected:
    std::unique_ptr<SpriteBatcher> m_spriteBatcher;
    
    std::unique_ptr<TextureWrapper> m_backgroundTexture;
    
private:
	void renderGameObject(GameObject &go, TextureRegion tr);
};

#endif /* defined(__nosfuratu__Renderer__) */