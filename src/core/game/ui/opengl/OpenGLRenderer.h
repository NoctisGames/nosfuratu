//
//  OpenGLRenderer.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLRenderer__
#define __nosfuratu__OpenGLRenderer__

#include "Renderer.h"

class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer();
    
    virtual bool isLoaded();
    
    virtual void beginFrame(float deltaTime);
    
    virtual void endFrame();

protected:
    virtual void loadShaderPrograms();
    
    virtual void addFramebuffers();
    
    virtual GpuTextureDataWrapper* loadTextureData(const char* textureName);
    
    virtual GpuTextureWrapper* loadTexture(GpuTextureDataWrapper* textureData, int repeatS);
    
    virtual void updateMatrix(float left, float right, float bottom, float top);
    
    virtual void bindToOffscreenFramebuffer(int index);
    
    virtual void clearFramebufferWithColor(float r, float g, float b, float a);
    
    virtual void bindToScreenFramebuffer();
    
    virtual void destroyTexture(GpuTextureWrapper& textureWrapper);
};

#endif /* defined(__nosfuratu__OpenGLRenderer__) */
