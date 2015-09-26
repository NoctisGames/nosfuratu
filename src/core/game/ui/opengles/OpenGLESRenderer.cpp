//
//  OpenGLESRenderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESRenderer.h"
#include "OpenGLESSpriteBatcher.h"
#include "TextureWrapper.h"
#include "OpenGLESManager.h"
#include "GameConstants.h"
#include "Assets.h"
#include "PhysicalEntity.h"
#include "TextureRegion.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Game.h"
#include "Jon.h"

extern "C"
{
#include "asset_utils.h"
#include "platform_gl.h"
}

OpenGLESRenderer::OpenGLESRenderer() : Renderer()
{
    m_spriteBatcher = std::unique_ptr<OpenGLESSpriteBatcher>(new OpenGLESSpriteBatcher());
    
    m_jon = std::unique_ptr<TextureWrapper>(loadTexture("jon"));
    m_vampire = std::unique_ptr<TextureWrapper>(loadTexture("vampire"));
    m_world_1_background = std::unique_ptr<TextureWrapper>(loadTexture("world_1_background"));
    m_world_1_foreground_more = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground_more"));
    m_world_1_foreground = std::unique_ptr<TextureWrapper>(loadTexture("world_1_foreground"));
    m_world_1_midground = std::unique_ptr<TextureWrapper>(loadTexture("world_1_midground"));
    
    m_framebuffer = std::unique_ptr<TextureWrapper>(new TextureWrapper(OGLESManager->fbo_texture));
}

void OpenGLESRenderer::updateMatrix(float left, float right, float bottom, float top)
{
    OGLESManager->createMatrix(left, right, bottom, top);
}

void OpenGLESRenderer::bindToOffscreenFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->fbo);
}

void OpenGLESRenderer::beginFrame()
{
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLESRenderer::clearFrameBufferWithColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLESRenderer::bindToScreenFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->m_screenFBO);
}

void OpenGLESRenderer::endFrame()
{
    glDisable(GL_BLEND);
    
    glDisable(GL_TEXTURE_2D);
}

GpuProgramWrapper& OpenGLESRenderer::getFramebufferToScreenGpuProgramWrapper()
{
    return *OGLESManager->m_fbToScreenProgram;
}

void OpenGLESRenderer::cleanUp()
{
    glDeleteTextures(1, &m_jon->texture);
    glDeleteTextures(1, &m_vampire->texture);
    glDeleteTextures(1, &m_world_1_background->texture);
    glDeleteTextures(1, &m_world_1_foreground_more->texture);
    glDeleteTextures(1, &m_world_1_foreground->texture);
    glDeleteTextures(1, &m_world_1_midground->texture);
}

TextureWrapper* OpenGLESRenderer::loadTexture(const char* textureName)
{
    size_t len = strlen(textureName);
    
    char* textureFileName = new char[len+5];
    
    strcpy(textureFileName, textureName);
    textureFileName[len] = '.';
    textureFileName[len+1] = 'p';
    textureFileName[len+2] = 'n';
    textureFileName[len+3] = 'g';
    textureFileName[len+4] = '\0';
    
    TextureWrapper* tw = new TextureWrapper(load_png_asset_into_texture(textureFileName));
    
    delete textureFileName;
    
    return tw;
}