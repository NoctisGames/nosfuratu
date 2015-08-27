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

extern "C"
{
#include "asset_utils.h"
#include "platform_gl.h"
}

OpenGLESRenderer::OpenGLESRenderer() : Renderer()
{
    m_spriteBatcher = std::unique_ptr<OpenGLESSpriteBatcher>(new OpenGLESSpriteBatcher());
    
    m_backgroundTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("background_texture.png")));
}

void OpenGLESRenderer::clearScreenWithColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLESRenderer::beginFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->fbo);
    
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLESRenderer::endFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->m_screenFBO);
    
    m_spriteBatcher->beginBatch();
    
    static PhysicalEntity go = PhysicalEntity(GAME_WIDTH / 2, GAME_HEIGHT / 2, GAME_WIDTH, GAME_HEIGHT, 0);
    
    static TextureRegion screenTr = TextureRegion(0, 0, 1, 1, 1, 1);
    renderPhysicalEntity(go, screenTr);
    
    TextureWrapper tw = TextureWrapper(OGLESManager->fbo_texture);
    m_spriteBatcher->endBatch(tw, *OGLESManager->m_textureVertFlipProgram);
    
    glDisable(GL_BLEND);
    
    glDisable(GL_TEXTURE_2D);
}

void OpenGLESRenderer::cleanUp()
{
    // TODO
}