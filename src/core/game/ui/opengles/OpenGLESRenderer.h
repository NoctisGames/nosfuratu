//
//  OpenGLESRenderer.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__OpenGLESRenderer__
#define __nosfuratu__OpenGLESRenderer__

#include "Renderer.h"

class OpenGLESRenderer : public Renderer
{
public:
    OpenGLESRenderer();
    
    virtual bool isLoaded();
    
    virtual void beginFrame();
    
    virtual void endFrame();

protected:
    virtual void loadShaders();
    
    virtual void addFramebuffers();
    
    virtual TextureWrapper* loadTexture(const char* textureName, int repeatS);
    
    virtual void updateMatrix(float left, float right, float bottom, float top);
    
    virtual void bindToOffscreenFramebuffer(int index);
    
    virtual void clearFrameBufferWithColor(float r, float g, float b, float a);
    
    virtual void bindToScreenFramebuffer();
    
    virtual void destroyTexture(TextureWrapper& textureWrapper);
};

#endif /* defined(__nosfuratu__OpenGLESRenderer__) */